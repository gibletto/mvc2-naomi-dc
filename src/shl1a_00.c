/* Abyss's third form's effects (character 1a's shot 00): nine kinds, picked by the setter, each
   with its own routines. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl1a_def.h"

/* the effect's own work */
typedef struct SHL1A_00_LOCAL_W {
	Float	x;	/* 000: where the spray's shot started */
} SHL1A_00_LOCAL_W;
#define SHL1A_00_LW( w )	( ( SHL1A_00_LOCAL_W * )&( w )->obj_flag )

static void shl1a_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1a_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1a_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x1a00 ;
		ew->timer1 = pl_w->char_no.w ;
	}
	return ew ;
}

/****************************************************************/
/*			A part that rides on the effect hw					*/
/****************************************************************/
void *shl1a_00_set2( PLAYER_WORK *hw, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl1a_00_main ;
		ew->master_w = hw->master_w ;
		ew->ef_w = hw ;
		ew->type = type ;
		ew->ef_no = 0x1a00 ;
		ew->timer1 = hw->timer1 ;
	}
	return ew ;
}

/****************************************************************/
/*			An effect set at a point							*/
/****************************************************************/
void *shl1a_00_pos_set( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1a_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->pos = *pos ;
		ew->ef_no = 0x1a00 ;
		ew->timer1 = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl1a_00_storm( PLAYER_WORK *ew );
static void shl1a_00_spot( PLAYER_WORK *ew );
static void shl1a_00_up( PLAYER_WORK *ew );
static void shl1a_00_wave( PLAYER_WORK *ew );
static void shl1a_00_wave_part( PLAYER_WORK *ew );
static void shl1a_00_spray( PLAYER_WORK *ew );
static void shl1a_00_shot( PLAYER_WORK *ew );
static void shl1a_00_blast( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_main( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_storm, shl1a_00_spot, shl1a_00_spot, shl1a_00_up, shl1a_00_wave,
		shl1a_00_wave_part, shl1a_00_spray, shl1a_00_shot, shl1a_00_blast,
	} ;

	shl1a_00_jmp[ew->type]( ew ) ;
}

static void shl1a_00_storm_init( PLAYER_WORK *ew );
static void shl1a_00_storm_move( PLAYER_WORK *ew );
static void shl1a_00_end( PLAYER_WORK *ew );
static void shl1a_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_storm( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_storm_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_storm_init, shl1a_00_storm_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_storm_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_00_storm_init( PLAYER_WORK *ew )
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
	ew->atck_no = pl1a_at_spray ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = 0 ;
	ew->dir = 0xff ;
	shl1a_00_storm_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_storm_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	if( pl_w->cg_flag1 == 0xff || ( Uint16 )ew->timer1 != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( !pl_w->cg_flag1 ) return ;
	ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	if( pl_w->cg_flag1 != ew->dir ){
		ew->dir = pl_w->cg_flag1 ;
		pl_char_set2( ew, Char_Effect, 4, ew->dir - 1 ) ;
		if( ew->cg_flag1 ){
			ew->cg_flag1 = 0 ;
			ew->atck_no = ew->cg_flag1 + 56 ;
			HIT_RESET( ew ) ;
		}
	}
	hit_work_set( ew ) ;
}

static void shl1a_00_spot_init( PLAYER_WORK *ew );
static void shl1a_00_spot_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_spot( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_spot_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_spot_init, shl1a_00_spot_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_spot_jmp[ew->routine_0]( ew ) ;
}

/* the turns a spot's piece can take, by Rnd() */
static Uint8 shl1a_00_dir_tbl[16] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
} ;

/*==============================================================*/
static void shl1a_00_spot_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	ew->dir = shl1a_00_dir_tbl[Rnd() & 15] ;
	ew->prio = 0 ;
	ew->pos = pl_w->spd ;
	ew->scale.x *= 0.6f ;
	ew->scale.y *= 0.6f ;
	if( ew->pos.y > pl_w->ground_y ){
		ew->scale.x *= 0.8f ;
		ew->scale.y *= 0.8f ;
		ew->col_flag = 1 ;
		ew->col_no = col_no_tbl[ew->work_no] + 2 ;
	} else {
		ew->scale.x *= 1.2f ;
		ew->scale.y *= 1.2f ;
	}
	ew->rot[2] = ( Uint16 )( ew->dir << 11 ) ;
	ef_char_set( ew, 27, 1 ) ;
	shl1a_00_spot_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_spot_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	if( pl_w->char_no.w != ( Uint16 )ew->timer1 ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->type == 1 && ew->cg_type ){
		ew->cg_type = 0 ;
		shl1a_00_set2( ew, 3 ) ;
	}
	ew->disp_flag = 1 ;
}

