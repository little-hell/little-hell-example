//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>

#include "SDL.h"

#include "config.h"

#include "doomtype.h"
#include "m_argv.h"
#include "m_config.h"
#include "m_misc.h"
#include "i_joystick.h"
#include "i_sound.h"
#include "i_timer.h"
#include "i_video.h"

#include "i_system.h"

#include "w_wad.h"
#include "z_zone.h"

#define DEFAULT_RAM 16 /* MiB */
#define MIN_RAM     4  /* MiB */


typedef struct atexit_listentry_s atexit_listentry_t;

struct atexit_listentry_s
{
    atexit_func_t func;
    boolean run_on_error;
    atexit_listentry_t *next;
};

static atexit_listentry_t *exit_funcs = NULL;

void I_AtExit(atexit_func_t func, boolean run_on_error)
{
    atexit_listentry_t *entry;

    entry = malloc(sizeof(*entry));

    entry->func = func;
    entry->run_on_error = run_on_error;
    entry->next = exit_funcs;
    exit_funcs = entry;
}

// Tactile feedback function, probably used for the Logitech Cyberman

void I_Tactile(int on, int off, int total)
{
}

// Zone memory auto-allocation function that allocates the zone size
// by trying progressively smaller zone sizes until one is found that
// works.

static byte *AutoAllocMemory(int *size, int default_ram, int min_ram)
{
    byte *zonemem;

    // Allocate the zone memory.  This loop tries progressively smaller
    // zone sizes until a size is found that can be allocated.
    // If we used the -mb command line parameter, only the parameter
    // provided is accepted.

    zonemem = NULL;

    while (zonemem == NULL)
    {
        // We need a reasonable minimum amount of RAM to start.

        if (default_ram < min_ram)
        {
            I_Error("Unable to allocate %i MiB of RAM for zone", default_ram);
        }

        // Try to allocate the zone memory.

        *size = default_ram * 1024 * 1024;

        zonemem = malloc(*size);

        // Failed to allocate?  Reduce zone size until we reach a size
        // that is acceptable.

        if (zonemem == NULL)
        {
            default_ram -= 1;
        }
    }

    return zonemem;
}

byte *I_ZoneBase(int *size)
{
    byte *zonemem;
    int min_ram, default_ram;
    int p;

    //!
    // @category obscure
    // @arg <mb>
    //
    // Specify the heap size, in MiB (default 16).
    //

    p = M_CheckParmWithArgs("-mb", 1);

    if (p > 0)
    {
        default_ram = atoi(myargv[p + 1]);
        min_ram = default_ram;
    }
    else
    {
        default_ram = DEFAULT_RAM;
        min_ram = MIN_RAM;
    }

    zonemem = AutoAllocMemory(size, default_ram, min_ram);

    printf("zone memory: %p, %x allocated for zone\n", zonemem, *size);

    return zonemem;
}

void I_PrintBanner(const char *msg)
{
    int i;
    int spaces = 35 - (strlen(msg) / 2);

    for (i = 0; i < spaces; ++i)
        putchar(' ');

    puts(msg);
}

void I_PrintDivider(void)
{
    int i;

    for (i = 0; i < 75; ++i)
    {
        putchar('=');
    }

    putchar('\n');
}

void I_PrintStartupBanner(const char *gamedescription)
{
    I_PrintDivider();
    I_PrintBanner(gamedescription);
    I_PrintDivider();

    printf(
        " " PACKAGE_NAME
        " is free software, covered by the GNU General Public\n"
        " License.  There is NO warranty; not even for MERCHANTABILITY or "
        "FITNESS\n"
        " FOR A PARTICULAR PURPOSE. You are welcome to change and distribute\n"
        " copies under certain conditions. See the source for more "
        "information.\n");

    I_PrintDivider();
}

//
// I_ConsoleStdout
//
// Returns true if stdout is a real console, false if it is a file
//

boolean I_ConsoleStdout(void)
{
    return isatty(fileno(stdout));
}

//
// I_Quit
//

void I_Quit(void)
{
    atexit_listentry_t *entry;

    // Run through all exit functions

    entry = exit_funcs;

    while (entry != NULL)
    {
        entry->func();
        entry = entry->next;
    }

    SDL_Quit();

    exit(0);
}


//
// I_Error
//

static boolean already_quitting = false;

void I_Error(const char *error, ...)
{
    char msgbuf[512];
    va_list argptr;
    atexit_listentry_t *entry;
    boolean exit_gui_popup;

    if (already_quitting)
    {
        fprintf(stderr, "Warning: recursive call to I_Error detected.\n");
        exit(-1);
    }
    else
    {
        already_quitting = true;
    }

    // Message first.
    va_start(argptr, error);
    //fprintf(stderr, "\nError: ");
    vfprintf(stderr, error, argptr);
    fprintf(stderr, "\n\n");
    va_end(argptr);
    fflush(stderr);

    // Write a copy of the message into buffer.
    va_start(argptr, error);
    memset(msgbuf, 0, sizeof(msgbuf));
    M_vsnprintf(msgbuf, sizeof(msgbuf), error, argptr);
    va_end(argptr);

    // Shutdown. Here might be other errors.

    entry = exit_funcs;

    while (entry != NULL)
    {
        if (entry->run_on_error)
        {
            entry->func();
        }

        entry = entry->next;
    }

    //!
    // @category obscure
    //
    // If specified, don't show a GUI window for error messages when the
    // game exits with an error.
    //
    exit_gui_popup = !M_ParmExists("-nogui");

    // Pop up a GUI dialog box to show the error message, if the
    // game was not run from the console (and the user will
    // therefore be unable to otherwise see the message).
    if (exit_gui_popup && !I_ConsoleStdout())
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, PACKAGE_STRING, msgbuf,
                                 NULL);
    }

    // abort();

    SDL_Quit();

    exit(-1);
}

//
// I_Realloc
//

void *I_Realloc(void *ptr, size_t size)
{
    void *new_ptr;

    new_ptr = realloc(ptr, size);

    if (size != 0 && new_ptr == NULL)
    {
        I_Error("I_Realloc: failed on reallocation of %zu bytes", size);
    }

    return new_ptr;
}
