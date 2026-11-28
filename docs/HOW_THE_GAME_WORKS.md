# How Marvel vs. Capcom 2 works

A tour of the Marvel vs. Capcom 2 arcade program, written from this source. It explains how the game hangs together:
what happens in each frame, how a button press becomes a fireball, how six fighters share one fight, and why the code is
shaped the way it is. You don't need to read C to follow it. The code samples are for those who do, and every name in
`code font` is a real name you can search for in the source.

A few things to know before you start:

- **Frame** means one video frame: one picture on the screen, about 1/60 of a second. Almost everything in the game is
  counted in frames.
- **The board draws polygons, but the fighters are still sprites.** The game runs on Sega's NAOMI board and never
  touches the video or sound hardware itself. It calls Sega's library, which draws textured polygons (flat shapes with a
  picture on them). The stages are 3D models. The fighters are flat pictures, cut into small textures. Their data still
  measures in the dots of Capcom's older boards (384 across, 224 down), and the program scales every distance up to this
  board's screen.
- **The names.** Names like `pl_char_set`, `act_st_set`, `vs_sys_work`, `BLACK_COMBO` and `piyo` are Capcom's own. They
  were found in text left in the unused space of the retail ROMs, and so were the names of source files such as
  `flashing.c` and `em_play.c`. The rest are ours, written in the same style. A field called `unk_` and a number is one
  we don't understand yet.

---

## The big picture: one loop per frame

The whole game is one loop in `main` (`src/main.c`). It goes round once per frame:

```c
while( 1 ){
	nlWaitVSync() ;		/* Sega's library: the frame's drawing goes out */
	credit_str_set() ;	/* the "CREDIT(S)" text */
	book_coin_ck() ;	/* count new coins in the book-keeping */
	sound_vsync() ;		/* keep the streamed music going */
	System_Main() ;		/* the game: one frame of whatever scene is on */
	vms_main() ;		/* the memory cards */
	SysTrans() ;		/* run the drawing the game queued, print its text */
	vsync_wait() ;		/* wait for the vertical blank */
	load_main() ;		/* read a little more from the ROM board */
}
```

(A few lines are left out.) The game never draws a dot. `System_Main` moves a few hundred small records (C structs) and
queues drawing routines. `SysTrans` then runs those routines, and they hand Sega's library a list of sprites and models
to draw. There is no timing code in the game itself: a walk is so many dots a frame, a punch lasts so many frames, and
the loop runs at the screen's rate.

### The vertical blank

The vertical blank is the short gap after one picture has been drawn and before the next one starts. An interrupt is a
hardware signal that pauses the running code and runs a fixed routine. The game hooks two routines to the vertical
blank's interrupts:

- `VBlankInIrq` (`src/system.c`) counts the frame (`Sys->vblank_cnt`) and reads the controls (`jamma_read`). So the
  sticks and buttons are sampled once a frame, at the same moment every frame, whatever the game is doing.
- `vblank_func` (`src/vib.c`) sets `vsync_flag`, which ends `vsync_wait`, and sends what must change between pictures:
  the palettes, the fog table and any textures a scene asked for.

---

## Boot-up: two programs in one ROM

The program ROM starts with a header that the board reads (`src/bootid.src`). It names the game in each region and says
where two programs lie in the ROM: the game at 0x1000, which is everything in `src/` outside `src/test/`, and the test
program at 0x3C0000, the operator's menus in `src/test/` (see the end of this tour).

Both load into the same part of memory, so only one of them is ever running. Between the two, at 0x250000, sits the
fighting data of every character (more on that below).

Before its loop, the game's `main` calls `system_init`, `warning_disp` and `System_Init`. `system_init` (`src/system.c`)
is shared with the test program. It:

1. Sets up the screen through Sega's library and prints "LOADING GAME NOW...".
2. Looks for the JAMMA I/O boards, which carry the sticks, buttons and coin switches. If none answers, or they don't
   have enough switches for two players, `jamma_err` is set.
3. Reads the book-keeping and the operator's settings from the backup memory, which keeps them with the power off
   (`sys_set_load`). A bad block is replaced by the defaults for the board's country (`sys_set_tbl`).
4. Loads the first files from the ROM board (the common sounds, the characters' fighting data, the fonts and the system
   textures) and hooks the two vertical blank routines.

`warning_disp` shows the title text and then the warning screen, 180 frames each. If `jamma_err` is set it stops here
for good, on "JAMMA I/O BOARD NOT FOUND." or its sister message. `System_Init` (`src/sys_task.c`) then copies the
settings into the system work and marks the characters that are open from the first day.

### Loading from the ROM board

The pictures, models, music and sound packs aren't in the program. They sit in the cartridge's other ROMs as 834
numbered files, and `stream_tbl` (`src/am_load.c`) gives each one's address and size. The game reads them in two ways:

- `file_load` reads a whole file and waits for it. This is fine at boot and between scenes.
- A queue reads in the background. `fight_load_start` queues the six fighters' files, their sound packs and the stage's
  two files. Then `load_main`, the last call of the frame loop, reads as many 2K blocks as the frame has time left for.
  The scene on the screen keeps running, and asks `load_ck` whether the queue is empty yet.

The second way is what the screen before a fight is for. `StartDemoMain` (`src/start_demo.c`) shows the two teams and
starts the queue, and `play_init` waits on `load_ck` before it sets the fight up.

A character's file holds its pictures: the cells its sprites are built from, the sprites and the colours. Its *fighting*
data is in the block that was loaded at boot. That is packed, and `pl_data_set` unpacks it with `lz_unpack16` into nine
parts: the animations (`char_adrs`), the box sets and boxes (`frame_box_tbl`, `hit_box_tbl`), the attack rows
(`hit_data_tbl`) and five tables for the computer's play. In this source those nine parts are the files in `data/`.

---

## Tasks and scenes: how the game knows which screen it's on

The game is a stack of state machines, all built the same way: a small number says which step you're on, and a table of
function pointers says what to do there. Here is the top one, from `sys_task` in `src/sys_task.c`:

```c
static void ( *sys_task_jmp[] )( void ) = {
	task_init,
	task_attract,
	task_game,
	test_mode_jump,
} ;
...
sys_task_jmp[Sys->task_no]() ;
```

To change screen, code writes a new number into `Sys->task_no` (or a number below it) and the table does the rest next
frame. The numbers nest:

| Number | Whose table | What it picks |
|---|---|---|
| `Sys->task_no` | `sys_task` | the task: set-up, attract mode, a game, or the jump to the test menu |
| `Sys->task_step` | the task's | attract: which screen of the loop. Game: `game_init`, then `game_move` |
| `Sys->scene_no` | `game_move` (`src/game.c`) | the scene of a game: select, the fights, continue, the staff roll, game over |
| `Sys->routine_0` | `game_main` (`src/s_play.c`) | inside the fights: set-up, the computer's team, the start demo, the fight, the ranking |
| `Sys->routine_1` | the fight's | `play_init`, `play_start`, `play_move`, `play_end` |

The same pattern goes all the way down. Every fighter and every fireball has its own `routine_0` to `routine_3`, and
each indexes a table of routines in the same way. Timers follow one pattern too: set a counter, count it down once a
frame, and move on at zero. "Show this for three seconds" is `Sys->timer0 = 180`.

### Attract mode

`task_attract` (`src/sys_mode.c`) is the loop the cabinet plays when nobody is playing: the logo, the opening, the
title, the copyright screen, a demo fight and the rankings, then round again. Each is a step of `Sys->task_step`.

