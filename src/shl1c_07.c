/* The parts of Mega Man's and Roll's Rush Drill that ride on them (character 1c's shot 07): the
   fourteen kinds, picked by the setter, each follows the player through the hyper combo and ends
   with it. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

/* a kind's start: its place from the player (spd), whether it is turned round, its drawing order
   and its animation */
typedef struct SHL1C_07_DATA {
	Sint16	spd_x;
	Sint16	spd_y;
	Uint8	flip;
	Sint8	order;
	Uint8	char_no;
	Uint8	unk_07;
} SHL1C_07_DATA;

/* a part's own work, at obj_flag */
typedef struct SHL1C_07_LOCAL_W {
	Sint16	flip;			/* 000: set to turn the part round */
	Sint16	unk_02;
} SHL1C_07_LOCAL_W;
#define SHL1C_07_LW( w )	( ( SHL1C_07_LOCAL_W * )&( w )->obj_flag )

static SHL1C_07_DATA shl1c_07_data[14] = {
	{ 0, 0, 0, -2, 0x09, 0 },
	{ 0, 0, 0, -4, 0x0b, 0 },
	{ 0, 0, 0, -1, 0x0c, 0 },
	{ 0, 0, 0, 1, 0x0e, 0 },
	{ 0, 0, 0, -3, 0x0d, 0 },
	{ 7, 9, 0, 1, 0x26, 0 },
	{ -1, 16, 0, 2, 0x26, 0 },
	{ 15, 16, 0, 2, 0x26, 0 },
	{ 16, 16, 0, 1, 0x27, 0 },
	{ 8, 32, 0, 2, 0x27, 0 },
	{ -4, 24, 0, 2, 0x27, 0 },
	{ -20, 4, 1, 1, 0x27, 0 },
	{ -16, 16, 1, 2, 0x27, 0 },
	{ -32, 16, 1, 2, 0x27, 0 },
} ;

static void shl1c_07_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1c_07_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_07_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void shl1c_07_init( PLAYER_WORK *ew );
static void shl1c_07_move( PLAYER_WORK *ew );
static void shl1c_07_end( PLAYER_WORK *ew );
static void shl1c_07_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_07_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_07_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_07_init, shl1c_07_move, shl1c_07_end, shl1c_07_free,
	} ;

	shl1c_07_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_07_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL1C_07_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c07 ;
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
	ew->timer0 = 0 ;
	dp = &shl1c_07_data[ew->type] ;
	ew->spd.x = DOT_X( dp->spd_x ) ;
	ew->spd.y = DOT_Y( dp->spd_y ) ;
	ew->obj_flag = dp->flip ;
	ew->sub_prio = dp->order ;
	pl_char_set( ew, 22, dp->char_no ) ;
	shl1c_07_move( ew ) ;
}

