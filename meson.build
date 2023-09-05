project('mindoom', 'c',
       default_options: ['default_library=static', 'buildtype=debugoptimized'])
add_project_arguments('-DLOG_USE_COLOR', language: 'c')

sdl2 = dependency('SDL2')
sdl2_net = dependency('SDL2_net')
sdl2_mixer = dependency('SDL2_mixer')

deps = [sdl2, sdl2_net, sdl2_mixer]

# Source files used by both the client binary and the server binary
src_dir = 'src'
common_source_files = files(
    src_dir / 'engine' / 'doomkeys.h',
    src_dir / 'engine' / 'doomtype.h',
    src_dir / 'engine' / 'i_main.c',
    src_dir / 'engine' / 'i_system.c',
    src_dir / 'engine' / 'm_argv.c',
    src_dir / 'engine' / 'm_misc.c',
    src_dir / 'engine' / 'z_native.c',
    src_dir / 'engine' / 'log.c',
  )

# Source files only used by the server binary 
dedicated_server_source_files = files(
    src_dir / 'engine' / 'd_dedicated.c',
    src_dir / 'engine' / 'd_iwad.c',
    src_dir / 'engine' / 'd_mode.c',
    src_dir / 'engine' / 'i_timer.c',
    src_dir / 'engine' / 'm_config.c',
    src_dir / 'engine' / 'net_common.c',
    src_dir / 'engine' / 'net_dedicated.c',
    src_dir / 'engine' / 'net_io.c',
    src_dir / 'engine' / 'net_packet.c',
    src_dir / 'engine' / 'net_sdl.c', 
    src_dir / 'engine' / 'net_query.c', 
    src_dir / 'engine' / 'net_server.c', 
    src_dir / 'engine' / 'net_structrw.c',
  )


# Source files only used by the game binary 
engine_source_files = files(
    src_dir / 'engine' / 'd_event.c',
    src_dir / 'engine' / 'doomtype.h',
    src_dir / 'engine' / 'd_iwad.c',
    src_dir / 'engine' / 'd_loop.c',
    src_dir / 'engine' / 'd_mode.c',
    src_dir / 'engine' / 'i_glob.c',
    src_dir / 'engine' / 'i_input.c',
    src_dir / 'engine' / 'i_joystick.c',
    src_dir / 'engine' / 'i_oplmusic.c',
    src_dir / 'engine' / 'i_sdlsound.c',
    src_dir / 'engine' / 'i_sound.c',
    src_dir / 'engine' / 'i_timer.c',
    src_dir / 'engine' / 'i_video.c',
    src_dir / 'engine' / 'midifile.c',
    src_dir / 'engine' / 'mus2mid.c',
    src_dir / 'engine' / 'm_bbox.c',
    src_dir / 'engine' / 'm_cheat.c',
    src_dir / 'engine' / 'm_config.c',
    src_dir / 'engine' / 'm_controls.c',
    src_dir / 'engine' / 'm_fixed.c',
    src_dir / 'engine' / 'net_client.c',
    src_dir / 'engine' / 'net_common.c',
    src_dir / 'engine' / 'net_dedicated.c',
    src_dir / 'engine' / 'net_defs.h',
    src_dir / 'engine' / 'net_io.c',
    src_dir / 'engine' / 'net_loop.c',
    src_dir / 'engine' / 'net_packet.c',
    src_dir / 'engine' / 'net_petname.c',
    src_dir / 'engine' / 'net_query.c',
    src_dir / 'engine' / 'net_sdl.c',
    src_dir / 'engine' / 'net_server.c',
    src_dir / 'engine' / 'net_structrw.c',
    src_dir / 'engine' / 'sha1.c',
    src_dir / 'engine' / 'memio.c',
    src_dir / 'engine' / 'tables.c',
    src_dir / 'engine' / 'v_diskicon.c',
    src_dir / 'engine' / 'v_video.c',
    src_dir / 'engine' / 'w_checksum.c',
    src_dir / 'engine' / 'w_main.c',
    src_dir / 'engine' / 'w_wad.c',
    src_dir / 'engine' / 'w_file.c',
    src_dir / 'engine' / 'w_file_stdc.c',
    src_dir / 'engine' / 'w_file_posix.c',
  )

