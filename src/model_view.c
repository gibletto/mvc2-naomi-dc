/* The model viewer, a debug mode of the arcade: it loads the stage, effect and damage models
   by their number, lists them, and shows one with a camera that turns, moves and zooms
  . */
#include "player.h"

extern Uint32 file_load( Sint32 no, void *adrs, Sint32 a ) ;
extern void SysTransReset( void ) ;
extern void System_Init( void ) ;
extern void obj_mat_count( int *obj, Sint32 *num ) ;	/* the number of materials of a model */
extern void obj_vtx_count( int *obj, Sint32 *num ) ;	/* and of its vertices */

/* the sets, by the kind a model has in model_kind */
extern VEC3 polar_pos2( Float len, Float elev, Float yaw ) ;

static void model_load_stage( void ) ;
static void model_put( int off, OBJ_FILE **dst, OBJ_FILE *adrs ) ;
static void model_file_select( void ) ;
static void model_file_st_demo( void ) ;
static void model_file_staff( void ) ;
static void model_file_ranking( void ) ;
static void model_file14( void ) ;
static void model_load_gauge( void ) ;
static void model_load_select( void ) ;
static void model_load_staff( void ) ;
static void model_load_st_demo( void ) ;
static void model_load_ranking( void ) ;
static void model_load14( void ) ;
static void model_load( int no ) ;
static int sel_lr( Uint16 sw, int no, int max ) ;
static int sel_ud( Uint16 sw, int no, int max ) ;
static Uint16 pad_get( Uint8 side ) ;
void model_end( void ) ;
void model_exit( void ) ;
void model_ck( void ) ;
static void model_back_set( void ) ;
static void model_view_init( void ) ;
static void model_sel( void ) ;
static void model_cam_reset( void ) ;
static void model_view_step( void ) ;
static void model_view_load( void ) ;
static void model_view( void ) ;
static void model_view_disp( void ) ;
static void model_axis_disp( void ) ;

#define MODEL_NUM ( sizeof( model_name ) / sizeof( model_name[0] ) )

/* the camera's offset when it starts */
VEC3 model_cam0 = { 0.0f, 0.0f, 0.0f } ;
/* the viewer's light */
VEC3 model_light = { 0.0f, 1.0f, 1.0f } ;
/* the models' names, in the order of the list */
const char *model_name[23] = {
	"efky",
	"stg00", "stg01", "stg02", "stg03", "stg04", "stg05", "stg06", "stg07", "stg08",
	"dm00", "dm01", "dm02", "dm03", "dm04", "dm05", "dm06", "dm07", "dm08", "dm09", "dm0a", "dm0b", "dm0c"
} ;
/* what is loaded for each: 1 the stage's, 2 the effects', 4 and up the damage works' */
Sint32 model_kind[23] = {
	2,
	1, 1, 1, 1, 1, 1, 1, 1, 1,
	4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16
} ;
/* the set each is shown from */
OBJ_FILE **model_set_tbl[23] = {
	&obj_efky,
	&obj_stg, &obj_stg, &obj_stg, &obj_stg, &obj_stg,
	&obj_stg, &obj_stg, &obj_stg, &obj_stg,
	&obj_dm00, &obj_dm01, &obj_dm02, &obj_dm03, &obj_dm04,
	&obj_dm05, &obj_dm06, &obj_dm07, &obj_dm08, &obj_dm09,
	&obj_dm0a, &obj_dm0b, &obj_dm0c
} ;
/* the background colours: below and above */
Uint32 model_back[8][2] = {
	{ 0xff000000, 0xff000000 }, { 0xff404040, 0xff808080 }, { 0xff004000, 0xff008000 }, { 0xff004040, 0xff008080 },
	{ 0xff400000, 0xff800000 }, { 0xff400040, 0xff800080 }, { 0xff404000, 0xff808000 }, { 0xff000040, 0xff000080 }
} ;
Sint32 model_file_no0[9] = { 0x316, 0x318, 0x31a, 0x31c, 0x31e, 0x320, 0x322, 0x324, 0x326 } ;
Sint32 model_file_no1[9] = { 0x315, 0x317, 0x319, 0x31b, 0x31d, 0x31f, 0x321, 0x323, 0x325 } ;
/* the modes: the set-up, the list and the view */
void ( *model_mode_tbl[3] )( void ) = { model_view_init, model_sel, model_view_step } ;
/* the view's steps: the load and the camera */
void ( *model_step_tbl[2] )( void ) = { model_view_load, model_view } ;
/* the axes: the lines of two lengths, and their colours */
VEC3 model_axis_big[3][2] = {
	{ { -10000.0f, 0.0f, 0.0f }, { 10000.0f, 0.0f, 0.0f } },
	{ { 0.0f, -10000.0f, 0.0f }, { 0.0f, 10000.0f, 0.0f } },
	{ { 0.0f, 0.0f, -10000.0f }, { 0.0f, 0.0f, 10000.0f } }
} ;
VEC3 model_axis_small[3][2] = {
	{ { -100.0f, 0.0f, 0.0f }, { 100.0f, 0.0f, 0.0f } },
	{ { 0.0f, -100.0f, 0.0f }, { 0.0f, 100.0f, 0.0f } },
	{ { 0.0f, 0.0f, -100.0f }, { 0.0f, 0.0f, 100.0f } }
} ;
Uint32 model_axis_col[3] = { 0xffff0000, 0xff00ff00, 0xff0000ff } ;