static void shl1c_07_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_type( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_07_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_07_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_07_type, shl1c_07_ck,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl1c_07_pos( ew, pl_w ) ;
	shl1c_07_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl1c_07_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_drill( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_4( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_07_type( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_07_type_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_07_0, shl1c_07_drill, shl1c_07_shot, shl1c_07_3, shl1c_07_4,
		shl1c_07_jump, shl1c_07_jump, shl1c_07_jump, shl1c_07_jump, shl1c_07_jump,
		shl1c_07_jump, shl1c_07_jump, shl1c_07_jump, shl1c_07_jump,
	} ;

	shl1c_07_type_jmp[ew->type]( ew, pl_w ) ;
}

static void shl1c_07_bob( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_07_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->sp_time ){
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		ew->disp_flag = 1 ;
		shl1c_07_ck( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( lw->turn ) ew->disp_flag = 0 ;
	shl1c_07_bob( ew, pl_w ) ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void shl1c_07_drill( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;
	Uint8 no, atck ;

	if( !lw->sp_time ){
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		ew->disp_flag = 1 ;
		shl1c_07_ck( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( lw->turn ) ew->disp_flag = 0 ;
	no = 73 ;
	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		ew->atck_flag = HIT_ON | HIT_OBJ ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
		ew->atck_no = no ;
		HIT_RESET( ew ) ;
		ew->timer1 = 2 ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		if( lw->sp_time <= 10 ){
			atck = 77 ;
		} else {
			atck = no ;
			if( !-- ew->timer1 ){
				ew->timer1 = 2 ;
				atck = 69 ;
			}
		}
		ew->atck_no = atck ;
		HIT_RESET( ew ) ;
	}
	shl1c_07_bob( ew, pl_w ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl1c_07_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( pl_w->routine_3 != 7 || lw->sp_step >= 3 || !lw->sp_time ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void shl1c_07_3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->sp_time ){
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		ew->disp_flag = 1 ;
		shl1c_07_ck( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( lw->turn ) ew->disp_flag = 0 ;
	pl_char_move( ew ) ;
}

static void shl1c_07_4_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_4_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_4_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_4_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_07_4( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_07_4_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_07_4_stand, shl1c_07_4_walk, shl1c_07_4_back, shl1c_07_4_shot,
	} ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->sp_time ){
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		ew->disp_flag = 1 ;
		shl1c_07_ck( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( lw->turn ) ew->disp_flag = 0 ;
	shl1c_07_4_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_07_4_stand( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_3 == 1 ){
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
	} else if( pl_w->routine_3 == 2 ){
		ew->routine_2 = 2 ;
		ew->routine_3 = 0 ;
	} else if( pl_w->routine_3 == 7 ){
		ew->routine_2 = 3 ;
		ew->routine_3 = 0 ;
	}
}

static void shl1c_07_4_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_4_walk_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_07_4_walk( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_07_4_walk_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_07_4_walk_init, shl1c_07_4_walk_move,
	} ;

	shl1c_07_4_walk_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_07_4_walk_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	pl_char_set2( ew, 22, 13, ew->cg_type ) ;
	shl1c_07_4_walk_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_07_4_walk_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_3 != 1 ){
		ew->routine_2 = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void shl1c_07_4_back_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_4_back_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_07_4_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_07_4_back_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_07_4_back_init, shl1c_07_4_back_move,
	} ;

	shl1c_07_4_back_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_07_4_back_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	pl_char_set2( ew, 22, 35, ew->cg_prm ) ;
	shl1c_07_4_back_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_07_4_back_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_3 != 2 ){
		ew->routine_2 = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void shl1c_07_4_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_07_4_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_07_4_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_07_4_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_07_4_shot_init, shl1c_07_4_shot_move,
	} ;

	shl1c_07_4_shot_jmp[ew->routine_3]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_07_4_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_3 ++ ;
	pl_char_set2( ew, 22, 36, ew->cg_type ) ;
	shl1c_07_4_shot_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_07_4_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_3 != 7 ){
		ew->routine_2 = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void shl1c_07_jump( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( lw->sp_step >= 3 || !lw->sp_time ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_07_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_07_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_07_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1c_07_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !( ew->disp_dir = pl_w->disp_dir ^ SHL1C_07_LW( ew )->flip ) ){
		ew->pos.x = pl_w->pos.x + ew->spd.x ;
	} else {
		ew->pos.x = pl_w->pos.x - ew->spd.x ;
	}
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
}

/*==============================================================*/
static void shl1c_07_bob( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 shl1c_07_bob_tbl[16] = {
		0, 1, 0, 2, 0, 0, 0, 1, 2, 0, 1, 0, 2, 0, 0, 0,
	} ;
	PL1C_LOCAL_W *lw ;

	if( pl_w->routine_3 < 3 || pl_w->routine_3 >= 6 ){
		ew->pos.y -= DOT_Y( shl1c_07_bob_tbl[ew->timer0] ) ;
		if( -- ew->timer0 < 0 ) ew->timer0 = 15 ;
	} else {
		lw = PL1C_LW( pl_w ) ;
		if( lw->sp_step <= 1 || lw->sp_step > 3 ) ew->pos.y -= DOT_Y( 5.0f ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_07.c" End of File	*/
