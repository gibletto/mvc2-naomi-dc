/* Akuma's effect 01 (character 1e's shot 01): the head work of his Messatsu Gou Hadou, which stays
   where he is, and the twelve parts it makes; each part strikes on its turn, as the head's count
   comes to it. */
#include "player.h"
#include "pl_char_old.h"
#include "pl1e_def.h"

/* the effect's own work, from obj_mtx; a part's part_no is its place in the row */
typedef struct SHL1E_01_LOCAL_W {
	Sint8	anim_st;		/* 000: the animation step, -1 for none */
	Sint8	anim_no;
	Uint8	part_cnt;		/* 002: how many parts have had their turn */
	Uint8	part_timer;		/* 003: frames to the next part's turn */
	Uint8	unk_04;
	Uint8	hit_cnt;		/* 005: the part's hits */
	Uint8	part_no;
	Uint8	hold_flag;		/* 007: set to hold the parts back */
	Float	x;		/* 008: where the head sits from Akuma */
	Float	y;		/* 00c */
	Uint8	no;		/* 010: the setter's number */
	Uint8	unk_11;
	Uint16	char_no;	/* 012: Akuma's animation when it was set; the effect ends when it changes */
} SHL1E_01_LOCAL_W;
#define SHL1E_01_LW( w )	( ( SHL1E_01_LOCAL_W * )&( w )->obj_mtx )

static SHL1E_01_LOCAL_W *shl1e_01_lw ;		/* the work being run */
static SHL1E_01_LOCAL_W *shl1e_01_master_lw ;	/* a part's head */
static PL1E_LOCAL_W *shl1e_01_pl_lw ;		/* Akuma's */

static void shl1e_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1e_01_set( PLAYER_WORK *pl_w, Uint8 no, Float x, Float y )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1e_01_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1e01 ;
		if( !pl_w->pl_dir ) x = -x ;
		SHL1E_01_LW( ew )->char_no = pl_w->char_no.w ;
		SHL1E_01_LW( ew )->no = no ;
		SHL1E_01_LW( ew )->x = x ;
		SHL1E_01_LW( ew )->y = y ;
	}
	return ew ;
}

/*==============================================================*/
static void shl1e_01_part_make( PLAYER_WORK *ew )
{
	PLAYER_WORK *part_w ;
	Sint8 i ;
	Sint8 n = 0 ;

	for( i = 0 ; i < 12 ; i ++ ){
		if( ( part_w = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			part_w->work_func = shl1e_01_main ;
			part_w->type = 1 ;
			part_w->master_w = ew ;
			part_w->ef_no = 1 ;
			SHL1E_01_LW( part_w )->part_no = n ;
			n ++ ;
			SHL1E_01_LW( part_w )->no = shl1e_01_lw->no ;
		}
	}
}

/*==============================================================*/
static void shl1e_01_part_pos( PLAYER_WORK *ew )
{
	Float x ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	x = shl1e_01_lw->part_no * 32 * 1.6666666f ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
}

static void shl1e_01_head_init( PLAYER_WORK *ew );
static void shl1e_01_head_move( PLAYER_WORK *ew );
static void shl1e_01_head_end( PLAYER_WORK *ew );
static void shl1e_01_free( PLAYER_WORK *ew );
static void shl1e_01_part_init( PLAYER_WORK *ew );
static void shl1e_01_part_move( PLAYER_WORK *ew );
static void shl1e_01_part_end( PLAYER_WORK *ew );
static void shl1e_01_part_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1e_01_main( PLAYER_WORK *ew )
{
	static void ( *shl1e_01_head_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_01_head_init, shl1e_01_head_move, shl1e_01_head_end, shl1e_01_free,
	} ;
	static void ( *shl1e_01_part_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_01_part_init, shl1e_01_part_move, shl1e_01_part_end, shl1e_01_part_free,
	} ;

	if( !ew->type ){
		shl1e_01_pl_lw = PL1E_LW( ew->master_w ) ;
		shl1e_01_lw = SHL1E_01_LW( ew ) ;
		shl1e_01_head_jmp[ew->routine_0]( ew ) ;
	} else {
		shl1e_01_pl_lw = PL1E_LW( ew->master_w ) ;
		shl1e_01_lw = SHL1E_01_LW( ew ) ;
		shl1e_01_master_lw = SHL1E_01_LW( ew->master_w ) ;
		shl1e_01_part_pos( ew ) ;
		shl1e_01_part_jmp[ew->routine_0]( ew ) ;
	}
}

/*==============================================================*/
static void shl1e_01_pos_set( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += shl1e_01_lw->x ;
	ew->pos.y += shl1e_01_lw->y ;
}

/*==============================================================*/
/*			The head's count to the next part's turn			*/
/*==============================================================*/
static void shl1e_01_step_up( PLAYER_WORK *ew )
{
	if( shl1e_01_lw->part_cnt < 12 ){
		if( -- shl1e_01_lw->part_timer <= 0 ){
			shl1e_01_lw->part_timer = 2 ;
			shl1e_01_lw->part_cnt ++ ;
		}
	}
}

/*==============================================================*/
static void shl1e_01_anim_up( PLAYER_WORK *ew )
{
	if( -- ew->timer0 > 0 ) return ;
	ew->timer0 = 2 ;
	shl1e_01_lw->anim_st ++ ;
	if( shl1e_01_lw->anim_st >= 8 ) shl1e_01_lw->anim_st = 5 ;
}

/*==============================================================*/
static void shl1e_01_anim_down( PLAYER_WORK *ew )
{
	if( -- ew->timer0 > 0 ) return ;
	ew->timer0 = 2 ;
	shl1e_01_lw->anim_st -- ;
}

/*==============================================================*/
/*			A part takes the animation step of its head:		*/
/*			three animations a step, the part's own by			*/
/*			anim_no												*/
/*==============================================================*/
static void shl1e_01_char_set( PLAYER_WORK *ew )
{
	Uint8 no ;
	int base ;

	if( shl1e_01_lw->anim_st != shl1e_01_master_lw->anim_st ){
		shl1e_01_lw->anim_st = shl1e_01_master_lw->anim_st ;
		base = shl1e_01_lw->anim_st * 3 ;
		no = 0 ;
		if( shl1e_01_lw->part_no ) no = shl1e_01_lw->anim_no ;
		no += base ;
		pl_char_set( ew, 25, no ) ;
	}
}

/*==============================================================*/
static void shl1e_01_head_init( PLAYER_WORK *ew )
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
	shl1e_01_pos_set( ew ) ;
	ew->prio = 0 ;
	shl1e_01_lw->anim_st = shl1e_01_lw->hold_flag = 0 ;
	ew->timer0 = 2 ;
	ew->dir = 12 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	shl1e_01_lw->part_cnt = 0 ;
	shl1e_01_lw->part_timer = 2 ;
	shl1e_01_part_make( ew ) ;
}

