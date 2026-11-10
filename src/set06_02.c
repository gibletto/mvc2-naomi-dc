/* Stage 06's object 2: two clock hands (objects 9 and 10 of the stage's file) that turn to the
   time the machine's clock gives, the minute hand and the hour hand. */
#include "player.h"
#include "naomi.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static VEC3 set06_02_pos_tbl[2] = {
	{ 578.0f, 657.0f, -2640.0f },
	{ 559.0f, 657.0f, -2670.0f },
} ;
static VEC3 set06_02_rot_tbl[2] = {
	{ 0.0f, 122.8f, 0.0f },
	{ 0.0f, 122.8f, 0.0f },
} ;

/*==============================================================*/
static void set06_02_main( PLAYER_WORK *ew )
{
	NLA_RTC_DATE rtc ;

	nlaGetRTC( &rtc ) ;
	switch( ew->type ){
	case 0 :
		ew->rot[0] = DEG_ANG( rtc.minute * 6 ) ;
		break ;
	case 1 :
		ew->rot[0] = DEG_ANG( ( rtc.minute + rtc.hour % 12 * 60 ) / 2 ) ;
		break ;
	}
}

/*==============================================================*/
static void set06_02_make( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_02_main ;
		ew->obj = obj_stg->obj[no + 9] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set06_02_pos_tbl[no] ;
		ew->rot[1] = DEG_ANG( set06_02_rot_tbl[no].y ) ;
		ew->type = no ;
	}
}

/****************************************************************/
void set06_02_set( void )
{
	int i ;

	for( i = 0 ; i < 2 ; i ++ ) set06_02_make( i ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set06_02.c" End of File	*/
