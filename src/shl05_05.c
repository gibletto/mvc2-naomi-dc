/* Strider's effect 05 (character 05's shot 05): the copies of Strider in Vajra, which leap off with
   him (type 0) and come down with him from round the opponent (type 1). Strider himself takes the
   third place, so the copies are numbered 0, 1 and 3. */
#include "player.h"
#include "pl_char.h"
#include "pl05_def.h"

static Float shl05_05_up_spd[4][2] = {
	{ DOT_X( 10.0f ), DOT_Y( 4.0f ) }, { DOT_X( 4.0f ), DOT_Y( 10.0f ) },
	{ -DOT_X( 4.0f ), DOT_Y( 10.0f ) }, { -DOT_X( 10.0f ), DOT_Y( 4.0f ) },
} ;
static Float shl05_05_down_spd[4][2] = {
	{ -DOT_X( 10.0f ), -DOT_Y( 4.0f ) }, { -DOT_X( 4.0f ), -DOT_Y( 10.0f ) },
	{ DOT_X( 4.0f ), -DOT_Y( 10.0f ) }, { DOT_X( 10.0f ), -DOT_Y( 4.0f ) },
} ;
static Float shl05_05_pos[4][2] = {
	{ 320.0f, DOT_Y( 128.0f ) }, { DOT_X( 80.0f ), DOT_Y( 160.0f ) },
	{ -DOT_X( 80.0f ), DOT_Y( 160.0f ) }, { -320.0f, DOT_Y( 128.0f ) },
} ;

static Sint32 *lw;	/* the running work's, unused */
static PL05_LOCAL_W *pl_lw;	/* Strider's, unused */

static void shl05_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl05_05_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_05_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x505 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_05_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = 1 ;
		ew->ef_no = 0x505 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_05_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = 3 ;
		ew->ef_no = 0x505 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	return ew ;
}

static void shl05_05_init( PLAYER_WORK *ew );
static void shl05_05_move( PLAYER_WORK *ew );
static void shl05_05_end( PLAYER_WORK *ew );
static void shl05_05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_05_main( PLAYER_WORK *ew )
{
	static void ( *shl05_05_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_05_init, shl05_05_move, shl05_05_end, shl05_05_free,
	} ;

	pl_lw = ( PL05_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = &ew->obj_flag ;
	shl05_05_jmp[ew->routine_0]( ew ) ;
}

static void shl05_05_up_init( PLAYER_WORK *ew );
static void shl05_05_down_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_05_init( PLAYER_WORK *ew )
{
	static void ( *shl05_05_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_05_up_init, shl05_05_down_init,
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
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	shl05_05_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl05_05_up_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 11 ;
	ew->timer0 = 8 ;
	ew->spd.x = ew->disp_dir ? shl05_05_up_spd[ew->sub_type][0] : -shl05_05_up_spd[ew->sub_type][0] ;
	ew->spd.y = shl05_05_up_spd[ew->sub_type][1] ;
	pl_char_set( ew, 21, ew->sub_type + 29 ) ;
}

/*==============================================================*/
/*			Above the opponent, where Strider has moved to		*/
/*==============================================================*/
static void shl05_05_down_init( PLAYER_WORK *ew )
{
	ew->prio = 11 ;
	ew->pos.x += ew->disp_dir ? shl05_05_pos[ew->sub_type][0] : -shl05_05_pos[ew->sub_type][0] ;
	ew->pos.y += shl05_05_pos[ew->sub_type][1] ;
	pl_char_set( ew, 21, ew->sub_type + 45 ) ;
}

static void shl05_05_up_move( PLAYER_WORK *ew );
static void shl05_05_down_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl05_05_move( PLAYER_WORK *ew )
{
	static void ( *shl05_05_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_05_up_move, shl05_05_down_move,
	} ;

	if( ew->master_w->routine_1 ) shl05_05_end( ew ) ;
	else shl05_05_move_jmp[ew->type]( ew ) ;
}

static void shl05_05_up_leap( PLAYER_WORK *ew );
static void shl05_05_up_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_05_up_move( PLAYER_WORK *ew )
{
	static void ( *shl05_05_up_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_05_up_leap, shl05_05_up_fly,
	} ;

	shl05_05_up_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl05_05_up_leap( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !--ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 21, ew->sub_type + 33 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_05_up_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ) shl05_05_end( ew ) ;
}

static void shl05_05_down_wait( PLAYER_WORK *ew );
static void shl05_05_down_dive( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_05_down_move( PLAYER_WORK *ew )
{
	static void ( *shl05_05_down_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_05_down_wait, shl05_05_down_dive,
	} ;

	shl05_05_down_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl05_05_down_wait( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? shl05_05_down_spd[ew->sub_type][0] : -shl05_05_down_spd[ew->sub_type][0] ;
		ew->spd.y = shl05_05_down_spd[ew->sub_type][1] ;
		pl_char_set( ew, 21, ew->sub_type + 41 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_05_down_dive( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ) shl05_05_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	shl05_05_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl05_05.c" End of File	*/
