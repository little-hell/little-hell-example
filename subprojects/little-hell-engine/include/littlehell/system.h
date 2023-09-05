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

#ifndef SYSTEM_H
#define SYSTEM_H

#include "d_ticcmd.h"
#include "d_event.h"

/** @defgroup System System
 *  @brief Cross-platform operating system functions and utilities.
 */

/**
 * Function callback upon exit
 */
typedef void (*on_exit_func_t)(void);

/**
 *  @ingroup ExitAPI
 *  @brief Exit function with callbacks
 *  @todo Document me!
 */
void system_exit();

/** @ingroup ExitAPI
 *  @brief Register a callback function to be called upon `system_exit()`
 *  @param func The callback function to execute
 *  @param run_on_error Whether this function should be executed if the system exists on an error.
 */
void system_on_exit(on_exit_func_t func, boolean run_on_error);

// Called by DoomMain.
void I_Init (void);

// Called by startup code
// to get the ammount of memory to malloc
// for the zone management.
byte*	I_ZoneBase (int *size);

boolean I_ConsoleStdout(void);


// Asynchronous interrupt functions should maintain private queues
// that are read by the synchronous functions
// to be converted into events.

// Either returns a null ticcmd,
// or calls a loadable driver to build it.
// This ticcmd will then be modified by the gameloop
// for normal input.
ticcmd_t* I_BaseTiccmd (void);

void I_Tactile (int on, int off, int total);

void *I_Realloc(void *ptr, size_t size);

boolean I_GetMemoryValue(unsigned int offset, void *value, int size);

// Add all system-specific config file variable bindings.

void I_BindVariables(void);

// Print startup banner copyright message.

void I_PrintStartupBanner(const char *gamedescription);

// Print a centered text banner displaying the given string.

void I_PrintBanner(const char *text);

// Print a dividing line for startup banners.

void I_PrintDivider(void);

#endif

