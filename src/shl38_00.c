/* Captain Commando's effects (character 38's shot 00): the hits and shots of his moves, a
   kind of work for each, picked by type. */
#include "player.h"
#include "pl_char.h"
#include "pl38_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL38_00_LOCAL_W {
	Sint32	unk_00;
	Sint32	unk_04;
	CHAR_NO	char_no;	/* 008: Captain Commando's animation when it was put out */
} SHL38_00_LOCAL_W;
#define SHL38_00_LW( w )	( ( SHL38_00_LOCAL_W * )&( w )->obj_flag )

static Float shl38_00_x_tbl[] = { -40.0f, -DOT_X( 88.0f ), -40.0f } ;

/* one step of the mark: its place from him, its draw order and animation */
typedef struct SHL38_00_MARK_STEP {
	Float	x, y;
	Sint16	sub_prio;
	Sint16	no;
} SHL38_00_MARK_STEP;

/* the marks' steps */
static SHL38_00_MARK_STEP shl38_00_mark_tbl[] = {
	{ 0.0f, 0.0f, 0, 0 },
	{ DOT_X( 29.0f ), DOT_Y( 52.0f ), -1, 1 },
	{ -DOT_X( 41.0f ), 0.0f, -1, 2 },
	{ 35.0f, DOT_Y( 117.0f ), -1, 3 },
	{ -35.0f, DOT_Y( 72.0f ), -1, 7 },
} ;

static void shl38_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *Shl38_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl38_00_main ;
		ew->type = type ;
		ew->sub_type = b ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = ew->type | 0x3800 ;
		SHL38_00_LW( ew )->char_no = pl_w->char_no ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *Shl38_set2( PLAYER_WORK *w, Uint8 type, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl38_00_main ;
		ew->type = type ;
		ew->sub_type = b ;
		ew->master_w = w->master_w ;
		ew->ef_w = w ;
		ew->ef_no = w->type | 0x3800 ;
		ew->pl_type = w->pl_type ;
		SHL38_00_LW( ew )->char_no = SHL38_00_LW( w )->char_no ;
	}
	return w ;
}

static void shl38_00_fire_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_corridor_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_corridor_side_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_mark_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_l_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_h_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t08_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_dust_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t11_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t12_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t13_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t14_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t15_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_h_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_l_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_main( PLAYER_WORK *ew )
{
	static void ( *shl38_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_fire_main, shl38_00_corridor_main, shl38_00_corridor_side_main, shl38_00_mark_main, shl38_00_fire_main, shl38_00_strike_l_main, shl38_00_strike_h_main,
		shl38_00_strike_x_main, shl38_00_t08_main, shl38_00_strike_x_shot_main,
		shl38_00_dust_main,
		shl38_00_t11_main,
		shl38_00_t12_main,
		shl38_00_t13_main,
		shl38_00_t14_main,
		shl38_00_t15_main,
		shl38_00_catch_h_main,
		shl38_00_catch_x_main,
		shl38_00_catch_l_main,
		shl38_00_fire_main, shl38_00_fire_main,
		shl38_00_catch_x_shot_main,
		shl38_00_fire_main,
	} ;

	shl38_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl38_00_fire_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_fire_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_fire_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_fire_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_fire_init, shl38_00_fire_move, shl38_00_end, shl38_00_free,
	} ;

	if( SHL38_00_LW( ew )->char_no.w != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	shl38_00_fire_jmp[ew->routine_0]( ew, pl_w ) ;
}

static Uint8 shl38_00_fire_atck[] = { 0x30, 0x4a, 0x33, 0x4c } ;

/*==============================================================*/
static void shl38_00_fire_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 atck ;

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
	ew->atck_flag = HIT_SHOT ;
	ew->atck_flag = HIT_SHOT ;
	atck = shl38_00_fire_atck[( ew->type >> 1 ) + ew->atck_level] ;
	ew->atck_no = atck ;
	HIT_RESET( ew ) ;
	if( ew->type ){
		ew->spd.x = -DOT_X( 16.0f ) ;
		if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		ew->spd.y = DOT_Y( 32.0f ) ;
	} else ew->spd.x = ew->spd.y = 0.0f ;
	pl_char_set( ew, 21, ( ew->atck_level << 1 ) + 3 ) ;
	shl38_00_fire_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_fire_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
	else if( !ew->hit_box_no ) hit_work_set( ew ) ;
}

