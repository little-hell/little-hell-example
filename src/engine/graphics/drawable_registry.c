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
 * drawable_registry.c
 *
 */

#include "drawable_registry.h"
#include "../../log.h"

const int INITIAL_CAPACITY = 10;
const int TAG_SIZE = 50;

typedef struct {
    char tag[TAG_SIZE];
    Drawable* drawable;
} DrawableTagPair;

struct DrawableRegistry {
    DrawableTagPair* data;
    int size;
    int capacity;
};

//** Initialize a DrawableRegistry **/
DrawableRegistryStatus drawable_registry_initialize(DrawableRegistry* registry) {
    log_trace("drawable_registry_initialize(): Initializing DrawableRegistry %x", registry);
    
    registry->size = 0;
    registry->capacity = INITIAL_CAPACITY;
    registry->data = (DrawableTagPair*)malloc(sizeof(DrawableTagPair) * registry->capacity);
    
    if (!registry->data) {
        log_fatal("drawable_registry_initialize(): Failure: Memory allocation for DrawableRegistry failed.");
        return DRAWABLE_REGISTRY_ALLOCATION_FAILED;
    }
        
    log_debug("drawable_registry_initialize(): Great Success! Memory allocated for initialization of DrawableRegistry successful.");
    return DRAWABLE_REGISTRY_SUCCESS;
}

DrawableRegistryStatus drawable_registry_resize(DrawableRegistry* registry) {
    log_trace("drawable_registry_resize(): Resizing DrawableRegistry %x.", registry);

    if !(registry)
    {
        log_error("drawable_registry_resize(): 💩 Failure: Cannot resize a NULL DrawableRegistry (%x).", registry);
    }

    registry->capacity *= 2;
    DrawableTagPair* new_data = (DrawableTagPair*)realloc(registry->data, sizeof(DrawableTagPair) * registry->capacity);
    
    if (!new_data) {
        log_fatal("drawable_registry_resize(): Failure: Memory allocation to resize DrawableRegistry %x failed.", registry);
        return DRAWABLE_REGISTRY_RESIZE_FAILED;
    }

    log_debug("drawable_registry_resize(): Great Success! Memory allocation to resize DrawableRegistry %x successful.", registry);

    registry->data = new_data;
    return DRAWABLE_REGISTRY_SUCCESS;
}

/**
 * Add a Drawable to a DrawableRegistry 
 * @param tag The tag this Drawable should be retrievable with 
 * @param drawable The Drawable to add to the registry 
 */
DrawableRegistryStatus drawable_registry_add(DrawableRegistry* registry, char* tag, Drawable* drawable) {
    log_trace("drawable_registry_add(): Adding new Drawable %x with tag \"%d\" to DrawableRegistry %x.", tag, drawable, registry);
    
    if !(registry)
    {
        log_error("drawable_registry_add(): Cannot add element to a NULL DrawableRegistry (%x).", registry);
    }

    if (strlen(tag) >= TAG_SIZE) {
        log_error("drawable_registry_add(): Failed to add element to DrawableRegistry: tag length exceeded.");
        return DRAWABLE_REGISTRY_TAG_TOO_LONG;
    }

    if (registry->size == registry->capacity) {
        DrawableRegistryStatus resizeStatus = drawable_registry_resize(registry);
        if (resizeStatus != DRAWABLE_REGISTRY_SUCCESS) {
            return resizeStatus;
        }
    }

    snprintf(registry->data[registry->size].tag, sizeof(registry->data[registry->size].tag), "%s", tag);
    registry->data[registry->size].drawable = drawable;
    registry->size++;
    
    log_debug("drawable_registry_add(): Great Success! Added Drawable %x with tag \"%d\" to DrawableRegistry %x.", tag, drawable, registry);

    return DRAWABLE_REGISTRY_SUCCESS;
}

/**
 * Get a Drawable from the DrawableRegistry 
 * @param tag The tag that should be used to find the Drawable
 * @return The Drawable or NULL if the Drawable is not found 
 */
Drawable* drawable_registry_get(const DrawableRegistry* registry, const char* tag) {
    for (int i = 0; i < registry->size; i++) {
        if (strcmp(registry->data[i].tag, tag) == 0) {
            return registry->data[i].drawable;
        }
    }
    return NULL; // If not found, return NULL
}

/**
 * Get a Drawable from the DrawableRegistry 
 * @param tag The tag that should be used to find the Drawable
 * @return The Drawable or NULL if the Drawable is not found 
 */
DrawableRegistryStatus drawable_registry_remove(DrawableRegistry* registry, char* tag) {
    for (int i = 0; i < registry->size; i++) {
        if (strcmp(registry->data[i].tag, tag) == 0) {
            for (int j = i; j < registry->size - 1; j++) {
                registry->data[j] = registry->data[j + 1];
            }
            registry->size--;
            return DRAWABLE_REGISTRY_SUCCESS;
        }
    }
    return DRAWABLE_REGISTRY_TAG_NOT_FOUND;
}

void drawable_registry_draw_all(const DrawableRegistry* registry) {
    for (int i = 0; i < registry->size; i++) {
        drawable_draw(registry->data[i].drawable);
    }
}

void drawable_registry_free(DrawableRegistry* registry) {
    if (registry->data) {
        free(registry->data);
        registry->data = NULL;
    }
    registry->size = 0;
    registry->capacity = 0;
}
