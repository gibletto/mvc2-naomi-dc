/* Doctor Doom's Photon Shot and Photon Array (character 0f's shot 02): one of a fan of shots, each set at
   its own place around him and flying in its own direction. */
#include "player.h"
#include "pl_char.h"
#include "pl0f_def.h"

/* the effect's own work */
typedef struct SHL0F_02_LOCAL_W {
	Uint16	char_no;	/* 000: Doom's animation when it was set */
} SHL0F_02_LOCAL_W;
#define SHL0F_02_LW( w )	( ( SHL0F_02_LOCAL_W * )&( w )->obj_flag )

/* by the shot's number: its animation, its place from Doom (x, y), and its direction */
Uint8 shl0f_02_char_tbl[15] = { 7, 9, 10, 14, 12, 5, 7, 8, 9, 10, 5, 6, 7, 8, 9 } ;
Sint16 shl0f_02_ofs[15 * 2] = {
	-89, 47, -103, 63, -106, 74, -100, 87, -76, 105,
	-1, 116, 30, 112, 36, 104, 34, 92, 34, 81,
	-18, 9, -33, 11, -52, 21, -67, 32, -66, 54,
} ;
Uint8 shl0f_02_dir[15] = { 19, 23, 24, 27, 29, 2, 5, 6, 7, 8, 18, 19, 20, 21, 23 } ;

static void shl0f_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0f_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	SHL0F_02_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0f_02_main ;
		ew->ef_no = 0x0f01 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		lw = SHL0F_02_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl0f_02_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_02_main( PLAYER_WORK *ew )
{
	static void ( *shl0f_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_02_0_main, shl0f_02_0_main,
	} ;

	shl0f_02_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0f_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_02_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_02_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_02_init, shl0f_02_move, shl0f_02_end, shl0f_02_free,
	} ;

	shl0f_02_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;

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
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 16 ;
	if( !ew->type ){
		ew->atck_no = pl0f_at_photon ;
	} else {
		ew->atck_no = pl0f_at_array ;
	}
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->pos.y += DOT_Y( shl0f_02_ofs[ew->sub_type * 2 + 1] ) ;
	x = DOT_X( shl0f_02_ofs[ew->sub_type * 2] ) ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->dir = shl0f_02_dir[ew->sub_type] ;
	if( ew->disp_dir ){
		ew->dir = 32 - ew->dir ;
		ew->dir &= 31 ;
	}
	pl_char_set( ew, Char_Effect, shl0f_02_char_tbl[ew->sub_type] ) ;
	shl0f_02_move( ew, pl_w ) ;
}

static void shl0f_02_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_02_fly,
	} ;

	ew->prio = pl_w->prio ;
	if( !scr_in_x_ck( ew ) || pl_w->ground_y > ew->pos.y ){
		ew->routine_0 ++ ;
		return ;
	}
	shl0f_02_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_02_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	dir_spd_move( ew, 700 ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl0f_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0f_02.c" End of File	*/