static void shl38_00_corridor_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_corridor_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_corridor_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_corridor_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_corridor_init, shl38_00_corridor_move, shl38_00_end, shl38_00_free,
	} ;

	if( SHL38_00_LW( ew )->char_no.w != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	shl38_00_corridor_jmp[ew->routine_0]( ew, pl_w ) ;
}

static Float shl38_00_corridor_y[] = { 120.0f, DOT_Y( 119.0f ), DOT_Y( 111.0f ), DOT_Y( 111.0f ), DOT_Y( 111.0f ) } ;

/*==============================================================*/
static void shl38_00_corridor_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int no ;
	Sint16 x ;
	PLAYER_WORK *w ;

	if( !ew->sub_type ){
		Shl38_set2( ew, 1, 4 ) ;
		Shl38_set2( ew, 1, 3 ) ;
		Shl38_set2( ew, 1, 2 ) ;
		Shl38_set2( ew, 1, 1 ) ;
	}
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
	x = shl38_00_x_tbl[( pl_w->pl_sp_type == 6 ) ? 2 : ew->atck_level] ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 64 ;
	ew->atck_flag = HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = 0 ;
	if( pl_w->pl_sp_type == 6 ) x = 64 ;
	else if( !ew->atck_level ) x = 49 ;
	else x = 78 ;
	ew->atck_no = x ;
	ew->hit_flag = ew->atck_attr = 0 ;
	ew->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	if( ew->sub_type == 4 ) no = ew->atck_level * 8 + 11 ;
	else no = ew->atck_level * 8 + ew->sub_type + 9 ;
	pl_char_set( ew, 22, no ) ;
	if( !ew->sub_type ) w = pl_w ;
	else w = ew->before_w ;
	ew->pos.y = w->pos.y + shl38_00_corridor_y[ew->sub_type] ;
	shl38_00_corridor_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_corridor_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	if( pl_char_move( ew ) < 0 ){
		work_free( ew ) ;
		return ;
	}
	else if( !ew->sub_type && ew->cg_type ){
		ew->cg_type = 0 ;
		Shl38_set2( ew, 2, 0 ) ;
		Shl38_set2( ew, 2, 1 ) ;
	}
	if( !pl_w->hit_flag ) hit_work_set( ew ) ;
	if( !scr_in_ck( ew ) ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
}

static void shl38_00_corridor_side_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_corridor_side_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_corridor_side_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_corridor_side_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_corridor_side_init, shl38_00_corridor_side_move, shl38_00_end, shl38_00_free,
	} ;

	if( SHL38_00_LW( ew )->char_no.w != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	shl38_00_corridor_side_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_corridor_side_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint32 i ;
	Uint8 atck ;

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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( !ew->atck_level ) atck = 72 ;
	else atck = 80 ;
	ew->atck_no = atck ;
	HIT_RESET( ew ) ;
	if( ew->sub_type ) ew->disp_dir ^= 1 ;
	if( pl_w->pl_sp_type == 6 ) i = 2 ;
	else i = ew->atck_level ;
	if( !pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + shl38_00_x_tbl[i] ;
	else ew->pos.x = pl_w->pos.x - shl38_00_x_tbl[i] ;
	ew->pos.y = pl_w->pos.y ;
	pl_char_set( ew, 22, ( ew->atck_level << 1 ) + 21 ) ;
	shl38_00_corridor_side_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_corridor_side_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
	if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
	else hit_work_set( ew ) ;
}

static void shl38_00_mark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_mark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_mark_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_mark_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_mark_init, shl38_00_mark_move, shl38_00_end, shl38_00_free,
	} ;

	if( SHL38_00_LW( ew )->char_no.w != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	shl38_00_mark_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_mark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->dir = 255 ;
	shl38_00_mark_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_mark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( pl_w->cg_prm < 0 ){
		work_free( ew ) ;
		return ;
	}
	else if( !pl_w->cg_prm ) return ;
	if( pl_w->cg_prm != ew->dir ){
		ew->dir = pl_w->cg_prm ;
		x = shl38_00_mark_tbl[ew->dir].x ;
		ew->spd.y = shl38_00_mark_tbl[ew->dir].y ;
		ew->sub_prio = shl38_00_mark_tbl[ew->dir].sub_prio ;
		ew->disp_flag = 1 ;
		pl_char_set( ew, 25, shl38_00_mark_tbl[ew->dir].no ) ;
		if( pl_w->disp_dir ) x = -x ;
		ew->spd.x = x ;
	}
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
	pl_char_move( ew ) ;
}

