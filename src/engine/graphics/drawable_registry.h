/*
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
 * drawable_registry.h 
 *
 *
 * Provides a registry for all Drawables. 
 */

#ifndef DRAWABLE_REGISTRY_H
#define DRAWABLE_REGISTRY_H

#include "drawable.h"

/** Defines return values of DrawableRegistry function **/
typedef enum {
    DRAWABLE_REGISTRY_SUCCESS = 0,
    /** Memory allocation failed **/
    DRAWABLE_REGISTRY_ALLOCATION_FAILED = -1,
    /** Memory allocation (resize) **/
    DRAWABLE_REGISTRY_RESIZE_FAILED = -2,
    /** Tag for an element exceed length limit **/
    DRAWABLE_REGISTRY_TAG_TOO_LONG = -3,
    /** No element with this tag found in the array **/
    DRAWABLE_REGISTRY_TAG_NOT_FOUND = -4
} DrawableRegistryStatus;

typedef struct DrawableRegistry DrawableRegistry;

DrawableRegistryStatus drawable_registry_initialize(DrawableRegistry* registry);
DrawableRegistryStatus drawable_registry_register(DrawableRegistry* registry, char* tag, Drawable* drawable);
Drawable* drawable_registry_get(const DrawableRegistry* registry, const char* tag);
DrawableRegistryStatus drawable_registry_remove(DrawableRegistry* registry, char* tag);
void drawable_registry_draw_all(const DrawableRegistry* registry);
void drawable_registry_free(DrawableRegistry* registry);

#endif // DRAWABLE_REGISTRY_H

