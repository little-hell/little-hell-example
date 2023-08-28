//
// Copyright(C) 2018 Simon Howard
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
//
// File globbing API. This allows the contents of the filesystem
// to be interrogated.
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "i_glob.h"
#include "m_misc.h"
#include "config.h"

#if defined(HAVE_DIRENT_H)
#include <dirent.h>
#include <sys/stat.h>
#else
#define NO_DIRENT_IMPLEMENTATION
#endif

#ifndef NO_DIRENT_IMPLEMENTATION

// Only the fields d_name and (as an XSI extension) d_ino are specified
// in POSIX.1.  Other than Linux, the d_type field is available mainly
// only on BSD systems.  The remaining fields are available on many, but
// not all systems.
struct glob_s
{
    char **globs;
    int num_globs;
    int flags;
    DIR *dir;
    char *directory;
    char *last_filename;
    // These fields are only used when the GLOB_FLAG_SORTED flag is set:
    char **filenames;
    int filenames_len;
    int next_index;
};

static void FreeStringList(char **globs, int num_globs)
{
    int i;
    for (i = 0; i < num_globs; ++i)
    {
        free(globs[i]);
    }
    free(globs);
}

glob_t *I_StartMultiGlob(const char *directory, int flags, const char *glob,
                         ...)
{
    char **globs;
    int num_globs;
    glob_t *result;
    va_list args;
    char *directory_native;

    globs = malloc(sizeof(char *));
    if (globs == NULL)
    {
        return NULL;
    }
    globs[0] = M_StringDuplicate(glob);
    num_globs = 1;

    va_start(args, glob);
    for (;;)
    {
        const char *arg = va_arg(args, const char *);
        char **new_globs;

        if (arg == NULL)
        {
            break;
        }

        new_globs = realloc(globs, sizeof(char *) * (num_globs + 1));
        if (new_globs == NULL)
        {
            FreeStringList(globs, num_globs);
        }
        globs = new_globs;
        globs[num_globs] = M_StringDuplicate(arg);
        ++num_globs;
    }
    va_end(args);

    result = malloc(sizeof(glob_t));
    if (result == NULL)
    {
        FreeStringList(globs, num_globs);
        return NULL;
    }

    directory_native = M_ConvertUtf8ToSysNativeMB(directory);

    result->dir = opendir(directory_native);
    if (result->dir == NULL)
    {
        FreeStringList(globs, num_globs);
        free(result);
        free(directory_native);
        return NULL;
    }

    result->directory = directory_native;
    result->globs = globs;
    result->num_globs = num_globs;
    result->flags = flags;
    result->last_filename = NULL;
    result->filenames = NULL;
    result->filenames_len = 0;
    result->next_index = -1;
    return result;
}

#endif /* #ifdef NO_DIRENT_IMPLEMENTATION */
