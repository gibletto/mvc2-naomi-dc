# Notices

## No game data

This repository holds source code only. The game's program, graphics and sound come from the user's own copy,
read at start-up: the arcade's ROM set, or the files of the Dreamcast disc.

## port/sys and port/cmake

The coroutine and division-by-zero support (`port/sys`) and the 3DS toolchain file (`port/cmake`) come from
[sfiii3-cps3-port](https://github.com/gibletto/sfiii3-cps3-port) (AGPL-3.0).

## port/naomi/naomi_m1.c

The M1 cartridge's packed-file scheme is as worked out in MAME's `naomim1.cpp` (BSD-3-Clause,
copyright-holders: Olivier Galibert).

## port/render/rd_gpu_shaders.h

Compiled from `rd_gpu.hlsl` by Microsoft's fxc and dxc (`port/tools/gpu_shaders.sh`); nothing else.

## Libraries

The PC build links [SDL](https://libsdl.org), and the Windows release carries `SDL3.dll`. The 3DS build links
devkitPro's [libctru](https://github.com/devkitPro/libctru) and [citro3d](https://github.com/devkitPro/citro3d).
All three are under the zlib license.

Marvel vs. Capcom 2 and Capcom are trademarks of Capcom; Marvel characters are trademarks of Marvel. This
project is not affiliated with or endorsed by Capcom, Marvel or Sega.