- Every frame of it stirs both random number generators (`nlRand` and the game's own `Rnd`), so the first fight of the
  day isn't the same each morning.
- The demo fight uses one of four set line-ups of six characters (`demo_pl_tbl`). It is a real fight, run by the same
  routines as a paid one, with the computer on both sides. Every frame it tops all six fighters back up to full vital,
  so nobody wins, and after 1080 frames it fades out.

`start_ck` watches the start buttons. When one is pressed and `credit_enough_ck` agrees, it clears the six fighters,
notes the side in `Sys->entry_side`, and sets `Sys->task_no = 2`: a game.

### A game

`task_game` (`src/game.c`) runs `entry_main` every frame, whatever the scene. `entry_main` (`src/entry.c`) looks after
each side on its own: an empty side watches its start button, a side that has lost counts its continue down, a side with
a score worth keeping enters its name. That is why a second player can join in at almost any moment: the check isn't
part of any one scene.

The scenes, by `Sys->scene_no`:

1. **Select** (`SelectMainMove_yu`, `src/select.c`). Each side picks three characters, and for each one of three assist
   types (`pl_personal`), then the speed, normal or turbo. A clock runs, and when it runs out the game picks for you.
2. **The fights** (`game_main`): the start demo, the fight, and the ranking screen if the game ended there.
3. **Continue** (`cont_main`). The count runs from 9, two seconds a number, and an attack button hurries it along.
4. **The staff roll** (`staff_main`), when `Sys->ending_flag` is set.
5. **Game over** (`over_main`), then back to attract mode.

A one-player game is eight fights. Each player is given one of four routes through the stages (`stage_tbl` in
`src/sel_decide.c`), and all four end on stage 8, where `Sys->boss_flag` is set and the boss comes back twice in new
forms. A two-player fight picks its stage at random.

---

## Shared state: the game's global variables

Most of the game's state is in a few large structs. They are defined in the headers that `include/player.h`
includes (`pl_work.h`, `system.h`, `vs_sys.h`, `scr.h`) and listed in `include/work.h`:

- **`sys_work`**, a `SYSTEM_WORK`, always reached through the pointer `Sys`. The scene numbers above, the general timers
  `timer0` to `timer3`, who is playing (`play_side`, a bit for each side), `vs_flag` (two humans), `stage_cnt` (which of
  the eight fights), and a copy of every operator setting.
- **`pl_work[6]`**, six `PLAYER_WORK`s, one for each fighter. Even numbers are the first side's three, odd numbers the
  second side's. `pl_work[0]` and `pl_work[1]` also carry what belongs to a *side*: its score, its name for the
  rankings, its continue count.
- **`vs_sys_work`**, a `VS_SYS_WORK`, also reached as `vs_sys_w`: the state of one fight. Who is in which place of each
  team (`team_w`), the clock (`round_time`), the fight's step (`vs_st`), both hyper combo gauges, the combo counts.
- **`sw_work`**: the controls, one `SW_WORK` for each side (next section).
- **`col_work`**: the palettes, 128 lines of 16 colours.
- **`book`**: the book-keeping, saved in the backup memory. Coins, games played, play time, and which characters have
  been opened.

The operator's settings are copied into `Sys` by `System_Init`: `renda_level` (DIFFICULTY on the test program's GAME
CONFIGURATION screen), `damage_level`, `time_speed`, `speed_fix` and the rest. They come up below where they are used.

Coins are counted and turned into credits by Sega's library, and the game only asks whether a side can pay
(`credit_enough_ck`). But it keeps a count of its own, **points**. `point_add` (`src/system.c`) adds them for coins and
for time played. Each time the total passes the next line of a table, a hidden character can be opened: one is picked at
random from a group and its bit is set in `Sys->char_open0` or `Sys->char_open1`. Twenty-eight characters start with
their bit set, and the select screen only offers a character whose bit is set or one that a player's memory card brings.

---

## Inputs

`jamma_read` runs in the vertical blank. It asks Sega's library for the I/O boards' switches and builds one `SW_WORK`
for each side. An `SW_WORK` keeps last frame's switches (`old`) beside this frame's (`now`), and works out the two
edges: `on` for the switches that went down this frame, `off` for those that came up.

One word holds everything, with a bit for each switch (`include/key.h`): `KEY_START`, the stick (`KEY_UP`, `KEY_DOWN`,
`KEY_LEFT`, `KEY_RIGHT`), light and heavy punch (`KEY_LP`, `KEY_HP`), light and heavy kick (`KEY_LK`, `KEY_HK`) and the
two assist buttons (`KEY_A1`, `KEY_A2`). So the game has four attack buttons, not six. A medium attack is the second
press of a light button, as the section on fighters explains.

`pl_sw_copy` gives each fighter its side's switches every frame, in `pad_sw`. Then `pl_sw_set` (`src/pl_sub.c`) turns
them into what the fight code reads. It makes two sets of words, and the difference matters:

- `pl_new_lvbt` (held) and `pl_now_lvbt` (just pressed) keep left and right as the cabinet has them.
- `pl_sw_new`, `pl_sw_now` and `pl_sw_old` are **flipped when the fighter faces the other way**. In these, the stick
  bits mean forward and back, which is why `key.h` gives the same two bits the second names `KEY_FORWARD` and
  `KEY_BACK`. Every special move is written once, facing one way, and works from either side of the screen.

Only the fighter on point reads the controls. If the side isn't human (`cpu_flag`), `em_play` runs first and writes its
own switches into `pad_sw`, and they go through exactly the same code: the computer plays with a virtual stick.

---

## Drawing: the game queues, Sega's library draws

A scene doesn't draw as it goes. During its frame it works out what should be on the screen, and at the end it queues a
routine with `SysTransSet`. The fight queues `game_disp` (`src/disp.c`). After the scene has finished, `SysTrans` runs
what was queued, and that is where the calls to Sega's library happen. Text works the same way: `STrPrint` puts a line
in a list, and `SysTrans` prints the list.

### Sprites

A fighter's picture is built from three things in its file. A **sprite** is a list of **cells**, each with a place, a
palette line and flips. A cell is a small packed picture, a multiple of 8 dots each way, with 16 colours. And the
animation frame names the sprite by number (`spr_no`).

The board has no sprite hardware, so each frame `sprite_tex_set` (`src/sprite.c`) does this for every work that will be
drawn: it unpacks the sprite's cells (`lz_unpack`), cuts them into square tiles of 8, 16 or 32 dots, and loads each tile
as a texture. `sprite_disp` later draws one textured square for each tile. A frame can use 384 tiles. There are two
banks of tile textures, and they swap each frame, so the one being loaded is never the one on the screen.

Every work has a draw priority (`prio`), and `disp_prio_set` files it under that priority. `game_disp` then walks the
priorities and gives each one its own depth, so the board draws them in the right order. The fighters' priorities are
worked out again each frame (`pl_prio_set`) from what each one is doing: attacking, being hit, in the air.

### Colour

Sprites are in 16-colour palettes, and `col_work` holds the lines in use. To change a fighter's look, the game rewrites
its lines and marks them with `trans_flag`, and `col_send` passes the marked lines to the library at the end of the
frame. `pl_color_set` (`src/pl_char.c`) is the usual way in. It can put the fighter's own colours back, halve their
brightness (for everyone but the fighter doing a hyper combo), or fill the lines with white for a flash.

### The stage and the camera

A stage is a 3D model with a list of moving objects, set up by `stage_set` (`src/stage.c`) with its own fog, background
colour and light. The fighters stand on a flat plane in that space, and the camera (`scr_work`, `src/scr.c`) is an eye
and a point it looks at. `scr_move` runs it each frame. In its usual modes, `scr_x_normal` keeps both point fighters on
the screen and sits halfway between them once they are far apart, and `scr_y_normal` rises with whichever of the two is
higher. That is how a super jump takes the camera up with it.

---

## Works: one struct for everything that moves

Everything on the screen during a fight is a **work**: a `PLAYER_WORK` (`include/pl_work.h`). The six fighters are
`pl_work[6]`. Everything else, every fireball, spark, dust cloud, hit mark, stage object and gauge, is taken from a pool
of 256 works (`work_buff`, run by `src/hit_work.c`).

An effect only uses the first part of the struct, the part before `act_st`. That part is the same for all of them:

| Fields | What they hold |
|---|---|
| `be_flag`, `pl_no`, `work_kind` | in use, which side, and what kind of work |
| `routine_0` to `routine_3` | the work's own state machine |
| `work_func` | the routine that runs it each frame |
| `master_w` | for an effect, the fighter that made it |
| `pos`, `spd`, `acc` | where it is, how fast it moves, how that changes |
| `cg_ctr`, `spr_no` and the other `cg_` fields | the animation frame it is showing |
| `atck_flag`, `dm_flag`, `hit_flag`, `atck_no` | whether it can hit or be hit, whether it has, and with what |

The pool is a linked list of free works. `work_get( base, list, mode )` takes one off it, clears it, and links it into
one of 14 lists. The list's number is also the work's kind. The caller then fills in `work_func`, and `work_list_move`
calls that routine once a frame until the work frees itself with `work_free`. The first call can come in the frame the
work was made. `work_list_move` walks its list from `work_top` at the moment it runs (`src/hit_work.c:60-70`), and
`all_work_move` moves the fighters before the effect and shot lists (`src/all_move.c:18-25`). So a fireball that a
fighter's routine makes runs its first step later in the same frame.

The lists keep kinds of thing apart, so the frame can run them in a fixed order, and they have names in
`include/hit_work.h`. Lists 1 to 4 are the fight's sprites: shots (`W_SHL`) and the fighters' effects (`W_PLS`), each
with a twin that keeps moving while a hyper combo stops the game. 5 is the stage's objects (`W_SET`), 7 the dust and
sparks (`W_EF`), and 11 what is drawn over the fight, such as the gauges (`W_SYS`). The last ten works of the pool are held back for callers that must not fail (`work_get2`).

