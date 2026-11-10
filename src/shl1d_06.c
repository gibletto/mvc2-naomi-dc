/* Roll's effect 06 (character 1d's shot 06): what she throws in her Hyper Roll, five kinds picked
   by the setter, in three ways of flying: straight, in a drop that lands, and a short throw. */
#include "player.h"
#include "pl_char.h"
#include "pl1d_def.h"

/* one kind's start: its speed (16.16 fixed point), its place from Roll, its animation and its
   attack */
typedef struct SHL1D_06_DATA {
	Sint32	spd_x;
	Sint32	spd_y;
	Sint16	x;
	Sint16	y;
	Uint8	char_no;
	Uint8	atck_no;
} SHL1D_06_DATA;

static void shl1d_06_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1d_06_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1d_06_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static SHL1D_06_DATA shl1d_06_data[5] = {
	{ -10 << 16, 0, -13, 34, 17, 64 },
	{ -10 << 16, 0, 7, 81, 15, 65 },
	{ -10 << 16, 0, 23, 31, 13, 66 },
	{ -0x78000, 0, -7, 96, 11, 67 },
	{ -0x78000, 0, -20, 99, 11, 67 },
} ;

static void shl1d_06_init( PLAYER_WORK *ew );
static void shl1d_06_move( PLAYER_WORK *ew );
static void shl1d_06_end( PLAYER_WORK *ew );
static void shl1d_06_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1d_06_main( PLAYER_WORK *ew )
{
	static void ( *shl1d_06_jmp[] )( PLAYER_WORK *ew ) = {
		shl1d_06_init, shl1d_06_move, shl1d_06_end, shl1d_06_free,
	} ;

	shl1d_06_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1d_06_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	SHL1D_06_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1d06 ;
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
	ew->prio = 8 ;
	ew->disp_flag = 1 ;
	dp = &shl1d_06_data[ew->type] ;
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
	shl1d_06_move( ew ) ;
}

static void shl1d_06_straight( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_short( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1d_06_move( PLAYER_WORK *ew )
{
	static void ( *shl1d_06_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1d_06_straight, shl1d_06_straight, shl1d_06_drop, shl1d_06_short, shl1d_06_short,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1d_06_move_jmp[ew->type]( ew, pl_w ) ;
}

static void shl1d_06_straight_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_straight_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_straight_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1d_06_straight( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1d_06_straight_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1d_06_straight_init, shl1d_06_straight_start, shl1d_06_straight_fly,
	} ;

	shl1d_06_straight_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1d_06_straight_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	shl1d_06_straight_start( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1d_06_straight_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void shl1d_06_straight_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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

static void shl1d_06_drop_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_drop_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_drop_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_drop_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1d_06_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1d_06_drop_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1d_06_drop_init, shl1d_06_drop_start, shl1d_06_drop_fall, shl1d_06_drop_fly,
	} ;

	shl1d_06_drop_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1d_06_drop_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->size_top = 24 ;
	ew->size_bottom = 24 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	shl1d_06_drop_start( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1d_06_drop_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void shl1d_06_drop_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void shl1d_06_drop_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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

static void shl1d_06_short_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_06_short_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1d_06_short( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1d_06_short_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1d_06_short_init, shl1d_06_short_fly,
	} ;

	shl1d_06_short_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1d_06_short_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	shl1d_06_short_fly( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1d_06_short_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1d_06_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1d_06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1d_06.c" End of File	*/
