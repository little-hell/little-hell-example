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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * drawable.c
*/

#include "../../log.h"
// Zone memory allocator
#include "../../z_zone.h"
// System functions (exit)
#include "../../i_system.h"
// Drawing code
#include "../../v_video.h"
// Endian-safe conversions
#include "../../i_swap.h"
// Safe string functions 
#include "../../m_misc.h"
// We are referring to patches.
#include "../../doom/r_defs.h"
// Working with WAD data 
#include "../../w_wad.h"

// Using boolean implementation
#include "../../doomtype.h"

#include "drawable.h"

// Module-wide resources accessible to all Drawables. Should not be accessed directly, but by using
// API functions. Contains pointers to all textures a Drawable may use.
static DrawableTextures *textures;
    
pixel_t *screen; 

DrawableTextures *drawable_get_textures() {
    return textures;
}

static pixel_t *create_backing_screen() {
    // TODO: Avoid use of these SCREENWIDTH macros
    log_debug("drawable_create_backing_screen(): Creating backing screen for Drawable rendering.");
    screen = Z_Malloc(SCREENWIDTH * SCREENHEIGHT * sizeof(*screen), PU_STATIC, 0);

    return screen;
}

static void load_texture(patch_t *texture, const char *name) {
    if (W_CheckNumForName(name) >= 0)
    {
        texture = W_CacheLumpName(name, PU_STATIC);
    }
    else 
    {
        log_fatal("Could not load texture \"%s\" from the WAD.", name);
        system_exit();
    }
}

static DrawableTextures *load_textures() {
    //TODO: I believe that with the amount of patches in the WAD, this
    //approach of manually assigned each patch to a variable will not scale.
    //Sure that's how it was done before I started rearchitecting the code, 
    //but it's just not ideal.

    textures = Z_Malloc(sizeof(DrawableTextures), PU_STATIC, 0);

    const int BUFFER_SIZE = 9;
    char patch_name_buffer[BUFFER_SIZE];


    // Load font numbers, both large and small fonts.
    for (int i = 0; i < BUFFER_SIZE + 1; i++)
    {
        M_snprintf(patch_name_buffer, BUFFER_SIZE, "STTNUM%d", i);
        load_texture(textures->large_numbers[i], patch_name_buffer);
        
        M_snprintf(patch_name_buffer, BUFFER_SIZE, "STYSNUM%d", i);
        load_texture(textures->small_numbers[i], patch_name_buffer);
    }
    
    // Load font special characters.
    load_texture(textures->large_minus_sign, "STTMINUS");
    load_texture(textures->large_percent_sign, "STTPRCNT");

    load_texture(textures->status_bar, "STBAR");

    return textures;
}

/** Initialize the Drawable API **/
void drawable_init() {
    textures = load_textures();
    screen = create_backing_screen();
}


void drawable_draw(const Drawable *drawable, boolean refresh) {
    switch (drawable->type) {
        case DRAWABLE_NUMBER:
            drawable_draw_number(drawable, refresh);
            break;
        // TODO: Add more drawable types here
        default:
            printf("Unsupported data type.\n");
    }
}

Drawable *drawable_create(Drawable drawable)
{
    log_trace("drawable_create(): Creating Drawable");

    if (drawable == NULL)
    {
        log_fatal("drawable_create(): Can't create a Drawable with NULL data parameter.");
        system_exit();
    }

    Drawable *drawable = Z_Malloc(sizeof(Drawable), PU_LEVEL, 0);

    if (drawable == NULL) 
    {
	log_fatal("drawable_create(): Memory allocation failed.");
	system_exit();
    }
    
    //drawable->options = options;
    //drawable->options->enabled = enabled;
    
    switch(data->type) 
    {
        case(DRAWABLE_NUMBER):
            break;
        case(DRAWABLE_TEXTURE):
            break;
    }


    if (texture == NULL) 
    {
	log_fatal("drawable_create(): Can not create a drawable with a NULL texture.");
	system_exit();
    }

    return drawable;
}

Drawable *drawable_create_number(
    int *value,
    DrawableOptions *options,
    boolean *enabled
    )
{
    log_debug(
        "drawable_create_number(): Creating a number drawable at (%d,%d) %d digits wide, enabled state: "
        "%s",
        options->x,
        options->y,
        options->digits,
        btoa(&enabled));

    Drawable *drawable = Z_Malloc(sizeof(Drawable), PU_LEVEL, 0);

    if (drawable == NULL) 
    {
	log_fatal("drawable_create_number(): Memory allocation failed.");
	system_exit();
    }

    drawable->options = options;

    drawable->type = DRAWABLE_NUMBER;
    drawable->options->enabled = enabled;
    drawable->value.number = value;

    return drawable;
}

void drawable_draw_number(const Drawable *drawable, boolean refresh)
{
    if (!drawable)
    {
        log_fatal("drawable_draw_number(): Can't draw a NULL drawable. Are you "
                  "trying to draw a drawable that doesn't exist?");
        system_exit();
    }

    // Don't draw drawables that have been turned off
    if (!drawable->options->enabled)
    {
        log_debug("drawable_draw_number(): drawable %x not enabled, so not drawing", drawable);
        return;
    }

    // if non-number, do not draw it
    int num = *drawable->value.number;
    if (num == 1994)
    {
        return;
    }

    patch_t **font; 
    
    patch_t *percent_sign = textures->large_percent_sign;
    patch_t *minus_sign = textures->large_minus_sign;
    
    // Some things can only be drawn with large fonts due to the textures available
    // in the WAD.
    if (drawable->options->use_large_font)
    {
        font = textures->large_numbers;
        
        // Don't draw these textures unless a refresh is happening. 
        if (refresh)
        {
            // Show a percent sign if we've been asked to.
            if (drawable->options->show_percent_sign) 
            {
                V_DrawPatch(drawable->options->x, drawable->options->y, percent_sign);
            }

            // Show a minus sign if the number to be drawn is negative, and if 
            // we've been asked to.
            if (drawable->value.number < 0)
            {
                V_DrawPatch(drawable->options->x - 8, drawable->options->y, minus_sign);
            }
        }
    }
    else 
    {
        font = textures->small_numbers;
    }


    // A fairly efficient way to draw a number
    //  based on differences from the old number.
    // Note: worth the trouble?
    //
    // TODO: refactor this heavily

    int numdigits = drawable->options->digits;

    int w = SHORT(font[0]->width);
    int h = SHORT(font[0]->height);
    int x = drawable->options->x;

    if (drawable->value.number)
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
    x = drawable->options->x - numdigits * w;
    
    // Good luck trying to draw off the screen.
    // TODO: Remove dependence on global ST_Y macro.
    // What if we don't want to draw on the status bar?

    /**
    if (drawable->y - ST_Y < 0)
    {
        log_fatal("drawable_draw_number(): drawable->y - ST_Y < 0");
	system_exit()
    } **/

    V_CopyRect(x, drawable->options->y, screen, w * numdigits, h, x, drawable->options->y);

    x = drawable->options->x;

    // in the special case of 0, you draw 0
    if (!num)
    {
        V_DrawPatch(x - w, drawable->options->y, font[0]);
    }

    // draw the new number
    while (num && numdigits--)
    {
        x -= w;
        V_DrawPatch(x, drawable->options->y, font[num % 10]);
        num /= 10;
    }
}

void drawable_update_number_value(Drawable *drawable, int *value)
{
    drawable->value.number = value;
}
