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
 *
 */

#ifndef REGISTRY_H
#define REGISTRY_H

/** @brief Return values / error codes for Registry API functions. **/
typedef enum
{
    REGISTRY_SUCCESS,
    REGISTRY_ALLOCATION_ERROR,
    REGISTRY_KEY_LIMIT_EXCEEDED,
    REGISTRY_KEY_NOT_FOUND
} RegistryStatus;

typedef struct
{
    char *key;
    void *value;
} Entry;

typedef struct
{
    Entry *data;
    int size;
    int capacity;
} Registry;

// Public API
Registry *registry_create();
RegistryStatus registry_add(Registry *registry, const char *key, void *value);
void *registry_get(Registry *registry, const char *key);
RegistryStatus registry_remove(Registry *registry, const char *key);
void registry_free(Registry *registry);

#endif // REGISTRY_H
