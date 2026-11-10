/* The hit marks of kinds 9 to 16 (kind 8 and 9 works; hit_mark.c picks them):
   a ring that stands on the hit and turns by the mark's kind and the side it came from, grows and
   fades in twelve frames (it is counted in hit_mark_cnt), and with it a second piece that spins on
   for six. */
#include "player.h"

/* the turn of the ring's picture, a quarter circle for each step of the mark's kind, mirrored with dir */
#define HIT_RING_ROT( ew, no, dir ) \
	switch( no ){ \
	case 9: \
		( ew )->rot[2] = ( dir ) ? 0x8000 : 0 ; \
		break ; \
	case 10: \
		( ew )->rot[2] = ( dir ) ? 0xa000 : 0xe000 ; \
		break ; \
	case 11: \
		( ew )->rot[2] = 0xc000 ; \
		break ; \
	case 12: \
		( ew )->rot[2] = ( dir ) ? 0xe000 : 0xa000 ; \
		break ; \
	case 13: \
		( ew )->rot[2] = ( dir ) ? 0 : 0x8000 ; \
		break ; \
	case 14: \
		( ew )->rot[2] = ( dir ) ? 0x2000 : 0x6000 ; \
		break ; \
	case 15: \
		( ew )->rot[2] = 0x4000 ; \
		break ; \
	case 16: \
		( ew )->rot[2] = ( dir ) ? 0x6000 : 0x2000 ; \
		break ; \
	}

static void hit_ring_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_ring_set( VEC3 *pos, int dir, int no, Uint8 b, Sint8 list )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, list, LIST_END ) ) ) return ;
	vs_sys_work.hit_mark_cnt ++ ;
	ew->disp_flag = 1 ;
	ew->work_func = hit_ring_main ;
	ew->pos = *pos ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE ;
	ew->obj = obj_efky->obj[95] ;
	HIT_RING_ROT( ew, no, dir )
}

/*==============================================================*/
/*			Its size across and up, and its alpha, by frame		*/
/*==============================================================*/
static void hit_ring_main( PLAYER_WORK *ew )
{
	static const Float hit_ring_x_tbl[13] = { 1.0f, 1.25f, 1.5f, 1.75f, 2.0f, 2.2f, 2.4f, 2.6f, 2.8f, 3.0f, 3.0f, 3.0f, 3.0f, } ;
	static const Float hit_ring_y_tbl[13] = { 1.0f, 0.85f, 0.7f, 0.55f, 0.4f, 0.34f, 0.28f, 0.22f, 0.16f, 0.1f, 0.1f, 0.1f, 0.1f, } ;
	static const Float hit_ring_fade_tbl[13] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, } ;

	const Float *xs = hit_ring_x_tbl, *ys = hit_ring_y_tbl, *fs = hit_ring_fade_tbl ;

	if( ew->timer0 >= 12 ){
		work_free( ew ) ;
		vs_sys_work.hit_mark_cnt -- ;
		return ;
	}
	ew->scale.x = xs[ew->timer0] ;
	ew->scale.y = ys[ew->timer0] ;
	ew->fade.r = fs[ew->timer0] ;
	ew->fade.g = fs[ew->timer0] ;
	ew->fade.b = fs[ew->timer0] ;
	ew->timer0 ++ ;
}

static void hit_ring_spin_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void hit_ring_spin_set( VEC3 *pos, int dir, int no, Uint8 b, Sint8 list )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, list, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = hit_ring_spin_main ;
	ew->pos = *pos ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE ;
	HIT_RING_ROT( ew, no, dir )
	ew->obj = obj_efky->obj[94] ;
}

/*==============================================================*/
static void hit_ring_spin_main( PLAYER_WORK *ew )
{
	static const Float hit_ring2_fade_tbl[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, } ;

	const Float *fs = hit_ring2_fade_tbl ;

	if( ew->timer0 >= 6 ){
		work_free( ew ) ;
		return ;
	}
	ew->rot[2] += 0x38e ;
	ew->fade.r = fs[ew->timer0] ;
	ew->fade.g = fs[ew->timer0] ;
	ew->fade.b = fs[ew->timer0] ;
	ew->timer0 ++ ;
}

/****************************************************************/
/*			The callers pass dir as a Uint16 and it is an		*/
/*			int here											*/
/****************************************************************/
void hit_mark_rings_set( VEC3 *pos, int dir, int no, Uint8 b, Sint8 list )
{
	hit_ring_spin_set( pos, dir, no, b, list ) ;
	hit_ring_set( pos, dir, no, b, list ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_mark_ring.c" End of File	*/
