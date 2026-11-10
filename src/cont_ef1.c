/* The continue scene's count (kind 11 works): the number, a model of
   obj_dm0b that slides in and then swings to the place of the count Sys->timer2 shows (the spring
   of step_1), the two fading copies that trail it, and the ring that grows and fades round a
   change of the count. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static const VEC3 cont_ef1_pos_tbl[2] = {
	{ 0.623f, 4.653f, -2.686f },
	{ 0.623f, -0.107f, -2.686f },
} ;
static const VEC3 cont_ef1_rot = { 56.83f, 0.0f, -90.0f } ;
/* the place of each count, from 9 to 1 */
static const Float cont_ef1_x_tbl[9] = { -173.03f, -144.95f, -120.66f, -94.88f, -69.44f, -44.11f, -18.9f, 6.99f, 31.74f } ;
static const Float cont_ef1_spd_x = 56.83f ;
static const VEC3 cont_ef1_ring_pos = { 0.373f, -0.109f, -1.28f } ;
static const VEC3 cont_ef1_ring_rot = { 0.0f, -90.0f, 0.0f } ;
static const Float cont_ef1_turn_tbl[2] = { 0.0f, 720.0f } ;

static void cont_ef1_main( PLAYER_WORK *ew ) ;
static void cont_ef1_step_init( PLAYER_WORK *ew ) ;
static void cont_ef1_step_move( PLAYER_WORK *ew ) ;
static void cont_ef1_ring_set( PLAYER_WORK *master ) ;
static void cont_ef1_trail_set( PLAYER_WORK *master ) ;
static void cont_ef1_trail_main( PLAYER_WORK *ew ) ;
static void cont_ef1_ring_main( PLAYER_WORK *ew ) ;
static void cont_ef1_ring_init( PLAYER_WORK *ew ) ;
static void cont_ef1_ring_fast( PLAYER_WORK *ew ) ;
static void cont_ef1_ring_slow( PLAYER_WORK *ew ) ;
static void cont_ef1_in( PLAYER_WORK *ew ) ;
static void cont_ef1_count( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void cont_ef1_move( PLAYER_WORK *ew )
{
	const VEC3 *p = &cont_ef1_pos_tbl[0], *q = &cont_ef1_pos_tbl[1] ;

	ew->pos.x = p->x + ( q->x - p->x ) * ew->timer0 / 20.0f ;
	ew->pos.y = p->y + ( q->y - p->y ) * ew->timer0 / 20.0f ;
	ew->pos.z = p->z + ( q->z - p->z ) * ew->timer0 / 20.0f ;
}

/*==============================================================*/
static void cont_ef1_step( PLAYER_WORK *ew )
{
	static void ( *cont_ef1_step_jmp[2] )( PLAYER_WORK *ew ) = {
		cont_ef1_step_init, cont_ef1_step_move,
	} ;

	cont_ef1_step_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void cont_ef1_step_init( PLAYER_WORK *ew )
{
	const Float *p = cont_ef1_x_tbl ;

	ew->routine_1 ++ ;
	switch( ew->routine_2 ){
	case 0:
		p += Sys->timer2 ;
		ew->timer0 = 25 ;
		break ;
	case 1:
		p += Sys->timer2 ;
		ew->timer0 = 10 ;
		break ;
	}
	ew->acc.x = ( *p - ew->spd.x ) / ew->timer0 ;
	ew->scale.x = *p ;
	cont_ef1_step_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void cont_ef1_step_move( PLAYER_WORK *ew )
{
	if( ew->timer0 ){
		ew->timer0 -- ;
		ew->spd.x += ew->acc.x ;
		ew->rot[0] = DEG_ANG( ew->spd.x ) ;
		ew->timer1 = 1 ;
		return ;
	}
	ew->spd.x = ew->scale.x ;
	if( ew->timer1 <= 10 ){
		ew->spd.x += ( ew->timer1 % 2 ? 2.0f / ew->timer1 : -2.0f / ew->timer1 ) ;
		ew->timer1 ++ ;
	}
	else if( !ew->work2 ) cont_ef1_ring_set( ew ) ;
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
}

/****************************************************************/
void cont_ef1_set( void )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = cont_ef1_main ;
	ew->obj = obj_dm0b->obj[8] ;
	ew->pos = cont_ef1_pos_tbl[0] ;
	ew->rot[0] = DEG_ANG( cont_ef1_rot.x ) ;
	ew->rot[1] = DEG_ANG( cont_ef1_rot.y ) ;
	ew->rot[2] = DEG_ANG( cont_ef1_rot.z ) ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
	ew->timer0 = 0 ;
	ew->timer1 = 0 ;
	ew->spd.x = cont_ef1_spd_x ;
	cont_ef1_trail_set( ew ) ;
}

/*==============================================================*/
static void cont_ef1_trail_set( PLAYER_WORK *master )
{
	PLAYER_WORK *ew ;
	const VEC3 *p = &cont_ef1_pos_tbl[0] ;
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		if( !( ew = work_get( 0, W_SYS, LIST_END ) ) ) return ;
		ew->master_w = master ;
		ew->sub_no = i ;
		ew->disp_flag = 1 ;
		ew->work_func = cont_ef1_trail_main ;
		ew->obj = obj_dm0b->obj[8] ;
		ew->pos = *p ;
		ew->rot[0] = DEG_ANG( cont_ef1_rot.x ) ;
		ew->rot[1] = DEG_ANG( cont_ef1_rot.y ) ;
		ew->rot[2] = DEG_ANG( cont_ef1_rot.z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_TRNSL | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
		ew->fade.trnsl = 1.0f - ( ew->sub_no + 1 ) * 0.2f ;
		ew->scale = ew->pos ;
	}
}

/*==============================================================*/
static void cont_ef1_trail_main( PLAYER_WORK *ew )
{
	if( Sys->scene_no != 3 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->master_w->routine_2 == 0 ){
		if( ew->master_w->timer0 == 0 && ew->master_w->timer1 <= 10 ){
			ew->disp_flag = 1 ;
			if( !ew->timer0 ){
				ew->pos = ew->scale ;
				ew->rot[0] = DEG_ANG( ew->spd.x ) ;
				ew->timer0 = ew->sub_no + 1 ;
				ew->scale = ew->master_w->pos ;
				ew->spd.x = ew->master_w->spd.x ;
			}
			else ew->timer0 -- ;
		}
		else{
			ew->disp_flag = 0 ;
			ew->timer0 = 0 ;
			ew->spd.x = ew->master_w->spd.x ;
		}
	}
	else ew->disp_flag = 0 ;
}

/*==============================================================*/
static void cont_ef1_ring_set( PLAYER_WORK *master )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->master_w = master ;
		ew->disp_flag = 1 ;
		ew->work_func = cont_ef1_ring_main ;
		ew->obj = obj_dm0b->obj[9] ;
		ew->pos = cont_ef1_ring_pos ;
		ew->rot[0] = DEG_ANG( cont_ef1_ring_rot.x ) ;
		ew->rot[1] = DEG_ANG( cont_ef1_ring_rot.y ) ;
		ew->rot[2] = DEG_ANG( cont_ef1_ring_rot.z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE | OBJ_BRIGHT ;
		ew->scale.x = 0.6f ;
		ew->scale.y = 0.6f ;
		ew->scale.z = 0.6f ;
		ew->spd.x = cont_ef1_ring_rot.x ;
		ew->master_w->work2 = 1 ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void cont_ef1_ring_main( PLAYER_WORK *ew )
{
	static void ( *cont_ef1_ring_jmp[3] )( PLAYER_WORK *ew ) = {
		cont_ef1_ring_init, cont_ef1_ring_fast, cont_ef1_ring_slow,
	} ;

	if( Sys->scene_no != 3 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->master_w->timer0 ){
		ew->master_w->work2 = 0 ;
		work_free( ew ) ;
		return ;
	}
	cont_ef1_ring_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void cont_ef1_ring_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->rot[0] = DEG_ANG( cont_ef1_ring_rot.x ) ;
	ew->spd.x = cont_ef1_ring_rot.x ;
	ew->acc.x = ( cont_ef1_turn_tbl[1] - ew->spd.x ) / ( Sys->timer3 / 10 ) ;
	ew->acc.y = 0.7f / ( Sys->timer3 / 10 ) ;
	ew->acc.z = 0.5f / ( Sys->timer3 / 10 ) ;
	ew->routine_0 ++ ;
	ew->timer1 = Sys->timer3 / 10 ;
	cont_ef1_ring_fast( ew ) ;
}

/*--------------------------------------------------------------*/
static void cont_ef1_ring_fast( PLAYER_WORK *ew )
{
	if( ew->timer1 > 0 ) ew->timer1 -- ;
	else{
		ew->routine_0 ++ ;
		ew->acc.y = 0.5f / ( Sys->timer3 / 3 ) ;
		ew->acc.z = 0.5f / ( Sys->timer3 / 3 ) ;
		ew->timer1 = Sys->timer3 / 3 ;
		return ;
	}
	ew->scale.x += ew->acc.y ;
	ew->scale.y += ew->acc.y ;
	ew->scale.z += ew->acc.y ;
	ew->fade.r -= ew->acc.z ;
	ew->fade.g -= ew->acc.z ;
	ew->fade.b -= ew->acc.z ;
	ew->rot[0] += 0xe39 ;
}

/*--------------------------------------------------------------*/
static void cont_ef1_ring_slow( PLAYER_WORK *ew )
{
	if( ew->timer1 ){
		ew->timer1 -- ;
		ew->scale.x += ew->acc.y ;
		ew->scale.y += ew->acc.y ;
		ew->scale.z += ew->acc.y ;
		ew->fade.r -= ew->acc.z ;
		ew->fade.g -= ew->acc.z ;
		ew->fade.b -= ew->acc.z ;
		ew->rot[0] += 0xe39 ;
	}
}

/*==============================================================*/
static void cont_ef1_main( PLAYER_WORK *ew )
{
	static void ( *cont_ef1_main_jmp[2] )( PLAYER_WORK *ew ) = {
		cont_ef1_in, cont_ef1_count,
	} ;

	if( Sys->scene_no != 3 ){
		work_free( ew ) ;
		return ;
	}
	cont_ef1_main_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void cont_ef1_in( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	cont_ef1_move( ew ) ;
	if( ew->timer0 == 20 ){
		ew->routine_0 ++ ;
		ew->timer0 = 0 ;
		ew->pos = cont_ef1_pos_tbl[1] ;
		ew->work3 = Sys->timer2 ;
		ew->routine_2 = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void cont_ef1_count( PLAYER_WORK *ew )
{
	if( ew->work3 != Sys->timer2 ){
		ew->routine_1 = 0 ;
		if( ew->work3 > Sys->timer2 ){
			ew->routine_2 = 0 ;
			na_req( 63 ) ;
		}
		if( ew->work3 < Sys->timer2 ) ew->routine_2 = 1 ;
	}
	cont_ef1_step( ew ) ;
	ew->work3 = Sys->timer2 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "cont_ef1.c" End of File	*/
