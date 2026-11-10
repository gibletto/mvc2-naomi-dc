/* M. Bison's effect 00 (character 26's shot 00): the Psycho Shot (type 0, from sp_shot), the Psycho
   Field (type 1, from sp_field: three kinds, a ring of sparks that turns and two that follow) and
   the Psycho Explosion's flame (type 2, from sp_explosion). */
#include "player.h"
#include "pl_char.h"
#include "pl26_def.h"

/* the effect's own work */
typedef struct SHL26_00_LOCAL_W {
	Uint8	time;		/* 000: the psycho shot's frames before it may leave the screen */
	Sint8	hit_cnt;	/* 001: the field's hits left */
	Sint8	turn_timer;	/* 002: frames to the field's next step round its circle */
} SHL26_00_LOCAL_W;
#define SHL26_00_LW( w )	( ( SHL26_00_LOCAL_W * )&( w )->obj_flag )

/* the psycho shot's speeds, by strength */
typedef struct SHL26_00_SPD {
	Float	spd_x, acc_x, spd_y, acc_y;
} SHL26_00_SPD;

static SHL26_00_LOCAL_W *lw;	/* the running work's */
static PL26_LOCAL_W *pl_lw;	/* M. Bison's */

static void shl26_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl26_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl26_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x2600 ;
	}
	return ew ;
}

/****************************************************************/
void *shl26_00_field_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl26_00_main ;
		ew->master_w = pl_w ;
		ew->type = 1 ;
		ew->sub_type = no ;
		ew->ef_no = 0x2600 ;
	}
	return ew ;
}