Sint8 model_mode ;	/* 0 set up, 1 the list, 2 the view */
Sint8 model_step ;	/* the view's step */
Sint8 model_back_no ;	/* the background colour */
Sint8 unk_0C2FB43B ;
OBJ_FILE *model_cur ;	/* the set shown */
Sint32 model_no ;	/* the model of the list */
Sint32 obj_no ;	/* the model of the set */
Float cam_len ;	/* the camera's distance */
Float cam_elev ;	/* its height angle */
Float cam_yaw ;	/* and its turn */
VEC3 cam_pos ;
VEC3 cam_ofs ;	/* where it looks */
Sint8 help_no ;	/* the help text shown: 0 and 1 show, 2 hides */

/*==============================================================*/
/*			The stage models, loaded where the stage's files	*/
/*			are													*/
/*==============================================================*/
static void model_load_stage( void )
{
	int i ;

	nlSetCommonTextureOffset( 0xc10 ) ;
	obj_stg = ( OBJ_FILE * )0xd82d000 ;
	for( i = 0 ; i < obj_stg->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_stg->obj[i], obj_stg->thd ) ;
	}
}

/*==============================================================*/
/*			A set at adrs, with its textures given to the		*/
/*			library from texture number off						*/
/*==============================================================*/
static void model_put( int off, OBJ_FILE **dst, OBJ_FILE *adrs )
{
	int i ;

	nlSetCommonTextureOffset( off ) ;
	*dst = adrs ;
	for( i = 0 ; i < ( *dst )->cnt ; i ++ ){
		nlObjPutLoadTexture( ( *dst )->obj[i], ( *dst )->thd ) ;
	}
}

/*==============================================================*/
static void model_file_select( void )
{
	file_load( 0x329, ( void * )0xd0c6000, 0 ) ;
}

/*==============================================================*/
static void model_file_st_demo( void )
{
}

/*==============================================================*/
static void model_file_staff( void )
{
}

/*==============================================================*/
static void model_file_ranking( void )
{
	file_load( 0x332, ( void * )0xd2e9000, 0 ) ;
}


/*==============================================================*/
static void model_file14( void )
{
	Uint16 *dst ;

	if( 0 ) *( Uint16 ** )0xd720020 = dst ;
	dst = ( Uint16 * )0xcc00000 ;
	if( Sys->cpu_level_type == 0 ){ }	/* tested, and nothing done either way */
	*( Uint16 ** )0xd720020 = dst ;
	lz_unpack16( ( Sint16 * )0xd721000, dst ) ;
}

/*==============================================================*/
static void model_load_gauge( void )
{
	int i ;

	obj_dm00 = ( OBJ_FILE * )0xd082000 ;
	nlSetCommonTextureOffset( 0xc90 ) ;
	for( i = 0 ; i < obj_dm00->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm00->obj[i], obj_dm00->thd ) ;
	}
}

/*==============================================================*/
static void model_load_select( void )
{
	int i ;

	obj_dm01 = ( OBJ_FILE * )0xd0c6000 ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm01->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm01->obj[i], obj_dm01->thd ) ;
	}
}

/*==============================================================*/
static void model_load_staff( void )
{
	int i ;

	obj_dm05 = ( OBJ_FILE * )0xd7cc000 ;
	obj_dm07 = ( OBJ_FILE * )0xd7cc000 ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm05->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm05->obj[i], obj_dm05->thd ) ;
	}
}

/*==============================================================*/
static void model_load_st_demo( void )
{
	int i ;

	obj_dm02 = ( OBJ_FILE * )0xd25c000 ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm02->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm02->obj[i], obj_dm02->thd ) ;
	}
}

