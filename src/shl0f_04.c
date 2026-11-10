/* Doctor Doom's jumping heavy punch beam (character 0f's shot 04, set by j_hp_move): the source on
   his hands (kind 0), which sets the beam's parts every frame (kinds 1 and 2) until he stops it. */
#include "player.h"
#include "pl_char.h"
#include "pl0f_def.h"

/* the effect's own work */
typedef struct SHL0F_04_LOCAL_W {
	Uint16	char_no;	/* 000: Doom's animation when it was set */
} SHL0F_04_LOCAL_W;
#define SHL0F_04_LW( w )	( ( SHL0F_04_LOCAL_W * )&( w )->obj_flag )

Sint16 shl0f_04_ofs[2] = { -23, 82 } ;	/* not read by the code */

static void shl0f_04_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0f_04_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	SHL0F_04_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0f_04_main ;
		ew->ef_no = 0x0f04 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		lw = SHL0F_04_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part of the beam, set by the work before it		*/
/*==============================================================*/
static void *shl0f_04_part_set( PLAYER_WORK *wk, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	SHL0F_04_LOCAL_W *lw, *wlw ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl0f_04_main ;
		ew->ef_no = 0x0f04 ;
		ew->master_w = wk->master_w ;
		ew->ef_w = wk ;
		ew->pl_type = wk->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		wlw = SHL0F_04_LW( wk ) ;
		lw = SHL0F_04_LW( ew ) ;
		lw->char_no = wlw->char_no ;
	}
	return ew ;
}

static void shl0f_04_source_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_04_main( PLAYER_WORK *ew )
{
	static void ( *shl0f_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_04_source_main, shl0f_04_part_main, shl0f_04_part_main,
	} ;

	shl0f_04_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0f_04_source_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_source_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_source_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_04_source_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_04_source_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_04_source_init, shl0f_04_source_move, shl0f_04_source_end, shl0f_04_free,
	} ;

	shl0f_04_source_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_04_source_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->pos.z = pl_w->pos.z ;
	ew->atck_no = ATCK_JUMP_HP ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 20 ) ;
	ew->timer0 = 1 ;
	shl0f_04_source_move( ew, pl_w ) ;
}

static void shl0f_04_source_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_source_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_source_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_04_source_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_04_source_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_04_source_start, shl0f_04_source_fire, shl0f_04_source_stop,
	} ;

	ew->prio = pl_w->prio ;
	shl0f_04_source_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl0f_04_source_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_04_source_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl0f_04_source_pos_set( ew, pl_w ) ;
	shl0f_04_part_set( ew, 2, 0 ) ;
	ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl0f_04_source_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *pl_lw = PL0F_LW( pl_w ) ;
	SHL0F_04_LOCAL_W *lw = SHL0F_04_LW( ew ) ;

	shl0f_04_source_pos_set( ew, pl_w ) ;
	pl_char_move( ew ) ;
	ew->timer0 ++ ;
	shl0f_04_part_set( ew, 1, ew->timer0 & 3 ) ;
	if( pl_lw->beam_stop_flag || pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_04_source_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl0f_04_source_end( ew, pl_w ) ;
	}
}

static void shl0f_04_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_04_part_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_04_part_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_04_part_init, shl0f_04_part_move, shl0f_04_part_end, shl0f_04_free,
	} ;

	shl0f_04_part_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_04_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *wk = ew->ef_w ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, wk ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = wk->pl_no ;
	ew->pl_type = wk->pl_type ;
	ew->scale.x = wk->scale.x ;
	ew->scale.y = wk->scale.y ;
	ew->atck_level = wk->atck_level ;
	ew->work_no = wk->work_no ;
	ew->col_cnt = wk->col_cnt ;
	ew->scale = wk->scale ;
	ew->prio = wk->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->atck_no = ATCK_JUMP_HP ;
	HIT_RESET( ew ) ;
	if( !ew->sub_type ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
	} else {
		ew->atck_flag = HIT_ON | HIT_TOUCH ;
	}
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_front = ew->size_back = 13 ;
	ew->pos.x = wk->pos.x ;
	ew->pos.y = wk->pos.y ;
	ew->pos.z = wk->pos.z ;
	if( !ew->disp_dir ){
		ew->pos.x += -DOT_X( 32.0f ) ;
	} else {
		ew->pos.x += DOT_X( 32.0f ) ;
	}
	ew->acc.x = !ew->disp_dir ? -25.0f : 25.0f ;
	ew->spd.x = 0.0f ;
	pl_char_set( ew, Char_Effect, ew->type == 2 ? 18 : 20 ) ;
	shl0f_04_part_move( ew, pl_w ) ;
}

static void shl0f_04_part_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_04_part_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_04_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_04_part_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_04_part_fly, shl0f_04_part_vanish,
	} ;

	ew->prio = pl_w->prio ;
	shl0f_04_part_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl0f_04_part_pos_set( PLAYER_WORK *ew, PLAYER_WORK *wk );

/*==============================================================*/
static void shl0f_04_part_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *pl_lw ;
	SHL0F_04_LOCAL_W *lw ;
	PLAYER_WORK *wk ;

	pl_lw = PL0F_LW( pl_w ) ;
	wk = ew->ef_w ;
	lw = SHL0F_04_LW( ew ) ;
	ew->spd.x += ew->acc.x ;
	shl0f_04_part_pos_set( ew, wk ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( pl_lw->beam_stop_flag || pl_w->char_no.w != lw->char_no ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, ew->type == 2 ? 19 : 21 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
		shl0f_04_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_04_part_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl0f_04_part_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_04_source_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0f_04_part_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0f_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			The source keeps its place on Doom's hands			*/
/*==============================================================*/
static void shl0f_04_source_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( !ew->disp_dir ){
		ew->pos.x += -DOT_X( 127.0f ) ;
	} else {
		ew->pos.x -= -DOT_X( 127.0f ) ;
	}
	ew->pos.y += DOT_Y( 96.0f ) ;
}

/*==============================================================*/
/*			A part keeps its place on the one before it,		*/
/*			moved on by its speed; the offsets by kind and		*/
/*			direction are all 0									*/
/*==============================================================*/
static void shl0f_04_part_pos_set( PLAYER_WORK *ew, PLAYER_WORK *wk )
{
	ew->pos.x = wk->pos.x ;
	ew->pos.y = wk->pos.y ;
	ew->pos.z = wk->pos.z ;
	if( ew->type == 2 ){
		if( !ew->disp_dir ){
			ew->pos.x += 0.0f ;
		} else {
			ew->pos.x += 0.0f ;
		}
	} else {
		if( !ew->disp_dir ){
			ew->pos.x += 0.0f ;
		} else {
			ew->pos.x += 0.0f ;
		}
	}
	ew->pos.x += ew->spd.x ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0f_04.c" End of File	*/
