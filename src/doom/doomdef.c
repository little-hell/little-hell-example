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
//  DoomDef - basic defines for DOOM, e.g. Version, game mode
//   and skill level, and display parameters.
//

#include "doomdef.h"

// This is the initial health a player has when starting anew.
// See G_PlayerReborn in g_game.c
const uint8_t INITIAL_HEALTH = 100; 

// This is the number of bullets the player has when starting anew.
// See G_PlayerReborn in g_game.c
const uint8_t INITIAL_BULLETS = 50;

// This is the maximum health that can be reached using health
// potions. See P_TouchSpecialThing in p_inter.c
const uint8_t MAX_HEALTH = 200;

// This is the maximum armor which can be reached by picking up
// armor helmets. See P_TouchSpecialThing in p_inter.c
const uint8_t MAX_ARMOR  = 200;

// This is the armor class that is given when picking up the green 
// armor or an armor helmet. See P_TouchSpecialThing in p_inter.c
const uint8_t GREEN_ARMOR_CLASS = 1;

// This is the armor class that is given when picking up the blue 
// armor or a megasphere. See P_TouchSpecialThing in p_inter.c
const uint8_t BLUE_ARMOR_CLASS = 2;

// The maximum health which can be reached by picking up the
// soulsphere. See P_TouchSpecialThing in p_inter.c
const uint8_t MAX_SOULSPHERE = 200;

// The amount of health bonus that picking up a soulsphere
// gives. See P_TouchSpecialThing in p_inter.c
const uint8_t SOULSPHERE_HEALTH = 100;

// This is what the health is set to after picking up a 
// megasphere. See P_TouchSpecialThing in p_inter.c
const uint8_t MEGASPHERE_HEALTH = 200;

// This is what the health value is set to when cheating using
// the IDDQD god mode cheat. See ST_Responder in st_stuff.c
const uint8_t GOD_MODE_HEALTH = 200;

// This is what the armor is set to when using the IDFA cheat.
// See ST_Responder in st_stuff.c
const uint8_t IDFA_ARMOR = 200;

// This is what the armor class is set to when using the IDFA cheat.
// See ST_Responder in st_stuff.c
const uint8_t IDFA_ARMOR_CLASS = 2;

// This is what the armor is set to when using the IDKFA cheat.
// See ST_Responder in st_stuff.c
const uint8_t IDKFA_ARMOR = 200;

// This is what the armor class is set to when using the IDKFA cheat.
// See ST_Responder in st_stuff.c
const uint8_t IDKFA_ARMOR_CLASS = 2;

// This is the number of CELLs firing the BFG uses up.
// See P_CheckAmmo and A_FireBFG in p_pspr.c
const uint8_t BFG_CELLS_PER_SHOT = 40;

// This controls whether monsters can harm other monsters of the same 
// species.  For example, whether an imp fireball will damage other
// imps.  The value of this in dehacked patches is weird - '202' means
// off, while '221' means on.
//
// See PIT_CheckThing in p_map.c
const uint8_t SPECIES_INFIGHTING = 0;
