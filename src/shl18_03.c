/* Abyss's effect 03 (character 18's shot 03): the row of ten dark pillars of pl18_sp_pillar. One
   effect (type 0) keeps the count and the pace and makes the ten (type 1), which rise one after
   another along the floor in front of him, strike, and sink again. */
#include "player.h"
#include "pl_char_old.h"
#include "pl18_def.h"

/* the work both kinds keep, from obj_mtx; the pillars read the controller's through a second
   pointer */
typedef struct SHL18_03_LOCAL_W {
	Sint8	anm_no;		/* 00: the picture's row: 0 to 6, -1 after a hit */
	Sint8	anm_ofs;		/* 01: the column of the picture */
	Uint8	pillar_cnt;		/* 02: pillars risen */
	Sint8	pillar_timer;		/* 03: frames to the next */
	Uint8	unk_04;
	Uint8	hit_cnt;		/* 05: hits taken, up to 4 */
	Uint8	pillar_no;		/* 06: the pillar's place in the row */
	Uint8	hit_ng_flag;		/* 07: the row is held */
	Float	x;		/* 08: where the row starts from Abyss */
	Float	y;		/* 0c */
	Sint8	no;		/* 10: the kind of row */
	Uint8	unk_11;
	Uint16	char_no;	/* 12: Abyss's animation when it began */
} SHL18_03_LOCAL_W;
#define SHL18_03_LW( w )	( ( SHL18_03_LOCAL_W * )&( w )->obj_mtx )

static SHL18_03_LOCAL_W *shl18_03_lw ;
static SHL18_03_LOCAL_W *shl18_03_c_lw ;

static void shl18_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl18_03_set( PLAYER_WORK *pl_w, Uint8 no, Float x, Float y )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl18_03_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1803 ;
		if( !pl_w->pl_dir ) x = -x ;
		SHL18_03_LW( ew )->char_no = pl_w->char_no.w ;
		SHL18_03_LW( ew )->no = no ;
		SHL18_03_LW( ew )->x = x ;
		SHL18_03_LW( ew )->y = y ;
	}
	return ew ;
}

/*==============================================================*/
/*			The ten pillars										*/
/*==============================================================*/
static void shl18_03_set_all( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint8 i ;
	Uint8 n ;

	n = 0 ;
	for( i = 0 ; i < 10 ; i ++ ){
		if( ( ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ) ){
			ew->work_func = shl18_03_main ;
			ew->type = 1 ;
			ew->master_w = pl_w ;
			ew->ef_no = 0x1803 ;
			SHL18_03_LW( ew )->pillar_no = n ;
			n ++ ;
			SHL18_03_LW( ew )->no = shl18_03_lw->no ;
		}
	}
}

/*==============================================================*/
/*			A pillar stands its place in the row from Abyss		*/
/*==============================================================*/
static void shl18_03_pos_set( PLAYER_WORK *ew )
{
	Float d ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	d = shl18_03_lw->pillar_no * 32 * 1.6666666f ;
	if( !ew->disp_dir ) d = -d ;
	ew->pos.x += d ;
}

static void shl18_03_c_init( PLAYER_WORK *ew );
static void shl18_03_c_move( PLAYER_WORK *ew );
static void shl18_03_c_end( PLAYER_WORK *ew );
static void shl18_03_c_free( PLAYER_WORK *ew );
static void shl18_03_p_init( PLAYER_WORK *ew );
static void shl18_03_p_move( PLAYER_WORK *ew );
static void shl18_03_p_end( PLAYER_WORK *ew );
static void shl18_03_p_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl18_03_main( PLAYER_WORK *ew )
{
	static void ( *shl18_03_c_jmp[] )( PLAYER_WORK *ew ) = {
		shl18_03_c_init, shl18_03_c_move, shl18_03_c_end, shl18_03_c_free,
	} ;
	static void ( *shl18_03_p_jmp[] )( PLAYER_WORK *ew ) = {
		shl18_03_p_init, shl18_03_p_move, shl18_03_p_end, shl18_03_p_free,
	} ;

	if( !ew->type ){
		shl18_03_lw = SHL18_03_LW( ew ) ;
		shl18_03_c_jmp[ew->routine_0]( ew ) ;
	}
	else {
		shl18_03_lw = SHL18_03_LW( ew ) ;
		shl18_03_c_lw = SHL18_03_LW( ew->master_w ) ;
		shl18_03_pos_set( ew ) ;
		shl18_03_p_jmp[ew->routine_0]( ew ) ;
	}
}

