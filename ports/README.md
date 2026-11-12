# Ports

A port of Marvel vs. Capcom 2, the NAOMI arcade game and the Dreamcast version, to PC and Nintendo 3DS,
built on their decompilation, which is the rest of this repository. It is not an emulator: the decompiled
game is compiled for the host, and the port supplies what the NAOMI board, the Dreamcast and Sega's libraries
did.

To play on Windows, unzip a release, put the game's data in `roms/mvsc2` and `disc` beside `mvc2.exe`, and
run it; `PLAYING.md` has the keys and settings. For the 3DS, see [3DS.md](3DS.md).

Library routines the port lacks are stubs that say so in the log (`port/tools/gen_host.py`).

Sound: the AICA's own driver runs on an emulated ARM7 with its DSP; the Dreamcast's ADX music is decoded by
the port. `MVC2_SOUND=0` turns it off; `MVC2_WAV=file` records it. On the 3DS the DSP is off, and `MVC2_REVERB=1` adds a cheap reverb in its place.

**No game data is included.** You need the MAME `mvsc2` set, unzipped into a folder, and for the
Dreamcast the files of your own USA disc, extracted into a folder.

## How it fits together

- `CMakeLists.txt` compiles the decompilation's C as it is (`MVC2_DECOMP`) twice: as the arcade's program
  and as the Dreamcast's (`TARGET_DC`, with `src/dc`), with the options in `port/include/port_prefix.h`
  standing in for SHC's built-ins. Each program becomes one relocatable object (`arc.o`, `dc.o`) whose
  only global name is `arc_program` or `dc_program`, so both link into one executable over one host.
