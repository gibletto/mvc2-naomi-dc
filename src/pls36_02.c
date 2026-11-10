/* Thanos's player effect 02: the chips that fly off the bubble of his Death Sphere (shl36_03), one,
   eight or nine at a time: each starts a little way from it and drifts off at a random speed till
   its time is out. */
#include "player.h"
#include "pl_char.h"
#include "pl36_def.h"

/* the effect's own work, from obj_flag: the player it is for */
typedef struct PLS36_02_LOCAL_W {
	PLAYER_WORK	*w;
} PLS36_02_LOCAL_W;
#define PLS36_02_LW( w )	( ( PLS36_02_LOCAL_W * )&( w )->obj_flag )

/* where each chip starts, from the bubble, by the kind of spread: one, eight and nine */
static Sint16 pls36_02_pos0[1][2] = {
	{ 0, 0 },
} ;
static Sint16 pls36_02_pos1[8][2] = {
	{ 0, -16 }, { 16, -8 }, { 16, 8 }, { 0, 16 }, { -16, -8 }, { -16, 8 }, { 0, 0 }, { 0, 0 },
} ;
static Sint16 pls36_02_pos2[9][2] = {
	{ -21, -21 }, { -21, 0 }, { -21, 21 }, { 0, -21 }, { 0, 0 }, { 0, 21 }, { 21, -21 }, { 21, 0 }, { 21, 21 },
} ;

static void pls36_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls36_02_set( PLAYER_WORK *w, Uint16 no )
{
	PLAYER_WORK *ew ;
	Sint16 *p ;
	Uint8 i, n ;

	p = pls36_02_pos1[0] ;
	n = 8 ;
	if( !no ){
		p = pls36_02_pos0[0] ;
		n = 1 ;
	}
	i = 0 ;
	if( no == 2 ){
		p = pls36_02_pos2[0] ;
		n = 9 ;
	}
	for( i = 0 ; i < n ; i ++ ){
		if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
			ew->work_func = pls36_02_main ;
			ew->master_w = w ;
			ew->pl_type = w->pl_type ;
			ew->type = no ;
			ew->ef_no = 0x3602 ;
			PLS36_02_LW( ew )->w = w->master_w ;
			ew->spd.x = w->spd.x ;
			ew->pos.x = w->pos.x + *p ++ * 1.6666666f ;
			ew->pos.y = w->pos.y + *p ++ * 2.142857f ;
		}
	}
	return ew ;
}

static void pls36_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_02_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls36_02_main( PLAYER_WORK *ew )
{
	static void ( *pls36_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls36_02_init, pls36_02_move, pls36_02_nop, pls36_02_free,
	} ;
	PLAYER_WORK *pl_w = PLS36_02_LW( ew )->w ;

	PL36_LW( pl_w )->shot_timer[0] = 4 ;
	pls36_02_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls36_02_init0( PLAYER_WORK *ew );
static void pls36_02_init1( PLAYER_WORK *ew );
static void pls36_02_init2( PLAYER_WORK *ew );

/*==============================================================*/
static void pls36_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls36_02_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls36_02_init0, pls36_02_init1, pls36_02_init2,
	} ;

	ew->routine_0 ++ ;
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
	ew->prio = 8 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pls36_02_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void pls36_02_init0( PLAYER_WORK *ew )
{
	Sint16 x ;

	x = Rnd() & 0xf0 ;
	if( ew->spd.x > 0.0f ) x = -x ;
	ew->spd.x = x * 1.6666666f / 256.0f ;
	ew->spd.y = ( ( Rnd() & 0xff ) - 128 ) * 2.142857f / 256.0f ;
	ew->timer0 = ( Rnd() & 31 ) + 16 ;
	pl_char_set( ew, Char_Effect, 7 ) ;
}

/*==============================================================*/
static void pls36_02_init1( PLAYER_WORK *ew )
{
	Sint16 x ;
	Uint8 no ;

	x = Rnd() & 63 ;
	if( ew->spd.x > 0.0f ) x = -x ;
	ew->spd.x = x * 1.6666666f / 256.0f ;
	ew->spd.y = ( ( ( Rnd() & 63 ) - 32 ) << 4 ) * 2.142857f / 256.0f ;
	ew->timer0 = ( Rnd() & 31 ) + 8 ;
	no = 6 ;
	if( !( Rnd() & 3 ) ) no = 7 ;
	pl_char_set( ew, Char_Effect, no ) ;
}

/*==============================================================*/
static void pls36_02_init2( PLAYER_WORK *ew )
{
	Uint8 no ;

	ew->spd.x = ( ( ( Rnd() & 63 ) - 32 ) << 4 ) * 1.6666666f / 256.0f ;
	ew->spd.y = ( ( ( Rnd() & 63 ) - 32 ) << 4 ) * 2.142857f / 256.0f ;
	ew->timer0 = ( Rnd() & 31 ) + 8 ;
	no = 6 ;
	if( !( Rnd() & 3 ) ) no = 7 ;
	pl_char_set( ew, Char_Effect, no ) ;
}

/*--------------------------------------------------------------*/
static void pls36_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ) pls36_02_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls36_02_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls36_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls36_02.c" End of File	*/
