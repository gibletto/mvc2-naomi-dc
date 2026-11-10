/* Stage 2's object 2: five works of kind 5 that each wait their turn, then show a run of the stage's
   pictures in one of three ways (grow, flicker, blink) while their glow fades. */
#include "player.h"

/* the object's own counters, from unk_d0: the round of 1300 frames, the frames left before it shows
   and the frames of the round */
typedef struct SET02_02_LOCAL_W {
	Sint32	cnt[3];
} SET02_02_LOCAL_W;
#define SET02_02_LW( w )	( ( SET02_02_LOCAL_W * )&( w )->work1 )

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* the stage's models */

static const VEC3 set02_02_pos[] = {
	{ -443.0f, 131.6f, -348.0f },
	{ -858.999f, 175.6f, -495.999f },
	{ -937.543f, 507.191f, -495.999f },
	{ 440.0f, 195.6f, -428.0f },
	{ 1212.807f, 291.176f, -209.0f },
} ;

/* the turns of each, in degrees */
static const VEC3 set02_02_rot[] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, -65.8901f },
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 20.0f },
} ;

/* for each round of 1300 frames, the frames each of the five waits before it shows (0 for not at all) */
static const Sint16 set02_02_wait_tbl[11][5] = {
	{ 20, 10, 40, 50, 30 },
	{ 10, 30, 20, 40, 50 },
	{ 50, 30, 20, 10, 40 },
	{ 40, 50, 10, 30, 20 },
	{ 10, 30, 0, 20, 0 },
	{ 20, 0, 10, 30, 0 },
	{ 30, 0, 20, 0, 10 },
	{ 10, 0, 20, 0, 30 },
	{ 30, 10, 0, 20, 0 },
	{ 0, 0, 10, 30, 20 },
	{ 0, 20, 30, 0, 10 },
} ;

static void set02_02_main( PLAYER_WORK *ew );
static void set02_02_start( PLAYER_WORK *ew );
static void set02_02_wait( PLAYER_WORK *ew );
static void set02_02_show( PLAYER_WORK *ew );
static void set02_02_hide( PLAYER_WORK *ew );
static Uint8 set02_02_grow( PLAYER_WORK *ew );
static Uint8 set02_02_grow_init( PLAYER_WORK *ew );
static Uint8 set02_02_grow_step( PLAYER_WORK *ew );
static Uint8 set02_02_flick( PLAYER_WORK *ew );
static Uint8 set02_02_flick_init( PLAYER_WORK *ew );
static Uint8 set02_02_flick_step( PLAYER_WORK *ew );
static Uint8 set02_02_blink( PLAYER_WORK *ew );
static Uint8 set02_02_blink_init( PLAYER_WORK *ew );
static Uint8 set02_02_blink_step( PLAYER_WORK *ew );
static Uint8 set02_02_blink_hold( PLAYER_WORK *ew );
static Uint8 set02_02_blink_end( PLAYER_WORK *ew );

/*==============================================================*/
static void set02_02_main( PLAYER_WORK *ew )
{
	static void ( *set02_02_jmp[] )( PLAYER_WORK *ew ) = {
		set02_02_start, set02_02_wait, set02_02_show, set02_02_hide,
	} ;

	set02_02_jmp[ew->routine_0]( ew ) ;
	if( ++ SET02_02_LW( ew )->cnt[2] > 0x514 ){
		SET02_02_LW( ew )->cnt[0] ++ ;
		SET02_02_LW( ew )->cnt[0] %= 12 ;
		SET02_02_LW( ew )->cnt[2] = 0 ;
		ew->routine_0 = 0 ;
	}
}

