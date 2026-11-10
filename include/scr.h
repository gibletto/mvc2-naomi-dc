/* The screen: the fade, the camera (SCR_WORK), the light, fog, brightness and background colour,
   the stage, the flash, text and the palette. player.h includes it. */
#ifndef SCR_H
#define SCR_H

extern void fade_init( void );
extern void fade_move( void );
extern Uint32 fade_cnt( void );

extern void scr_excep_x_clr( void );
extern void scr_excep_y_clr( void );
extern void scr_excep_clr( void );
extern void scr_excep_set( PLAYER_WORK *pl_w, Sint8 a, Sint8 b );

/* the screen fade (fade.c) */
typedef struct FADE_WORK {
	Sint32	on;		/* 000: the fade is running */
	Sint32	out;	/* 004: 0 it clears, 1 it covers the screen */
	Uint32	col;	/* 008: the colour, its alpha in the top byte */
	Uint32	cnt;	/* 00c: frames left */
	Uint32	time;	/* 010: the fade's length in frames */
} FADE_WORK;
extern void fade_disp( void );

#define FADE_IN		0	/* fade_set()'s c: the colour clears */
#define FADE_OUT	1	/* the colour covers the screen and stays */
extern void fade_set( int a, int b, int c );

typedef struct SCR_WORK {
	Sint8	scr_type;		/* 000: the camera: 0 the fight's, 1 a path of set keys (the scenes outside a fight) */
	Sint8	x_mode;		/* 001: the fight camera's mode across, an index of the x mode table */
	Sint8	y_mode;		/* 002: and its mode up and down, of the y mode table */
	Sint8	stop_x_mode;		/* 003: set by a hyper combo's flash: the mode across while the game stops */
	Sint8	stop_y_mode;		/* 004: and the mode up and down */
	Sint8	quake_level;		/* 005: the screen quake: 0 none, 1 to 3 small to large */
	Sint8	quake_flag;		/* 006: start the quake over */
	Sint8	scr_routine[3];	/* 007: the camera's step, its path and the path's key */
	Sint8	unk_0a;		/* 00a */
	VEC3	eye[2];	/* 00c: the camera's eye (its x the screen centre's x in the stage), and where it was last frame */
	Uint8	unk_24[0x54 - 0x24];
	VEC3	target[2];	/* 054: the point the camera looks at, and where it was last frame */
	Float	fov_x;		/* 06c: the field of view across, in degrees */
	Float	unk_70;		/* 070 */
	Uint8	unk_74[0x78 - 0x74];
	Float	rise_y;		/* 078: how far the higher player is above the height the camera starts to rise */
	Uint8	unk_7c[0x80 - 0x7c];
	Float	shift_y;		/* 080: the projection's shift up and down */
	Sint32	roll;		/* 084: the camera's roll, an angle */
	Float	scr_left;		/* 088: the screen's left edge */
	Float	scr_right;		/* 08c: the screen's right edge */
	Float	scr_top;		/* 090: the screen's top edge */
	Float	scr_bottom;		/* 094: the screen's bottom edge */
	Float	stage_left;		/* 098: the stage's left end, as far as the screen scrolls */
	Float	stage_right;		/* 09c: the stage's right end */
	Float	stage_top;		/* 0a0: the stage's top */
	Float	stage_bottom;		/* 0a4: the stage's bottom */
	Float	center_y;		/* 0a8: the screen centre's y */
	VEC3	eye_goal;		/* 0ac: where the eye is going */
	VEC3	target_goal;		/* 0b8: where the point looked at is going */
	VEC3	eye_turn;		/* 0c4: a path key's turn of the eye about the point looked at, in degrees */
	VEC3	target_turn;		/* 0d0: and its turn of the point looked at about the eye */
	VEC3	eye_step;		/* 0dc: the eye's move each frame */
	VEC3	target_step;		/* 0e8: the point looked at's move each frame */
	Uint8	eye_move_st;		/* 0f4: the eye's move: 0 to start, 1 moving, 2 there */
	Uint8	target_move_st;		/* 0f5: the point looked at's move */
	Uint8	eye_turn_st;		/* 0f6: the eye's turn */
	Uint8	target_turn_st;		/* 0f7: the point looked at's turn */
	Uint8	init_flag;		/* 0f8: set after the camera's first frame */
	Uint8	excep_side;		/* 0f9: the side whose move set the camera's mode */
	Uint8	unk_fa[0x170 - 0xfa];
} SCR_WORK;

/* the scenes' set-up of the screen, its light and the fog (in other modules) */
extern void SysTransSet( void ( *func )( void ) );
extern void back_color_set( Uint32 col );
extern void scr_work_init( void );
extern void fog_init();	/* sys_task passes it an argument it doesn't read */
extern void bright_init( void );

