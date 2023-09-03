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
 * widget.c (previously st_lib.c)
 * Reusable UI widget library 
*/	

#include <stdio.h>
#include <ctype.h>

#include "../../doom/doomdef.h"

#include "../../log.h"

#include "../../z_zone.h"
#include "../../v_video.h"

#include "../../i_swap.h"
#include "../../i_system.h"

#include "../../w_wad.h"

#include "../../doom/hud/statusbar.h"
#include "widget.h"
#include "../../doom/r_local.h"


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

void STlib_initMultIcon(st_multicon_t *i, int x, int y, patch_t **il, int *inum, boolean *on)
{
    i->x = x;
    i->y = y;
    i->oldinum = -1;
    i->inum = inum;
    i->on = on;
    i->p = il;
}


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

void STlib_initBinIcon(st_binicon_t *b, int x, int y, patch_t *i, boolean *val, boolean *on)
{
    b->x = x;
    b->y = y;
    b->oldval = false;
    b->val = val;
    b->on = on;
    b->p = i;
}

// TODO: To be replaced by a new function called widget_draw_icon_widget()
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
