/* Sakura's effect 00 (character 22's shot 00): the shots and sparks of her special moves, by type.
   Types 0, 1, 6 and 7 are the fireballs, 2 and 3 fall and bounce, 4 follows her, 5 flies at random. */
#include "player.h"
#include "pl_char.h"
#include "pl22_def.h"

/* frames a shot lasts, by its sort and her attack level */
static Uint16 shl22_00_time_tbl[] = { 10, 30, 30, 50 } ;

/* the speeds a shot starts with: spd.x, acc.x, spd.y, acc.y */
static Float shl22_00_hadou_spd_tbl[][4] = {
	{ -1.6666666f, 0.104166664f, 2.142857f, -0.13392857f },
	{ -DOT_X( 4.0f ), 0.052083332f, DOT_Y( 4.0f ), -0.06696428f },
} ;

static Float shl22_00_jhadou_spd_tbl[][4] = {
	{ -1.6666666f, 0.104166664f, -2.142857f, -0.13392857f },
	{ -DOT_X( 5.0f ), 0.052083332f, -DOT_Y( 4.0f ), -0.06696428f },
} ;

static Float shl22_00_fire_spd_tbl[][4] = {
	{ -DOT_X( 4.0f ), 0, 0, 0 },
	{ -10.0f, 0, 0, 0 },
	{ -DOT_X( 4.0f ), 0, -DOT_Y( 2.0f ), 0 },
	{ -10.0f, 0, -DOT_Y( 3.0f ), 0 },
} ;

#define SHL22_00_FLY( w )	\
	( w )->pos.x += ( w )->spd.x ;	\
	( w )->spd.x += ( w )->acc.x ;	\
	( w )->pos.y += ( w )->spd.y ;	\
	( w )->spd.y += ( w )->acc.y

static void shl22_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl22_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl22_00_main ;
		ew->master_w = pl_w ;
		ew->obj_flag = pl_w->char_no.w ;
		ew->type = no ;
		ew->ef_no = 0x2200 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl22_00_set_ex( PLAYER_WORK *pl_w, Uint8 no, Uint8 arg )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl22_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = arg ;
		ew->ef_no = 0x2200 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl22_00_set2( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL_ST, LIST_TOP ) ) ){
		ew->work_func = shl22_00_main ;
		ew->master_w = pl_w ;
		ew->obj_flag = pl_w->char_no.w ;
		ew->type = no ;
		ew->ef_no = 0x2200 ;
	}
	return ew ;
}

static void shl22_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl22_00_main( PLAYER_WORK *ew )
{
	static void ( *shl22_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl22_00_init, shl22_00_move, shl22_00_end, shl22_00_free,
	} ;

	shl22_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void shl22_00_init_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_init_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_init_bounce( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_init_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_init_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl22_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl22_00_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl22_00_init_shot, shl22_00_init_shot, shl22_00_init_fall, shl22_00_init_bounce,
		shl22_00_init_follow, shl22_00_init_spark, shl22_00_init_shot, shl22_00_init_shot,
	} ;

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
	shl22_00_init_jmp[ew->type]( ew, pl_w ) ;
	shl22_00_move( ew, pl_w ) ;
}

/*==============================================================*/
static void shl22_00_init_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 shl22_00_char_tbl[] = { 6, 8, 28, 30 } ;
	Float *spd ;
	Uint8 no ;

	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->sub_prio = -1 ;
	ew->atck_flag = HIT_SHOT ;
	ew->dm_flag = 0 ;
	spd = shl22_00_fire_spd_tbl[0] ;
	switch( ew->type ){
	case 0:
		no = 0 ;
		spd = shl22_00_hadou_spd_tbl[ew->atck_level] ;
		ew->atck_no = ( ( Uint32 )ew->atck_level * 2 ) + 68 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		break ;
	case 1:
		spd = shl22_00_jhadou_spd_tbl[ew->atck_level] ;
		no = 1 ;
		ew->atck_no = ( ( Uint32 )ew->atck_level * 2 ) + 78 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		break ;
	case 6:
		spd = &spd[ew->atck_level * 4] ;
		if( !ew->disp_dir ) ew->pos.x -= 130.0f ;
		else ew->pos.x += 130.0f ;
		no = 2 ;
		ew->pos.y += DOT_Y( 55.0f ) ;
		ew->atck_no = ew->atck_level + 109 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		break ;
	case 7:
		spd += ew->atck_level * 4 ;
		spd += 8 ;
		if( !ew->disp_dir ) ew->pos.x -= 115.0f ;
		else ew->pos.x += 115.0f ;
		no = 3 ;
		ew->pos.y += DOT_Y( 39.0f ) ;
		ew->atck_no = ew->atck_level + 111 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		break ;
	}
	ew->spd.x = *spd ++ ;
	ew->acc.x = *spd ++ ;
	ew->spd.y = *spd ++ ;
	ew->acc.y = *spd ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->timer0 = shl22_00_time_tbl[( no & 2 ) + ew->atck_level] ;
	pl_char_set( ew, Char_Effect, shl22_00_char_tbl[no] ) ;
}

