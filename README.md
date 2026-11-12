# Marvel vs. Capcom 2 - NAOMI and Dreamcast program source

**AI Disclosure**:
Yes, this used Claude, the same way the 3rd Strike decomp did.
The names in here are Capcom's wherever they could be found, so blame them for `shl1a_06.c` etc.

## What this is

This is the program of the arcade game *Marvel vs. Capcom 2: New Age of Heroes* (the NAOMI cartridge, MAME set
`mvsc2`), turned back into source code. It is a decompilation: the machine code in the program ROM has been
rewritten as C, and as SH-4 assembler where the original was assembler (the SH-4 is the NAOMI board's CPU), so
people can read it and change it.

It is also a byte-for-byte match. It builds with the original toolchain (Hitachi's SHC 5.1 Release 08, asmsh 4.0,
lnk 6.0A and elf2bin) and Sega's NAOMI libraries: the compiler turns the C into machine code, the assembler does
the same for the assembler files, and the linker joins the pieces into one program. Every routine comes out as the
arcade's exact bytes, and the finished file is the cartridge's program ROM:

    epr-23085a.ic11   4,194,304 bytes   sha1 f58c31b245fc33fa541f9f074548402a63f7c3d3

The ROM holds two programs, the game and the operator's test menu, and the fighting data of the 59 characters
(moves, hit boxes, the computer's play). All of that is built from this source. The pictures, models and sound
are in the cartridge's other ROMs and are not part of this.

The same source also builds the Dreamcast version, from the USA disc (see The Dreamcast version below).

For a tour of how the game itself works (the frame loop, inputs, special moves, hit boxes, damage), read
`docs/HOW_THE_GAME_WORKS.md`.

## Building

You need Windows (the compiler's tools are Win32 programs), Python 3, your own ROM, and two zips from Sega's NAOMI
SDK. The Internet Archive has the SDK as the item `NaomiSDK`:

- `archive/hitachi/pcv51r08.zip` - the compiler, sha1 104e78c75f83a70b023850daa1ea3b729e852ab2
- `archive/naomi/libwin.zip` - Sega's libraries, sha1 a8723959364f9793d0185a73889d64706108b2d1

Then:

1. Put your `epr-23085a.ic11` (or the whole `mvsc2.zip`) in `rom/`.
2. Unpack the two zips, and tell the build where they are. `SHC` is the compiler's `bin` folder, and `SDK_LIBS` is
   four library folders, in this order:

       set SHC=C:\pcv51r08\bin
       set SDK_LIBS=C:\libwin\lib\zero;C:\libwin\lib;C:\libwin\libsrc\libjamma\zero;C:\pcv51r08\lib

3. Run `build.bat` (or `make`).

The build writes `build/epr-23085a.ic11` and prints whether it has the arcade ROM's SHA-1.

A few things are not in this source, so the build cuts them from your ROM first (`tools/mvc2rom.py`):

- seven model files that the game never reads
- 16 KB inside the test program that no build can give: the linker leaves these blocks alone, and the ROM has
  whatever was on Capcom's disk before, pieces of older builds
- 8 KB of Z80 code. The NAOMI has a small chip of its own (315-6149) between the game and the cabinet's
  controls and coin slots, with a Z80 inside. It has no program until the game gives it one, so Sega's library
  carries the Z80 program as a block of data and sends it to the chip at start-up. It is Sega's, and it is not
  SH-4 code, so it is not written out here

No other version of the compiler or the libraries will do. Sega's libraries are part of the program, and most of
their modules are linked straight from `libwin.zip`. The modules that the zip only has in a later version than the
one Capcom linked are built from `src/lib`.

## The Dreamcast version

The same source also builds the Dreamcast version: the USA disc's main program, and the characters' code,
which the disc keeps in files of its own. It is the same game with a home menu around it: options, a training
mode, a shop that sells the extra characters and colours for points, a memory card save, and a network lobby
with a software keyboard. Capcom built it from the arcade's source in the spring of 2000, so most of the files
are the arcade's own.

A file that differs between the two says so with `#ifdef TARGET_DC`, and where the disc changed between its
releases, with the build's date (`#if BUILD_DATE >= 20000412`). The files only the home version has are in
`src/dc`, and the Dreamcast's own build of a Sega library module is in `src/dc/lib`. Every routine comes out as
the disc's exact bytes:

    1ST_READ.BIN    1,809,817 bytes   sha1 b0c3604e3494b12f751793a74fc9b494ed67a30c

and so do the 354 character files (`S_PL00A.BIN` and the rest: each character's code, linked for each of the
six places a fighter's code can be loaded). `dcovl.sha1` lists them.

To build it you need the arcade's two zips, and two of Sega's Katana SDKs for the libraries only the Dreamcast
has. The Internet Archive item `dc_sdks` has them:

- "Sega Katana Development Software - Release 10.zip", sha1 6b063a04e543f851ace49e5aef49488e4d976ce4: `mdmapi.lib`, `modem.lib`,
  `nt_utl.lib` and `shinobi.lib`
- "Sega Katana Development Software - Release 9.zip", sha1 a8b0d630851bd90b39dceb65cd2957bd13208383: `cri_adxn.lib`

Both are needed: Release 10 has no `cri_adxn.lib`, and Release 9's Shinobi is another build. Each zip holds a
disc image with an installer. Install them, then set `SDK_LIBS` to six folders, in this order (the arcade's,
with Release 10's library folder fourth and Release 9's last):

    set SDK_LIBS=C:\libwin\lib\zero;C:\libwin\lib;C:\libwin\libsrc\libjamma\zero;C:\KATANA\Lib;C:\pcv51r08\lib;C:\KATANA_R9\Lib

and run `build.bat dc` (or `make dc`). The build writes `build/mvc2dc.bin` and the characters' files in
`build/ovl/`, and checks their SHA-1s. It needs nothing from your disc.

Things that were learned on the way:

- **A module can be more than one file.** The compiler empties its pool of constants at the end of each file.
  The disc's pools break at five places where one file never would, so the home game's SECRET FACTOR is six
  files (`src/dc/secret.c` and the rest).
- **A routine can be too big to optimise.** At about 256 branches in one routine, SHC gives up on it ("Function
  not optimized") and keeps every local in memory. `secret_factor_3_save` is one. Its code only matches with
  the same number of branches, so it uses a statement that makes no code, `VM_NOP()`, where Capcom's source
  most likely had a macro that was empty in this build.
- **Some of the fake matches are the disc's.** A few routines here only match with a statement that never
  runs (`if( 0 ) ... ;`, marked "left out of the build"). The compiler still counts what it reads and writes,
  so it can change which register a value gets. These are the routines where nothing more natural has been
  found yet, and they are a good place to start contributing.

## Layout

    src/           game code, a file for each of Capcom's
    src/lib/       the modules of Sega's libraries that the SDK has no matching version of (see Sega's libraries)
    src/test/      the test menu's own code
    src/dc/        the files only the Dreamcast version has (src/dc/lib: its builds of Sega's modules)
    data/          the characters' fighting data
    include/       headers; player.h includes the shared ones (pl_work.h has PLAYER_WORK)
    docs/          HOW_THE_GAME_WORKS.md: a tour of the game's code
    mvc2.sub       link order and section addresses (mvc2test.sub for the test menu, mvc2dc.sub for the Dreamcast)
    dcovl.sha1     the SHA-1s of the Dreamcast's characters' files
    functions.tsv  every routine of the game program: arcade address, size, name, file
    tools/         mvc2rom.py: reads your ROM; chrpack.py: packs the characters' data
    rom/           your ROM goes here

A character's code is `src/s_plXX.c`, where XX is the character's number in hexadecimal (`s_pl00.c` is Ryu). Its
fireballs and other effects are in `shlXX_YY.c` and `plsXX_YY.c`, and its data in `data/`.

## Names

Capcom left more than the game in the cartridge. The unused space of the ROMs still holds pieces of what was on the
developers' PC when the images were made: a list of 215 source files, pages of a linker map with 532 symbols, the
compiler's options, and scraps of C and assembler source. This text is in every retail cartridge.

That is where the names here come from. Files named like the ones in that list (`s_pl02.c`, `shl05_00.c`,
`pl_dmg.c`, `game.c`, `flashing.c`) are Capcom's files. Routines, structures and fields found in the map and the
scraps (`PLAYER_WORK`, `routine_2`, `pl_char_set`, `Rnd`) are Capcom's too. Everything else is named in the same
style for what it does. A field we don't understand yet is called `unk_` and its offset.

## Compiler

Two versions of the same compiler can turn the same C into different machine code, and so can two sets of
options. The options Capcom used were sitting in the ROM's unused space:

    -round=near -cpu=sh4 -fpu=single -endian=little -macsave=0 -pic=0 -optimize=1 -size -string=const -extra=a=400

`-extra=a=400` is not in the manual. It makes the assembler stage put a `nop` after each `mov Rm,R0`, and the game
code has 3,325 of them.

With these options, the compiler builds all the C here to the exact bytes: the arcade's 18,306 routines in 700 files
(and the test menu's three files), and the Dreamcast's 18,991 routines in 773 files, 73 of them the home version's
own. Nothing is patched afterwards. The few assembler files (`.src`) are ones that were assembler to begin with: the
start-up, the cache routines, a table, and some of Sega's library modules.

Not every file saw the same headers. The machine code shows how each file thought a routine was declared, and
for a few routines the files do not agree: two dozen of them pass `pl_char_set` its animation number as an `int`,
the rest as a byte. Most likely a prototype changed during development and those files were not compiled again
afterwards. Such a file includes a header with the older prototype (`include/pl_char_old.h` and the other
`_old.h` files), or sets a switch before `player.h` (`#define PL_HYPER_START_NEW`).

## Sega's libraries

This part is for anyone taking apart another NAOMI game.

A NAOMI game is not only the game. Sega's libraries are linked into it: Kamui2 (the graphics driver), Shinobi (the
system library the Dreamcast also had), the NAOMI libraries on top of them (`libnaomi`, `libnlobj`, `libspr`,
`libam`, `libjamma` and others), and the compiler's own run-time library. In this game that is 301 modules.

Sega's NAOMI SDK is on the Internet Archive, and its libraries still link with the same linker. So the first thing
to try is to not decompile them at all: find each library module in the ROM and link Sega's object as it is. Here
260 of the 301 modules come straight out of `libwin.zip` and the compiler's zip, byte for byte. `mvc2.sub` names
each one, for example `libkamui2.lib(kmSetVertex_km2vtxlst_lib_)`.

The catch is the dates. The SDK's NAOMI libraries were built in August 2000. This game was built in February 2000
with what Capcom had then, and the libraries print their own build dates, which are still in the ROM:

    NAOMI LIBRARY Ver 0.9 AM R&D          Dec 22 1999
    NLOBJPUT Ver 0.99                     Dec 17 1999
    libspr Ver 0.2                        Dec 16 1999
    nlam Ver 1.00                         Nov 08 1999
    syChain Ver 1.50am, syHw Ver 1.07am   Nov 09 1999
    libintr Ver 1.051                     Oct 28 1999

Search your own ROM for `Build:` and `Ver` and you will know which libraries you have before you start.

Where the SDK's module is a later version than the ROM's, it does not match, and there is no older SDK to take it
from. Those 41 modules were decompiled like the game was, and are in `src/lib`, one folder for each library. They
build to the ROM's exact bytes too. If your game is from about the same months, they may be the very modules you
are missing.

Things that were learned on the way, and that should hold for other games:

- **Each library has its own compiler options**, and they are not the game's. Sega built most of the NAOMI
  libraries with `-round=zero -align16 -division=cpu`, where Capcom used `-round=nearest`. The options are in
  `src/lib/LIBRARY/options`. A few modules inside a library differ again, and have a `MODULE.options` file:
  `nlmath` and `nlproj` need `-round=nearest`, or constants like `0.001f` come out one bit low.
- **Shinobi and Kamui2 use their own section names**, `-section=p=PSG,c=CSG,d=DSG,b=BSG`. That is why the
  linker's map groups them apart from the game's `P`, `C`, `D` and `B`.
- **Kamui2 was built with `-extra=a=800`**, an option that is not in the manual. It lines code up on 32 bytes and
  puts each routine's constants on the next 32-byte line. Every Kamui2 object looks like that. Capcom's own
  `-extra=a=400` is its neighbour. Two of Shinobi's modules (`kdapi_`, `mpapi_`) were built without optimisation,
  for speed.
- **Two builds of each library are in the zip**, `lib` and `lib/zero`. Some modules match from one and some from
  the other, so the link takes each module from whichever has the ROM's bytes.
- **Some modules were assembler to begin with.** Seven of `libnlobj`'s (the vertex and model code) are in the
  objects as assembler, and are `.src` files here. A few C modules have routines written with
  `#pragma inline_asm` (the `ftrv` matrix code in `nlproj`, the interrupt mask in Shinobi's `syIntCtrl_`). The
  compiler cannot build those to an object directly: it writes assembler source (`-code=asmcode`) and the
  assembler finishes the job. The Makefile does it that way.
- **`#pragma aligndata8( names )`** is another thing the manual leaves out. It puts the named variables on 8-byte
  lines, as Sega's matrix globals are. There is an `aligndata32` too.
- **`libjamma` is the one library the SDK has source for** (`libsrc/libjamma`). The three modules here follow it,
  changed where the ROM's older version differs.

The compiler's run-time library (`sh4nlfzn.lib`: division, block moves, `sprintf` and the like) links straight
from the compiler's zip, apart from two small modules.

## Contributing

As this is an exact byte-match, every contribution must keep the match: build, and check that the SHA-1 line still
says the ROM is the arcade's. Contributions should improve or explain the code: a better name, a field worked out,
a clearer way to write a routine that still compiles to the same bytes.

A "fake" match is source that builds the right bytes but is unlikely to be what Capcom wrote: an odd loop, an
assignment in a strange place, a local that is never used. The compiler is sensitive to such things, and some
routines only match with them. Changes should lower their number or explain them, with evidence or better code.

## License

GNU Affero General Public License v3.0 (`LICENSE`). `NOTICE.md` lists what the license does not cover.
