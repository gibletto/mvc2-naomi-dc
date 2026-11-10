/* Roll's effect 00 (character 1d's shot 00): four kinds, picked by the setter, that start beside
   her during her Flower Present, then fly off on their speed until they hit or touch the ground. */
#include "player.h"
#include "pl_char.h"
#include "pl1d_def.h"

/* one kind: its speed and its change (16.16 fixed point), its animation and its attack */
typedef struct SHL1D_00_DATA {
	Sint32	spd_x;
	Sint32	acc_x;
	Sint32	spd_y;
	Sint32	acc_y;
	Uint8	char_no;
	Uint8	atck_no;
} SHL1D_00_DATA;

/* where a kind sits beside Roll, by her animation frame */
typedef struct SHL1D_00_POS {
	Sint16	x;
	Sint16	y;
} SHL1D_00_POS;

static void shl1d_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1d_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;
	PL1D_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1d_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		lw = PL1D_LW( pl_w ) ;
		lw->shot_cnt ++ ;
	}
	return ew ;
}

static SHL1D_00_DATA shl1d_00_data[4] = {
	{ -4 << 16, 0x1000, 3 << 16, -0x2000, 26, 51 },
	{ -8 << 16, 0x2000, 4 << 16, -0x2000, 26, 51 },
	{ -4 << 16, 0x1000, 3 << 16, -0x2000, 26, 52 },
	{ -8 << 16, 0x2000, 4 << 16, -0x2000, 26, 52 },
} ;

static void shl1d_00_init( PLAYER_WORK *ew );
static void shl1d_00_move( PLAYER_WORK *ew );
static void shl1d_00_end( PLAYER_WORK *ew );
static void shl1d_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1d_00_main( PLAYER_WORK *ew )
{
	static void ( *shl1d_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl1d_00_init, shl1d_00_move, shl1d_00_end, shl1d_00_free,
	} ;

	shl1d_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1d_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL1D_00_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1d00 ;
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
	ew->prio = 11 ;
	ew->col_flag = 1 ;
	ew->col_no += 6 ;
	dp = &shl1d_00_data[ew->type] ;
	if( !ew->disp_dir ){
		ew->spd.x = dp->spd_x * 1.6666666f / 65536.0f ;
		ew->acc.x = dp->acc_x * 1.6666666f / 65536.0f ;
	} else {
		ew->spd.x = -( dp->spd_x * 1.6666666f / 65536.0f ) ;
		ew->acc.x = -( dp->acc_x * 1.6666666f / 65536.0f ) ;
	}
	ew->spd.y = dp->spd_y * 2.142857f / 65536.0f ;
	ew->acc.y = dp->acc_y * 2.142857f / 65536.0f ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = dp->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, dp->char_no ) ;
	shl1d_00_move( ew ) ;
}

static void shl1d_00_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It follows Roll's Flower Present until it is let	*/
/*			go													*/
/*==============================================================*/
static void shl1d_00_move( PLAYER_WORK *ew )
{
	static void ( *shl1d_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1d_00_wait, shl1d_00_fly,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_w->act_st == N_SUPER ){
		ew->col_flag = pl_w->col_flag ;
		ew->col_no = pl_w->col_no ;
	}
	shl1d_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl1d_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1d_00_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->cg_type ){
		ew->routine_1 ++ ;
		return ;
	}
	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != pl1d_sp_flower ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1d_00_pos_set( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1d_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->routine_2 ){
		if( ew->spd.x * ew->acc.x > 0.0f ){
			ew->routine_2 ++ ;
			ew->spd.x = ew->acc.x = 0.0f ;
		}
	}
	if( !ew->hit_flag && !ew->hit_box_no ){
		pos.y = pl_w->ground_y + DOT_Y( 8.0f ) ;
		if( ew->pos.y <= pos.y ) ew->pos.y = pos.y ;
		else {
			hit_work_set( ew ) ;
			return ;
		}
	}
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pos.x = ew->pos.x ;
	pos.y = ew->pos.y - DOT_Y( 8.0f ) ;
	burst_set( ew, &pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
}

/*--------------------------------------------------------------*/
static void shl1d_00_end( PLAYER_WORK *ew )
{
	PL1D_LOCAL_W *lw = PL1D_LW( ew->master_w ) ;

	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	if( lw->shot_cnt ) lw->shot_cnt -- ;
}

/*--------------------------------------------------------------*/
static void shl1d_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1d_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static SHL1D_00_POS shl1d_00_pos[4] = {
		{ 11, 41 }, { -20, 72 }, { 22, 48 }, { -6, 71 },
	} ;

	if( pl_w->disp_dir == 0 ) ew->pos.x = pl_w->pos.x + DOT_X( shl1d_00_pos[pl_w->cg_prm].x ) ;
	else ew->pos.x = pl_w->pos.x - shl1d_00_pos[pl_w->cg_prm].x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( shl1d_00_pos[pl_w->cg_prm].y ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1d_00.c" End of File	*/
