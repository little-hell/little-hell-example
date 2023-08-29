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
 * DESCRIPTION:
 * 	Status bar widget code
 *
 * This file was previously named "st_lib.c"
 */

#include <stdio.h>
#include <ctype.h>

#include "../doomdef.h"

#include "../../log.h"

#include "../../z_zone.h"
#include "../../v_video.h"

#include "../../i_swap.h"
#include "../../i_system.h"

#include "../../w_wad.h"

#include "statusbar.h"
#include "widget.h"
#include "../r_local.h"


//
// Hack display negative frags.
//  Loads and store the stminus lump.
//
patch_t *sttminus;

void STlib_init(void)
{
    if (W_CheckNumForName("STTMINUS") >= 0)
    {
        sttminus = (patch_t *) W_CacheLumpName("STTMINUS", PU_STATIC);
    }
    else
    {
        sttminus = NULL;
    }
}

/**
 * @brief Creates a new status bar widget for displaying a number 
 * @param x The x position of the new widget
 * @param y The y position of the new widget
 * @param pl The patch list for number graphics 
 * @param num The number to be displayed by the widget
 * @param on Whether the widget is enabled (and thus drawn) 
 * @param width The amount of numbers that can be displayed (i.e width=3 for a 3 digit number like health or ammo.)
 * @param percent_sign_graphic The graphics patch for the percent sign symbol. Use `NULL` for creating number widgets without a percent sign.
 * @return The newly-created widget. 
 * 
 * **Note**: The return value must be freed after use. 
 *
 */
widget_number_t *STWidget_CreateNumberWidget(int x, int y, patch_t **pl, int *num,
                                         boolean *on, int width, patch_t *percent_sign_graphic)
{
    log_debug("STWidget_CreateNumberWidget(): Creating a number widget at (%d,%d) of width %dpx, enabled %s", x, y, width, btoa(on));
    widget_number_t *widget = malloc(sizeof(widget_number_t));

    widget->x = x;
    widget->y = y;
    widget->oldnum = 0;
    widget->width = width;
    widget->num = num;
    widget->on = on;
    widget->p = pl;
    widget->percent_sign_graphic = percent_sign_graphic;

    return widget;
}

/**
 * \deprecated Use STWidget_CreateNumberWidget()
 */
void STlib_initNum(widget_number_t *n, int x, int y, patch_t **pl, int *num, boolean *on,
                   int width)
{
    n->x = x;
    n->y = y;
    n->oldnum = 0;
    n->width = width;
    n->num = num;
    n->on = on;
    n->p = pl;
}


/**
 * @brief Draws a number widget to the status bar. 
 *
 */
void STWidget_DrawNumberWidget(widget_number_t *widget, boolean refresh)
{
    // Don't draw widgets that have been turned off
    if (!*widget->on)
    {
        return; 
    }
        
    // Draw the percentage sign if we've been given a graphic patch for it 
    if (refresh && widget->percent_sign_graphic != NULL)
    {
        V_DrawPatch(widget->x, widget->y, widget->percent_sign_graphic);
    }
        
    // A fairly efficient way to draw a number
    //  based on differences from the old number.
    // Note: worth the trouble?
    //
    // TODO: refactor this heavily

    int numdigits = widget->width;
    int num = *widget->num;

    int w = SHORT(widget->p[0]->width);
    int h = SHORT(widget->p[0]->height);
    int x = widget->x;

    int neg;

    widget->oldnum = *widget->num;

    neg = num < 0;

    if (neg)
    {
        if (numdigits == 2 && num < -9)
        {
            num = -9;
        }
        else if (numdigits == 3 && num < -99)
        {
            num = -99;
        }

        num = -num;
    }

    // clear the area
    x = widget->x - numdigits * w;

    if (widget->y - ST_Y < 0)
    {
        I_Error("drawNum: widget->y - ST_Y < 0");
    }

    V_CopyRect(x, widget->y - ST_Y, st_backing_screen, w * numdigits, h, x, widget->y);

    // if non-number, do not draw it
    if (num == 1994)
    {
        return;
    }

    x = widget->x;

    // in the special case of 0, you draw 0
    if (!num)
    {
        V_DrawPatch(x - w, widget->y, widget->p[0]);
    }

    // draw the new number
    while (num && numdigits--)
    {
        x -= w;
        V_DrawPatch(x, widget->y, widget->p[num % 10]);
        num /= 10;
    }

    // draw a minus sign if necessary
    if (neg && sttminus)
    {
        V_DrawPatch(x - 8, widget->y, sttminus);
    }
}

//
// A fairly efficient way to draw a number
//  based on differences from the old number.
// Note: worth the trouble?
//
/**
 * \deprecated Use STWidget_DrawNumberWidget()
 */