static void shl1e_01_head_wait( PLAYER_WORK *ew );
static void shl1e_01_head_go( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl1e_01_head_move( PLAYER_WORK *ew )
{
	static void ( *shl1e_01_head_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_01_head_wait, shl1e_01_head_go,
	} ;

	shl1e_01_pos_set( ew ) ;
	shl1e_01_head_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl1e_01_head_wait( PLAYER_WORK *ew )
{
	if( ew->pl_type != ew->master_w->pl_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->master_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->master_w->char_no.b.no != 8 ){
		ew->routine_1 ++ ;
		shl1e_01_lw->anim_st = 4 ;
		ew->timer0 = 2 ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	shl1e_01_lw->hold_flag = 0 ;
	if( ew->master_w->timer0 <= 0 ){
		ew->routine_1 ++ ;
		shl1e_01_lw->anim_st = 4 ;
		ew->timer0 = 2 ;
	}
	shl1e_01_step_up( ew ) ;
	shl1e_01_anim_up( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1e_01_head_go( PLAYER_WORK *ew )
{
	if( ew->master_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1e_01_lw->hold_flag = 0 ;
	shl1e_01_anim_down( ew ) ;
	if( shl1e_01_lw->anim_st < 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl1e_01_head_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl1e_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1e_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1e_01_part_init( PLAYER_WORK *ew )
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
	shl1e_01_lw->anim_st = -1 ;
	shl1e_01_lw->hit_cnt = 0 ;
	shl1e_01_lw->anim_no = 1 ;
	ew->atck_no = pl1e_at_gou_hadou ;
	HIT_RESET( ew ) ;
	shl1e_01_char_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1e_01_part_move( PLAYER_WORK *ew )
{
	Uint8 no ;

	if( ew->master_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		shl1e_01_free( ew ) ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	shl1e_01_lw->anim_no = 1 ;
	ew->disp_flag = 1 ;
	if( shl1e_01_master_lw->part_cnt < shl1e_01_lw->part_no ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( shl1e_01_master_lw->part_cnt == shl1e_01_lw->part_no ){
		shl1e_01_lw->anim_no = 2 ;
		shl1e_01_lw->anim_st = -1 ;
		ew->disp_flag = 1 ;
	}
	if( ew->hit_flag ){
		shl1e_01_lw->anim_no = 1 ;
		shl1e_01_lw->anim_st = -1 ;
		shl1e_01_master_lw->hold_flag = 0xff ;
		if( !-- ew->hit_stop ){
			no = 86 ;
			if( ++ shl1e_01_lw->hit_cnt >= 3 ){
				shl1e_01_lw->hit_cnt = 0 ;
				no = 85 ;
			}
			ew->atck_no = no ;
			HIT_RESET( ew ) ;
		}
	}
	shl1e_01_char_set( ew ) ;
	if( !shl1e_01_master_lw->hold_flag ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1e_01_part_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl1e_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1e_01_part_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1e_01.c" End of File	*/