static void shl38_00_strike_l_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_l_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_strike_l_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_strike_l_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_strike_l_init, shl38_00_strike_l_move, shl38_00_end, shl38_00_free,
	} ;

	pl_w->team_ng_timer = PL38_LW( pl_w )->shot_timer = 2 ;
	shl38_00_strike_l_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_strike_l_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 64 ;
	ew->sub_prio = -8 ;
	ew->atck_no = pl38_at_strike_l ;
	HIT_RESET( ew ) ;
	ew->timer1 = pl_w->ground_y ;
	ew->pos.y = pl_w->ground_y + DOT_Y( 96.0f ) ;
	if( ( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x = pl_w->pos.x + DOT_X( 16.0f ) ;
		ew->spd.x = DOT_X( 4.0f ) ;
		ew->spd.y = 0.0f ;
	} else {
		ew->pos.x = pl_w->pos.x - DOT_X( 16.0f ) ;
		ew->spd.x = -DOT_X( 4.0f ) ;
		ew->spd.y = 0.0f ;
	}
	ew->acc.y = -DOT_Y( 0.375f ) ;
	ew->timer0 = 2 ;
	pl_char_set( ew, 21, 18 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*==============================================================*/
/*			It leaves upward, drifting the way it faces			*/
/*==============================================================*/
static void shl38_00_strike_out_set( PLAYER_WORK *ew )
{
	Float spd ;

	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	spd = -DOT_X( 4.0f ) ;
	if( ew->disp_dir ) spd = DOT_X( 4.0f ) ;
	ew->spd.x = spd ;
	ew->spd.y = DOT_Y( 12.0f ) ;
	if( !ew->hit_box_no ) hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl38_00_strike_l_hit( PLAYER_WORK *ew )
{
	ew->routine_1 = 3 ;
	pl_voice_req( ew->master_w, 1 ) ;
	pl_char_set( ew, 20, 3 ) ;
	catch_hitmark( ew ) ;
	shl38_00_strike_out_set( ew ) ;
}

static void shl38_00_strike_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_00_strike_l_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	switch( ew->routine_1 ){
	case 0:
		pl_char_move( ew ) ;
		if( !ew->cg_type ) return ;
		ew->routine_2 = 0 ;
		ew->cg_type = 0 ;
		ew->routine_1 ++ ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		Shl38_set2( ew, 8, 0 ) ;
		pl_voice_req( pl_w, 27 ) ;
		break ;
	case 1:
		if( ew->hit_box_no ){
			shl38_00_strike_l_hit( ew ) ;
			return ;
		} else if( ew->hit_stop ){
			ew->hit_stop -- ;
			pl_frame_jump( ew ) ;
			break ;
		}
		if( ew->hit_flag && ew->timer0 ){
			ew->timer0 -- ;
			ew->atck_no = pl38_at_strike_l ;
			HIT_RESET( ew ) ;
		}
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !ew->routine_2 && pl_w->ground_y + DOT_Y( 72.0f ) > ew->pos.y ){
			ew->routine_2 ++ ;
			pl_char_set( ew, 21, 20 ) ;
			pl_se_req( pl_w, 26 ) ;
		}
		if( pl_w->ground_y > ew->pos.y ){
			ew->routine_1 ++ ;
			pl_char_set( ew, 21, 21 ) ;
		}
		hit_work_set( ew ) ;
		break ;
	case 2:
		if( ew->hit_box_no ){
			shl38_00_strike_l_hit( ew ) ;
			break ;
		}
		pl_char_move( ew ) ;
		if( ew->cg_type ){
			ew->cg_type = 0 ;
			ew->routine_1 ++ ;
			shl38_00_strike_out_set( ew ) ;
			break ;
		}
		if( !ew->hit_box_no ) hit_work_set( ew ) ;
		break ;
	case 3:
		shl38_00_strike_out( ew, pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void shl38_00_strike_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_stop ){
		ew->hit_stop -- ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
	else shl38_00_end( ew, pl_w ) ;
}

static void shl38_00_strike_h_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_h_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_strike_h_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_strike_h_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_strike_h_init, shl38_00_strike_h_move, shl38_00_end, shl38_00_free,
	} ;

	pl_w->team_ng_timer = PL38_LW( pl_w )->shot_timer = 2 ;
	shl38_00_strike_h_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_strike_h_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->size_top = 128 ;
	ew->size_bottom = 0 ;
	ew->size_front = ew->size_back = 64 ;
	ew->sub_prio = -8 ;
	ew->atck_no = pl38_at_strike_h ;
	HIT_RESET( ew ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->pos.y = pl_w->ground_y + DOT_Y( 128.0f ) ;
	if( pl_w->disp_dir ){
		ew->pos.x = scr_work.scr_left - DOT_X( 64.0f ) ;
		ew->spd.x = DOT_X( 10.0f ) ;
	} else {
		ew->pos.x = scr_work.scr_right + DOT_X( 64.0f ) ;
		ew->spd.x = -DOT_X( 10.0f ) ;
	}
	ew->spd.y = -15.0f ;
	ew->acc.y = DOT_Y( 0.25f ) ;
	ew->timer0 = 2 ;
	pl_char_set( ew, 21, 22 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_h_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int out ;

	if( ew->routine_1 != 0 ){
		shl38_00_strike_out( ew, pl_w ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_1 ++ ;
		pl_voice_req( pl_w, 1 ) ;
		pl_char_set( ew, 20, 9 ) ;
		catch_hitmark( ew ) ;
		shl38_00_strike_out_set( ew ) ;
		return ;
	} else if( ew->hit_stop ){
		ew->hit_stop -- ;
		hit_work_set( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->timer0 && ew->cg_flag1 && ew->hit_flag ){
		ew->timer0 -- ;
		ew->atck_no = ew->cg_flag1 ;
		HIT_RESET( ew ) ;
		ew->cg_prm = 0 ;
	}
	out = 0 ;
	if( ew->disp_dir ){
		if( !( scr_work.scr_right + DOT_X( 64.0f ) > ew->pos.x ) ) out = 1 ;
	} else {
		if( !( ew->pos.x > scr_work.scr_left - DOT_X( 64.0f ) ) ) out = 1 ;
	}
	if( out ) shl38_00_end( ew, pl_w ) ;
	else hit_work_set( ew ) ;
}

static void shl38_00_strike_x_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_strike_x_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_strike_x_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_strike_x_init, shl38_00_strike_x_move, shl38_00_end, shl38_00_free,
	} ;

	pl_w->team_ng_timer = PL38_LW( pl_w )->shot_timer = 2 ;
	shl38_00_strike_x_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_strike_x_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int x ;

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
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_flag = 0 ;
	ew->size_top = 128 ;
	ew->size_bottom = 0 ;
	ew->size_front = ew->size_back = 64 ;
	ew->sub_prio = -8 ;
	ew->atck_no = pl38_at_strike_x ;
	HIT_RESET( ew ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->pos.y = pl_w->ground_y + DOT_Y( 256.0f ) ;
	if( pl_w->disp_dir ){
		ew->pos.x = scr_work.scr_left - DOT_X( 32.0f ) ;
		x = 160 ;
	} else {
		ew->pos.x = scr_work.scr_right + DOT_X( 32.0f ) ;
		x = -160 ;
	}
	ew->spd.x = x >> 4 ;
	ew->spd.y = -DOT_Y( 16.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 2 ;
	pl_char_set( ew, 21, 24 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

static void shl38_00_strike_x_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_shot_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_shot_miss( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_strike_x_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_strike_x_in, shl38_00_strike_x_land, shl38_00_strike_x_shot, shl38_00_strike_x_wait, shl38_00_strike_x_out,
		shl38_00_strike_x_hit, shl38_00_strike_x_shot_hit, shl38_00_strike_x_shot_miss, shl38_00_strike_x_end, shl38_00_strike_x_out,
	} ;

	shl38_00_strike_x_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_strike_x_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_box_no ){
		shl38_00_strike_x_hit( ew, pl_w ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		pl_char_move( ew ) ;
		pl_se_req( pl_w, 52 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_box_no ) shl38_00_strike_x_hit( ew, pl_w ) ;
	else {
		if( pl_char_move( ew ) < 0 ){
			ew->routine_1 ++ ;
			ew->dm_flag = 0 ;
			pl_char_set( ew, 21, 25 ) ;
		}
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		ew->timer0 = 180 ;
		pl_se_req( pl_w, 75 ) ;
		if( !Shl38_set2( ew, 9, 0 ) ) ew->timer0 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( -- ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 21, 30 ) ;
		shl38_00_strike_out_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
	else shl38_00_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 = 4 ;
	pl_char_set( ew, 21, 30 ) ;
	shl38_00_strike_out_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_shot_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 = 8 ;
	pl_char_set( ew, 21, 28 ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_shot_miss( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 = 8 ;
	pl_char_set( ew, 21, 29 ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		ew->cg_type = 0 ;
		shl38_00_strike_out_set( ew ) ;
	} else if( !ew->hit_box_no ) hit_work_set( ew ) ;
}

static void shl38_00_t08_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t08_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_t08_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_t08_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_t08_init, shl38_00_t08_move, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_t08_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_t08_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w ;
	Float x, spd, acc ;

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
	w = ew->ef_w ;
	ew->pos.y = w->pos.y - DOT_Y( 25.0f ) ;
	x = DOT_X( 46.0f ) ;
	spd = DOT_X( 4.0f ) ;
	acc = -DOT_X( 0.25f ) ;
	if( ew->disp_dir ){
		spd = -DOT_X( 4.0f ) ;
		acc = DOT_X( 0.25f ) ;
		x = -DOT_X( 46.0f ) ;
	}
	ew->pos.x = w->pos.x + x ;
	ew->spd.x = spd ;
	ew->acc.x = acc ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ew->timer0 = 40 ;
	pl_char_set( ew, 21, 19 ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_t08_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int old, now ;

	pl_char_move( ew ) ;
	old = ew->spd.x ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	now = ew->spd.x ;
	if( ( old ^ now ) < 0 ){
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
	}
	if( -- ew->timer0 < 0 ){
		shl38_00_free( ew, pl_w ) ;
		return ;
	}
	if( ew->timer0 < 30 ){
		if( Sys->game_timer0 & 1 ) ew->disp_flag = 0 ;
		else ew->disp_flag = 1 ;
	}
}

static void shl38_00_strike_x_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_strike_x_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_strike_x_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_strike_x_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_strike_x_shot_init, shl38_00_strike_x_shot_move, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_strike_x_shot_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_strike_x_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int x ;
	PLAYER_WORK *w ;

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
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->size_top = 48 ;
	ew->size_bottom = 16 ;
	ew->size_front = 48 ;
	ew->size_back = 112 ;
	ew->sub_prio = -16 ;
	ew->atck_no = pl38_at_strike_x ;
	HIT_RESET( ew ) ;
	w = ew->ef_w ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = -DOT_X( 4.0f ) ;
	ew->acc.x = -DOT_X( 0.25f ) ;
	ew->pos.y = w->pos.y + DOT_Y( 64.0f ) ;
	x = -0x180000 ;
	if( ( ew->disp_dir = w->disp_dir ) ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
		x = 0x180000 ;
	}
	ew->pos.x = w->pos.x + x * 1.6666666f / 65536.0f ;
	pl_char_set( ew, 21, 26 ) ;
	Shl38_set2( ew, 14, 0 ) ;
	pl_voice_req( pl_w, 25 ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_strike_x_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int x ;
	Float spd, lim, stop ;
	VEC3 pos ;
	PLAYER_WORK *w ;

	if( !ew->routine_1 ){
		if( ew->hit_flag || ew->hit_box_no ){
			w = ew->ef_w ;
			ew->routine_1 ++ ;
			ew->timer0 = 62 ;
			pl_char_set( ew, 21, 27 ) ;
			ew->spd.y = DOT_Y( 8.0f ) ;
			ew->acc.y = -DOT_Y( 0.25f ) ;
			x = -0x80000 ;
			if( w->disp_dir ) x = 0x80000 ;
			ew->spd.x = ( w->pos.x + x * 1.6666666f / 65536.0f - ew->pos.x ) / 64.0f ;
			ew->acc.x = 0.0f ;
			pos.x = -DOT_X( 32.0f ) ;
			if( ew->disp_dir ) pos.x = -pos.x ;
			pos.x += ew->pos.x ;
			pos.y = ew->pos.y ;
			burst_set( pl_w, &pos, 1, 249 ) ;
			pl_se_req( pl_w, 73 ) ;
			goto move ;
		} else {
			pl_char_move( ew ) ;
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
			spd = ew->spd.x ;
			stop = lim = DOT_X( 8.0f ) ;
			if( ew->acc.x ){
				if( ew->acc.x >= 0.0f ){
					spd = -spd ;
					stop = -DOT_X( 8.0f ) ;
				}
				if( spd >= lim ){
					ew->acc.x = 0.0f ;
					ew->spd.x = stop ;
				}
			}
			if( !scr_in_x_ck( ew ) ){
				PLAYER_WORK *ef_w = ew->ef_w ;

				if( ef_w->routine_1 == 3 ) ef_w->routine_1 = 5 ;
				shl38_00_end( ew, pl_w ) ;
			} else hit_work_set( ew ) ;
			goto end ;
		}
	}
move:
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 <= 0 ){
		PLAYER_WORK *ef_w = ew->ef_w ;
		PLAYER_WORK *master_w = ew->master_w ;

		if( !ew->hit_box_no && ew->hit_flag && ew->em_w && master_w->target_w == ew->em_w && !( ew->hit_flag & HR_GUARD ) ){
			ef_w->routine_1 = 6 ;
			shl38_00_end( ew, pl_w ) ;
		} else {
			ef_w->routine_1 = 7 ;
			shl38_00_end( ew, pl_w ) ;
		}
	}
end:
	;
}

static void shl38_00_dust_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_dust_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_dust_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_dust_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_dust_init, shl38_00_dust_move, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_dust_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_dust_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x, spd ;

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
	ew->pos.y = pl_w->ground_y ;
	x = -DOT_X( 112.0f ) ;
	spd = -DOT_X( 44.0f ) ;
	if( !pl_w->disp_dir ){
		ew->pos.x = pl_w->pos.x + x ;
		ew->spd.x = spd ;
	} else {
		ew->pos.x = pl_w->pos.x - x ;
		ew->spd.x = DOT_X( 44.0f ) ;
	}
	ew->timer0 = 0 ;
	ew->timer1 = 2 ;
	ew->size_top = ew->size_bottom = 0 ;
	ew->size_front = ew->size_back = 48 ;
	shl38_00_dust_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_dust_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( scr_in_x_ck( ew ) ){
		if( -- ew->timer0 < 0 ){
			ew->timer0 = ew->timer1 ;
			land_dust_set( &ew->pos, -1 ) ;
		}
	} else shl38_00_end( ew, pl_w ) ;
}

static void shl38_00_t11_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t12_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t13_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_t11_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_t11_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_t11_free, shl38_00_t11_free, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_t11_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_t11_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl38_00_t12_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_t12_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_t12_free, shl38_00_t12_free, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_t12_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_t12_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl38_00_t13_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_t13_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_t13_free, shl38_00_t13_free, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_t13_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_t13_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

static void shl38_00_t14_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t14_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_t14_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_t14_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_t14_init, shl38_00_t14_move, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_t14_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_t14_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	pl_char_set( ew, 21, 31 ) ;
	shl38_00_t14_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_t14_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w = ew->ef_w ;

	if( w->routine_0 >= 2 || w->char_no.b.grp != 21 || w->char_no.b.no != 26 ){
		shl38_00_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x = w->pos.x ;
	ew->pos.y = w->pos.y ;
	ew->prio = w->prio ;
	ew->sub_prio = w->sub_prio ;
	pl_char_move( ew ) ;
}

static void shl38_00_t15_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_t15_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_t15_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_t15_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_t15_init, shl38_00_t15_move, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_t15_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_t15_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	pl_char_set( ew, 15, 1 ) ;
	shl38_00_t15_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_t15_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 != 1 || SHL38_00_LW( ew )->char_no.w != pl_w->char_no.w || LO_BYTE( pl_w->cg_rival ) != 1 ){
		work_free( ew ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->catch_w->prio ;
	ew->sub_prio = 8 ;
}

static void shl38_00_catch_h_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_h_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_catch_h_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_catch_h_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_catch_h_init, shl38_00_catch_h_move, shl38_00_end, shl38_00_free,
	} ;

	PL38_LW( pl_w )->shot_timer = 2 ;
	shl38_00_catch_h_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_catch_h_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x, spd ;

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
	ew->sub_prio = 8 ;
	ew->ef_w = pl_w->catch_w ;
	ew->size_top = 128 ;
	ew->size_bottom = 0 ;
	ew->size_front = ew->size_back = 64 ;
	ew->pos.y = DOT_Y( 480.0f ) ;
	x = 160.0f ;
	spd = -DOT_X( 2.0f ) ;
	if( ew->disp_dir ){
		x = -160.0f ;
		spd = DOT_X( 2.0f ) ;
	}
	ew->pos.x = pl_w->pos.x + x ;
	ew->spd.x = spd ;
	ew->spd.y = -60.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, 20, 9 ) ;
}

static void shl38_00_catch_h_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_h_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_h_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_00_catch_h_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_catch_h_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_catch_h_in, shl38_00_catch_h_wait, shl38_00_catch_out, shl38_00_catch_h_up,
	} ;

	ew->prio = pl_w->prio ;
	shl38_00_catch_h_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_catch_h_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->pos.y ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_char_set( ew, 20, 14 ) ;
		pl_voice_req( pl_w, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_h_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w = ew->ef_w ;
	Float spd ;

	if( w->routine_1 != PL_CAUGHT ){
		ew->routine_1 = 2 ;
		pl_char_set( ew, 20, 9 ) ;
		spd = DOT_X( 2.0f ) ;
		ew->spd.y = 60.0f ;
		ew->acc.y = 0.0f ;
		if( ew->disp_dir ) spd = -DOT_X( 2.0f ) ;
		ew->spd.x = spd ;
		shl38_00_catch_out( ew, pl_w ) ;
	} else {
		if( !w->catch_type || w->pos.y < DOT_Y( 128.0f ) ) return ;
		ew->routine_1 = 3 ;
		ew->pos.x = w->pos.x ;
		ew->atck_flag = HIT_ON | HIT_OBJ ;
		ew->dm_flag = 0 ;
		ew->atck_no = pl38_at_throw_prop ;
		HIT_RESET( ew ) ;
		ew->timer0 = 2 ;
		ew->spd.y = DOT_Y( 12.0f ) ;
		spd = -0.052083332f ;
		if( ew->disp_dir ) spd = 0.052083332f ;
		ew->spd.x = spd ;
		pl_char_set( ew, 20, 26 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ) shl38_00_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_h_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_stop ) ew->hit_stop -- ;
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( ew->timer0 && ew->hit_flag ){
			ew->timer0 -- ;
			ew->atck_no = pl38_at_throw_prop ;
			HIT_RESET( ew ) ;
		}
		if( scr_work.scr_top > ew->pos.y ) hit_work_set( ew ) ;
		else shl38_00_end( ew, pl_w ) ;
	}
}

static void shl38_00_catch_x_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_catch_x_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_catch_x_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_catch_x_init, shl38_00_catch_x_move, shl38_00_end, shl38_00_free,
	} ;

	PL38_LW( pl_w )->shot_timer = 2 ;
	ew->prio = pl_w->prio ;
	shl38_00_catch_x_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_catch_x_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x, spd ;

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
	ew->sub_prio = 8 ;
	ew->ef_w = pl_w->catch_w ;
	ew->size_top = 128 ;
	ew->size_bottom = 0 ;
	ew->size_front = 96 ;
	ew->size_back = 64 ;
	ew->pos.y = DOT_Y( 480.0f ) ;
	x = 160.0f ;
	spd = -DOT_X( 2.0f ) ;
	if( ew->disp_dir ){
		x = -160.0f ;
		spd = DOT_X( 2.0f ) ;
	}
	ew->pos.x = pl_w->pos.x + x ;
	ew->spd.x = spd ;
	ew->spd.y = -60.0f ;
	ew->acc.y = 0.0f ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, 20, 15 ) ;
}

