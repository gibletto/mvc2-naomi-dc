/* Iceman's effect 02 (character 09's shot 02): a block of ice that drops from above him, with a
   trail of pieces, and breaks into shards when it lands or is hit. */
#include "player.h"
#include "pl_char.h"

static void shl09_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl09_02_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl09_02_main ;
		ew->type = 0 ;
		ew->sub_type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x902 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The trail, seventeen pieces on the block (ef_w)		*/
/*==============================================================*/
static void shl09_02_trail_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pw ;
	Sint8 i ;

	for( i = 0 ; i <= 16 ; i ++ ){
		if( ( pw = work_get( 0, W_SHL, LIST_END ) ) ){
			pw->work_func = shl09_02_main ;
			pw->type = 1 ;
			pw->sub_type = i ;
			pw->master_w = ew->master_w ;
			pw->ef_w = ew ;
			pw->pl_type = ew->pl_type ;
			pw->ef_no = 0x901 ;
		}
	}
}

/*==============================================================*/
/*			The shards, eight of them							*/
/*==============================================================*/
static void shl09_02_shard_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pw ;
	Sint8 i ;

	for( i = 0 ; i <= 7 ; i ++ ){
		if( ( pw = work_get( 0, W_SHL, LIST_END ) ) ){
			pw->work_func = shl09_02_main ;
			pw->type = 2 ;
			pw->sub_type = i ;
			pw->master_w = ew->master_w ;
			pw->ef_w = ew ;
			pw->pl_type = ew->pl_type ;
			pw->ef_no = 0x901 ;
			pw->routine_3 = ew->routine_3 ;
		}
	}
}

static Sint16 shl09_02_trail_ofs[32] = {	/* a trail piece's place, x and y */
	-64, -48, -80, -40, -88, 48, 56, -48, -56, 72, 72, 56, 64, 64, 32, -72,
	-48, 64, -40, -80, 48, -88, -48, 56, 72, -56, 56, 72, 48, 48, -72, -32,
} ;
static Sint16 shl09_02_shard_ofs[16] = {	/* a shard's place, x and y */
	-30, -10, -20, 10, -18, -8, 0, -10, 10, -12, 5, -2, 30, -10, 20, -1,
} ;
static Float shl09_02_shard_spd[8][4] = {	/* a shard's speed and its change, x and y */
	{ -5.0f, 0.104166664f, DOT_Y( 4.0f ), -DOT_Y( 0.125f ) }, { -DOT_X( 2.5f ), 0.104166664f, 7.5f, -DOT_Y( 0.125f ) },
	{ 1.6666666f, 0.052083332f, DOT_Y( 5.0f ), -DOT_Y( 0.125f ) }, { DOT_X( 0.5f ), 0.0f, DOT_Y( 2.0f ), -0.13392857f },
	{ DOT_X( 2.0f ), 0.104166664f, DOT_Y( 5.0f ), -DOT_Y( 0.125f ) }, { DOT_X( 2.0f ), 0.104166664f, DOT_Y( 4.0f ), -DOT_Y( 0.125f ) },
	{ 5.0f, 0.104166664f, DOT_Y( 4.0f ), -DOT_Y( 0.125f ) }, { 4.6875f, 0.104166664f, DOT_Y( 2.0f ), -0.13392857f },
} ;

static void shl09_02_init( PLAYER_WORK *ew );
static void shl09_02_move( PLAYER_WORK *ew );
static void shl09_02_end( PLAYER_WORK *ew );
static void shl09_02_free( PLAYER_WORK *ew );
static void shl09_02_ck( PLAYER_WORK *ew );

/*==============================================================*/
static void shl09_02_main( PLAYER_WORK *ew )
{
	static void ( *shl09_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_02_init, shl09_02_move, shl09_02_end, shl09_02_free,
	} ;

	shl09_02_ck( ew ) ;
	shl09_02_jmp[ew->routine_0]( ew ) ;
}

static void shl09_02_block_init( PLAYER_WORK *ew );
static void shl09_02_trail_init( PLAYER_WORK *ew );
static void shl09_02_shard_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl09_02_init( PLAYER_WORK *ew )
{
	static void ( *shl09_02_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_02_block_init, shl09_02_trail_init, shl09_02_shard_init,
	} ;

	shl09_02_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl09_02_block_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 1 ;
	ew->prio = 11 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->atck_no = ew->sub_type + 48 ;
	HIT_RESET( ew ) ;
	ew->timer0 = 180 ;
	ew->timer1 = 20 ;
	ew->routine_3 = 0 ;
	ew->pos.x += ew->disp_dir ? DOT_X( 176.0f ) : -DOT_X( 176.0f ) ;
	ew->pos.y += DOT_Y( 192.0f ) ;
	if( ew->sub_type & 1 ){
		ew->spd.x = ew->disp_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
		if( ew->disp_dir ) ew->acc.x = -0.104166664f ;
		else ew->acc.x = 0.104166664f ;
	} else {
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
	}
	ew->spd.y = -DOT_Y( 5.0f ) ;
	ew->acc.y = -DOT_Y( 0.125f ) ;
	pl_char_set( ew, 21, 1 ) ;
	shl09_02_trail_set( ew ) ;
}

