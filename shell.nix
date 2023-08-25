{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    jack2
    meson
    ninja
    pulseaudio
    alsa-lib
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