static void shl38_00_catch_x_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_00_catch_x_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_catch_x_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_catch_x_in, shl38_00_catch_x_wait, shl38_00_catch_out, shl38_00_catch_x_shot, shl38_00_catch_x_keep, shl38_00_catch_x_end,
	} ;

	shl38_00_catch_x_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_catch_x_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_w->ground_y > ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->pos.y ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_char_set( ew, 20, 17 ) ;
		pl_voice_req( pl_w, 25 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_x_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w = ew->ef_w ;
	Float spd ;

	if( w->routine_1 != PL_CAUGHT ){
		ew->routine_1 = 2 ;
		pl_char_set( ew, 20, 15 ) ;
		spd = DOT_X( 2.0f ) ;
		ew->spd.y = 60.0f ;
		ew->acc.y = 0.0f ;
		if( ew->disp_dir ) spd = -DOT_X( 2.0f ) ;
		ew->spd.x = spd ;
		shl38_00_catch_out( ew, pl_w ) ;
	} else {
		if( !w->catch_type || w->pos.y < DOT_Y( 192.0f ) ) return ;
		ew->routine_1 = 3 ;
		pl_char_set( ew, 20, 20 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_x_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->pos.y - pl_w->ground_y < DOT_Y( 96.0f ) ){
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
		pl_se_req( pl_w, 75 ) ;
		Shl38_set2( ew, 21, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_x_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( -- ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 20, 21 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_x_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 = 2 ;
		pl_char_set( ew, 20, 15 ) ;
		spd = DOT_X( 2.0f ) ;
		ew->spd.y = 60.0f ;
		ew->acc.y = 0.0f ;
		if( ew->disp_dir ) spd = -DOT_X( 2.0f ) ;
		ew->spd.x = spd ;
		shl38_00_catch_out( ew, pl_w ) ;
	}
}

static void shl38_00_catch_l_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_l_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_catch_l_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_catch_l_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_catch_l_init, shl38_00_catch_l_move, shl38_00_end, shl38_00_free,
	} ;

	PL38_LW( pl_w )->shot_timer = 2 ;
	ew->prio = pl_w->prio ;
	shl38_00_catch_l_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_catch_l_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

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
	ew->sub_prio = 8 ;
	ew->ef_w = pl_w->catch_w ;
	ew->size_top = 80 ;
	ew->size_bottom = 0 ;
	ew->size_front = 80 ;
	ew->size_back = 48 ;
	ew->pos.y = pl_w->ground_y ;
	ew->pos.x = pl_w->pos.x ;
	x = -DOT_X( 80.0f ) ;
	if( ew->disp_dir ) x = DOT_X( 80.0f ) ;
	ew->pos.x = pl_w->pos.x + x ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, 20, 8 ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_l_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_1 ++ ;
			ew->disp_dir ^= 1 ;
			ew->spd.x = DOT_X( 2.0f ) ;
			ew->spd.y = 60.0f ;
			pl_char_set( ew, 20, 3 ) ;
		}
	} else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( !scr_in_ck( ew ) ) shl38_00_end( ew, pl_w ) ;
	}
}

static void shl38_00_catch_x_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_00_catch_x_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_00_catch_x_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_00_catch_x_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_00_catch_x_shot_init, shl38_00_catch_x_shot_move, shl38_00_end, shl38_00_free,
	} ;

	shl38_00_catch_x_shot_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_00_catch_x_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	pl_char_set( ew, 20, 25 ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_catch_x_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w = ew->ef_w ;

	if( w->char_no.w == 0x1414 ){
		ew->pos.x = w->pos.x ;
		ew->pos.y = w->pos.y ;
		ew->prio = 0 ;
		if( pl_char_move( ew ) >= 0 ) return ;
	}
	shl38_00_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl38_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl38_00.c" End of File	*/