/* the light */
typedef struct LIGHT_WORK {
	VEC3	dir;	/* 000: its direction */
	Float	amb;		/* 00c: the ambient light */
} LIGHT_WORK;
extern void light_init( void );	/* the light's set-up */
extern void light_dir_set( VEC3 *dir );	/* the light's direction */
extern void light_amb_set( Float amb );	/* the ambient light */
extern void light_dummy( void );

/* fog.c: the fog table */
#define FOG_TABLE	129
typedef struct FOG_WORK {
	Uint8	on;		/* 000: the table is sent; the fog is off without it */
	Uint8	unk_01[3];
	Sint32	len;	/* 004: the table's entries that fade, the rest are clear */
	Uint8	unk_08;
	Uint8	r;		/* 009: the fog's colour */
	Uint8	g;		/* 00a */
	Uint8	b;		/* 00b */
	Float	density;	/* 00c */
	Float	table[FOG_TABLE];	/* 010 */
} FOG_WORK;
extern void fog_set( void );
extern void fog_disp( void );

/* bright.c: the screen's brightness */
typedef struct BRIGHT_WORK {
	Uint8	unk_00[8];
	Float	rate[4];	/* 008: a scale for each of alpha, red, green and blue; 1.0 at full */
} BRIGHT_WORK;
extern void bright_init( void );
extern void bright_set( Float rate );
extern void bright_stop_set( void );

/* back_color.c: the background colour */
typedef union BACK_COL {
	Uint32	argb;
	struct {
		Uint8	b, g, r, a;
	} c;
} BACK_COL;
typedef struct BACK_WORK {
	Sint32	mode;	/* 000: 0 one colour, 1 to 3 the gradients of back_color_set1 to set3 */
	BACK_COL	col[3];	/* 004 */
} BACK_WORK;
extern void back_color_set1( Uint32 col0, Uint32 col1 );
extern void back_color_set2( Uint32 col0, Uint32 col1 );
extern void back_color_set3( Uint32 col0, Uint32 col1, Uint32 col2 );
extern void back_color_disp( void );

/* stage.c: the stage's set-up, and its changes while a fight goes on */
typedef struct STAGE_WORK {
	Sint8	set_cnt;	/* 000: counts the times its objects were set */
	Sint8	step;	/* 001: the step of the stage's own change (stage_move) */
	Uint8	unk_02[2];
	Sint32	event;	/* 004: set by vs_sys.c when a fight in game mode 8 ends, 1 or 2; stage 08 plays it out */
	Float	unk_08;	/* 008: 1.0 from stage_set */
} STAGE_WORK;

/* flashing.c: the screen's flash */
typedef struct FLASH_WORK {
	Sint32	flag;	/* 000: this frame's flash, 0 for none, else 1 to 3 */
	Sint32	type;	/* 004: the kind of flash */
	Sint32	timer;	/* 008: its frames left */
	Uint32	col;	/* 00c: its colour */
} FLASH_WORK;
extern void flash_blink_set( void );
extern void flash_white_set( void );
extern void flash_ko_set( void );
extern void flash_long_set( void );
extern void flash_dark_long_set( void );
extern void flash_dark_set( void );
extern void flash_finish_set( void );
extern void flash_ending_set( void );
extern void flash_move( void );
extern Sint32 flash_flag_get( void );

/* message.c: lines of text and sprite words */
typedef struct MES_WORK {
	Uint8	x;		/* 00: the place, in characters; past 99 ends a list */
	Uint8	y;		/* 01 */
	Sint8	type;	/* 02: the font, colour and size */
	char	str[61];	/* 03 */
} MES_WORK;
extern void mes_puts( const MES_WORK *mw );
extern void mes_font_disp( const MES_WORK *mw );
extern void spr_mes_put( int sprno, int no );
extern void pts_get_disp( int side, int pts );
extern void spr_num_disp( int a, int b );
extern void spr_mark_disp( void );

extern void scr_move( void );

/* 0x30-byte entries, from the player's col_no on */
typedef struct COL_WORK {
	Uint32	pal_no;	/* 000: the hardware palette line it is sent to (nlSetPaletteData's offset is pal_no * 16) */
	Sint32	cnt;	/* 004: the colours it sends, 16 */
	Sint32	trans_flag;	/* 008: send the line this frame: 1 as it is, 2 at half brightness; cleared once sent */
	Uint8	unk_0c[0x10 - 0x0c];
	Uint16	col[16];	/* 010: the line's 16 colours, ARGB4444 */
} COL_WORK;

/* a colour, ARGB4444 */
#define COL_RGB		0x0fff	/* mask: the colour without its alpha */
#define COL_ALPHA	0xf000	/* mask: the colour's alpha, 4 bits (0xf000 solid) */

#endif
