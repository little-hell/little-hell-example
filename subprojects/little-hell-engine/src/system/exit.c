/*
 * Copyright(C) 1993-1996 Id Software, Inc.
 * Copyright(C) 2005-2014 Simon Howard
 * Copyright(C) 2023 Joshua Murphy
 *
 * This file is part of the little hell engine.
 *
 * Little Hell is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Little Hell is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Little Hell Engine.
 * If not, see <https://www.gnu.org/licenses/>.
 */

#include "littlehell/littlehell/system.h"

const char* ExitCodeStrings[EXIT_CODE_COUNT] = {
    "EXIT_SUCCESS",
    "EXIT_INVALID_INPUT",
    "EXIT_OUT_OF_MEMORY",
    "EXIT_FILE_NOT_FOUND",
    "EXIT_IO_ERROR",
    "EXIT_UNEXPECTED_ERROR"
};

typedef void (*on_exit_func_t)(void);

typedef struct on_exit_listentry_s on_exit_listentry_t;

struct on_exit_listentry_s
{
    on_exit_func_t func;
    boolean run_on_error;
    on_exit_listentry_t *next;
};

static on_exit_listentry_t *exit_funcs = NULL;

void system_on_exit(on_exit_func_t func, boolean run_on_error)
{
    on_exit_listentry_t *entry;

    entry = malloc(sizeof(*entry));

    entry->func = func;
    entry->run_on_error = run_on_error;
    entry->next = exit_funcs;
    exit_funcs = entry;
}


static boolean already_quitting = false;

void system_exit()
{
    on_exit_listentry_t *entry;
    boolean exit_gui_popup;

    if (already_quitting)
    {
        log_error("Warning: recursive call to system_exit() detected.\n");
        exit(-1);
    }
    else
    {
        already_quitting = true;
    }

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

    SDL_Quit();

    exit(-1);
}
