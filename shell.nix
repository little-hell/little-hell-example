{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    clang
    clang-tools
    cloc
    cppcheck
    gcc
    gdb
    glib
    meson
    ninja
    pkg-config
    SDL2
    SDL2_mixer
    SDL2_net
  ];
}
