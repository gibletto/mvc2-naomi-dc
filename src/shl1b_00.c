/* Chun-Li's effects (character 1b's shot 00): the Kikouken, the hyper's shots and kicks, and the
   pictures of her other moves, twelve kinds picked by the setter. */
#include "player.h"
#include "pl_char.h"
#include "pl1b_def.h"

/* the effect's own work */
typedef struct SHL1B_00_LOCAL_W {
	Uint16	char_no;	/* 000: Chun-Li's animation when it was set */
	Uint8	unk_2[2];
	Float	x;		/* 004: kind 3's distance moved */
} SHL1B_00_LOCAL_W;
#define SHL1B_00_LW( w )	( ( SHL1B_00_LOCAL_W * )&( w )->obj_flag )

static SHL1B_00_LOCAL_W *lw;	/* the running work's */
static PL1B_LOCAL_W *pl_lw;	/* the master's */

static void shl1b_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1b_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1b_00_main ;
		ew->master_w = pl_w ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->ef_no = 0x1b00 ;
	}
	return ew ;
}

/****************************************************************/
/*			N parts that ride on the effect hw, numbered in		*/
/*			sub_type											*/
/****************************************************************/
void *shl1b_00_set2( PLAYER_WORK *hw, Uint8 type, Uint8 n )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < n ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl1b_00_main ;
			ew->master_w = hw->master_w ;
			ew->ef_w = hw ;
			ew->type = type ;
			ew->sub_type = i ;
			ew->ef_no = 0x1b00 ;
		} else break ;
	}
	return ew ;
}

static void shl1b_00_kikou( PLAYER_WORK *ew );
static void shl1b_00_kikoushou( PLAYER_WORK *ew );
static void shl1b_00_kikoushou_pic( PLAYER_WORK *ew );
static void shl1b_00_3( PLAYER_WORK *ew );
static void shl1b_00_kick( PLAYER_WORK *ew );
static void shl1b_00_ball_pic( PLAYER_WORK *ew );
static void shl1b_00_ball( PLAYER_WORK *ew );
static void shl1b_00_shichisei( PLAYER_WORK *ew );
static void shl1b_00_9( PLAYER_WORK *ew );
static void shl1b_00_10( PLAYER_WORK *ew );
static void shl1b_00_11( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_main( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_kikou, shl1b_00_kikoushou, shl1b_00_kikoushou_pic, shl1b_00_3, shl1b_00_kick, shl1b_00_ball_pic,
		shl1b_00_ball, shl1b_00_kikou, shl1b_00_shichisei, shl1b_00_9, shl1b_00_10, shl1b_00_11,
	} ;

	pl_lw = ( PL1B_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = SHL1B_00_LW( ew ) ;
	shl1b_00_jmp[ew->type]( ew ) ;
}

static void shl1b_00_kikou_init( PLAYER_WORK *ew );
static void shl1b_00_kikou_move( PLAYER_WORK *ew );
static void shl1b_00_kikou_hit( PLAYER_WORK *ew );
static void shl1b_00_end( PLAYER_WORK *ew );
static void shl1b_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_kikou( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_kikou_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_kikou_init, shl1b_00_kikou_move, shl1b_00_kikou_hit, shl1b_00_free,
	} ;

	shl1b_00_kikou_jmp[ew->routine_0]( ew ) ;
}

/* the Kikouken's speed by strength */
static Float shl1b_00_kikou_spd[2] = { DOT_X( 4.0f ), 10.0f } ;

