/* The frame's drawing: the stage's models and the works' models in their lists, the sprites by
   draw priority, and the screen fade. */
#include "player.h"
#include "char_tbl.h"

extern void ef12_disp( PLAYER_WORK *w ) ;
extern void scr_matrix_set( void ) ;	/* the camera for the stage */
extern void scr_far_matrix_set( void ) ;	/* the camera for the far stage */
extern void scr_2d_matrix_set( void ) ;	/* the camera for the front */
extern void scr_matrix_set3( void ) ;
extern void sel_mes_disp( void ) ;
extern void spr_mark_disp( void ) ;
extern int sprite_disp( PLAYER_WORK *w ) ;

static void obj_calc_list( Uint8 list ) ;
static void obj_calc( PLAYER_WORK *w, Uint8 list ) ;
static void obj_disp_list( Uint8 list ) ;
static void obj_disp( PLAYER_WORK *w, Uint8 list ) ;
static void obj_fade_set( PLAYER_WORK *w ) ;
static void obj_func( PLAYER_WORK *w ) ;
static void prio_disp( void ) ;
static void pl_disp( PLAYER_WORK *w ) ;
static void ef_disp( PLAYER_WORK *w ) ;
static void stage_disp( void ) ;
static void stage_fade_set( void ) ;
static void stage_obj_disp( void ) ;
static void stage_obj2_disp( void ) ;
static void far_obj_disp( void ) ;
static void far_obj2_disp( void ) ;
static void front_ef_disp( void ) ;
static void front_disp( void ) ;

/* the box drawn over the screen in a scene's colour */
static NLVERTEX00 scene_vtx[4] = {
	{ 0.0f, 0.0f, 10.0f, 0x30202020 },
	{ 0.0f, 480.0f, 10.0f, 0x30202020 },
	{ 640.0f, 0.0f, 10.0f, 0x30202020 },
	{ 640.0f, 480.0f, 10.0f, 0x30202020 },
} ;

/* the box the fade draws */
static NLVERTEX00 fade_vtx[4] = {
	{ 0.0f, 0.0f, 10.0f, 0xffffffff },
	{ 0.0f, 480.0f, 10.0f, 0xffffffff },
	{ 640.0f, 0.0f, 10.0f, 0xffffffff },
	{ 640.0f, 480.0f, 10.0f, 0xffffffff },
} ;

/****************************************************************/
/*			The largest of a scale's three parts				*/
/****************************************************************/
Float obj_scale_max( VEC3 *scale )
{
	Float max ;

	max = scale->x ;
	if( scale->y > max ) max = scale->y ;
	if( scale->z > max ) max = scale->z ;
	return max ;
}

/*==============================================================*/
/*			Make the matrices of a list's models				*/
/*==============================================================*/
static void obj_calc_list( Uint8 list )
{
	PLAYER_WORK *w ;

	w = work_top[list] ;
	if( !w ) return ;
	do {
		obj_calc( w, list ) ;
		w = w->behind_w ;
	} while( w ) ;
}

/*==============================================================*/
/*			Make a model's matrix: its place, its turns			*/
/*			towards the camera and its own, and its scale		*/
/*==============================================================*/
static void obj_calc( PLAYER_WORK *w, Uint8 list )
{
	VEC3 pos ;

	nlPushMatrix( 0 ) ;
	if( w->base_mtx ) nlLoadMatrix( w->base_mtx ) ;
	else nlUnitMatrix() ;
	if( w->obj_flag & OBJ_MTX ) nlMultiMatrix( &w->obj_mtx ) ;
	if( w->obj_flag & OBJ_POS ){
		switch( list ){
		case 5:
		case 6:
		case 11:
		case 13:
			nlTranslateV( &w->pos ) ;
			break ;
		case 7:
		case 8:
		case 9:
			pos = w->pos ;
			pos.x *= 0.1f ;
			pos.y *= 0.1f ;
			pos.z *= 0.1f ;
			nlTranslateV( &pos ) ;
			break ;
		}
	}
	if( w->obj_flag & OBJ_FACE_Y ){
		VEC3 v ;

		v.x = scr_work.eye[1].x - w->pos.x ;
		v.z = scr_work.eye[1].z - w->pos.z ;
		nlRotateAY( nlArcTan( v.x / v.z ) ) ;
	}
	if( w->obj_flag & OBJ_FACE_X ){
		VEC3 v ;

		v.y = scr_work.eye[1].y - w->pos.y ;
		v.z = scr_work.eye[1].z - w->pos.z ;
		nlRotateAX( nlArcTan( -v.y / v.z ) ) ;
	}
	if( w->obj_flag & OBJ_ROT_Z ) nlRotateAZ( w->rot[2] ) ;
	if( w->obj_flag & OBJ_ROT_Y ) nlRotateAY( w->rot[1] ) ;
	if( w->obj_flag & OBJ_ROT_X ) nlRotateAX( w->rot[0] ) ;
	if( w->obj_flag & OBJ_SCALE ) nlScaleV( &w->scale ) ;
	nlGetMatrix( &w->obj_mtx ) ;
	nlPopMatrix( 1 ) ;
}

