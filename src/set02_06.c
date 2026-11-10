/* Stage 02's object 6: a set of models (objects 8 to 19 of the stage's file) that turn, and swing
   between two sets of angles, some with a second model riding on them. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* how far each moves or turns in a frame, on each axis */
static VEC3 set02_06_slide_a_step ;
static VEC3 set02_06_slide_a_arm_step ;
static VEC3 set02_06_swing_b_step ;
static VEC3 set02_06_swing_a_step ;
static VEC3 set02_06_swing_a_arm_step ;
static VEC3 set02_06_slide_b_arm_step ;
static VEC3 set02_06_slide_b_step ;
static Uint8 unk_0C2FB694[652] ;

static const VEC3 set02_06_unused_vec = { 0.0f, 0.0f, 0.0f } ;

static const VEC3 set02_06_spin0_pos = { -298.0f, 0.0f, -436.0f } ;
static const VEC3 set02_06_swing_a_pos = { -560.999f, 127.231f, -743.256f } ;
static const VEC3 set02_06_swing_a_tbl[2] = {
	{ -17.0f, 0.0f, 0.0f },
	{ 17.0f, 0.0f, 0.0f },
} ;
static const VEC3 set02_06_swing_a_arm_tbl[2] = {
	{ 17.0f, 0.0f, 0.0f },
	{ -17.0f, 0.0f, 0.0f },
} ;
static const VEC3 set02_06_swing_a_arm_pos = { 2.581f, 8.329f, 454.496f } ;
static const VEC3 set02_06_slide_a_tbl[2] = {
	{ -995.0f, 100.0f, -1031.0f },
	{ -995.0f, -190.0f, -1031.0f },
} ;
static const VEC3 set02_06_slide_a_arm_pos = { -82.921f, 305.0f, 8.2f } ;
static const VEC3 set02_06_slide_a_arm_tbl[2] = {
	{ -19.9389f, -20.0f, 0.0f },
	{ 19.2514f, -20.0f, 0.0f },
} ;
static const VEC3 set02_06_swing_b_pos = { -1173.4f, 849.157f, -773.845f } ;
static const VEC3 set02_06_swing_b_tbl[2] = {
	{ 77.9223f, -20.0f, 0.0f },
	{ 20.3973f, -20.0f, 0.0f },
} ;
static const VEC3 set02_06_spin1_pos = { -1243.41f, 158.793f, -364.339f } ;
static const VEC3 set02_06_spin1_rot = { 0.0f, 72.8113f, 0.0f } ;
static const VEC3 set02_06_spin2_pos = { -1240.75f, 838.199f, -797.21f } ;
static const VEC3 set02_06_spin2_rot = { 0.0f, -21.1994f, 0.0f } ;
static const VEC3 set02_06_spin3_pos = { -1302.61f, 838.199f, -822.564f } ;
static const VEC3 set02_06_spin3_rot = { 0.0f, -20.6265f, 0.0f } ;
static const VEC3 set02_06_spin4_pos = { -1396.16f, 831.2f, -858.146f } ;
static const VEC3 set02_06_spin4_rot = { 0.0f, -21.7724f, 0.0f } ;
static const VEC3 set02_06_slide_b_tbl[2] = {
	{ -849.8f, -50.0f, -384.64f },
	{ -849.8f, 120.0f, -384.64f },
} ;
static const VEC3 set02_06_unused_vec2 = { 0.0f, 0.0f, 0.0f } ;
static const VEC3 set02_06_slide_b_arm_pos = { -12.968f, 198.234f, -9.922f } ;
static const VEC3 set02_06_slide_b_arm_tbl[2] = {
	{ 14.0f, 63.0f, 0.0f },
	{ -28.0f, 63.0f, 0.0f },
} ;

/*==============================================================*/
static void set02_06_spin0( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->rot[0] = ( Sint32 )( ew->timer0 * 98304.0f / 360.0f + 0.5f ) & 0xffff ;
		if( ++ ew->timer0 >= 240 ) ew->timer0 = 0 ;
		break ;
	}
}

/*==============================================================*/
static void set02_06_spin0_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_spin0 ;
		ew->obj = obj_stg->obj[8] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_06_spin0_pos ;
	}
}

