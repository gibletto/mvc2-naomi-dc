/* Iron Man's effect 02 (character 33's shot 02): a bomb of his Smart Bomb, one for each
   sub_type (0 or 1). Type 0 flies from him along a path set by his atck_level, bursts on the floor
   or on a hit and ends; type 1 are the four trailing images that follow it. */
#include "player.h"
#include "pl_char.h"
#include "pl33_def.h"

/* where the bomb starts from Iron Man, x and y, by sub_type */
static Sint16 shl33_02_ofs_tbl[] = {
	6, 81, -51, 81,
} ;

/* its path, by sub_type * 9 and Iron Man's atck_level * 3, then: x speed, x acceleration, y
   acceleration, in 16.16 fixed point */
static Sint32 shl33_02_spd_tbl[] = {
	-0x48000, 0x1000, -0x3000, -0x68000, 0x1800, -0x3000, -0x88000, 0x2000, -0x3000,
	-0x50000, 0x1000, -0x2000, -0x70000, 0x1800, -0x2000, -0x90000, 0x2000, -0x2000,
} ;

/* the trail's last four places, which an image keeps in its speeds (spd and acc) */
typedef struct SHL33_02_TRAIL {
	Sint16	x[4];
	Sint16	y[4];
} SHL33_02_TRAIL;
#define SHL33_02_TRAIL_W( w )	( ( SHL33_02_TRAIL * )&( w )->spd )

static void shl33_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl33_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl33_02_main ;
		ew->ef_no = 0x3302 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

/*==============================================================*/
/*			An image that follows the bomb hw					*/
/*==============================================================*/
static void *shl33_02_part_set( PLAYER_WORK *hw, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl33_02_main ;
		ew->ef_no = 0x3302 ;
		ew->ef_w = hw ;
		ew->pl_type = hw->pl_type ;
		ew->master_w = hw->master_w ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl33_02_bomb_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_02_trail_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_02_main( PLAYER_WORK *ew )
{
	static void ( *shl33_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_02_bomb_main, shl33_02_trail_main,
	} ;

	shl33_02_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl33_02_bomb_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_02_bomb_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_02_bomb_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_02_bomb_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_02_bomb_init, shl33_02_bomb_move, shl33_02_end, shl33_02_free,
	} ;

	shl33_02_bomb_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_02_bomb_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )&pl_w->tlocal[0] ;

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
	ew->sub_prio = -2 ;
	lw->bomb_cnt ++ ;
	ew->atck_no = pl33_at_bomb ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( shl33_02_ofs_tbl[ew->sub_type * 2] ) ;
	else ew->pos.x -= DOT_X( shl33_02_ofs_tbl[ew->sub_type * 2] ) ;
	ew->pos.y += DOT_Y( shl33_02_ofs_tbl[ew->sub_type * 2 + 1] ) ;
	ew->spd.x = shl33_02_spd_tbl[pl_w->atck_level * 3 + ew->sub_type * 9] * 1.6666666f / 65536.0f ;
	ew->acc.x = shl33_02_spd_tbl[pl_w->atck_level * 3 + ew->sub_type * 9 + 1] * 1.6666666f / 65536.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = shl33_02_spd_tbl[pl_w->atck_level * 3 + ew->sub_type * 9 + 2] * 1.6666666f / 65536.0f ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	pl_char_set( ew, Char_Effect, 12 ) ;
	shl33_02_part_set( ew, 1, 0 ) ;
	shl33_02_part_set( ew, 1, 0 ) ;
	shl33_02_part_set( ew, 1, 0 ) ;
	shl33_02_part_set( ew, 1, 0 ) ;
	shl33_02_bomb_move( ew, pl_w ) ;
}

static void shl33_02_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_02_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl33_02_bomb_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_02_bomb_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_02_fall, shl33_02_burst, shl33_02_wait,
	} ;

	ew->prio = pl_w->prio ;
	shl33_02_bomb_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It flies on its path until the floor or a hit		*/
/*==============================================================*/
static void shl33_02_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->spd.x * ew->acc.x > 0.0f ){
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
	}
	if( pl_w->ground_y >= ew->pos.y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->timer0 = 12 ;
		return ;
	}
	if( ew->hit_box_no ){
		/* a hit on a body needs no test of its own */
	} else {
		hit_work_set( ew ) ;
		if( !ew->hit_flag ) return ;
	}
	ew->routine_1 ++ ;
	ew->timer0 = 1 ;
}

/*--------------------------------------------------------------*/
static void shl33_02_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		burst_set( ew, &ew->pos, 1, 8 ) ;
		pl_se_req( ew, 73 ) ;
		ew->timer0 = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void shl33_02_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )&pl_w->tlocal[0] ;

	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		lw->bomb_cnt -- ;
		shl33_02_end( ew, pl_w ) ;
	}
}

static void shl33_02_trail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_02_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_02_trail_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_02_trail_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_02_trail_init, shl33_02_trail_move, shl33_02_end, shl33_02_free,
	} ;

	shl33_02_trail_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_02_trail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *hw = ew->ef_w ;
	SHL33_02_TRAIL *trail = SHL33_02_TRAIL_W( ew ) ;
	int i ;

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
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.z = hw->pos.z ;
	for( i = 0 ; i < 4 ; i ++ ){
		trail->x[i] = ew->pos.x ;
		trail->y[i] = ew->pos.y ;
	}
	pl_char_set( ew, Char_Effect, 12 ) ;
	shl33_02_trail_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			It takes the place the image before it had four		*/
/*			frames ago											*/
/*==============================================================*/
static void shl33_02_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL33_02_TRAIL *trail = SHL33_02_TRAIL_W( ew ) ;
	PLAYER_WORK *bw = ew->before_w ;
	PLAYER_WORK *hw = ew->ef_w ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x = trail->x[3] ;
	trail->x[3] = trail->x[2] ;
	trail->x[2] = trail->x[1] ;
	trail->x[1] = trail->x[0] ;
	trail->x[0] = bw->pos.x ;
	ew->pos.y = trail->y[3] ;
	trail->y[3] = trail->y[2] ;
	trail->y[2] = trail->y[1] ;
	trail->y[1] = trail->y[0] ;
	trail->y[0] = bw->pos.y ;
	if( hw->routine_1 == 2 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl33_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl33_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl33_02.c" End of File	*/
