/* The continue scene's picture (kind 11): the model 6 of obj_dm0b, which
   slides to its place in 30 frames, shakes for 20 and then ripples its texture. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

/* where it starts and stops */
static const VEC3 cont_ef0_pos_tbl[2] = {
	{ 2.86f, -3.75f, -1.24f },
	{ 0.43f, 0.13f, -2.98f },
} ;
/* its turn, in degrees, at the start and the stop (the same, so only the first is read) */
static const VEC3 cont_ef0_rot_tbl[2] = {
	{ -18.43f, -20.71f, -58.03f },
	{ -18.43f, -20.71f, -58.03f },
} ;

static void cont_ef0_main( PLAYER_WORK *ew ) ;
static void cont_ef0_in( PLAYER_WORK *ew ) ;
static void cont_ef0_shake( PLAYER_WORK *ew ) ;
static void cont_ef0_scroll( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void cont_ef0_move( PLAYER_WORK *ew )
{
	const VEC3 *p = &cont_ef0_pos_tbl[0], *q = &cont_ef0_pos_tbl[1] ;

	ew->pos.x = p->x + ( q->x - p->x ) * ew->timer0 / 30.0f ;
	ew->pos.y = p->y + ( q->y - p->y ) * ew->timer0 / 30.0f ;
	ew->pos.z = p->z + ( q->z - p->z ) * ew->timer0 / 30.0f ;
}

/****************************************************************/
void cont_ef0_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = cont_ef0_main ;
		ew->obj = obj_dm0b->obj[6] ;
		ew->pos = cont_ef0_pos_tbl[0] ;
		ew->rot[0] = DEG_ANG( cont_ef0_rot_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( cont_ef0_rot_tbl[0].y ) ;
		ew->rot[2] = DEG_ANG( cont_ef0_rot_tbl[0].z ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		obj_uv_clear( ew->obj ) ;
	}
}

/*==============================================================*/
static void cont_ef0_main( PLAYER_WORK *ew )
{
	static void ( *cont_ef0_jmp[3] )( PLAYER_WORK *ew ) = {
		cont_ef0_in, cont_ef0_shake, cont_ef0_scroll,
	} ;

	if( Sys->scene_no != 3 ){
		work_free( ew ) ;
		return ;
	}
	cont_ef0_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It slides in										*/
/*==============================================================*/
static void cont_ef0_in( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	cont_ef0_move( ew ) ;
	if( ew->timer0 == 30 ){
		ew->routine_0 ++ ;
		ew->timer0 = 0 ;
		ew->pos = cont_ef0_pos_tbl[1] ;
	}
}

/*==============================================================*/
/*			It shakes up and down								*/
/*==============================================================*/
static void cont_ef0_shake( PLAYER_WORK *ew )
{
	Float f ;

	ew->timer0 ++ ;
	f = 0.05f ;
	if( ew->timer0 & 1 ) f = -0.05f ;
	ew->pos.y = cont_ef0_pos_tbl[1].y + f ;
	if( ew->timer0 == 20 ){
		ew->routine_0 ++ ;
		ew->timer0 = 0 ;
		ew->pos = cont_ef0_pos_tbl[1] ;
	}
}

/*==============================================================*/
/*			Its texture ripples									*/
/*==============================================================*/
static void cont_ef0_scroll( PLAYER_WORK *ew )
{
	Float x, y ;

	ew->timer1 ++ ;
	if( ew->timer1 >= 200 ) ew->timer1 = 0 ;
	obj_uv_begin( obj_dm0b->obj[7], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &x, &y ) ;
		x += 1.0f - ew->timer1 * 0.005f ;
		obj_uv_put( &x, &y ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "cont_ef0.c" End of File	*/