/*==============================================================*/
static void model_load_ranking( void )
{
	int i ;

	obj_dm06 = ( OBJ_FILE * )0xd2bb000 ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm06->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm06->obj[i], obj_dm06->thd ) ;
	}
}

/*==============================================================*/
static void model_load14( void )
{
	model_put( 0xd10, &obj_dm0a, ( OBJ_FILE * )0xd720000 ) ;
}

/*==============================================================*/
/*			Load what a kind of model needs						*/
/*==============================================================*/
static void model_load( int no )
{
	switch( no ){
	case 1:
		model_load_stage() ;
		break ;
	case 2:
		model_put( 0xc50, &obj_efky, ( OBJ_FILE * )0xda00000 ) ;
		break ;
	case 4:
		model_load_gauge() ;
		break ;
	case 5:
		model_file_select() ;
		model_load_select() ;
		break ;
	case 6:
		model_file_st_demo() ;
		model_load_st_demo() ;
		break ;
	case 8:
		model_put( 0xd10, &obj_dm04, ( OBJ_FILE * )0xd2b1000 ) ;
		break ;
	case 9:
		model_file_staff() ;
		model_load_staff() ;
		break ;
	case 10:
		model_file_ranking() ;
		model_load_ranking() ;
		break ;
	case 11:
		model_put( 0x810, &obj_dm07, ( OBJ_FILE * )0xda28000 ) ;
		break ;
	case 12:
		model_put( 0x810, &obj_dm08, ( OBJ_FILE * )0xd4ce000 ) ;
		break ;
	case 13:
		model_put( 0x810, &obj_dm09, ( OBJ_FILE * )0xd6cc000 ) ;
		break ;
	case 14:
		model_file14() ;
		model_load14() ;
		break ;
	case 15:
		model_put( 0x850, &obj_dm0b, ( OBJ_FILE * )0xd7a9000 ) ;
		break ;
	}
}

/*==============================================================*/
/*			Left and right move a number, up and down			*/
/*			another												*/
/*==============================================================*/
static int sel_lr( Uint16 sw, int no, int max )
{
	if( ( sw & 0x800 ) == 0x800 ){
		if( -- no < 0 ) no = max ;
	}
	if( ( sw & 0x400 ) == 0x400 ){
		if( ++ no > max ) no = 0 ;
	}
	return no ;
}

/*==============================================================*/
static int sel_ud( Uint16 sw, int no, int max )
{
	if( ( sw & 0x2000 ) == 0x2000 ){
		if( -- no < 0 ) no = max ;
	}
	if( ( sw & 0x1000 ) == 0x1000 ){
		if( ++ no > max ) no = 0 ;
	}
	return no ;
}

/*==============================================================*/
/*			The keys of the sides in side (bit 0 and 1)			*/
/*==============================================================*/
static Uint16 pad_get( Uint8 side )
{
	Uint16 sw = 0 ;

	if( side & 1 ){
		sw |= ( sw_work[0].rep & KEY_LEVER ) | sw_work[0].on ;
	}
	if( side & 2 ){
		sw |= ( sw_work[1].rep & KEY_LEVER ) | sw_work[1].on ;
	}
	return sw ;
}

/****************************************************************/
void model_end( void )
{
	SysTransReset() ;
	bright_stop_set() ;
	model_back_set() ;
	back_color_disp() ;
}

/****************************************************************/
void model_exit( void )
{
	System_Init() ;
}

/****************************************************************/
/*			START, R and L together start and end the viewer	*/
/****************************************************************/
void model_ck( void )
{
	if( dbg_mode == 0 || dbg_mode == 4 ){
		if( ( sw_work[0].now & KEY_A1 ) && ( sw_work[0].now & KEY_A2 ) && ( sw_work[0].on & KEY_HP ) ){
			dbg_mode ^= 4 ;
			if( dbg_mode & 4 ){
				model_mode = 0 ;
				model_step = 0 ;
				model_back_no = 0 ;
				unk_0C2FB43B = 0 ;
			} else {
				model_exit() ;
			}
		}
	}
}

/*==============================================================*/
/*			The mode, and the background colour					*/
/*==============================================================*/
static void model_back_set( void )
{
	model_mode_tbl[model_mode]() ;
	back_color_set2( model_back[model_back_no][1], model_back[model_back_no][0] ) ;
}

