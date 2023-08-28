project('mindoom', 'c',
       default_options: ['default_library=static', 'buildtype=debugoptimized'])
add_project_arguments('-DDISABLE_SDL_MIXER', language: 'c')

sdl2 = dependency('SDL2')
sdl2_net = dependency('SDL2_net')
sdl2_mixer = dependency('SDL2_mixer')

deps = [sdl2, sdl2_net, sdl2_mixer]

# Source files used by both the client binary and the server binary
src_dir = 'src'
common_source_files = files(
    src_dir / 'doomkeys.h',
    src_dir / 'doomtype.h',
    src_dir / 'i_main.c',
    src_dir / 'i_system.c',
    src_dir / 'm_argv.c',
    src_dir / 'm_misc.c',
  )

# Source files only used by the server binary 
dedicated_server_source_files = files(
    src_dir / 'd_dedicated.c',
    src_dir / 'd_iwad.c',
    src_dir / 'd_mode.c',
    src_dir / 'i_timer.c',
    src_dir / 'm_config.c',
    src_dir / 'net_common.c',
    src_dir / 'net_dedicated.c',
    src_dir / 'net_io.c',
    src_dir / 'net_packet.c',
    src_dir / 'net_sdl.c', 
    src_dir / 'net_query.c', 
    src_dir / 'net_server.c', 
    src_dir / 'net_structrw.c',
    src_dir / 'z_native.c' 
  )


# Source files only used by the game binary 
doom_source_dir = 'doom'
game_source_files = files(
    src_dir / 'd_event.c',
    src_dir / 'doomtype.h',
    src_dir / 'd_iwad.c',
    src_dir / 'd_loop.c',
    src_dir / 'd_mode.c',
    src_dir / 'i_endoom.c',
    src_dir / 'i_glob.c',
    src_dir / 'i_input.c',
    src_dir / 'i_joystick.c',
    src_dir / 'i_oplmusic.c',
    src_dir / 'i_sdlsound.c',
    src_dir / 'i_sound.c',
    src_dir / 'i_timer.c',
    src_dir / 'i_video.c',
    src_dir / 'i_videohr.c',
    src_dir / 'midifile.c',
    src_dir / 'mus2mid.c',
    src_dir / 'm_bbox.c',
    src_dir / 'm_cheat.c',
    src_dir / 'm_config.c',
    src_dir / 'm_controls.c',
    src_dir / 'm_fixed.c',
    src_dir / 'net_client.c',
    src_dir / 'net_common.c',
    src_dir / 'net_dedicated.c',
    src_dir / 'net_defs.h',
    src_dir / 'net_gui.c',
    src_dir / 'net_io.c',
    src_dir / 'net_loop.c',
    src_dir / 'net_packet.c',
    src_dir / 'net_petname.c',
    src_dir / 'net_query.c',
    src_dir / 'net_sdl.c',
    src_dir / 'net_server.c',
    src_dir / 'net_structrw.c',
    src_dir / 'sha1.c',
    src_dir / 'memio.c',
    src_dir / 'tables.c',
    src_dir / 'v_diskicon.c',
    src_dir / 'v_video.c',
    src_dir / 'w_checksum.c',
    src_dir / 'w_main.c',
    src_dir / 'w_wad.c',
    src_dir / 'w_file.c',
    src_dir / 'w_file_stdc.c',
    src_dir / 'w_file_posix.c',
    src_dir / 'w_merge.c',
    src_dir / 'z_zone.c',

    src_dir / doom_source_dir / 'am_map.c',
    src_dir / doom_source_dir / 'd_items.c',
    src_dir / doom_source_dir / 'd_main.c',
    src_dir / doom_source_dir / 'd_net.c',
    src_dir / doom_source_dir / 'doomdef.c',
    src_dir / doom_source_dir / 'doom_icon.c',
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
    src_dir / doom_source_dir / 'st_lib.c',
    src_dir / doom_source_dir / 'st_stuff.c',
    src_dir / doom_source_dir / 'wi_stuff.c'
)

# Source files for the textscreen library
textscreen_source_dir= 'textscreen'
textscreen_source_files = files(
    src_dir / 'doomkeys.h',
    textscreen_source_dir / 'textscreen.h',
    textscreen_source_dir / 'txt_button.c',
    textscreen_source_dir / 'txt_button.h',
    textscreen_source_dir / 'txt_checkbox.c',
    textscreen_source_dir / 'txt_checkbox.h',
    textscreen_source_dir / 'txt_conditional.c',
    textscreen_source_dir / 'txt_conditional.h',
    textscreen_source_dir / 'txt_desktop.c',
    textscreen_source_dir / 'txt_desktop.h',
    textscreen_source_dir / 'txt_dropdown.c',
    textscreen_source_dir / 'txt_dropdown.h',
    textscreen_source_dir / 'txt_fileselect.c',
    textscreen_source_dir / 'txt_fileselect.h',
    textscreen_source_dir / 'txt_gui.c',
    textscreen_source_dir / 'txt_gui.h',
    textscreen_source_dir / 'txt_inputbox.c',
    textscreen_source_dir / 'txt_inputbox.h',
    textscreen_source_dir / 'txt_io.c',
    textscreen_source_dir / 'txt_io.h',
    textscreen_source_dir / 'txt_label.c',
    textscreen_source_dir / 'txt_label.h',
    textscreen_source_dir / 'txt_main.h',
    textscreen_source_dir / 'txt_radiobutton.c',
    textscreen_source_dir / 'txt_radiobutton.h',
    textscreen_source_dir / 'txt_scrollpane.c',
    textscreen_source_dir / 'txt_scrollpane.h',
    textscreen_source_dir / 'txt_sdl.c',
    textscreen_source_dir / 'txt_sdl.h',
    textscreen_source_dir / 'txt_separator.c',
    textscreen_source_dir / 'txt_separator.h',
    textscreen_source_dir / 'txt_spinctrl.c',
    textscreen_source_dir / 'txt_spinctrl.h',
    textscreen_source_dir / 'txt_strut.c',
    textscreen_source_dir / 'txt_strut.h',
    textscreen_source_dir / 'txt_table.c',
    textscreen_source_dir / 'txt_table.h',
    textscreen_source_dir / 'txt_utf8.c',
    textscreen_source_dir / 'txt_utf8.h',
    textscreen_source_dir / 'txt_widget.c',
    textscreen_source_dir / 'txt_widget.h',
    textscreen_source_dir / 'txt_window_action.c',
    textscreen_source_dir / 'txt_window_action.h',
    textscreen_source_dir / 'txt_window.c',
    textscreen_source_dir / 'txt_window.h',
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

textscreen = library('textscreen', textscreen_source_files, include_directories: include_directories(src_dir), dependencies: deps)
opl = library('opl', opl_source_files, dependencies: deps)

# Build the game binary
executable('mindoom', 
    sources: [
        common_source_files, 
        game_source_files,
    ],
    link_with: [
        textscreen, 
        opl,
    ], 
    include_directories: [
        include_directories('src'),
        include_directories(textscreen_source_dir),
        include_directories(opl_source_dir), 
    ],
    dependencies: deps
)

# Build the server binary
executable('mindoom-server', common_source_files, dedicated_server_source_files, dependencies:
  deps)

run_target('clang-tidy',
  command : 'scripts/clang-tidy.sh')

#run_target('clang-format',
#  command : 'scripts/clang-format.sh')

run_target('cppcheck',
  command : 'scripts/cppcheck.sh')

