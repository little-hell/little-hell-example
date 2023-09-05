#!/bin/sh
cd "${MESON_SOURCE_ROOT}"/src
clang-tidy --checks=readability-* *.c -p "${MESON_BUILD_ROOT}"/compile_commands.json
