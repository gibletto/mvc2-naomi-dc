/* Stage 02's object 5: a model (object 20) that rises and sinks between two heights, with three
   lit parts riding on it (objects 42 and 43 that fade their colour, and 44), and a second model
   (object 41) that turns and fades its colour. */
#include "player.h"

/* the model's own work, from unk_d0: [1] the frames it holds at its stop, [2] the height it stops at */
typedef struct SET02_05_LOCAL_W {
	Sint32	work[3];
} SET02_05_LOCAL_W;
#define SET02_05_LW( w )	( ( SET02_05_LOCAL_W * )&( w )->work1 )

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static const VEC3 set02_05_pos = { -1250.57f, 577.0f, -2874.37f } ;
static const VEC3 set02_05_rot = { 0.0f, -8.5944f, 0.0f } ;
static const Float set02_05_y_tbl[2] = { -1000.0f, 3000.0f } ;
static const VEC3 set02_05_glow_pos = { 0.0f, 0.0f, 0.0f } ;
static const VEC3 set02_05_glow2_pos = { 0.0f, 0.0f, 0.0f } ;
static const VEC3 set02_05_pole_pos = { -1536.0f, 1160.999f, -1843.99f } ;
static const VEC3 set02_05_pole_rot = { 0.0f, 0.0f, 0.0f } ;
static const VEC3 set02_05_lamp_pos = { 0.0f, 0.0f, 0.0f } ;

/*==============================================================*/
/*			The lamp is lit while the model it rides on is		*/
/*			on its way											*/
/*==============================================================*/
static void set02_05_lamp_main( PLAYER_WORK *ew )
{
	if( ew->ef_w->routine_1 ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
}

/*==============================================================*/
static void set02_05_lamp_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_05_lamp_main ;
		ew->obj = obj_stg->obj[44] ;
		ew->ef_w = pl_w ;
		ew->obj_flag = OBJ_BRIGHT ;
		ew->pos = set02_05_lamp_pos ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
	return ;
}

