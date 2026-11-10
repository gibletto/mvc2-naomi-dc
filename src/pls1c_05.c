/* Mega Man's player effect 05: the item that pls1c_04's picture throws: it rises and
   falls in front of the player (types 0 to 2, the item) with two parts that follow (types 3 and 4).
   When it touches the player it becomes their item and makes a picture (pls1c_00) to wear. */
#include "player.h"
#include "ef_char.h"
#include "pl1c_def.h"

static void pls1c_05_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Made by pls1c_04, which it rides on					*/
/****************************************************************/
PLAYER_WORK *pls1c_05_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1c_05_main ;
		ew->type = type ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
	}
	return ew ;
}

/*==============================================================*/
/*			The two parts, made by the shot itself				*/
/*==============================================================*/
static PLAYER_WORK *pls1c_05_part_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_PLS, LIST_BEHIND ) ) ){
		ew->work_func = pls1c_05_main ;
		ew->type = type ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

/* the shot's animation of group 27, for Mega Man (first three) and for Roll */
static Uint8 pls1c_05_no[2][3] = { { 11, 12, 10 }, { 11, 12, 10 } } ;

static void pls1c_05_a( PLAYER_WORK *ew );
static void pls1c_05_b( PLAYER_WORK *ew );
static void pls1c_05_c( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_05_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_05_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_05_a, pls1c_05_a, pls1c_05_a, pls1c_05_b, pls1c_05_c,
	} ;

	pls1c_05_jmp[ew->type]( ew ) ;
}

static void pls1c_05_init( PLAYER_WORK *ew );
static void pls1c_05_move( PLAYER_WORK *ew );
static void pls1c_05_end( PLAYER_WORK *ew );
static void pls1c_05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_05_a( PLAYER_WORK *ew )
{
	static void ( *pls1c_05_a_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_05_init, pls1c_05_move, pls1c_05_end, pls1c_05_free,
	} ;

	pls1c_05_a_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_05_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *ef_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0xc054 ;	/* not 0x1c05 */
	ew->timer0 = 300 ;
	pl_w = ew->master_w ;
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
	ew->prio = 9 ;
	ew->size_top = 12 ;
	ew->size_bottom = 12 ;
	ew->size_front = 12 ;
	ew->size_back = 12 ;
	ew->disp_dir = 0 ;
	ef_w = ew->ef_w ;
	if( !ef_w->disp_dir ){
		ew->pos.x = ef_w->pos.x + -DOT_X( 2.0f ) ;
		ew->spd.x = -2.5f ;
	} else {
		ew->pos.x = ef_w->pos.x + DOT_X( 2.0f ) ;
		ew->spd.x = 2.5f ;
	}
	ew->acc.x = 0.0f ;
	ew->pos.y = ef_w->pos.y + DOT_Y( 27.0f ) ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->pl_type == PL_MEGAMAN ){
		ew->char_no.b.no = pls1c_05_no[0][ew->type] ;
	} else {
		ew->char_no.b.no = pls1c_05_no[1][ew->type] ;
	}
	ef_char_set( ew, 27, ew->char_no.b.no ) ;
	pls1c_05_part_set( ew, 3 ) ;
	pls1c_05_part_set( ew, 4 ) ;
	pls1c_05_move( ew ) ;
}