/*==============================================================*/
static void shl09_02_trail_init( PLAYER_WORK *ew )
{
	Float x, y, old ;

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
	ew->disp_flag = 1 ;
	ew->prio = 10 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	pl_char_set( ew, 21, ( Sint32 )Rnd() % 3 + 2 ) ;
	x = DOT_X( shl09_02_trail_ofs[ew->sub_type * 2] ) ;
	if( ew->disp_dir ) x = -x ;
	old = ew->pos.x ;
	ew->pos.x += x ;
	ew->spd.x = ( old - ew->pos.x ) / 32.0f ;
	ew->acc.x = 0.0f ;
	y = DOT_Y( shl09_02_trail_ofs[ew->sub_type * 2 + 1] ) ;
	old = ew->pos.y ;
	ew->pos.y += y ;
	ew->spd.y = ( old - ew->pos.y ) / 32.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 16 ;
}

/*==============================================================*/
static void shl09_02_shard_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x, acc, add ;

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
	ew->disp_flag = 1 ;
	if( ew->sub_type & 1 ) ew->disp_flag = 0 ;
	ew->prio = 10 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	pl_char_set( ew, 21, ( Sint32 )Rnd() % 3 + 2 ) ;
	ew->timer0 = 30 ;
	x = DOT_X( shl09_02_shard_ofs[ew->sub_type * 2] ) ;
	if( ew->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( shl09_02_shard_ofs[ew->sub_type * 2 + 1] ) ;
	x = shl09_02_shard_spd[ew->sub_type][0] ;
	acc = shl09_02_shard_spd[ew->sub_type][1] ;
	if( ew->disp_dir ){
		x = -x ;
		acc = -acc ;
	}
	ew->spd.x = x ;
	ew->acc.x = acc ;
	ew->spd.y = shl09_02_shard_spd[ew->sub_type][2] ;
	ew->acc.y = shl09_02_shard_spd[ew->sub_type][3] ;
	if( !ew->routine_3 ){
		add = DOT_X( 4.0f ) ;
		if( pl_w->pl_dir ) add = -DOT_X( 4.0f ) ;
		ew->spd.x += add ;
	}
}

static void shl09_02_block( PLAYER_WORK *ew );
static void shl09_02_trail( PLAYER_WORK *ew );
static void shl09_02_shard( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl09_02_move( PLAYER_WORK *ew )
{
	static void ( *shl09_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_02_block, shl09_02_trail, shl09_02_shard,
	} ;

	shl09_02_move_jmp[ew->type]( ew ) ;
}

static void shl09_02_wait( PLAYER_WORK *ew );
static void shl09_02_fall( PLAYER_WORK *ew );
static void shl09_02_blink( PLAYER_WORK *ew );

/*==============================================================*/
static void shl09_02_block( PLAYER_WORK *ew )
{
	static void ( *shl09_02_block_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_02_wait, shl09_02_fall, shl09_02_blink,
	} ;

	shl09_02_block_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			It waits above him, then starts to fall				*/
/*==============================================================*/
static void shl09_02_wait( PLAYER_WORK *ew )
{
	if( --ew->timer1 <= 0 ){
		ew->routine_1 ++ ;
		ew->be_flag = 1 ;
		pl_shadow_set( ew ) ;
	}
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void shl09_02_break( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	pl_voice_req( ew, 31 ) ;
	shl09_02_shard_set( ew ) ;
	ew->timer1 = 10 ;
}

/*--------------------------------------------------------------*/
static void shl09_02_fall( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	VEC3 pos ;

	if( ew->hit_stop > 0 ){
		ew->hit_stop -- ;
		return ;
	}
	if( ew->hit_box_no ){
		shl09_02_break( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( --ew->timer0 <= 0 ){
		ew->routine_0 ++ ;
		shl09_02_end( ew ) ;
		return ;
	}
	if( ew->pos.y <= pl_w->ground_y ){
		ew->pos.y = pl_w->ground_y ;
		pos.x = ew->pos.x ;
		pos.y = ew->pos.y ;
		pos.z = ew->pos.z ;
		land_dust_set( &pos, -1 ) ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
		ew->routine_3 = 1 ;
		shl09_02_break( ew ) ;
		ew->be_flag = 0 ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Broken: it blinks out								*/
/*==============================================================*/
static void shl09_02_blink( PLAYER_WORK *ew )
{
	if( --ew->timer1 ){
		ew->disp_flag ^= 1 ;
		return ;
	}
	ew->routine_0 ++ ;
	shl09_02_end( ew ) ;
}

/*==============================================================*/
static void shl09_02_trail( PLAYER_WORK *ew )
{
	if( --ew->timer0 <= 0 ){
		shl09_02_end( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*==============================================================*/
static void shl09_02_shard( PLAYER_WORK *ew )
{
	if( --ew->timer0 <= 0 ){
		shl09_02_end( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	ew->disp_flag ^= 1 ;
}

/*--------------------------------------------------------------*/
static void shl09_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	shl09_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl09_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl09_02_ck( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl09_02.c" End of File	*/
