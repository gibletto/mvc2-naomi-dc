/* Jin's effect 00 (character 37's shot 00): the ten sparks of his Saotome Dynamite, each thrown out
   from its own place by him in its own direction. They slow down on the way and flicker away a few
   frames after a hit. */
#include "player.h"
#include "pl_char.h"
#include "pl37_def.h"

/* one spark: where it starts from Jin (x and y), its direction, and its animation */
typedef struct SHL37_00_SPARK {
	Sint16	x, y;
	Sint16	dir;
	Sint16	char_no;
} SHL37_00_SPARK;

/* the effect's own work, from obj_flag */
typedef struct SHL37_00_LOCAL_W {
	Sint8	cnt;		/* 000: frames to the next slowing down */
	Sint8	spd_no;		/* 001: its place in the speed table */
} SHL37_00_LOCAL_W;
#define SHL37_00_LW( w )	( ( SHL37_00_LOCAL_W * )&( w )->obj_flag )

static SHL37_00_SPARK shl37_00_spark_tbl[] = {
	{ -37, 23, 23, 15 }, { -27, 44, 24, 17 }, { -2, 45, 26, 19 }, { -31, 82, 28, 11 },
	{ -17, 88, 30, 13 }, { -4, 59, 0, 20 }, { 11, 84, 2, 14 }, { 34, 84, 4, 12 },
	{ 13, 41, 6, 18 }, { 25, 26, 8, 16 },
} ;

static Sint8 shl37_00_cnt_tbl[] = {
	2, 4, 2, 5, 2, 6, 2, 4, 2, 4, 2, 6, 1, 5, 1, 3, 1, 4, 1, 2,
} ;

static Sint16 shl37_00_spd_tbl[] = {
	100, 300, 500, 700, 900, 1100, 1300, 1500, 1700, 1900, 2100, 2300,
} ;

static void shl37_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl37_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl37_00_main ;
		ew->ef_no = 0x3700 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl37_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_00_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_00_main( PLAYER_WORK *ew )
{
	static void ( *shl37_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_00_type_0,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp != Char_Special ){
		shl37_00_free( ew, pl_w ) ;
		return ;
	}
	shl37_00_jmp[ew->type]( ew, pl_w ) ;
}

static void shl37_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_00_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_00_type_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_00_init, shl37_00_move, shl37_00_end, shl37_00_free,
	} ;

	shl37_00_type_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_00_LOCAL_W *lw = SHL37_00_LW( ew ) ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->atck_no = ew->sub_type + 60 ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( shl37_00_spark_tbl[ew->sub_type].x ) ;
	else ew->pos.x -= DOT_X( shl37_00_spark_tbl[ew->sub_type].x ) ;
	ew->pos.y += DOT_Y( shl37_00_spark_tbl[ew->sub_type].y ) ;
	ew->dir = shl37_00_spark_tbl[ew->sub_type].dir ;
	if( ew->disp_dir ){
		ew->dir = 32 - ew->dir ;
		ew->dir &= 31 ;
	}
	lw->cnt = shl37_00_cnt_tbl[ew->sub_type * 2] ;
	lw->spd_no = shl37_00_cnt_tbl[ew->sub_type * 2 + 1] ;
	dir_spd_move( ew, shl37_00_spd_tbl[lw->spd_no] ) ;
	pl_char_set( ew, Char_Effect, shl37_00_spark_tbl[ew->sub_type].char_no ) ;
	ew->timer0 = 20 ;
	shl37_00_move( ew, pl_w ) ;
}

static void shl37_00_fly( PLAYER_WORK *ew );
static void shl37_00_flicker( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl37_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl37_00_fly, shl37_00_flicker,
	} ;

	ew->prio = pl_w->prio ;
	shl37_00_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			It slows down every few frames until it hits;		*/
/*			then it lasts 16 more								*/
/*==============================================================*/
static void shl37_00_fly( PLAYER_WORK *ew )
{
	SHL37_00_LOCAL_W *lw = SHL37_00_LW( ew ) ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ! -- ew->timer0 ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ! -- lw->cnt ){
		lw->cnt = 2 ;
		if( lw->spd_no ) lw->spd_no -- ;
		dir_spd_move( ew, shl37_00_spd_tbl[lw->spd_no] ) ;
	}
	if( ew->hit_box_no ){
		ew->routine_1 ++ ;
		ew->timer0 = 16 ;
		return ;
	}
	hit_work_set( ew ) ;
	if( ew->hit_flag ){
		ew->routine_1 ++ ;
		ew->timer0 = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_00_flicker( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->disp_flag ^= 1 ;
	if( ! -- ew->timer0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl37_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl37_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl37_00.c" End of File	*/
