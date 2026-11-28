# Ports

A port of Marvel vs. Capcom 2, the NAOMI arcade game and the Dreamcast version, to PC, Nintendo 3DS, PSP and MiSTer,
built on their decompilation, which is the rest of this repository. It is not an emulator: the decompiled
game is compiled for the host, and the port supplies what the NAOMI board, the Dreamcast and Sega's libraries
did.

To play on Windows, unzip a release, put the game's data in `roms/mvsc2` and `disc` beside `mvc2.exe`, and
run it; `PLAYING.md` has the keys and settings. On Linux the same, with the `.AppImage`. For the 3DS, see
[3DS.md](3DS.md); for the PSP, which plays the Dreamcast version, [PSP.md](PSP.md); for the MiSTer, with the
Pinedra GPU core, [MISTER.md](MISTER.md).

For the Dreamcast version, on the PC, the 3DS and the PSP alike, put any one of these in `disc`:

| What                                   | Example                                                 |
|----------------------------------------|---------------------------------------------------------|
| a CHD                                  | `Marvel vs. Capcom 2 (USA).chd`                         |
| a GDI with its tracks                  | `disc.gdi`, `track01.bin`, `track02.raw`, `track03.bin` |
| a CUE with its BINs (Redump's set)     | `Marvel vs. Capcom 2 (USA).cue` and its three `.bin`s   |
| track 3 alone, raw or as an ISO        | `Marvel vs. Capcom 2 (USA) (Track 3).bin`, `mvc2.iso`   |
| the files already extracted            | `1ST_READ.BIN` and the rest                             |

On the PC an image is read where it is; the 3DS and the PSP unpack it into `disc` at the first start. A CHD made with
zstd (`chdman createcd -c cdzs,cdfl`) unpacks fastest.

Library routines the port lacks are stubs that say so in the log (`port/tools/gen_host.py`).

Sound: the AICA's own driver runs on an emulated ARM7 with its DSP; the Dreamcast's ADX music is decoded by
the port. `MVC2_SOUND=0` turns it off; `MVC2_WAV=file` records it. On the 3DS the DSP is off, and `MVC2_REVERB=1` adds a cheap reverb in its place.

**No game data is included.** You need the MAME `mvsc2` set (the zip or its files), and for the
Dreamcast your own USA disc: an image of it (a `.chd`, a `.gdi` or Redump `.cue` with its tracks, or an `.iso`
or `.bin` of track 3), or its files already extracted.

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

Both versions run at 59.94 frames a second by default, the Dreamcast's VGA (858 by 525 at 27 MHz), whatever the
monitor's refresh rate: a timer paces every renderer, and vsync only times each present to a refresh. The arcade
board runs a little slower: its Kamui library sets 852 by 530 at 31 kHz (59.79 Hz) and 852 by 269 at 15 kHz
(58.90 Hz). `rate` in `mvc2.cfg` (`dc`, `arcade`, `arcade15` or 57 to 61) sets the timer and each frame's sound
together. Where vsync would hold the game back (a display slower than the rate, by its mode or measured: frames
late with vsync on and in time without), it is turned off and the log says so; `vsync = on` or `off` decides it
instead. `MVC2_FPS=n` sets any rate for a test, and `MVC2_FPS=0` runs as fast as it can.

The picture is drawn by the GPU through SDL_GPU (Direct3D 12 or Vulkan), by the PowerVR2's rules, at the
window's resolution: `resolution = 1` to `12` in `mvc2.cfg` sets a multiple of 640 by 480 instead (`4.2`, say), `aa = 2` to `4`
draws that many times larger again and filters it down, and `sprites = sharp`, `smooth`, `pixel`, `arcade` or `texel`
sets how a flat textured triangle (a sprite, the HUD, text) is read when drawn larger. The first four take its colours
at the 640 by 480 frame's pixels, as the arcade shows them; `texel` reads its texels where they fall, each pixel the
mean of those under it, so the characters' pixels (1.67 by 2.14 of the frame's) come out evenly sized, and it draws at
the window's own size rather than a whole multiple filtered down. At 1920 by 1440 a character's pixel is 5 by 6.43;
at `resolution = 4.2`, 2688 by 2016, it is 7 by 9 exactly. With `texel` and `scale = integer`
the frame is shown pixel for pixel in the window's middle: 2688 by 2016 on a screen at least that size
(a 4K screen keeps 576-pixel bars at the sides, 72 above and below), else 1920 by 1440 or 1280 by 960.
`texel_grid = shared` (the default) moves every sprite's origin to one grid of the characters' pixels (5/3 by 15/7
of the frame's, from the screen's corner), as the CPS2-era art was laid out, so the parts of a character drawn as
separate sprites meet without slivers or gaps; `object` keeps each sprite on its own. [SPRITES.md](SPRITES.md) shows
why, in pictures. `hud = arcade` (the default), `smooth` or `sharp` sets the rest: the game's front (models put
without the camera's view, and strips: the timer, gauges, words and menus) in the 640 by 480 frame's own pixels,
each a block, or anything that isn't a sprite read as the game filters it, or all of it as sprites are.
Where SDL_GPU can't start (a card with neither Vulkan nor Direct3D 12), OpenGL draws the frame's lists with its
fixed function as the MiSTer's does (`port/render/rd_gl1.c`), in SDL's OpenGL renderer's context
(`port/sdl/gl_sdl.c`), at the same sizes, without aa or the sprites' and HUD's modes; `renderer = gl` asks for it.
`renderer = soft`, or neither starting, draws it in software at 640 by 480 instead, on as many threads as the machine
has (`MVC2_RDTHREADS=n` sets the count; the picture is the same for any count). On a slow machine,
`MVC2_RDQUALITY=half` draws it at 320 by 240 and shows each pixel as four: about three and a half times faster, but
small text and sprites lose detail.
`render =` in `mvc2.cfg` sets the quality for every run, and `MVC2_RDQUALITY` overrides it for one. Its words:
`exact` (the default, every pixel as the PowerVR2 draws it), `fast` (the nearest texel, one mip level a
triangle), `lite` (near the exact pixels, in integers: with `fast`, the quickest at 640 by 480), `half`, `fast+half`,
`nofog` (no fog) and `nosort` (translucent triangles not sorted per pixel).

