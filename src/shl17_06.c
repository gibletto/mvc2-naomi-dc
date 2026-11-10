/* Cable's effect 06 (character 17's shot 06): the muzzle flashes of his guns, six kinds placed
   by Cable, each with a shot that carries the hit. */
#include "player.h"
#include "pl_char.h"
#include "pl17_def.h"

static void shl17_06_main( PLAYER_WORK *ew );
static void shl17_06_hit_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/****************************************************************/
void *shl17_06_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = shl17_06_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1705 ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->sub_no = no ;
		shl17_06_hit_set( ew, pl_w ) ;
		return ew ;
	}
}

static void shl17_06_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_06_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_06_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl17_06_main( PLAYER_WORK *ew )
{
	static void ( *shl17_06_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl17_06_init, shl17_06_move, shl17_06_end, shl17_06_free,
	} ;

	shl17_06_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* by kind: the place in front of Cable, and the animation */
static Float shl17_06_ofs_x[] = { 208.0f, 150.0f, 117.0f, 208.0f, 208.0f, 208.0f } ;
static Float shl17_06_ofs_y[] = { 180.0f, 282.0f, 255.0f, 180.0f, 208.0f, 208.0f } ;
static Uint8 shl17_06_char[] = { 16, 17, 16, 16, 16, 16 } ;

/*==============================================================*/
static void shl17_06_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( ew->sub_no >= 6 ){
		work_free( ew ) ;
		return ;
	}
	x = shl17_06_ofs_x[ew->sub_no] ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + shl17_06_ofs_y[ew->sub_no] ;
	if( ew->sub_no == 1 ) ew->rot[2] = 0xe4fb ;
}

/*==============================================================*/
static void shl17_06_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	shl17_06_pos_set( ew, pl_w ) ;
	pl_char_set( ew, Char_Effect, shl17_06_char[ew->sub_no] ) ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	if( ew->sub_no >= 4 ) ew->alpha = 0.4f ;
	shl17_06_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl17_06_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->pl_type != pl_w->pl_type ){
		shl17_06_free( ew, pl_w ) ;
		return ;
	}
	shl17_06_pos_set( ew, pl_w ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl17_06_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

static void shl17_06_hit_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			The flash's shot, which is not drawn				*/
/*==============================================================*/
static void shl17_06_hit_set( PLAYER_WORK *fw, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SHL, LIST_END ) ) ) return ;
	ew->work_func = shl17_06_hit_main ;
	ew->master_w = pl_w ;
	ew->ef_no = 0x1705 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->sub_no = fw->sub_no ;
}

static void shl17_06_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_06_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl17_06_hit_main( PLAYER_WORK *ew )
{
	static void ( *shl17_06_hit_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl17_06_hit_init, shl17_06_hit_move, shl17_06_end, shl17_06_free,
	} ;

	shl17_06_hit_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl17_06_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	shl17_06_pos_set( ew, pl_w ) ;
	pl_char_set( ew, Char_Effect, 20 ) ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	switch( ew->sub_no ){
	case 3:
		ew->atck_no = pl17_at_snapback ;
		HIT_RESET( ew ) ;
		break ;
	case 4:
		ew->atck_no = pl17_at_timeflip_shot ;
		HIT_RESET( ew ) ;
		break ;
	case 5:
		ew->atck_no = pl17_at_timeflip_shot_2 ;
		HIT_RESET( ew ) ;
		break ;
	default:
		ew->atck_no = ew->sub_no + 70 ;
		HIT_RESET( ew ) ;
		break ;
	}
	ew->hit_box_no = 0 ;
	ew->routine_1 = 0 ;
	if( ew->sub_no == 1 ){
		ew->spd.x = ew->disp_dir ? nlCos( 0x1b06 ) * 23.333334f : -( nlCos( 0x1b06 ) * 23.333334f ) ;
		ew->spd.y = nlSin( 0x1b06 ) * 23.333334f ;
	} else ew->spd.x = ew->disp_dir ? 23.333334f : -23.333334f ;
	ew->timer0 = 30 ;
	shl17_06_hit_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			It hits until it has struck something, and goes		*/
/*			for thirty frames									*/
/*==============================================================*/
static void shl17_06_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_box_no ) ew->routine_1 ++ ;
	if( ew->timer0 ) ew->timer0 -- ;
	else ew->routine_0 = 2 ;
	if( !ew->routine_1 ) hit_work_set( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl17_06.c" End of File	*/
