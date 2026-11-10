/* Ryu's Shinkuu Hadouken (character 00's shot 01): a head held while Ryu's animation lasts, and a
   line of twelve parts behind it that hit, each drawn once the head has reached it. */
#include "player.h"
#include "pl_char_old.h"
#include "pl00_def.h"

/* the effect's own work, from obj_mtx */
typedef struct SHL00_01_LOCAL_W {
	Sint8	anm_no;		/* 000: the animation's step: up to 8, then from 5 again */
	Sint8	anm_ofs;		/* 001: a part's: added to the animation */
	Uint8	part_cnt;		/* 002: the head's: the parts reached, to 12 */
	Uint8	part_timer;		/* 003: the head's: frames to the next part */
	Uint8	unk_4;
	Uint8	hit_cnt;		/* 005: a part's hits */
	Uint8	part_no;		/* 006: a part's place in the line */
	Uint8	hit_ng_flag;		/* 007: the head's: set while the parts don't hit */
	Float	x;		/* 008: where the head is from Ryu */
	Float	y;		/* 00c */
	Uint8	no;		/* 010: the setter's no */
	Uint8	unk_11;
	Uint16	char_no;	/* 012: Ryu's animation when it was set */
} SHL00_01_LOCAL_W;
#define SHL00_01_LW( w )	( ( SHL00_01_LOCAL_W * )&( w )->obj_mtx )

static SHL00_01_LOCAL_W *lw;	/* the running work's */
static SHL00_01_LOCAL_W *head_lw;	/* a part's: the head's */
static PL00_LOCAL_W *pl_lw;	/* the master's */
static Uint8 unk_0C2FB300[4];

static void shl00_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl00_01_set( PLAYER_WORK *pl_w, Uint8 no, Float x, Float y )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl00_01_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->ef_no = 1 ;
		if( !pl_w->pl_dir ) x = -x ;
		SHL00_01_LW( ew )->char_no = pl_w->char_no.w ;
		SHL00_01_LW( ew )->no = no ;
		SHL00_01_LW( ew )->x = x ;
		SHL00_01_LW( ew )->y = y ;
	}
	return ew ;
}

/*==============================================================*/
/*			The twelve parts behind the head					*/
/*==============================================================*/
static void shl00_01_part_set( PLAYER_WORK *hw )
{
	PLAYER_WORK *ew ;
	Sint8 i ;
	Uint8 n ;

	n = 0 ;
	for( i = 0 ; i < 12 ; i ++ ){
		if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
			ew->work_func = shl00_01_main ;
			ew->type = 1 ;
			ew->master_w = hw ;
			ew->ef_no = 1 ;
			SHL00_01_LW( ew )->part_no = n ;
			n ++ ;
			SHL00_01_LW( ew )->no = lw->no ;
		}
	}
}

/*==============================================================*/
static void shl00_01_part_pos( PLAYER_WORK *ew )
{
	Float x ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	x = lw->part_no * 32 * DOT_X( 1.0f ) ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
}

static void shl00_01_init( PLAYER_WORK *ew );
static void shl00_01_move( PLAYER_WORK *ew );
static void shl00_01_end( PLAYER_WORK *ew );
static void shl00_01_free( PLAYER_WORK *ew );
static void shl00_01_part_init( PLAYER_WORK *ew );
static void shl00_01_part_move( PLAYER_WORK *ew );
static void shl00_01_part_end( PLAYER_WORK *ew );
static void shl00_01_part_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl00_01_main( PLAYER_WORK *ew )
{
	static void ( *shl00_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl00_01_init, shl00_01_move, shl00_01_end, shl00_01_free,
	} ;
	static void ( *shl00_01_part_jmp[] )( PLAYER_WORK *ew ) = {
		shl00_01_part_init, shl00_01_part_move, shl00_01_part_end, shl00_01_part_free,
	} ;

	if( !ew->type ){
		pl_lw = ( PL00_LOCAL_W * )ew->master_w->tlocal ;
		lw = SHL00_01_LW( ew ) ;
		shl00_01_jmp[ew->routine_0]( ew ) ;
	} else {
		pl_lw = ( PL00_LOCAL_W * )ew->master_w->tlocal ;
		lw = SHL00_01_LW( ew ) ;
		head_lw = SHL00_01_LW( ew->master_w ) ;
		shl00_01_part_pos( ew ) ;
		shl00_01_part_jmp[ew->routine_0]( ew ) ;
	}
}

/*==============================================================*/
/*			The head, where the setter put it from Ryu			*/
/*==============================================================*/
static void shl00_01_pos_set( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += lw->x ;
	ew->pos.y += lw->y ;
}