/*==============================================================*/
static void set02_02_start( PLAYER_WORK *ew )
{
	const Sint16 *p = set02_02_wait_tbl[0] ;

	p += SET02_02_LW( ew )->cnt[0] * 5 ;

	ew->disp_flag = 0 ;
	p += ew->sub_no ;
	if( *p ){
		SET02_02_LW( ew )->cnt[1] = *p ;
		ew->routine_0 = 1 ;
		set02_02_wait( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void set02_02_wait( PLAYER_WORK *ew )
{
	if( -- SET02_02_LW( ew )->cnt[1] == 0 ){
		ew->routine_0 = 2 ;
		ew->routine_1 = Rnd() ;
		ew->routine_1 %= 3 ;
		set02_02_show( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void set02_02_show( PLAYER_WORK *ew )
{
	static Uint8 ( *set02_02_beh_jmp[] )( PLAYER_WORK *ew ) = {
		set02_02_blink, set02_02_grow, set02_02_flick,
	} ;

	ew->disp_flag = 1 ;
	if( set02_02_beh_jmp[ew->routine_1]( ew ) ){
		ew->routine_0 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_02_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 set02_02_grow( PLAYER_WORK *ew )
{
	static Uint8 ( *set02_02_grow_jmp[] )( PLAYER_WORK *ew ) = {
		set02_02_grow_init, set02_02_grow_step,
	} ;

	if( set02_02_grow_jmp[ew->routine_2]( ew ) ){
		ew->routine_2 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 set02_02_grow_init( PLAYER_WORK *ew )
{
	ew->fade.trnsl = 1.0f ;
	ew->scale.x = 0.8f ;
	ew->routine_2 = 1 ;
	set02_02_grow_step( ew ) ;
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 set02_02_grow_step( PLAYER_WORK *ew )
{
	if( ew->timer1 > 8 ){
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->fade.trnsl = 1.0f ;
		return 1 ;
	}
	if( !ew->timer0 ){
		ew->obj = obj_stg->obj[27 + ew->timer1] ;
	}
	ew->timer0 ++ ;
	ew->fade.trnsl -= 0.0125f ;
	if( ew->scale.x < 1.0f ){
		ew->scale.x += 0.0125f ;
		if( ew->scale.x >= 1.0f ){
			ew->scale.x = 1.0f ;
		}
	}
	if( ew->timer0 > 7 ){
		ew->timer0 = 0 ;
		ew->timer1 ++ ;
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 set02_02_flick( PLAYER_WORK *ew )
{
	static Uint8 ( *set02_02_flick_jmp[] )( PLAYER_WORK *ew ) = {
		set02_02_flick_init, set02_02_flick_step,
	} ;

	if( set02_02_flick_jmp[ew->routine_2]( ew ) ){
		ew->routine_2 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 set02_02_flick_init( PLAYER_WORK *ew )
{
	ew->fade.trnsl = 1.0f ;
	ew->scale.x = 1.0f ;
	ew->routine_2 = 1 ;
	ew->routine_3 = 0 ;
	set02_02_flick_step( ew ) ;
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 set02_02_flick_step( PLAYER_WORK *ew )
{
	if( ew->timer1 > 7 ){
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->routine_3 ++ ;
		if( ew->routine_3 > 19 ){
			ew->routine_2 = 0 ;
			return 1 ;
		}
	}
	if( !ew->timer0 ){
		ew->obj = obj_stg->obj[27 + ew->timer1] ;
	}
	ew->timer0 ++ ;
	ew->fade.trnsl -= 0.00625f ;
	if( ew->timer0 > 0 ){
		ew->timer0 = 0 ;
		ew->timer1 ++ ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 set02_02_blink( PLAYER_WORK *ew )
{
	static Uint8 ( *set02_02_blink_jmp[] )( PLAYER_WORK *ew ) = {
		set02_02_blink_init, set02_02_blink_step, set02_02_blink_hold, set02_02_blink_end,
	} ;

	if( set02_02_blink_jmp[ew->routine_2]( ew ) ){
		ew->routine_2 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static Uint8 set02_02_blink_init( PLAYER_WORK *ew )
{
	ew->fade.trnsl = 1.0f ;
	ew->scale.x = 1.0f ;
	ew->routine_2 = 1 ;
	ew->routine_3 = 0 ;
	set02_02_blink_step( ew ) ;
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 set02_02_blink_step( PLAYER_WORK *ew )
{
	if( ew->timer1 > 7 ){
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		ew->routine_2 = 2 ;
		set02_02_blink_hold( ew ) ;
		return 0 ;
	}
	if( !ew->timer0 ){
		ew->obj = obj_stg->obj[27 + ew->timer1] ;
	}
	ew->fade.trnsl -= 0.0625f ;
	if( ++ ew->timer0 > 0 ){
		ew->timer0 = 0 ;
		ew->timer1 ++ ;
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 set02_02_blink_hold( PLAYER_WORK *ew )
{
	if( ew->timer1 > 7 ){
		ew->timer0 = 60 ;
		ew->timer1 = 0 ;
		ew->routine_2 = 3 ;
		set02_02_blink_end( ew ) ;
		return 0 ;
	}
	if( !ew->timer0 ){
		ew->obj = obj_stg->obj[27 + ew->timer1] ;
	}
	ew->fade.trnsl -= 0.0625f ;
	if( ++ ew->timer0 > 0 ){
		ew->timer0 = 0 ;
		ew->timer1 ++ ;
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static Uint8 set02_02_blink_end( PLAYER_WORK *ew )
{
	if( ew->timer0 -- == 0 ){
		if( ++ ew->routine_3 > 5 ){
			return 1 ;
		}
		ew->fade.trnsl = 1.0f ;
		ew->routine_2 = 1 ;
		ew->timer0 = 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static void set02_02_parts( Sint32 obj_no )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 5 ; i ++ ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ){
			break ;
		}
		ew->disp_flag = 1 ;
		ew->work_func = set02_02_main ;
		ew->obj = obj_stg->obj[27] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE | OBJ_BRIGHT ;
		ew->sub_no = i ;
		ew->pos = set02_02_pos[i] ;
		ew->rot[0] = DEG_ANG( set02_02_rot[i].x ) ;
		ew->rot[1] = DEG_ANG( set02_02_rot[i].y ) ;
		ew->rot[2] = DEG_ANG( set02_02_rot[i].z ) ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->fade.trnsl = 1.0f ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/****************************************************************/
void set02_02_set( void )
{
	set02_02_parts( 27 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set02_02.c" End of File	*/
