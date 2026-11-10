/* The parts that ride the select scene's doors (kind 5 works), a module after sel_door. */
#include "player.h"
#include "select.h"

extern void sel_obj_pos_add( PLAYER_WORK *wk, Sint16 no, Float x, Float y );
void sel_door_follow_set( PLAYER_WORK *pl_w, Uint8 flag );

/* where the second door's parts stand */
static VEC3 sel_door_pos2_tbl[2] = {
	{ -110.0f, 400.0f, 310.0f },
	{ 110.0f, 400.0f, 310.0f },
} ;

static void sel_door_follow_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A part that rides a door: it takes the door's		*/
/*			place and turn										*/
/****************************************************************/
void sel_door_follow_set( PLAYER_WORK *pl_w, Uint8 flag )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->master_w = pl_w ;
	ew->ef_w = pl_w->ef_w ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_door_follow_main ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->dir = flag ;
	ew->obj = obj_dm01->obj[pl_w->type + 110] ;
	ew->pos = pl_w->pos ;
	ew->rot[0] = pl_w->rot[0] ;
	ew->rot[1] = pl_w->rot[1] ;
	ew->rot[2] = pl_w->rot[2] ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_BRIGHT ;
	if( flag ) sel_obj_pos_add( ew, ew->type + 110, 0.1875f, 0.0f ) ;
}

static void sel_door_follow( PLAYER_WORK *ew );
static void sel_door_follow_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_door_follow_main( PLAYER_WORK *ew )
{
	static void ( *ef05_door_follow_jmp[] )( PLAYER_WORK *ew ) = {
		sel_door_follow, sel_door_follow, sel_door_follow, sel_door_follow, sel_door_follow, sel_door_follow_free,
	} ;

	ef05_door_follow_jmp[ew->master_w->routine_0]( ew ) ;
}

/*==============================================================*/
static void sel_door_follow( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->master_w->disp_flag ;
	ew->rot[2] = ew->master_w->rot[2] ;
}

/*--------------------------------------------------------------*/
static void sel_door_follow_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/* where each side's three lights stand */
static VEC3 sel_light_pos_tbl[2][3] = {
	{
		{ -35.0f, -295.0f, -29.9f },
		{ 0.0f, -325.0f, 0.1f },
		{ 35.0f, -355.0f, 30.1f },
	},
	{
		{ 35.0f, -295.0f, -29.9f },
		{ 0.0f, -325.0f, 0.1f },
		{ -35.0f, -355.0f, 30.1f },
	},
} ;

static void sel_light_main( PLAYER_WORK *ew );
static void sel_flare_main( PLAYER_WORK *ew );
static void sel_flare_uv( PLAYER_WORK *w );
extern void obj_func_set( PLAYER_WORK *w, void ( *func )( PLAYER_WORK *w ) );

static void sel_light_set( PLAYER_WORK *pl_w );

/****************************************************************/
/*			The three lights of a door, each with a glow of		*/
/*			its own												*/
/****************************************************************/
void sel_light_make( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *tw = pl_w->ef_w ;
	Sint16 i ;

	for( i = 0 ; i < 3 ; i ++ ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
		ew->master_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->ef_w = pl_w->ef_w ;
		ew->disp_flag = 1 ;
		ew->work_func = sel_light_main ;
		ew->type = pl_w->type ;
		ew->sub_type = i ;
		ew->pl_type = tw->load_type ;
		ew->obj = obj_dm01->obj[i + 99] ;
		ew->pos = sel_light_pos_tbl[ew->type][i] ;
		ew->rot[0] = 0x2000 ;
		ew->rot[1] = 0 ;
		ew->rot[2] = 0 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_SCALE | OBJ_BRIGHT ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 0.01f ;
		ew->scale.z = 1.0f ;
		sel_light_set( ew ) ;
	}
}

/*==============================================================*/
static void sel_light_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->master_w = pl_w ;
	ew->base_mtx = pl_w->base_mtx ;
	ew->ef_w = pl_w->ef_w ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_flare_main ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->pl_type = pl_w->pl_type ;
	if( Sys->cpu_level_type ) ew->obj = obj_dm01->obj[108] ;
	else ew->obj = obj_dm01->obj[102] ;
	ew->pos = sel_light_pos_tbl[ew->type][ew->sub_type] ;
	ew->rot[0] = 0x2000 ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_SCALE | OBJ_BRIGHT ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 0.01f ;
	ew->scale.z = 1.0f ;
	obj_func_set( ew, sel_flare_uv ) ;
}