/*==============================================================*/
static void shl22_00_init_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->spd.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->pos.y += DOT_Y( 94.0f ) ;
	ew->spd.x = 2.5f ;
	ew->spd.y = DOT_Y( 3.0f ) ;
	ew->acc.y = -DOT_Y( 0.125f ) ;
	if( pl_w->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->pos.x += DOT_X( 71.0f ) ;
	}
	else ew->pos.x += -DOT_X( 71.0f ) ;
	pl_char_set( ew, 19, 6 ) ;
}

/*==============================================================*/
static void shl22_00_init_bounce( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->atck_flag = HIT_ON ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl22_at_win_ball ;
	HIT_RESET( ew ) ;
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->spd.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->pos.y += DOT_Y( 94.0f ) ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	if( pl_w->disp_dir ) ew->pos.x += DOT_X( 71.0f ) ;
	else ew->pos.x += -DOT_X( 71.0f ) ;
	ew->spd.x = ( pl_w->target_w->pos.x - pl_w->pos.x ) / 64.0f ;
	pl_char_set( ew, 19, 6 ) ;
}

/*==============================================================*/
static void shl22_00_init_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->timer1 = 10 ;
	ew->timer0 = 2 ;
	ew->sub_prio = -8 ;
	pl_char_set( ew, Char_Effect, 10 ) ;
}

/*==============================================================*/
static void shl22_00_init_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;
	Sint32 n ;

	ew->sub_prio = -1 ;
	ew->disp_flag = 1 ;
	ew->spd.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	x = DOT_X( 92.0f ) ;
	ew->spd.x = -DOT_X( 8.0f ) ;
	ew->spd.y = DOT_X( 8.0f ) ;
	n = Rnd() & 63 ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		n = -n ;
		x = -DOT_X( 92.0f ) ;
	}
	ew->pos.x = pl_w->pos.x - x + 1.6666666f * n ;
	n = Rnd() & 63 ;
	ew->pos.y = pl_w->pos.y + 2.142857f * ( 132 - n ) ;
	ew->atck_flag = HIT_SHOT ;
	ew->dm_flag = 0 ;
	if( ew->sub_type == 1 ){
		ew->atck_no = pl22_at_shinkuu_last ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	} else {
		ew->atck_no = pl22_at_shinkuu ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	}
	pl_voice_req( ew, 30 ) ;
	pl_char_set( ew, Char_Effect, 11 ) ;
}

static void shl22_00_move_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_move_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_move_bounce( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_move_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_move_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_move_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl22_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl22_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl22_00_move_shot, shl22_00_move_shot, shl22_00_move_fall, shl22_00_move_bounce,
		shl22_00_move_follow, shl22_00_move_spark, shl22_00_move_fire, shl22_00_move_fire,
	} ;

	shl22_00_move_jmp[ew->type]( ew, pl_w ) ;
}

