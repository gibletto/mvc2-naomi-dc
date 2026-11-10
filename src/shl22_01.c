/* Sakura's effect 01 (character 22's shot 01): the swarm of petals of her dark Shinkuu Hadouken. Type 0
   is the master that stays with her and makes twelve petals, type 1, which follow it in a line. */
#include "player.h"
#include "pl_char.h"
#include "pl22_def.h"

/* the effect's own work, from obj_mtx */
typedef struct SHL22_01_LOCAL_W {
	Sint8	anim_no;		/* 000: the animation the effect shows, -1 for none */
	Sint8	anim_ofs;
	Uint8	part_cnt;		/* 002: how many petals the master has let out */
	Uint8	part_timer;
	Uint8	unk_04;
	Uint8	hit_cnt;		/* 005: the petal's hits so far */
	Uint8	part_no;		/* 006: the petal's place in the line */
	Uint8	part_hit_flag;		/* 007: set while a petal hits */
	Float	x, y;		/* 008: where the master is from Sakura */
	Sint8	unk_10;		/* 010: copied to each petal */
	Uint8	unk_11;
	Uint16	char_no;	/* 012: Sakura's animation when it was made */
} SHL22_01_LOCAL_W;
#define SHL22_01_LW( w )	( ( SHL22_01_LOCAL_W * )&( w )->obj_mtx )

static void shl22_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl22_01_set( PLAYER_WORK *pl_w, Float x, Float y )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl22_01_main ;
		ew->type = 0 ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2201 ;
		if( pl_w->pl_dir ) x = -x ;
		SHL22_01_LW( ew )->char_no = pl_w->char_no.w ;
		SHL22_01_LW( ew )->unk_10 = 0 ;
		SHL22_01_LW( ew )->x = x ;
		SHL22_01_LW( ew )->y = y ;
	}
	return ew ;
}

/*==============================================================*/
static void shl22_01_clone_set( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw )
{
	PLAYER_WORK *pt_w ;
	Sint8 i ;
	Uint8 j ;

	j = 0 ;
	for( i = 0; i < 12; i ++ ){
		if( ( pt_w = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			pt_w->work_func = shl22_01_main ;
			pt_w->type = 1 ;
			pt_w->master_w = ew ;
			pt_w->ef_no = 0x2201 ;
			SHL22_01_LW( pt_w )->part_no = j ;
			j ++ ;
			SHL22_01_LW( pt_w )->unk_10 = lw->unk_10 ;
		}
	}
}

/*==============================================================*/
/*			A petal's place in the line behind the master		*/
/*==============================================================*/
static void shl22_01_part_pos_set( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw )
{
	Float x ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	x = ( lw->part_no << 5 ) * 1.6666666f ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
}

static void shl22_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw );
static void shl22_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw );
static void shl22_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw );
static void shl22_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw );
static void shl22_01_part_init( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw );
static void shl22_01_part_move( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw );
static void shl22_01_part_end( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw );
static void shl22_01_part_free( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw );

/*==============================================================*/
static void shl22_01_main( PLAYER_WORK *ew )
{
	static void ( *shl22_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw ) = {
		shl22_01_init, shl22_01_move, shl22_01_end, shl22_01_free,
	} ;
	static void ( *shl22_01_part_jmp[] )( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw ) = {
		shl22_01_part_init, shl22_01_part_move, shl22_01_part_end, shl22_01_part_free,
	} ;
	SHL22_01_LOCAL_W *lw = SHL22_01_LW( ew ) ;
	SHL22_01_LOCAL_W *mlw ;

	if( !ew->type ){
		shl22_01_jmp[ew->routine_0]( ew, ew->master_w, lw ) ;
	}
	else {
		mlw = SHL22_01_LW( ew->master_w ) ;
		shl22_01_part_pos_set( ew, lw ) ;
		ew->prio = ew->master_w->prio ;
		shl22_01_part_jmp[ew->routine_0]( ew, lw, mlw ) ;
	}
}

/*==============================================================*/
/*			The master stays on Sakura, shifted by where it		*/
/*			was set												*/
/*==============================================================*/
static void shl22_01_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.x += lw->x ;
	ew->pos.y += lw->y ;
}

/*==============================================================*/
/*			Lets petals out, one for every three frames			*/
/*==============================================================*/
static void shl22_01_count( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw )
{
	if( lw->part_cnt >= 12 ) return ;
	if( --lw->part_timer > 0 ) return ;
	lw->part_timer = 2 ;
	lw->part_cnt ++ ;
}