/*==============================================================*/
/*			One more part reached every two frames				*/
/*==============================================================*/
static void shl00_01_part_cnt( PLAYER_WORK *ew )
{
	if( lw->part_cnt < 12 ){
		if( --lw->part_timer <= 0 ){
			lw->part_timer = 2 ;
			lw->part_cnt ++ ;
		}
	}
}

/*==============================================================*/
static void shl00_01_anm_up( PLAYER_WORK *ew )
{
	if( --ew->timer0 <= 0 ){
		ew->timer0 = 2 ;
		lw->anm_no ++ ;
		if( lw->anm_no >= 8 ) lw->anm_no = 5 ;
	}
}

/*==============================================================*/
static void shl00_01_anm_down( PLAYER_WORK *ew )
{
	if( --ew->timer0 <= 0 ){
		ew->timer0 = 2 ;
		lw->anm_no -- ;
	}
}

/*==============================================================*/
/*			A part takes the head's step of the animation		*/
/*==============================================================*/
static void shl00_01_char_set( PLAYER_WORK *ew )
{
	int base ;
	Uint8 no ;

	if( lw->anm_no != head_lw->anm_no ){
		lw->anm_no = head_lw->anm_no ;
		base = lw->anm_no * 3 ;
		no = 0 ;
		if( lw->part_no ) no = lw->anm_ofs ;
		no += base ;
		pl_char_set( ew, 25, no ) ;
	}
}

/*==============================================================*/
static void shl00_01_init( PLAYER_WORK *ew )
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
	shl00_01_pos_set( ew ) ;
	ew->prio = 0 ;
	lw->anm_no = lw->hit_ng_flag = 0 ;
	ew->timer0 = 2 ;
	ew->dir = 12 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	lw->part_cnt = 0 ;
	lw->part_timer = 2 ;
	shl00_01_part_set( ew ) ;
}

static void shl00_01_hold( PLAYER_WORK *ew );
static void shl00_01_fade( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl00_01_move( PLAYER_WORK *ew )
{
	static void ( *shl00_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl00_01_hold, shl00_01_fade,
	} ;

	shl00_01_pos_set( ew ) ;
	shl00_01_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			While Ryu holds the pose							*/
/*==============================================================*/
static void shl00_01_hold( PLAYER_WORK *ew )
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
	if( ew->master_w->char_no.b.no != 4 && ew->master_w->char_no.b.no != 6 ){
		ew->routine_1 ++ ;
		lw->anm_no = 4 ;
		ew->timer0 = 2 ;
		return ;
	}
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		lw->hit_ng_flag = 0 ;
		if( ew->master_w->timer0 <= 0 ){
			ew->routine_1 ++ ;
			lw->anm_no = 4 ;
			ew->timer0 = 2 ;
		}
		shl00_01_part_cnt( ew ) ;
		shl00_01_anm_up( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl00_01_fade( PLAYER_WORK *ew )
{
	if( ew->master_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	lw->hit_ng_flag = 0 ;
	shl00_01_anm_down( ew ) ;
	if( lw->anm_no < 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl00_01_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl00_01_free( ew ) ;
	pl_voice_req( ew, 43 ) ;
}

/*--------------------------------------------------------------*/
static void shl00_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl00_01_part_init( PLAYER_WORK *ew )
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
	lw->anm_no = -1 ;
	lw->hit_cnt = 0 ;
	lw->anm_ofs = 1 ;
	ew->atck_no = pl00_at_shinkuu_beam ;
	HIT_RESET( ew ) ;
	shl00_01_char_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl00_01_part_move( PLAYER_WORK *ew )
{
	Uint8 no ;

	if( ew->master_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		shl00_01_free( ew ) ;
		return ;
	}
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		lw->anm_ofs = 1 ;
		ew->disp_flag = 1 ;
		if( head_lw->part_cnt < lw->part_no ){
			ew->disp_flag = 0 ;
			return ;
		}
		if( head_lw->part_cnt == lw->part_no ){
			lw->anm_ofs = 2 ;
			lw->anm_no = -1 ;
			ew->disp_flag = 1 ;
		}
		if( ew->hit_flag ){
			lw->anm_ofs = 1 ;
			lw->anm_no = -1 ;
			head_lw->hit_ng_flag = 0xff ;
			if( !--ew->hit_stop ){
				no = 86 ;
				if( ++lw->hit_cnt >= 3 ){
					lw->hit_cnt = 0 ;
					no = 85 ;
				}
				ew->atck_no = no ;
				HIT_RESET( ew ) ;
			}
		}
		shl00_01_char_set( ew ) ;
		if( !head_lw->hit_ng_flag ) hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl00_01_part_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl00_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl00_01_part_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl00_01.c" End of File	*/
