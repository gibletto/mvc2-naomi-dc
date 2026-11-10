/* Mega Man's player effect 00: the pictures that ride on Mega Man and Roll in their moves,
   fifty-seven kinds. Each keeps to the player's place and plays an animation of group 27. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl1c_def.h"

static void pls1c_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1c_00_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1c_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
	}
	return ew ;
}

/****************************************************************/
/*			The same, riding on another effect					*/
/****************************************************************/
PLAYER_WORK *pls1c_00_set2( PLAYER_WORK *pl_w, PLAYER_WORK *ef_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( ef_w, W_PLS, LIST_BEHIND ) ) ){
		ew->work_func = pls1c_00_main ;
		ew->master_w = pl_w ;
		ew->ef_w = ef_w ;
		ew->type = type ;
	}
	return ew ;
}

/* each kind's look: where it rides (x, y from the player) and the animation of group 27 */
typedef struct PLS1C_00_DATA {
	Uint8	col_flag ;
	Sint8	col_no ;
	Sint16	sub_prio ;
	Sint16	x ;
	Sint16	y ;
	Uint8	grp ;
	Uint8	no ;
} PLS1C_00_DATA ;

static PLS1C_00_DATA pls1c_00_data[57] = {
	{ 0, 0, -1, -24, -46, 27, 3 },
	{ 0, 0, -1, -24, -34, 27, 3 },
	{ 0, 0, -1, -29, -68, 27, 3 },
	{ 0, 0, -1, -24, -46, 27, 4 },
	{ 0, 0, -1, -24, -34, 27, 4 },
	{ 0, 0, -1, -29, -68, 27, 4 },
	{ 0, 0, -1, 2, -22, 27, 0 },
	{ 1, 5, 0, 0, 0, 27, 36 },
	{ 0, 0, -1, 0, 0, 27, 5 },
	{ 0, 0, -1, 0, 0, 27, 17 },
	{ 0, 0, -2, -24, -181, 27, 18 },
	{ 0, 0, -2, -67, -159, 27, 19 },
	{ 0, 0, -2, -79, -134, 27, 20 },
	{ 0, 0, -2, -70, -104, 27, 21 },
	{ 0, 0, -2, -85, -24, 27, 22 },
	{ 0, 0, -2, -83, -11, 27, 23 },
	{ 0, 0, -2, -51, -3, 27, 24 },
	{ 0, 0, -2, 67, 6, 27, 25 },
	{ 0, 0, -2, -51, -5, 27, 26 },
	{ 0, 0, -2, 89, -3, 27, 27 },
	{ 0, 0, -2, 94, -17, 27, 28 },
	{ 0, 0, -2, 126, -107, 27, 29 },
	{ 0, 0, -2, 119, -153, 27, 30 },
	{ 0, 0, -2, 111, -174, 27, 31 },
	{ 0, 0, -2, 75, -165, 27, 32 },
	{ 0, 0, -2, 35, -181, 27, 33 },
	{ 0, 0, -1, -4, -44, 27, 34 },
	{ 1, -2, -1, -35, -39, 27, 4 },
	{ 1, -2, -1, -41, -47, 27, 4 },
	{ 0, 0, -1, -4, -44, 27, 34 },
	{ 0, 0, -1, 0, 0, 27, 17 },
	{ 0, 0, -2, -14, -143, 27, 18 },
	{ 0, 0, -2, -48, -116, 27, 19 },
	{ 0, 0, -2, -47, -102, 27, 20 },
	{ 0, 0, -2, -43, -87, 27, 21 },
	{ 0, 0, -2, -33, -51, 27, 22 },
	{ 0, 0, -2, -49, 6, 27, 23 },
	{ 0, 0, -2, -24, 10, 27, 24 },
	{ 0, 0, -2, 39, 10, 27, 25 },
	{ 0, 0, -2, 51, 11, 27, 26 },
	{ 0, 0, -2, 62, 4, 27, 27 },
	{ 0, 0, -2, 29, -66, 27, 27 },
	{ 0, 0, -2, 46, -93, 27, 28 },
	{ 0, 0, -2, 35, -118, 27, 29 },
	{ 0, 0, -2, 33, -130, 27, 30 },
	{ 0, 0, -2, 16, -141, 27, 31 },
	{ 0, 0, -2, 14, -142, 27, 32 },
	{ 0, 0, 0, 0, 0, 27, 13 },
	{ 0, 0, 0, 0, 0, 27, 14 },
	{ 0, 0, 0, 0, 0, 27, 15 },
	{ 1, 5, 0, 0, 0, 27, 36 },
	{ 1, 5, 0, 0, 0, 27, 36 },
	{ 0, 0, -1, 0, 0, 27, 42 },
	{ 0, 0, 1, 0, 0, 27, 38 },
	{ 0, 0, 2, 0, 0, 27, 39 },
	{ 0, 0, -1, 0, 0, 27, 40 },
	{ 0, 0, 3, 0, 0, 27, 41 },
} ;

static void pls1c_00_init( PLAYER_WORK *ew );
static void pls1c_00_move( PLAYER_WORK *ew );
static void pls1c_00_end( PLAYER_WORK *ew );
static void pls1c_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1c_00_main( PLAYER_WORK *ew )
{
	static void ( *pls1c_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls1c_00_init, pls1c_00_move, pls1c_00_end, pls1c_00_free,
	} ;

	pls1c_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls1c_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS1C_00_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c00 ;
	ew->disp_flag = 1 ;
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
	dp = &pls1c_00_data[ew->type] ;
	if( dp->col_flag ){
		ew->col_flag = dp->col_flag ;
		ew->col_no += dp->col_no ;
	}
	ew->sub_prio = dp->sub_prio ;
	ew->spd.x = DOT_X( dp->x ) ;
	ew->spd.y = -DOT_Y( dp->y ) ;
	ef_char_set( ew, dp->grp, dp->no ) ;
	pls1c_00_move( ew ) ;
}

