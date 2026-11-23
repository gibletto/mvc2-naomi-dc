# Marvel vs. Capcom 2 on the MiSTer

The arcade game and the Dreamcast version, run on the DE10-Nano's ARM and drawn by the Pinedra GPU core. Pinedra
GPU must be installed first (release 20261115 or later); it is not part of this package, and no other core is needed.

- Pinedra GPU: [download and install it](https://www.patreon.com/pinedragpu/posts/pinedra-gpu-for-175129847) as its
  author describes on [Patreon](https://www.patreon.com/pinedragpu).

## Installing

Copy `mvc2-mister-VERSION.tar.gz` and `mvc2_setup.sh` into `/media/fat/Scripts`, then run `mvc2_setup` from the
MiSTer's Scripts menu. It says which Pinedra it found, puts the game's files in place, adds a `[MVC2 Pinedra]`
section to MiSTer.ini (after keeping a dated copy of it, `MiSTer.ini.mvc2-DATE`) and says which game data it
found. Run it again to update; `mvc2_setup.sh --uninstall` removes it all except the game's data and saves.
The menu entries load Pinedra's core, `pinedra/config/Pinedra Collection.rbf`; with a Pinedra from before release
20261122, which called it Pinedra Config, they load that one, and after updating Pinedra, run `mvc2_setup` again.

Besides that section and the copy, it writes only these:

    /media/fat/MiSTer_MVC2                  the MiSTer main that starts the game on Pinedra's core
    /media/fat/_Other/MVC2 Arcade (Pinedra).mgl      the menu entries
    /media/fat/_Other/MVC2 Dreamcast (Pinedra).mgl
    /media/fat/Scripts/mvc2_setup.sh
    /media/fat/games/mvc2/                  the game (mvc2-pinedra, bin/), this README, licenses/, logs/

## The game's data

No game data comes with it:

- the arcade: MAME's `mvsc2.zip` in `games/mvc2/roms/mvsc2` (or its files, unzipped there). The first start
  unpacks the zip into `mvsc2.bin` beside it (136 MB, with a bar on the OSD);
- the Dreamcast: your own USA disc in `games/mvc2/disc`: a `.chd`, a `.gdi` with its tracks, a `.cue` with its
  `.bin`s, an `.iso` of track 3, or its files (`1ST_READ.BIN` and the rest). The first start unpacks an image
  there (210 MB).

In the menu, under _Other, "MVC2 Arcade (Pinedra)" starts the arcade game and "MVC2 Dreamcast (Pinedra)" the
Dreamcast version. If what one needs is missing, it says so on the screen and goes back to the menu
(`games/mvc2/logs/last-run.log` has the details).

## Controls

Set the buttons once in the OSD (System, Define joystick buttons), as for any core. A six-button pad or stick
plays as the cabinet: Y X L over B A R.

| Button      |                     |
|-------------|---------------------|
| D-pad       | move                |
| Y, X        | light, heavy punch  |
| B, A        | light, heavy kick   |
| L, L2       | assist 1            |
| R, R2       | assist 2            |
| Start       | start               |
| Select      | coin                |
| L3          | the arcade's test menu (difficulty, timer, free play) |
| R3          | service             |

The first pad is player 1's, the second player 2's.

## The OSD

F12 (or the pad's OSD button) opens it. Its options are Pinedra's own, as for its games: the analog output
(31 kHz 480p or 15 kHz), interlaced or progressive at 15 kHz, 60 or 50 Hz (the game is made for 60), and which
output the frame rate follows. For a 15 kHz CRT, Pinedra Collection's "Enable 240p mode" (its Settings page) draws the
game in 240 lines, shown without interlace. A line in `games/mvc2/env.txt` sets it for this game alone:
`PND_240P=1` on, `PND_240P=0` off. The game's own settings (difficulty, timer and the rest) are in its options or the
arcade's test menu, and kept in `games/mvc2`.

A TV with only S-Video or composite: `vga_mode=svideo` or `vga_mode=cvbs` in MiSTer.ini (with `vga_scaler=0`), and
15 kHz in the OSD. Pinedra's core makes the colour (release 20261115 or later; not yet tried on a real set).

## The ARM's clock

While the game runs, the Pinedra driver holds the DE10-Nano's ARM at 1000 MHz (release 20261121 or later) and
puts back the clock it found when the game ends. At 1000 MHz the arcade game's select and fights keep 60 frames a
second; at the MiSTer's own 800 MHz a few frames are lost, most of them while a pick loads. A board that doesn't
take 1000 MHz (the game hangs or the MiSTer stops answering): `PND_CPU_MHZ=800` in `games/mvc2/env.txt`.

## Quitting

Hold Start and Select for two seconds: the game ends and the MiSTer's menu comes back. Or choose Core in the
OSD's System page and load another.

## Licences

The game's port is under the licence in `licenses/LICENSE.txt`. MiSTer_MVC2 is built from Main_MiSTer and is under
the GNU AGPL 3.0; its source is in `licenses/MiSTer_MVC2`. Pinedra GPU is © its author and is not included.

## How it fits together

- The menu entries `_Other/MVC2 Arcade (Pinedra).mgl` and `MVC2 Dreamcast (Pinedra).mgl` load Pinedra's core as
  "MVC2 Pinedra" (one setname: one MiSTer.ini section, one button map); MiSTer.ini's `[MVC2 Pinedra]` has the MVC2
  wrapper (`MiSTer_MVC2`) start the game there. Each names its version in a `<file>`
  path (`arcade`, `dc`), which the wrapper passes on as `--arcade` or `--dc`. When the game ends with 10 + a
  `PICK_` reason (its data missing or unreadable), the wrapper says so on the OSD, and while the first start
  unpacks the data it shows the progress the game puts on the shared page. It reads the core's video mode every
  500 ms as MiSTer's own main does, which sets the S-Video and composite colour up.
- The picture: each frame's lists drawn by rd_gl1 and swapped at the core's vblank. `MVC2_GL1` sets its options
  (default `less pool order clut`). `order`: the translucent list goes to the Pinedra driver in the game's own order
  as window-space vertices, and the driver orders and cuts it as the PVR did, on its thread on the second core;
  the pictures differ from rd_gl1's own order by at most 3 of 255 on a few pixels. `clut`: a paletted texture goes
  as its indices and colours and the driver makes its texels, so the sprite pool isn't needed; the same pictures.
  Both need Pinedra release 20261123 (driver 0.5.44) or later, and `MVC2_GL1="less pool"` turns them off. At
  800 MHz (`PND_CPU_MHZ=800`) the launcher leaves `order` out: the driver's thread would fill the second core in
  the character select.
- The sound: made on a thread of its own, resampled to 48 kHz and written to `/dev/MrAudio`, the ring the core
  plays; a read of the device gives the bytes it has left, and the sound is stretched by up to 0.5% to keep three
  frames there.
- The controls: the wrapper's pads from its shared page (`MVC2_JOY_SHM`), else `/dev/input/event*`: a keyboard as
  the PC's keys and pads by place, as on the PC; Start and Select held two seconds quits (Start and Coin on the
  wrapper's pads). SIGTERM ends the run.
- The log (`games/mvc2/logs/last-run.log`) has the start's lines and any error, written on a thread of its own:
  the card is mounted sync. `MVC2_STATS=n` adds each part's time a frame every n frames, the memory and rd_gl1's
  work, and a line for each frame over 50 ms (a run of `MVC2_FRAMES` has them every 300).

Test switches, as on the PC: `MVC2_FRAMES`, `MVC2_SHOT`, `MVC2_PRESS`, `MVC2_WAV`, `MVC2_SOUND=1`. And
`MVC2_MRAUDIO=0` (the sound made but not played), `MVC2_SNDTHREAD=0` (made on the game's thread), `MVC2_VSYNC=0`
(paced by the clock), `MVC2_FRAMELOG=file` (each frame's parts), `MVC2_PROF=file` (the CPUs sampled,
as address counts for `nm`).

## Building it

`tools/mister/build.sh VERSION OUTDIR` builds the package as the releases are made: the game in `tools/mister/Dockerfile`'s
toolchain (Debian bullseye's armhf gcc, for the MiSTer's glibc 2.31), MiSTer_MVC2 with `build-hps.sh`, then `package.sh`.
