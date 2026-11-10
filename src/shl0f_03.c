/* Doctor Doom's Plasma Beam (character 0f's shot 03): the flash on his hand (kind 0), and the shot it
   fires (kind 1), both while he holds the pose. */
#include "player.h"
#include "pl_char.h"
#include "pl0f_def.h"

/* the effect's own work */
typedef struct SHL0F_03_LOCAL_W {
	Uint16	char_no;	/* 000: Doom's animation when it was set */
} SHL0F_03_LOCAL_W;
#define SHL0F_03_LW( w )	( ( SHL0F_03_LOCAL_W * )&( w )->obj_flag )

/* on the ground and in the air: the flash's place from Doom (x, y), the shot's direction, and the
   shot's place */
Sint16 shl0f_03_spd[2 * 2] = { -31, 85, -21, 44 } ;
Uint8 shl0f_03_dir[2] = { 24, 19 } ;
Sint16 shl0f_03_ofs[2 * 2] = { -52, 87, -25, 48 } ;

static void shl0f_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0f_03_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	SHL0F_03_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0f_03_main ;
		ew->ef_no = 0x0f00 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		lw = SHL0F_03_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl0f_03_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_03_1_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_03_main( PLAYER_WORK *ew )
{
	static void ( *shl0f_03_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_03_0_main, shl0f_03_1_main,
	} ;

	shl0f_03_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0f_03_0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_03_0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_03_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_03_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_03_0_init, shl0f_03_0_move, shl0f_03_end, shl0f_03_free,
	} ;

	shl0f_03_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_03_0_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = DOT_X( shl0f_03_spd[ew->sub_type * 2] ) ;
	ew->spd.y = DOT_Y( shl0f_03_spd[ew->sub_type * 2 + 1] ) ;
	if( pl_w->disp_dir ) ew->spd.x = -ew->spd.x ;
	if( !ew->sub_type ){
		ew->char_no.b.no = 24 ;
	} else {
		ew->char_no.b.no = 16 ;
	}
	pl_char_set( ew, Char_Effect, ew->char_no.b.no ) ;
	shl0f_03_0_move( ew, pl_w ) ;
}

static void shl0f_03_0_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The flash keeps its place on Doom's hand, by spd	*/
/*==============================================================*/
static void shl0f_03_0_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_03_0_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_03_0_wait,
	} ;
	SHL0F_03_LOCAL_W *lw = SHL0F_03_LW( ew ) ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	shl0f_03_0_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_03_0_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

static void shl0f_03_1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_03_1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_03_1_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_03_1_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_03_1_init, shl0f_03_1_move, shl0f_03_end, shl0f_03_free,
	} ;

	shl0f_03_1_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_03_1_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	x = DOT_X( shl0f_03_ofs[ew->sub_type * 2] ) ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( shl0f_03_ofs[ew->sub_type * 2 + 1] ) ;
	ew->dir = shl0f_03_dir[ew->sub_type] ;
	if( ew->disp_dir ){
		ew->dir = 32 - ew->dir ;
		ew->dir &= 31 ;
	}
	if( !ew->sub_type ){
		ew->atck_no = pl_w->atck_level + 50 ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl_w->atck_level + 52 ;
		HIT_RESET( ew ) ;
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 23 ) ;
	shl0f_03_1_move( ew, pl_w ) ;
}

static void shl0f_03_1_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_03_1_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_03_1_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_03_1_fly,
	} ;
	SHL0F_03_LOCAL_W *lw = SHL0F_03_LW( ew ) ;

	if( pl_w->char_no.w != lw->char_no ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !scr_in_x_ck( ew ) || pl_w->ground_y > ew->pos.y ){
		ew->routine_0 ++ ;
		return ;
	}
	shl0f_03_1_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_03_1_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	dir_spd_move( ew, 1600 ) ;
	hit_work_set( ew ) ;
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl0f_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0f_03.c" End of File	*/
