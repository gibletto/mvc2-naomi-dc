/* Gambit's effects (character 28's shot 00), eleven kinds picked by the work's type: the four
   pieces set along the ground at his entrance, the pieces that show on him while an animation of
   his lasts, the cards he throws and what they leave behind, and Royal Flush's cards and shower.
   Most are made by another of the kinds. */
#include "player.h"
#include "pl_char.h"
#include "pl28_def.h"

/* the effect's own work, from obj_mtx */
typedef struct SHL28_00_LOCAL_W {
	Uint16	char_no;	/* 000: the animation of the master it goes on while */
	Uint16	pl_type;	/* 002 */
	Uint8	unk_04[0x0d - 0x04];
	Uint8	pose;		/* 00d: which of the card's animations */
	Uint8	unk_0e[2];
	Uint16	part_kind;		/* 010: the kind its parts are made as, less 5 */
	Uint8	unk_12[2];
	Uint32	ofs_x;		/* 014: a step across, in 16.16 */
} SHL28_00_LOCAL_W;
#define SHL28_00_LW( w )	( ( SHL28_00_LOCAL_W * )&( w )->obj_mtx )

/* what each kind copies from the player it belongs to */
#define SHL28_00_COPY( ew, mw ) \
	CHAR_PART_COPY( ew, mw ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( mw )->pl_no ; \
	( ew )->pl_type = ( mw )->pl_type ; \
	( ew )->scale.x = ( mw )->scale.x ; \
	( ew )->scale.y = ( mw )->scale.y ; \
	( ew )->atck_level = ( mw )->atck_level ; \
	( ew )->work_no = ( mw )->work_no ; \
	( ew )->col_cnt = ( mw )->col_cnt ; \
	( ew )->scale = ( mw )->scale

static void shl28_00_main( PLAYER_WORK *ew );
static void shl28_00_end( PLAYER_WORK *ew );
static void shl28_00_free( PLAYER_WORK *ew );
static void shl28_00_ex_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl28_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;
	SHL28_00_LOCAL_W *lw ;

	if( pl_w->team_type == 6 && type == 9 ){
		if( ( ew = work_get( 0, W_SHL_ST, LIST_TOP ) ) ){
			ew->work_func = shl28_00_ex_main ;
			ew->master_w = pl_w ;
			ew->type = type ;
			ew->ef_no = 0x2800 ;
			lw = SHL28_00_LW( ew ) ;
			lw->pl_type = ew->pl_type = pl_w->pl_type ;
			lw->char_no = pl_w->char_no.w ;
			return ew ;
		}
	}
	else {
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl28_00_main ;
			ew->master_w = pl_w ;
			ew->type = type ;
			ew->ef_no = 0x2800 ;
			lw = SHL28_00_LW( ew ) ;
			lw->pl_type = ew->pl_type = pl_w->pl_type ;
			lw->char_no = pl_w->char_no.w ;
			return ew ;
		}
	}
	return 0 ;
}

/*==============================================================*/
/*			A part of a kind-3 or kind-4 effect, made as the	*/
/*			kind its master asks for							*/
/*==============================================================*/
static PLAYER_WORK *shl28_00_part_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pw ;
	SHL28_00_LOCAL_W *lw, *pw_lw ;

	if( ( pw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		lw = SHL28_00_LW( ew ) ;
		pw->work_func = shl28_00_main ;
		pw->master_w = ew ;
		pw->hit_flag = 0 ;
		pw->hit_box_no = 0 ;
		pw_lw = SHL28_00_LW( pw ) ;
		pw->type = lw->part_kind + 5 ;
		pw->ef_no = 0x2800 ;
		pw_lw->pl_type = pw->pl_type = ew->master_w->pl_type ;
		pw_lw->char_no = ew->char_no.w ;
		return pw ;
	}
	return 0 ;
}

