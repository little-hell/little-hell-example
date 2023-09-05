# Source files used by both the client binary and the server binary
#common_source_files = files(
#    src_dir / 'littlehell' / 'doomkeys.h',
#    src_dir / 'littlehell' / 'doomtype.h',
#    src_dir / 'littlehell' / 'i_main.c',
#    src_dir / 'littlehell' / 'i_system.c',
#    src_dir / 'littlehell' / 'm_argv.c',
#    src_dir / 'littlehell' / 'm_misc.c',
#    src_dir / 'littlehell' / 'z_native.c',
#    src_dir / 'littlehell' / 'log.c',
#  )

# Source files only used by the server binary 
dedicated_server_source_files = files(
    'net_dedicated.c',
  )

doom_source_dir = 'doom'
game_source_files = files(
    'am_map.c',
    'd_items.c',
    'd_main.c',
    'd_net.c',
    'doomdef.c',
    'doomstat.c',
    'dstrings.c',
    'f_finale.c',
    'f_wipe.c',
    'g_game.c',
    'hu_lib.c',
    'hu_stuff.c',
    'info.c',
    'm_menu.c',
    'm_random.c',
    'p_ceilng.c',
    'p_doors.c',
    'p_enemy.c',
    'p_floor.c',
    'p_inter.c',
    'p_lights.c',
    'p_map.c',
    'p_maputl.c',
    'p_mobj.c',
    'p_mobj.h',
    'p_plats.c',
    'p_pspr.c',
    'p_saveg.c',
    'p_setup.c',
    'p_sight.c',
    'p_spec.c',
    'p_switch.c',
    'p_telept.c',
    'p_tick.c',
    'p_user.c',
    'r_bsp.c',
    'r_data.c',
    'r_draw.c',
    'r_main.c',
    'r_plane.c',
    'r_segs.c',
    'r_sky.c',
    'r_things.c',
    'sounds.c',
    's_sound.c',
    'statdump.c',
    'hud' / 'widget.c',
    'hud' / 'statusbar.c',
    'wi_stuff.c'
)

# Build the game binary
executable('mindoom', 
    sources: [
        game_source_files,
    ],
    dependencies: littlehell_dep 
)

# Build the server binary
#executable('mindoom-server', common_source_files, dedicated_server_source_files, dependencies: deps)

run_target('clang-tidy',
  command : '../scripts/clang-tidy.sh')

run_target('clang-format',
  command : '../scripts/clang-format.sh')

run_target('cppcheck',
  command : '../scripts/cppcheck.sh')