/*==============================================================*/
static void model_view_init( void )
{
	model_mode = 1 ;
	model_step = 0 ;
	model_back_no = 7 ;
	unk_0C2FB43B = 0 ;
	light_dir_set( &model_light ) ;
	light_amb_set( 1.0f ) ;
	fog_init() ;
	bright_init() ;
	stream_stop() ;
	sound_all_off() ;
	se_all_off() ;
}

/*==============================================================*/
/*			The list of models: a choice by the keys, 18 to		*/
/*			a column											*/
/*==============================================================*/
static void model_sel( void )
{
	int col, row ;
	Uint16 pad ;
	int i ;
	int c, r ;

	col = model_no / 18 ;
	row = model_no % 18 ;
	pad = pad_get( 1 ) ;
	c = sel_lr( pad, col, 1 ) ;
	r = sel_ud( pad, row, 17 ) ;
	model_no = c * 18 + r ;
	if( sw_work[0].on & KEY_ATCK ){
		model_mode = 2 ;
		model_step = 0 ;
	}
	STrPrint( 2, 5, 4, "Select Model" ) ;
	STrPrint( 2, 7, 4, "UP,DOWN... select" ) ;
	STrPrint( 2, 8, 4, "SHOT...... ok" ) ;
	STrPrint( c * 8 + 17, r + 5, 2, "*" ) ;
	for( i = 0 ; i < MODEL_NUM ; i ++ ){
		STrPrint( i / 18 * 8 + 18, i % 18 + 5, 0, "%s", model_name[i] ) ;
	}
}

/*==============================================================*/
/*			The camera, as far as the model's size				*/
/*==============================================================*/
static void model_cam_reset( void )
{
	if( model_no == 0 ) cam_len = 50.0f ;
	else if( model_no >= 30 ) cam_len = 100.0f ;
	else cam_len = 1000.0f ;
	cam_elev = 0.0f ;
	cam_yaw = 0.0f ;
	cam_ofs = model_cam0 ;
}

/*==============================================================*/
static void model_view_step( void )
{
	model_step_tbl[model_step]() ;
}

/*==============================================================*/
static void model_view_load( void )
{
	Texture_Init() ;
	if( model_kind[model_no] == 1 ) stage_no = model_no + 0xff ;
	model_load( model_kind[model_no] ) ;
	model_cur = *model_set_tbl[model_no] ;
	obj_no = 0 ;
	help_no = 0 ;
	model_cam_reset() ;
	light_dir_set( &model_light ) ;
	light_amb_set( 1.0f ) ;
	fog_init() ;
	bright_init() ;
	model_step = 1 ;
}

/*==============================================================*/
/*			The view: the keys turn, move and zoom the			*/
/*			camera												*/
/*==============================================================*/
static void model_view( void )
{
	Uint16 pad, now, on, rep ;
	Sint32 mat, vtx ;

	pad = pad_get( 1 ) ;
	now = sw_work[0].now ;
	rep = sw_work[0].rep ;
	on = sw_work[0].on ;
	if( pad & 0x80 ){
		if( ++ help_no >= 3 ) help_no = 0 ;
	}
	if( now & KEY_LP ){
		if( now & KEY_RIGHT ){
			if( -- cam_yaw < 0.0f ) cam_yaw = 359.0f ;
		}
		if( now & KEY_LEFT ){
			if( ++ cam_yaw >= 360.0f ) cam_yaw = 0.0f ;
		}
		if( now & KEY_UP ){
			if( ++ cam_elev >= 90.0f ) cam_elev = 89.0f ;
		}
		if( now & KEY_DOWN ){
			if( -- cam_elev <= -90.0f ) cam_elev = -89.0f ;
		}
	} else if( now & KEY_LK ){
		if( now & KEY_RIGHT ) cam_ofs.x -= 0.5f ;
		if( now & KEY_LEFT ) cam_ofs.x += 0.5f ;
		if( now & KEY_UP ) cam_ofs.y -= 0.5f ;
		if( now & KEY_DOWN ) cam_ofs.y += 0.5f ;
	} else {
		obj_no = sel_lr( pad, obj_no, model_cur->cnt - 1 ) ;
		if( now & KEY_UP ){
			if( rep & KEY_UP ) cam_len -= 0.5f ;
			else cam_len -= 5.0f ;
			if( cam_len <= 2.0f ) cam_len = 2.0f ;
		}
		if( now & KEY_DOWN ){
			if( rep & KEY_DOWN ) cam_len += 0.5f ;
			else cam_len += 5.0f ;
			if( cam_len >= 10000.0f ) cam_len = 10000.0f ;
		}
	}
	if( on & KEY_HP ){
		if( ++ model_back_no >= 8 ) model_back_no = 0 ;
	}
	if( on & KEY_HK ) model_mode = 1 ;
	if( on & KEY_START ) model_cam_reset() ;
	SysTransSet( model_view_disp ) ;
	if( help_no == 0 || help_no == 1 ){
		STrPrint( 4, 5, 4, "model  = %s", model_name[model_no] ) ;
		STrPrint( 4, 6, 4, "number = %d", obj_no ) ;
		obj_mat_count( model_cur->obj[obj_no], &mat ) ;
		obj_vtx_count( model_cur->obj[obj_no], &vtx ) ;
		STrPrint( 4, 7, 4, "material num = %d", mat ) ;
		STrPrint( 4, 8, 4, "vertex num   = %d", vtx ) ;
		STrPrint( 22, 27, 4, "PRESS B BUTTON TO EXIT" ) ;
	}
}

