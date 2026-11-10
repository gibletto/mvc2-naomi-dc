/* Abyss's second form's volley of 16 shots (effect 03): a hidden controller (type 0) lets the shots
   (type 1) go one after another; each one flies across the screen along a line of its own. */
#include "player.h"
#include "pl_char_old.h"
#include "pl19_def.h"

/* each work's own work, from obj_flag */
typedef struct SHL19_03_LOCAL_W {
	Uint8	kind;		/* 000: a shot's kind: 0 the first, 1 the shots not yet let go, 2 the rest */
	Uint8	cnt;		/* 001: the controller's count of the shots let go */
	Uint8	hit_cnt;		/* 002: counts a shot's hits */
	Uint8	hit_ng_flag;		/* 003: 0xff while a shot hit, 0 otherwise */
} SHL19_03_LOCAL_W;
#define SHL19_03_LW( w )	( ( SHL19_03_LOCAL_W * )&( w )->obj_flag )

/* the work being run, and its controller's */
static SHL19_03_LOCAL_W *shl19_03_lw ;
static SHL19_03_LOCAL_W *shl19_03_master_lw ;

static void shl19_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl19_03_set( PLAYER_WORK *pl_w, Sint8 a )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl19_03_main ;
		ew->type = a ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1903 ;
		ew->atck_level = pl_w->atck_level ;
	}
	return ew ;
}

/*==============================================================*/
/*			The 16 shots, each numbered in sub_no				*/
/*==============================================================*/
static void shl19_03_child_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint8 i ;
	int n = 0 ;

	for( i = 0 ; i < 16 ; i ++ ){
		if( ( ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ) ){
			ew->work_func = shl19_03_main ;
			ew->type = 1 ;
			ew->master_w = pl_w ;
			ew->ef_no = 0x1903 ;
			ew->atck_level = pl_w->atck_level ;
			ew->sub_no = n ;
			n ++ ;
		}
	}
}

static void shl19_03_init( PLAYER_WORK *ew );
static void shl19_03_move( PLAYER_WORK *ew );
static void shl19_03_child_init( PLAYER_WORK *ew );
static void shl19_03_child_move( PLAYER_WORK *ew );
static void shl19_03_end( PLAYER_WORK *ew );
static void shl19_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl19_03_main( PLAYER_WORK *ew )
{
	static void ( *shl19_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl19_03_init, shl19_03_move, shl19_03_end, shl19_03_free,
	} ;
	static void ( *shl19_03_child_jmp[] )( PLAYER_WORK *ew ) = {
		shl19_03_child_init, shl19_03_child_move, shl19_03_end, shl19_03_free,
	} ;

	if( !ew->type ){
		shl19_03_lw = SHL19_03_LW( ew ) ;
		shl19_03_jmp[ew->routine_0]( ew ) ;
	} else {
		shl19_03_lw = SHL19_03_LW( ew ) ;
		shl19_03_master_lw = SHL19_03_LW( ew->master_w ) ;
		shl19_03_child_jmp[ew->routine_0]( ew ) ;
	}
}

/* where the controller rides, by the master's animation frame */
static struct { Sint16 x, y ; } shl19_03_pos_tbl[6] = {
	{ -58, 83 }, { -59, 82 }, { -58, 83 }, { -59, 82 }, { -58, 83 }, { -59, 82 },
} ;

/*==============================================================*/
static void shl19_03_pos_set( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->master_w->disp_dir == 0 ){
		ew->pos.x += DOT_X( shl19_03_pos_tbl[ew->master_w->cg_prm].x ) ;
	} else {
		ew->pos.x -= DOT_X( shl19_03_pos_tbl[ew->master_w->cg_prm].x ) ;
	}
	ew->pos.y += DOT_Y( shl19_03_pos_tbl[ew->master_w->cg_prm].y ) ;
}

/*==============================================================*/
/*			A shot lines up across the screen, 32 apart			*/
/*==============================================================*/
static void shl19_03_child_pos( PLAYER_WORK *ew )
{
	Float x ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	x = ( ew->sub_no << 5 ) * 1.6666666f ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
}

/*==============================================================*/
/*			Lets the next shot go every frame, up to 16			*/
/*==============================================================*/
static void shl19_03_count( PLAYER_WORK *ew )
{
	if( shl19_03_lw->cnt < 16 ){
		if( --ew->timer1 <= 0 ){
			ew->timer1 = 1 ;
			shl19_03_lw->cnt ++ ;
		}
	}
}

