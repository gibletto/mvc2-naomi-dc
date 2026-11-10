/* Stage 08's object 10, which nothing calls: a spark that grows through five
   models and sizes, then jumps to one of six places and throws puffs (set08_09.c) around it. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void set08_09_make( VEC3 *pos ) ;

static Sint32 set08_0a_obj_tbl[10] = { 21, 21, 22, 23, 24, 21, 21, 22, 23, 24, } ;
static Float set08_0a_scale_tbl[10] = {
	0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.25f, 0.5f, 0.5f, 0.5f, 0.5f,
} ;
static VEC3 set08_0a_pos_tbl[6] = {
	{ 350.0f, -60.0f, -800.0f },
	{ 50.0f, -60.0f, -1300.0f },
	{ 500.0f, -60.0f, -1500.0f },
	{ -900.0f, -60.0f, -2300.0f },
	{ -880.0f, -60.0f, -1000.0f },
	{ -900.0f, -60.0f, 1100.0f },
} ;

static void set08_0a_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set08_0a_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_0a_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_BRIGHT ;
		ew->obj = obj_stg->obj[set08_0a_obj_tbl[0]] ;
		ew->pos = set08_0a_pos_tbl[0] ;
		ew->scale.x = set08_0a_scale_tbl[0] ;
		ew->scale.y = set08_0a_scale_tbl[0] ;
		ew->scale.z = set08_0a_scale_tbl[0] ;
	}
}

/*==============================================================*/
static void set08_0a_main( PLAYER_WORK *ew )
{
	if( ++ ew->timer1 >= 5 ){
		ew->timer1 = 0 ;
		if( ( Uint32 )( ++ ew->timer0 ) >= 10 ){
			ew->pos = set08_0a_pos_tbl[( Uint32 )nlRand() % 6] ;
			ew->timer0 = 0 ;
			set08_09_make( &ew->pos ) ;
		}
		if( ew->timer0 == 5 ){
			ew->pos.x += 50.0f * nlSin( DEG_ANG( nlRand() % 360 ) ) ;
			ew->pos.z += 50.0f * nlCos( DEG_ANG( nlRand() % 360 ) ) ;
		}
		ew->obj = obj_stg->obj[set08_0a_obj_tbl[ew->timer0]] ;
		ew->scale.x = set08_0a_scale_tbl[ew->timer0] ;
		ew->scale.y = set08_0a_scale_tbl[ew->timer0] ;
		ew->scale.z = set08_0a_scale_tbl[ew->timer0] ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_0a.c" End of File	*/