/*==============================================================*/
static void shl1b_00_kikou_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->prio = 0 ;
	pl_lw->shot_flag = 1 ;
	lw->char_no = ew->master_w->char_no.w ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( pl_w->team_type == 3 ){
		ew->atck_no = pl1b_at_kikou_counter ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl1b_at_kikou ;
		HIT_RESET( ew ) ;
	}
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->disp_dir ? DOT_X( 80.0f ) : -DOT_X( 80.0f ) ;
	ew->pos.y += DOT_Y( 64.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? shl1b_00_kikou_spd[ew->atck_level] : -shl1b_00_kikou_spd[ew->atck_level] ;
	pl_char_set( ew, 20, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_kikou_move( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_lw->shot_flag = 0 ;
		pl_char_set( ew, 20, 1 ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		pl_lw->shot_flag = 0 ;
		shl1b_00_end( ew ) ;
		return ;
	}
	ew->prio = 0 ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_kikou_hit( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl1b_00_end( ew ) ;
	return ;
}

static void shl1b_00_kikoushou_init( PLAYER_WORK *ew );
static void shl1b_00_kikoushou_move( PLAYER_WORK *ew );
static void shl1b_00_kikoushou_hold( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_kikoushou( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_kikoushou_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_kikoushou_init, shl1b_00_kikoushou_move, shl1b_00_kikoushou_hold, shl1b_00_free,
	} ;

	shl1b_00_kikoushou_jmp[ew->routine_0]( ew ) ;
}

static void shl1b_00_kikoushou_far_ck( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_kikoushou_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	pl_lw->kikoushou_shot_flag = 1 ;
	lw->char_no = ew->master_w->char_no.w ;
	ew->routine_3 = 0 ;
	ew->timer0 = 8 ;
	ew->master_w->timer1 = 8 ;
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl1b_at_kikoushou ;
	HIT_RESET( ew ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	shl1b_00_kikoushou_far_ck( ew ) ;
	pl_char_set( ew, 20, 2 ) ;
	hit_work_set( ew ) ;
}

/* the frames between the hits */
static Sint16 shl1b_00_kikoushou_time[16] = {
	12, 12, 10, 10, 8, 8, 4, 4, 4, 4, 4, 4, 3, 3, 2, 2,
} ;

/*--------------------------------------------------------------*/
static void shl1b_00_kikoushou_move( PLAYER_WORK *ew )
{
	if( lw->char_no != ew->master_w->char_no.w ){
		shl1b_00_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->hit_box_no = 0 ;
	if( -- ew->timer0 < 0 ){
		ew->routine_3 ++ ;
		ew->timer0 = shl1b_00_kikoushou_time[ew->routine_3] ;
		ew->atck_no = ew->master_w->timer1 == 1 ? 65 : 64 ;
		HIT_RESET( ew ) ;
		shl1b_00_kikoushou_far_ck( ew ) ;
		if( -- ew->master_w->timer1 < 0 ){
			ew->routine_0 ++ ;
			pl_lw->kikoushou_shot_flag = 0 ;
			pl_char_set( ew, 20, 4 ) ;
		} else {
			pl_char_move( ew ) ;
		}
	} else {
		pl_char_move( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_kikoushou_hold( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !pl_w->routine_1 && pl_w->act_st == N_SUPER && pl_w->pl_sp_type == pl1b_sp_kikoushou ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->prio = 0 ;
		if( pl_char_move( ew ) >= 0 ) return ;
	}
	shl1b_00_end( ew ) ;
}

/*==============================================================*/
/*			The last hits knock down a target far from her		*/
/*==============================================================*/
static void shl1b_00_kikoushou_far_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *em = ew->master_w->target_w ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	if( pl_w->timer1 == 1 ) return ;
	x = pl_w->pos.x + ( pl_w->pl_dir ? DOT_X( 64.0f ) : -DOT_X( 64.0f ) ) ;
	x = em->pos.x - x ;
	if( fabs( x ) > 80.0f ) ew->atck_attr = ATR_BACK_POS ;
}

static void shl1b_00_kikoushou_pic_init( PLAYER_WORK *ew );
static void shl1b_00_kikoushou_pic_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_kikoushou_pic( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_kikoushou_pic_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_kikoushou_pic_init, shl1b_00_kikoushou_pic_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_kikoushou_pic_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1b_00_kikoushou_pic_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->disp_dir = ew->master_w->disp_dir ;
	lw->char_no = ew->master_w->char_no.w ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	shl1b_00_kikoushou_pic_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_kikoushou_pic_move( PLAYER_WORK *ew )
{
	if( lw->char_no != ew->master_w->char_no.w ){
		shl1b_00_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
	if( HI_BYTE( ew->master_w->cg_rival ) != HI_BYTE( ew->cg_rival ) ) pl_char_move( ew ) ;
}

static void shl1b_00_3_init( PLAYER_WORK *ew );
static void shl1b_00_3_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_3( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_3_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_3_init, shl1b_00_3_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_3_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1b_00_3_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->disp_dir = ew->master_w->disp_dir ;
	lw->char_no = ew->master_w->char_no.w ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	lw->x = 0.0f ;
	ew->spd.x = ew->disp_dir ? 1.6666666f : -1.6666666f ;
	pl_char_set( ew, Char_Effect, 3 ) ;
	shl1b_00_3_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_3_move( PLAYER_WORK *ew )
{
	if( lw->char_no != ew->master_w->char_no.w ){
		shl1b_00_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
	lw->x += ew->spd.x ;
	ew->pos.x += lw->x ;
	if( ew->master_w->timer1 <= 0 ){
		shl1b_00_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void shl1b_00_ball_pic_init( PLAYER_WORK *ew );
static void shl1b_00_ball_pic_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_ball_pic( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_ball_pic_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_ball_pic_init, shl1b_00_ball_pic_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_ball_pic_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1b_00_ball_pic_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
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
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->prio = 0 ;
	lw->char_no = ew->master_w->char_no.w ;
	pl_char_set( ew, Char_Effect, 8 ) ;
	shl1b_00_ball_pic_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_ball_pic_move( PLAYER_WORK *ew )
{
	if( lw->char_no != ew->master_w->char_no.w ){
		shl1b_00_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( !ew->routine_1 ){
		if( HI_BYTE( ew->master_w->cg_rival ) < 0 ) ew->routine_1 ++ ;
		else {
			while( HI_BYTE( ew->cg_rival ) != HI_BYTE( ew->master_w->cg_rival ) ) pl_char_move( ew ) ;
			return ;
		}
	}
	pl_char_move( ew ) ;
}

static void shl1b_00_ball_init( PLAYER_WORK *ew );
static void shl1b_00_ball_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_ball( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_ball_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_ball_init, shl1b_00_ball_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_ball_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1b_00_ball_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->prio = 0 ;
	lw->char_no = pl_w->char_no.w ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == pl1b_sp_snapback ){
		ew->atck_no = pl1b_at_snapback ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = ew->atck_level ? 89 : 87 ;
		HIT_RESET( ew ) ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pl_char_set( ew, 20, 8 ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_ball_move( PLAYER_WORK *ew )
{
	if( lw->char_no != ew->master_w->char_no.w ){
		shl1b_00_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->atck_level ){
		ew->hit_box_no = 0 ;
		if( ew->hit_flag && !( ew->hit_flag & HR_GUARD ) && ew->cg_flag1 ){
			ew->atck_no = ew->cg_flag1 ;
			HIT_RESET( ew ) ;
			ew->cg_flag1 = 0 ;
		}
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->master_w->routine_3 = 2 ;
		return ;
	}
	if( !ew->hit_box_no ) hit_work_set( ew ) ;
}

static void shl1b_00_kick_init( PLAYER_WORK *ew );
static void shl1b_00_kick_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_kick( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_kick_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_kick_init, shl1b_00_kick_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_kick_jmp[ew->routine_0]( ew ) ;
}

/* the kick picture's animation (kind 4), by the move that set it */
static int shl1b_00_kick_char[4] = { 4, 5, 6, 7 } ;

/*==============================================================*/
static void shl1b_00_kick_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
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
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->prio = 0 ;
	lw->char_no = ew->master_w->char_no.w ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pl_char_set( ew, Char_Effect, shl1b_00_kick_char[ew->sub_type] ) ;
	shl1b_00_kick_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_kick_move( PLAYER_WORK *ew )
{
	Sint8 no ;

	if( lw->char_no != ew->master_w->char_no.w ){
		shl1b_00_end( ew ) ;
		return ;
	}
	if( ( no = HI_BYTE( ew->master_w->cg_rival ) & 0xfe ) < 0 ){
		shl1b_00_end( ew ) ;
		return ;
	}
	if( !no ){
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	while( HI_BYTE( ew->cg_rival ) != no ) pl_char_move( ew ) ;
}

static void shl1b_00_shichisei_init( PLAYER_WORK *ew );
static void shl1b_00_shichisei_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_shichisei( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_shichisei_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_shichisei_init, shl1b_00_shichisei_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_shichisei_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1b_00_shichisei_init( PLAYER_WORK *ew )
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
	ew->timer0 = 10 ;
	ew->timer1 = 16 ;
	shl1b_00_shichisei_move( ew ) ;
}

static void shl1b_00_shichisei_wait( PLAYER_WORK *ew );
static void shl1b_00_shichisei_count( PLAYER_WORK *ew );
static void shl1b_00_shichisei_hold( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl1b_00_shichisei_move( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_shichisei_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_shichisei_wait, shl1b_00_shichisei_count, shl1b_00_shichisei_hold,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->timer0 ) ew->timer0 -- ;
	if( pl_w->routine_1 || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != pl1b_sp_shichisei || pl_w->team_no ){
		shl1b_00_end( ew ) ;
		return ;
	}
	shl1b_00_shichisei_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl1b_00_shichisei_wait( PLAYER_WORK *ew )
{
	ew->timer0 = 2 ;
	if( -- ew->timer1 < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 360 ;
		ew->timer1 = 138 ;
		shl1b_00_set2( ew, 10, nlRand() & 7 ? 7 : 8 ) ;
		shl1b_00_set2( ew, 9, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl1b_00_shichisei_count( PLAYER_WORK *ew )
{
	ew->timer0 = 2 ;
	if( !-- ew->timer1 ){
		ew->routine_1 ++ ;
		ew->timer0 = 64 ;
	ew->timer1 = 64 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1b_00_shichisei_hold( PLAYER_WORK *ew )
{
	PLAYER_WORK *em = ew->master_w->catch_w ;

	ew->timer0 = 2 ;
	if( em->routine_1 != PL_CAUGHT || em->catch_type || -- ew->timer1 < 0 ){
		ew->timer0 = 0 ;
		shl1b_00_end( ew ) ;
	}
}

static void shl1b_00_9_init( PLAYER_WORK *ew );
static void shl1b_00_9_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_9( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_9_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_9_init, shl1b_00_9_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_9_jmp[ew->routine_0]( ew ) ;
}

/* kind 9's five pictures: the frames before each shows, its animation and its place */
static Uint8 shl1b_00_9_time[5] = { 0x24, 0x2e, 0x4a, 0x54, 0x62 } ;
static Sint16 shl1b_00_9_char[5] = { 9, 10, 11, 12, 13 } ;
static VEC2 shl1b_00_9_pos[5] = {
	{ 640.0f, DOT_Y( 80.0f ) }, { 640.0f, DOT_Y( 160.0f ) }, { DOT_X( 128.0f ), DOT_Y( 64.0f ) },
	{ DOT_X( 128.0f ), DOT_Y( 128.0f ) }, { DOT_X( 128.0f ), DOT_Y( 192.0f ) },
} ;

/*==============================================================*/
static void shl1b_00_9_init( PLAYER_WORK *ew )
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
	ew->disp_dir = 0 ;
	ew->prio = 11 ;
	ew->timer0 = shl1b_00_9_time[ew->sub_type] ;
	ew->pos.x = scr_work.eye[0].x + shl1b_00_9_pos[ew->sub_type].x + -DOT_X( 256.0f ) ;
	ew->pos.y = scr_work.center_y - shl1b_00_9_pos[ew->sub_type].y + DOT_Y( 128.0f ) ;
	ew->char_no.b.no = shl1b_00_9_char[ew->sub_type] ;
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	shl1b_00_9_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_9_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->ef_w->timer0 || pl_w->team_no ){
		shl1b_00_end( ew ) ;
		return ;
	}
	if( !ew->routine_1 && !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
	}
}

static void shl1b_00_10_init( PLAYER_WORK *ew );
static void shl1b_00_10_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_10( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_10_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_10_init, shl1b_00_10_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_10_jmp[ew->routine_0]( ew ) ;
}

/* kind 10's eight pictures: the frames before each shows, its animation and its place */
typedef struct SHL1B_00_PIC {
	Uint8	time;
	Uint8	char_no;
} SHL1B_00_PIC;

static SHL1B_00_PIC shl1b_00_10_tbl[8] = {
	{ 0x11, 0x10 }, { 0x13, 0x10 }, { 0x15, 0x10 }, { 0x17, 0x0f },
	{ 0x16, 0x10 }, { 0x14, 0x10 }, { 0x12, 0x0f }, { 0x18, 0x0e },
} ;
static VEC2 shl1b_00_10_pos[8] = {
	{ 640.0f, DOT_Y( 32.0f ) }, { DOT_X( 416.0f ), DOT_Y( 92.0f ) }, { DOT_X( 340.0f ), DOT_Y( 144.0f ) },
	{ DOT_X( 280.0f ), DOT_Y( 104.0f ) }, { 340.0f, DOT_Y( 146.0f ) }, { 260.0f, DOT_Y( 164.0f ) },
	{ 160.0f, 480.0f }, { DOT_X( 140.0f ), DOT_Y( 158.0f ) },
} ;

/*==============================================================*/
static void shl1b_00_10_init( PLAYER_WORK *ew )
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
	ew->disp_dir = 0 ;
	ew->prio = 12 ;
	ew->timer0 = shl1b_00_10_tbl[ew->sub_type].time ;
	ew->char_no.b.no = shl1b_00_10_tbl[ew->sub_type].char_no ;
	ew->pos.x = scr_work.eye[0].x + shl1b_00_10_pos[ew->sub_type].x + -DOT_X( 256.0f ) ;
	ew->pos.y = scr_work.center_y - shl1b_00_10_pos[ew->sub_type].y + DOT_Y( 128.0f ) ;
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_10_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 != PL_NORM || pl_w->team_no ){
		shl1b_00_end( ew ) ;
		return ;
	}
	if( !ew->ef_w->timer0 ){
		shl1b_00_end( ew ) ;
		return ;
	}
	if( !ew->routine_1 ){
		if( -- ew->timer0 > 0 ) return ;
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
	}
	pl_char_move( ew ) ;
}

static void shl1b_00_11_init( PLAYER_WORK *ew );
static void shl1b_00_11_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1b_00_11( PLAYER_WORK *ew )
{
	static void ( *shl1b_00_11_jmp[] )( PLAYER_WORK *ew ) = {
		shl1b_00_11_init, shl1b_00_11_move, shl1b_00_end, shl1b_00_free,
	} ;

	shl1b_00_11_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1b_00_11_init( PLAYER_WORK *ew )
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
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	lw->char_no = ew->master_w->char_no.w ;
	pl_char_set( ew, 20, 11 ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_11_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( lw->char_no != pl_w->char_no.w ){
		shl1b_00_end( ew ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( pl_lw->kikoushou_shot_flag ){
		if( 0 ) pl_char_move( pl_w ) ;
		else pl_char_move( ew ) ;
		return ;
	}
	shl1b_00_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1b_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1b_00.c" End of File	*/
