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
// DESCRIPTION:
//	System specific file globbing interface.
//


#ifndef __I_GLOB__
#define __I_GLOB__

#define GLOB_FLAG_NOCASE  0x01
#define GLOB_FLAG_SORTED  0x02

typedef struct glob_s glob_t;

// Same as I_StartGlob but multiple glob patterns can be provided. The list
// of patterns must be terminated with NULL.
glob_t *I_StartMultiGlob(const char *directory, int flags,
                         const char *glob, ...);

#endif

