/* The stage's set-up (the objects of each stage are setXX_YY.c): the fog, the
   background colour and the light a stage starts with, the table of the objects each stage sets
   (kind 5 works), and how a fight changes the fog and the sky of two stages as it goes on. */
#include "player.h"

/* the objects of each stage: setXX_YY_set sets the YYth object of stage XX */
extern void set00_00_set( void ) ;
extern void set00_01_set( void ) ;
extern void set00_02_set( void ) ;
extern void set00_03_set( void ) ;
extern void set00_04_set( void ) ;
extern void set01_00_set( void ) ;
extern void set01_01_set( void ) ;
extern void set01_02_set( void ) ;
extern void set01_03_set( void ) ;
extern void set01_04_set( void ) ;
extern void set01_05_set( void ) ;
extern void set02_00_set( void ) ;
extern void set02_02_set( void ) ;
extern void set02_03_set( void ) ;
extern void set02_04_set( void ) ;
extern void set02_05_set( void ) ;
extern void set02_06_set( void ) ;
extern void set02_07_set( void ) ;
extern void set03_00_set( void ) ;
extern void set03_01_set( void ) ;
extern void set03_02_set( void ) ;
extern void set03_03_set( void ) ;
extern void set03_04_set( void ) ;
extern void set03_05_set( void ) ;
extern void set03_06_set( void ) ;
extern void set03_07_set( void ) ;
extern void set03_09_set( void ) ;
extern void set04_00_set( void ) ;
extern void set04_01_set( void ) ;
extern void set04_02_set( void ) ;
extern void set04_03_set( void ) ;
extern void set04_04_set( void ) ;
extern void set04_05_set( void ) ;
extern void set04_06_set( void ) ;
extern void set04_07_set( void ) ;
extern void set05_00_set( void ) ;
extern void set05_01_set( void ) ;
extern void set05_03_set( void ) ;
extern void set05_04_set( void ) ;
extern void set05_05_set( void ) ;
extern void set05_06_set( void ) ;
extern void set06_00_set( void ) ;
extern void set06_01_set( void ) ;
extern void set06_02_set( void ) ;
extern void set06_04_set( void ) ;
extern void set07_00_set( void ) ;
extern void set07_02_set( void ) ;
extern void set08_00_set( void ) ;
extern void set08_01_set( void ) ;
extern void set08_02_set( void ) ;
extern void set08_03_set( void ) ;
extern void set08_04_set( void ) ;
extern void set08_05_set( void ) ;


/* the background colour of each stage: its one colour, or the two of a gradient */
typedef struct STAGE_BACK {
	Uint32	col0, col1, col2;
} STAGE_BACK;
static const STAGE_BACK stage_back_tbl[9] = {
	{ 0x007f7f7f, 0x00000000, 0x00000000 },
	{ 0x006061e3, 0x00000000, 0x00000000 },
	{ 0x00000000, 0x00000000, 0x00000000 },
	{ 0x00000000, 0x00b0459a, 0x00000000 },
	{ 0x007f7f7f, 0x00000000, 0x00000000 },
	{ 0x00000000, 0x00000000, 0x00000000 },
	{ 0x007f7f7f, 0x00000000, 0x00000000 },
	{ 0x007f7f7f, 0x00000000, 0x00000000 },
	{ 0x00000000, 0x00000000, 0x00000000 },
} ;

/* the fog of a stage: how many entries of the table fade, its density and its colour */
typedef struct STAGE_FOG {
	Sint32	len;	/* -1 for none */
	Float	density;
	Uint8	unk_08;
	Uint8	r, g, b;
} STAGE_FOG;

static void stage_back_set( void ) ;
static void stage_fog_set( void ) ;
static void stage_obj_set( void ) ;
static void stage_move( void ) ;

