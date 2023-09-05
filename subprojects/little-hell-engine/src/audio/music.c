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

#include <stdio.h>
#include <stdlib.h>

#include "SDL_mixer.h"

#include "music.h"

MusicPlayer* music_create(MusicBackend* backend) {
    MusicPlayer* music = malloc(sizeof(MusicPlayer));
    music->backend = backend;
    return music;
}

void music_play_song(MusicPlayer* music, const char* song_path) {
    if (music->backend && music->backend->play_song) {
        music->backend->play_song(music->backend, song_path);
    }
}

void music_stop_song(MusicPlayer* music) {
    if (music->backend && music->backend->stop_song) {
        music->backend->stop_song(music->backend);
    }
}

void music_destroy(MusicPlayer* music) {
    if (music->backend) {
        if (music->backend->shutdown) {
            music->backend->shutdown(music->backend);
        }
        free(music->backend);
    }
    free(music);
}
