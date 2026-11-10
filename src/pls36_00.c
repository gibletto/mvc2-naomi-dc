/* Thanos's player effect 00: the sparks and chips that fly off the blasts of Gauntlet Reality
   (shl36_01): type 0 falls and breaks into three of type 1, which fall and go out at the ground. */
#include "player.h"
#include "ef_char.h"
#include "pl36_def.h"

/* the effect's own work, from obj_flag: the work that made it */
typedef struct PLS36_00_LOCAL_W {
	PLAYER_WORK	*w;
} PLS36_00_LOCAL_W;
#define PLS36_00_LW( w )	( ( PLS36_00_LOCAL_W * )&( w )->obj_flag )

static void pls36_00_main( PLAYER_WORK *ew );

/****************************************************************/
/*			No: its type in the high byte, sub_type in the		*/
/*			low													*/
/****************************************************************/
PLAYER_WORK *pls36_00_set( PLAYER_WORK *w, Float x, Float y, Uint16 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls36_00_main ;
		ew->master_w = w->master_w ;
		ew->pl_type = w->pl_type ;
		PLS36_00_LW( ew )->w = w ;
		ew->type = no >> 8 ;
		ew->sub_type = no ;
		ew->ef_no = 0x3600 ;
		ew->pos.x = x ;
		ew->pos.y = y ;
		ew->dir = w->dir ;
		w->dir ^= 1 ;
	}
	return ew ;
}

static void pls36_00_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_00_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls36_00_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->sub_type ) PL36_LW( pl_w )->hyper_timer = 4 ;
	if( !ew->type ) pls36_00_main0( ew, pl_w ) ;
	else pls36_00_main1( ew, pl_w ) ;
}

static void pls36_00_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_00_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls36_00_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls36_00_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls36_00_init0, pls36_00_move0, pls36_00_end, pls36_00_end,
	} ;

	pls36_00_jmp0[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls36_00_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint32 no ;

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
	ew->dir = 0 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	no = Rnd() & 0xff ;
	no -= 128 ;
	ew->spd.x = ( Sint32 )( no << 9 ) * 1.6666666f / 65536.0f ;
	ew->acc.y = -0.4017857f ;
	ef_char_set( ew, 27, ( Rnd() & 3 ) + 5 ) ;
}

/*--------------------------------------------------------------*/
static void pls36_00_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint16 i ;
	Uint16 no ;

	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > pl_w->ground_y ) return ;
	ew->routine_0 ++ ;
	for( i = 0 ; i < 3 ; i ++ ){
		no = 0x100 + ew->sub_type ;
		pls36_00_set( ew, ew->pos.x, ew->pos.y + DOT_Y( 4.0f ), no ) ;
	}
}

static void pls36_00_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls36_00_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls36_00_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls36_00_jmp1[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls36_00_init1, pls36_00_move1, pls36_00_end, pls36_00_end,
	} ;

	pls36_00_jmp1[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls36_00_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint32 no ;

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
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	no = Rnd() & 0xff ;
	no -= 128 ;
	ew->spd.x = ( Sint32 )( no << 7 ) * 1.6666666f / 65536.0f ;
	no = Rnd() & 0xff ;
	ew->spd.y = ( Sint32 )( ( no + 96 ) << 10 ) * 2.142857f / 65536.0f ;
	ew->acc.y = -0.4017857f ;
	ef_char_set( ew, 27, ( Rnd() & 3 ) + 9 ) ;
	pls36_00_move1( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls36_00_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->disp_flag = ( Sys->game_timer0 + ew->dir ) & 1 ;
	if( ew->pos.y <= pl_w->ground_y ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls36_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls36_00.c" End of File	*/
