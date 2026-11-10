/* Tron Bonne's player effect 01: the pieces that fly off her and bounce on the ground, eleven
   that turn as they fall (types 0 to 10, from a table of where each lands) and one that bounces
   three times and slides off (type 11). */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"

/* where a piece starts and lands against the player, how high it is thrown, how far it turns,
   its draw priority and animation */
typedef struct PLS10_01_TBL {
	Sint16	x0 ;
	Sint16	y0 ;
	Sint16	x1 ;
	Sint16	y1 ;
	Float	spd_y ;
	Sint16	rot ;
	Sint8	prio ;
	Uint8	no ;
} PLS10_01_TBL ;

static PLS10_01_TBL pls10_01_tbl[11] = {
	{ -8, 56, -42, 26, DOT_Y( 10.0f ), -8, 15, 0 },
	{ 16, 40, 99, 20, DOT_Y( 9.5f ), 8, 15, 1 },
	{ 0, 48, 10, 34, DOT_Y( 8.0f ), 3, 14, 2 },
	{ 24, 40, 63, 13, DOT_Y( 9.0f ), 5, 13, 3 },
	{ -24, 40, -61, 15, DOT_Y( 9.125f ), -5, 12, 4 },
	{ -16, 40, 104, 2, DOT_Y( 9.5f ), 5, 11, 5 },
	{ -8, 40, 71, -11, 21.5625f, -8, 10, 6 },
	{ 0, 40, 54, -9, DOT_Y( 11.5f ), 9, 9, 7 },
	{ 8, 40, -34, 5, DOT_Y( 9.25f ), -7, 9, 8 },
	{ 12, 40, -56, -5, 22.5f, 10, 9, 9 },
	{ -12, 16, -26, -9, 18.75f, -4, 8, 10 },
} ;

/* the turn each frame, in the effect's own work */
typedef struct PLS10_01_LOCAL_W {
	Sint32	rot_spd ;
} PLS10_01_LOCAL_W ;
#define PLS10_01_LW( w )	( ( PLS10_01_LOCAL_W * )&( w )->obj_flag )

static void pls10_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls10_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls10_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

static void pls10_01_init( PLAYER_WORK *ew );
static void pls10_01_move( PLAYER_WORK *ew );
static void pls10_01_end( PLAYER_WORK *ew );
static void pls10_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls10_01_main( PLAYER_WORK *ew )
{
	static void ( *pls10_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls10_01_init, pls10_01_move, pls10_01_end, pls10_01_free,
	} ;

	pls10_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls10_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_0 ++ ;
	ew->ef_no = 0x1001 ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	pls10_01_move( ew ) ;
}