static void pls1c_00_t0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t4( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t8( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t10( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls1c_00_move( PLAYER_WORK *ew )
{
	static void ( *pls1c_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_00_t1, pls1c_00_t1, pls1c_00_t1, pls1c_00_t1, pls1c_00_t1, pls1c_00_t1,
		pls1c_00_t2, pls1c_00_t4, pls1c_00_t3, pls1c_00_t5, pls1c_00_t6, pls1c_00_t6,
		pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6,
		pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6,
		pls1c_00_t6, pls1c_00_t6, pls1c_00_t0, pls1c_00_t1, pls1c_00_t1, pls1c_00_t0,
		pls1c_00_t5, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6,
		pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6,
		pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t6, pls1c_00_t7,
		pls1c_00_t7, pls1c_00_t7, pls1c_00_t4, pls1c_00_t4, pls1c_00_t8, pls1c_00_t9,
		pls1c_00_t9, pls1c_00_t9, pls1c_00_t10,
	} ;

	pls1c_00_move_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls1c_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_pos_set2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_00_t0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls1c_00_t1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = 8 ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls1c_00_t2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	if( !pl_w->cg_type ){
		ew->routine_0 ++ ;
	} else if( pl_w->cg_type < 0 ){
		ew->disp_flag = 1 ;
	}
}

/*==============================================================*/
static void pls1c_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ef_char_move( ew ) ;
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
}

static void pls1c_00_t4_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t4_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_00_t4( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_00_t4_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_00_t4_init, pls1c_00_t4_move,
	} ;

	pls1c_00_t4_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_00_t4_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->timer0 = 0 ;
	ew->prio = 15 ;
	pls1c_00_t4_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_00_t4_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls1c_00_pos_set2( ew, pl_w ) ;
	ef_char_move( ew ) ;
	if( pl_w->cg_type == 0 ){
		ew->routine_0 ++ ;
	}
	ew->disp_flag = 1 ;
	if( !( ++ew->timer0 % 4 ) ){
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls1c_00_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	if( !pl_w->cg_type ){
		ew->routine_0 ++ ;
	} else {
		ew->disp_flag = 0 ;
		if( ew->dir == 0 ){
			ew->disp_flag = 1 ;
		}
	}
}

/*==============================================================*/
static void pls1c_00_t6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
	} else if( pl_w->dir == 0 ){
		ew->disp_flag = 1 ;
	}
}

static void pls1c_00_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t7_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls1c_00_t7_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls1c_00_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls1c_00_t7_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls1c_00_t7_init, pls1c_00_t7_move, pls1c_00_t7_fall,
	} ;

	pls1c_00_t7_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls1c_00_t7_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->prio = 8 ;
	ew->size_top = 48 ;
	ew->size_bottom = 48 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	pls1c_00_pos_set( ew, pl_w ) ;
	pls1c_00_t7_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls1c_00_t7_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float spd ;

	if( ef_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		if( !pl_w->obj_flag ){
			spd = -DOT_X( 2.0f ) ;
		} else {
			spd = DOT_X( 2.0f ) ;
		}
		ew->spd.x = spd ;
		ew->acc.x = spd ;
	}
}

/*--------------------------------------------------------------*/
static void pls1c_00_t7_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
	}
}

/*==============================================================*/
static void pls1c_00_t8( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( ( ew->disp_dir = pl_w->disp_dir ^ 1 ) ){
		ew->pos.x += ew->spd.x ;
	} else {
		ew->pos.x -= ew->spd.x ;
	}
	ew->pos.y += ew->spd.y ;
	ew->prio = pl_w->prio ;
	if( !pl_w->cg_type ){
		ew->routine_0 ++ ;
	} else {
		ef_char_set2( ew, 27, 42, pl_w->cg_type & 15 ) ;
	}
}

/*==============================================================*/
static void pls1c_00_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->col_flag = pl_w->master_w->col_flag ;
	ew->col_no = pl_w->master_w->col_no ;
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ef_char_move( ew ) ;
}

/*==============================================================*/
static void pls1c_00_t10( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->col_flag = pl_w->master_w->col_flag ;
	ew->col_no = pl_w->master_w->col_no ;
	pls1c_00_pos_set( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
}

/*--------------------------------------------------------------*/
static void pls1c_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1c_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Put the effect at the master's place, moved by		*/
/*			its speed to the side it faces						*/
/*==============================================================*/
static void pls1c_00_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x += ew->spd.x ;
	} else {
		ew->pos.x -= ew->spd.x ;
	}
	ew->pos.y += ew->spd.y ;
}

/*==============================================================*/
/*			The same, for the master's own side					*/
/*==============================================================*/
static void pls1c_00_pos_set2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos = pl_w->pos ;
	if( !pl_w->disp_dir ){
		ew->pos.x += ew->spd.x ;
	} else {
		ew->pos.x -= ew->spd.x ;
	}
	ew->pos.y += ew->spd.y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1c_00.c" End of File	*/