static void ( *stage00_obj_tbl[] )( void ) = {
	set00_00_set, set00_01_set, set00_02_set, set00_03_set, set00_04_set,
} ;
static void ( *stage01_obj_tbl[] )( void ) = {
	set01_00_set, set01_01_set, set01_02_set, set01_03_set, set01_04_set, set01_05_set,
} ;
static void ( *stage02_obj_tbl[] )( void ) = {
	set02_00_set, 0, set02_02_set, set02_03_set, set02_04_set, set02_05_set, set02_06_set, set02_07_set,
} ;
static void ( *stage03_obj_tbl[] )( void ) = {
	set03_00_set, set03_01_set, set03_02_set, set03_03_set, set03_04_set, set03_05_set, set03_06_set, set03_07_set, 0, set03_09_set,
} ;
static void ( *stage04_obj_tbl[] )( void ) = {
	set04_00_set, set04_01_set, set04_02_set, set04_03_set, set04_04_set, set04_05_set, set04_06_set, set04_07_set,
} ;
static void ( *stage05_obj_tbl[] )( void ) = {
	set05_00_set, set05_01_set, 0, set05_03_set, set05_04_set, set05_05_set, set05_06_set,
} ;
static void ( *stage06_obj_tbl[] )( void ) = {
	set06_00_set, set06_01_set, set06_02_set, 0, set06_04_set,
} ;
static void ( *stage07_obj_tbl[] )( void ) = {
	set07_00_set, 0, set07_02_set,
} ;
static void ( *stage08_obj_tbl[] )( void ) = {
	set08_00_set, set08_01_set, set08_02_set, set08_03_set, set08_04_set, set08_05_set,
} ;

/* each stage's table of its objects */
static void ( **stage_obj_tbl[9] )( void ) = {
	stage00_obj_tbl, stage01_obj_tbl, stage02_obj_tbl, stage03_obj_tbl, stage04_obj_tbl,
	stage05_obj_tbl, stage06_obj_tbl, stage07_obj_tbl, stage08_obj_tbl,
} ;

/* the order a stage sets its objects in, ended by -1 */
static Sint32 stage00_no_tbl[] = { 0, 1, 2, 3, 4, -1, } ;
static Sint32 stage01_no_tbl[] = { 0, 1, 2, 3, 4, 5, -1, } ;
static Sint32 stage02_no_tbl[] = { 0, 2, 3, 4, 5, 6, 7, -1, } ;
static Sint32 stage03_no_tbl[] = { 0, 1, 2, 3, 4, 5, 6, 7, 9, -1, } ;
static Sint32 stage04_no_tbl[] = { 0, 1, 2, 3, 4, 5, 6, 7, -1, } ;
static Sint32 stage05_no_tbl[] = { 0, 1, 1, 1, 3, 4, 5, 6, -1, } ;
static Sint32 stage06_no_tbl[] = { 0, 1, 2, 4, -1, } ;
static Sint32 stage07_no_tbl[] = { 0, 2, -1, } ;
static Sint32 stage08_no_tbl[] = { 0, 1, 2, 3, 4, 5, -1, } ;

static Sint32 *stage_obj_no[9] = {
	stage00_no_tbl, stage01_no_tbl, stage02_no_tbl, stage03_no_tbl, stage04_no_tbl,
	stage05_no_tbl, stage06_no_tbl, stage07_no_tbl, stage08_no_tbl,
} ;

static STAGE_FOG stage_fog_tbl[9] = {
	{ 82, 12000.0f, 0, 0x80, 0x94, 0xa6 },
	{ 7, 1300000.0f, 0, 0xb3, 0x8c, 0x99 },
	{ -1, 0.0f, 0, 0x7f, 0x7f, 0x7f },
	{ -1, 0.0f, 0, 0x7f, 0x7f, 0x7f },
	{ 128, 30000.0f, 0, 0xff, 0xff, 0xd8 },
	{ 64, 40000.0f, 0, 0x22, 0x22, 0x4b },
	{ 45, 38420.0f, 0, 0x8f, 0x8f, 0xcf },
	{ 110, 1260000.0f, 0, 0xcb, 0x51, 0x46 },
	{ 115, 47000.0f, 0, 0x00, 0x00, 0x00 },
} ;

/* the light's direction: the first stage's, and the others' */
static VEC3 stage_light_tbl[2] = {
	{ 1.0f, 1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f },
} ;