/*==============================================================*/
/*			The controller follows Abyss, offset by the			*/
/*			row's start and the picture's row					*/
/*==============================================================*/
static void shl18_03_part_pos( PLAYER_WORK *ew )
{
	static Sint16 shl18_03_ofs_tbl[] = { 0, 0, 0, -104, -96, -96, -96, -96 } ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += shl18_03_lw->x ;
	ew->pos.y += shl18_03_lw->y ;
	if( !ew->disp_dir ) ew->pos.x += 1.6666666f * shl18_03_ofs_tbl[shl18_03_lw->anm_no] ;
	else ew->pos.x -= 1.6666666f * shl18_03_ofs_tbl[shl18_03_lw->anm_no] ;
}

/*==============================================================*/
/*			One more pillar every frame, ten at most			*/
/*==============================================================*/
static void shl18_03_tick( PLAYER_WORK *ew )
{
	if( shl18_03_lw->pillar_cnt < 10 ){
		if( ( Uint8 )( -- shl18_03_lw->pillar_timer ) <= 0 ){
			shl18_03_lw->pillar_timer = 1 ;
			shl18_03_lw->pillar_cnt ++ ;
		}
	}
}

/*==============================================================*/
/*			The row rises: the picture's row goes up from 4		*/
/*			to 6												*/
/*==============================================================*/
static void shl18_03_tick_up( PLAYER_WORK *ew )
{
	if( -- ew->timer0 <= 0 ){
		ew->timer0 = 1 ;
		shl18_03_lw->anm_no ++ ;
		if( shl18_03_lw->anm_no >= 7 ) shl18_03_lw->anm_no = 4 ;
	}
}

/*==============================================================*/
/*			And sinks again										*/
/*==============================================================*/
static void shl18_03_tick_down( PLAYER_WORK *ew )
{
	if( -- ew->timer0 <= 0 ){
		ew->timer0 = 1 ;
		shl18_03_lw->anm_no -- ;
	}
}