/*==============================================================*/
/*			Draw a list's models								*/
/*==============================================================*/
static void obj_disp_list( Uint8 list )
{
	PLAYER_WORK *w ;

	w = work_top[list] ;
	if( !w ) return ;
	do {
		if( w->disp_flag && w->obj ){
			if( w->disp_func ) obj_func( w ) ;
			obj_disp( w, list ) ;
		}
		w = w->behind_w ;
	} while( w ) ;
}

/*==============================================================*/
/*			Draw a model										*/
/*==============================================================*/
static void obj_disp( PLAYER_WORK *w, Uint8 list )
{
	nlPushMatrix( 0 ) ;
	nlLoadMatrix( &w->obj_mtx ) ;
	if( w->obj_flag & OBJ_SCALE ) nlSetScaleFactor( obj_scale_max( &w->scale ) ) ;
	obj_fade_set( w ) ;
	if( w->obj_flag & OBJ_SHADOW ){
		nlObjPutMvSetIntensity( 0x80 ) ;
		nlObjPutMvClosedVolumeUseViewMatrix( w->obj ) ;
	} else {
		if( w->obj_flag & OBJ_ENV ) nlObjPutCheapEnvMap( w->obj ) ;
		if( w->obj_flag & OBJ_TRNSL ){
			if( list == 11 || list == 13 ) nlObjPutTrnsl( w->obj, w->fade.trnsl ) ;
			else nlObjPutUseViewMatrixTrnsl( w->obj, w->fade.trnsl ) ;
		} else {
			if( list == 11 || list == 13 ) nlObjPut( w->obj ) ;
			else nlObjPutUseViewMatrix( w->obj ) ;
		}
	}
	nlPopMatrix( 1 ) ;
}

/*==============================================================*/
/*			A model's fade colour: its own, the stage's,		*/
/*			both, or none										*/
/*==============================================================*/
static void obj_fade_set( PLAYER_WORK *w )
{
	Float r, g, b ;

	if( w->obj_flag & OBJ_FADE ){
		if( w->obj_flag & OBJ_BRIGHT ){
			r = w->fade.r * bright_work.rate[1] ;
			g = w->fade.g * bright_work.rate[2] ;
			b = w->fade.b * bright_work.rate[3] ;
		} else {
			r = w->fade.r ;
			g = w->fade.g ;
			b = w->fade.b ;
		}
	} else if( w->obj_flag & OBJ_BRIGHT ){
		r = bright_work.rate[1] ;
		g = bright_work.rate[2] ;
		b = bright_work.rate[3] ;
	} else {
		r = 1.0f ;
		g = 1.0f ;
		b = 1.0f ;
	}
	nlObjPutSetFadeColor( r, g, b ) ;
}

/****************************************************************/
/*			Give a model a routine to run before it is drawn	*/
/****************************************************************/
void obj_func_set( PLAYER_WORK *w, void ( *func )( PLAYER_WORK *w ) )
{
	w->disp_func = func ;
}

/*==============================================================*/
/*			Run it												*/
/*==============================================================*/
static void obj_func( PLAYER_WORK *w )
{
	w->disp_func( w ) ;
}

/****************************************************************/
/*			Draw the front, and unless only the front is		*/
/*			wanted the stage, the far stage and list 1, then	*/
/*			spr_num_disp's display								*/
/****************************************************************/
void disp_all( void )
{
	SYSTEM_WORK *sys ;

	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
	if( Sys->heart_step > 1 ) return ;
	scr_matrix_set() ;
	obj_calc_list( 5 ) ;
	obj_disp_list( 5 ) ;
	scr_far_matrix_set() ;
	obj_calc_list( 7 ) ;
	obj_disp_list( 7 ) ;
	obj_calc_list( 1 ) ;
	obj_disp_list( 1 ) ;
	sys = Sys ;
	if( ( Sys->cpu_level_type || sys->vms_mode ) && sys->scene_no == 1 && !sys->heart_step ){
		sel_mes_disp() ;
		spr_num_disp( vms_work.point_lv, vms_work.point ) ;
	}
}

/****************************************************************/
/*			Draw the front, and the stage's objects unless		*/
/*			only the front is wanted							*/
/****************************************************************/
void disp_front_stage( void )
{
	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
	if( Sys->heart_step > 1 ) return ;
	scr_matrix_set() ;
	obj_calc_list( 5 ) ;
	obj_disp_list( 5 ) ;
}

