/*
 * Copyright(C) 1993-1996 Id Software, Inc.
 * Copyright(C) 2005-2014 Simon Howard
 * Copyright(C) 2023 Joshua Murphy 
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * widget.h (previously st_lib.h)
 * Reusable UI widget library 
*/

#ifndef __STLIB__
#define __STLIB__

// We are referring to patches.
#include "../../doom/r_defs.h"

// Multiple Icon widget
typedef struct
{
    // center-justified location of icons
    int x;
    int y;

    // last icon number
    int oldinum;

    // pointer to current icon
    int *inum;

    // pointer to boolean stating
    //  whether to update icon
    boolean *on;

    // list of icons
    patch_t **p;

    // user data
    int data;

} st_multicon_t;


// Binary Icon widget

typedef struct
{
    // center-justified location of icon
    int x;
    int y;

    // last icon value
    boolean oldval;

    // pointer to current icon status
    boolean *val;

    // pointer to boolean
    //  stating whether to update icon
    boolean *on;


    patch_t *p; // icon
    int data;   // user data

} st_binicon_t;

//
// Widget creation, access, and update routines
//

void STlib_init(void);

// Multiple Icon widget routines
void STlib_initMultIcon(st_multicon_t *mi, int x, int y, patch_t **il, int *inum, boolean *on);


void STlib_updateMultIcon(st_multicon_t *mi, boolean refresh);

// Binary Icon widget routines

void STlib_initBinIcon(st_binicon_t *b, int x, int y, patch_t *i, boolean *val, boolean *on);

void STlib_updateBinIcon(st_binicon_t *bi, boolean refresh);

#endif
