{ pkgs ? import <nixpkgs> { } }:

pkgs.clangStdenv.mkDerivation {
  name = "mindoom-dev-shell";

  nativeBuildInputs = with pkgs.buildPackages; [
    clangStdenv
    clang
    clang-tools
    cloc
    cppcheck
    ctags
    doxygen
    gcc
    gdb
    gdbgui
    glib
    hotspot
    meson
    ninja
    linuxKernel.packages.linux_6_1.perf
    pkg-config
    SDL2
    SDL2_mixer
    SDL2_net
    valgrind
  ];
}