static void shl28_00_t0_main( PLAYER_WORK *ew );
static void shl28_00_t1_main( PLAYER_WORK *ew );
static void shl28_00_t2_main( PLAYER_WORK *ew );
static void shl28_00_t3_main( PLAYER_WORK *ew );
static void shl28_00_t4_main( PLAYER_WORK *ew );
static void shl28_00_t5_main( PLAYER_WORK *ew );
static void shl28_00_t6_main( PLAYER_WORK *ew );
static void shl28_00_t7_main( PLAYER_WORK *ew );
static void shl28_00_t8_main( PLAYER_WORK *ew );
static void shl28_00_t9_main( PLAYER_WORK *ew );
static void shl28_00_t10_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t0_main, shl28_00_t1_main, shl28_00_t2_main, shl28_00_t3_main,
		shl28_00_t4_main, shl28_00_t5_main, shl28_00_t6_main, shl28_00_t7_main,
		shl28_00_t8_main, shl28_00_t9_main, shl28_00_t10_main,
	} ;

	shl28_00_jmp[ew->type]( ew ) ;
}

/* kind 2 */
static void shl28_00_t2_init( PLAYER_WORK *ew );
static void shl28_00_t2_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t2_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t2_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t2_init, shl28_00_t2_move,
	} ;
	SHL28_00_LOCAL_W *lw = SHL28_00_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.w != lw->char_no ){
		shl28_00_end( ew ) ;
		return ;
	}
	shl28_00_t2_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t2_init( PLAYER_WORK *ew )
{
	static const Uint8 shl28_00_pose_tbl[] = { 0, 1, 2, 3, 4 } ;
	SHL28_00_LOCAL_W *lw ;

	ew->routine_0 ++ ;
	lw = SHL28_00_LW( ew ) ;
	SHL28_00_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 1 ;
	ew->timer0 = -1 ;
	pl_char_set( ew, Char_Effect, shl28_00_pose_tbl[lw->pose] ) ;
	shl28_00_t2_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t2_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint8 no ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
again :
	no = HI_BYTE( pl_w->cg_rival ) ;
	no &= 0xfe ;
	if( !no ){
		ew->disp_flag = 0 ;
		return ;
	}
	if( no < 0 ) shl28_00_end( ew ) ;
	else {
		if( HI_BYTE( ew->cg_rival ) != no ){
			pl_char_move( ew ) ;
			goto again ;
		}
		ew->disp_flag = 1 ;
	}
}

/* kind 10 */
static void shl28_00_t10_init( PLAYER_WORK *ew );
static void shl28_00_t10_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t10_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t10_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t10_init, shl28_00_t10_move,
	} ;
	SHL28_00_LOCAL_W *lw = SHL28_00_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.w != lw->char_no ){
		shl28_00_end( ew ) ;
		return ;
	}
	shl28_00_t10_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t10_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	SHL28_00_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 1 ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 8.0f ) ;
	pl_char_set( ew, Char_Effect, 22 ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t10_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->prio = 0 ;
	if( pl_char_move( ew ) < 0 ) shl28_00_end( ew ) ;
}

/* kind 0 */
static void shl28_00_t0_init( PLAYER_WORK *ew );
static void shl28_00_t0_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t0_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t0_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t0_init, shl28_00_t0_move,
	} ;
	SHL28_00_LOCAL_W *lw = SHL28_00_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.w != lw->char_no ){
		shl28_00_end( ew ) ;
		return ;
	}
	shl28_00_t0_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t0_init( PLAYER_WORK *ew )
{
	static const Uint8 shl28_00_pose2_tbl[] = { 5, 6 } ;
	SHL28_00_LOCAL_W *lw = SHL28_00_LW( ew ) ;
	Sint32 prio ;

	ew->routine_0 ++ ;
	SHL28_00_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 1 ;
	prio = lw->pose ? 7 : 0 ;
	ew->prio = prio ;
	ew->timer0 = prio ;
	pl_char_set( ew, Char_Effect, shl28_00_pose2_tbl[lw->pose] ) ;
	shl28_00_t0_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t0_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = ew->timer0 ;
	pl_char_move( ew ) ;
}

