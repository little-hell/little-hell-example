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
// 	The status bar widget code.
//

#ifndef __STLIB__
#define __STLIB__


// We are referring to patches.
#include "../r_defs.h"

//
// Typedefs of widgets
//

// Number widget

typedef struct
{
    // upper right-hand corner
    //  of the number (right-justified)
    int x;
    int y;

    // max # of digits in number
    int num_digits;

    // last number value
    int oldnum;

    // pointer to current value
    int *value;

    // pointer to boolean stating
    //  whether to update number
    boolean *enabled;

    // list of patches for 0-9
    patch_t **patches;

    // user data
    int data;

    // percent sign graphics patch (used for drawing numbers with a percent sign)
    // or NULL for regular numbers
    patch_t *percent_sign_patch;

} widget_number_t;

typedef struct
{
    widget_number_t *numerator;
    widget_number_t *denominator;
} widget_fraction_t;


/**
 * \deprecated Use widget_percent_t
 */
// Percent widget ("child" of number widget,
//  or, more precisely, contains a number widget.)
typedef struct
{
    // number information
    widget_number_t n;

    // percent sign graphic
    patch_t *p;

} st_percent_t;


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

// Initializes widget library.
// More precisely, initialize STMINUS,
//  everything else is done somewhere else.
//
void STlib_init(void);


widget_number_t *STWidget_CreateNumberWidget(
    int x,
    int y,
    int num_digits,
    int *value,
    boolean *enabled,
    patch_t **patches,
    patch_t *percent_sign_patch);
widget_fraction_t *STWidget_CreateFractionWidget(
    int x,
    int y,
    int *numerator_value,
    int *denominator_value,
    boolean *enabled,
    patch_t **patches);

void STWidget_DrawNumberWidget(widget_number_t *widget, pixel_t *screen, boolean refresh);
void STWidget_DrawFractionWidget(widget_fraction_t *widget, pixel_t *screen, boolean refresh);

void STlib_initNum(
    widget_number_t *n, int x, int y, patch_t **pl, int *num, boolean *on, int width);

void STlib_updateNum(widget_number_t *n, boolean refresh);

// Multiple Icon widget routines
void STlib_initMultIcon(st_multicon_t *mi, int x, int y, patch_t **il, int *inum, boolean *on);


void STlib_updateMultIcon(st_multicon_t *mi, boolean refresh);

// Binary Icon widget routines

void STlib_initBinIcon(st_binicon_t *b, int x, int y, patch_t *i, boolean *val, boolean *on);

void STlib_updateBinIcon(st_binicon_t *bi, boolean refresh);

#endif