Since a fireball is the same struct as a fighter, the same routines animate it, draw it and check its hits. Very little
code in the game is about one kind of object.

---

## The fight: one frame, in order

`play_move` (`src/s_play.c`) is one frame of a fight. With a few lines left out:

```c
again:
	vs_sys_w->slow_step = ( vs_sys_w->slow_step + 1 ) & 15 ;
	all_work_move() ;		/* move everything */
	hit_ck0() ;			/* who hit whom */
	vs_sys_move() ;			/* the clock, KOs, the fight's own steps */
	if( vs_sys_w->speed_rate && !vs_sys_w->speed_cnt ){
		vs_sys_w->speed_cnt = vs_sys_w->speed_rate ;
		vs_sys_w->speed_flag = 0xff ;
		goto again ;
	}
```

After that it files every sprite for drawing (`pl_work_disp`, `work_list_disp`) and queues `game_disp`.

`all_work_move` (`src/all_move.c`) is the "move everything" step, and its order is fixed:

1. `pl_work_move`: the six fighters (next section), then the tags and other team changes they asked for.
2. The effect lists 3, 4, 1 and 2 (`W_PLS`, `W_PLS_ST`, `W_SHL`, `W_SHL_ST`).
3. `scr_move`: the camera.
4. The remaining lists: the stage's objects and the rest.
5. `pl_hit_work_set`: the fighters enter themselves for the hit check.
6. The stage's own frame, the things drawn in front, and the fog.

Hits found this frame are recorded at once, but the fighters only react in their own move next frame.

### Turbo

Look at the `goto again`. The game has no faster animation data for turbo. Every so many frames it runs the whole fight
a second time before drawing. `speed_rate` is that "so many": 6 at normal speed and 4 at turbo. So normal speed is seven
game frames for every six pictures, and turbo is five for every four.

### Slow motion and the hyper combo freeze

**Slow motion.** `slow_step` counts round from 0 to 15, and `slow_bit` is a mask of 16 bits. On a frame whose bit is
set, the fighters don't move. A KO sets `slow_bit` to `SLOW_HALF` (every other bit), so the last hit plays at half
speed.

**The freeze.** A hyper combo sets its fighter's `hyper_stop_flag`, and `hyper_stop_set` copies it into
`vs_sys_work.game_stop` (`src/pl_sub.c:1372-1380`). While it is set, `all_work_move` runs only a few of the lists, the
screen is at half brightness, `hit_ck0` checks no hits and `hit_work_set` enters nothing (`src/hit.c:37`,
`src/hit.c:278`), the clock stands still, and `player_move` returns at once for the other side.

The fighter who caused it does not run its move either. `player_move` gives every fighter of that side only
`pl_frame_jump`, the step of the frames that carry a jump code (see "Hit stop" below), unless the fighter has
`stop_move_flag` set (`src/pl_main.c:159-165`). Its effects keep moving, because they are in the lists `W_PLS_ST` and
`W_SHL_ST` that run during the freeze (`src/all_move.c:27-32`). `stop_move_flag` is set for 16 frames on two
partners that come in: the variable counter's (`src/pl_team.c:354-356`) and the delayed hyper combo's, which gets
`team_type` 6 (`src/pl_team.c:443-446`). A character's own hyper combo can set it too, as Ryu's Shinkuu Hadouken does
when `team_type` is 6 (`src/s_pl00.c:1491-1496`).

The flash is a work in `W_PLS_ST` (`src/hyper_ef.c:15`). It counts `timer0` down from 44, and at the end it gives each
fighter of the other side 10 frames of hit stop, unless that fighter is being thrown, and ends the freeze with
`pl_hyper_stop_clr` (`src/hyper_ef.c:68-72`, `src/hyper_ef.c:84-101`). The freeze also ends early when its fighter
leaves `N_SUPER` and `N_CHANGE` (`pl_hyper_stop_ck`, `src/pl_sub.c:1401`).

---

## Fighters

### The state machine

A fighter's frame is `player_move` (`src/pl_main.c`). Three numbers say what the fighter is doing, each picking inside
the one before:

- `routine_0`: `PL_APPEAR` (0) while it makes its entrance, `PL_FIGHT` (1) in the fight.
- `routine_1`: the broad state. `PL_NORM` (0), `PL_CATCH` (1, throwing), `PL_CAUGHT` (2, being thrown) or
  `PL_DAMAGE` (3, taking a hit).
- `act_st`: in the normal state, the act. `act_st_set` writes it, and the values have names:

| `act_st` | Meaning |
|---|---|
| `N_STAND`, `N_WALK`, `N_CROUCH`, `N_TURN` | on the ground, free |
| `N_JUMP`, `N_SJUMP`, `N_AIR_DASH`, `N_FLY`, `N_WALL_JUMP` | in the air, by one means or another |
| `N_DASH`, `N_BACK_DASH`, `N_GUARD`, `N_AIR_GUARD` | dashing, guarding |
| `N_NML_ATCK`, `N_CMD_ATCK` | a normal attack, or the one done with down-forward |
| `N_SPECIAL`, `N_SUPER` | a special move, a hyper combo |
| `N_CHANGE` | coming in or going out: tags, assists, team moves |
| `N_DOWN`, `N_GET_UP`, `N_DIZZY`, `N_WIN` | on the floor, getting up, dizzy, the win pose |

Each frame in the fight goes like this:

1. If the computer plays this fighter, `em_play` presses its buttons. Then `pl_sw_set` reads the controls (point fighter
   only).
2. `pl_wait_com_ck` runs the character's command check. This is where a finished special-move motion is noticed,
   whatever the fighter is doing.
3. Slot 0 of the character's table runs, then `pl_renda_ck`, the mash check (`src/pl_main.c:244-251`).
4. **Hit stop.** If `hit_stop` is above zero it counts down, and the timers and the move are skipped. Buttons pressed
   during the freeze are remembered (`pl_stop_now`), so they aren't lost. The fighter is not wholly frozen: the next
   section says what still runs (`src/pl_main.c:253-261`).
5. The timers count down (`cancel_timer` and the rest), the table for `routine_1` runs, and in the normal state the
   table for `act_st`. Then the walls: the fighter is kept on the screen and inside the stage (`src/pl_main.c:262-292`).

### Hit stop

A hit stops both fighters. `pl_damage` gives the struck fighter and the attacking work the attack row's `hit_stop`
(`src/pl_dmg.c:176-177`), on a hit and on a guard alike, since it does so before it asks about the guard. When the
attacking work is a shot, its player stops too only if the shot's `atck_flag` has `HIT_OWNER` (`src/pl_dmg.c:178`).
`HIT_OWNER` also hands the player the shot's `hit_flag` and `em_w` (`src/hit.c:191-195`) and the `HR_GUARD` of a guard
(`src/pl_dmg.c:516`). Ryu's Hadouken does not set it (`src/shl00_00.c:211`), so Ryu goes on recovering while his
fireball and its target stop. Marrow's shot `shl16_03` sets it (`src/shl16_03.c:61`).

During its hit stop a fighter still runs its input, its command check and slot 0. Three more things happen
(`src/pl_main.c:253-261`, `src/pl_main.c:290-292`):

- **The jump code.** Unless the fighter is in `PL_DAMAGE`, `pl_frame_jump` runs (`src/pl_sub.c:1410-1432`). It does
  nothing unless the frame's `cg_prm` has its top bit (0x80). Then it steps the animation with `pl_char_move`, as a
  free frame would. If that moved the animation to a frame with another `cg_prm`, the low bits pick where it goes
  instead: with 0x40 clear, `cg_prm & 15` sends it back to the frame `( cg_prm & 15 ) - 1` frames before the one that
  carried the code (0x81 plays that frame again), and 0 lets it go on. With 0x40 set it skips forward
  `( cg_prm & 0x3f ) * 4` bytes. So an attack whose frames carry 0x80 keeps recovering through the hit stop, and the
  hit stop does not delay those frames. A routine that uses `cg_prm` as its own signal and clears it after `pl_char_move` takes
  the bit away. Captain Commando's standing heavy punch does this (`src/s_pl38.c:371-377`).
- **The shake.** `dm_ko_shake` moves a fighter in `PL_DAMAGE` one dot to and fro, by the bits of 0xCCCCCCCC picked by
  `hit_stop` (`src/pl_dmg.c:2375-2386`). Its comment speaks of a KO, but it shakes every struck fighter.
- **The walls.** The hit stop jumps to the label `wall`, so `pl_wall_ck` and `pl_wall_wide_ck` still run.