void STlib_drawNum(widget_number_t *n, boolean refresh)
{

    int numdigits = n->width;
    int num = *n->num;

    int w = SHORT(n->p[0]->width);
    int h = SHORT(n->p[0]->height);
    int x = n->x;

    int neg;

    n->oldnum = *n->num;

    neg = num < 0;

    if (neg)
    {
        if (numdigits == 2 && num < -9)
        {
            num = -9;
        }
        else if (numdigits == 3 && num < -99)
        {
            num = -99;
        }

        num = -num;
    }

    // clear the area
    x = n->x - numdigits * w;

    if (n->y - ST_Y < 0)
    {
        I_Error("drawNum: n->y - ST_Y < 0");
    }

    V_CopyRect(x, n->y - ST_Y, st_backing_screen, w * numdigits, h, x, n->y);

    // if non-number, do not draw it
    if (num == 1994)
    {
        return;
    }

    x = n->x;

    // in the special case of 0, you draw 0
    if (!num)
    {
        V_DrawPatch(x - w, n->y, n->p[0]);
    }

    // draw the new number
    while (num && numdigits--)
    {
        x -= w;
        V_DrawPatch(x, n->y, n->p[num % 10]);
        num /= 10;
    }

    // draw a minus sign if necessary
    if (neg && sttminus)
    {
        V_DrawPatch(x - 8, n->y, sttminus);
    }
}

/**
 * \deprecated Use STWidget_DrawNumberWidget()
 */
void STlib_updateNum(widget_number_t *n, boolean refresh)
{
    if (*n->on)
    {
        STlib_drawNum(n, refresh);
    }
}

/**
  * \deprecated Use STWidget_CreateNumberWidget()
  */
void STlib_initPercent(st_percent_t *p, int x, int y, patch_t **pl, int *num, boolean *on,
                       patch_t *percent)
{
    STlib_initNum(&p->n, x, y, pl, num, on, 3);
    p->p = percent;
}

/**
  * \deprecated Use STWidget_CreateNumberWidget()
  */
void STlib_updatePercent(st_percent_t *per, int refresh)
{
    if (refresh && *per->n.on)
    {
        V_DrawPatch(per->n.x, per->n.y, per->p);
    }

    STlib_updateNum(&per->n, refresh);
}


// TODO: To be replaced by a new function called STWidget_CreateMultiIconWidget()
void STlib_initMultIcon(st_multicon_t *i, int x, int y, patch_t **il, int *inum,
                        boolean *on)
{
    i->x = x;
    i->y = y;
    i->oldinum = -1;
    i->inum = inum;
    i->on = on;
    i->p = il;
}


// TODO: To be replaced by a new function called STWidget_UpdateMultiIconWidget()
void STlib_updateMultIcon(st_multicon_t *mi, boolean refresh)
{
    int w;
    int h;
    int x;
    int y;

    if (*mi->on && (mi->oldinum != *mi->inum || refresh) && (*mi->inum != -1))
    {
        if (mi->oldinum != -1)
        {
            x = mi->x - SHORT(mi->p[mi->oldinum]->leftoffset);
            y = mi->y - SHORT(mi->p[mi->oldinum]->topoffset);
            w = SHORT(mi->p[mi->oldinum]->width);
            h = SHORT(mi->p[mi->oldinum]->height);

            if (y - ST_Y < 0)
            {
                I_Error("updateMultIcon: y - ST_Y < 0");
            }
            V_CopyRect(x, y - ST_Y, st_backing_screen, w, h, x, y);
        }
        V_DrawPatch(mi->x, mi->y, mi->p[*mi->inum]);
        mi->oldinum = *mi->inum;
    }
}

// TODO: To be replaced by a new function called STWidget_CreateBinaryIconWidget()
void STlib_initBinIcon(st_binicon_t *b, int x, int y, patch_t *i, boolean *val,
                       boolean *on)
{
    b->x = x;
    b->y = y;
    b->oldval = false;
    b->val = val;
    b->on = on;
    b->p = i;
}

// TODO: To be replaced by a new function called STWidget_UpdateBinaryIconWidget()
void STlib_updateBinIcon(st_binicon_t *bi, boolean refresh)
{
    int x;
    int y;
    int w;
    int h;

    if (*bi->on && (bi->oldval != *bi->val || refresh))
    {
        x = bi->x - SHORT(bi->p->leftoffset);
        y = bi->y - SHORT(bi->p->topoffset);
        w = SHORT(bi->p->width);
        h = SHORT(bi->p->height);

        if (y - ST_Y < 0)
        {
            I_Error("updateBinIcon: y - ST_Y < 0");
        }
        if (*bi->val)
        {
            V_DrawPatch(bi->x, bi->y, bi->p);
        }
        else
        {
            V_CopyRect(x, y - ST_Y, st_backing_screen, w, h, x, y);
        }
        bi->oldval = *bi->val;
    }
}
