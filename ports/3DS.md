# Marvel vs. Capcom 2 on the 3DS

The 3DS port is a work in progress. It runs both versions, but on an Old 3DS fights are only about 10 fps
so far.

The arcade game and the Dreamcast version, built for the original 3DS and 2DS (a New 3DS also gets its faster
CPU). No game data comes with it: you need the MAME `mvsc2` set, the Dreamcast USA disc's files, or both.

## Installing

Copy the zip's `sd` folder to the root of the SD card, then install `mvc2.cia` with FBI, or start
`mvc2.3dsx` from the Homebrew Launcher. On an Old 3DS or 2DS use the CIA: the 3dsx has only the memory for the
Dreamcast version there. A New 3DS runs near full speed with the CIA.

Sound needs the usual `sdmc:/3ds/dspfirm.cdc`.

## The game's data

    sdmc:/3ds/mvc2/
        mvc2.3dsx
        roms/mvsc2/     the mvsc2 set, unzipped
        disc/           the USA disc's files (1ST_READ.BIN and the rest)
        mvc2.cfg        settings
        mvc2.log        the last run's log (mvc2_o3ds.log for the Old 3DS test build)

The first start joins the ROM set into one file, `roms/mvsc2/mvsc2.bin` (136 MB); the set's other files can be
deleted after. With both versions there it asks which to play (A arcade, B Dreamcast) and keeps the answer in
`mvc2.cfg` as `mode =`.

## Controls

| Button            |                                   |
|-------------------|-----------------------------------|
| D-pad, Circle Pad | move                              |
| Y X               | light, heavy punch                |
| B A               | light, heavy kick                 |
| L (or ZL)         | assist 1                          |
| R (or ZR)         | assist 2                          |
| START             | start                             |
| SELECT            | coin                              |
| SELECT + START    | switch to the other version       |
| SELECT + L        | the arcade's test menu            |
| SELECT + B        | quit                              |

On the Dreamcast, Y X L B A R are its X Y L A B R.

## Settings

`mvc2.cfg` has one `key = value` a line:

- `scale = stretch` fills the top screen (the default is 1:1, 320 by 240).
- `render = exact` draws at 640 by 480 and halves it: sharper, slower.
- `arcade =` and `dc =` set other data folders.

## Building it

See the 3DS section of `ports/README.md` in the source. The Old 3DS test build runs on a New 3DS as an Old 3DS does.
