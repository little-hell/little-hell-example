# This where you include all the source files for your game.
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
    'i_main.c',
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
executable('my-little-hell-example-game', 
    sources: [
        game_source_files,
    ],
    # Provided by the little-hell-engine Meson subproject, and found by the top-level meson.build
    dependencies: littlehell_dep 
)

run_target('clang-tidy',
  command : 'scripts/clang-tidy.sh')

run_target('clang-format',
  command : 'scripts/clang-format.sh')

run_target('cppcheck',
  command : 'scripts/cppcheck.sh')
