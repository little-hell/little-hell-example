project('littlehell', 'c', default_options: ['default_library=static', 'buildtype=debugoptimized'])

add_project_arguments('-DLOG_USE_COLOR', language: 'c')

sdl2 = dependency('SDL2')
sdl2_net = dependency('SDL2_net')
sdl2_mixer = dependency('SDL2_mixer')

deps = [sdl2, sdl2_net, sdl2_mixer]

# Source files only used by the game binary 
engine_source_files = files(
    'd_event.c',
    'doomtype.h',
    'd_mode.c',
    'd_iwad.c',
    'd_loop.c',
    'i_glob.c',
    'i_input.c',
    'i_joystick.c',
    'i_oplmusic.c',
    'i_sdlsound.c',
    'i_sound.c',
    'i_system.c',
    'i_timer.c',
    'i_video.c',
    'midifile.c',
    'mus2mid.c',
    'm_argv.c',
    'log.c',
    'm_bbox.c',
    'm_cheat.c',
    'm_config.c',
    'm_controls.c',
    'm_misc.c',
    'm_fixed.c',
    'net_client.c',
    'net_common.c',
    'net_defs.h',
    'net_io.c',
    'net_loop.c',
    'net_packet.c',
    'net_petname.c',
    'net_query.c',
    'net_sdl.c',
    'net_server.c',
    'net_structrw.c',
    'sha1.c',
    'memio.c',
    'tables.c',
    'v_diskicon.c',
    'v_video.c',
    'w_checksum.c',
    'w_main.c',
    'w_wad.c',
    'w_file.c',
    'w_file_stdc.c',
    'w_file_posix.c',
    'z_native.c'
  )


# Source files for the OPL emulation library
opl_source_dir=  'libs' / 'opl'
opl_source_files = files(
    opl_source_dir / 'opl3.c',
    opl_source_dir / 'opl.c',
    opl_source_dir / 'opl_queue.c',
    opl_source_dir / 'opl_sdl.c',
    opl_source_dir / 'opl_timer.c',
)

opl = library('opl', opl_source_files, dependencies: deps)

littlehell_lib = library('littlehell', engine_source_files, include_directories: include_directories(opl_source_dir), dependencies: [sdl2, sdl2_net, sdl2_mixer], link_with: opl)

# When declaring the dependency, also specify the include directories.

if meson.is_subproject()
  littlehell_dep = declare_dependency(link_with: littlehell_lib, dependencies: [sdl2, sdl2_net, sdl2_mixer], include_directories: include_directories('.'))
endif

run_target('clang-tidy',
  command : '../../scripts/clang-tidy.sh')

run_target('clang-format',
  command : '../../scripts/clang-format.sh')

run_target('cppcheck',
  command : '../../scripts/cppcheck.sh')