static void set02_06_swing_a_init( PLAYER_WORK *ew ) ;
static void set02_06_swing_a_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_06_swing_a_main( PLAYER_WORK *ew )
{
	static void ( *set02_06_swing_a_jmp[] )( PLAYER_WORK *ew ) = {
		set02_06_swing_a_init, set02_06_swing_a_move,
	} ;

	set02_06_swing_a_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_06_swing_a_init( PLAYER_WORK *ew )
{
	Float frames ;
	ew->spd = set02_06_swing_a_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = set02_06_swing_a_tbl[ew->routine_1] ;
	frames = 150.0f ;
	set02_06_swing_a_step.x = ( ew->acc.x - ew->spd.x ) / frames ;
	set02_06_swing_a_step.y = ( ew->acc.y - ew->spd.y ) / frames ;
	set02_06_swing_a_step.z = ( ew->acc.z - ew->spd.z ) / frames ;
	ew->routine_0 = 1 ;
	set02_06_swing_a_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_06_swing_a_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 150 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[2] = DEG_ANG( ew->spd.z ) ;
	ew->rot[0] += DEG_ANG( set02_06_swing_a_step.x * ew->timer0 ) ;
	ew->rot[1] += DEG_ANG( set02_06_swing_a_step.y * ew->timer0 ) ;
	ew->rot[2] += DEG_ANG( set02_06_swing_a_step.z * ew->timer0 ) ;
}

static void set02_06_swing_a_arm_set( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void set02_06_swing_a_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_swing_a_main ;
		ew->obj = obj_stg->obj[9] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_06_swing_a_pos ;
		set02_06_swing_a_arm_set( ew ) ;
	}
}

