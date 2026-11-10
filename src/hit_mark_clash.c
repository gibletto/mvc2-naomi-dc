/* The mark of two attacks that strike each other, or of a fighter set down hard (kind 8 works;
   pl_dmg.c's clash_mark_set and pl_main.c make it through clash_ef_set): six pieces
   made together at the spot: a star that shrinks, four that wait a frame or two, spin and fade, and a
   ring that grows and fades. */
#include "player.h"

static void clash_hide( PLAYER_WORK *ew ) ;
static void clash_end( PLAYER_WORK *ew ) ;

/* the turn about y of the last piece, by the side the blow came from */
static const Sint32 clash5_rot_tbl[2] = { 0xa000, 0xe001 } ;
/* the alpha of the first piece, by frame */
static const Float clash0_fade_tbl[14] = { 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.0f, 0.0f, 0.0f, } ;
/* the alpha of the four that wait (its seventh frame reads the next table's first) */
static const Float clash_fade_tbl[6] = { 0.0f, 0.5f, 1.0f, 1.0f, 0.5f, 0.0f, } ;
/* the alpha of the last piece */
static const Float clash5_fade_tbl[15] = { 0.0f, 0.25f, 0.5f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.75f, 0.5f, 0.25f, 0.0f, } ;

static void clash0_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash0_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = clash0_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->obj = obj_efky->obj[31] ;
		ew->pos = *pos ;
		ew->rot[2] = 0 ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

static void clash0_start( PLAYER_WORK *ew ) ;
static void clash0_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash0_main( PLAYER_WORK *ew )
{
	static void ( *clash0_jmp[] )( PLAYER_WORK *ew ) = {
		clash0_start, clash0_move, clash_hide, clash_end,
	} ;

	clash0_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void clash0_start( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	clash0_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void clash0_move( PLAYER_WORK *ew )
{
	const Float *fade = clash0_fade_tbl ;

	fade += ew->routine_1 ;
	ew->scale.x -= 0.03846154f ;
	ew->scale.y -= 0.03846154f ;
	ew->rot[2] += 0x348 ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	if( ++ ew->routine_1 > 13 ) ew->routine_0 ++ ;
}

static void clash1_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash1_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = clash1_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FADE ;
		ew->work1 = 2 ;
		ew->obj = obj_efky->obj[34] ;
		ew->pos = *pos ;
		ew->rot[2] = 0 ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

static void clash1_wait( PLAYER_WORK *ew ) ;
static void clash1_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash1_main( PLAYER_WORK *ew )
{
	static void ( *clash1_jmp[] )( PLAYER_WORK *ew ) = {
		clash1_wait, clash1_move, clash_hide, clash_end,
	} ;

	clash1_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void clash1_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	clash1_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void clash1_move( PLAYER_WORK *ew )
{
	const Float *fade = clash_fade_tbl ;

	fade += ew->routine_1 ;
	ew->rot[2] += 0x2f7 ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	if( ++ ew->routine_1 > 6 ) ew->routine_0 ++ ;
}

static void clash2_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash2_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = clash2_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FADE ;
		ew->work1 = 3 ;
		ew->obj = obj_efky->obj[35] ;
		ew->pos = *pos ;
		ew->rot[2] = 0x38e ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

static void clash2_wait( PLAYER_WORK *ew ) ;
static void clash2_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash2_main( PLAYER_WORK *ew )
{
	static void ( *clash2_jmp[] )( PLAYER_WORK *ew ) = {
		clash2_wait, clash2_move, clash_hide, clash_end,
	} ;

	clash2_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void clash2_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	clash2_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void clash2_move( PLAYER_WORK *ew )
{
	const Float *fade = clash_fade_tbl ;

	fade += ew->routine_1 ;
	ew->rot[2] += 0x2f7 ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	if( ++ ew->routine_1 > 6 ) ew->routine_0 ++ ;
}

static void clash3_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash3_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = clash3_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FADE ;
		ew->work1 = 4 ;
		ew->obj = obj_efky->obj[36] ;
		ew->pos = *pos ;
		ew->rot[2] = 0x71c ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

static void clash3_wait( PLAYER_WORK *ew ) ;
static void clash3_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash3_main( PLAYER_WORK *ew )
{
	static void ( *clash3_jmp[] )( PLAYER_WORK *ew ) = {
		clash3_wait, clash3_move, clash_hide, clash_end,
	} ;

	clash3_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void clash3_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	clash3_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void clash3_move( PLAYER_WORK *ew )
{
	const Float *fade = clash_fade_tbl ;

	fade += ew->routine_1 ;
	ew->rot[2] += 0x2f7 ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	if( ++ ew->routine_1 > 6 ) ew->routine_0 ++ ;
}

static void clash4_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash4_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = clash4_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_FADE ;
		ew->work1 = 5 ;
		ew->obj = obj_efky->obj[37] ;
		ew->pos = *pos ;
		ew->rot[2] = 0x71c ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

static void clash4_wait( PLAYER_WORK *ew ) ;
static void clash4_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash4_main( PLAYER_WORK *ew )
{
	static void ( *clash4_jmp[] )( PLAYER_WORK *ew ) = {
		clash4_wait, clash4_move, clash_hide, clash_end,
	} ;

	clash4_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void clash4_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	clash4_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void clash4_move( PLAYER_WORK *ew )
{
	const Float *fade = clash_fade_tbl ;

	fade += ew->routine_1 ;
	ew->rot[2] += 0x2f7 ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	if( ++ ew->routine_1 > 6 ) ew->routine_0 ++ ;
}

static void clash_ring_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash_ring_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = clash_ring_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_FACE_Y | OBJ_FADE ;
		ew->work1 = 5 ;
		ew->obj = obj_efky->obj[39] ;
		ew->pos = *pos ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->rot[1] = clash5_rot_tbl[dir] ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

static void clash_ring_wait( PLAYER_WORK *ew ) ;
static void clash_ring_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void clash_ring_main( PLAYER_WORK *ew )
{
	static void ( *clash5_jmp[] )( PLAYER_WORK *ew ) = {
		clash_ring_wait, clash_ring_move, clash_hide, clash_end,
	} ;

	clash5_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void clash_ring_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	clash_ring_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void clash_ring_move( PLAYER_WORK *ew )
{
	const Float *fade = clash5_fade_tbl ;

	fade += ew->routine_1 ;
	ew->scale.x += 0.35714287f ;
	ew->scale.y += 0.35714287f ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	if( ++ ew->routine_1 > 14 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void clash_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void clash_end( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/****************************************************************/
void clash_ef_set( VEC3 *pos, int dir )
{
	clash0_set( pos, dir ) ;
	clash1_set( pos, dir ) ;
	clash2_set( pos, dir ) ;
	clash3_set( pos, dir ) ;
	clash4_set( pos, dir ) ;
	clash_ring_set( pos, dir ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_mark_clash.c" End of File	*/
