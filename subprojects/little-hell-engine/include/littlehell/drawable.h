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

#ifndef __DRAWABLE__
#define __DRAWABLE__

// We are referring to patches.
#include "../../doom/r_defs.h"

/** @defgroup DrawableAPI Drawable (Developer API)
 *  @ingroup Graphics
 *  @brief Drawable is any 2D element shown on screen that is not part of the game world.
 *  @note drawable_init() Must be called for using the Drawable API.
 *
 *  ## Usage
 *  For example, the ammo counter is a Drawable, an enemy or barrel is not. More specifically,
 *  Drawable provides high-level functions to easily draw a texture (known in DOOM jargon as a
 *  patch) to the screen. Fundamentally every on-screen element (that is not part of the
 *  gameworld) is one or more textures (patches) all the way down.
 *
 *  ## Example
 *
 *  Before creating a Drawable, the Drawable API must be initialized. Unless you're off the beaten
 *  path, it will already be initialized by the engine at start time.
 *
 *  @code
 *  // drawable_init(); Somewhere else the engine calles this function to initialize the API for
 *  // us to use.
 *  Drawable *drawable = drawable_create();
 *  @endcode

 *  If we're drawing a texture such as the icon for the blue keycard, we simply use the Drawable
 *  type and assign its texture to the blue keycard texture found in the WAD.
 *
 *  However, DOOM doesn't support the loading or drawing of abritrary fonts and images on the
 *  filesystem. Those must be textures of some kind, and they must exist in the WAD. So, if we
 *  want to draw numbers or strings to the screen, (whether they be used as ammo counters in the
 *  case of numbers, menu items or chat messages in the case of strings, etc) we're really drawing
 *  a series of individual textures for each alphanumeric symbol.
 *
 *  To expand upon the basic functionality of texture loading and drawing provided by Drawable,
 *  a member sub_type (`DrawableSubType`) is present. This is an enum containing values for valid
 *  sub-types. If no sub-type is being used, the enum value is simply `DRAWABLE`.
 *
 *  Examples of such sub-types are those used to faciliate drawing numbers and strings in a high-
 *  level manner. A sub-type value of `NUMBER` denotes that a `NumberDrawable` will be used.
 *  A sub-type value of `TEXT` denotes that a TextDrawable will be used. If no sub-type will be
 *  used, the value is just `DRAWABLE`.
 *
 *  @{
 */

/** @struct DrawableTextures
 *
 *  Contains textures comprising fonts, UI elements, icons, etc.
 *
 *  DrwableTextures encapsulates and provides easy-access to textures (known as
 *  patches in DOOM jargon).
 *
 *  @var DrawableTextures::large_numbers
 *  Textures for the large red numbers (0 through 9), used in Drawables such as the
 *  health counter. They can be viewed by looking at lumps `STTNUM<x>` in a WAD editor.

 *  @var DrawableTextures::small_numbers
 *  Textures for the small yellow numbers (0 through 9) used in the small ammo counters.
 *  They can be viewed by looking at lumps `STYSNUM<x>` in a WAD editor.

 *  @var DrawableTextures::large_percent_symbol
 *  Texture for a large percent symbol, it can be viewed by looking at lump `STTPRCNT` in a WAD
 *  editor.
 *  \image html STTPRCNT.png "Large percent symbol (`STTPRCNT`)"
 *
 *  @var DrawableTextures::large_minus_symbol
 *  Texture for a large minus symbol, it can be viewed by looking at lump `STTMINUS` in a WAD
 *  editor.
 *
 *  @var DrawableTextures::status_bar
 *  @warning If there is no `STBAR` lump in your WAD, you're not going to be able to use this.
 *
 *  The background texture for the status bar.
 *
 *  @note Inside the WAD, this texture must exist as a lump called `STBAR`.
 *  For reference, here is what the original texture looks like from DOOM.
 *  @image html img/STBAR.png
 *  Note that the texture actually assumes a multiplayer game is happening - there is a frag
 *  counter in place of where the arms counter / weapon inventory usually is. This is actually
 *  a secondary texture called `STARMS` that is simply drawn over the status bar when not in
 *  deathmatch.
 *
 *
 */

typedef struct DrawableTextures
{
    patch_t *large_numbers[10];
    patch_t *small_numbers[10];
    patch_t *large_percent_sign;
    patch_t *large_minus_sign;
    patch_t *status_bar;
} DrawableTextures;

DrawableTextures *drawable_get_textures();

/** @union DrawableValue
 *  @brief Union type encapsulating the types of values a Drawable can draw to the screen.
 */

typedef union DrawableValue
{
    /** Value for a number Drawable **/
    int *number;
    /** Value for a text Drawable **/
    char *text;
    /** Value for a texture Drawable **/
    patch_t *texture;
} DrawableValue;

/**
 * @brief All valid types that a Drawable can be.
 */
typedef enum DrawableType
{
    /** Drawable is a number **/
    DRAWABLE_NUMBER,
    /** Drawable is text **/
    DRAWABLE_TEXT,
    /** Drawable is a texture **/
    DRAWABLE_TEXTURE
} DrawableType;

typedef struct Drawable
{
    DrawableValue value;
    DrawableType type;

    /** The x position of the new Drawable **/

    int x;
    /** The y position of the new Drawable **/
    int y;

    /** Whether a Drawable that draws characters should use the large red font present in the DOOM
    wad (true), or the small yellow font (false). **/
    boolean use_large_font;

    /** Whether the Drawable is enabled and should be updated **/
    boolean *enabled;

    /** The number of digits that (applicable when using a number Drawable **/
    int digits;

    /** Whether or not a percent sign should be shown after a number Drawable **/
    boolean show_percent_sign;
} Drawable;

void drawable_init();

/**
 * @brief Draws a DrawableTexture to the screen.
 * @param drawable The Drawable to be drawn
 */

void drawable_draw(const Drawable *drawable);

/**
 * @brief Creates a Drawable for displaying a DrawableTexture to the screen.
 * @return The newly-created drawable.
 */

Drawable *drawable_create(Drawable drawable);

/**
 * @brief Returns a default Drawable to be used in the creation of a new Drawable.
 * @return The default Drawable.
 */
Drawable drawable_default();


/**
 * @brief Draws a number drawable to the status bar.
 */
void drawable_draw_number(const Drawable *drawable);

void drawable_draw_all();
Drawable *drawable_get_drawable(const char *tag);

/** @} End of Drawable documentation */

#endif // Header guard