static void set02_05_glow_step( PLAYER_WORK *ew ) ;
static void set02_05_glow_wait( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_05_glow_main( PLAYER_WORK *ew )
{
	static void ( *set02_05_glow_jmp[] )( PLAYER_WORK *ew ) = {
		set02_05_glow_step, set02_05_glow_wait,
	} ;

	set02_05_glow_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
/*			The colour goes up or down by a twentieth of		*/
/*			timer1 a frame										*/
/*==============================================================*/
static void set02_05_glow_step( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( !( ew->timer0 %= 20 ) ){
		if( ew->timer1 == 1 ){
			ew->fade.r = 1.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 1.0f ;
		} else {
			ew->fade.r = 0.0f ;
			ew->fade.g = 0.0f ;
			ew->fade.b = 0.0f ;
		}
		ew->timer1 *= -1 ;
		ew->routine_0 = 1 ;
	}
	ew->fade.r += ew->timer1 * 1.0f / 20.0f ;
	ew->fade.g += ew->timer1 * 1.0f / 20.0f ;
	ew->fade.b += ew->timer1 * 1.0f / 20.0f ;
}

/*--------------------------------------------------------------*/
static void set02_05_glow_wait( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( !( ew->timer0 %= 40 ) ) ew->routine_0 = 0 ;
}

/*==============================================================*/
static void set02_05_glow_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_05_glow_main ;
		ew->obj = obj_stg->obj[42] ;
		ew->obj_flag = OBJ_FADE | OBJ_BRIGHT ;
		ew->pos = set02_05_glow_pos ;
		ew->timer1 = 1 ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
	return ;
}

/*==============================================================*/
static void set02_05_glow2_main( PLAYER_WORK *ew )
{
	static void ( *set02_05_glow2_jmp[] )( PLAYER_WORK *ew ) = {
		set02_05_glow_step, set02_05_glow_wait,
	} ;

	set02_05_glow2_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_05_glow2_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_05_glow2_main ;
		ew->obj = obj_stg->obj[43] ;
		ew->obj_flag = OBJ_FADE | OBJ_BRIGHT ;
		ew->pos = set02_05_glow2_pos ;
		ew->timer1 = 1 ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
	return ;
}

static void set02_05_init( PLAYER_WORK *ew ) ;
static void set02_05_move( PLAYER_WORK *ew ) ;
static void set02_05_wait( PLAYER_WORK *ew ) ;
static void set02_05_hold( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_05_main( PLAYER_WORK *ew )
{
	static void ( *set02_05_jmp[] )( PLAYER_WORK *ew ) = {
		set02_05_init, set02_05_move, set02_05_wait, set02_05_hold,
	} ;

	set02_05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It starts from one height towards the other,		*/
/*			with a random height from 500 to 1500 to stop at	*/
/*==============================================================*/
static void set02_05_init( PLAYER_WORK *ew )
{
	ew->pos.y = set02_05_y_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->spd.y = ( set02_05_y_tbl[ew->routine_1] - ew->pos.y ) / 600.0f ;
	ew->timer0 = 600 ;
	ew->routine_2 = 1 ;
	SET02_05_LW( ew )->work[2] = Rnd() ;
	SET02_05_LW( ew )->work[2] *= 4 ;
	SET02_05_LW( ew )->work[2] %= 1001 ;
	SET02_05_LW( ew )->work[2] += 500 ;
	ew->routine_0 = 1 ;
	set02_05_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void set02_05_move( PLAYER_WORK *ew )
{
	ew->pos.y += ew->spd.y ;
	if( ew->routine_2 ){
		if( ew->routine_1 ){
			if( SET02_05_LW( ew )->work[2] <= ew->pos.y ){
				SET02_05_LW( ew )->work[1] = 500 ;
				ew->routine_0 = 3 ;
			}
		} else {
			if( ew->pos.y <= SET02_05_LW( ew )->work[2] ){
				SET02_05_LW( ew )->work[1] = 500 ;
				ew->routine_0 = 3 ;
			}
		}
	}
	if( !( -- ew->timer0 ) ){
		ew->timer0 = 200 ;
		ew->routine_0 = 2 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_05_wait( PLAYER_WORK *ew )
{
	if( !( -- ew->timer0 ) ) ew->routine_0 = 0 ;
}

/*--------------------------------------------------------------*/
static void set02_05_hold( PLAYER_WORK *ew )
{
	if( !( -- SET02_05_LW( ew )->work[1] ) ){
		ew->routine_2 = 0 ;
		ew->routine_0 = 1 ;
	}
}

/*==============================================================*/
static void set02_05_make( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_05_main ;
		ew->obj = obj_stg->obj[20] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set02_05_pos ;
		ew->rot[0] = DEG_ANG( set02_05_rot.x ) ;
		ew->rot[1] = DEG_ANG( set02_05_rot.y ) ;
		ew->rot[2] = DEG_ANG( set02_05_rot.z ) ;
		set02_05_glow_set( ew ) ;
		set02_05_glow2_set( ew ) ;
		set02_05_lamp_set( ew ) ;
	}
}

/*==============================================================*/
/*			The second model: its colour goes up and down		*/
/*			over 40 frames, and it turns						*/
/*==============================================================*/
static void set02_05_pole_main( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	if( !( ew->routine_0 %= 40 ) ){
		if( ew->timer1 == 1 ){
			ew->fade.r = 1.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 1.0f ;
		}
		if( ew->timer1 == -1 ){
			ew->fade.r = 0.0f ;
			ew->fade.g = 0.0f ;
			ew->fade.b = 0.0f ;
		}
		ew->timer1 *= -1 ;
	} else {
		ew->fade.r += ew->timer1 * 1.0f / 40.0f ;
		ew->fade.g += ew->timer1 * 1.0f / 40.0f ;
		ew->fade.b += ew->timer1 * 1.0f / 40.0f ;
	}
	ew->timer0 += 3 ;
	ew->timer0 %= 360 ;
	ew->rot[1] = DEG_ANG( ew->timer0 ) ;
}

/*==============================================================*/
static void set02_05_pole_make( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_05_pole_main ;
		ew->obj = obj_stg->obj[41] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_FADE | OBJ_BRIGHT ;
		ew->pos = set02_05_pole_pos ;
		ew->rot[1] = DEG_ANG( set02_05_pole_rot.y ) ;
		ew->timer1 = 1 ;
	}
}

/****************************************************************/
void set02_05_set( void )
{
	set02_05_make( 20 ) ;
	set02_05_pole_make( 41 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set02_05.c" End of File	*/