The front end is built with SDL3 (`SDL3.dll` beside the executable). When the GPU can't start, it says why and
draws with OpenGL, or in software; test runs draw in software unless `mvc2.cfg` names a renderer.
`-DMVC2_SDL3=OFF` builds it with SDL2 (`mingw-w64-i686-SDL2`), software only.
`-DMVC2_FMAC=OFF` rounds the sums the SH-4 fuses into one instruction twice, a multiply and then an add, as
Flycast Dojo does: for checking against Fightcade's replays. The default rounds them once, as the board does.
`-DMVC2_RAM_BASE=0x0A000000` maps the game's 32 MB of main RAM there instead of at its own 0x0C000000, for a
host that can't have that address (a multiple of 64 KB, clear of 0x0C000000 to 0x0E000000);
`-DMVC2_RAM_SIZE=0x01000000` maps only the Dreamcast's 16 MB, for a build that runs only its program.

`build/mvc2dc` is the Dreamcast alone. Test runs show no message boxes and run as fast as they can unless
`MVC2_FPS` is set:

- `MVC2_FRAMES=n` stops after n frames; `MVC2_SHOT=n,m,...` saves those frames as `shot_n.bmp` (on the GPU, the
  window's picture at 640 by 480);
- `MVC2_PRESS=n:key,...` has player 1 press start, up, down, left, right or b1 to b6 at frame n, and
  `n:coin` and `n:switch` put a coin in or switch versions;
- the arcade's unless `--dc`; no memory card unless `MVC2_VMU=FILE` (a missing file is a blank card);
  `eeprom.bin` and `backup.bin` in the current folder; only `MVC2_UNLOCK` and `MVC2_FREEPLAY`, not `unlock =` or `freeplay =`;
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
| F2                           | the arcade's test menu (the arcade only)                          |
| 9                            | the arcade's service button                                       |
| F3                           | the game's and the drawing's time a frame, and the frame rate     |
| F9                           | switch to the other version                                       |
| F10                          | scaling: `aspect` (4:3), `integer` (whole multiples) or `stretch` |
| F11, Alt+Enter               | full screen, `desktop` or `exclusive` (F1's FULL SCREEN)          |
| Escape                       | quit                                                              |

On the controls screen, Enter sets a control from the next key or button, Delete puts its default back, and
Escape closes; it also sets the renderer, the picture and full screen. `mvc2.cfg` keeps the controls, a line
each (`p1 b1 = key:A, pad:x`), and the settings, the scaling and the window's size.

F2 opens the arcade's test menu: its game data, and its settings (difficulty, damage, timer, continue, game
speed and the rest) with free play added. The arrows choose, Enter sets and Escape goes back; SAVE & EXIT
writes `eeprom.bin` (and `freeplay = on` or `off` in `mvc2.cfg`), and the game starts again with them on
leaving. On the 3DS, the bottom screen's DIP button opens it.

`unlock = all` in `mvc2.cfg` (or `MVC2_UNLOCK=1`) offers every character, colour and stage. Only the select
scene sees it (a `HOST_PORT` arm in the decomp's `select.c`), so the saved unlocks stay the game's own.

Saves are kept beside the executable: the Dreamcast's memory card as `vmu_save_A1.bin` (a VMU's 128 KB flash
image, as Flycast keeps it), the arcade's settings and book-keeping as `eeprom.bin` and `backup.bin`.

## Linux (32-bit x86 and ARM)

On x86 it is the PC build for Linux (a Steam Machine, or any x86-64 system with 32-bit libraries): `i686`, as on
Windows. The release's is built in `tools/linux/Dockerfile`'s image (Debian bullseye's i386 gcc, so glibc 2.31, and
SDL 3.4.2 built there) by `sh tools/linux/build.sh VERSION OUTDIR`, which writes `mvc2-linux-VERSION.tar.gz`: the
Windows zip's files, with `MvC2-VERSION-i686.AppImage` for the executables (`mvc2`, its RPATH `$ORIGIN`, beside
`libSDL3.so.0` and the `libxkbcommon.so.0` that SDL's Wayland needs, with `tools/linux/mvc2.desktop` and the icon,
behind AppImage's type-2 runtime). Run from an
AppImage (`$APPIMAGE`), the game keeps its data, `mvc2.cfg` and saves in the AppImage's folder. Elsewhere, with
gcc's 32-bit libraries and an i686 SDL3:

    cmake -G Ninja -B build-linux -DCMAKE_C_FLAGS=-m32 -DCMAKE_PREFIX_PATH=SDL_PREFIX
    ninja -C build-linux

Drawn in software, its frames are Windows', bit for bit. It links at 0x400000, as Windows does, so its heap starts far below the
game's RAM; `port/sys/divzero_i386.c` makes a division by zero give 0 from SIGFPE.

32-bit ARM (`armhf`: a Raspberry Pi's 32-bit system, MiSTer's Cortex-A9) builds natively or with a cross
compiler, against SDL3 or SDL2. Cross-building from Debian, with SDL built for `armhf`:

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
first start unpacks the ROM set into one file, `roms/mvsc2/mvsc2.bin` (136 MB), which the game then
reads in 64 KB blocks through a cache of what memory allows, a thread on the other core reading ahead. The
picture is drawn by the GPU at 320 by 240, 1:1 in the middle of the top screen; `scale = stretch` (or
`MVC2_STRETCH=1`) stretches it across, and `render = exact` draws it at 640 by 480 and halves it (slower). `wide = on`
(or `MVC2_WIDE=1`, the default on a New 3DS) draws to all 800 of the top screen's columns, 640 by 240 between the
pillars, or 640 by 480 halved down the screen alone with `render = exact`. Test runs read `env.txt`'s
`NAME=value` lines as environment variables (`MVC2_FRAMES`, `MVC2_SHOT`, `MVC2_PRESS` as on the PC).

## PSP (pspdev)

The Dreamcast version alone, for now: the arcade's needs 32 MB of game RAM, more than the PSP has to spare. A PSP-2000,
PSP-3000 or PSP Go (64 MB) with custom firmware; [PSP.md](PSP.md) has the rest.

## Which game it runs

At start-up `mvc2` looks for each version's data, checking that the files are there with the right sizes
(it does not read them through):

- the arcade's: the MAME `mvsc2` set (`epr-23085a.ic11` and `mpr-23048.ic17s` to `mpr-23084.ic32s`), or its
  `mvsc2.zip` in the folder or beside it, which the first start unpacks into `mvsc2.bin`;
- the Dreamcast's: the files of the USA disc (`1ST_READ.BIN` and the rest), or an image of the disc there,
  read in place through a directory of track 3's ISO 9660 file system kept in memory (`port/dc/dc_image.c`): a
  `.chd` (libchdr), a `.gdi` with its tracks, a `.cue` with its `.bin`s, an `.iso` of track 3 or track 3's
  `.bin` alone. The handhelds unpack it into the folder once instead (`MVC2_DISC_IN_PLACE=1` reads it in place,
  `MVC2_DISC_CHD=1` a CHD too), each file written as `NAME.part` and renamed, `1ST_READ.BIN` last, so a stopped
  unpack starts again.

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
