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
    clang
    SDL2
    SDL2_mixer
    SDL2_net
  ];
}