doom_source_dir = 'doom'
game_source_files = files(
    src_dir / doom_source_dir / 'am_map.c',
    src_dir / doom_source_dir / 'd_items.c',
    src_dir / doom_source_dir / 'd_main.c',
    src_dir / doom_source_dir / 'd_net.c',
    src_dir / doom_source_dir / 'doomdef.c',
    src_dir / doom_source_dir / 'doomstat.c',
    src_dir / doom_source_dir / 'dstrings.c',
    src_dir / doom_source_dir / 'f_finale.c',
    src_dir / doom_source_dir / 'f_wipe.c',
    src_dir / doom_source_dir / 'g_game.c',
    src_dir / doom_source_dir / 'hu_lib.c',
    src_dir / doom_source_dir / 'hu_stuff.c',
    src_dir / doom_source_dir / 'info.c',
    src_dir / doom_source_dir / 'm_menu.c',
    src_dir / doom_source_dir / 'm_random.c',
    src_dir / doom_source_dir / 'p_ceilng.c',
    src_dir / doom_source_dir / 'p_doors.c',
    src_dir / doom_source_dir / 'p_enemy.c',
    src_dir / doom_source_dir / 'p_floor.c',
    src_dir / doom_source_dir / 'p_inter.c',
    src_dir / doom_source_dir / 'p_lights.c',
    src_dir / doom_source_dir / 'p_map.c',
    src_dir / doom_source_dir / 'p_maputl.c',
    src_dir / doom_source_dir / 'p_mobj.c',
    src_dir / doom_source_dir / 'p_mobj.h',
    src_dir / doom_source_dir / 'p_plats.c',
    src_dir / doom_source_dir / 'p_pspr.c',
    src_dir / doom_source_dir / 'p_saveg.c',
    src_dir / doom_source_dir / 'p_setup.c',
    src_dir / doom_source_dir / 'p_sight.c',
    src_dir / doom_source_dir / 'p_spec.c',
    src_dir / doom_source_dir / 'p_switch.c',
    src_dir / doom_source_dir / 'p_telept.c',
    src_dir / doom_source_dir / 'p_tick.c',
    src_dir / doom_source_dir / 'p_user.c',
    src_dir / doom_source_dir / 'r_bsp.c',
    src_dir / doom_source_dir / 'r_data.c',
    src_dir / doom_source_dir / 'r_draw.c',
    src_dir / doom_source_dir / 'r_main.c',
    src_dir / doom_source_dir / 'r_plane.c',
    src_dir / doom_source_dir / 'r_segs.c',
    src_dir / doom_source_dir / 'r_sky.c',
    src_dir / doom_source_dir / 'r_things.c',
    src_dir / doom_source_dir / 'sounds.c',
    src_dir / doom_source_dir / 's_sound.c',
    src_dir / doom_source_dir / 'statdump.c',
    src_dir / doom_source_dir / 'hud' / 'widget.c',
    src_dir / doom_source_dir / 'hud' / 'statusbar.c',
    src_dir / doom_source_dir / 'wi_stuff.c'
)

# Source files for the OPL emulation library
opl_source_dir= 'opl'
opl_source_files = files(
    opl_source_dir / 'opl3.c',
    opl_source_dir / 'opl.c',
    opl_source_dir / 'opl_queue.c',
    opl_source_dir / 'opl_sdl.c',
    opl_source_dir / 'opl_timer.c',
)

opl = library('opl', opl_source_files, dependencies: deps)

littlehell = library('littlehell', engine_source_files, include_directories: [include_directories(src_dir), include_directories(opl_source_dir)], dependencies: [sdl2, sdl2_net, sdl2_mixer], link_with: opl)

# Build the game binary
executable('mindoom', 
    sources: [
        common_source_files, 
        game_source_files,
    ],
    link_with: [
        littlehell
    ], 
    include_directories: [
        include_directories('src/engine'),
        include_directories('src/doom'),
        include_directories(opl_source_dir), 
    ],
    dependencies: deps
)

# Build the server binary
#executable('mindoom-server', common_source_files, dedicated_server_source_files, dependencies:
#  deps)

run_target('clang-tidy',
  command : 'scripts/clang-tidy.sh')

run_target('clang-format',
  command : 'scripts/clang-format.sh')

run_target('cppcheck',
  command : 'scripts/cppcheck.sh')

