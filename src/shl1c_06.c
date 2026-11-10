/* Hyper Mega Man's shots and missiles (character 1c's shot 06): kinds 0 to 2 are the
   shots that leave the player, kinds 3 to 14 turn through a table of directions, and kind 15 is
   the flame that follows a missile. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl1c_def.h"

/* a missile's place from the player: kinds 3 to 14 */
typedef struct SHL1C_06_POS {
	Float	x;
	Float	y;
} SHL1C_06_POS;

/* the table of directions a missile turns through, as the pointer in ef_w */
#define SHL1C_06_SEQ( w )	( *( Sint8 ** )&( w )->ef_w )

static void shl1c_06_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1c_06_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_06_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

/****************************************************************/
void *shl1c_06_set2( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl1c_06_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static PL1C_SHOT_DATA shl1c_06_data[3] = {
	{ -10 << 16, 0, -47, 45, 17, 64 },
	{ -10 << 16, 0, 3, 92, 15, 65 },
	{ -10 << 16, 0, 36, 41, 13, 66 },
} ;

static SHL1C_06_POS shl1c_06_pos[12] = {
	{ -37.0f, 132.0f }, { -21.0f, 132.0f },
	{ -37.0f, 125.0f }, { -21.0f, 125.0f },
	{ -37.0f, 118.0f }, { -21.0f, 118.0f },
	{ 11.0f, 132.0f }, { 27.0f, 132.0f },
	{ 11.0f, 125.0f }, { 27.0f, 125.0f },
	{ 11.0f, 118.0f }, { 27.0f, 118.0f },
} ;

/* pairs of a direction (in 16ths of a turn) and its time; 255 holds the last */
static Sint8 shl1c_06_seq[12][32] = {
	{ 13, 4, 14, 4, 15, 4, 0, 1, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, 6, 1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12, 255 },
	{ 11, 4, 10, 6, 11, 4, 12, 255, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 255 },
	{ 13, 4, 14, 4, 15, 4, 0, 1, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, 6, 1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12, 255 },
	{ 11, 4, 10, 6, 11, 4, 12, 255, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 255 },
	{ 13, 4, 14, 4, 15, 4, 0, 1, 1, 1, 2, 1, 3, 1, 4, 1, 5, 1, 6, 1, 7, 1, 8, 1, 9, 1, 10, 1, 11, 1, 12, 255 },
	{ 11, 4, 10, 6, 11, 4, 12, 255, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 255 },
	{ 2, 2, 1, 1, 0, 1, 15, 1, 14, 1, 13, 1, 12, 255, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 255 },
	{ 12, 4, 11, 4, 10, 4, 9, 4, 8, 4, 9, 4, 10, 4, 11, 4, 12, 4, 11, 4, 10, 4, 11, 4, 12, 4, 11, 4, 10, 4, 11, 255 },
	{ 2, 2, 1, 1, 0, 1, 15, 1, 14, 1, 13, 1, 12, 255, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 255 },
	{ 12, 4, 11, 4, 10, 4, 9, 4, 8, 4, 9, 4, 10, 4, 11, 4, 12, 4, 11, 4, 10, 4, 11, 4, 12, 4, 11, 4, 10, 4, 11, 255 },
	{ 2, 2, 1, 1, 0, 1, 15, 1, 14, 1, 13, 1, 12, 255, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 255 },
	{ 12, 4, 11, 4, 10, 4, 9, 4, 8, 4, 9, 4, 10, 4, 11, 4, 12, 4, 11, 4, 10, 4, 11, 4, 12, 4, 11, 4, 10, 4, 11, 255 },
} ;

static Sint8 *shl1c_06_seq_tbl[12] = {
	shl1c_06_seq[0], shl1c_06_seq[1], shl1c_06_seq[2], shl1c_06_seq[3],
	shl1c_06_seq[4], shl1c_06_seq[5], shl1c_06_seq[6], shl1c_06_seq[7],
	shl1c_06_seq[8], shl1c_06_seq[9], shl1c_06_seq[10], shl1c_06_seq[11],
} ;

static void shl1c_06_init( PLAYER_WORK *ew );
static void shl1c_06_move( PLAYER_WORK *ew );
static void shl1c_06_end( PLAYER_WORK *ew );
static void shl1c_06_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_06_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_06_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_06_init, shl1c_06_move, shl1c_06_end, shl1c_06_free,
	} ;

	shl1c_06_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_06_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c06 ;
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
	ew->prio = 8 ;
	ew->disp_flag = 1 ;
	shl1c_06_move( ew ) ;
}

