/* Hulk's Gamma Quake rocks (character 0d's shot 02): a work that drops rocks while Hulk holds the
   pose (type 0), the rocks, which come down from the top of the screen (type 1), and the shards
   they leave where they land (type 2). */
#include "player.h"
#include "pl_char.h"
#include "pl0d_def.h"

Uint8 shl0d_02_rock_char[8] = { 21, 22, 23, 24, 25, 26, 23, 25 } ;
Uint8 shl0d_02_shard_char[8] = { 28, 28, 29, 30, 28, 29, 30, 30 } ;
Sint16 shl0d_02_shard_spd[4 * 2] = {
	256, 1536,
	-512, 1280,
	512, 1024,
	-256, 768,
} ;

static void shl0d_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0d_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0d_02_main ;
		ew->ef_no = 0x0d02 ;
		ew->master_w = pl_w ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->pl_type = pl_w->pl_type ;
	}
	return ew ;
}

/*==============================================================*/
/*			A rock or a shard, thrown off by the work before	*/
/*			it													*/
/*==============================================================*/
static void *shl0d_02_part_set( PLAYER_WORK *wk, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0d_02_main ;
		ew->ef_no = 0x0d02 ;
		ew->master_w = wk->master_w ;
		ew->ef_w = wk ;
		ew->type = a ;
		ew->sub_type = b ;
		ew->pl_type = wk->pl_type ;
	}
	return ew ;
}

static void shl0d_02_quake_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_02_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_02_shard_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_02_main( PLAYER_WORK *ew )
{
	static void ( *shl0d_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_02_quake_main, shl0d_02_rock_main, shl0d_02_shard_main,
	} ;

	shl0d_02_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0d_02_quake_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_02_quake_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_02_quake_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_02_quake_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_02_quake_init, shl0d_02_quake_move, shl0d_02_end, shl0d_02_free,
	} ;

	shl0d_02_quake_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_02_quake_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->timer0 = 1 ;
	ew->timer1 = 48 ;
	shl0d_02_quake_move( ew, pl_w ) ;
}

static void shl0d_02_quake_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_02_quake_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_02_quake_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_02_quake_drop,
	} ;

	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	shl0d_02_quake_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_02_quake_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->timer0 = 3 ;
		shl0d_02_part_set( ew, 1, ew->timer1 ) ;
	}
	if( !-- ew->timer1 ){
		ew->routine_0 ++ ;
		shl0d_02_end( ew, pl_w ) ;
	}
}

static void shl0d_02_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_02_rock_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_02_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_02_rock_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_02_rock_init, shl0d_02_rock_move, shl0d_02_end, shl0d_02_free,
	} ;

	shl0d_02_rock_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_02_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *sp_w = ew->ef_w ;
	Sint16 ofs ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->atck_no = pl0d_at_quake_rock ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = sp_w->pos.x ;
	ew->pos.y = scr_work.scr_top ;
	ew->pos.z = sp_w->pos.z ;
	ofs = Rnd() & 255 ;
	ew->pos.x += ( 128 - ofs ) * 1.6666666f ;
	ofs = Rnd() & 127 ;
	ew->pos.y += ( 64 - ofs ) * 2.142857f ;
	ofs = -106 ;
	if( ew->disp_dir ){
		ofs = 106 ;
	}
	ew->pos.x += ofs ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.625f ) ;
	pl_char_set( ew, Char_Effect, shl0d_02_rock_char[Rnd() & 7] ) ;
	shl0d_02_rock_move( ew, pl_w ) ;
}

static void shl0d_02_rock_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_02_rock_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_02_rock_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_02_rock_fall,
	} ;

	ew->prio = pl_w->prio ;
	shl0d_02_rock_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_02_rock_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( pl_w->ground_y >= ew->pos.y ){
		ew->pos.y = pl_w->ground_y ;
		if( !( ew->sub_type & 15 ) ){
			pl_voice_req( ew, 30 ) ;
		}
		shl0d_02_part_set( ew, 2, 0 ) ;
		ew->routine_0 ++ ;
		shl0d_02_end( ew, pl_w ) ;
	}
}

static void shl0d_02_shard_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_02_shard_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_02_shard_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_02_shard_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_02_shard_init, shl0d_02_shard_move, shl0d_02_end, shl0d_02_free,
	} ;

	shl0d_02_shard_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_02_shard_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *rock_w = ew->ef_w ;
	Sint16 *dt ;
	Uint8 no ;
	int x, y ;

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
	ew->sub_prio = -2 ;
	ew->disp_flag = 1 ;
	ew->pos.x = rock_w->pos.x ;
	ew->pos.y = rock_w->pos.y ;
	ew->pos.z = rock_w->pos.z ;
	no = Rnd() & 3 ;
	dt = &shl0d_02_shard_spd[no * 2] ;
	x = dt[0] << 8 ;
	y = dt[1] << 8 ;
	ew->spd.x = x * 1.6666666f / 65536.0f ;
	ew->spd.y = y * 2.142857f / 65536.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = -0.4017857f ;
	no = Rnd() & 7 ;
	pl_char_set( ew, Char_Effect, shl0d_02_shard_char[no] ) ;
	shl0d_02_shard_move( ew, pl_w ) ;
}

static void shl0d_02_shard_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_02_shard_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_02_shard_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_02_shard_fly,
	} ;

	ew->prio = pl_w->prio ;
	shl0d_02_shard_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_02_shard_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( pl_w->ground_y >= ew->pos.y ){
		ew->routine_0 ++ ;
		shl0d_02_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl0d_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0d_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0d_02.c" End of File	*/
