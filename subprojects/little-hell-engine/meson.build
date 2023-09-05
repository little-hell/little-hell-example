project('little-hell-engine', 'c', default_options: ['default_library=static', 'buildtype=debugoptimized'])

add_project_arguments('-DLOG_USE_COLOR', language: 'c')

sdl2 = dependency('SDL2')
sdl2_net = dependency('SDL2_net')
sdl2_mixer = dependency('SDL2_mixer')

deps = [sdl2, sdl2_net, sdl2_mixer]

src_dir = 'src'

# Source files only used by the game binary 
engine_source_files = files(
    src_dir / 'd_event.c',
    src_dir / 'doomtype.h',
    src_dir / 'd_mode.c',
    src_dir / 'd_iwad.c',
    src_dir / 'd_loop.c',
    src_dir / 'i_glob.c',
    src_dir / 'i_input.c',
    src_dir / 'i_joystick.c',
    src_dir / 'i_oplmusic.c',
    src_dir / 'i_sdlsound.c',
    src_dir / 'i_sound.c',
    src_dir / 'i_system.c',
    src_dir / 'i_timer.c',
    src_dir / 'i_video.c',
    src_dir / 'midifile.c',
    src_dir / 'mus2mid.c',
    src_dir / 'm_argv.c',
    src_dir / 'log.c',
    src_dir / 'm_bbox.c',
    src_dir / 'm_cheat.c',
    src_dir / 'm_config.c',
    src_dir / 'm_controls.c',
    src_dir / 'm_misc.c',
    src_dir / 'm_fixed.c',
    src_dir / 'net_client.c',
    src_dir / 'net_common.c',
    src_dir / 'net_dedicated.c',
    src_dir / 'net_defs.h',
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
    src_dir / 'z_native.c'
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

littlehell_lib = library('liblittlehell', engine_source_files, include_directories: include_directories(opl_source_dir), dependencies: [sdl2, sdl2_net, sdl2_mixer], link_with: opl)

# When declaring the dependency, also specify the include directories.

if meson.is_subproject()
  littlehell_dep = declare_dependency(link_with: littlehell_lib, dependencies: [sdl2, sdl2_net, sdl2_mixer], include_directories: include_directories(src_dir))
endif

run_target('clang-tidy',
  command : 'scripts/clang-tidy.sh')

run_target('clang-format',
  command : 'scripts/clang-format.sh')

run_target('cppcheck',
  command : 'scripts/cppcheck.sh')

