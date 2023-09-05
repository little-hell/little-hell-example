# This where you include all the source files for your game.
server_source_files = files(
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
