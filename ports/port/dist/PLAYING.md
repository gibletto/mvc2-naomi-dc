# Marvel vs. Capcom 2: playing

`mvc2.exe` runs the arcade game or the Dreamcast version; `mvc2dc.exe` only the Dreamcast's. No game
data comes with it.

## The game's data

- The arcade: MAME's `mvsc2` set, unzipped into `roms/mvsc2` beside the executable.
- The Dreamcast: the files of your own USA disc (`1ST_READ.BIN` and the rest) in `disc`.

Other folders can be set in `mvc2.cfg` (`arcade = ...`, `dc = ...`) or given on the command line
(`mvc2 --dc D:\games\mvc2-disc`). With both found it asks which to play, once. If something is missing,
it says what and where; put it there and press Enter.

## Keys

|                 | Player 1        | Player 2        |
|-----------------|-----------------|-----------------|
| move            | arrows          | I J K L         |
| punch (L, H)    | A S             | R T             |
| assist 1        | D               | Y               |
| kick (L, H)     | Z X             | F G             |
| assist 2        | C               | H               |
| start           | 1               | 2               |

5 is a coin, 9 service, F2 the arcade's test menu.

- F1: the controls screen (or Back with the right shoulder held). Enter sets a control from the next key
  or button, Delete puts it back, Escape closes. It also switches the renderer and the picture quality.
- F3: frame times.
- F9: switch between the arcade and the Dreamcast.
- F10: scaling (aspect, integer, stretch). F11 or Alt+Enter: full screen.
- Escape: quit.

Controllers: the first plugged in is player 1's. X Y punch, A B kick, the shoulders assist 1, the triggers
assist 2, Back a coin.

## Settings

`mvc2.cfg` beside the executable has the settings, each explained there. On a slow PC, `render = half`
is much faster; `renderer = gpu` draws with the graphics card.

`unlock = all` lets you choose every character, colour and stage. Your saves keep only what you really
unlocked, but games played this way still count in the records.

## Sound

The AICA's own driver runs on an emulated ARM7 with its DSP; the Dreamcast's ADX music is decoded by the
port. `MVC2_SOUND=0` turns it off; `MVC2_WAV=file` records it.

## Saves

The Dreamcast's memory card is `vmu_save_A1.bin` beside the executable, the same file Flycast uses. The
arcade keeps its settings and book-keeping in `eeprom.bin` and `backup.bin` there.