The hyper combo freeze uses the same `pl_frame_jump` for the side that caused it (see "Slow motion and the hyper combo
freeze").

### Deciding what to do

When a fighter is standing free, its act asks a fixed list of questions, and the first one that says yes wins. For
standing, in order: a super jump, a dash, the down-forward attack, a normal attack, a guard, a turn, a jump, a crouch, a
walk. Two of the rules behind them:

- **Guard.** `pl_guard_req_ck` starts a guard only when the stick is held away from the opponent *and* the opponent has
  an attack out. Holding back with nothing coming is just walking back.
- **Normal attacks.** `pl_nml_atck_ck` takes the button, then gives the throw the first chance: if the character's throw
  check finds someone in reach, the button becomes a throw.

**Chains and the missing medium buttons.** A light button gives a light attack. Each normal attack sets a small table,
`chain_tbl`, that says what each button gives *next* if it is pressed while this attack has connected. These are Ryu's:

```c
static Sint8 lp_tbl[4] = {  1,  2,  1,  2 } ;	/* after a light punch: lights give the medium, heavies the heavy */
static Sint8 mp_tbl[4] = { -1,  2, -1,  2 } ;	/* after a medium: only the heavies */
static Sint8 hp_tbl[4] = { -1, -1, -1, -1 } ;	/* after a heavy: nothing */
```

So the medium attack is a second press of a light button during a light attack that touched. That one table is both the
chain combo system and the answer to having four attack buttons for six strengths.

### One module per character

A character's code is one file, `src/s_plXX.c`, where XX is its number in hexadecimal (`s_pl00.c` is Ryu). Its fireballs
and other effects are in `shlXX_YY.c` and `plsXX_YY.c`, and its constants in `include/plXX_def.h`.

The shared code never calls a character's routine by name. Each module has a table of 28 routines in a fixed order, and
`plXX_tbl_set` copies it into the fighter's `pl_tbl` when the fighter is set up. From then on the shared code calls
*slots*:

| Slot | Ryu's routine | Called |
|---|---|---|
| 0 | `pl00_main` | every frame |
| 1 | `pl00_com_ck` | every frame: the command check |
| 2, 3 | `pl00_atck_set`, `pl00_atck` | when a normal attack starts, and each frame of it |
| 8 | `pl00_sp_atck` | each frame of a special move or hyper combo |
| 9 to 12 | `pl00_catch_ck` and three more | throws: the reach check, the start, the throw, the hold |
| 13 | `pl00_delay_ck` | during a partner's hyper combo, to see whether one of its own has been entered |
| 16, 17 | `pl00_sp_unison`, `pl00_sp_gattai` | the team hyper combo |
| 18, 19 | `pl00_sp_assist`, `pl00_sp_zero` | when called in as an assist, or as a variable counter |

The last few slots are spare, and a handful of characters use one for something only they have. Each fighter also has
`tlocal`, a small block of its own whose layout the module decides. Ryu's is `PL00_LOCAL_W`, and one of its fields,
`shot_flag`, will matter later. All 59 modules have the same skeleton, and most of their length is special moves.

### Teams

A side's three fighters all exist for the whole fight. `team_no` says where each stands (0 on point, 1 and 2 behind),
and `be_flag` says whether it is on the screen. Vital (the game's word for health) is 144 for everyone, in `vital_new`.
Characters differ in how much of a hit they take, not in how much vital they have.

A fighter never switches itself. It writes a request number into `change_req`, and after all six fighters have moved,
`team_change_ck` (`src/pl_team.c`) carries the requests out:

- **Tag.** Light punch and light kick together call the first partner, the two heavies the second (`pl_change_ck`). The
  point fighter plays its hand-over animation for 16 frames, then the partner jumps in from beyond the edge of the
  screen and takes `team_no = 0`.
- **Assist.** Tap an assist button: press it and let go within 16 frames (`pl_assist_ck`). The partner jumps in, lands
  and runs slot 18, the move its assist type picks. It can't be called while it is still recovering from its last visit
  (`vital_red_wait`). The landing is in `act_change_assist_move` (`src/pl_main.c:1637-1655`): the partner falls by its
  own speeds, and when it reaches the ground it stands (`Char_Stand` 0). If the point fighter is being thrown or hit by
  then (`routine_1` at `PL_CAUGHT` or above), the partner only stands and never attacks.
- **Snapback.** Down, down-forward, forward and an assist button, for one gauge level (`pl_snapback_ck`). If it hits,
  the fighter it struck is sent off the screen and the partner the button chose comes in instead.
- **Variable counter.** While guarding: back, down-back, down and an assist button, for one gauge level
  (`pl_vcounter_ck`). The partner appears and runs slot 19.
- **Team hyper combo.** Both assist buttons together (`pl_v_combi_ck`). It takes as many partners as there are gauge
  levels, at a level each, and each one runs slot 17.
- **Delayed hyper combo.** During a hyper combo, the next partner's slot 13 is asked whether the controls amount to one
  of its own hyper combos (`pl_delay_hyper_ck`). If so the first fighter leaves and the partner comes in already doing
  it.

**Red vital.** Three quarters of every hit is also taken off a second value, `vital_red`, which therefore stays above
`vital_new`: the gap is the red part of the bar. A fighter that is off the screen gets a point of vital back about every
50 frames (`vital_red_recover`), up to `vital_red`.

---

## Special moves, hyper combos and animation

### Commands

A special move's input is a short table. This is Ryu's Hadouken, from `src/s_pl00.c`:

```c
static Uint16 com_hadou[] = { 3, COM_GROUND, pl00_sp_hadou, 0, KEY_DOWN, KEY_DOWN | KEY_FORWARD, KEY_FORWARD, KEY_PUNCH } ;
```

Three stick steps; on the ground only; the move's number; then the three steps and the buttons that finish it.
`com_nml_ck` (`src/pl_com.c`) reads such a table against the controls. It is a little state machine with its place kept
in a `COM_WORK`, and there is no record of past inputs anywhere:

- Step 0 waits for the stick to be at the first position.
- Each further position must arrive before a timer runs out. The time allowed is picked at random for every step,
  between 12 and 17 frames (`com_time_tbl`). After the last position there are 15 frames for the button.
- The button test is `( pl_sw_new ^ pl_sw_old ) & buttons`: a button that *changed*. So letting go of a button finishes
  a move as well as pressing it.

Each of a fighter's commands has a `COM_WORK` of its own (`com_w`, then `com_w0` to `com_w12`), so several motions can
be part-way done at once. Other readers handle the other kinds of input: `com_tame_ck` for charge moves, `com_rot_ck`
for full turns of the stick, `com_renda_ck` for mashing.

**Charge moves.** In a charge command the first number is not a count of steps but the charge time. Guile's Sonic Boom
is `{ 40, COM_GROUND, pl02_sp_sonic, 0, KEY_BACK, KEY_FORWARD, KEY_PUNCH }` (`src/s_pl02.c:55`): hold back for 40
frames, then forward, then a punch. `com_tame_ck` (`src/pl_com.c:646-712`) works in steps:

- `com_tame_start` and `com_tame_hold` count `com[0]` frames with the direction `com[4]` held. Letting go starts the
  count again.
- `com_tame_release` waits for the direction to be let go. The fighter must still face the same way, unless the charge
  is up or down. Then `com_tame_lever` gives the direction `com[5]` 15 frames (plus the option's `COM_TIME_ADD` on the
  Dreamcast, `src/pl_com.c:15-19`), and it is checked in the same frame as the release.
- `com_tame_sw` gives the button `com[6]` the same time again.

So the charge time is a number in the command, and it differs between moves: Cyclops's Running Neckbreaker needs 50
frames and his Rapid Punches 60 (`src/s_pl06.c:59-60`).

**One command, two moves.** A move done on the ground and in the air is often one routine with two branches on
`routine_2`. The command check can set the branch: Morrigan's `soul_ck` sets `routine_2 = 0` and `jsoul_ck` sets
`routine_2 = 1`, and `sp_soul` calls `sp_soul_jmp[routine_2]` (`src/s_pl03.c:152`, `src/s_pl03.c:170`,
`src/s_pl03.c:1440-1446`). Or the routine sets it from the stance, as Doctor Doom's Plasma Beam does
(`src/s_pl0f.c:1387-1388`).

When the motion and button are in, `com_ok` asks whether the move may start now. `COM_GROUND` and `COM_AIR` are checked
against the fighter's stance, and `sp_ok_ck` refuses during guard stun, during another special, and while being thrown
or hit. If all is well, `com_sw_set` notes which button it was: `atck_level` is 0 for a light button and 1 for a heavy
one.

The order of the checks in a character's command routine is the priority of its moves. Ryu's `pl00_com_ck` asks for the
team moves first, then his hyper combos, then the Shoryuken, the Hadouken and the rest, and the first that says yes ends
the check.

### Hyper combos

A hyper combo is a special move with three differences:

- Its command has `COM_HYPER`, and usually `COM_ALL_SW`: both buttons of the pair, together.
- It needs gauge. The side's gauge fills from 0 to 144, then becomes a level, up to five levels. Most hyper combos take
  one level, and the test in the command routine is `*pl_w->gauge_level >= 1`.
- It may start *during* a special move (`sp_ok_hyper_ck`), while that move's attack is out or its `cancel_timer` runs.
  That is the cancel from a special into a hyper combo.

The move's first step calls `pl_hyper_start` (`src/pl_sub.c`), which takes the gauge (`pl_gauge_sub`), starts the flash
and the sounds, darkens the other fighters' palettes and sets the freeze described earlier.

### Animation: the script is the move

Every animation is a list of frames in the character's `data/char_XX.c`, grouped by kind.
`pl_char_set( pl_w, group, no )` starts one, and `pl_char_move` steps it once a frame. The groups have names:
`Char_Stand`, `Char_Jump`, `Char_Stand_P` (standing punches), `Char_Damage`, `Char_Special`, `Char_Super` and so on.

A frame is one `CG_FRAME` line, and `pl_char_set` copies it into the work's `cg_` fields. Ryu's standing light punch:

```c
/*        prm   type  ctr          end   spr  prio shadow             atck flag1         se zoom unk flag2  rival  box */
CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
CG_FRAME( 0x80,    0,    3,           0,   201, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
CG_FRAME(    0,    0,    5,           0,   201, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 136 ),
CG_FRAME(    0,    0,    3, END_OF_DATA,   200, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
CG_NEXT( pl00_stand_p_00 ),
```

The columns that matter most:

- `ctr`: how many frames this line stays up, showing sprite `spr`. `se` is a voice or a sound effect to play with it.
- `box`: the row of the character's box table for this frame (next section).
- `atck`: whether the move is still in its attacking part. This is what chains, cancels and the computer look at.
- `type`: a signal to the move's code. It means whatever the move wants it to mean.

There is no table of start-up, active and recovery times anywhere. Read the `ctr` column and the `box` column together:
3 frames with row 8 (no attack box), 3 frames with row 15 (which has one), 5 frames with row 136 (the same pose without
it), 3 frames back to standing. Change a number and the frame data changes.

`pl_char_move` returns a negative number once the last line has run, and nearly every move ends the same way:
`if( pl_char_move( pl_w ) < 0 )`, go back to standing.

### How a frame line is stepped

`pl_char_move` (`src/pl_char.c:97-111`) counts `cg_ctr` down, and when it reaches zero it moves on 20 bytes, one
line. If the line that has ended had `END_OF_DATA` (the program's `CG_END`), it goes to the line that the `CG_NEXT`
after it names, and the value it returns is the end flag, 0x80, which is negative in its `Sint8`. Lines with `ctr` 0
are passed over in the same call. `pl_char_set` (`src/pl_char.c:116-135`) copies the first line and passes over lines
with `ctr` 0 the same way. So:

- **An animation does not stop at its end.** It goes on at its `CG_NEXT`, and only the routine that sees the negative
  value ends the move. A `CG_NEXT` can name its own animation. Captain America's `pl0b_super_03_loop` is one line of 16
  frames that names itself (`data/char_0b.c:2386-2388`), so it plays until his routine changes it.
- **An animation without `END_OF_DATA` runs on into the animation stored after it.** Felicia's `pl20_special_00` has
  two lines and no end, so it goes on into `pl20_special_28` (`data/char_20.c:1566-1576`).
- **The first line counts from the frame of `pl_char_set`.** A routine that sets an animation and then calls the next
  step in the same call steps it once more before the first full frame. Colossus's `swing_init` calls `swing_move`
  (`src/s_pl32.c:1256-1257`).

A routine can also change a line's length while it plays. Some step the animation only while `cg_type` is clear, and
hold a line until their code clears it, as Marrow's `ride_rise` and Rogue's `leap_fly` do (`src/s_pl16.c:1388`,
`src/s_pl0a.c:1305`). Amingo cuts a line short with `cg_ctr = 1` and a `pl_char_move` (`src/s_pl15.c:1094-1095`).
Dan lengthens the first line of two hyper combos with `cg_ctr += 8` (`src/s_pl23.c:1677-1678`,
`src/s_pl23.c:1895-1896`).

### What decides how long a move lasts

The script is the move's timing only when the routine ends the move at the script's end. Many routines end it on a
count of their own:

- **A timer.** Colossus's `swing_move` sets `timer0` to 10 or 20 by strength, and `swing_rush` ends the rush when it
  runs out, whatever the animation is doing (`src/s_pl32.c:1264-1267`, `src/s_pl32.c:1289-1293`).
- **A count of marks.** Ryu's air Tatsumaki counts the lines whose `type` is negative in `timer0`
  (`src/s_pl00.c:1369-1375`), and Ken's Tatsumaki counts the lines with a `cg_prm` (`src/s_pl27.c:1185-1187`). The
  number of turns is the count, not the length of the script.
- **Mashing.** `sw_renda_ck` (`src/pl_com.c:1142-1148`) says whether an attack button was pressed this frame. A partner
  (`team_no` not 0) never mashes, and the computer mashes by chance (`cpu_renda_ck`). SonSon's `fuusetsu_move` adds 2
  to `timer0` for each frame with a press during its first 30 frames, and ends when `timer0` runs out
  (`src/s_pl14.c:1392-1393`, `src/s_pl14.c:1430-1439`).
- **A shot.** Jill's grenade throw waits in `grenade_wait` until her shot sets `grenade_end_flag`
  (`src/s_pl11.c:1782-1789`, `src/shl11_02.c:198`). If no work was free for the shot, the flag is set at once
  (`src/s_pl11.c:1766-1767`).
- **A landing.** An air move usually ends when the fighter touches the ground (next section).

### Air moves and landing

A fighter in the air moves by `pos += spd`, then `spd += acc`, once in each call of its routine. Ryu's air Tatsumaki
shows the usual pattern (`src/s_pl00.c:1333-1401`). `jtatsu_init` adds to `spd.x` and leaves `spd.y` and `acc.y` as
the jump had them, so the spin keeps the jump's rise or fall. Each frame of the spin moves him and then tests
`pos.y <= ground_y`. At the ground it puts him on it and starts the landing, `Char_Jump` 3, which then runs to its end.

The jump's own landing is `pl_land_ck` and `pl_land_set` (`src/pl_act.c:949-970`, `src/pl_act.c:1003-1022`).
`pl_land_ck` puts the fighter on `ground_y` and clears `sjump_flag` and `air_guard_cnt`, except while
`fly_flag` is set. `pl_land_set` stops all speeds, sets `N_LAND` and plays `Char_Jump` 3, 4 or 5 for a jump straight up,
forward or back (`jump_dir` -1, 0 or 1, `src/pl_act.c:1279-1281`). One exception makes a missed air attack cost more: in `N_NML_ATCK`, when `hit_flag` is clear
and the opponent has not guarded (`guard_cnt` 0), the landing line is 20 dots below the ground
(`src/pl_act.c:955-958`). The fighter falls those 20 dots before it lands.

---

## Hits, damage and gauges

### Boxes

Three tables in a character's data describe where it can hit and be hit:

- `data/hit_XX.c`: the boxes. A `HIT_BOX` is an offset from the fighter's position, a half width and a half height.
- `data/frame_boxXX.c`: the box *sets*. A `FRAME_BOX` row names up to two attack boxes, four body boxes and one box for
  being thrown. The `box` column of an animation frame picks a row.
- `data/hit_dtXX.c`: the attack rows, one `HIT_DATA` for each attack. A work's `atck_no` picks the row.

Ryu's rows 8 and 15, the two used by the punch above:

```c
/* atck        atck2 body0 body1 body2        body3 catch */
{    0,            0,     1,     2,     3,            0,   240,      0 },	/* 8: head, body and legs */
{   32,            0,     1,     2,     3,  0x2000 | 16,   240,      0 },	/* 15: attack box 32, and the arm */
```

The arm that punches is itself a body box. Sticking a limb out makes it a target.

### The hit check

Works don't check themselves. Anything that wants to hit or be hit this frame calls `hit_work_set`, which adds it to its
side's list. `hit_ck0` (`src/hit.c`) then takes each side's attackers against the other side's targets:

- An attacker needs `atck_flag` set and `hit_flag` clear. `hit_flag` is set when an attack lands, so one attack hits
  once. A move that hits several times clears it again (`HIT_RESET`).
- A target needs `dm_flag` set and must not have been hit already this frame.
- The attack box is tested against each of the target's body boxes. Boxes are mirrored for facing and scaled to the
  screen: by 5/3 across and 15/7 down, times the work's `scale` (`src/hit.c:226-256`).
- The frame's second attack box, `atck_box2`, is tested only when it is positive and the first box has missed
  (`src/hit.c:84-96`).
- When the attack box overlaps several body boxes, the hit is on the one it overlaps most (`src/hit.c:149-170`).

A work takes part only in a frame in which its routine called `hit_work_set` (`src/hit.c:276-284`), and `hit_work_next` empties the
lists after each check (`src/hit.c:268-273`). A shot's first frame need not call it. Ryu's Hadouken does not in its first step
(`shl00_00_init`, `src/shl00_00.c:68-88`), so it can first hit in the frame after the one it was made in.

A hit is only *recorded* here: who hit whom, and on which box. Afterwards `pl_damage` (`src/pl_dmg.c`) is called for
each fighter that was struck, and that is where the game decides what the hit does.

Throws use the same boxes differently: a throw's reach is the second attack box of the thrower's frame, tested against
the target's `catch_box`. Fighters also push each other apart, in `pl_push_ck`.

### Hitting more than once

`hit_flag` holds the result of the attack. `hit_set` puts `HR_HIT` (0x80) in it on every contact, a guarded one too,
with the kind bits of what was hit (`src/hit.c:142-147`), and `guard_set` adds `HR_GUARD` (`src/pl_dmg.c:515`). Since
`HR_HIT` is the top bit and `hit_flag` is an `Sint8`, a routine can test `hit_flag < 0` for "it touched". Moves branch
on it: Ken's Tatsumaki goes on to a last hit only if a spin has hit (`src/s_pl27.c:1187-1194`), and Captain America's
Final Justice dash goes on or misses by it (`src/s_pl0b.c:2206-2216`).

While `hit_flag` is set the attacker is passed over, so one attack hits once. A move hits again only when its routine
clears it with `HIT_RESET` (`include/pl_def.h:22-26`), which also clears `atck_attr` and counts a new attack in
`atck_cnt`. Routines do this on a signal from the script:

- On `type`: Tron Bonne's standing heavy punch sets `atck_no = ATCK_STAND_HP` and resets when `cg_type` is set
  (`src/s_pl10.c:553-559`).
- On `flag1`: SonSon's Fuusetsu Zan resets on each line with a `cg_flag1` (`src/s_pl14.c:1425-1429`).

Each reset is good for one more hit at most, at the first frame after it whose box touches. A routine often writes a
new `atck_no` with the reset, so each hit reads its own attack row, with its own damage, hit stop and reaction. The
routine runs in `all_work_move`, before `hit_ck0` in the same frame (`src/s_play.c:334-335`), so an attack re-armed in
a frame can hit in that frame.

### An attack row

Ryu's standing light punch and his ground fireball, from `data/hit_dt00.c`:

```c
{  4, ZU,                  A_NOM,     K_S, 4, 10, 10, 0, 0x18, 4, 0x70, MARK_N1, SC100, SD_NOR_S, ... },	/* 00: ATCK_STAND_LP */
{ 14, ZU | NO_VEC | NO_ZU, A_SPECIAL, K_L, 0,  6, 20, 0, 0x1a, 8, 0x60,       3, SC200,        8, ... },	/* 30 */
```

In order: the damage (4 and 14), the reaction (`ZU`: pushed back along the ground), how it must be guarded and what
class of attack it is, its strength, the stun it adds (`piyo`), the hit stop, times for the air and the floor
(`sandbag_time`, `sleep_time`), the direction and force of the knock-back, its reach, the hit mark, the score and the
sound. Further along the row, `shc` is the hyper combo gauge the hit gives.

A few flags do a lot of work:

- `A_HI` and `A_LO`: an overhead, guarded only standing, and a low, guarded only crouching.
- `A_SPECIAL` and `A_HYPER`: the attack's class. These do chip damage.
- `BLACK_COMBO`: a launcher. The attacker may super jump after it by holding up. `AERIAL_FINISH` ends an air combo, and
  `NO_SCALE`, `NO_CHIP` and `NO_KO` are exceptions to the rules below.

### What a hit does

`pl_damage` first asks whether the hit was guarded. The fighter must already be in `N_GUARD` or `N_AIR_GUARD`, and the
row's guard bits must allow its stance. A computer player's guard can also be a roll of the dice (`em_guard_ck`).

**If it was guarded,** the fighter is held in guard stun for a time that depends on the attack's strength (18, 14 or 11
frames unless the row gives its own). Specials and hyper combos do chip damage: the full damage after all the rates
below, divided by 8, at least 1.

The guard's posture is picked by the attack, not only by the stick (`guard_char_set`, `src/pl_dmg.c:573-587`). On the
ground the fighter guards crouching (`Char_Guard` 7) against an `A_LO` attack, standing (`Char_Guard` 3) against an
`A_HI` one, and against any other by whether down is held. Against an attacker with `fly_flag` set only the stick
counts. In the air the guard takes the air knock-back and `Char_Air_Guard` 2
(`src/pl_dmg.c:560-572`).

**If it hit,** the damage is worked out in layers, each a table lookup and a multiply. Most rates are counted in 32nds:

1. The row's `damage`. If the computer threw it, a rate for its level of play is applied (75% at the lowest level, 125%
   at the highest).
2. Combo scaling, from `dm_hit_rate_tbl`. The first four hits of a combo do full damage, then it steps down, to a half
   by the ninth hit and to almost nothing by the seventeenth. Hyper combos use kinder rows.
3. The defender's rate (`def_rate_tbl`). This is where characters differ: Ryu takes 32/32, Zangief 28/32, Morrigan
   38/32.
4. A rate for low vital: with 36 points or fewer the fighter takes three quarters, with 19 or fewer five eighths.
5. The operator's DAMAGE LEVEL setting (`damage_level`: 16, 32, 48 or 64 over 32), and any power-ups running on either
   side (`atck_pow`, `def_pow`).

The result is never below 1. It comes off `vital_new`, and three quarters of it off `vital_red`. After the damage:

- **Combo.** `combo_hit2` (`src/combo.c`) adds the hit to the attacker's `combo_cnt` and to a count kept on the victim
  (`dm_hit_cnt`), the one that scales the damage.
- **Gauge.** Both sides gain. The attacker's gain shrinks as the combo grows (`gauge_combo_tbl`): full for the first two
  hits, a half by the fifth, nearly nothing by the ninth.
- **Stun.** The row's `piyo`, plus a small random amount, comes off `stun_cnt`, which starts at 80. At zero the fighter
  is dizzy. After 60 frames without being hit it goes back to 80.
- **The reaction.** `dm_tbl` picks a routine from the victim's stance and the row's reaction: pushed back, knocked away,
  swept, slammed, frozen, and so on. The victim goes to `routine_1 = PL_DAMAGE`, and that routine runs until it recovers.
- **KO.** If `vital_new` reached zero, `ko_flag` is set and both sides get 10 frames of hit stop.

**The reaction is not always the row's.** The row's reaction is only the first word. `pl_damage` and `dm_data_set`
change `dm_no` before `dm_tbl` is called:

- `ATR_FREEZE` in the attacker's `atck_attr` makes it 9, frozen. A freeze on a fighter already frozen becomes 1, and so
  does reaction 20 on a fighter already in it (`src/pl_dmg.c:341-349`). `ATR_KNOCK_AWAY` makes it 10
  (`src/pl_dmg.c:372-375`).
- A fighter whose `stun_cnt` has run out gets 3, dizzy, and one with a `snap_partner` gets 12, the snapback
  (`src/pl_dmg.c:232-235`).
- A KO makes it 2 (`src/pl_dmg.c:241-252`).

Then `dm_tbl[pl_stance][dm_no]` picks the routine (`src/pl_dmg.c:142-150`, `src/pl_dmg.c:285`), and the stance
changes it again. A fighter in the air pushed along the ground (`dm_zuri`, 0) is knocked away instead (`dm_buttobi`),
and a sweep knocks it away too. A fighter lying down takes `dm_air` for nearly every reaction.

### Push-back

Pushed back along the ground, on a guard (`guard_back_set`) or on a hit with reaction 0 (`dm_zuri`, which calls the
same routine), the fighter slides by a table of steps (`src/pl_dmg.c:700-722`, `src/pl_dmg.c:1193-1200`). The table is
one of the five in `back_step_tbl` (`src/chrdef.c:14-19`). A row with `K_BACK_MAX` takes the longest, a row with a
level in `K_LV_MASK` takes that level's, and any other row takes one by its `power`: the first under 5, the second from
5, the third from 12.

`dm_back_move` (`src/pl_dmg.c:872-912`) takes one step a frame, and a step is the table's number times 1.6666666. A
guarding fighter steps every frame of `act_guard` (`src/pl_main.c:897-904`), from the first frame after the hit stop. A
fighter hit with reaction 0 spends its first frame of `dm_zuri_move` stopping its fall, and steps from the second
(`src/pl_dm.c:257-267`). Since the fighters move apart, a later attack box of the same move can miss.

At the screen's edge the step goes to the attacker. When the pushed fighter is at the wall behind it, the row lacks
`NO_ZU`, and the attacker is not at a wall itself, the step moves the attacker back instead
(`src/pl_dmg.c:893-904`). The wall is `wall_flag`, which `pl_wall_ck` sets when it holds the fighter 285 units either
side of the camera's eye (`src/pl_sub.c:912-923`).

Three bits of `atck_attr` change the push-back (`include/pl_work.h:527-535`). `ATR_BACK_POS` pushes the fighter away
from where the attacking player stands (`back_move_push`, `src/pl_dmg.c:917-930`). `ATR_BACK_WORK` pushes it away from
the striking work, on a guard too (`src/pl_dmg.c:707-711`, `src/pl_dmg.c:886-889`). `ATR_BACK_TURN` pushes it by its
own facing (`src/pl_dmg.c:891`). `HIT_RESET` clears `atck_attr`, so a routine that sets these bits sets them again
after each reset.

### The clock, the win and the score

`vs_sys_move` (`src/vs_sys.c`) is the fight's own state machine, on `vs_st`: a wait, the entrances, the start message,
the fight, the wait for everyone to finish, the win pose, and the way out.

- **The clock** starts at 99 (`round_time`). How many frames one count lasts comes from the TIMER SPEED setting
  (`time_speed`): 180, 120, 90 or 70.
- **A KO.** `ko_ck` adds up each team's three vitals every frame. A team with none left has lost.
- **Time over.** The team with more vital in total wins, and equal totals are a draw. There are no rounds: a fight is
  one long round between two teams of three.

Score only counts against the computer (`score_add`). Points come from the first attack of the fight, from every hit (by
its damage), from combos (the hits times the hits, in thousands), and from the bonuses at the end for vital and time
left.

---

## Worked example: Ryu throws a fireball

You press down, down-forward, forward and light punch. Here is everything that happens, in order.

**1. The motion.** Every frame, `pl_wait_com_ck` calls slot 1 of Ryu's table, `pl00_com_ck`. Among its checks is the
Hadouken's, which runs `com_nml_ck( pl_w, com_hadou, &pl_w->com_w )`. Over a few frames the `COM_WORK` steps through
down, down-forward and forward. Then the light punch changes state, and the command is done.

**2. The move may start.** `com_ok` checks that Ryu is on the ground (`COM_GROUND`) and that `sp_ok_ck` has no
objection. Then Ryu's own check adds one more rule:

```c
if( !pl_w->sjump_flag && lw->shot_flag ) return 0 ;
com_sw_set( pl_w, &pl_w->com_w ) ;
pl_w->routine_1 = PL_NORM ;
pl_w->routine_2 = pl_w->routine_3 = 0 ;
pl_w->pl_sp_type = pl00_sp_hadou ;
act_st_set( pl_w, N_SPECIAL ) ;
```

`shot_flag` is set while a ground fireball of his is out. One at a time. Otherwise the button's strength is noted
(`atck_level = 0`, light), the move's number goes in `pl_sp_type`, and the act becomes `N_SPECIAL`.

**3. The routine.** Still in the same frame, the fighter's move reaches its act. For `N_SPECIAL` that is slot 8,
`pl00_sp_atck`, which picks a routine by `pl_sp_type`. The Hadouken's has two steps, on `routine_2`. The first adds a
little gauge and starts the animation with `pl_char_set( pl_w, Char_Special, pl_w->atck_level )`: group `Char_Special`,
animation 0 for light. The heavy punch would give animation 1.

**4. The animation.** In `data/char_00.c` that is `pl00_special_00`. Its first three lines are six frames of wind-up (1,
4 and 1). The fourth has 0x80 in its `type` column and a voice in its `se` column. No box row in the whole animation has
an attack box: Ryu's own body never hits anything here.

**5. The signal.** The routine's second step steps the animation with `pl_char_move` once a frame, and watches for the
signal:

```c
if( pl_w->cg_type ){
	pl_w->cg_type = 0 ;
	shl00_00_set( pl_w, 0 ) ;
}
```

On the seventh frame the script's `type` shows up in `cg_type`, and the fireball is born on exactly that frame. The rest
of the script is recovery, and when it runs out Ryu stands again.

**6. The fireball.** `shl00_00_set` (`src/shl00_00.c`) takes a work from the pool, into the shots' list, and sets its `work_func`
to `shl00_00_main` and its `master_w` to Ryu. The shots' list runs after the fighters, so its first step comes later in
the same frame. In that step the new work copies Ryu's picture and animation tables,
so it draws from his sprites and hits with his attack rows. It gives up at once if Ryu's animation has changed since it
was asked for (he was hit first). Then it sets Ryu's `shot_flag` and reads its row of `shl00_00_tbl`, picked by
strength:

```c
{ -6, 0, -96, 113, 11, 48 },		/* light */
{ -9, 0, -96, 113, 12, 48 },		/* heavy */
```

A speed across and up, a place to appear in front of Ryu, an animation and an attack row. The light one moves 6 a frame,
the heavy one 9, and that is the whole difference between them. It then starts its own animation with the same
`pl_char_set` a fighter uses.

**7. Flight.** Each frame the fireball asks three questions, then moves:

```c
if( !scr_in_x_ck( ew ) )	/* off the screen: free the work */
if( ew->hit_flag )		/* it hit something */
if( ew->hit_box_no )		/* something hit it */
ew->pos.x += ew->spd.x ;
pl_char_move( ew ) ;
hit_work_set( ew ) ;
```

Its animation frames use box row 66 of Ryu's table: one attack box and one body box. The body box is why two fireballs
cancel each other out.

**8. The hit.** `hit_ck0` finds the fireball's attack box over one of the opponent's body boxes and records it.
`pl_damage` reads attack row 48 (0x30, the second row shown earlier): 14 damage, class `A_SPECIAL`, so it chips when
guarded. The opponent reacts or guards, and the opponent and the fireball take the row's hit stop. Ryu does not: the
fireball's `atck_flag` lacks `HIT_OWNER`. On its side, the fireball sees its `hit_flag`, switches to its fading animation,
clears Ryu's `shot_flag`, and frees its work when that animation ends. Ryu can throw another.

The whole chain, in one block:

```
pl_sw_set -> pl00_com_ck -> com_nml_ck( com_hadou ) -> pl_sp_type -> pl_tbl[TBL_SP_ATCK] -> pl00_sp_atck
-> pl_char_set( Char_Special, 0 ) -> cg_type -> shl00_00_set -> work_get -> shl00_00_tbl
-> hit_work_set -> hit_ck0 -> pl_damage -> hit_data_tbl[48]
```

Every arrow is an index, a pointer or a slot in a table. Every other fireball in the game goes down the same road with
its own module and data.

---

## The computer player

The computer doesn't have its own way of moving a fighter. `em_play` (`src/em_play.c`) runs at the top of a computer
fighter's frame and fills in `pad_sw`, the same switches a human's stick would. What it presses comes from data: five of
the nine parts of each character's data are the computer's.

**Scripts.** The computer plays from command lists written in a small language (`include/cpu_cmd.h`). This one is from
`data/taiki00.c`, Ryu's lists for when nothing much is happening:

```c
const Uint8 taiki00_cmd_02[] = {
	STAND,
	IF_DIST( 64, 240, 32, 128 ),
		WALK_B( 64 ),
	CMD_ELSE,
		IF_DIST( 64, 240, 160, 128 ),
			WALK_F( 64 ),
		CMD_ELSE,
			WAIT( 20 ),
		CMD_ENDIF,
	CMD_ENDIF,
	...
```

If the opponent is close, walk back. If he is at middle distance, walk forward. Otherwise wait. Each command is a
routine in `src/em_script.c`: walks, dashes, jumps, attacks, guards, throws, calls for a partner, and tests of distance,
vital, gauge, the clock and chance.

**Modes.** Each frame, a row of event checks looks at what both fighters are doing, and an event can switch the computer
to another mode with its own lists (`data/wariXX.c`): a throw is in reach, it has just guarded or been hit, it is
getting up, the opponent is dashing, jumping or flying. When an attack is coming, the list is chosen by the *attack*:
every attack row has a number (`cpu_answer_no`) that picks the computer's answers to it from `data/yokXX.c`. Whether a
mode is taken, and which of its lists is run, is a draw of `Rnd` against tables of 32 entries. The tables are indexed by
the level of play, so a stronger computer runs the same lists with better odds.

**The level of play** is `cpu_level_work.level`, 0 to 31 (`src/cpu_level.c`). It follows how well the human is doing:
the player earns points for every five seconds a fight lasts and for the share of attacks that land, and the level is
the points in steps of 80. A table caps it, by the DIFFICULTY setting and by which of the eight fights this is, so the
first opponents can't play at full strength however well you do.

**One shortcut.** The computer doesn't waggle its stick through special-move motions. The script command `SP_CMD` names
the move by number, and `com_nml_ck` starts with this:

```c
if( pl_w->cpu_flag ){
	if( pl_w->cpu_sp_flag && pl_w->cpu_sp_no == COM_NO( com ) ) return com_ok( pl_w, com, cw, 0 ) ;
	return 0 ;
}
```

The motion is taken as done. Everything after it is not skipped: `com_ok` still asks whether the move may start, so the
computer can't do anything a player couldn't do from the same position.

---

## Sound

The game makes no sound itself. It sends requests to Sega's sound driver, and all of them go through `src/sound.c`.

- **Music** is streamed from the ROM board. `stream_req` picks one of 14 streams, and each stage has its own.
- **Sound effects and voices** are in packs loaded into the sound memory: common ones, and one for each character in the
  fight. `pl_se_req` plays a common effect and `voice_req` a character's voice, and both pan the sound to the side of
  the screen the fighter is on.
- An animation frame can ask for a sound by itself, through its `se` column (`cg_se_req`). Most swings, steps and shouts
  are started that way, by the script.
- `sound_req` drops every request during attract mode unless the operator has demo sound on.

---

## The test program

The second program in the ROM is small. Its `main` (`src/test/main.c`) calls the same `system_init` and then runs
`test_main` once a frame. In the game, `test_mode_jump` hands over to the board's own test menu through Sega's library,
and the game test is started from there.

Its menus (`src/test/test_mode.c`) are INPUT TEST, which shows every switch as the I/O boards report it; GAME DATA, the
book-keeping counters; GAME CONFIGURATION, which edits the operator's settings and saves them to the backup memory on
the way out; and ROM BOARD TEST, which adds up every ROM on the ROM board and compares the sums with a table
(`rom_sum_tbl`), printing GOOD or BAD beside each.

---

## The Dreamcast version

The home version is the same game, built from the same files with the switch `TARGET_DC` set. Most of what this
tour has said holds for it unchanged: the frame loop, the works, the fight, the characters. The differences are in
where things come from and in what is wrapped around the fight.

### Loading from the disc

The cartridge has all its ROMs on the bus at once, so the arcade's game reads its data where it lies. The Dreamcast
has 16 MB of memory and a disc, so `src/dc/am_load.c` reads files instead: `file_name_tbl` names all 712 of them,
and the scenes' and the fight's files are read in the background, through queues, while the game goes on drawing.

The biggest change is the characters' code. On the arcade, all 59 character modules are linked into the program, and
`pl_work_init` calls the right `plXX_tbl_set` from a table. On the Dreamcast they are not in the program at all. Each
module is linked by itself, six times over, at six fixed addresses (0CE30000, 0CE38000, and so on, 32 KB apart):
one for each of the six fighters of a match, two teams of three. Those are the files `S_PL00A.BIN` to `S_PL3AF.BIN`.
When a fighter is set up, `tex_load.c` loads its character's file for its place, and `pl_work_init` calls the
address of that place instead of a name:

```c
#ifdef TARGET_DC	/* the characters' modules load at six places, one for each player */
	static void ( *tbl_set_jmp[6] )( PLAYER_WORK *pl_w ) = {
		( void ( * )( PLAYER_WORK *pl_w ) )0x0ce30000, ( void ( * )( PLAYER_WORK *pl_w ) )0x0ce38000,
		...
```

A loaded module calls back into the main program by name, so each file is linked against the main program's own
symbols. That is why there are six of each: the same code, at six addresses.

### The home menus

There is no coin to wait for. The game starts at a ring of modes (`src/dc/home_menu.c`, drawn by the `dm0c_`
files), numbered by `HOME_ARCADE` to `HOME_RECORDS` in `include/system.h`: arcade, versus, training, the options,
the memory card, the network, the SECRET FACTOR, the score attack and the records. Arcade, versus, training and
the score attack go into the arcade's own game with their rules set, and come back to the ring when it ends.

- **Training** runs an ordinary fight with a pause menu over it (`pause_*.c`) and the settings in
  `training_work`: how the dummy guards, whether it recovers in the air or changes partner, the speed, and gauges
  that fill by themselves. They reach the fight through `Sys`, as `Sys->dummy_guard` and the rest.
- **The SECRET FACTOR** (`secret.c` to `secret_3.c`, drawn by `dm0f_`) is a shop. Playing earns points, and the
  shop offers three goods at a time: a hidden character, a second colour for one already opened, or the next of
  seven extras (the first costs 1,000 points, and each after it 500 more). What has been bought is kept in the
  memory card's save (`vms_work`).
- **The memory card** menus (`secret_vm.c`, `vms.c`, drawn by `dm10_`) save and load that file, and the SECRET
  FACTOR can also save a small game for the card's own screen.
- **The options** (`dm0c_option.c` and the pages beside it) are the arcade's operator settings made into a menu, with
  the screen's position and the pad's buttons added.

### The network

The disc has a modem lobby: an entry screen, rooms, player records, a chat log and a keyboard (`src/dc/kbd.c`
reads a real one or draws one on screen, with the kana input and kana-kanji conversion of the Japanese release). It is
drawn by the `dm0d_` files and speaks to the server through Sega's modem libraries. The USA disc still has all of
it, but its network menu is switched off: `src/dc/net_menu.c` is an empty routine and an unused table.

---

## Where to look in the source

| You want | Start at |
|---|---|
| boot | `src/bootid.src`, `src/system.c` `system_init`, `src/sys_task.c` `System_Init` |
| the frame loop | `src/main.c`, `src/sys_task.c` `System_Main` |
| loading | `src/am_load.c` |
| scenes | `src/sys_mode.c` (attract), `src/game.c`, `src/s_play.c` |
| joining in, select | `src/entry.c`, `src/select.c`, `src/sel_decide.c` |
| the controls | `src/system.c` `jamma_read`, `src/pl_sub.c` `pl_sw_set`, `include/key.h` |
| the structs | `include/pl_work.h`, `system.h`, `vs_sys.h`, `scr.h`, `work.h` |
| works and lists | `src/hit_work.c`, `src/all_move.c` |
| sprites and colour | `src/sprite.c`, `src/pl_char.c`, `src/disp.c` |
| the stage and camera | `src/stage.c`, `src/scr.c` |
| fighter states | `src/pl_main.c`, `src/pl_act.c` |
| commands, tags, assists | `src/pl_com.c`, `src/pl_team.c` |
| a character | `src/s_plXX.c`, `include/plXX_def.h`, `src/shlXX_YY.c`, `src/plsXX_YY.c` |
| animation scripts | `src/pl_char.c`, `include/char_tbl.h`, `data/char_XX.c` |
| boxes and hits | `src/hit.c`, `data/hit_XX.c`, `data/frame_boxXX.c`, `data/hit_dtXX.c`, `include/hit_equ.h` |
| damage, gauges, combos | `src/pl_dmg.c`, `src/pl_dm.c`, `src/chrdef.c`, `src/combo.c`, `include/pl_def.h` (`HIT_RESET`) |
| hit stop, the hyper combo freeze | `src/pl_main.c` `pl_move_fight`, `src/pl_sub.c` `pl_frame_jump`, `src/hyper_ef.c` |
| the clock and the win | `src/vs_sys.c` |
| the computer | `src/em_play.c`, `src/em_script.c`, `src/cpu_level.c`, `include/cpu_cmd.h`, `data/taikiXX.c` |
| sound | `src/sound.c` |
| the test program | `src/test/main.c`, `src/test/test_mode.c` |
| the Dreamcast's loading | `src/dc/am_load.c`, `src/tex_load.c`, `src/pl_sub.c` `pl_work_init` |
| the home menus | `src/dc/home_menu.c`, `src/dc/secret.c`, `src/vms.c`, the `src/dc/dm*` files |
| training | `src/pl_sw.c`, `src/dc/pause_*.c`, `include/system.h` `TRAINING_WORK` |
| the network | `src/dc/dm0d_*.c`, `src/dc/kbd.c`, `src/dc/font.c` |