static void shl22_00_shot_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl22_00_move_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl22_00_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl22_00_shot_start, shl22_00_shot_fly, shl22_00_shot_end,
	} ;

	ew->prio = pl_w->prio ;
	shl22_00_shot_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl22_00_shot_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->routine_1 ++ ;
		if( !ew->type ){
			x = DOT_X( 80.0f ) ;
			if( ew->disp_dir ) x = -DOT_X( 80.0f ) ;
			ew->pos.y += DOT_Y( 124.0f ) ;
			ew->pos.x -= x ;
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl22_00_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL22_00_FLY( ew ) ;
	pl_char_move( ew ) ;
	if( ew->type && pl_w->ground_y > ew->pos.y ) goto change ;
	if( ew->hit_flag || ew->hit_box_no || !--ew->timer0 ){
change :
		ew->routine_1 ++ ;
		ew->spd.x /= 4.0f ;
		ew->acc.x /= 4.0f ;
		ew->spd.y /= 4.0f ;
		ew->acc.y /= 4.0f ;
		if( !ew->type ) pl_char_set( ew, Char_Effect, 7 ) ;
		else pl_char_set( ew, Char_Effect, 9 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl22_00_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL22_00_FLY( ew ) ;
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
}

static void shl22_00_fall_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl22_00_move_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl22_00_fall_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl22_00_fall_start, shl22_00_land, shl22_00_anim,
	} ;

	if( ew->obj_flag != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	shl22_00_fall_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl22_00_fall_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	SHL22_00_FLY( ew ) ;
	if( pl_w->ground_y + DOT_Y( 85.0f ) > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y + DOT_Y( 85.0f ) ;
		ew->spd.x /= 2.0f ;
		ew->spd.y = 2.142857f ;
	}
}

/*--------------------------------------------------------------*/
static void shl22_00_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	SHL22_00_FLY( ew ) ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		pl_char_set( ew, 19, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl22_00_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

static void shl22_00_bounce_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl22_00_move_bounce( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl22_00_bounce_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl22_00_bounce_start, shl22_00_land, shl22_00_anim,
	} ;

	if( ew->obj_flag != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	shl22_00_bounce_jmp[ew->routine_1]( ew, pl_w ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl22_00_bounce_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	SHL22_00_FLY( ew ) ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->spd.x /= 2.0f ;
		ew->spd.y = 2.142857f ;
	}
}

/*==============================================================*/
static void shl22_00_move_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	if( ew->obj_flag != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		return ;
	}
	if( ew->routine_1 == 1 ){
		pl_char_move( ew ) ;
		if( !( vs_sys_work.game_stop & ( 1 << pl_w->pl_no ) ) ){
			ew->routine_1 ++ ;
			pl_char_set( ew, Char_Effect, 27 ) ;
		}
		return ;
	}
	else {
		if( pl_char_move( ew ) < 0 && --ew->timer1 <= 0 ){
			pl_w->routine_2 ++ ;
			work_free( ew ) ;
			return ;
		}
	}
	if( ew->cg_type & 1 ){
		ew->cg_type &= 0xfe ;
		shl22_00_set_ex( pl_w, 5, ew->timer1 ) ;
	}
	if( ew->cg_type & 2 ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
}

/*==============================================================*/
static void shl22_00_move_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	if( !ew->routine_1 ){
		pl_char_move( ew ) ;
		SHL22_00_FLY( ew ) ;
		if( scr_in_x_ck( ew ) ){
			if( ew->hit_flag ){
				ew->routine_1 ++ ;
				pl_char_set( ew, Char_Effect, 12 ) ;
			}
			hit_work_set( ew ) ;
			return ;
		}
	}
	else if( pl_char_move( ew ) >= 0 ) return ;
	shl22_00_end( ew, pl_w ) ;
}

static void shl22_00_fire_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl22_00_fire_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl22_00_move_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl22_00_fire_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl22_00_fire_fly, shl22_00_fire_end,
	} ;

	ew->prio = pl_w->prio ;
	shl22_00_fire_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl22_00_fire_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL22_00_FLY( ew ) ;
	pl_char_move( ew ) ;
	if( ew->type == 7 && pl_w->ground_y > ew->pos.y ) goto change ;
	if( ew->hit_flag || ew->hit_box_no || !--ew->timer0 ){
change :
		ew->routine_1 ++ ;
		ew->spd.x /= 4.0f ;
		ew->acc.x /= 4.0f ;
		ew->spd.y /= 4.0f ;
		ew->acc.y /= 4.0f ;
		if( ew->type == 6 ) pl_char_set( ew, Char_Effect, 29 ) ;
		else pl_char_set( ew, Char_Effect, 31 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl22_00_fire_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL22_00_FLY( ew ) ;
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl22_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl22_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl22_00.c" End of File	*/