/****************************************************************/
/*			Draw the front's models								*/
/****************************************************************/
void disp_front_obj( void )
{
	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
}

/****************************************************************/
/*			Draw the front's models, a box over the screen		*/
/*			and the sprites of list 6							*/
/****************************************************************/
void disp_front_spr( void )
{
	PLAYER_WORK *w ;

	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
	nlDispStripTrnsl( 4, scene_vtx ) ;
	nlSetCommonTextureOffset( 0 ) ;
	w = work_top[W_BACK] ;
	if( !w ) return ;
	do {
		nlSprPut( &w->tile_top ) ;
		w = w->behind_w ;
	} while( w ) ;
}

/****************************************************************/
/*			The same as disp_front_stage						*/
/****************************************************************/
void disp_front_stage2( void )
{
	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
	if( Sys->heart_step > 1 ) return ;
	scr_matrix_set() ;
	obj_calc_list( 5 ) ;
	obj_disp_list( 5 ) ;
}

/****************************************************************/
/*			Draw the front's models, then spr_num_disp's		*/
/*			display												*/
/****************************************************************/
void disp_front_info( void )
{
	SYSTEM_WORK *sys ;

	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
	sys = Sys ;
	if( sys->cpu_level_type == 0 && sys->vms_mode == 0 ) return ;
	spr_num_disp( vms_work.point_lv, vms_work.point ) ;
}

/****************************************************************/
/*			Draw the stage's objects from another camera		*/
/****************************************************************/
void disp_stage_obj( void )
{
	scr_matrix_set3() ;
	obj_calc_list( 5 ) ;
	obj_disp_list( 5 ) ;
}

/****************************************************************/
/*			Draw the stage's objects, then the front's			*/
/****************************************************************/
void disp_stage_front( void )
{
	scr_matrix_set() ;
	obj_calc_list( 5 ) ;
	obj_disp_list( 5 ) ;
	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
}

/****************************************************************/
/*			The fight's drawing, in the mode flash_flag_get		*/
/*			gives												*/
/****************************************************************/
void game_disp( void )
{
	int mode ;

	if( ( mode = flash_flag_get() ) ){
		if( mode == 1 ){
			far_obj2_disp() ;
			front_disp() ;
		} else {
			prio_disp() ;
			far_obj_disp() ;
			far_obj2_disp() ;
		}
	} else {
		prio_disp() ;
		if( !Sys->hyper_bg ){
			stage_disp() ;
			stage_obj_disp() ;
		}
		stage_obj2_disp() ;
		far_obj_disp() ;
		front_disp() ;
	}
	if( Sys->cpu_level_max == 1 ) spr_mark_disp() ;
}

/*==============================================================*/
/*			Draw the sprites, by priority						*/
/*==============================================================*/
static void prio_disp( void )
{
	int i, j ;
	PLAYER_WORK *w ;

	for( i = 0 ; i < PRIO_MAX ; i ++ ){
		for( j = 0 ; j < disp_prio_cnt[i] ; j ++ ){
			w = disp_prio_w[i][j] ;
			if( w->work_kind == W_PL ) pl_disp( w ) ;
			else ef_disp( w ) ;
		}
	}
}

/*==============================================================*/
/*			Draw a player's sprite								*/
/*==============================================================*/
static void pl_disp( PLAYER_WORK *w )
{
	Float sx, sy, sz ;
	Float k ;
	SPR_PRM *p ;
	int n ;

	if( !w->disp_flag ) return ;
	scr_matrix_set() ;
	nlMakeModelViewProjectMatrix( 640.0f, 480.0f ) ;
	nlScreenPoint( w->pos.x, w->pos.y, w->pos.z, &sx, &sy, &sz ) ;
	w->disp_x = sx ;
	w->disp_y = sy ;
	w->disp_z = prio_z[w->prio] + scr_work.eye[1].z * 0.1f ;
	w->disp_zm_x = DOT_X( w->scale.x ) ;
	w->disp_zm_y = DOT_Y( w->scale.y ) ;
	w->disp_ang = w->rot[2] ;
	w->disp_flip = w->disp_dir ;
	w->disp_list = 0 ;
	w->disp_ofs_x = w->rot_ofs_x ;
	w->disp_ofs_y = w->rot_ofs_y ;
	k = scr_work.eye[1].z / 812.3573f ;
	w->disp_zm_x /= k ;
	w->disp_zm_y /= k ;
	if( w->cg_zoom ){
		p = ( SPR_PRM * )w->char_adrs + w->cg_zoom ;
		w->disp_zm_x *= p->zm_x / 1000.0f ;
		w->disp_zm_y *= p->zm_y / 1000.0f ;
		w->disp_ang = ( Uint16 )( w->disp_ang + p->ang ) ;
		w->disp_ofs_x = p->ofs_x ;
		w->disp_ofs_y = p->ofs_y ;
		w->disp_flip ^= p->flip ;
	}
	n = sprite_disp( w ) ;
	Sys->spr_cnt += n ;
	prio_z[w->prio] += n * 0.001f ;
}

