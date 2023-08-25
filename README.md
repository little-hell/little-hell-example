# zendoom

Zendoom is the [MINIX](https://en.wikipedia.org/wiki/Minix) of Doom ports. It's a hyper-minimalist fork of Chocolate Doom that aims to:
- Have the smallest, easiest to study version of the Doom codebase, by throwing backwards compatibility to the wind (if necessary).
- Make exploring the core principles of the Doom engine as zen-like as possible.
- Integrate an embedded scripting language (such as Lua or Guile) to allow experimentation with the engine in a higher-level language.

One could argue that this completely guts and kills the soul of DOOM. And you probably wouldn't even be wrong, but this is for science and John Carmack released Doom under the GPL.

## Motivation

### Zen in the build system
Build systems for C can be awfully yucky and not very Zen. Zendoom supports one build system: Meson. There aren't a dozen nested `CMakeLists.txt` and `Makefile.am` files. There is one build file, 99% of which is just listing the source files to be compiled. It's _great_. This means:

- [x] Add Nix shell for Zen-like, project dependency management and an isolated development environment. _Your system package manager is not Zen._
- [x] Add Meson build 
- [x] Remove CMake build
- [x] Remove GNU autotools build

### Zen in platform support, packaging and distribution
`zendoom` simply does not care about packaging and distribution. This means: 
- [x] Removal of packaging support for Win32 and macOS.
- [x] No Linux packages either. No `.rpm` or `.deb` files. 

It's a project built around exploration of the codebase - build the code yourself. 

### Zen in sound support and emulation
- [x] Remove GUS emulation
- [x] Remove PC speaker emulation
- [x] Remove PAS 
- [x] Remove MIDI music via Fluidsynth, Timidity, and SDL.
- [x] Remove music packs

All we use is the OPL emulation. And we drop the hardware support for using a real OPL chip.

### Zen in features and legacy support
A vast majority of the wonderful features added to source ports are extraneous to our goal of understanding the Doom engine. To that end, an awful lot has been removed:

- [x] Remove DOOM II, Strife, Heretic, and Hexen support - we're talking about _Doom_ and _Doom_ only.
- [ ] Remove emulation and handling of various DOOM 1 minor version releases - we do Doom v1.9 only. 
- [ ] No hacks. No DeHackEd, no wad-merging like DeuTex. Just the vanilla stuff.
- [x] Remove the `setup` application. Edit the `.cfg` by hand. Minimalism, baby.

The only features it _adds_ are to change the default controls to utilize the `WASD` key cluster, with `E` for interaction. Because anything else would not be Zen.

### Zen statistics

Lines of code aren't a metric of code quality, but they sure are a metric of code quantity.

| Chocolate Doom     | files | blank | comment | code   | Zen Doom           | files | blank | comment | code  |
|--------------------|-------|-------|---------|--------|--------------------|-------|-------|---------|-------|
| C                  | 329   | 37986 | 40769   | 198641 | C                  | 165   | 17626 | 13785   | 66092 |
| C/C++ Header       | 223   | 5647  | 7973    | 24521  | C/C++ Header       | 142   | 2942  | 4702    | 9482  |
| Markdown           | 13    | 499   | 9       | 2485   | Markdown           | 4     | 162   | 0       | 733   |
| Meson              |       |       |         |        | Meson              | 1     | 14    | 8       | 253   |
| make               | 19    | 258   | 14      | 1072   | make               |       |       |         |       |
| CMake              | 16    | 98    | 258     | 858    | CMake              | 1     | 1     | 0       | 76    |
| Objective-C        | 5     | 303   | 196     | 789    | Objective-C        |       |       |         |       |
| Python             | 5     | 276   | 228     | 709    | Python             | 1     | 15    | 32      | 57    |
| Nix                |       |       |         |        | Nix                | 1     | 0     | 0       | 19    |
| XML                | 2     | 3     | 2       | 614    | XML                |       |       |         |       |
| YAML               | 4     | 28    | 3       | 296    | YAML               |       |       |         |       |
| m4                 | 1     | 40    | 4       | 246    | m4                 |       |       |         |       |
| Bourne Again Shell | 4     | 3     | 3       | 32     | Bourne Again Shell | 1     | 0     | 0       | 6     |
| Dockerfile         | 1     | 11    | 8       | 20     | Dockerfile         |       |       |         |       |
| TNSDL              | 1     | 1     | 0       | 11     | TNSDL              |       |       |         |       |
| Sum                | 626   | 45186 | 49487   | 230396 | Sum                | 317   | 20761 | 18528   | 76885 |

## Building

```bash
git clone https://github.com/zendoom/doom
cd doom
meson build
ninja -C build
```

### Dependencies

The only dependency _you_ need is `git`, and Nix. All the dependencies that _Doom_ needs are taken care of. 


## Compability

Over the years a lot of configuration options have been added to various source ports of Doom as they add support for different things. Here is an (incomplete) table that overviews what options remain in zendoom:

| Configuration Value        | Removed?    |
|----------------------------|-------------|
| video_driver               |             |
| window_position            |             |
| fullscreen                 |             |
| video_display              |             |
| aspect_ratio_correct       |             |
| integer_scaling            |             |
| vga_porch_flash            |To be removed|
| window_width               |             |
| window_height              |             |
| fullscreen_width           |             |
| fullscreen_height          |             |
| force_software_renderer    |             |
| max_scaling_buffer_pixels  |             |
| startup_delay              |             |
| show_endoom                |             |
| show_diskicon              |             |
| png_screenshots            |To be removed|
| snd_samplerate             |             |
| snd_cachesize              |             |
| snd_maxslicetime_ms        |             |
| snd_pitchshift             |             |
| snd_musiccmd               |Removed      |
| snd_dmxoption              |Removed      |
| opl_io_port                |Removed      |
| use_libsamplerate          |             |
| libsamplerate_scale        |             |
| music_pack_path            |Removed      |
| timidity_cfg_path          |Removed      |
| gus_patch_path             |Removed      |
| gus_ram_kb                 |Removed      |
| vanilla_savegame_limit     |             |
| vanilla_demo_limit         |             |
| vanilla_keyboard_mapping   |             |
| player_name                |             |
| grabmouse                  |             |
| novert                     |             |
| mouse_acceleration         |             |
| mouse_threshold            |             |
| mouseb_strafeleft          |             |
| mouseb_straferight         |             |
| mouseb_use                 |             |
| mouseb_backward            |             |
| mouseb_prevweapon          |             |
| mouseb_nextweapon          |             |
| dclick_use                 |             |
| joystick_guid              |             |
| joystick_index             |             |
| joystick_x_axis            |             |
| joystick_x_invert          |             |
| joystick_y_axis            |             |
| joystick_y_invert          |             |
| joystick_strafe_axis       |             |
| joystick_strafe_invert     |             |
| joystick_look_axis         |             |
| joystick_look_invert       |             |
| joystick_physical_button0  |             |
| joystick_physical_button1  |             |
| joystick_physical_button2  |             |
| joystick_physical_button3  |             |
| joystick_physical_button4  |             |
| joystick_physical_button5  |             |
| joystick_physical_button6  |             |
| joystick_physical_button7  |             |
| joystick_physical_button8  |             |
| joystick_physical_button9  |             |
| joystick_physical_button10 |             |
| joyb_strafeleft            |             |
| joyb_straferight           |             |
| joyb_menu_activate         |             |
| joyb_toggle_automap        |             |
| joyb_prevweapon            |             |
| joyb_nextweapon            |             |
| key_pause                  |             |
| key_menu_activate          |             |
| key_menu_up                |             |
| key_menu_down              |             |
| key_menu_left              |             |
| key_menu_right             |             |
| key_menu_back              |             |
| key_menu_forward           |             |
| key_menu_confirm           |             |
| key_menu_abort             |             |
| key_menu_help              |             |
| key_menu_save              |             |
| key_menu_load              |             |
| key_menu_volume            |             |
| key_menu_detail            |             |
| key_menu_qsave             |             |
| key_menu_endgame           |             |
| key_menu_messages          |             |
| key_menu_qload             |             |
| key_menu_quit              |             |
| key_menu_gamma             |             |
| key_spy                    |             |
| key_menu_incscreen         |             |
| key_menu_decscreen         |             |
| key_menu_screenshot        |             |
| key_map_toggle             |             |
| key_map_north              |             |
| key_map_south              |             |
| key_map_east               |             |
| key_map_west               |             |
| key_map_zoomin             |             |
| key_map_zoomout            |             |
| key_map_maxzoom            |             |
| key_map_follow             |             |
| key_map_grid               |             |
| key_map_mark               |             |
| key_map_clearmark          |             |
| key_weapon1                |             |
| key_weapon2                |             |
| key_weapon3                |             |
| key_weapon4                |             |
| key_weapon5                |             |
| key_weapon6                |             |
| key_weapon7                |             |
| key_weapon8                |             |
| key_prevweapon             |             |
| key_nextweapon             |             |
| key_message_refresh        |             |
| key_demo_quit              |             |
| key_multi_msg              |             |
| key_multi_msgplayer1       |             |
| key_multi_msgplayer2       |             |
| key_multi_msgplayer3       |             |
| key_multi_msgplayer4       |             |

## Roadmap
- v0.0.1:
    - [x] Remove CMake build system
    - [x] Remove Autotools build system
    - [x] Remove codebases for Strife, Hexen, and Heretic
    - [x] Remove packaging and distribution code
    - [x] Misc. repository cleanup
- v0.0.2:
    - [x] Remove GUS emulation
    - [x] Remove music packs
    - [x] Remove native MIDI playback
    - [x] Remove fluidsynth music playback
    - [x] Remove timidity support
    - [x] Only support/emulation of OPL3 / Soundblaster.
        - [ ] No support for real OPL hardware (just the emulation)
    - [ ] Refactor the remaining sound subsystems       
- v0.0.3:
    - [ ] Remove all references to non-DOOM v1.9 in the code.
- v0.0.4:
    - [ ] Remove DeHackEd
    - [ ] Remove WAD merging (a la DeuTex) 

## TODO:

- Github actions
- Embedded scripting of some kind with a well-documented API
- A simpler configuration system? Text files are fine, but surely we can clean it up someway.
- Only support PNG screenshots (no PCX)
- Remove DOS emulation of null Read Access Violation (system.c:300)
- (Gradually) replace bespoke file I/O with posix ones
- Remove gamma correction feature
