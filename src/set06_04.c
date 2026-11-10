/* Stage 06's object 4: when a side has won (three KOs) and the winner stands at the stage's right
   end, nine models (objects 12 to 20) fly from the building's top to their places, one by one, the
   last three shaking the screen, then wobble and are freed. Object 3 rides on it. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* Kamui and the effect library */
extern void move_pos_spd( PLAYER_WORK *w ) ;
extern void land_big_ef_set( VEC3 *pos, int big ) ;
extern void set06_03_set( PLAYER_WORK *pl_w ) ;

/* where object 4 and the pieces start, where each piece lands and how it lands turned */
static VEC3 set06_04_pos = { 1524.0f, 785.0f, -951.0f } ;
static VEC3 set06_04_from[4] = {
	{ 1299.0f, 1066.0f, -950.0f },
	{ 1305.0f, 1012.0f, -892.0f },
	{ 1332.0f, 955.0f, -839.0f },
	{ 1363.0f, 898.0f, -789.0f },
} ;
static VEC3 set06_04_rot_from[4] = {
	{ 0.0f, -82.1f, 0.0f },
	{ 0.0f, -51.7f, 0.0f },
	{ 0.0f, -45.0f, 0.0f },
	{ 0.0f, -45.0f, 0.0f },
} ;
static VEC3 set06_04_to[9] = {
	{ 945.0f, 36.0f, -960.0f },
	{ 846.0f, 22.0f, -794.0f },
	{ 1084.0f, 36.0f, -730.0f },
	{ 1152.0f, 22.0f, -561.0f },
	{ 964.0f, -125.0f, -667.0f },
	{ 1524.0f, 318.0f, -951.0f },
	{ 1524.0f, 395.0f, -931.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 1405.0f, 498.0f, -897.0f },
} ;
static VEC3 set06_04_rot_to[9] = {
	{ 181.16f, -82.1f, 0.0f },
	{ 0.0f, -51.7f, 0.0f },
	{ 188.04f, -45.0f, 0.0f },
	{ 99.78f, -44.17f, -16.23f },
	{ -38.9f, -69.0f, -54.9f },
	{ 0.0f, -45.0f, 0.0f },
	{ 11.45f, -45.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 123.1f, -45.0f, 0.0f },
} ;