/*==============================================================*/
/*			Draw the model with the camera, and the help		*/
/*==============================================================*/
static void model_view_disp( void )
{
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlShiftPerspective( 0.0f, 0.0f ) ;
	if( !( sw_work[0].now & KEY_A2 ) ) nlPerspectiveX( 0x1e94, 1.3333334f, 0.1f, 12000.0f ) ;
	else nlPerspectiveX( 0x4000, 1.3333334f, 0.1f, 12000.0f ) ;
	cam_pos = polar_pos2( cam_len, cam_elev, cam_yaw ) ;
	cam_pos.x += cam_ofs.x ;
	cam_pos.y += cam_ofs.y ;
	cam_pos.z += cam_ofs.z ;
	nlMatrixMode( 2 ) ;
	nlUnitMatrix() ;
	nlLookAt( &cam_pos, &cam_ofs, 0 ) ;
	nlMatrixMode( 1 ) ;
	nlUnitMatrix() ;
	nlObjPutSetFadeColor( 1.0f, 1.0f, 1.0f ) ;
	if( !( sw_work[0].now & KEY_A2 ) ){
		nlObjPutUseViewMatrix( model_cur->obj[obj_no] ) ;
		model_axis_disp() ;
	} else {
		nlObjPut( model_cur->obj[obj_no] ) ;
	}
	if( help_no == 1 ){
		STrPrint( 20, 5, 4, "number(-) .... LEFT" ) ;
		STrPrint( 20, 6, 4, "number(+) .... RIGHT" ) ;
		STrPrint( 20, 7, 4, "zoom in ...... UP" ) ;
		STrPrint( 20, 8, 4, "zoom out ..... DOWN" ) ;
		STrPrint( 20, 9, 4, "back color ... Y" ) ;
		STrPrint( 20, 10, 4, "camera rot ... X + UDLR" ) ;
		STrPrint( 20, 11, 4, "camera move .. A + UDLR" ) ;
		STrPrint( 20, 12, 4, "camera reset . START" ) ;
		STrPrint( 20, 13, 4, "menu disp .... L" ) ;
		STrPrint( 20, 14, 4, "cockpit view . R" ) ;
	}
}

/*==============================================================*/
/*			The axes, and a grid on the floor					*/
/*==============================================================*/
static void model_axis_disp( void )
{
	int i ;
	VEC3 b, a ;
	Float x ;

	nlLineSetThickness( 1.0f ) ;
	nlUnitMatrix() ;
	for( i = 0 ; i < 3 ; i ++ ){
		if( model_no == 0 ) nlSingleLineUseViewMatrix( &model_axis_small[i][0], &model_axis_small[i][1], model_axis_col[i] ) ;
		else nlSingleLineUseViewMatrix( &model_axis_big[i][0], &model_axis_big[i][1], model_axis_col[i] ) ;
	}
	for( i = -5 ; i <= 5 ; i ++ ){
		if( i == 0 ) continue ;
		b.x = x = ( Float )( i * 10 ) ;
		a.x = x ;
		b.y = 0.0f ;
		a.y = 0.0f ;
		b.z = -50.0f ;
		a.z = 50.0f ;
		nlSingleLineUseViewMatrix( &b, &a, 0xff00ffff ) ;
	}
	for( i = -5 ; i <= 5 ; i ++ ){
		if( i == 0 ) continue ;
		b.x = -50.0f ;
		a.x = 50.0f ;
		b.y = 0.0f ;
		a.y = 0.0f ;
		b.z = x = ( Float )( i * 10 ) ;
		a.z = x ;
		nlSingleLineUseViewMatrix( &b, &a, 0xff00ffff ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "model_view.c" End of File	*/