/* where each side's two glows stand */
static VEC3 sel_glow_pos_tbl[2][2] = {
	{
		{ 24.0f, -345.0f, 20.1f },
		{ -24.0f, -305.0f, -19.9f },
	},
	{
		{ -24.0f, -345.0f, 20.1f },
		{ 24.0f, -305.0f, -19.9f },
	},
} ;

static void sel_glow_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The two glows of the second door					*/
/****************************************************************/
void sel_glow_make( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint16 i ;

	for( i = 0 ; i < 2 ; i ++ ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
		ew->master_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->ef_w = pl_w->ef_w ;
		ew->disp_flag = 1 ;
		ew->work_func = sel_glow_main ;
		ew->type = pl_w->type ;
		ew->sub_type = i ;
		ew->obj = obj_dm01->obj[107 - i] ;
		ew->pos = sel_glow_pos_tbl[ew->type][i] ;
		ew->rot[0] = 0x2000 ;
		ew->rot[1] = 0 ;
		ew->rot[2] = 0 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_SCALE | OBJ_BRIGHT ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 0.01f ;
		ew->scale.z = 1.0f ;
	}
}

/* the texture offsets of each glow's thirteen steps */
static Float sel_glow_uv_tbl[13][2] = {
	{ 0.0625f, 1.0f },
	{ 0.0625f, 0.875f },
	{ 0.0625f, 0.75f },
	{ 0.0625f, DOT_X( 0.375f ) },
	{ 0.0625f, 0.5f },
	{ 0.0625f, 0.375f },
	{ 0.0625f, 0.25f },
	{ 0.0625f, 0.125f },
	{ 0.5625f, 1.0f },
	{ 0.5625f, 0.875f },
	{ 0.5625f, 0.75f },
	{ 0.5625f, DOT_X( 0.375f ) },
	{ 0.5625f, 0.5f },
} ;

static Sint8 sel_glow_step_tbl[60][3] = {
	4, 0, 2, 1, 6, 7, 4, 0, 12, 4, 0, 12,
	1, 6, 5, 1, 5, 0, 0, 4, 2, 2, 4, 3,
	4, 0, 12, 0, 5, 12, 4, 3, 6, 0, 4, 3,
	10, 2, 4, 1, 3, 4, 2, 5, 11, 5, 4, 0,
	6, 4, 0, 8, 3, 0, 2, 4, 12, 4, 10, 9,
	8, 0, 4, 8, 9, 12, 0, 4, 2, 0, 4, 5,
	0, 2, 1, 0, 2, 1, 0, 2, 1, 0, 4, 2,
	0, 4, 12, 0, 5, 12, 0, 4, 2, 0, 4, 5,
	2, 1, 5, 0, 4, 12, 3, 0, 2, 0, 4, 5,
	4, 3, 2, 0, 1, 4, 0, 5, 2, 4, 0, 2,
	0, 2, 11, 1, 3, 5, 0, 2, 5, 2, 0, 11,
	0, 10, 11, 0, 2, 12, 0, 4, 5, 1, 0, 11,
	6, 4, 1, 0, 5, 1, 3, 4, 11, 0, 4, 11,
	0, 11, 1, 1, 4, 11, 10, 3, 11, 2, 4, 11,
	1, 4, 2, 1, 11, 5, 0, 4, 12, 0, 0, 0,
} ;

extern void sel_halo_set( PLAYER_WORK *ew, Uint8 flag );
extern void sel_mark_make( PLAYER_WORK *ew );
extern void sel_nop( PLAYER_WORK *ew );

static void sel_light_follow( PLAYER_WORK *ew );
static void sel_light_in( PLAYER_WORK *ew );
static void sel_light_glow( PLAYER_WORK *ew );
static void sel_light_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_light_main( PLAYER_WORK *ew )
{
	static void ( *ef05_light_jmp[] )( PLAYER_WORK *ew ) = {
		sel_light_follow, sel_light_follow, sel_light_in, sel_light_glow,
		sel_light_follow, sel_light_free,
	} ;

	ef05_light_jmp[ew->master_w->routine_0]( ew ) ;
}

/*--------------------------------------------------------------*/
static void sel_light_follow( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->master_w->disp_flag ;
}

/*==============================================================*/
/*			The light grows to full height as its door comes	*/
/*			in													*/
/*==============================================================*/
static void sel_light_in( PLAYER_WORK *ew )
{
	switch( ew->routine_1 ){
	case 0:
		ew->routine_1 ++ ;
		sel_halo_set( ew, 0 ) ;
		sel_mark_make( ew ) ;
		sel_nop( ew ) ;
	case 1:
		ew->scale.y += 0.1f ;
		if( ew->scale.y >= 1.0f ){
			ew->routine_1 ++ ;
			ew->scale.y = 1.0f ;
		}
		break ;
	case 2:
		break ;
	}
}