/*==============================================================*/
/*			The picture follows the controller's row			*/
/*==============================================================*/
static void shl18_03_anim( PLAYER_WORK *ew )
{
	Uint8 no ;
	Float one ;

	if( shl18_03_lw->anm_no != shl18_03_c_lw->anm_no ){
		shl18_03_lw->anm_no = shl18_03_c_lw->anm_no ;
		no = shl18_03_lw->anm_no * 3 ;
		if( shl18_03_lw->pillar_no ) no += shl18_03_lw->anm_ofs ;
		pl_char_set( ew, 25, no ) ;
	}
	one = 2.0f ;
	if( shl18_03_lw->anm_no >= 4 ){
		ew->scale.x = one ;
		ew->scale.y = one ;
	}
	else if( shl18_03_lw->anm_no == 3 ){
		ew->scale.x = one ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
}

/*==============================================================*/
/*			0 once the pillar is off the screen					*/
/*==============================================================*/
static int shl18_03_out_ck( PLAYER_WORK *ew )
{
	int ok = 1 ;

	if( ( ew->disp_dir == 0 && scr_work.scr_left - ew->size_back * 1.6666666f > ew->pos.x ) ||
		( ew->disp_dir != 0 && ew->pos.x > scr_work.scr_right + DOT_X( ew->size_back ) ) ) ok = 0 ;
	return ok ;
}

static void shl18_03_c_wait( PLAYER_WORK *ew );
static void shl18_03_c_fade( PLAYER_WORK *ew );

/*==============================================================*/
static void shl18_03_c_init( PLAYER_WORK *ew )
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
	if( ew->pl_type != ew->master_w->pl_type ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 0 ;
	ew->prio = 0 ;
	shl18_03_lw->anm_no = shl18_03_lw->hit_ng_flag = 0 ;
	ew->timer0 = 1 ;
	ew->dir = 12 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	shl18_03_lw->pillar_cnt = 1 ;
	shl18_03_lw->pillar_timer = 1 ;
	shl18_03_part_pos( ew ) ;
	shl18_03_set_all( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_03_c_move( PLAYER_WORK *ew )
{
	static void ( *shl18_03_c_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl18_03_c_wait, shl18_03_c_fade,
	} ;

	shl18_03_c_move_jmp[ew->routine_1]( ew ) ;
	shl18_03_part_pos( ew ) ;
}

/*==============================================================*/
/*			It holds the row up while Abyss is in the			*/
/*			animation											*/
/*==============================================================*/
static void shl18_03_c_wait( PLAYER_WORK *ew )
{
	if( ew->pl_type != ew->master_w->pl_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->master_w->char_no.w != 0x1503 ){
		ew->routine_1 ++ ;
		shl18_03_lw->anm_no = 3 ;
		ew->timer0 = 1 ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	shl18_03_lw->hit_ng_flag = 0 ;
	if( ew->master_w->timer0 <= 0 ){
		ew->routine_1 ++ ;
		shl18_03_lw->anm_no = 3 ;
		ew->timer0 = 1 ;
	}
	shl18_03_tick( ew ) ;
	shl18_03_tick_up( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_03_c_fade( PLAYER_WORK *ew )
{
	shl18_03_lw->hit_ng_flag = 0 ;
	shl18_03_tick_down( ew ) ;
	if( shl18_03_lw->anm_no < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl18_03_c_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl18_03_c_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_03_c_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl18_03_p_init( PLAYER_WORK *ew )
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
	if( ew->pl_type != ew->master_w->pl_type ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 0 ;
	ew->dir = 12 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = 11 ;
	if( !shl18_03_lw->pillar_no ){
		ew->size_top = 16 ;
		ew->size_bottom = 16 ;
		ew->size_front = 16 ;
		ew->size_back = 128 ;
	}
	else {
		ew->size_top = 16 ;
		ew->size_bottom = 16 ;
		ew->size_front = 16 ;
		ew->size_back = 16 ;
	}
	shl18_03_lw->anm_no = -1 ;
	shl18_03_lw->hit_cnt = 0 ;
	shl18_03_lw->anm_ofs = 1 ;
	ew->atck_no = pl18_at_pillar ;
	HIT_RESET( ew ) ;
	shl18_03_anim( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_03_p_move( PLAYER_WORK *ew )
{
	int atck ;

	if( ew->master_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		shl18_03_c_free( ew ) ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	shl18_03_lw->anm_ofs = 1 ;
	if( shl18_03_c_lw->pillar_cnt < shl18_03_lw->pillar_no ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( shl18_03_c_lw->pillar_cnt == shl18_03_lw->pillar_no ){
		shl18_03_lw->anm_ofs = 2 ;
		shl18_03_lw->anm_no = -1 ;
	}
	ew->disp_flag = shl18_03_out_ck( ew ) ;
	if( ew->hit_flag ){
		shl18_03_lw->anm_ofs = 1 ;
		shl18_03_lw->anm_no = -1 ;
		shl18_03_c_lw->hit_ng_flag = 0xff ;
		if( ! -- ew->hit_stop ){
			atck = 53 ;
			if( ++ shl18_03_lw->hit_cnt >= 4 ){
				shl18_03_lw->hit_cnt = 0 ;
				atck = 54 ;
			}
			ew->atck_no = atck ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		}
	}
	if( ew->disp_flag ) shl18_03_anim( ew ) ;
	if( !shl18_03_c_lw->hit_ng_flag ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_03_p_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl18_03_p_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_03_p_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl18_03.c" End of File	*/