static void set02_06_swing_a_arm_init( PLAYER_WORK *ew ) ;
static void set02_06_swing_a_arm_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_06_swing_a_arm_main( PLAYER_WORK *ew )
{
	static void ( *set02_06_swing_a_arm_jmp[] )( PLAYER_WORK *ew ) = {
		set02_06_swing_a_arm_init, set02_06_swing_a_arm_move,
	} ;

	set02_06_swing_a_arm_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_06_swing_a_arm_init( PLAYER_WORK *ew )
{
	Float frames ;
	ew->spd = set02_06_swing_a_arm_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = set02_06_swing_a_arm_tbl[ew->routine_1] ;
	frames = 150.0f ;
	set02_06_swing_a_arm_step.x = ( ew->acc.x - ew->spd.x ) / frames ;
	set02_06_swing_a_arm_step.y = ( ew->acc.y - ew->spd.y ) / frames ;
	set02_06_swing_a_arm_step.z = ( ew->acc.z - ew->spd.z ) / frames ;
	ew->routine_0 = 1 ;
	set02_06_swing_a_arm_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_06_swing_a_arm_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 150 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[2] = DEG_ANG( ew->spd.z ) ;
	ew->rot[0] += DEG_ANG( set02_06_swing_a_arm_step.x * ew->timer0 ) ;
	ew->rot[1] += DEG_ANG( set02_06_swing_a_arm_step.y * ew->timer0 ) ;
	ew->rot[2] += DEG_ANG( set02_06_swing_a_arm_step.z * ew->timer0 ) ;
}

/*==============================================================*/
static void set02_06_swing_a_arm_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_swing_a_arm_main ;
		ew->obj = obj_stg->obj[10] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_06_swing_a_arm_pos ;
		ew->rot[0] = DEG_ANG( set02_06_swing_a_arm_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( set02_06_swing_a_arm_tbl[0].y ) ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

static void set02_06_slide_a_init( PLAYER_WORK *ew ) ;
static void set02_06_slide_a_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_06_slide_a_main( PLAYER_WORK *ew )
{
	static void ( *set02_06_slide_a_jmp[] )( PLAYER_WORK *ew ) = {
		set02_06_slide_a_init, set02_06_slide_a_move,
	} ;

	set02_06_slide_a_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_06_slide_a_init( PLAYER_WORK *ew )
{
	Float frames ;
	ew->spd = set02_06_slide_a_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = set02_06_slide_a_tbl[ew->routine_1] ;
	frames = 100.0f ;
	set02_06_slide_a_step.x = ( ew->acc.x - ew->spd.x ) / frames ;
	set02_06_slide_a_step.y = ( ew->acc.y - ew->spd.y ) / frames ;
	set02_06_slide_a_step.z = ( ew->acc.z - ew->spd.z ) / frames ;
	ew->routine_0 = 1 ;
	set02_06_slide_a_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_06_slide_a_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 100 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->pos.x = ew->spd.x ;
	ew->pos.y = ew->spd.y ;
	ew->pos.z = ew->spd.z ;
	ew->pos.x += set02_06_slide_a_step.x * ew->timer0 ;
	ew->pos.y += set02_06_slide_a_step.y * ew->timer0 ;
	ew->pos.z += set02_06_slide_a_step.z * ew->timer0 ;
}

static void set02_06_slide_a_arm_set( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void set02_06_slide_a_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_slide_a_main ;
		ew->obj = obj_stg->obj[13] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set02_06_slide_a_tbl[0] ;
		set02_06_slide_a_arm_set( ew ) ;
	}
}

static void set02_06_slide_a_arm_init( PLAYER_WORK *ew ) ;
static void set02_06_slide_a_arm_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_06_slide_a_arm_main( PLAYER_WORK *ew )
{
	static void ( *set02_06_slide_a_arm_jmp[] )( PLAYER_WORK *ew ) = {
		set02_06_slide_a_arm_init, set02_06_slide_a_arm_move,
	} ;

	set02_06_slide_a_arm_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_06_slide_a_arm_init( PLAYER_WORK *ew )
{
	Float frames ;
	ew->spd = set02_06_slide_a_arm_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = set02_06_slide_a_arm_tbl[ew->routine_1] ;
	frames = 100.0f ;
	set02_06_slide_a_arm_step.x = ( ew->acc.x - ew->spd.x ) / frames ;
	set02_06_slide_a_arm_step.y = ( ew->acc.y - ew->spd.y ) / frames ;
	set02_06_slide_a_arm_step.z = ( ew->acc.z - ew->spd.z ) / frames ;
	ew->routine_0 = 1 ;
	set02_06_slide_a_arm_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_06_slide_a_arm_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 100 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[2] = DEG_ANG( ew->spd.z ) ;
	ew->rot[0] += DEG_ANG( set02_06_slide_a_arm_step.x * ew->timer0 ) ;
	ew->rot[1] += DEG_ANG( set02_06_slide_a_arm_step.y * ew->timer0 ) ;
	ew->rot[2] += DEG_ANG( set02_06_slide_a_arm_step.z * ew->timer0 ) ;
}

/*==============================================================*/
static void set02_06_slide_a_arm_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_slide_a_arm_main ;
		ew->obj = obj_stg->obj[14] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_06_slide_a_arm_pos ;
		ew->rot[0] = DEG_ANG( set02_06_slide_a_arm_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( set02_06_slide_a_arm_tbl[0].y ) ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

static void set02_06_swing_b_init( PLAYER_WORK *ew ) ;
static void set02_06_swing_b_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_06_swing_b_main( PLAYER_WORK *ew )
{
	static void ( *set02_06_swing_b_jmp[] )( PLAYER_WORK *ew ) = {
		set02_06_swing_b_init, set02_06_swing_b_move,
	} ;

	set02_06_swing_b_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_06_swing_b_init( PLAYER_WORK *ew )
{
	Float frames ;
	ew->spd = set02_06_swing_b_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = set02_06_swing_b_tbl[ew->routine_1] ;
	frames = 100.0f ;
	set02_06_swing_b_step.x = ( ew->acc.x - ew->spd.x ) / frames ;
	set02_06_swing_b_step.y = ( ew->acc.y - ew->spd.y ) / frames ;
	set02_06_swing_b_step.z = ( ew->acc.z - ew->spd.z ) / frames ;
	ew->routine_0 = 1 ;
	set02_06_swing_b_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_06_swing_b_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 100 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[2] = DEG_ANG( ew->spd.z ) ;
	ew->rot[0] += DEG_ANG( set02_06_swing_b_step.x * ew->timer0 ) ;
	ew->rot[1] += DEG_ANG( set02_06_swing_b_step.y * ew->timer0 ) ;
	ew->rot[2] += DEG_ANG( set02_06_swing_b_step.z * ew->timer0 ) ;
}

/*==============================================================*/
static void set02_06_swing_b_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_swing_b_main ;
		ew->obj = obj_stg->obj[15] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_06_swing_b_pos ;
		ew->rot[0] = DEG_ANG( set02_06_swing_b_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( set02_06_swing_b_tbl[0].y ) ;
	}
}

/*==============================================================*/
static void set02_06_spin1( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->rot[0] = ( Sint32 )( ew->timer0 * 32768.0f / 360.0f + 0.5f ) & 0xffff ;
		if( ++ ew->timer0 >= 720 ) ew->timer0 = 0 ;
		break ;
	}
}

/*==============================================================*/
static void set02_06_spin1_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_spin1 ;
		ew->obj = obj_stg->obj[16] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_06_spin1_pos ;
		ew->rot[1] = DEG_ANG( set02_06_spin1_rot.y ) ;
	}
}

/*==============================================================*/
static void set02_06_spin2( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->rot[0] = DEG_ANG( ew->timer0 ) ;
		ew->timer0 += 3 ;
		if( ew->timer0 >= 360 ) ew->timer0 %= 360 ;
		break ;
	}
}

/*==============================================================*/
static void set02_06_spin2_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_spin2 ;
		ew->obj = obj_stg->obj[17] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_06_spin2_pos ;
		ew->rot[1] = DEG_ANG( set02_06_spin2_rot.y ) ;
	}
}

