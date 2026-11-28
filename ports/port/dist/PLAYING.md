# Marvel vs. Capcom 2: playing

`mvc2.exe` runs the arcade game or the Dreamcast version; `mvc2dc.exe` only the Dreamcast's. On Linux the
`.AppImage` runs both. No game data comes with it.

## The game's data

- The arcade: MAME's `mvsc2.zip` in `roms/mvsc2` beside the executable (or its files, unzipped there). The first
  start unpacks the zip into `roms/mvsc2/mvsc2.bin` (136 MB).
- The Dreamcast: your own USA disc in `disc`, as an image or as its files:
  - a `.chd`;
  - a `.gdi` with its track files;
  - a `.cue` with its `.bin` tracks (Redump's set);
  - an `.iso` of track 3, or track 3's `.bin` on its own;
  - or the files already extracted (`1ST_READ.BIN` and the rest).

  The first start unpacks an image's files into `disc` (210 MB). The image can be deleted after.

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

5 is a coin, 9 service. F2 opens the arcade's test menu: its game data and settings (difficulty, timer,
free play and the rest). Arrows choose, Enter sets, Escape goes back; SAVE & EXIT keeps them for the next game.

- F1: the controls screen (or Back with the right shoulder held). Enter sets a control from the next key
  or button, Delete puts it back, Escape closes. It also sets the renderer, the picture, the sprites, the HUD
  and full screen.
- F3: frame times.
- F9: switch between the arcade and the Dreamcast.
- F10: scaling (aspect, integer, stretch). F11 or Alt+Enter: full screen.
- Escape: quit.

Controllers: the first plugged in is player 1's, the second player 2's.

| Button                 |                    |
|------------------------|--------------------|
| d-pad, left stick      | move               |
| West (X), North (Y)    | light, heavy punch |
| South (A), East (B)    | light, heavy kick  |
| left or right shoulder | assist 1           |
| left or right trigger  | assist 2           |
| Start                  | start              |
| Back                   | coin               |

## Settings

F1 sets the picture while you play. `mvc2.cfg` beside the executable keeps every setting, each explained there.

### Renderer (`renderer`)

- **GPU** (`gpu`, the default): the graphics card draws the game larger than the arcade's 640 by 480, so 3D
  stages, polygons and effects come out sharp at your screen's resolution. It needs Vulkan or Direct3D 12.
- **GL** (`gl`): for an older card with neither, OpenGL draws the game (OpenGL 2.1 is enough), larger too, but
  without the GPU's aa, sprites and HUD settings. The game uses it by itself when the GPU can't start.
- **Soft** (`soft`): the CPU draws the arcade's own 640 by 480, as the board did. For a PC whose graphics card
  can't run the game; the game uses it by itself when neither of the others can start.

F1's RENDERER row shows the one drawing, and goes through GPU, GL and SOFT.

### Picture, on the GPU or GL (`resolution`, `aa`)

The game is drawn at a whole multiple of 640 by 480, then filtered down to the window. F1's PICTURE row shows the
size it draws at in your window. Each step up draws more pixels, so a slower card should stay at **auto**.

| F1             | mvc2.cfg                     | drawn at, full screen at 1080p | at 4K (2160p)  |
|----------------|------------------------------|--------------------------------|----------------|
| auto           | `resolution = auto`          | 1920 by 1440                   | 3200 by 2400   |
| auto, aa 2x    | `aa = 2`                     | 3200 by 2400                   | 5760 by 4320   |
| auto, aa 3x    | `aa = 3`                     | 4480 by 3360                   | 7680 by 5760   |
| auto, aa 4x    | `aa = 4`                     | 5760 by 4320                   | 7680 by 5760   |
| 4.2x           | `resolution = 4.2`           | 2688 by 2016                   | 2688 by 2016   |
| 1x, the arcade's | `resolution = 1`           | 640 by 480                     | 640 by 480     |

aa smooths the edges by drawing larger still; 7680 by 5760 (12 times) is the most. `resolution` takes any
multiple from 1 to 12. GL draws at the same sizes without the aa; on a slow card, `resolution = 1` asks the least
of it.

### Picture, in software (`render`)

