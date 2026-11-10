/* Captain America's effect 01 (character 0b's shot 01): two parts drawn on his shield (ef_w),
   placed by the shield's frame data and turned with him. */
#define PL_CHAR_SET2_OLD
#include "player.h"
#include "pl_char_old.h"
#include "pl0b_def.h"

extern int dbg_flag_ck( int a, int b ) ;

static void shl0b_01_main( PLAYER_WORK *ew );

/****************************************************************/
int shl0b_01_set( PLAYER_WORK *sw )
{
	PLAYER_WORK *ew ;

	if( work_rest_cnt < 2 ) return 0 ;
	if( ( ew = work_get( 0, W_SHL_ST, LIST_END ) ) ){
		ew->work_func = shl0b_01_main ;
		ew->master_w = sw->master_w ;
		ew->ef_w = sw ;
		ew->pl_type = sw->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0xb01 ;
	}
	if( ( ew = work_get( 0, W_SHL_ST, LIST_END ) ) ){
		ew->work_func = shl0b_01_main ;
		ew->master_w = sw->master_w ;
		ew->ef_w = sw ;
		ew->pl_type = sw->pl_type ;
		ew->type = 0 ;
		ew->sub_type = 1 ;
		ew->ef_no = 0xb01 ;
	}
}

/****************************************************************/
void *shl0b_01_set2( PLAYER_WORK *sw )
{
	PLAYER_WORK *ew ;

	if( work_rest_cnt < 2 ) return 0 ;
	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl0b_01_main ;
		ew->master_w = sw->master_w ;
		ew->ef_w = sw ;
		ew->pl_type = sw->pl_type ;
		ew->type = 0x80 ;
		ew->sub_type = 0 ;
		ew->ef_no = 0xb01 ;
	}
	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl0b_01_main ;
		ew->master_w = sw->master_w ;
		ew->ef_w = sw ;
		ew->pl_type = sw->pl_type ;
		ew->type = 0x80 ;
		ew->sub_type = 1 ;
		ew->ef_no = 0xb01 ;
	}
	return ew ;
}

static void shl0b_01_init( PLAYER_WORK *ew, PLAYER_WORK *sw );
static void shl0b_01_move( PLAYER_WORK *ew, PLAYER_WORK *sw );
static void shl0b_01_end( PLAYER_WORK *ew, PLAYER_WORK *sw );
static void shl0b_01_free( PLAYER_WORK *ew, PLAYER_WORK *sw );

/*==============================================================*/
static void shl0b_01_main( PLAYER_WORK *ew )
{
	static void ( *shl0b_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *sw ) = {
		shl0b_01_init, shl0b_01_move, shl0b_01_end, shl0b_01_free,
	} ;

	PLAYER_WORK *sw = ew->ef_w ;

	ew->prio = sw->prio ;
	shl0b_01_jmp[ew->routine_0]( ew, sw ) ;
}

/*==============================================================*/
static void shl0b_01_init( PLAYER_WORK *ew, PLAYER_WORK *sw )
{
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
	ew->disp_flag = 0 ;
}

static void shl0b_01_rot_move( PLAYER_WORK *ew, PLAYER_WORK *sw );
static void shl0b_01_norm_move( PLAYER_WORK *ew, PLAYER_WORK *sw );

/*==============================================================*/
/*			Hidden while the shield is away or hidden, and		*/
/*			gone once the player's work ends					*/
/*==============================================================*/
static void shl0b_01_move( PLAYER_WORK *ew, PLAYER_WORK *sw )
{
	PLAYER_WORK *mw = ew->master_w ;
	PL0B_LOCAL_W *lw = ( PL0B_LOCAL_W * )mw->tlocal ;

	if( mw->routine_0 >= 2 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( lw->shield_st ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( !sw->disp_flag ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( sw->char_no.b.grp != 26 ){
		shl0b_01_norm_move( ew, sw ) ;
		return ;
	}
	shl0b_01_rot_move( ew, sw ) ;
}

/*--------------------------------------------------------------*/
static void shl0b_01_end( PLAYER_WORK *ew, PLAYER_WORK *sw )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0b_01_free( PLAYER_WORK *ew, PLAYER_WORK *sw )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl0b_01_disp_ck( PLAYER_WORK *ew, PLAYER_WORK *sw )
{
	if( !scr_in_x_ck( ew ) ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
}

/*==============================================================*/
/*			The shield's animation group 26: each part at		*/
/*			its offset from the shield's place, turned with		*/
/*			the player about his centre (rot_ofs_x, rot_ofs_y)	*/
/*==============================================================*/
static void shl0b_01_rot_move( PLAYER_WORK *ew, PLAYER_WORK *sw )
{
	SHL0B_00_FRAME *fr = SHL0B_00_LW( sw )->frame ;
	PLAYER_WORK *mw = ew->master_w ;
	Sint8 *part ;
	Sint16 ox, oy, x, y, dx, dy ;

	if( !mw->cg_rival || !fr->part_flag ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( !ew->sub_type ) part = fr->part[0] ;
	else part = fr->part[1] ;
	ew->sub_prio = part[2] ;
	ew->pos.x = mw->pos.x ;
	ew->pos.y = mw->pos.y ;
	ew->disp_dir = mw->disp_dir ;
	ew->rot[2] = mw->rot[2] ;
	ox = fr->x + part[0] ;
	oy = part[1] + fr->y ;
	if( ew->disp_dir ) ox = ~ox + 1 ;	/* negated by not and add */
	x = DOT_X( ox ) ;
	y = -oy * 2.142857f ;
	dx = ( x + mw->rot_ofs_x ) * nlCos( ew->rot[2] ) + ( y + mw->rot_ofs_y * 2 ) * nlSin( ew->rot[2] ) - mw->rot_ofs_x ;
	dy = ( -x - mw->rot_ofs_x ) * nlSin( ew->rot[2] ) + ( y + mw->rot_ofs_y * 2 ) * nlCos( ew->rot[2] ) - mw->rot_ofs_y * 2 ;
	ew->pos.x += dx ;
	ew->pos.y += dy ;
	if( dbg_flag_ck( 0, 9 ) ){
		ew->disp_flag = 0 ;
	} else {
		ew->disp_flag = 1 ;
		ew->char_no.b.grp = 28 ;
		ew->char_no.b.no = 0 ;
		pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, part[3] ) ;
	}
	shl0b_01_disp_ck( ew, sw ) ;
}

/*==============================================================*/
/* the shield's other animations: the first part only, on the player, at the shield's frame.
   It keeps shl0b_01_rot_move's part, unused. */
static void shl0b_01_norm_move( PLAYER_WORK *ew, PLAYER_WORK *sw )
{
	SHL0B_00_FRAME *fr = SHL0B_00_LW( sw )->frame ;
	PLAYER_WORK *mw = ew->master_w ;
	Sint8 *part ;

	if( ew->sub_type ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( !sw->cg_rival ){
		ew->disp_flag = 0 ;
		return ;
	}
	part = fr->part[0] ;
	ew->pos.x = mw->pos.x ;
	ew->pos.y = mw->pos.y ;
	ew->disp_dir = mw->disp_dir ;
	ew->rot[2] = mw->rot[2] ;
	if( dbg_flag_ck( 0, 9 ) ){
		ew->disp_flag = 0 ;
	} else {
		ew->disp_flag = 1 ;
		ew->char_no.b.grp = 28 ;
		ew->char_no.b.no = 1 ;
		pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, ( Uint16 )sw->cg_rival ) ;
	}
	shl0b_01_disp_ck( ew, sw ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0b_01.c" End of File	*/