static void shl1a_00_up_init( PLAYER_WORK *ew );
static void shl1a_00_up_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_up( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_up_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_up_init, shl1a_00_up_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_up_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_00_up_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Uint8 no ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	CHAR_PART_COPY( ew, ew->before_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->before_w->pl_no ;
	ew->pl_type = ew->before_w->pl_type ;
	ew->scale.x = ew->before_w->scale.x ;
	ew->scale.y = ew->before_w->scale.y ;
	ew->atck_level = ew->before_w->atck_level ;
	ew->work_no = ew->before_w->work_no ;
	ew->col_cnt = ew->before_w->col_cnt ;
	ew->scale = ew->before_w->scale ;
	ew->prio = ew->before_w->prio ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	no = 53 ;
	if( !( Rnd() % 3 ) ) no = 52 ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	if( pl_w->atck_level == 1 ) ew->atck_attr = ATR_BACK_POS ;
	ew->size_top = ew->size_bottom = 128 ;
	ew->pos = ew->before_w->pos ;
	ew->prio = 7 ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 10.0f ) ;
	ew->acc.y = 2.142857f ;
	pl_char_set( ew, Char_Effect, 5 ) ;
	shl1a_00_up_move( ew ) ;
	pl_voice_req( ew, 28 ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_up_move( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_top_ck( ew ) ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->disp_flag = 1 ;
	hit_work_set( ew ) ;
}

static void shl1a_00_wave_init( PLAYER_WORK *ew );
static void shl1a_00_wave_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_wave( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_wave_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_wave_init, shl1a_00_wave_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_wave_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_00_wave_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	ew->atck_no = pl1a_at_wave_shot ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = ew->master_w->prio ;
	ew->sub_prio = -1 ;
	pl_char_set( ew, Char_Effect, 9 ) ;
	if( !ew->sub_type ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_wave_move( PLAYER_WORK *ew )
{
	int i ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = ew->master_w->prio ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		for( i = 0 ; i < 7 ; i ++ ) shl1a_00_set2( ew, 5 ) ;
	}
	if( !ew->sub_type ) hit_work_set( ew ) ;
}

static void shl1a_00_wave_part_init( PLAYER_WORK *ew );
static void shl1a_00_wave_part_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_wave_part( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_wave_part_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_wave_part_init, shl1a_00_wave_part_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_wave_part_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_00_wave_part_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->ef_w->pl_no ;
	ew->pl_type = ew->ef_w->pl_type ;
	ew->scale.x = ew->ef_w->scale.x ;
	ew->scale.y = ew->ef_w->scale.y ;
	ew->atck_level = ew->ef_w->atck_level ;
	ew->work_no = ew->ef_w->work_no ;
	ew->col_cnt = ew->ef_w->col_cnt ;
	ew->scale = ew->ef_w->scale ;
	ew->prio = ew->ef_w->prio ;
	ew->atck_no = pl1a_at_wave_shot ;
	HIT_RESET( ew ) ;
	ew->sub_type = 0xff ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->sub_prio = ew->before_w->sub_prio ;
	shl1a_00_wave_part_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_wave_part_move( PLAYER_WORK *ew )
{
	if( ew->ef_w->routine_0 >= 2 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos = ew->before_w->pos ;
	if( ew->before_w->type == 4 ) ew->pos.y += DOT_Y( 143.0f ) * ew->scale.y ;
	else ew->pos.y += DOT_Y( 31.0f ) * ew->scale.y ;
	if( ew->sub_type != HI_BYTE( ew->ef_w->cg_rival ) ){
		ew->sub_type = HI_BYTE( ew->ef_w->cg_rival ) ;
		pl_char_set2( ew, Char_Effect, 10, ew->sub_type - 1 ) ;
	}
	if( !ew->sub_type ) hit_work_set( ew ) ;
}

static void shl1a_00_spray_init( PLAYER_WORK *ew );
static void shl1a_00_spray_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_spray( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_spray_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_spray_init, shl1a_00_spray_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_spray_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_00_spray_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	int no ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	ew->prio = 0 ;
	ew->scale.x *= 0.5f ;
	ew->scale.y *= 0.5f ;
	ew->pos = pl_w->pos ;
	no = Rnd() % 7 ;
	ew->spd.x = -( no * 8 ) * pl_w->scale.x * 1.6666666f ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->spd.y = ( ew->sub_type % 3 * 32 + 56 ) * pl_w->scale.y * 2.142857f ;
	if( ew->disp_dir ) ew->dir = 4 ;
	else ew->dir = 12 ;
	if( ( Sint8 )ew->sub_type >= 0 ){
		pl_char_set( ew, Char_Effect, 14 ) ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl1a_at_spray_shot ;
		HIT_RESET( ew ) ;
	} else {
		ef_char_set( ew, 27, 3 ) ;
	}
	shl1a_00_spray_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_spray_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->disp_flag = 0 ;
	if( pl_w->char_no.w != ( Uint16 )ew->timer1 && ( Sint8 )ew->sub_type < 0 ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	ew->pos.z = pl_w->pos.z ;
	if( ( Sint8 )ew->sub_type >= 0 ){
		if( pl_char_move( ew ) < 0 && ( Sint8 )ew->sub_type >= 0 ){
			ew->routine_0 ++ ;
			shl1a_00_set2( ew, 7 ) ;
			return ;
		}
		hit_work_set( ew ) ;
	} else if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->disp_flag = 1 ;
}

static void shl1a_00_shot_init( PLAYER_WORK *ew );
static void shl1a_00_shot_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_shot( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_shot_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_shot_init, shl1a_00_shot_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_shot_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_00_shot_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float ofs ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	CHAR_PART_COPY( ew, ew->before_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->before_w->pl_no ;
	ew->pl_type = ew->before_w->pl_type ;
	ew->scale.x = ew->before_w->scale.x ;
	ew->scale.y = ew->before_w->scale.y ;
	ew->atck_level = ew->before_w->atck_level ;
	ew->work_no = ew->before_w->work_no ;
	ew->col_cnt = ew->before_w->col_cnt ;
	ew->scale = ew->before_w->scale ;
	ew->prio = ew->before_w->prio ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl1a_at_spray_shot ;
	HIT_RESET( ew ) ;
	BODY_SIZE_SET( ew, 0x302828f0 ) ;
	ew->pos = ew->before_w->pos ;
	ofs = ew->scale.x * DOT_X( 224.0f ) ;
	if( !ew->disp_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->prio = 0 ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = -20.0f ;
	ew->acc.x = -DOT_X( 0.25f ) ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	if( pl_w->pl_sp_type == 0 ){
		switch( Sys->game_timer0 & 3 ){
		case 0 :
			ew->spd.y = 2.142857f ;
			break ;
		case 1 :
			ew->spd.y = -2.142857f ;
			break ;
		}
	}
	ew->spd.z = ( pl_w->scale.x * 1.2f - ew->scale.x ) / 30.0f ;
	ew->acc.z = ( pl_w->scale.y * 1.2f - ew->scale.y ) / 30.0f ;
	ew->timer0 = 5 ;
	ew->timer1 = 120 ;
	SHL1A_00_LW( ew )->x = ew->pos.x ;
	ew->dir = 1 ;
	ew->disp_flag = 1 ;
	pl_char_set2( ew, Char_Effect, 11, 0 ) ;
	shl1a_00_shot_move( ew ) ;
	pl_voice_req( ew, 27 ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_shot_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint16 no ;
	Sint16 frame ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->dir ){
		no = ( SHL1A_00_LW( ew )->x - ew->pos.x ) / ew->scale.x / 1.6666666f ;
		if( no < 0 ) no = -no ;
		if( no >= 0xe0 ){
			ew->dir = 1 ;
			pl_char_set( ew, Char_Effect, 11 ) ;
		} else {
			frame = ( no & 0xe0 ) >> 5 ;
			pl_char_set2( ew, Char_Effect, 13, frame ) ;
		}
	}
	ew->disp_flag = scr_in_x_ck( ew ) ;
	switch( ew->routine_3 ){
	case 0 :
		pl_char_move( ew ) ;
		if( ew->scale.x < pl_w->scale.x * 1.2f ) ew->scale.x += ew->spd.z ;
		if( ew->scale.y < pl_w->scale.y * 1.2f ) ew->scale.y += ew->acc.z ;
		if( -- ew->timer1 <= 0 ) goto end ;
		if( !ew->hit_flag ){
			hit_work_set( ew ) ;
			break ;
		}
		if( -- ew->timer0 <= 0 ){
end:
			ew->routine_3 = 2 ;
			pl_char_set( ew, Char_Effect, 12 ) ;
			break ;
		}
		ew->routine_3 ++ ;
		hit_work_set( ew ) ;
		break ;
	case 1 :
		pl_char_move( ew ) ;
		if( ew->scale.x < pl_w->scale.x * 1.2f ) ew->scale.x += ew->spd.z ;
		if( ew->scale.y < pl_w->scale.y * 1.2f ) ew->scale.y += ew->acc.z ;
		if( -- ew->hit_stop > 0 ) break ;
		ew->routine_3 -- ;
		ew->atck_no = pl1a_at_spray_shot ;
		HIT_RESET( ew ) ;
		hit_work_set( ew ) ;
		break ;
	case 2 :
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
		break ;
	}
}

static void shl1a_00_blast_init( PLAYER_WORK *ew );
static void shl1a_00_blast_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_00_blast( PLAYER_WORK *ew )
{
	static void ( *shl1a_00_blast_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_00_blast_init, shl1a_00_blast_move, shl1a_00_end, shl1a_00_free,
	} ;

	shl1a_00_blast_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_00_blast_init( PLAYER_WORK *ew )
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
	pl_char_set( ew, Char_Effect, 16 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = 0 ;
	ew->rot[2] = 0x19a ;
	ew->scale.x *= 1.5f ;
	ew->scale.y *= 0.6666667f ;
	ew->timer0 = 8 ;
	ew->atck_no = pl1a_at_blast ;
	HIT_RESET( ew ) ;
	ew->routine_3 = 1 ;
	shl1a_00_blast_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_blast_move( PLAYER_WORK *ew )
{
	Float ofs ;
	Uint8 no ;

	if( ew->master_w->routine_1 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos = ew->master_w->pos ;
	ofs = -( ew->master_w->scale.x * DOT_X( 106.0f ) ) ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->pos.y += 120.0f * ew->master_w->scale.y ;
	if( !ew->routine_1 ){
		pl_char_move( ew ) ;
		if( -- ew->timer0 <= 0 ){
			ew->timer0 = 16 ;
			if( ++ ew->routine_3 % 3 == 0 ) no = 59 ;
			else no = 60 ;
			ew->atck_no = no ;
			HIT_RESET( ew ) ;
		}
		if( ew->master_w->char_no.w != ( Uint16 )ew->timer1 ){
			ew->routine_1 ++ ;
			ew->atck_no |= ATCK_DIR ;
			pl_char_set( ew, Char_Effect, 17 ) ;
		}
		hit_work_set( ew ) ;
	} else if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1a_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1a_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1a_00.c" End of File	*/
