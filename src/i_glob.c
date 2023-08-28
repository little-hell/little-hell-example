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

#endif /* #ifdef NO_DIRENT_IMPLEMENTATION */