- `port/tools/gen_host.py` finds what a program uses that nothing defines: its data left in place (filled
  from the user's program image at start-up and relocated), fixed buffers, and a stub for each library
  routine the port lacks yet.
- `port/naomi/` is the board: main RAM mapped at its arcade address (0x0C000000), the cartridge ROM, the
  game run as a coroutine, its interrupts, and Sega's library routines as the port implements them.
- `port/dc/` is what the Dreamcast adds: its program from `1ST_READ.BIN`, the disc (gdFs) over a folder,
  Shinobi's calls, the memory card and the ADX music's decoder.
- `port/snd/` is the sound: the AICA's ARM7 and DSP, which run the game's own sound driver, and Sega's sound
  libraries' calls over them.
- `port/sdl/` is the PC front end (`start.c` picks the version from the data it finds), `port/ctr/` the 3DS
  one. `port/sys/` runs the game as a coroutine and makes a division by zero give 0, as SHC's code did.

## PC (Windows, MSYS2 mingw32)

32-bit, because the game stores addresses in 32 bits. Needs `mingw-w64-i686-gcc`, `-cmake`, `-ninja` and
Python 3, and SDL3: MSYS2's `mingw-w64-i686-sdl3` where it still has one, or the `i686-w64-mingw32` folder of
libsdl's `SDL3-devel-VERSION-mingw.tar.gz`, given as `-DCMAKE_PREFIX_PATH=FOLDER`. From this folder:

    cmake -G Ninja -B build -DCMAKE_C_COMPILER=gcc
    ninja -C build
    build/mvc2 [--arcade | --dc] [FOLDER]

The game's C, symbols and pointer tables come from the folder above. For a separate checkout of the
decompilation, pass `-DMVC2_DECOMP=FOLDER` and make its tables with `tools/pointers.py` (and `PROGRAM=dc`).

`cmake --install build --prefix FOLDER` makes a folder to play from: the two executables, SDL's DLL,
`PLAYING.md` and a sample `mvc2.cfg` (kept if the folder has one), from `port/dist`.

Both versions run at their boards' own 59.94 frames a second, whatever the monitor's refresh rate (both drive
VGA from a 27 MHz clock, 858 by 525). `MVC2_FPS=n` sets another rate, and `MVC2_FPS=0` runs as fast as it can.

The picture is drawn in software, on as many threads as the machine has (`MVC2_RDTHREADS=n` sets the count;
the picture is the same for any count). On a slow machine, `MVC2_RDQUALITY=half` draws it at 320 by 240 and
shows each pixel as four: about three and a half times faster, but small text and sprites lose detail.
`render =` in `mvc2.cfg` sets the quality for every run, and `MVC2_RDQUALITY` overrides it for one. Its words:
`exact` (the default, every pixel as the PowerVR2 draws it), `fast` (the nearest texel, one mip level a
triangle), `half`, `fast+half`, `nofog` (no fog) and `nosort` (translucent triangles not sorted per pixel).

The front end is built with SDL3 (`SDL3.dll` beside the executable). `renderer = gpu` in `mvc2.cfg` draws with
the GPU through SDL_GPU (Direct3D 12 or Vulkan); when it can't, it says why and draws in software.
`-DMVC2_SDL3=OFF` builds it with SDL2 (`mingw-w64-i686-SDL2`), software only.

`build/mvc2dc` is the Dreamcast alone. Test runs show no message boxes and run as fast as they can unless
`MVC2_FPS` is set:

- `MVC2_FRAMES=n` stops after n frames; `MVC2_SHOT=n,m,...` saves those frames as `shot_n.bmp`;
- `MVC2_PRESS=n:key,...` has player 1 press start, up, down, left, right or b1 to b6 at frame n, and
  `n:coin` and `n:switch` put a coin in or switch versions;
- the arcade's unless `--dc`; no memory card unless `MVC2_VMU=FILE` (a missing file is a blank card);
  `eeprom.bin` and `backup.bin` in the current folder; only `MVC2_UNLOCK`, not `unlock =`;
- `MVC2_CLOCK=seconds` (UTC) fixes the clocks.

### Controls

The keyboard:

|                | Player 1 | Player 2 |
|----------------|----------|----------|
| move           | arrows   | I J K L  |
| punch (L, H)   | A S      | R T      |
| assist 1       | D        | Y        |
| kick (L, H)    | Z X      | F G      |
| assist 2       | C        | H        |
| start          | 1        | 2        |
| coin           | 5        |          |

Controllers: the first plugged in is player 1's, the second player 2's. Each button is named by its place, with
the Xbox letter after it:

| Button                 |                    |
|------------------------|--------------------|
| d-pad, left stick      | move               |
| West (X), North (Y)    | light, heavy punch |
| South (A), East (B)    | light, heavy kick  |
| left or right shoulder | assist 1           |
| left or right trigger  | assist 2           |
| Start                  | start              |
| Back                   | coin               |

So an arcade stick's top row punches and its bottom row kicks, as on the cabinet. On the Dreamcast, the six
buttons (light punch to assist 2) are its X Y L A B R. `gamecontrollerdb.txt` beside the executable adds
controllers SDL doesn't know.

| Key                          |                                                                   |
|------------------------------|-------------------------------------------------------------------|
| F1, or Back + right shoulder | the controls screen, over the paused game                         |
| F2                           | the arcade's test button (its test menu)                          |
| 9                            | the arcade's service button                                       |
| F3                           | the game's and the drawing's time a frame, and the frame rate     |
| F9                           | switch to the other version                                       |
| F10                          | scaling: `aspect` (4:3), `integer` (whole multiples) or `stretch` |
| F11, Alt+Enter               | full screen                                                       |
| Escape                       | quit                                                              |

On the controls screen, Enter sets a control from the next key or button, Delete puts its default back, and
Escape closes; it also switches the renderer and the picture quality. `mvc2.cfg` keeps the controls, a line
each (`p1 b1 = key:A, pad:x`), and the settings, the scaling and the window's size.

`unlock = all` in `mvc2.cfg` (or `MVC2_UNLOCK=1`) offers every character, colour and stage. Only the select
scene sees it (a `HOST_PORT` arm in the decomp's `select.c`), so the saved unlocks stay the game's own.

Saves are kept beside the executable: the Dreamcast's memory card as `vmu_save_A1.bin` (a VMU's 128 KB flash
image, as Flycast keeps it), the arcade's settings and book-keeping as `eeprom.bin` and `backup.bin`.

## Linux (32-bit ARM)

It builds for 32-bit ARM Linux (`armhf`: a Raspberry Pi's 32-bit system, MiSTer's Cortex-A9), natively or
with a cross compiler, against SDL3 or SDL2, but it has not been run yet. 32-bit x86 Linux needs coroutines
and a division-by-zero handler in `port/sys` first. Cross-building from Debian, with SDL built for `armhf`:

    cmake -G Ninja -B build-arm -DCMAKE_SYSTEM_NAME=Linux -DCMAKE_SYSTEM_PROCESSOR=armv7l \
          -DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -DCMAKE_C_FLAGS="-mcpu=cortex-a9 -mfpu=neon" \
          -DCMAKE_FIND_ROOT_PATH=SDL_PREFIX
    ninja -C build-arm

Main RAM is mapped at 0x0C000000 as on Windows (it says so and stops if it can't be). Division by zero gives 0
as on the arcade through libgcc's helpers; a `-mcpu` with hardware division (Cortex-A7, A15, A53) would give a
remainder by zero of the dividend instead, so keep to the plain `armv7-a` the distributions build for.

## 3DS (devkitARM, libctru, citro3d)

One build for the Old 3DS and 2DS, which on a New 3DS also turns on the faster CPU. With `DEVKITPRO` set:

    cmake -G Ninja -B build-3ds -DCMAKE_TOOLCHAIN_FILE=port/cmake/3ds-native.cmake
    ninja -C build-3ds

That makes `mvc2.3dsx`, and `mvc2.cia` too when `makerom` and `bannertool` are on the path. On an Old 3DS
the CIA asks for the 80 MB memory mode, which leaves about 8 MB for a cache of the arcade's ROM, where a 3dsx's
64 MB leaves next to none (the program takes about 13 MB, main RAM 32, the GPU 8, the sound RAM 8 on the arcade
and 2 on the Dreamcast), so there the 3dsx runs the Dreamcast version and the arcade's asks for the CIA.
`-DOLD_3DS_TEST=ON` makes the Old 3DS test build, which runs on a New 3DS as an Old 3DS does: its own title, no
faster CPU, and no more memory than an Old 3DS gives (64 MB as a 3dsx, 80 as the CIA).

Everything is in `sdmc:/3ds/mvc2`: `mvc2.cfg`, `roms/mvsc2` and `disc` as on the PC, and `mvc2.log`. The
first start joins the ROM set's files into one file, `roms/mvsc2/mvsc2.bin` (136 MB), which the game then
reads in 64 KB blocks through a cache of what memory allows, a thread on the other core reading ahead. The
picture is drawn by the GPU at 320 by 240, 1:1 in the middle of the top screen; `scale = stretch` stretches
it across, and `render = exact` draws it at 640 by 480 and halves it (slower). Test runs read `env.txt`'s
`NAME=value` lines as environment variables (`MVC2_FRAMES`, `MVC2_SHOT`, `MVC2_PRESS` as on the PC).

## Which game it runs

At start-up `mvc2` looks for each version's data, checking that the files are there with the right sizes
(it does not read them through):

- the arcade's: the MAME `mvsc2` set unzipped (`epr-23085a.ic11` and `mpr-23048.ic17s` to `mpr-23084.ic32s`);
- the Dreamcast's: the files of the USA disc (`1ST_READ.BIN` and the rest).

With one found, that one runs. With both, it asks which to play, once, and keeps the answer in `mvc2.cfg`
(`mode=arcade` or `mode=dc`); `--arcade` or `--dc` picks one for a run. With neither, it says in its window what each
needs and where to put it, and Enter looks again.

Each is looked for in its folder: a `FOLDER` on the command line (the one picked by `--arcade` or `--dc`, or
without either, whichever it holds), else `MVC2_ROM` or `MVC2_DC_DISC`, else `arcade=` or `dc=` in
`mvc2.cfg`, else `roms/mvsc2` and `disc`. `mvc2.cfg` is in the executable's folder, and the folders it
names, or the default ones, are relative to that folder too. A folder on the command line or in an environment
variable is relative to the current folder. For example:

    # where each mode's data is, and the mode to start in when both are found
    arcade = D:/games/mvsc2
    dc = D:/games/mvc2-disc
    mode = dc
    render = exact

While it runs, F9 switches to the other version, when its data was found too. The switch is a fresh start:
the host's layers and each program are linked as one relocatable whose data lies between two names
(`port/game/span_begin.c`, `span_end.c`), kept as it starts and put back with the board stopped
(`port/sys/restart.c`), so the other version starts exactly as it would from the command line.
