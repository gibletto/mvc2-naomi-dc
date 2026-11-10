/* The finish of a round won with a hyper combo (kind 9 works; vs_sys.c makes it
   with finish_ef_set beside flash_finish_set): three models that fly past the camera, one that carries
   another along, each turned to face the eye. */
#include "player.h"

static void finish_a_set( PLAYER_WORK *ew ) ;
static void finish_b_set( PLAYER_WORK *a_w ) ;
static void finish_c_set( PLAYER_WORK *ew ) ;

/****************************************************************/
/*			Ew is never set: the setters only use it to hold	*/
/*			the work they get									*/
/****************************************************************/
void finish_ef_set( void )
{
	PLAYER_WORK *ew ;

	finish_a_set( ew ) ;
	finish_c_set( ew ) ;
	if( vs_sys_work.game_mode != 8 ){
		s3l_fade_out_set( 1 ) ;
		return ;
	}
	if( Sys->vs_flag ){
		s3l_fade_out_set( 1 ) ;
		return ;
	}
	if( vs_sys_work.round_cnt == 2 ) s3l_fade_out_set( 1 ) ;
}

static void finish_a_main( PLAYER_WORK *ew ) ;
static void finish_b_main( PLAYER_WORK *ew ) ;
static void finish_c_main( PLAYER_WORK *ew ) ;
static void finish_face( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			The first model, far behind the screen				*/
/*==============================================================*/
static void finish_a_set( PLAYER_WORK *ew )
{
	if( ( ew = work_get2( 0, W_FINISH, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = finish_a_main ;
		ew->obj = obj_efky->obj[238] ;
		ew->pos.x = scr_work.eye[0].x ;
		ew->pos.z = -100000.0f ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z ;
		ew->timer0 = 0 ;
		finish_face( ew ) ;
		finish_b_set( ew ) ;
	}
}

/*==============================================================*/
/*			The second, on the first's matrix					*/
/*==============================================================*/
static void finish_b_set( PLAYER_WORK *a_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get2( 0, W_FINISH, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = finish_b_main ;
		ew->obj = obj_efky->obj[239] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z ;
		ew->base_mtx = &a_w->obj_mtx ;
		ew->pos.z = 60000.0f ;
		ew->timer0 = 0 ;
	}
}

/*==============================================================*/
/*			The third, at the screen's centre					*/
/*==============================================================*/
static void finish_c_set( PLAYER_WORK *ew )
{
	if( ( ew = work_get2( 0, W_FINISH, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = finish_c_main ;
		ew->obj = obj_efky->obj[240] ;
		ew->pos.x = scr_work.eye[0].x ;
		ew->pos.y = scr_work.center_y ;
		ew->pos.z = -40000.0f ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z | OBJ_TRNSL ;
		ew->timer0 = 0 ;
		ew->fade.trnsl = 1.0f ;
		finish_face( ew ) ;
	}
}

/*==============================================================*/
/*			It comes in, faster and faster, and then ends		*/
/*			the fade											*/
/*==============================================================*/
static void finish_a_main( PLAYER_WORK *ew )
{
	ew->pos.x = scr_work.eye[0].x ;
	if( ew->timer0 ){
		if( ew->timer0 <= 10 ){
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		if( ew->timer0 <= 35 ){
			ew->pos.z += 200.0f ;
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		if( ew->timer0 <= 60 ){
			ew->pos.z += 400.0f ;
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		if( ew->timer0 <= 85 ){
			ew->pos.z += 800.0f ;
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		if( ew->timer0 <= 110 ){
			ew->pos.z += 1200.0f ;
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		if( ew->timer0 <= 135 ){
			ew->pos.z += 1600.0f ;
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		if( ew->timer0 <= 160 ){
			ew->pos.z += 2000.0f ;
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		if( ew->timer0 <= 185 ){
			ew->pos.z += 1000.0f ;
			ew->timer0 ++ ;
			finish_face( ew ) ;
			return ;
		}
		work_free( ew ) ;
		if( vs_sys_work.game_mode != 8 ){
			s3l_fade_in_set( 60 ) ;
			return ;
		}
		if( Sys->vs_flag ){
			s3l_fade_in_set( 60 ) ;
			return ;
		}
		if( vs_sys_work.round_cnt == 2 ) s3l_fade_in_set( 60 ) ;
		return ;
	}
	finish_face( ew ) ;
	ew->timer0 ++ ;
}

/*==============================================================*/
/*			It turns and swings by frame, and ends after 135	*/
/*==============================================================*/
static void finish_b_main( PLAYER_WORK *ew )
{
	if( ew->timer0 ){
		if( ew->timer0 <= 10 ){
			ew->timer0 ++ ;
			return ;
		}
		if( ew->timer0 <= 35 ){
			ew->pos.z += 400.0f ;
			ew->rot[2] += 0x1b5 ;
			ew->timer0 ++ ;
			return ;
		}
		if( ew->timer0 <= 60 ){
			ew->rot[2] += 0x2d8 ;
			ew->timer0 ++ ;
			return ;
		}
		if( ew->timer0 <= 85 ){
			ew->pos.z += -600.0f ;
			ew->rot[2] += 0x765 ;
			ew->timer0 ++ ;
			return ;
		}
		if( ew->timer0 <= 110 ){
			ew->pos.z += 600.0f ;
			ew->rot[2] += 0x36a ;
			ew->timer0 ++ ;
			return ;
		}
		if( ew->timer0 <= 135 ){
			ew->pos.z += -600.0f ;
			ew->timer0 ++ ;
			return ;
		}
		work_free( ew ) ;
		return ;
	}
	ew->timer0 ++ ;
}

/*==============================================================*/
static void finish_c_main( PLAYER_WORK *ew )
{
	ew->pos.x = scr_work.eye[0].x ;
	if( ew->timer0 >= 80 ) ew->fade.trnsl -= 0.014285714f ;
	if( ew->timer0 >= 150 ){
		work_free( ew ) ;
		return ;
	}
	finish_face( ew ) ;
	ew->timer0 ++ ;
}

/*==============================================================*/
/*			It stands at the height the eye sees along its		*/
/*			depth, turned up to face the eye					*/
/*==============================================================*/
static void finish_face( PLAYER_WORK *ew )
{
	Float dz ;

	ew->pos.y = ( scr_work.eye[0].z - ew->pos.z ) * ( scr_work.center_y - scr_work.eye[0].y ) / scr_work.eye[0].z ;
	ew->rot[0] = nlArcTan2( ew->pos.y, scr_work.eye[0].z - ew->pos.z ) ;
	dz = scr_work.eye[0].z - ew->pos.z ;
	if( 0.0f > dz ) ew->rot[0] += 0x8000 ;
	ew->pos.y += scr_work.eye[0].y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "finish_ef.c" End of File	*/