static void pls10_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls10_01_move( PLAYER_WORK *ew )
{
	static void ( *pls10_01_jmp_type[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_01_fly, pls10_01_fly, pls10_01_fly, pls10_01_fly,
		pls10_01_fly, pls10_01_fly, pls10_01_fly, pls10_01_fly,
		pls10_01_fly, pls10_01_fly, pls10_01_fly, pls10_01_bound,
	} ;

	pls10_01_jmp_type[ew->type]( ew, ew->master_w ) ;
}

static void pls10_01_fly_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_fly_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_fly_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static int pls10_01_frames( PLAYER_WORK *ew, Float y_end );

/*==============================================================*/
static void pls10_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_01_jmp_fly[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_01_fly_set, pls10_01_fly_move, pls10_01_fly_end,
	} ;

	pls10_01_jmp_fly[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Picks its speeds so that it lands on its place		*/
/*			in as many frames as it takes to fall				*/
/*==============================================================*/
static void pls10_01_fly_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC2 to ;
	PLS10_01_LOCAL_W *lw = PLS10_01_LW( ew ) ;
	PLS10_01_TBL *tbl ;
	int n ;

	ew->routine_1 ++ ;
	tbl = &pls10_01_tbl[ew->type] ;
	ew->prio = tbl->prio ;
	if( !ew->disp_dir ) ew->pos.x = pl_w->pos.x + DOT_X( tbl->x0 ) ;
	else ew->pos.x = pl_w->pos.x - tbl->x0 * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( tbl->y0 ) ;
	if( !pl_w->disp_dir ) to.x = DOT_X( tbl->x1 ) - tbl->x0 * 1.6666666f ;
	else to.x = -DOT_X( tbl->x1 ) + DOT_X( tbl->x0 ) ;
	to.y = pl_w->ground_y + DOT_Y( tbl->y1 ) ;
	ew->spd.y = tbl->spd_y ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	n = pls10_01_frames( ew, to.y ) ;
	ew->spd.x = to.x / n ;
	ew->acc.x = 0 ;
	lw->rot_spd = tbl->rot * 0x10000 / n ;
	ef_char_set( ew, 27, tbl->no ) ;
	pls10_01_fly_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls10_01_fly_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_01_LOCAL_W *lw = PLS10_01_LW( ew ) ;
	PLS10_01_TBL *tbl ;

	ew->rot[2] += lw->rot_spd ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	tbl = &pls10_01_tbl[ew->type] ;
	if( ew->spd.y <= 0 && ew->pos.y <= pl_w->ground_y + DOT_Y( tbl->y1 ) ){
		ew->routine_1 ++ ;
		ew->rot[2] = 0 ;
		ew->pos.y = pl_w->ground_y + DOT_Y( tbl->y1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_01_fly_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

static void pls10_01_bound_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_bound_first( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_bound_second( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_bound_third( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_bound_slide( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_bound_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls10_01_bound_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls10_01_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls10_01_bound_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls10_01_bound_init, pls10_01_bound_first, pls10_01_bound_second, pls10_01_bound_third,
		pls10_01_bound_slide, pls10_01_bound_wait, pls10_01_bound_anim,
	} ;

	pls10_01_bound_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls10_01_bound_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->prio = 11 ;
	if( !ew->disp_dir ) ew->pos.x = pl_w->pos.x + -DOT_X( 32.0f ) ;
	else ew->pos.x = pl_w->pos.x + DOT_X( 32.0f ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 48.0f ) ;
	if( !ew->disp_dir ) ew->spd.x = -DOT_X( 0.5f ) ;
	else ew->spd.x = DOT_X( 0.5f ) ;
	ew->acc.x = 0 ;
	ew->spd.y = DOT_Y( 9.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	ef_char_set( ew, 27, 11 ) ;
	pls10_01_bound_first( ew, pl_w ) ;
}

/*==============================================================*/
/*			The first bounce									*/
/*==============================================================*/
static void pls10_01_bound_first( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y + 0.0f ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y + 0.0f ;
		ew->spd.y = -( ew->spd.y / 4.0f ) ;
	}
}

/*==============================================================*/
/*			The second											*/
/*==============================================================*/
static void pls10_01_bound_second( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y + 0.0f ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y + 0.0f ;
		ew->spd.y = -( ew->spd.y / 4.0f ) ;
	}
}

/*==============================================================*/
/*			The third, after which it slides and turns			*/
/*==============================================================*/
static void pls10_01_bound_third( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_01_LOCAL_W *lw = PLS10_01_LW( ew ) ;

	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y + 0.0f ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y + 0.0f ;
		if( !ew->disp_dir ) ew->spd.x = -1.6666666f ;
		else ew->spd.x = 1.6666666f ;
		ew->acc.x = 0 ;
		ew->spd.y = 0 ;
		ew->acc.y = 0 ;
		ew->timer0 = 60 ;
		lw->rot_spd = ( Uint16 )( 0x10000 / ew->timer0 ) ;
		ef_char_set2( ew, 27, 11, 2 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_01_bound_slide( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS10_01_LOCAL_W *lw = PLS10_01_LW( ew ) ;

	ew->rot[2] += lw->rot_spd ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 20 ;
		ew->rot[2] = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_01_bound_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( -- ew->timer0 == 0 ){
		ew->routine_1 ++ ;
		ef_char_set( ew, 27, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls10_01_bound_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls10_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls10_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The frames a piece thrown from where it is takes	*/
/*			to fall to y_end									*/
/*==============================================================*/
static int pls10_01_frames( PLAYER_WORK *ew, Float y_end )
{
	Float y = ew->pos.y ;
	Float vy = ew->spd.y ;
	int n = 0 ;

	do {
		y += vy ;
		vy += ew->acc.y ;
		n ++ ;
	} while( vy > 0 || y > y_end ) ;
	return n ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls10_01.c" End of File	*/