| F1           | mvc2.cfg            | what it does                                                     | time a frame |
|--------------|---------------------|------------------------------------------------------------------|--------------|
| exact        | `render = exact`    | every pixel as the arcade's PowerVR2 draws it                    | all of it    |
| fast         | `render = fast`     | the nearest texel, no texture smoothing                          | about 3/4    |
| lite+fast    | `render = lite+fast`| fast, drawn with integers: very close to exact, a little quicker | about 2/3    |
| fast+half    | `render = fast+half`| drawn at 320 by 240, each pixel shown as four; small text blurs  | about 1/4    |

`mvc2.cfg` also takes `lite` on its own (texture smoothing kept), `half`, `nofog` (no fog) and `nosort`
(see-through effects not sorted per pixel). The CPU draws on all its cores.

### Sprites (`sprites`, GPU only)

How the characters and other flat pictures look when drawn larger:

- **sharp** (default): the characters keep their pixels, crisp at any size.
- **smooth**: their pixels smoothed.
- **pixel**: no smoothing at all.
- **arcade**: the arcade's own filtering.
- **texel**: the characters' own pixels, each the same size. With `scale = integer` (the default full screen on a
  4K screen), the game is shown pixel for pixel, 2688 by 2016. `texel_grid = shared` (the default) puts every sprite on one grid of the
  characters' pixels, so a character's parts meet with no slivers; `object` puts each on its own.

### HUD (`hud`, GPU only)

The timer, gauges, words and menus:

- **arcade** (default): in the arcade's own pixels.
- **smooth**: smoothed.
- **sharp**: drawn as the sprites are.

### The window

- F10, or `scale`, sets how the picture fits the window:
  - **aspect**: as large as it fits, at 4:3.
  - **integer**: whole multiples only, pixel exact.
  - **stretch**: the whole window.
- Full screen: F1's FULL SCREEN, or `fullscreen`:
  - **off** (default): a window.
  - **desktop**: the whole screen, at the desktop's resolution and refresh rate.
  - **exclusive**: the whole screen in a display mode of the game's own: the desktop's resolution, at the highest
    refresh rate that is a whole number of times the game's 60 frames a second (240 or 120 Hz, else 60), for the
    least delay; `refresh = 60` asks for 60 Hz. Alt+Tab gives the desktop back its own. Not on Wayland, where it is
    desktop. With variable refresh (G-Sync, FreeSync) on, the display follows the game's own frames a second (`rate`);
    without, each frame is shown for the same number of refreshes, the game a hair faster (60.02 at 240 Hz) so none
    is shown longer. `vrr = auto` asks NVIDIA's driver; with another card, set `vrr = off` for a display without
    variable refresh (or with it turned off), `on` for one with.
- F11 or Alt+Enter: full screen on and off, as the kind chosen last (kept as `fullscreen_kind`, so the next start
  too).

### Unlock

`unlock = all` lets you choose every character, colour and stage. Your saves keep only what you really
unlocked, but games played this way still count in the records.

### Console (Windows)

`console = yes` opens a console beside the game with what it reports: the data it found, the controllers,
and why it stopped if it does. It is off by default. Started from a terminal, the game writes there anyway; to
keep it in a file, `mvc2.exe > log.txt 2>&1` in cmd, or `.\mvc2.exe 2>&1 | Out-File log.txt` in PowerShell.

## Sound

The AICA's own driver runs on an emulated ARM7 with its DSP; the Dreamcast's ADX music is decoded by the
port. `MVC2_SOUND=0` turns it off; `MVC2_WAV=file` records it.

## Saves

The Dreamcast's memory card is `vmu_save_A1.bin` beside the executable, the same file Flycast uses. The
arcade keeps its settings and book-keeping in `eeprom.bin` and `backup.bin` there.

## Linux and a Steam Machine

The Linux build is one file, `MvC2-VERSION-i686.AppImage`, a 32-bit x86 program. It needs only the system's 32-bit
C library, FUSE (to open the AppImage) and, for the GPU renderer, its 32-bit Vulkan driver (for GL, its 32-bit
OpenGL one): SteamOS and Steam install them. Unpack the archive in a folder you can write to (your home folder):
the game's data, `mvc2.cfg` and the saves stay beside the AppImage.

On a Steam Machine or Steam Deck, in Desktop Mode: unpack it (say into `~/Games/mvc2`), put the game's data in
`roms/mvsc2` and `disc` there, then in Steam choose Games, "Add a Non-Steam Game to My Library", Browse, and pick
the `.AppImage`. Back in Game Mode it is in the library; the controller works as on a PC. To start one version
directly, put `--arcade` or `--dc` in its Launch Options.