static void shl1c_06_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_missile( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_flame( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_06_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_06_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_06_shot, shl1c_06_shot, shl1c_06_ground,
		shl1c_06_missile, shl1c_06_missile, shl1c_06_missile, shl1c_06_missile, shl1c_06_missile, shl1c_06_missile,
		shl1c_06_missile, shl1c_06_missile, shl1c_06_missile, shl1c_06_missile, shl1c_06_missile, shl1c_06_missile,
		shl1c_06_flame,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1c_06_move_jmp[ew->type]( ew, pl_w ) ;
}

static void shl1c_06_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_06_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_06_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_06_shot_init, shl1c_06_shot_fly, shl1c_06_shot_move,
	} ;

	shl1c_06_shot_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_06_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_SHOT_DATA *dp ;

	ew->routine_1 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	dp = &shl1c_06_data[ew->type] ;
	ew->pos = pl_w->pos ;
	if( !ew->disp_dir ){
		ew->pos.x += DOT_X( dp->x ) ;
		ew->spd.x = dp->spd_x * 1.6666666f / 65536.0f ;
	} else {
		ew->pos.x += -DOT_X( dp->x ) ;
		ew->spd.x = -( dp->spd_x * 1.6666666f / 65536.0f ) ;
	}
	ew->pos.y += DOT_Y( dp->y ) ;
	ew->spd.y = dp->spd_y * 2.142857f / 65536.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = dp->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, dp->char_no ) ;
	shl1c_06_shot_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !ew->cg_type ){
		ew->routine_1 ++ ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

static void shl1c_06_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_ground_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_ground_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_06_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_06_ground_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_06_ground_init, shl1c_06_ground_fly, shl1c_06_ground_fall, shl1c_06_ground_move,
	} ;

	shl1c_06_ground_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_06_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_SHOT_DATA *dp ;

	ew->routine_1 ++ ;
	ew->size_top = 24 ;
	ew->size_bottom = 24 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	dp = &shl1c_06_data[ew->type] ;
	ew->pos = pl_w->pos ;
	if( !ew->disp_dir ){
		ew->pos.x += DOT_X( dp->x ) ;
		ew->spd.x = dp->spd_x * 1.6666666f / 65536.0f ;
	} else {
		ew->pos.x += -DOT_X( dp->x ) ;
		ew->spd.x = -( dp->spd_x * 1.6666666f / 65536.0f ) ;
	}
	ew->pos.y += DOT_Y( dp->y ) ;
	ew->spd.y = dp->spd_y * 2.142857f / 65536.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = dp->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, dp->char_no ) ;
	shl1c_06_ground_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_ground_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !ew->cg_type ){
		ew->routine_1 ++ ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_ground_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

static void shl1c_06_missile_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_missile_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_06_missile( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_06_missile_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_06_missile_init, shl1c_06_missile_move,
	} ;

	shl1c_06_missile_jmp[ew->routine_1]( ew, pl_w ) ;
}

static Uint8 shl1c_06_dir_ofs( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_seq_step( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_06_missile_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->disp_dir = 0 ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->pos = pl_w->pos ;
	if( !ew->disp_dir ){
		ew->pos.x += DOT_X( shl1c_06_pos[ew->type - 3].x ) ;
	} else {
		ew->pos.x += -( DOT_X( shl1c_06_pos[ew->type - 3].x ) ) ;
	}
	ew->pos.y += DOT_Y( shl1c_06_pos[ew->type - 3].y ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl1c_at_hyper_rock_missile ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 11 ) ;
	ew->dir = shl1c_06_dir_ofs( ew, pl_w ) ;
	SHL1C_06_SEQ( ew ) = shl1c_06_seq_tbl[ew->type - 3] ;
	ew->timer0 = 1 ;
	shl1c_06_set2( ew, 15 ) ;
	shl1c_06_missile_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_missile_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl1c_06_seq_step( ew, pl_w ) ;
	dir_spd_move( ew, 700 ) ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_set2( ew, Char_Effect, 11, ew->dir >> 1 & 15 ) ;
	hit_work_set( ew ) ;
}

static void shl1c_06_flame_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_06_flame_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_06_flame( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_06_flame_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_06_flame_init, shl1c_06_flame_move,
	} ;

	shl1c_06_flame_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_06_flame_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->disp_dir = 0 ;
	ew->sub_prio = 1 ;
	ew->pos = pl_w->pos ;
	ef_char_set( ew, 27, 43 ) ;
	shl1c_06_flame_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_flame_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	ew->prio = pl_w->prio ;
	ef_char_move( ew ) ;
	if( ew->cg_prm != pl_w->cg_prm ) ef_char_set2( ew, 27, 43, pl_w->cg_prm ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static Uint8 shl1c_06_dir_ofs( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 dir = 24 ;

	if( pl_w->disp_dir ) dir = 8 ;
	return dir ;
}

/*==============================================================*/
static void shl1c_06_seq_step( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint8 *sp ;
	Uint8 dir ;

	if( -- ew->timer0 ) return ;
	sp = SHL1C_06_SEQ( ew ) ;
	dir = *sp ++ << 1 ;
	if( pl_w->disp_dir ) dir = 32 - dir & 31 ;
	ew->dir = dir ;
	if( ( ew->timer0 = *sp ++ & 0xff ) == 255 ) return ;
	SHL1C_06_SEQ( ew ) = sp ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_06.c" End of File	*/