static void shl26_00_shot_main( PLAYER_WORK *ew );
static void shl26_00_field_main( PLAYER_WORK *ew );
static void shl26_00_explosion_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_main( PLAYER_WORK *ew )
{
	static void ( *shl26_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_shot_main, shl26_00_field_main, shl26_00_explosion_main,
	} ;

	pl_lw = ( PL26_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = SHL26_00_LW( ew ) ;
	shl26_00_jmp[ew->type]( ew ) ;
}

static void shl26_00_shot_init( PLAYER_WORK *ew );
static void shl26_00_shot_move( PLAYER_WORK *ew );
static void shl26_00_shot_end( PLAYER_WORK *ew );
static void shl26_00_shot_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_shot_main( PLAYER_WORK *ew )
{
	static void ( *shl26_00_shot_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_shot_init, shl26_00_shot_move, shl26_00_shot_end, shl26_00_shot_free,
	} ;

	shl26_00_shot_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl26_00_shot_init( PLAYER_WORK *ew )
{
	static SHL26_00_SPD shl26_00_spd[] = {
		{ DOT_X( 0.25f ), DOT_X( 0.375f ), DOT_Y( 6.0f ), -0.6696428f },
		{ DOT_X( 0.25f ), DOT_X( 0.375f ), -DOT_Y( 5.0f ), DOT_Y( 0.375f ) },
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
	lw->time = 16 ;
	pl_lw->shot_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
	ew->atck_no = ew->atck_level ? 56 : 54 ;
	HIT_RESET( ew ) ;
	ew->spd.x = ew->master_w->pl_dir ? shl26_00_spd[ew->atck_level].spd_x : -shl26_00_spd[ew->atck_level].spd_x ;
	ew->acc.x = ew->master_w->pl_dir ? shl26_00_spd[ew->atck_level].acc_x : -shl26_00_spd[ew->atck_level].acc_x ;
	ew->spd.y = shl26_00_spd[ew->atck_level].spd_y ;
	ew->acc.y = shl26_00_spd[ew->atck_level].acc_y ;
	ew->pos.x += DOT_X( 16.0f ) ;
	ew->pos.y += DOT_Y( 72.0f ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_back = ew->size_front = 64 ;
	ew->size_bottom = ew->size_top = 64 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_shot_move( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( !lw->time || ! -- lw->time ){
		if( !scr_in_ck( ew ) ){
			shl26_00_shot_free( ew ) ;
			return ;
		}
	}
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		pl_lw->shot_flag = 0 ;
		pl_char_set( ew, Char_Effect, 1 ) ;
		shl26_00_shot_end( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_shot_end( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		shl26_00_shot_free( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl26_00_shot_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	if( ew->pl_type == ew->master_w->pl_type ) pl_lw->shot_flag = 0 ;
	work_free( ew ) ;
}

static void shl26_00_field_head_main( PLAYER_WORK *ew );
static void shl26_00_field_spark_main( PLAYER_WORK *ew );
static void shl26_00_field_spark2_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_field_main( PLAYER_WORK *ew )
{
	static void ( *shl26_00_field_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_field_head_main, shl26_00_field_spark_main, shl26_00_field_spark2_main,
	} ;

	shl26_00_field_jmp[ew->sub_type]( ew ) ;
}

static void shl26_00_field_head_init( PLAYER_WORK *ew );
static void shl26_00_field_head_move( PLAYER_WORK *ew );
static void shl26_00_field_head_end( PLAYER_WORK *ew );
static void shl26_00_field_head_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_field_head_main( PLAYER_WORK *ew )
{
	static void ( *shl26_00_field_head_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_field_head_init, shl26_00_field_head_move, shl26_00_field_head_end, shl26_00_field_head_free,
	} ;

	shl26_00_field_head_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The first kind: the head of the field, which		*/
/*			sets the other two									*/
/*==============================================================*/
static void shl26_00_field_head_init( PLAYER_WORK *ew )
{
	Float ofs ;

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
	pl_lw->field_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
	ew->pos.y += ( ew->master_w->pl_stance == STANCE_AIR ) ? DOT_Y( 108.0f ) : 180.0f ;
	ofs = ( ew->master_w->pl_stance == STANCE_AIR ) ? DOT_X( 68.0f ) : 120.0f ;
	ew->pos.x += ew->master_w->pl_dir ? ofs : -ofs ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->master_w->pl_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->timer0 = 20 ;
	lw->hit_cnt = 6 ;
	pl_char_set( ew, Char_Effect, 2 ) ;
}

static void shl26_00_field_head_wait( PLAYER_WORK *ew );
static void shl26_00_field_head_fly( PLAYER_WORK *ew );
static void shl26_00_field_head_swirl( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl26_00_field_head_move( PLAYER_WORK *ew )
{
	static void ( *shl26_00_field_head_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_field_head_wait, shl26_00_field_head_fly, shl26_00_field_head_swirl,
	} ;

	shl26_00_field_head_move_jmp[ew->routine_1]( ew ) ;
}

static void shl26_00_field_head_stop( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_field_head_wait( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	pl_char_move( ew ) ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->timer0 = ew->atck_level ? 80 : 20 ;
		ew->atck_no = pl26_at_field ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_field_head_fly( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( ew->master_w->routine_1 != PL_NORM ) shl26_00_field_head_stop( ew ) ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		pl_voice_req( ew, 32 ) ;
		shl26_00_field_set( ew, 1 ) ;
		shl26_00_field_set( ew, 2 ) ;
		ew->timer0 = 50 ;
		ew->dir = 0 ;
		lw->turn_timer = 2 ;
		ew->timer1 = 2 ;
		pl_char_set( ew, Char_Effect, 3 ) ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			The head turns on a circle; each hit takes one		*/
/*			of its six lives, and the third shot it lets go		*/
/*==============================================================*/
static void shl26_00_field_head_swirl( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( ew->master_w->routine_1 != PL_NORM ) shl26_00_field_head_stop( ew ) ;
	if( -- lw->turn_timer < 0 ){
		lw->turn_timer = 3 ;
		ew->dir = ++ ew->dir & 31 ;
	}
	ew->spd.x = nlCos( ( ( 40 - ew->dir ) & 31 ) << 11 ) * 1.6666666f ;
	ew->spd.y = nlSin( ( ( 40 - ew->dir ) & 31 ) << 11 ) * 2.142857f ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ! -- ew->timer0 ) shl26_00_field_head_stop( ew ) ;
	if( ew->hit_flag ){
		if( -- lw->hit_cnt < 0 ){
			if( -- ew->timer1 < 0 ) shl26_00_field_head_stop( ew ) ;
			lw->hit_cnt = 6 ;
			ew->atck_no = pl26_at_field_hit ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_field_head_end( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( pl_char_move( ew ) < 0 ) shl26_00_field_head_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_field_head_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
static void shl26_00_field_head_stop( PLAYER_WORK *ew )
{
	ew->routine_0 = 2 ;
	ew->routine_1 = 0 ;
	pl_lw->field_flag = 0 ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

static void shl26_00_field_spark_init( PLAYER_WORK *ew );
static void shl26_00_field_spark_move( PLAYER_WORK *ew );
static void shl26_00_field_spark_end( PLAYER_WORK *ew );
static void shl26_00_field_spark_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_field_spark_main( PLAYER_WORK *ew )
{
	static void ( *shl26_00_field_spark_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_field_spark_init, shl26_00_field_spark_move, shl26_00_field_spark_end, shl26_00_field_spark_free,
	} ;

	shl26_00_field_spark_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The second kind, a spark that follows the head's	*/
/*			turn												*/
/*==============================================================*/
static void shl26_00_field_spark_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->col_no = ew->master_w->col_no ;
	ew->col_flag = ew->master_w->col_flag ;
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->dir = 10 ;
	ew->timer0 = 50 ;
	ew->timer1 = 2 ;
	lw->turn_timer = 2 ;
	lw->hit_cnt = 6 ;
	ew->atck_no = pl26_at_field ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

static void shl26_00_field_spark_stop( PLAYER_WORK *ew );

/*==============================================================*/
/*			The sparks of kinds 1 and 2 turn as the head		*/
/*			does, and end with it (a hit ends in the head's		*/
/*			stop)												*/
/*==============================================================*/
static void shl26_00_field_spark_move( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	ew->col_no = ew->master_w->col_no ;
	ew->col_flag = ew->master_w->col_flag ;
	if( ew->master_w->routine_0 == 2 ) shl26_00_field_spark_stop( ew ) ;
	if( -- lw->turn_timer < 0 ){
		lw->turn_timer = 3 ;
		ew->dir = ++ ew->dir & 31 ;
	}
	ew->spd.x = nlCos( ( ( 40 - ew->dir ) & 31 ) << 11 ) * 1.6666666f ;
	ew->spd.y = nlSin( ( ( 40 - ew->dir ) & 31 ) << 11 ) * 2.142857f ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ! -- ew->timer0 ){
		shl26_00_field_spark_stop( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		if( -- lw->hit_cnt < 0 ){
			if( -- ew->timer1 < 0 ){
				shl26_00_field_head_stop( ew ) ;
				return ;
			}
			lw->hit_cnt = 6 ;
			ew->atck_no = pl26_at_field_hit ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_field_spark_end( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( pl_char_move( ew ) < 0 ) shl26_00_field_spark_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_field_spark_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
static void shl26_00_field_spark_stop( PLAYER_WORK *ew )
{
	ew->routine_0 = 2 ;
	ew->routine_1 = 0 ;
	pl_lw->field_flag = 0 ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

static void shl26_00_field_spark2_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_field_spark2_main( PLAYER_WORK *ew )
{
	static void ( *shl26_00_field_spark2_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_field_spark2_init, shl26_00_field_spark_move, shl26_00_field_spark_end, shl26_00_field_spark_free,
	} ;

	shl26_00_field_spark2_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The third kind: the same spark, starting a			*/
/*			quarter of the way round							*/
/*==============================================================*/
static void shl26_00_field_spark2_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->dir = 20 ;
	ew->timer0 = 50 ;
	ew->timer1 = 2 ;
	lw->turn_timer = 2 ;
	lw->hit_cnt = 6 ;
	ew->atck_no = pl26_at_field ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

static void shl26_00_explosion_init( PLAYER_WORK *ew );
static void shl26_00_explosion_move( PLAYER_WORK *ew );
static void shl26_00_explosion_end( PLAYER_WORK *ew );
static void shl26_00_explosion_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl26_00_explosion_main( PLAYER_WORK *ew )
{
	static void ( *shl26_00_explosion_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_explosion_init, shl26_00_explosion_move, shl26_00_explosion_end, shl26_00_explosion_free,
	} ;

	shl26_00_explosion_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl26_00_explosion_init( PLAYER_WORK *ew )
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += DOT_X( 16.0f ) ;
	ew->pos.y += DOT_Y( 72.0f ) ;
	ew->prio = 0 ;
	ew->sub_prio ++ ;
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_front = ew->size_back = 56 ;
	ew->scale.x = 1.25f ;
	ew->scale.y = 1.25f ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	ew->atck_no = pl26_at_explosion ;
	HIT_RESET( ew ) ;
	ew->atck_attr = ATR_BACK_POS ;
	pl_char_set( ew, Char_Effect, 17 ) ;
}

static void shl26_00_explosion_fly( PLAYER_WORK *ew );
static void shl26_00_explosion_hit( PLAYER_WORK *ew );
static void shl26_00_explosion_vanish( PLAYER_WORK *ew );

/*==============================================================*/
/*			The flame lasts while M. Bison is in the			*/
/*			animation group Char_Super							*/
/*==============================================================*/
static void shl26_00_explosion_move( PLAYER_WORK *ew )
{
	static void ( *shl26_00_explosion_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl26_00_explosion_fly, shl26_00_explosion_hit, shl26_00_explosion_vanish,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp != Char_Super ){
		shl26_00_explosion_end( ew ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl26_00_explosion_end( ew ) ;
		return ;
	}
	shl26_00_explosion_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl26_00_explosion_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *em_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->hit_flag ){
		ew->atck_no = pl26_at_explosion_2 ;
		HIT_RESET( ew ) ;
		ew->atck_attr = ATR_BACK_POS ;
		em_w = ew->em_w ;
		if( em_w->work_kind ) return ;
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 18 ) ;
		ew->timer0 = 14 ;
		ew->timer1 = 0 ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Holds the enemy, hitting every eighth frame,		*/
/*			until the time is out								*/
/*==============================================================*/
static void shl26_00_explosion_hit( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ! ( ++ ew->timer1 & 7 ) ){
		ew->timer1 = 0 ;
		if( -- ew->timer0 <= 0 ){
			ew->routine_1 ++ ;
			ew->rot[2] = 0 ;
			pl_char_set( ew, Char_Effect, 19 ) ;
		} else if( ew->timer0 > 1 ){
			if( ew->timer0 & 1 ){
				ew->atck_no = pl26_at_explosion ;
				HIT_RESET( ew ) ;
				ew->atck_attr = ATR_BACK_POS ;
			} else {
				ew->atck_no = pl26_at_explosion_2 ;
				HIT_RESET( ew ) ;
				ew->atck_attr = 0 ;
			}
		} else {
			ew->atck_no = pl26_at_explosion_3 ;
			HIT_RESET( ew ) ;
			ew->atck_attr = 0 ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_explosion_vanish( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl26_00_explosion_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	pl_lw->ef_end_flag = 1 ;
	shl26_00_explosion_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl26_00_explosion_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl26_00.c" End of File	*/