/*==============================================================*/
static void shl22_01_next( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw )
{
	if( --ew->timer0 <= 0 ){
		ew->timer0 = 2 ;
		lw->anim_no ++ ;
		if( lw->anim_no >= 8 ) lw->anim_no = 5 ;
	}
}

/*==============================================================*/
static void shl22_01_prev( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw )
{
	if( --ew->timer0 <= 0 ){
		ew->timer0 = 2 ;
		lw->anim_no -- ;
	}
}

/*==============================================================*/
/*			A petal shows the master's animation, three of		*/
/*			its own to each of the master's						*/
/*==============================================================*/
static void shl22_01_part_anim( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw )
{
	Sint32 no, n ;

	if( lw->anim_no == mlw->anim_no ) return ;
	lw->anim_no = mlw->anim_no ;
	no = lw->anim_no * 3 ;
	n = 0 ;
	if( lw->part_no ) n = lw->anim_ofs ;
	n += no ;
	pl_char_set( ew, 25, n ) ;
}

/*==============================================================*/
static void shl22_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw )
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
	shl22_01_pos_set( ew, pl_w, lw ) ;
	ew->sub_prio = -16 ;
	lw->part_hit_flag = 0 ;
	lw->anim_no = 0 ;
	ew->timer0 = 2 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	lw->part_cnt = 0 ;
	lw->part_timer = 2 ;
	shl22_01_clone_set( ew, lw ) ;
}

static void shl22_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw );
static void shl22_01_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void shl22_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw )
{
	static void ( *shl22_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw ) = {
		shl22_01_wait, shl22_01_back,
	} ;

	ew->prio = pl_w->prio ;
	shl22_01_pos_set( ew, pl_w, lw ) ;
	shl22_01_move_jmp[ew->routine_1]( ew, pl_w, lw ) ;
}

/*==============================================================*/
/*			It lets the petals out while Sakura's animation		*/
/*			Char_Super 12 runs									*/
/*==============================================================*/
static void shl22_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw )
{
	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_w->char_no.b.no != 12 ){
		ew->routine_1 ++ ;
		lw->anim_no = 4 ;
		ew->timer0 = 2 ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	lw->part_hit_flag = 0 ;
	if( pl_w->timer0 <= 0 ){
		ew->routine_1 ++ ;
		lw->anim_no = 4 ;
		ew->timer0 = 2 ;
	}
	shl22_01_count( ew, lw ) ;
	shl22_01_next( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl22_01_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw )
{
	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	lw->part_hit_flag = 0 ;
	shl22_01_prev( ew, lw ) ;
	if( lw->anim_no < 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl22_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl22_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL22_01_LOCAL_W *lw )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl22_01_part_init( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw )
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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->sub_prio = -16 ;
	lw->anim_no = -1 ;
	lw->hit_cnt = 0 ;
	lw->anim_ofs = 1 ;
	ew->atck_no = pl22_at_dshinkuu ;
	HIT_RESET( ew ) ;
	shl22_01_part_anim( ew, lw, mlw ) ;
}

/*--------------------------------------------------------------*/
static void shl22_01_part_move( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw )
{
	Uint8 atck_no ;

	if( ew->master_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		work_free( ew ) ;
		return ;
	}
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	lw->anim_ofs = 1 ;
	ew->disp_flag = 1 ;
	if( mlw->part_cnt < lw->part_no ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( mlw->part_cnt == lw->part_no ){
		lw->anim_ofs = 2 ;
		lw->anim_no = -1 ;
		ew->disp_flag = 1 ;
	}
	if( ew->hit_flag ){
		lw->anim_ofs = 1 ;
		lw->anim_no = -1 ;
		mlw->part_hit_flag = 0xff ;
		if( !--ew->hit_stop ){
			atck_no = 86 ;
			if( ++lw->hit_cnt >= 3 ){
				lw->hit_cnt = 0 ;
				atck_no = 85 ;
			}
			ew->atck_no = atck_no ;
			HIT_RESET( ew ) ;
		}
	}
	shl22_01_part_anim( ew, lw, mlw ) ;
	if( !mlw->part_hit_flag ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl22_01_part_end( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl22_01_part_free( PLAYER_WORK *ew, SHL22_01_LOCAL_W *lw, SHL22_01_LOCAL_W *mlw )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl22_01.c" End of File	*/