static void pls1c_05_start( PLAYER_WORK *ew );
static void pls1c_05_up( PLAYER_WORK *ew );
static void pls1c_05_fall( PLAYER_WORK *ew );
static void pls1c_05_wait( PLAYER_WORK *ew );
static void pls1c_05_blink( PLAYER_WORK *ew );
static void pls1c_05_blink_fast( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls1c_05_move( PLAYER_WORK *ew )
{
	static void ( *pls1c_05_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_05_start, pls1c_05_up, pls1c_05_fall, pls1c_05_wait, pls1c_05_blink, pls1c_05_blink_fast,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( --ew->timer0 == 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->timer0 > 60 ){
		if( ew->pl_type != pl_w->pl_type || pl_w->routine_0 >= 2 ){
			ew->routine_0 = 2 ;
			ew->routine_1 = 0 ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	if( ew->timer0 > 20 ){
		if( pl_w->act_st == N_SPECIAL ){
			if( pl_w->pl_sp_type == pl1c_sp_hyper_rock || pl_w->pl_sp_type == pl1c_sp_drill
			 || pl_w->pl_sp_type == pl1c_sp_beat || pl_w->pl_sp_type == pl1c_sp_13 ){
				ew->routine_1 = 5 ;
				ew->timer0 = 20 ;
				return ;
			}
		}
	}
	pls1c_05_move_jmp[ew->routine_1]( ew ) ;
	ef_char_move( ew ) ;
}

static void pls1c_05_hit_ck( PLAYER_WORK *ew );

/*==============================================================*/
/*			Goes up												*/
/*==============================================================*/
static void pls1c_05_start( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	pls1c_05_up( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_05_up( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.y <= 0.0f ) ew->routine_1 ++ ;
}

/*==============================================================*/
/*			Comes down to just above the floor					*/
/*==============================================================*/
static void pls1c_05_fall( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y + DOT_Y( 10.0f ) ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y + DOT_Y( 10.0f ) ;
	}
	pls1c_05_hit_ck( ew ) ;
}

/*==============================================================*/
/*			Waits												*/
/*==============================================================*/
static void pls1c_05_wait( PLAYER_WORK *ew )
{
	if( ew->timer0 <= 60 ) ew->routine_1 ++ ;
	pls1c_05_hit_ck( ew ) ;
}

/*==============================================================*/
/*			Waits, then flashes									*/
/*==============================================================*/
static void pls1c_05_blink( PLAYER_WORK *ew )
{
	if( ew->timer0 <= 20 ) ew->routine_1 ++ ;
	if( ew->timer0 & 3 ){
		ew->disp_flag = 1 ;
	} else {
		ew->disp_flag = 0 ;
	}
	pls1c_05_hit_ck( ew ) ;
}

/*==============================================================*/
/*			Flashes fast										*/
/*==============================================================*/
static void pls1c_05_blink_fast( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->timer0 & 1 ;
	pls1c_05_hit_ck( ew ) ;
}

static void pls1c_05_b_init( PLAYER_WORK *ew );
static void pls1c_05_b_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_05_b( PLAYER_WORK *ew )
{
	static void ( *pls1c_05_b_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_05_b_init, pls1c_05_b_move, pls1c_05_end, pls1c_05_free,
	} ;

	pls1c_05_b_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_05_b_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c05 ;
	pl_w = ew->master_w ;
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
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	ef_char_set( ew, 27, 8 ) ;
	pls1c_05_b_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_05_b_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos = pl_w->pos ;
	ew->disp_flag = pl_w->disp_flag ;
	ef_char_move( ew ) ;
}

static void pls1c_05_c_init( PLAYER_WORK *ew );
static void pls1c_05_c_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_05_c( PLAYER_WORK *ew )
{
	static void ( *pls1c_05_c_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_05_c_init, pls1c_05_c_move, pls1c_05_end, pls1c_05_free,
	} ;

	pls1c_05_c_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_05_c_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c05 ;
	pl_w = ew->master_w ;
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
	ew->prio = pl_w->prio ;
	ew->sub_prio = 1 ;
	ef_char_set( ew, 27, 9 ) ;
	pls1c_05_c_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_05_c_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos = pl_w->pos ;
	ew->disp_flag = pl_w->disp_flag ;
}

/*--------------------------------------------------------------*/
static void pls1c_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The player has it: it ends, and the player wears	*/
/*			the picture for its kind							*/
/*==============================================================*/
static void pls1c_05_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 pls1c_05_pic[3] = { 47, 48, 49 } ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	ew->routine_0 = 2 ;
	lw->item = ew->type ;
	ew->obj_flag = pl_w->pl_no ;
	pl_se_req( ew, 42 ) ;
	pls1c_00_set( ew, pls1c_05_pic[ew->type] ) ;
}

static int pls1c_05_box_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Whether the shot is in the box of the player or		*/
/*			of the opponent, when he is Mega Man or Roll too	*/
/*==============================================================*/
static void pls1c_05_hit_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->shield_flag ){
		if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != pl1c_sp_item ){
			if( pls1c_05_box_ck( ew, pl_w ) ){
				pls1c_05_hit( ew, pl_w ) ;
				return ;
			}
		}
	}
	pl_w = pl_w->target_w ;
	if( pl_w->pl_type == PL_MEGAMAN || pl_w->pl_type == PL_ROLL ){
		lw = PL1C_LW( pl_w ) ;
		if( !lw->shield_flag ){
			if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != pl1c_sp_item ){
				if( pls1c_05_box_ck( ew, pl_w ) ){
					pls1c_05_hit( ew, pl_w ) ;
				}
			}
		}
	}
}

/*==============================================================*/
/*			Whether the shot is inside the player's catch		*/
/*			box of this frame									*/
/*==============================================================*/
static int pls1c_05_box_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	HIT_BOX *box = &pl_w->hit_box_tbl[pl_w->frame_box->catch_box] ;
	Float x, y, bx, by ;

	bx = DOT_X( box->x ) ;
	x = ew->pos.x ;
	if( pl_w->disp_dir ) bx = -bx ;
	bx += pl_w->pos.x ;
	x -= bx ;
	if( x < 0.0f ) x = -x ;
	if( DOT_X( box->w ) == 0.0f || !( DOT_X( box->w ) > x ) ) return 0 ;
	y = ew->pos.y ;
	by = DOT_Y( box->y ) ;
	by = -by + pl_w->pos.y ;
	y -= by ;
	if( y < 0.0f ) y = -y ;
	if( DOT_Y( box->h ) != 0.0f && box->h * 2.142857f > y ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_05.c" End of File	*/