static void set02_06_slide_b_init( PLAYER_WORK *ew ) ;
static void set02_06_slide_b_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_06_slide_b_main( PLAYER_WORK *ew )
{
	static void ( *set02_06_slide_b_jmp[] )( PLAYER_WORK *ew ) = {
		set02_06_slide_b_init, set02_06_slide_b_move,
	} ;

	set02_06_slide_b_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_06_slide_b_init( PLAYER_WORK *ew )
{
	Float frames ;
	ew->spd = set02_06_slide_b_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = set02_06_slide_b_tbl[ew->routine_1] ;
	frames = 60.0f ;
	set02_06_slide_b_step.x = ( ew->acc.x - ew->spd.x ) / frames ;
	set02_06_slide_b_step.y = ( ew->acc.y - ew->spd.y ) / frames ;
	set02_06_slide_b_step.z = ( ew->acc.z - ew->spd.z ) / frames ;
	ew->routine_0 = 1 ;
	set02_06_slide_b_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_06_slide_b_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 60 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->pos.x = ew->spd.x ;
	ew->pos.y = ew->spd.y ;
	ew->pos.z = ew->spd.z ;
	ew->pos.x += set02_06_slide_b_step.x * ew->timer0 ;
	ew->pos.y += set02_06_slide_b_step.y * ew->timer0 ;
	ew->pos.z += set02_06_slide_b_step.z * ew->timer0 ;
}

static void set02_06_slide_b_arm_set( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void set02_06_slide_b_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_slide_b_main ;
		ew->obj = obj_stg->obj[11] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set02_06_slide_b_tbl[0] ;
		set02_06_slide_b_arm_set( ew ) ;
	}
}

static void set02_06_slide_b_arm_init( PLAYER_WORK *ew ) ;
static void set02_06_slide_b_arm_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_06_slide_b_arm_main( PLAYER_WORK *ew )
{
	static void ( *set02_06_slide_b_arm_jmp[] )( PLAYER_WORK *ew ) = {
		set02_06_slide_b_arm_init, set02_06_slide_b_arm_move,
	} ;

	set02_06_slide_b_arm_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_06_slide_b_arm_init( PLAYER_WORK *ew )
{
	Float frames ;
	ew->spd = set02_06_slide_b_arm_tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = set02_06_slide_b_arm_tbl[ew->routine_1] ;
	frames = 60.0f ;
	set02_06_slide_b_arm_step.x = ( ew->acc.x - ew->spd.x ) / frames ;
	set02_06_slide_b_arm_step.y = ( ew->acc.y - ew->spd.y ) / frames ;
	set02_06_slide_b_arm_step.z = ( ew->acc.z - ew->spd.z ) / frames ;
	ew->routine_0 = 1 ;
	set02_06_slide_b_arm_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_06_slide_b_arm_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 60 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[2] = DEG_ANG( ew->spd.z ) ;
	ew->rot[0] += DEG_ANG( set02_06_slide_b_arm_step.x * ew->timer0 ) ;
	ew->rot[1] += DEG_ANG( set02_06_slide_b_arm_step.y * ew->timer0 ) ;
	ew->rot[2] += DEG_ANG( set02_06_slide_b_arm_step.z * ew->timer0 ) ;
}

/*==============================================================*/
static void set02_06_slide_b_arm_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_slide_b_arm_main ;
		ew->obj = obj_stg->obj[12] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_06_slide_b_arm_pos ;
		ew->rot[0] = DEG_ANG( set02_06_slide_b_arm_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( set02_06_slide_b_arm_tbl[0].y ) ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
static void set02_06_spin3( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->rot[0] = DEG_ANG( ew->timer0 ) ;
		ew->timer0 += 2 ;
		if( ew->timer0 >= 360 ) ew->timer0 %= 360 ;
		break ;
	}
}

/*==============================================================*/
static void set02_06_spin3_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_spin3 ;
		ew->obj = obj_stg->obj[18] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_06_spin3_pos ;
		ew->rot[1] = DEG_ANG( set02_06_spin3_rot.y ) ;
	}
}

/*==============================================================*/
static void set02_06_spin4( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->rot[0] = DEG_ANG( ew->timer0 ) ;
		ew->timer0 += 1 ;
		if( ew->timer0 >= 360 ) ew->timer0 %= 360 ;
		break ;
	}
}

/*==============================================================*/
static void set02_06_spin4_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_06_spin4 ;
		ew->obj = obj_stg->obj[19] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_06_spin4_pos ;
		ew->rot[1] = DEG_ANG( set02_06_spin4_rot.y ) ;
	}
}

/****************************************************************/
void set02_06_set( void )
{
	set02_06_spin0_set( 8 ) ;
	set02_06_swing_a_set( 9 ) ;
	set02_06_slide_a_set( 13 ) ;
	set02_06_swing_b_set( 15 ) ;
	set02_06_spin1_set( 16 ) ;
	set02_06_spin2_set( 17 ) ;
	set02_06_spin3_set( 18 ) ;
	set02_06_spin4_set( 19 ) ;
	set02_06_slide_b_set( 11 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set02_06.c" End of File	*/
