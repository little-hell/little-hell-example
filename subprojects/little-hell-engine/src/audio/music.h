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

#ifndef MUSIC_H
#define MUSIC_H

#include <stdbool.h>

/** @defgroup MusicAPI Music (Developer API)
 *  @ingroup Audio
 *  @brief Music player
 */

/** @ingroup MusicAPI
 *  @brief Interface for a music backend
 *
 *  The MusicBackend interface allows the creation of different backends for playing music.
 *  Those wishing to create their own backends for playing music may implement this interface
 *  and use it with MusicPlayer.
 */
typedef struct MusicBackend {
    void* context;

    void (*initialize)(struct MusicBackend* backend, const char* config);
    void (*play_song)(struct MusicBackend* backend, const char* song_path);
    void (*stop_song)(struct MusicBackend* backend);
    void (*shutdown)(struct MusicBackend* backend);
} MusicBackend;

/** @ingroup MusicAPI
 *  @brief Music Player class
 *
 *  The MusicPlayer is an extendable and modular API for playing music. Developers may use
 *  an existing MusicBackend or create their own, and control it with a common set of functions
 *  for playing and stopping music.
 */
typedef struct MusicPlayer {
    MusicBackend* backend;
} MusicPlayer;

/** @ingroup MusicAPI
 *  @brief Creates a new music player
 *  @param backend Class that implements the MusicBackend interface.
 */
MusicPlayer* music_create(MusicBackend* backend);

/** @ingroup MusicAPI
 *  @brief Begins playing a song.
 *  @param backend Class that implements the MusicBackend interface.
 *  @todo The function parameters should be changed such that a developer can either
 *  pass a MIDI struct, or simply the name of a MIDI lump. Either way, it should take
 *  care of as much heavy lifting as possible.
 */
void music_play_song(MusicPlayer* music_player, const char* song_path);

/** @ingroup MusicAPI
 *  @brief Stops the currently playing song.
 */
void music_stop_song(MusicPlayer* music_player);

/** @ingroup MusicAPI
 *  @brief Destroys and frees a music player
 *  @param music_player The music player to be destroyed.
 */
void music_destroy(MusicPlayer* music_player);

#endif
