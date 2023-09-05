project('littlehell-example', 'c')

# A temporary hack to get rid of the need for config.h 
add_global_arguments('-DPACKAGE_STRING="littlehell"', '-DPROGRAM_PREFIX=""', '-DPACKAGE_NAME="littlehell"', language: 'c')

# Try to find the engine dependency on the system. If not found, fall back to the subproject.
engine_dep = dependency('littlehell', required: false)

if not engine_dep.found()
    littlehell_subproj = subproject('littlehell')
    littlehell_dep = littlehell_subproj.get_variable('littlehell_dep')
endif

subdir('game')
