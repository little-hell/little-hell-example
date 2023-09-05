/*
 * Copyright(C) 2023 Joshua Murphy
 *
 * This file is part of Little Hell.
 *
 * Little Hell is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Little Hell is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with Little Hell.
 * If not, see https://www.gnu.org/licenses/
 */

#include "sleep.h"

#ifdef _WIN32
#include <windows.h>
/** @defgroup SleepInternals Sleep (Internals)
 *  @ingroup System
 *  @brief Internal documentation of Sleep for engine developers.
 *  @note If you aren't developing the engine itself, and just creating a game, this
 *  documentation isn't what you're looking for! Try the Drawable API documentation instead.
 */

/** @ingroup SleepInternals
 *  @brief Windows-specific sleep function
 *  @note This is an internal function, do not call it directly.
 *  @warning This is a platform-specific function. It is not cross-platform. Use the cross-platform wrapper system_sleep()
 *
 *  Windows provides a `Sleep()` function (not `sleep()` as with UNIX) that uses miliseconds
 *  instead of seconds.
 */
static void windows_sleep(unsigned int seconds) {
	// Sleep on Windows is measured in miliseconds.
    Sleep(seconds * 1000);
}
#endif

#ifdef !_WIN32
#include <unistd.h>

/** @ingroup SleepInternals
 *  @brief UNIX-specific sleep function
 *  @note This is an internal function, do not call it directly.
 *  @warning This is a platform-specific function. It is not cross-platform. Use the cross-platform wrapper system_sleep()
 *
 *  Standard wrapper around the UNIX `sleep()` function.
 */

static void unix_sleep(unsigned int seconds) {
    sleep(seconds);
}

#endif


void sleep_seconds(unsigned int seconds) {
    #ifdef _WIN32
        windows_sleep(seconds);
    #else
        unix_sleep(seconds);
    #endif
}