/*==============================================================*/
/*			The light of the side's own personal place			*/
/*			glows, the others dim								*/
/*==============================================================*/
static void sel_light_glow( PLAYER_WORK *ew )
{
	ew->rot[2] = ew->master_w->rot[2] ;
	switch( ew->routine_2 ){
	case 0:
		if( ew->ef_w->pl_personal == ew->sub_type ) ew->routine_2 = 1 ;
		else ew->routine_2 = 2 ;
		break ;
	case 1:
		ew->scale.y += 0.1f ;
		if( ew->scale.y >= 1.0f ){
			ew->routine_2 = 99 ;
			ew->scale.y = 1.0f ;
		}
		break ;
	case 2:
		ew->scale.y -= 0.1f ;
		if( ew->scale.y <= 0.0f ){
			ew->routine_2 = 99 ;
			ew->scale.y = 0.01f ;
		}
		break ;
	case 99:
		break ;
	}
}

/*--------------------------------------------------------------*/
static void sel_light_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

static void sel_glow_in( PLAYER_WORK *ew );
static void sel_glow_pick( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_glow_main( PLAYER_WORK *ew )
{
	static void ( *sel_glow_jmp[] )( PLAYER_WORK *ew ) = {
		sel_light_follow, sel_light_follow, sel_glow_in, sel_glow_pick,
		sel_light_follow, sel_light_free,
	} ;

	sel_glow_jmp[ew->master_w->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The glow grows to full height as its door comes		*/
/*			in													*/
/*==============================================================*/
static void sel_glow_in( PLAYER_WORK *ew )
{
	switch( ew->routine_1 ){
	case 0:
		ew->routine_1 ++ ;
		sel_halo_set( ew, 1 ) ;
	case 1:
		ew->scale.y += 0.1f ;
		if( ew->scale.y >= 1.0f ){
			ew->routine_1 ++ ;
			ew->scale.y = 1.0f ;
		}
		break ;
	case 2:
		break ;
	}
}

/*==============================================================*/
/*			The glow of the setting's own level lights, the		*/
/*			others dim											*/
/*==============================================================*/
static void sel_glow_pick( PLAYER_WORK *ew )
{
	ew->rot[2] = ew->master_w->rot[2] ;
	switch( ew->routine_2 ){
	case 0:
		if( Sys->game_speed == ew->sub_type ) ew->routine_2 = 1 ;
		else ew->routine_2 = 2 ;
		break ;
	case 1:
		ew->scale.y += 0.1f ;
		if( ew->scale.y >= 1.0f ){
			ew->routine_2 = 99 ;
			ew->scale.y = 1.0f ;
		}
		break ;
	case 2:
		ew->scale.y -= 0.1f ;
		if( ew->scale.y <= 0.0f ){
			ew->routine_2 = 99 ;
			ew->scale.y = 0.01f ;
		}
		break ;
	case 99:
		break ;
	}
}

static void sel_flare_follow( PLAYER_WORK *ew );

/*==============================================================*/
/*			The glow of a light follows the light				*/
/*==============================================================*/
static void sel_flare_main( PLAYER_WORK *ew )
{
	static void ( *sel_flare_jmp[] )( PLAYER_WORK *ew ) = {
		sel_flare_follow, sel_light_follow, sel_flare_follow, sel_flare_follow,
		sel_flare_follow, sel_light_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w->master_w ;

	sel_flare_jmp[pl_w->routine_0]( ew ) ;
}

/*==============================================================*/
static void sel_flare_follow( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->master_w->disp_flag ;
	ew->scale = ew->master_w->scale ;
}

/*==============================================================*/
/*			The glow's picture is the step of its light in		*/
/*			the sheet											*/
/*==============================================================*/
static void sel_flare_uv( PLAYER_WORK *w )
{
	Sint8 no = sel_glow_step_tbl[w->pl_type][w->sub_type] ;
	Float x = sel_glow_uv_tbl[no][0] - sel_glow_uv_tbl[0][0] ;
	Float y = sel_glow_uv_tbl[0][1] - sel_glow_uv_tbl[no][1] ;

	if( Sys->cpu_level_type ) sel_obj_pos_add( w, 109, x, y ) ;
	else sel_obj_pos_add( w, 103, x, y ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_light.c" End of File	*/
