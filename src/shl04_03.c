/* Anakaris's effect 03 (character 04's shot 03): the rows of his Pharaoh Cobra Throw. Each button
   sends a head (type 0) out along its row, which leaves a body part (type 1) every 80 dots, and
   when it reaches the screen's edge it waits, then draws back into him with its parts. */
#include "player.h"
#include "pl_char.h"
#include "pl04_def.h"

static void shl04_03_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A head, on row no (0 to 3)							*/
/****************************************************************/
PLAYER_WORK *shl04_03_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl04_03_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->dir = no ;
		ew->ef_no = 0x403 ;
	}
	return ew ;
}

/*==============================================================*/
/*			Body part no, behind the head w						*/
/*==============================================================*/
static PLAYER_WORK *shl04_03_body_set( PLAYER_WORK *w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl04_03_main ;
		ew->master_w = w->master_w ;
		ew->ef_w = w ;
		ew->type = type ;
		ew->dir = no ;
		ew->ef_no = 0x403 ;
	}
	return ew ;
}

static void shl04_03_init( PLAYER_WORK *ew );
static void shl04_03_move( PLAYER_WORK *ew );
static void shl04_03_end( PLAYER_WORK *ew );
static void shl04_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_03_main( PLAYER_WORK *ew )
{
	static void ( *shl04_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_03_init, shl04_03_move, shl04_03_end, shl04_03_free,
	} ;

	shl04_03_jmp[ew->routine_0]( ew ) ;
}

static void shl04_03_head_init( PLAYER_WORK *ew );
static void shl04_03_body_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_03_init( PLAYER_WORK *ew )
{
	static void ( *shl04_03_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_03_head_init, shl04_03_body_init,
	} ;

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
	ew->disp_flag = 1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_se_req( ew, 75 ) ;
	shl04_03_init_jmp[ew->type]( ew ) ;
	shl04_03_move( ew ) ;
}

/*==============================================================*/
/*			The head: sub_type keeps its row, dir counts its	*/
/*			parts, spd.z is how far out it is, and acc.x how	*/
/*			far it has gone since the last part.				*/
/*==============================================================*/
static void shl04_03_head_init( PLAYER_WORK *ew )
{
	static const Float shl04_03_y_tbl[] = {
		240.0f, DOT_Y( 88.0f ), DOT_Y( 64.0f ), DOT_Y( 40.0f ), DOT_Y( 48.0f ), 240.0f,
	} ;
	static const Uint8 shl04_03_prio_tbl[] = { 7, 12, 7, 13, 14, 15 } ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = -80.0f ;
	ew->pos.y = shl04_03_y_tbl[ew->dir] ;
	ew->prio = shl04_03_prio_tbl[ew->dir] ;
	ew->sub_type = ew->dir ;
	ew->spd.z = -80.0f ;
	ew->spd.x = -DOT_X( 16.0f ) ;
	if( ( ew->disp_dir = pl_w->disp_dir ) ){
		ew->spd.x = -ew->spd.x ;
		ew->spd.z = -ew->spd.z ;
	}
	ew->pos.y += pl_w->pos.y ;
	ew->pos.x = pl_w->pos.x + ew->spd.z ;
	ew->acc.x = 80.0f ;
	ew->dir = 0 ;
	if( Sys->game_timer0 & 3 ){
		ew->atck_no = pl04_at_cobras_2 ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl04_at_cobras ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 22, 17 ) ;
}

/*==============================================================*/
/*			A body part: it draws as the head does, dir			*/
/*			parts behind it										*/
/*==============================================================*/
static void shl04_03_body_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *mw = ew->ef_w ;

	ew->prio = mw->prio ;
	if( Sys->game_timer0 & 3 ){
		ew->atck_no = pl04_at_cobras_2 ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl04_at_cobras ;
		HIT_RESET( ew ) ;
	}
	ew->spd.x = 80.0f ;
	if( ( ew->disp_dir = mw->disp_dir ) ) ew->spd.x = -ew->spd.x ;
}

static void shl04_03_head_move( PLAYER_WORK *ew );
static void shl04_03_body_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			It lasts while Anakaris is in the hyper combo		*/
/*==============================================================*/
static void shl04_03_move( PLAYER_WORK *ew )
{
	static void ( *shl04_03_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_03_head_move, shl04_03_body_move,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != pl04_sp_cobras ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl04_03_move_jmp[ew->type]( ew ) ;
}

static void shl04_03_head_go( PLAYER_WORK *ew );
static void shl04_03_head_wait( PLAYER_WORK *ew );
static void shl04_03_head_back( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_03_head_move( PLAYER_WORK *ew )
{
	static void ( *shl04_03_head_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_03_head_go, shl04_03_head_wait, shl04_03_head_back,
	} ;

	shl04_03_head_move_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl04_03_head_go( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	int edge = 0 ;

	if( ew->disp_dir ){
		if( ew->pos.x > scr_work.scr_right - DOT_X( 64.0f ) ) edge = 1 ;
	}
	else {
		if( ew->pos.x < scr_work.scr_left + DOT_X( 64.0f ) ) edge = 1 ;
	}
	if( edge ){
		ew->routine_2 ++ ;
		ew->timer0 = 8 ;
		pl_char_set( ew, 22, 18 ) ;
	}
	ew->spd.z += ew->spd.x ;
	ew->pos.x = pl_w->pos.x + ew->spd.z ;
	ew->acc.x += fabs( ew->spd.x ) ;
	if( ew->acc.x > 80.0f ){
		ew->acc.x -= 80.0f ;
		ew->dir ++ ;
		shl04_03_body_set( ew, 1, ew->dir ) ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl04_03_head_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !--ew->timer0 ){
		ew->routine_2 ++ ;
		return ;
	}
	ew->pos.x = pl_w->pos.x + ew->spd.z ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Back in Anakaris, it frees its row for the			*/
/*			button again										*/
/*==============================================================*/
static void shl04_03_head_back( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->spd.z -= ew->spd.x ;
	ew->pos.x = pl_w->pos.x + ew->spd.z ;
	pl_char_move( ew ) ;
	if( fabs( pl_w->pos.x - ew->pos.x ) < DOT_X( 16.0f ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		if( ew->sub_type == 0 ) pl_w->timer1 &= 0xfffe ;
		if( ew->sub_type == 1 ) pl_w->timer1 &= 0xfffd ;
		if( ew->sub_type == 2 ) pl_w->timer1 &= 0xfffb ;
		if( ew->sub_type == 3 ) pl_w->timer1 &= 0xfff7 ;
	}
}

/*==============================================================*/
/*			It ends when the head, drawing back, is in			*/
/*			Anakaris											*/
/*==============================================================*/
static void shl04_03_body_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *mw = ew->ef_w ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( mw->routine_2 == 2 && fabs( pl_w->pos.x - ew->pos.x ) < DOT_X( 16.0f ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = mw->pos.x + ew->spd.x * ew->dir ;
	ew->pos.y = mw->pos.y ;
	pl_char_set2( ew, 22, 19, mw->cg_type - 1 ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl04_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl04_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl04_03.c" End of File	*/
