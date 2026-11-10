/* Mega Man's and Roll's Tornado Hold (character 1c's shot 03): type 0 is the fan that runs along
   the floor, types 1 to 6 are the pieces of the tornado that stand up one above the other where it
   stops. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

static void shl1c_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl1c_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PL1C_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_03_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
		lw->tornado_flag = 0xff ;
	}
	return ew ;
}

/*==============================================================*/
/*			A piece of the tornado rides on the fan hw			*/
/*==============================================================*/
static PLAYER_WORK *shl1c_03_part_set( PLAYER_WORK *hw, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl1c_03_main ;
		ew->master_w = hw ;
		ew->type = no ;
	}
	return ew ;
}

/* where each kind starts, and its animation */
typedef struct SHL1C_03_DATA {
	Sint8	sub_prio;		/* its draw order */
	Uint8	unk_01;
	Sint16	y;		/* its height above the fan */
	Uint8	grp;
	Uint8	no;
} SHL1C_03_DATA;

static SHL1C_03_DATA shl1c_03_data[7] = {
	{ 0, 0, 0, 23, 4 },
	{ 0, 0, 0, 23, 8 },
	{ 0, 0, 8, 23, 6 },
	{ -1, 0, 72, 23, 6 },
	{ -2, 0, 136, 23, 6 },
	{ -3, 0, 200, 23, 6 },
	{ -4, 0, 264, 23, 6 },
} ;

static void shl1c_03_init( PLAYER_WORK *ew );
static void shl1c_03_move( PLAYER_WORK *ew );
static void shl1c_03_end( PLAYER_WORK *ew );
static void shl1c_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_03_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_03_init, shl1c_03_move, shl1c_03_end, shl1c_03_free,
	} ;

	shl1c_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_03_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL1C_03_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c03 ;
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
	dp = &shl1c_03_data[ew->type] ;
	pl_char_set( ew, dp->grp, dp->no ) ;
	ew->prio = 11 ;
	ew->sub_prio = dp->sub_prio ;
	shl1c_03_move( ew ) ;
}

static void shl1c_03_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_03_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_03_c_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_03_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_03_a_move, shl1c_03_b_move, shl1c_03_c_move, shl1c_03_c_move,
		shl1c_03_c_move, shl1c_03_c_move, shl1c_03_c_move,
	} ;

	shl1c_03_move_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl1c_03_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_03_a_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_03_a_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_03_a_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The fan												*/
/*==============================================================*/
static void shl1c_03_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_03_a_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_03_a_init, shl1c_03_a_fly, shl1c_03_a_grow, shl1c_03_a_keep,
	} ;
	PL1C_LOCAL_W *lw ;

	if( pl_w->routine_0 >= 2 || ew->pl_type != pl_w->pl_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	else if( pl_w->routine_1 >= PL_CAUGHT ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
		lw->tornado_flag = 0 ;
		return ;
	}
	shl1c_03_a_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/* its speed by the strength of the attack: x, the change in x, y and the change in y */
static Float shl1c_03_spd_tbl[][4] = {
	{ -10.0f, 0.0f, -DOT_Y( 4.0f ), 0.0f },
	{ -DOT_X( 10.0f ), 0.0f, DOT_Y( 4.0f ), -DOT_Y( 0.75f ) },
} ;

/* the time to set it and the pieces: the hits, the time to the first piece, between pieces and after the last */
static Sint16 shl1c_03_tbl[4] = { 4, 1, 4, 16 } ;

/*==============================================================*/
/*			It starts in front of the character					*/
/*==============================================================*/
static void shl1c_03_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC2 ofs ;

	ew->routine_1 ++ ;
	ew->work2 = 0 ;
	if( pl_w->pl_type == PL_MEGAMAN ){
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 44.0f ) ;
	} else {
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 38.0f ) ;
	}
	if( !pl_w->pl_dir ){
		ew->pos.x = pl_w->pos.x + ofs.x ;
		ew->spd.x = shl1c_03_spd_tbl[pl_w->atck_level][0] ;
		ew->acc.x = shl1c_03_spd_tbl[pl_w->atck_level][1] ;
	} else {
		ew->pos.x = pl_w->pos.x - ofs.x ;
		ew->spd.x = -shl1c_03_spd_tbl[pl_w->atck_level][0] ;
		ew->acc.x = -shl1c_03_spd_tbl[pl_w->atck_level][1] ;
	}
	ew->pos.y = pl_w->pos.y + ofs.y ;
	ew->spd.y = shl1c_03_spd_tbl[pl_w->atck_level][2] ;
	ew->acc.y = shl1c_03_spd_tbl[pl_w->atck_level][3] ;
	ew->obj_flag = shl1c_03_tbl[0] ;
	ew->work1 = 1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl1c_at_item ;
	HIT_RESET( ew ) ;
	shl1c_03_a_fly( ew, pl_w ) ;
}

/*==============================================================*/
/*			It runs along, and stops at the screen's edge or	*/
/*			the floor											*/
/*==============================================================*/
static void shl1c_03_a_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->work2 ){
		if( ew->spd.x < 0.0f ){
			if( ew->pos.x <= scr_work.scr_left + DOT_X( 32.0f ) ){
				ew->work2 = 0xff ;
				ew->spd.x = 0.0f ;
				ew->acc.x = 0.0f ;
			}
		} else if( ew->pos.x >= scr_work.scr_right + -DOT_X( 32.0f ) ){
			ew->work2 = 0xff ;
			ew->spd.x = 0.0f ;
			ew->acc.x = 0.0f ;
		}
	}
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->timer0 = shl1c_03_tbl[1] ;
		ew->prio = 15 ;
		pl_char_set( ew, Char_Effect, 5 ) ;
		pl_se_req( ew, 75 ) ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			The pieces go up one by one							*/
/*==============================================================*/
static void shl1c_03_a_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->timer0 = shl1c_03_tbl[2] ;
		if( !-- ew->obj_flag ){
			ew->routine_1 ++ ;
			ew->timer0 = shl1c_03_tbl[3] ;
		}
		shl1c_03_part_set( ew, ew->work1 ) ;
		ew->work1 ++ ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->atck_no = pl1c_at_item ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_03_a_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;

	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		lw->tornado_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->atck_no = pl1c_at_item ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			The first piece										*/
/*==============================================================*/
static void shl1c_03_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL1C_03_DATA *dp ;

	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		dp = &shl1c_03_data[ew->type] ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + dp->y ;
	}
	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void shl1c_03_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_03_c_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_03_c_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The other pieces									*/
/*==============================================================*/
static void shl1c_03_c_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_03_c_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_03_c_init, shl1c_03_c_keep, shl1c_03_c_end,
	} ;

	shl1c_03_c_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_03_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL1C_03_DATA *dp ;

	ew->routine_1 ++ ;
	dp = &shl1c_03_data[ew->type] ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + dp->y ;
	shl1c_03_c_keep( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_03_c_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_0 >= 2 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 7 ) ;
	}
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			It moves the fan's animation here, not the			*/
/*			piece's												*/
/*==============================================================*/
static void shl1c_03_c_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_char_move( pl_w ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_03.c" End of File	*/