/*==============================================================*/
/*			1 while the shot is still on the screen				*/
/*==============================================================*/
static int shl19_03_scr_ck( PLAYER_WORK *ew )
{
	int ok = 1 ;

	if( !ew->disp_dir && scr_work.scr_left - ew->size_back * 1.6666666f > ew->pos.x ) ok = 0 ;
	else if( ew->disp_dir && ew->pos.x > scr_work.scr_right + DOT_X( ew->size_back ) ) ok = 0 ;
	return ok ;
}

/*==============================================================*/
static void shl19_03_init( PLAYER_WORK *ew )
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
	ew->prio = 11 ;
	ew->scale.x = ew->scale.y = 1.0f ;
	shl19_03_lw->hit_ng_flag = 0 ;
	shl19_03_lw->cnt = 1 ;
	ew->timer1 = 1 ;
	shl19_03_child_set( ew ) ;
	shl19_03_move( ew ) ;
}

static void shl19_03_wait( PLAYER_WORK *ew );
static void shl19_03_fade( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl19_03_move( PLAYER_WORK *ew )
{
	static void ( *shl19_03_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl19_03_wait, shl19_03_fade,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl19_03_move_jmp[ew->routine_1]( ew ) ;
	shl19_03_pos_set( ew ) ;
}

/*==============================================================*/
/*			The shots go while Abyss is in the throw's			*/
/*			animations											*/
/*==============================================================*/
static void shl19_03_wait( PLAYER_WORK *ew )
{
	if( !( ew->master_w->char_no.w == 0x1506 || ew->master_w->char_no.w == 0x1508 || ew->master_w->char_no.w == 0x1507 || ew->master_w->char_no.w == 0x1509 ) ){
		ew->routine_1 ++ ;
		ew->timer0 = 1 ;
		return ;
	}
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	shl19_03_lw->hit_ng_flag = 0 ;
	if( ew->atck_level == 0 ){
		ew->scale.y = ( ew->master_w->timer0 & 1 ) ? 1.0f : 0.5f ;
	} else {
		ew->scale.y = ( ew->master_w->timer0 & 1 ) ? 1.0f : 0.65f ;
	}
	if( ew->master_w->timer0 <= 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 10 ;
		ew->scale.y = 1.0f ;
	}
	shl19_03_count( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl19_03_fade( PLAYER_WORK *ew )
{
	shl19_03_lw->hit_ng_flag = 0xff ;
	ew->scale.y += ( 0.0f - ew->scale.y ) / ew->timer0 ;
	if( --ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl19_03_child_init( PLAYER_WORK *ew )
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
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	if( !ew->sub_no ) shl19_03_lw->kind = 0 ;
	else shl19_03_lw->kind = 2 ;
	shl19_03_lw->hit_cnt = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->atck_level + 51 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 25, shl19_03_lw->kind * 2 + ew->atck_level ) ;
	shl19_03_child_move( ew ) ;
}

/*==============================================================*/
/*			A part follows its master and, every other			*/
/*			frame, sets its hit									*/
/*==============================================================*/
static void shl19_03_child_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	int no ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	if( ew->sub_no != 0 ){
		if( ew->sub_no > shl19_03_master_lw->cnt ){
			ew->disp_flag = 0 ;
			return ;
		}
		if( ew->sub_no != shl19_03_master_lw->cnt ) shl19_03_lw->kind = 1 ;
	}
	shl19_03_child_pos( ew ) ;
	ew->disp_flag = shl19_03_scr_ck( ew ) ;
	if( ew->hit_flag ){
		shl19_03_master_lw->hit_ng_flag = 0xff ;
		if( !--ew->hit_stop ){
			no = ew->atck_level + 51 ;
			if( ++shl19_03_lw->hit_cnt & 1 ) no = ew->atck_level + 53 ;
			ew->atck_no = no ;
			HIT_RESET( ew ) ;
		}
	}
	if( ew->disp_flag ) pl_char_set( ew, 25, shl19_03_lw->kind * 2 + ew->atck_level ) ;
	if( !shl19_03_master_lw->hit_ng_flag && ( ew->sub_no & 1 ) == ( Sys->game_timer0 & 1 ) ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl19_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl19_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl19_03.c" End of File	*/