/****************************************************************/
/*			Stage_set: set up a stage's fog, colour, light		*/
/*			and objects											*/
/****************************************************************/
void stage_set( void )
{
	memset( &stage_work, 0, sizeof( stage_work ) ) ;
	stage_work.unk_08 = 1.0f ;
	if( stage_no == 0 ) light_dir_set( &stage_light_tbl[0] ) ;
	else light_dir_set( &stage_light_tbl[1] ) ;
	light_amb_set( 1.0f ) ;
	stage_back_set() ;
	stage_fog_set() ;
	stage_obj_set() ;
}

/****************************************************************/
/*			Each frame of a fight								*/
/****************************************************************/
void stage_frame( void )
{
	stage_back_set() ;
	stage_move() ;
}

/*==============================================================*/
/*			Set the stage's objects in its order				*/
/*==============================================================*/
static void stage_obj_set( void )
{
	void ( *obj_set )( void ) ;
	Sint32 k, no ;

	for( k = 0 ; ( no = stage_obj_no[stage_no][k] ) != -1 ; k ++ ){
		if( ( obj_set = stage_obj_tbl[stage_no][no] ) != 0 ) obj_set() ;
	}
	stage_work.set_cnt ++ ;
}

/*==============================================================*/
static void stage_fog_set( void )
{
	STAGE_FOG *fog = &stage_fog_tbl[stage_no] ;

	if( fog->len == -1 ) fog_work.on = 0 ;
	else {
		fog_work.on = 1 ;
		fog_work.density = fog->density ;
		fog_work.len = fog->len ;
		fog_work.r = bright_work.rate[1] * fog->r ;
		fog_work.g = bright_work.rate[2] * fog->g ;
		fog_work.b = bright_work.rate[3] * fog->b ;
	}
}

static void stage00_move( void ) ;
static void stage04_move( void ) ;

/*==============================================================*/
static void stage_move( void )
{
	switch( stage_no ){
	case 0:
		stage00_move() ;
		break ;
	case 4:
		stage04_move() ;
		break ;
	}
}

/*==============================================================*/
/*			The fog of the first stage thickens as the fight	*/
/*			goes on												*/
/*==============================================================*/
static void stage00_move( void )
{
	Float x ;

	switch( stage_work.step ){
	case 0:
		if( Sys->play_timer >= 2000 ) stage_work.step ++ ;
		break ;
	case 1:
		x = ( Float )( Sys->play_timer - 2000 ) / 500.0f ;
		fog_work.len = 82.0f + x * 18.0f ;
		fog_work.density = 12000.0f + x * 48000.0f ;
		if( Sys->play_timer >= 2500 ) stage_work.step ++ ;
		break ;
	case 2:
		if( Sys->play_timer >= 4500 ) stage_work.step ++ ;
		break ;
	case 3:
		x = ( Float )( Sys->play_timer - 4500 ) / 1500.0f ;
		fog_work.len = 100.0f + x * 20.0f ;
		fog_work.density = 60000.0f + x * 340000.0f ;
		if( Sys->play_timer >= 6000 ) stage_work.step ++ ;
		break ;
	case 4:
		break ;
	}
}

/*==============================================================*/
/*			The sky of the fifth stage moves					*/
/*==============================================================*/
static void stage04_move( void )
{
	Float x ;

	switch( stage_work.step ){
	case 0:
		if( Sys->play_timer >= 720 ) stage_work.step ++ ;
		break ;
	case 1:
		x = ( Float )( Sys->play_timer - 720 ) / 480.0f ;
		fog_work.density = 30000.0f + x * 1000000.0f ;
		if( Sys->play_timer >= 1200 ) stage_work.step ++ ;
		break ;
	}
}

/*==============================================================*/
static void stage_back_set( void )
{
	if( stage_no == 3 || stage_no == 12 ){
		back_color_set2( stage_back_tbl[stage_no].col0, stage_back_tbl[stage_no].col1 ) ;
	} else {
		back_color_set( stage_back_tbl[stage_no].col0 ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "stage.c" End of File	*/