static void set06_04_piece_init( PLAYER_WORK *ew ) ;
static void set06_04_piece_move( PLAYER_WORK *ew ) ;
static void set06_04_piece_wobble( PLAYER_WORK *ew ) ;
static void set06_04_piece_wait( ) ;
static void set06_04_piece_hide( PLAYER_WORK *ew ) ;
static void set06_04_piece_free( PLAYER_WORK *ew ) ;
static void set06_04_piece_fall( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set06_04_piece_main( PLAYER_WORK *ew )
{
	static void ( *set06_04_piece_jmp[] )( PLAYER_WORK *ew ) = {
		set06_04_piece_init, set06_04_piece_move, set06_04_piece_wobble,
		set06_04_piece_wait, set06_04_piece_hide, set06_04_piece_free,
	} ;

	set06_04_piece_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The piece waits `timer1` frames, which				*/
/*			`piece_set` made longer for each piece after the	*/
/*			first												*/
/*==============================================================*/
static void set06_04_piece_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 50 ;
	if( ew->type == 8 ){
		pl_se_req( ew, 48 ) ;
		pl_se_req( ew, 73 ) ;
	}
}

/*==============================================================*/
/*			It flies for 50 frames and lands					*/
/*==============================================================*/
static void set06_04_piece_move( PLAYER_WORK *ew )
{
	VEC3 pt ;

	if( ew->timer1 ){
		ew->timer1 -- ;
		return ;
	}
	if( ew->timer0 ){
		ew->timer0 -- ;
		move_pos_spd( ew ) ;
		if( ew->type >= 4 ){
			ew->rot[0] += DEG_ANG( set06_04_rot_to[ew->type].x / 50.0f ) ;
			ew->rot[1] += DEG_ANG( ( set06_04_rot_to[ew->type].y + 45.0f ) / 50.0f ) ;
			ew->rot[2] += DEG_ANG( set06_04_rot_to[ew->type].z / 50.0f ) ;
		}else{
			VEC3 *rf = &set06_04_rot_from[ew->type] ;

			ew->rot[0] += DEG_ANG( ( set06_04_rot_to[ew->type].y - rf->x ) / 50.0f ) ;
			ew->rot[1] += DEG_ANG( ( set06_04_rot_to[ew->type].y - rf->y ) / 50.0f ) ;
			ew->rot[2] += DEG_ANG( ( set06_04_rot_to[ew->type].y - rf->z ) / 50.0f ) ;
		}
		if( ew->type == 8 ){
			if( ew->timer0 == 20 ) pl_se_req( ew, 73 ) ;
		}
		return ;
	}
	ew->routine_0 ++ ;
	ew->pos = set06_04_to[ew->type] ;
	ew->rot[0] = DEG_ANG( set06_04_rot_to[ew->type].x ) ;
	ew->rot[1] = DEG_ANG( set06_04_rot_to[ew->type].y ) ;
	ew->rot[2] = DEG_ANG( set06_04_rot_to[ew->type].z ) ;
	ew->timer0 = 32 ;
	if( ew->type >= 6 ){
		if( ew->type == 6 ){
			scr_work.quake_level = 1 ;
			scr_work.quake_flag = 1 ;
		}
		if( ew->type == 7 ){
			pt.x = 1191.0f ;
			pt.y = 26.0f ;
			pt.z = -338.0f ;
			scr_work.quake_level = 2 ;
			scr_work.quake_flag = 1 ;
			land_big_ef_set( &pt, 1 ) ;
		}
		if( ew->type == 8 ){
			pt.x = 782.0f ;
			pt.y = 36.0f ;
			pt.z = -650.0f ;
			scr_work.quake_level = 3 ;
			scr_work.quake_flag = 1 ;
			land_big_ef_set( &pt, 1 ) ;
			pl_se_req( ew, 49 ) ;
		}
	}
}

/*==============================================================*/
/*			The first four keep falling and bounce, the			*/
/*			others shake in place								*/
/*==============================================================*/
static void set06_04_piece_wobble( PLAYER_WORK *ew )
{
	if( ew->timer0 ){
		ew->timer0 -- ;
		if( ew->type <= 3 ) set06_04_piece_fall( ew ) ;
		else{
			ew->pos.x += ( ew->timer0 % 2 ) ? 2 : -2 ;
			ew->pos.y += ( ew->timer0 % 2 ) ? 4 : -4 ;
		}
	}else{
		ew->routine_0 ++ ;
		ew->timer0 = 30 ;
	}
}

/*==============================================================*/
static void set06_04_piece_fall( PLAYER_WORK *ew )
{
	VEC3 *to = &set06_04_to[ew->type] ;

	move_pos_spd( ew ) ;
	if( ew->pos.y < to->y ){
		ew->pos.y = to->y ;
		ew->spd.y *= -0.12f ;
		ew->spd.x *= 0.8f ;
		ew->spd.z *= 0.8f ;
	}
}

/*--------------------------------------------------------------*/
static void set06_04_piece_wait( )
{
}

/*--------------------------------------------------------------*/
static void set06_04_piece_hide( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void set06_04_piece_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Each piece flies from its start to its place in		*/
/*			50 frames, falling faster as it goes				*/
/*==============================================================*/
static void set06_04_piece_set( int no )
{
	PLAYER_WORK *ew ;
	VEC3 *from, *rf ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_04_piece_main ;
		ew->obj = obj_stg->obj[no + 12] ;
		if( no >= 4 ){
			ew->pos = set06_04_pos ;
			ew->rot[1] = 0xe001 ;
			ew->spd.x = ( set06_04_to[no].x - set06_04_pos.x ) / 50.0f ;
			ew->spd.y = 0.0f ;
			ew->spd.z = ( set06_04_to[no].z - set06_04_pos.z ) / 50.0f ;
			ew->acc.y = 2.0f * ( set06_04_to[no].y - set06_04_pos.y ) / 2500.0f ;
		}else{
			from = &set06_04_from[no] ;
			rf = &set06_04_rot_from[no] ;
			ew->pos = *from ;
			ew->rot[0] = DEG_ANG( rf->x ) ;
			ew->rot[1] = DEG_ANG( rf->y ) ;
			ew->rot[2] = DEG_ANG( rf->z ) ;
			ew->spd.x = ( set06_04_to[no].x - from->x ) / 50.0f ;
			ew->spd.y = 0.0f ;
			ew->spd.z = ( set06_04_to[no].z - from->z ) / 50.0f ;
			ew->acc.y = 2.0f * ( set06_04_to[no].y - from->y ) / 2500.0f ;
		}
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->type = no ;
		ew->timer1 = no * 6 ;
	}
}

static void set06_04_check( PLAYER_WORK *ew ) ;
static void set06_04_idle( ) ;

/*==============================================================*/
static void set06_04_main( PLAYER_WORK *ew )
{
	static void ( *set06_04_jmp[] )( PLAYER_WORK *ew ) = {
		set06_04_check, set06_04_idle,
	} ;

	set06_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			When a side has three KOs and the winner is at		*/
/*			the stage's right end, the pieces fly				*/
/*==============================================================*/
static void set06_04_check( PLAYER_WORK *ew )
{
	PLAYER_WORK *a = vs_sys_work.team_w[0][0] ;
	PLAYER_WORK *b = vs_sys_work.team_w[1][0] ;
	int i ;

	if( vs_sys_work.ko_cnt[0] == 3 || vs_sys_work.ko_cnt[1] == 3 ){
		if( side_flag_get( 1 ) ){
			if( a->fly_screen_flag ){
				if( a->pos.x > scr_work.stage_right + 50.0f || scr_work.stage_right - 50.0f > a->pos.x ) return ;
				ew->routine_0 ++ ;
				ew->timer0 = 100 ;
				ew->obj = obj_stg->obj[8] ;
				for( i = 0 ; i < 9 ; i ++ ) set06_04_piece_set( i ) ;
			}else{
				if( b->pos.x > scr_work.stage_right + 50.0f || scr_work.stage_right - 50.0f > b->pos.x ) return ;
				ew->routine_0 ++ ;
				ew->timer0 = 100 ;
				ew->obj = obj_stg->obj[8] ;
				for( i = 0 ; i < 9 ; i ++ ) set06_04_piece_set( i ) ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void set06_04_idle( )
{
}

/****************************************************************/
void set06_04_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set06_04_main ;
		ew->obj = obj_stg->obj[4] ;
		ew->pos = set06_04_pos ;
		ew->rot[1] = 0xcaac ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		set06_03_set( ew ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set06_04.c" End of File	*/