/* kind 1 */
static void shl28_00_t1_init( PLAYER_WORK *ew );
static void shl28_00_t1_run( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t1_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t1_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t1_init, shl28_00_t1_run, shl28_00_end, shl28_00_free,
	} ;

	shl28_00_t1_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t1_init( PLAYER_WORK *ew )
{
	static const Float shl28_00_ofs_tbl[] = { DOT_X( 128.0f ), DOT_X( 112.0f ), DOT_X( 80.0f ), DOT_X( 16.0f ) } ;
	PLAYER_WORK *pl_w ;
	Float ofs ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w->master_w ;
	SHL28_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;	/* copied as in the other kinds, then its own */
	ew->prio = 0 ;
	ew->pos.y = pl_w->ground_y ;
	ew->pos.x = scr_work.eye[0].x ;
	ew->disp_flag = 1 ;
	ofs = shl28_00_ofs_tbl[ew->dir] ;
	if( ew->disp_dir ) ofs -= DOT_X( 128.0f ) ;
	ew->pos.x += ofs ;
	pl_char_set( ew, Char_Effect, 20 ) ;
	pl_voice_req( ew, 31 ) ;
}

static void shl28_00_t1_move( PLAYER_WORK *ew );
static void shl28_00_t1_hit( PLAYER_WORK *ew );
static void shl28_00_t1_fin( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl28_00_t1_run( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t1_run_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t1_move, shl28_00_t1_hit, shl28_00_t1_fin,
	} ;

	shl28_00_t1_run_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t1_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->master_w->hit_flag -- ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_t1_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *mw = ew->master_w ;
	VEC3 pos ;

	if( mw->hit_box_no ){
		ew->routine_1 ++ ;
		pos = ew->pos ;
		burst_set( ew, &ew->pos, 1, 8 ) ;
		pl_se_req( ew, 73 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_t1_fin( PLAYER_WORK *ew )
{
	ew->master_w->hit_box_no -- ;
	shl28_00_end( ew ) ;
}

/* kind 3 */
static void shl28_00_t3_init( PLAYER_WORK *ew );
static void shl28_00_t3_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t3_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t3_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t3_init, shl28_00_t3_move,
	} ;

	shl28_00_t3_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t3_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	SHL28_00_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 0 ;
	ew->timer0 = 6 ;
	ew->timer1 = 4 ;
	ew->dir = 0 ;
	ew->hit_flag = ew->timer1 ;
	ew->hit_box_no = 0 ;
}

static void shl28_00_t3_make( PLAYER_WORK *ew );
static void shl28_00_t3_wait( PLAYER_WORK *ew );
static void shl28_00_t3_fin( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl28_00_t3_move( PLAYER_WORK *ew )
{
	void ( *shl28_00_t3_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t3_make, shl28_00_t3_wait, shl28_00_t3_fin,
	} ;

	shl28_00_t3_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t3_make( PLAYER_WORK *ew )
{
	PLAYER_WORK *cw ;

	ew->timer0 -- ;
	if( ew->timer0 < 0 ){
		cw = shl28_00_set( ew, 1 ) ;
		if( cw ){
			ew->timer0 = 6 ;
			cw->dir = ew->dir ;
			ew->dir ++ ;
			ew->timer1 -- ;
			if( ew->timer1 <= 0 ){
				ew->routine_1 ++ ;
				shl28_00_t3_wait( ew ) ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_t3_wait( PLAYER_WORK *ew )
{
	if( !ew->hit_flag ){
		ew->hit_box_no = 4 ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_t3_fin( PLAYER_WORK *ew )
{
	if( !ew->hit_box_no ) shl28_00_end( ew ) ;
}

/* kind 4 */
static void shl28_00_t4_init( PLAYER_WORK *ew );
static void shl28_00_t4_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t4_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t4_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t4_init, shl28_00_t4_move,
	} ;

	shl28_00_t4_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t4_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	SHL28_00_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 0 ;
	ew->timer0 = 5 ;
	ew->timer1 = 0 ;
	ew->hit_flag = 0 ;
}

static void shl28_00_t4_make( PLAYER_WORK *ew );
static void shl28_00_t4_wait( PLAYER_WORK *ew );
static void shl28_00_t4_hit_ck( PLAYER_WORK *ew );
static void shl28_00_t4_fin( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl28_00_t4_move( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t4_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t4_make, shl28_00_t4_wait, shl28_00_t4_fin,
	} ;

	shl28_00_t4_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t4_make( PLAYER_WORK *ew )
{
	PLAYER_WORK *cw ;

	cw = shl28_00_part_set( ew ) ;
	if( !cw ){
		ew->routine_1 ++ ;
		shl28_00_t4_wait( ew ) ;
		return ;
	}
	ew->timer1 ++ ;
	ew->timer0 -- ;
	if( !ew->timer0 ) ew->routine_1 ++ ;
	shl28_00_t4_wait( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t4_wait( PLAYER_WORK *ew )
{
	if( ew->timer1 ) shl28_00_t4_hit_ck( ew ) ;
	else shl28_00_end( ew ) ;
}

/*==============================================================*/
/*			Waits on the cards behind it: the first one hit		*/
/*			or with a hit box sets the hit flag					*/
/*==============================================================*/
static void shl28_00_t4_hit_ck( PLAYER_WORK *ew )
{
	register PLAYER_WORK *w ;
	register Sint16 n ;

	w = ew->behind_w ;
	n = ew->timer1 - 1 ;
	for(;;){
		if( w->ef_no != 0x2800 ) return ;
		if( w->hit_flag || w->hit_box_no ){
			ew->routine_1 ++ ;
			ew->hit_flag = HR_GUARD ;
			return ;
		} else if( -- n < 0 ) return ;
		w = w->behind_w ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_t4_fin( PLAYER_WORK *ew )
{
	if( !ew->timer1 ) shl28_00_end( ew ) ;
}

/* kind 5, a card that flies */
static void shl28_00_t5_init( PLAYER_WORK *ew );
static void shl28_00_t5_move( PLAYER_WORK *ew );
static void shl28_00_t5_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t5_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t5_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t5_init, shl28_00_t5_move, shl28_00_t5_end, shl28_00_free,
	} ;

	shl28_00_t5_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t5_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *mw ;
	PLAYER_WORK *pl_w ;
	Float vx ;

	ew->routine_0 ++ ;
	mw = ew->master_w ;
	pl_w = mw->master_w ;
	SHL28_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += DOT_Y( 48.0f ) ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->type == 7 ){
		ew->spd.y = DOT_Y( 24.0f ) ;
		ew->pos.y += DOT_Y( 32.0f ) ;
	}
	vx = ew->disp_dir ? DOT_X( 32.0f ) : -DOT_X( 32.0f ) ;
	ew->spd.x = vx ;
	ew->pos.x += vx ;
	if( ew->atck_level == 0 ){
		ew->atck_no = pl28_at_card ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl28_at_card_h ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 20, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t5_move( PLAYER_WORK *ew )
{
	Sint8 hf ;

	if( !ew->master_w->hit_flag ){
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( scr_in_x_ck( ew ) ){
			hit_work_set( ew ) ;
			return ;
		}
	}
	ew->master_w->timer1 -- ;
	if( !ew->hit_box_no ){
		hf = ew->hit_flag ;
		if( !hf || !( hf & 1 ) ){
			shl28_00_end( ew ) ;
			return ;
		}
	}
	ew->routine_0 ++ ;
	burst_set( ew, &ew->pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t5_end( PLAYER_WORK *ew )
{
	shl28_00_end( ew ) ;
}

/*==============================================================*/
/*			Kind 7 goes as kind 5								*/
/*==============================================================*/
static void shl28_00_t7_main( PLAYER_WORK *ew )
{
	shl28_00_t5_main( ew ) ;
}

/* kind 6, a card that falls */
static void shl28_00_t6_init( PLAYER_WORK *ew );
static void shl28_00_t6_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t6_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t6_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t6_init, shl28_00_t6_move, shl28_00_t5_end, shl28_00_free,
	} ;

	shl28_00_t6_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t6_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w->master_w ;
	SHL28_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += DOT_Y( 96.0f ) ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = -DOT_Y( 24.0f ) ;
	if( ew->disp_dir ) ew->spd.x = DOT_X( 32.0f ) ;
	else ew->spd.x = -DOT_X( 32.0f ) ;
	ew->pos.x += -DOT_X( 16.0f ) ;
	if( ew->atck_level == 0 ){
		ew->atck_no = pl28_at_card ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl28_at_card_h ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 20, 0 ) ;
}

static void shl28_00_t6_miss( PLAYER_WORK *ew );
static void shl28_00_t6_hit( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl28_00_t6_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *mw = ew->master_w ;
	PLAYER_WORK *pl_w = mw->master_w ;

	if( ew->routine_1 ){
		if( pl_char_move( ew ) < 0 ) shl28_00_t6_hit( ew ) ;
	} else if( mw->hit_flag ) shl28_00_t6_miss( ew ) ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !scr_in_x_ck( ew ) ) shl28_00_t6_miss( ew ) ;
		else if( pl_w->ground_y > ew->pos.y ){
			ew->pos.y = pl_w->ground_y ;
			mw->timer1 -- ;
			if( mw->timer1 ) shl28_00_end( ew ) ;
			else {
				ew->routine_1 ++ ;
				pl_char_set( ew, 20, 1 ) ;
			}
		} else hit_work_set( ew ) ;
	}
}

/*==============================================================*/
static void shl28_00_t6_miss( PLAYER_WORK *ew )
{
	Sint8 hf ;

	ew->master_w->timer1 -- ;
	if( !ew->hit_box_no ){
		hf = ew->hit_flag ;
		if( !hf || !( hf & 1 ) ){
			shl28_00_end( ew ) ;
			return ;
		}
	}
	shl28_00_t6_hit( ew ) ;
}

/*==============================================================*/
static void shl28_00_t6_hit( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	burst_set( ew, &ew->pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
	shl28_00_end( ew ) ;
}

/* kind 8, a card thrown along a line */
static void shl28_00_t8_init( PLAYER_WORK *ew );
static void shl28_00_t8_move( PLAYER_WORK *ew );
static void shl28_00_t8_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t8_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t8_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t8_init, shl28_00_t8_move, shl28_00_t8_end, shl28_00_free,
	} ;

	shl28_00_t8_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t8_init( PLAYER_WORK *ew )
{
	static const Uint8 shl28_00_dir_tbl[] = { 23, 24, 25, 24, 23, 24, 25, 24 } ;

	ew->routine_0 ++ ;
	SHL28_00_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 48.0f ) ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->sub_type ){
		ew->atck_no = pl28_at_royal ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl28_at_royal_card8 ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 20, 0 ) ;
	ew->dir = shl28_00_dir_tbl[ew->dir] ;
	if( ew->disp_dir ){
		ew->dir = -ew->dir ;
		ew->dir &= 31 ;
	}
	shl28_00_t8_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t8_move( PLAYER_WORK *ew )
{
	dir_spd_move( ew, 0x960 ) ;
	if( ew->hit_flag ) goto flag ;
	if( ew->hit_box_no ) goto hit ;
	if( scr_in_x_ck( ew ) ){
		hit_work_set( ew ) ;
		return ;
	}
	goto end ;
flag:
	if( ew->hit_flag & HR_GUARD ) goto hit ;
end:
	shl28_00_end( ew ) ;
	return ;
hit:
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	burst_set( ew, &ew->pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_t8_end( PLAYER_WORK *ew )
{
	shl28_00_free( ew ) ;
}

/* kind 9, the hyper combo's shower */
static void shl28_00_t9_init( PLAYER_WORK *ew );
static void shl28_00_t9_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_00_t9_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t9_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t9_init, shl28_00_t9_move, shl28_00_end, shl28_00_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		shl28_00_end( ew ) ;
		return ;
	}
	shl28_00_t9_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t9_init( PLAYER_WORK *ew )
{
	SHL28_00_LOCAL_W *lw ;
	Float dx ;

	ew->routine_0 ++ ;
	SHL28_00_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->atck_no = pl28_at_royal_shower ;
	HIT_RESET( ew ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 104.0f ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	lw = SHL28_00_LW( ew ) ;
	if( ew->master_w->pl_dir ){
		lw->ofs_x = 0xFFE80000 ;
		dx = -40.0f ;
		ew->spd.x = dx ;
		ew->pos.x += dx ;
		ew->spd.x = DOT_X( 0.5f ) ;
	}
	else {
		lw->ofs_x = 0x180000 ;
		dx = 40.0f ;
		ew->spd.x = dx ;
		ew->pos.x += dx ;
		ew->spd.x = -DOT_X( 0.5f ) ;
	}
	ew->spd.y = DOT_Y( 12.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	pl_char_set( ew, 22, 2 ) ;
}

static void shl28_00_t9_up( PLAYER_WORK *ew );
static void shl28_00_t9_fall( PLAYER_WORK *ew );
static void shl28_00_t9_stay( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl28_00_t9_move( PLAYER_WORK *ew )
{
	static void ( *shl28_00_t9_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t9_up, shl28_00_t9_fall, shl28_00_t9_stay,
	} ;

	ew->pos.x = ew->master_w->pos.x ;
	shl28_00_t9_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl28_00_t9_up( PLAYER_WORK *ew )
{
	SHL28_00_LOCAL_W *lw = SHL28_00_LW( ew ) ;

	ew->pos.x += lw->ofs_x * 1.6666666f / 65536.0f ;
	if( ( Sint8 )ew->master_w->cg_flag1 < 0 ){
		ew->routine_1 ++ ;
		shl28_00_t9_fall( ew ) ;
		return ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_t9_fall( PLAYER_WORK *ew )
{
	SHL28_00_LOCAL_W *lw ;
	Float floor_y ;

	if( !( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) ){
		pl_char_move( ew ) ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	floor_y = ew->master_w->pos.y + DOT_Y( 88.0f ) ;
	if( !( ew->pos.y > floor_y ) ){
		lw = SHL28_00_LW( ew ) ;
		ew->routine_1 ++ ;
		ew->pos.y = floor_y ;
		ew->pos.x = ew->master_w->pos.x ;
		if( ew->pl_dir ) lw->ofs_x = 0x290000 ;
		else lw->ofs_x = 0xFFD70000 ;
		ew->pos.x += lw->ofs_x * 1.6666666f / 65536.0f ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_char_set( ew, 22, 4 ) ;
	}
	if( ew->spd.y >= 0.0f ){
		floor_y = ew->master_w->pos.y + DOT_Y( 328.0f ) ;
		if( ew->pos.y > floor_y ) hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_t9_stay( PLAYER_WORK *ew )
{
	SHL28_00_LOCAL_W *lw = SHL28_00_LW( ew ) ;

	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.x += lw->ofs_x * 1.6666666f / 65536.0f ;
	if( ew->master_w->cg_flag1 ){
		if( ( Sint8 )ew->master_w->cg_flag1 < 0 ) shl28_00_end( ew ) ;
		else {
			ew->master_w->cg_ctr = 8 ;
			ew->master_w->cg_flag1 = 0 ;
		}
	}
}

/*==============================================================*/
/*			The same shower for the hyper combo made as a		*/
/*			team move											*/
/*==============================================================*/
static void shl28_00_ex_main( PLAYER_WORK *ew )
{
	static void ( *shl28_00_ex_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_00_t9_init, shl28_00_t9_move, shl28_00_end, shl28_00_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ) shl28_00_end( ew ) ;
	else {
		if( vs_sys_work.game_stop & ( 1 << ( pl_w->pl_no ^ 1 ) ) ) return ;
		if( vs_sys_work.game_stop & ( 1 << pl_w->pl_no ) ){
			if( !pl_w->stop_move_flag ) return ;
			if( !pl_w->stop_move_timer ) return ;
		}
		shl28_00_ex_jmp[ew->routine_0]( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl28_00_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl28_00.c" End of File	*/
