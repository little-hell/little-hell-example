{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    gdb
    meson
    ninja
    libsndfile
    pcre2
    glib
    pkg-config
    cppcheck
    clang
    clang-tools
    SDL2
    SDL2_mixer
    SDL2_net
  ];
}