/*==============================================================*/
/*			Draw an effect's sprite								*/
/*==============================================================*/
static void ef_disp( PLAYER_WORK *w )
{
	Float sx, sy, sz ;
	Float k ;
	SPR_PRM *p ;
	int n ;
	int prio ;

	if( !w->disp_flag ) return ;
	if( w->work_kind <= 0 ) return ;
	if( w->work_kind >= 5 ) return ;
	prio = w->prio ;
	nlScreenPoint( w->pos.x, w->pos.y, w->pos.z, &sx, &sy, &sz ) ;
	w->disp_x = sx ;
	w->disp_y = sy ;
	w->disp_z = prio_z[prio] + scr_work.eye[1].z * 0.1f ;
	w->disp_zm_x = DOT_X( w->scale.x ) ;
	w->disp_zm_y = DOT_Y( w->scale.y ) ;
	w->disp_ang = w->rot[2] ;
	w->disp_flip = w->disp_dir ;
	w->disp_list = 0 ;
	w->disp_ofs_x = w->rot_ofs_x ;
	w->disp_ofs_y = w->rot_ofs_y ;
	k = scr_work.eye[1].z / 812.3573f ;
	w->disp_zm_x /= k ;
	w->disp_zm_y /= k ;
	if( w->cg_zoom ){
		p = ( SPR_PRM * )pl_work[w->work_no].char_adrs + w->cg_zoom ;
		w->disp_zm_x *= p->zm_x / 1000.0f ;
		w->disp_zm_y *= p->zm_y / 1000.0f ;
		w->disp_ang = ( Uint16 )( w->disp_ang + p->ang ) ;
		w->disp_ofs_x = p->ofs_x ;
		w->disp_ofs_y = p->ofs_y ;
		w->disp_flip ^= p->flip ;
	}
	n = sprite_disp( w ) ;
	Sys->spr_cnt += n ;
	prio_z[prio] += n * 0.001f ;
}

/*==============================================================*/
/*			Draw the stage's model								*/
/*==============================================================*/
static void stage_disp( void )
{
	if( stage_no == 8 ) return ;
	scr_matrix_set() ;
	nlPushMatrix( 0 ) ;
	nlUnitMatrix() ;
	stage_fade_set() ;
	nlObjPutUseViewMatrix( obj_stg->obj[0] ) ;
	nlPopMatrix( 1 ) ;
}

/*==============================================================*/
static void stage_fade_set( void )
{
	nlObjPutSetFadeColor( bright_work.rate[1], bright_work.rate[2], bright_work.rate[3] ) ;
}

/*==============================================================*/
static void stage_obj_disp( void )
{
	scr_matrix_set() ;
	obj_calc_list( 5 ) ;
	obj_disp_list( 5 ) ;
}

/*==============================================================*/
static void stage_obj2_disp( void )
{
	scr_matrix_set() ;
	obj_calc_list( 6 ) ;
	obj_disp_list( 6 ) ;
}

/*==============================================================*/
static void far_obj_disp( void )
{
	scr_far_matrix_set() ;
	obj_calc_list( 7 ) ;
	obj_disp_list( 7 ) ;
	scr_far_matrix_set() ;
	obj_calc_list( 8 ) ;
	obj_disp_list( 8 ) ;
}

/*==============================================================*/
static void far_obj2_disp( void )
{
	scr_far_matrix_set() ;
	obj_calc_list( 9 ) ;
	obj_disp_list( 9 ) ;
}

/*==============================================================*/
/*			The effects drawn in front							*/
/*==============================================================*/
static void front_ef_disp( void )
{
	PLAYER_WORK *w ;

	w = work_top[W_FRONT] ;
	if( !w ) return ;
	do {
		if( w->disp_flag ) ef12_disp( w ) ;
		w = w->behind_w ;
	} while( w ) ;
}

/*==============================================================*/
static void front_disp( void )
{
	scr_2d_matrix_set() ;
	obj_calc_list( 11 ) ;
	obj_disp_list( 11 ) ;
	if( Sys->play_mode == 64 ) return ;
	front_ef_disp() ;
}

/****************************************************************/
/*			Draw the fade's colour over the screen				*/
/****************************************************************/
void fade_disp( void )
{
	int i ;

	for( i = 0 ; i < 4 ; i ++ ) fade_vtx[i].col = fade_work.col ;
	nlDispStripCoordMode( 0 ) ;
	nlDispStripTrnsl( 4, fade_vtx ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "disp.c" End of File	*/
