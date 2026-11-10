/* Marrow's effect 01 (character 16's shot 01): the sixteen thorns of her hyper Bone Burst.
   Each is made in a ring around her, turns towards the opponent a step at a time on a curved path
   and strikes from then on. All sixteen read one shared aim kept in the work of the one being run. */
#include "player.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl16_def.h"

/* one thorn: its animation, its turn and its first direction (32 to a circle) */
typedef struct SHL16_01_TBL {
	Uint8	char_no;
	Uint8	unk_1;
	Sint16	rot;
	Sint16	dir;
} SHL16_01_TBL;

/* the thorn's own work, from obj_flag */
typedef struct SHL16_01_LOCAL_W {
	Sint16	spd;		/* 00: the speed along the curve, up by 50 each frame */
	Sint8	turn_cnt;		/* 02: turns still to make, 16 at the start */
	Sint8	turn_timer;		/* 03: frames to the next turn */
	VEC3	aim_pos;		/* 04: where it aims, above the opponent */
} SHL16_01_LOCAL_W;
#define SHL16_01_LW( w )	( ( SHL16_01_LOCAL_W * )&( w )->obj_flag )

static SHL16_01_TBL shl16_01_tbl[] = {
	{ 0, 0, 0x4000, 0 }, { 2, 0, 0x5000, 2 }, { 0, 0, 0x6000, 4 }, { 1, 0, 0x7000, 6 },
	{ 0, 0, -0x8000, 8 }, { 1, 0, -0x7000, 10 }, { 0, 0, -0x6000, 12 }, { 2, 0, -0x5000, 14 },
	{ 0, 0, -0x4000, 16 }, { 2, 0, -0x3000, 18 }, { 0, 0, -0x2000, 20 }, { 1, 0, -0x1000, 22 },
	{ 0, 0, 0, 24 }, { 1, 0, 0x1000, 26 }, { 0, 0, 0x2000, 28 }, { 2, 0, 0x3000, 30 },
} ;

static Float shl16_01_acc_tbl[] = {
	0.0f, 0.12f, 0.0f, 0.07f, 0.0f, 0.07f, 0.0f, 0.12f,
	0.0f, 0.12f, 0.0f, 0.07f, 0.0f, 0.07f, 0.0f, 0.12f,
} ;

static Uint8 shl16_01_turn_tbl[] = { 1, 3, 2, 4, 1, 3, 2, 4, 1, 3, 2, 4, 1, 3, 2, 4 } ;

static SHL16_01_LOCAL_W *shl16_01_lw ;

static void shl16_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl16_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 16 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl16_01_main ;
			ew->master_w = pl_w ;
			ew->ef_no = 0x1601 ;
			ew->sub_type = i ;
		} else break ;
	}
	return ew ;
}

static void shl16_01_init( PLAYER_WORK *ew );
static void shl16_01_move( PLAYER_WORK *ew );
static void shl16_01_strike( PLAYER_WORK *ew );
static void shl16_01_end( PLAYER_WORK *ew );
static void shl16_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl16_01_main( PLAYER_WORK *ew )
{
	static void ( *shl16_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl16_01_init, shl16_01_move, shl16_01_strike, shl16_01_end, shl16_01_free,
	} ;

	shl16_01_lw = SHL16_01_LW( ew ) ;
	shl16_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl16_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_top = 255 ;
	ew->size_bottom = 255 ;
	ew->size_front = 128 ;
	ew->size_back = 128 ;
	ew->prio = 11 ;
	pl_char_set( ew, Char_Effect, shl16_01_tbl[ew->sub_type].char_no + 1 ) ;
	ew->timer1 = 1 ;
	ew->rot[2] = shl16_01_tbl[ew->sub_type].rot ;
	ew->dir = shl16_01_tbl[ew->sub_type].dir ;
	if( !ew->disp_dir ) ew->dir = ( 32 - ew->dir ) & 31 ;
	shl16_01_lw->turn_cnt = 16 ;
	shl16_01_lw->spd = 500 ;
	shl16_01_lw->turn_timer = shl16_01_turn_tbl[ew->sub_type] ;
	shl16_01_lw->aim_pos = pl_w->target_w->pos ;
	shl16_01_lw->aim_pos.y += DOT_Y( 32.0f ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 96.0f ) ;
	if( ew->disp_dir ) ew->pos.x -= nlCos( ew->rot[2] ) * 60.0f ;
	else ew->pos.x += nlCos( ew->rot[2] ) * 60.0f ;
	ew->pos.y += nlSin( ew->rot[2] ) * 60.0f ;
	ew->timer0 = 0 ;
	ew->type = 0 ;
	ew->acc.x = shl16_01_acc_tbl[ew->sub_type] ;
	ew->atck_no = pl16_at_burst ;
	HIT_RESET( ew ) ;
	shl16_01_move( ew ) ;
}

static void shl16_01_aim( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl16_01_move( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->timer1 -- ;
	if( ew->timer1 <= 0 ){
		ew->timer1 = 0 ;
		shl16_01_aim( ew ) ;
		dir_move( ew, shl16_01_lw->spd ) ;
		shl16_01_lw->spd += 50 ;
	}
	if( !scr_in_ck( ew ) ){
		shl16_01_end( ew ) ;
		return ;
	}
	if( ew->hit_flag ) ew->routine_0 ++ ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			After a hit it flies straight on, striking up to	*/
/*			five times											*/
/*==============================================================*/
static void shl16_01_strike( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	dir_move( ew, shl16_01_lw->spd ) ;
	shl16_01_lw->spd += 50 ;
	if( !scr_in_ck( ew ) ){
		shl16_01_end( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
	if( ++ ew->type < 5 ){
		ew->atck_no = ew->type + 62 ;
		HIT_RESET( ew ) ;
	} else return ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Turns its direction a step towards the aim,			*/
/*			until it faces it									*/
/*==============================================================*/
static void shl16_01_aim( PLAYER_WORK *ew )
{
	Uint8 to ;
	int turn ;

	if( !shl16_01_lw->turn_cnt ) return ;
	if( -- shl16_01_lw->turn_timer ) return ;
	shl16_01_lw->turn_timer = shl16_01_turn_tbl[ew->sub_type] ;
	shl16_01_lw->turn_cnt -- ;
	to = caldir_pos_256( &ew->pos, &shl16_01_lw->aim_pos ) / 8 ;
	if( to == ew->dir ) shl16_01_lw->turn_cnt = 0 ;
	else {
		if( ( ( to - ew->dir ) & 31 ) >= 15 ) turn = 255 ;
		else turn = 1 ;
		ew->dir = ( ew->dir + turn ) & 31 ;
		if( ew->disp_dir ) ew->rot[2] = ( Uint16 )( ew->dir * 2048 + 0x4000 ) ;
		else ew->rot[2] = ( Uint16 )( 0x4000 - ew->dir * 2048 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl16_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl16_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl16_01.c" End of File	*/
