/* Stage 08's first object: a wall that lights up and bursts into flying pieces, which land, bounce
   and are freed. The file ends with the entry stub set08_00_set. */
#include "player.h"
#include "pl18_def.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void pl18_se_req( Uint8 no ) ;
extern void s_set08_set( void ) ;

/* the uv scroll of a model */
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void obj_uv_clear( void *obj ) ;

static VEC3 set08_00_chip_pos_tbl[2] = {
	{ 0.0f, 975.0f, -500.0f },
	{ 0.0f, 975.0f, -1000.0f },
} ;
static VEC3 set08_00_glow_pos_tbl[2] = {
	{ 0.0f, 0.0f, -800.0f },
	{ -16.155f, -195.496f, -1000.0f },
} ;
static VEC3 set08_00_glow_rot_tbl[2] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 1.2342f, -3.8024f, -7.6063f },
} ;
static VEC3 set08_00_cover_pos_tbl[6] = {
	{ -970.0f, 970.5f, -370.0f },
	{ 970.0f, 970.5f, -370.0f },
	{ -780.25f, 820.5f, -170.0f },
	{ 780.25f, 820.5f, -170.0f },
	{ -270.5f, 670.5f, -190.5f },
	{ 270.5f, 670.5f, -190.5f },
} ;
static VEC3 set08_00_part_pos_tbl[8] = {
	{ 0.0f, 800.0f, -1200.0f },
	{ 0.0f, 1200.0f, -1200.0f },
	{ -270.0f, 390.0f, -260.0f },
	{ 270.0f, 390.0f, -260.0f },
	{ -780.0f, 490.75f, -180.0f },
	{ 780.0f, 490.75f, -180.0f },
	{ -780.0f, 800.0f, -1100.0f },
	{ 780.0f, 800.0f, -1100.0f },
} ;
static Float set08_00_part_floor_tbl[8][2] = {
	{ 270.0f, -200.0f },
	{ 330.0f, -200.0f },
	{ 280.0f, -200.0f },
	{ 280.0f, -200.0f },
	{ 380.0f, -200.0f },
	{ 380.0f, -200.0f },
	{ 600.0f, -200.0f },
	{ 600.0f, -200.0f },
} ;
static Sint32 set08_00_part_ang_tbl[8] = { 270, 180, 90, 180, 270, 0, 90, 180, } ;
typedef struct SET08_00_TURN {
	Float	x, z;
} SET08_00_TURN;
static SET08_00_TURN set08_00_part_turn_tbl[8] = {
	{ 0.0f, 0.5f },
	{ 0.5f, 0.0f },
	{ 0.0f, -0.5f },
	{ -0.5f, 0.0f },
	{ 0.0f, 0.5f },
	{ 0.5f, 0.0f },
	{ 0.0f, -0.5f },
	{ -0.5f, 0.0f },
} ;
static Float set08_00_part_spd_tbl[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, } ;
static Sint32 set08_00_unk_tbl1[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } ;

typedef struct SET08_00_BURST {
	Sint32	kind;
	VEC3	pos;
} SET08_00_BURST;
static SET08_00_BURST set08_00_burst_tbl[6][10] = {
	{
		{ 1, { 580.75f, 880.0f, -2150.0f } },
		{ 0, { 1420.5f, 500.15f, -410.5f } },
		{ 0, { 1550.0f, 570.5f, -870.5f } },
		{ 1, { 0.0f, 900.0f, -2170.0f } },
		{ 1, { -1000.0f, 1330.0f, -2300.0f } },
		{ 1, { -350.0f, 1960.0f, -2100.0f } },
		{ 0, { -1550.0f, 570.5f, -870.5f } },
		{ 1, { 1650.0f, 750.0f, -1350.0f } },
		{ 1, { 1060.75f, 520.0f, -2100.0f } },
		{ 1, { -780.25f, 1270.5f, -1070.0f } },
	},
	{
		{ 0, { 1500.0f, 520.5f, -1070.5f } },
		{ 0, { -1550.0f, 650.0f, 0.0f } },
		{ 0, { -1400.0f, 850.0f, -630.0f } },
		{ 1, { 0.0f, 1630.5f, -1240.3f } },
		{ 1, { -1650.0f, 750.0f, -1350.0f } },
		{ 1, { -1560.25f, 1120.6f, -2410.66f } },
		{ 1, { -870.0f, 800.01f, -2400.0f } },
		{ 1, { -280.75f, 900.0f, -2300.0f } },
		{ 1, { 350.0f, 1960.0f, -2100.0f } },
		{ 1, { 1650.0f, 2000.0f, -1900.0f } },
	},
	{
		{ 1, { 200.9f, 520.0f, -2080.0f } },
		{ 0, { -1400.0f, 400.0f, -980.0f } },
		{ 0, { -1420.0f, 450.0f, 0.0f } },
		{ 0, { 1360.5f, 850.0f, -180.0f } },
		{ 1, { 1400.0f, 400.0f, -980.0f } },
		{ 1, { -580.75f, 880.0f, -2150.0f } },
		{ 1, { 400.9f, 1300.65f, -2280.2f } },
		{ 1, { 1570.0f, 650.0f, -2500.0f } },
		{ 1, { -1650.0f, 2000.0f, -1900.0f } },
		{ 1, { 1520.0f, 1520.0f, -2250.0f } },
	},
	{
		{ 1, { -400.9f, 1300.6f, -2280.2f } },
		{ 1, { -780.25f, 1270.5f, -1070.0f } },
		{ 1, { -1500.0f, 520.5f, -1070.5f } },
		{ 0, { -1360.5f, 850.0f, -180.0f } },
		{ 0, { 1400.0f, 850.0f, -630.0f } },
		{ 1, { -1120.6f, 1800.3f, 600.0f } },
		{ 1, { -1520.0f, 1520.0f, -2250.0f } },
		{ 1, { 640.0f, 530.0f, -2140.0f } },
		{ 1, { 870.0f, 800.0f, -2400.0f } },
		{ 1, { 1000.0f, 1330.0f, -2300.0f } },
	},
	{
		{ 0, { -1550.0f, 1070.0f, -1300.0f } },
		{ 0, { -1420.0f, 500.0f, -410.0f } },
		{ 0, { 1650.0f, 1410.0f, -900.0f } },
		{ 0, { 1550.0f, 650.0f, 0.0f } },
		{ 1, { -200.9f, 520.0f, -2080.0f } },
		{ 1, { -840.5f, 950.2f, -2700.0f } },
		{ 1, { 1520.0f, 470.5f, -1660.2f } },
		{ 1, { 1150.0f, 860.0f, -2200.0f } },
		{ 1, { 800.0f, 1960.0f, -1900.0f } },
		{ 1, { -1570.0f, 650.0f, -2500.0f } },
	},
	{
		{ 0, { 1420.0f, 450.0f, 0.0f } },
		{ 1, { 1550.0f, 1070.3f, -1300.0f } },
		{ 1, { -640.0f, 530.0f, -2140.0f } },
		{ 1, { -800.0f, 1960.0f, -1900.0f } },
		{ 1, { 280.75f, 900.0f, -2300.0f } },
		{ 1, { -1520.0f, 470.5f, -1660.25f } },
		{ 1, { -1650.0f, 1410.0f, -900.0f } },
		{ 1, { 1560.25f, 1120.6f, -2410.7f } },
		{ 1, { 1120.5f, 1800.3f, -1600.0f } },
		{ 1, { -1150.0f, 860.0f, -2200.0f } },
	},
} ;

/* a piece's heading by kind, and how it turns each frame about x and z */
static Sint32 set08_00_ang_tbl[5] = { 270, 0, 90, 180, 45, } ;
static SET08_00_TURN set08_00_turn_tbl[5] = {
	{ 0.0f, 0.5f },
	{ 0.5f, 0.0f },
	{ 0.0f, -0.5f },
	{ -0.5f, 0.0f },
	{ 0.0f, 0.0f },
} ;
static VEC3 set08_00_spd = { 3.0f, 0.0f, 0.0f } ;
static Float set08_00_unused = 0.0f ;
static VEC3 set08_00_acc = { 0.0f, -0.5f, 0.0f } ;
static VEC3 set08_00_part_acc = { 0.0f, -0.2f, 0.0f } ;
static VEC3 set08_00_scroll_pos_tbl[3] = {
	{ -1600.0f, -19.0f, -1500.0f },
	{ 1600.0f, -19.0f, -1500.0f },
	{ 0.0f, -19.0f, -2000.0f },
} ;
static VEC3 set08_00_block_pos_tbl[4] = {
	{ 780.0f, 0.0f, -200.0f },
	{ 270.0f, 150.0f, -270.0f },
	{ -270.0f, 150.0f, -270.0f },
	{ -780.0f, 0.0f, -200.0f },
} ;
static Float set08_00_block_y_tbl[4] = { 200.0f, 100.0f, 100.0f, 200.0f, } ;
static Sint8 set08_00_block_obj_tbl[4] = { 70, 71, 69, 68, } ;
static VEC3 set08_00_lamp_pos_tbl[4] = {
	{ -780.0f, -19.0f, -200.0f },
	{ -270.0f, -19.0f, -260.0f },
	{ -760.0f, -19.0f, -350.0f },
	{ -260.0f, -19.0f, -420.0f },
} ;

/*==============================================================*/
/*			A piece thrown out: it flies, lands, bounces and	*/
/*			is freed											*/
/*==============================================================*/
static void set08_00_piece_a_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		ew->spd.x = set08_00_spd.x * nlSin( DEG_ANG( set08_00_ang_tbl[ew->type] ) ) ;
		ew->spd.y = 0.0f ;
		ew->spd.z = ( &set08_00_spd.x )[0] * nlCos( DEG_ANG( set08_00_ang_tbl[ew->type] ) ) ;
		ew->acc = set08_00_acc ;
		break ;
	case 1 :
		ew->routine_0 ++ ;
		land_ef_set( &ew->pos, 1 ) ;
		break ;
	case 2 :
		move_pos_spd( ew ) ;
		ew->rot[0] += DEG_ANG( set08_00_turn_tbl[ew->type].x ) ;
		ew->rot[2] += DEG_ANG( set08_00_turn_tbl[ew->type].z ) ;
		if( ew->pos.x > -250.0f && ew->pos.x < 250.0f ){
			if( ew->pos.y > 300.0f ) break ;
			ew->pos.y = 300.0f ;
		} else {
			if( ew->pos.y > 0.0f ) break ;
			ew->pos.y = 0.0f ;
		}
		ew->spd.y *= -0.3f ;
		if( ++ ew->timer0 > 2 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
		}
		break ;
	case 3 :
		if( ++ ew->timer0 >= 15 ) work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
/*			A piece of kind no at a place						*/
/****************************************************************/
void set08_00_piece_a_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_piece_a_main ;
		ew->obj = *( obj_stg->obj + no + 1 ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->type = no ;
		ew->pos = *pos ;
	}
}

/*==============================================================*/
/*			The same for the other set of models				*/
/*==============================================================*/
static void set08_00_piece_b_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		ew->spd.x = set08_00_spd.x * nlSin( DEG_ANG( set08_00_ang_tbl[ew->type] ) ) ;
		ew->spd.y = 0.0f ;
		ew->spd.z = ( &set08_00_spd.x )[0] * nlCos( DEG_ANG( set08_00_ang_tbl[ew->type] ) ) ;
		ew->acc = set08_00_acc ;
		break ;
	case 1 :
		ew->routine_0 ++ ;
		land_ef_set( &ew->pos, 1 ) ;
		break ;
	case 2 :
		move_pos_spd( ew ) ;
		ew->rot[0] += DEG_ANG( set08_00_turn_tbl[ew->type].x ) ;
		ew->rot[2] += DEG_ANG( set08_00_turn_tbl[ew->type].z ) ;
		if( ew->pos.x > -250.0f && ew->pos.x < 250.0f ){
			if( ew->pos.y > 300.0f ) break ;
			ew->pos.y = 300.0f ;
		} else {
			if( ew->pos.y > 0.0f ) break ;
			ew->pos.y = 0.0f ;
		}
		ew->spd.y *= -0.3f ;
		if( ++ ew->timer0 > 2 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
		}
		break ;
	case 3 :
		if( ++ ew->timer0 >= 15 ) work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
void set08_00_piece_b_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_piece_b_main ;
		ew->obj = *( obj_stg->obj + no + 42 ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->type = no ;
		ew->pos = *pos ;
	}
}

static void set08_00_lamp_set( int no ) ;

/*==============================================================*/
/* the wall waits, flashes white, slides and turns to its other pose, waits, then sinks away */
static void set08_00_glow_main( PLAYER_WORK *ew )
{
	Float t ;

	switch( ew->routine_0 ){
	case 0 :
		if( stage_work.event == 1 ){
			ew->routine_0 ++ ;
			ew->timer1 = 120 ;
		}
		break ;
	case 1 :
		if( ++ ew->timer0 > 240 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			ew->timer1 = 240 ;
			ew->obj_flag = ew->obj_flag | OBJ_FADE ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 1.0f ;
			set08_00_lamp_set( 0 ) ;
			set08_00_lamp_set( 1 ) ;
			pl18_se_req( 31 ) ;
		}
		break ;
	case 2 :
		t = ( Float )ew->timer0 / ( Float )ew->timer1 ;
		ew->pos.x = set08_00_glow_pos_tbl[0].x + t * ( set08_00_glow_pos_tbl[1].x - set08_00_glow_pos_tbl[0].x ) ;
		ew->pos.y = set08_00_glow_pos_tbl[0].y + t * ( set08_00_glow_pos_tbl[1].y - set08_00_glow_pos_tbl[0].y ) ;
		ew->pos.z = set08_00_glow_pos_tbl[0].z + t * ( set08_00_glow_pos_tbl[1].z - set08_00_glow_pos_tbl[0].z ) ;
		ew->rot[0] = DEG_ANG( set08_00_glow_rot_tbl[0].x + t * ( set08_00_glow_rot_tbl[1].x - set08_00_glow_rot_tbl[0].x ) ) ;
		ew->rot[1] = DEG_ANG( set08_00_glow_rot_tbl[0].y + t * ( set08_00_glow_rot_tbl[1].y - set08_00_glow_rot_tbl[0].y ) ) ;
		ew->rot[2] = DEG_ANG( set08_00_glow_rot_tbl[0].z + t * ( set08_00_glow_rot_tbl[1].z - set08_00_glow_rot_tbl[0].z ) ) ;
		ew->fade.r = 1.0f - t / 2.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f - t / 2.0f ;
		if( ew->timer0 % 2 ) ew->pos.x += 5.0f ;
		else ew->pos.x -= 5.0f ;
		if( ++ ew->timer0 >= ew->timer1 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			ew->fade.r = 0.5f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 0.5f ;
		}
		break ;
	case 3 :
		if( stage_work.event >= 2 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			ew->timer1 = 240 ;
			pl18_se_req( 31 ) ;
		}
		break ;
	case 4 :
		t = ( Float )ew->timer0 / ( Float )ew->timer1 ;
		ew->pos.y += -2.0f ;
		ew->fade.r = 0.5f + t / 2.0f ;
		ew->fade.g = 1.0f - t ;
		ew->fade.b = 0.5f - t / 2.0f ;
		if( ew->timer0 % 2 ) ew->pos.x += 5.0f ;
		else ew->pos.x -= 5.0f ;
		if( ++ ew->timer0 > 240 ){
			ew->routine_0 ++ ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 0.0f ;
			ew->fade.b = 0.0f ;
		}
		break ;
	case 5 :
		if( 0 ){
			if( stage_work.event > 240 ) if( stage_no == 0 || stage_no == 1 ) ;
		}
		break ;
	}
}

/****************************************************************/
void set08_00_glow_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_glow_main ;
		ew->obj = obj_stg->obj[5] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->pos = set08_00_glow_pos_tbl[0] ;
	}
}

/*==============================================================*/
/*			A cover slides in and splits into its pieces		*/
/*==============================================================*/
static void set08_00_cover_main( PLAYER_WORK *ew )
{
	int i ;

	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		ew->pos = set08_00_cover_pos_tbl[ew->type] ;
		break ;
	case 1 :
		if( stage_work.event == 1 ){
			ew->routine_0 ++ ;
			for( i = 0 ; i < 4 ; i ++ ) set08_00_piece_a_set( &ew->pos, i ) ;
		}
		break ;
	case 2 :
		work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
void set08_00_cover_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_cover_main ;
		ew->obj = *( obj_stg->obj + no + 34 ) ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->type = no ;
	}
}

/*==============================================================*/
/*			A part that falls from its place, bounces twice		*/
/*			on the floor and is freed							*/
/*==============================================================*/
static void set08_00_part_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		ew->pos = set08_00_part_pos_tbl[ew->type] ;
		break ;
	case 1 :
		if( stage_work.event == 1 ){
			ew->routine_0 ++ ;
			ew->spd.x = set08_00_part_spd_tbl[ew->type] * nlSin( DEG_ANG( set08_00_part_ang_tbl[ew->type] ) ) ;
			ew->spd.y = 0.0f ;
			ew->spd.z = set08_00_part_spd_tbl[ew->type] * nlCos( DEG_ANG( set08_00_part_ang_tbl[ew->type] ) ) ;
			ew->acc = set08_00_part_acc ;
		}
		break ;
	case 2 :
		move_pos_spd( ew ) ;
		ew->rot[0] += DEG_ANG( set08_00_part_turn_tbl[ew->type].x ) ;
		ew->rot[2] += DEG_ANG( set08_00_part_turn_tbl[ew->type].z ) ;
		if( ew->pos.y > set08_00_part_floor_tbl[ew->type][ew->timer0] ) break ;
		ew->spd.y *= -0.3f ;
		if( ++ ew->timer0 >= 2 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
		}
		break ;
	case 3 :
		if( ++ ew->timer0 >= 15 ) work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
void set08_00_part_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_part_main ;
		ew->obj = *( obj_stg->obj + no + 50 ) ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->type = no ;
	}
}

/*==============================================================*/
/*			A burst starts the pieces of a wall, each wall's	*/
/*			own set of ten places								*/
/*==============================================================*/
static void set08_00_burst_main( PLAYER_WORK *ew )
{
	int i, j ;

	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		break ;
	case 1 :
		if( stage_work.event == 1 ) ew->routine_0 ++ ;
		break ;
	case 2 :
		if( ++ ew->timer0 > ew->type * 30 ){
			ew->routine_0 ++ ;
			for( i = 0 ; i < 10 ; i ++ ){
				switch( set08_00_burst_tbl[ew->type][i].kind ){
				case 0 :
					for( j = 0 ; j < 4 ; j ++ ) set08_00_piece_a_set( &set08_00_burst_tbl[ew->type][i].pos, j ) ;
					break ;
				case 1 :
					for( j = 0 ; j < 5 ; j ++ ) set08_00_piece_b_set( &set08_00_burst_tbl[ew->type][i].pos, j ) ;
					break ;
				}
			}
		}
		break ;
	case 3 :
		work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
void set08_00_burst_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_burst_main ;
		ew->obj = *( obj_stg->obj + no + 58 ) ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->type = no ;
	}
}

/*==============================================================*/
/*			A chip that drops and is freed on reaching the		*/
/*			ground												*/
/*==============================================================*/
static void set08_00_chip_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->pos.y += ew->spd.y ;
		ew->spd.y -= 1.0f ;
		if( ew->pos.y > 0.0f ) break ;
		ew->routine_0 ++ ;
		break ;
	case 1 :
		work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
void set08_00_chip_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_chip_main ;
		ew->obj = *( obj_stg->obj + ( no * 3 + nlRand() % 3 + 28 ) ) ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = *pos ;
	}
}

/*==============================================================*/
/*			A model whose texture slides, a little more each	*/
/*			frame, round and round								*/
/*==============================================================*/
static void set08_00_scroll_main( PLAYER_WORK *ew )
{
	Float px, py ;

	switch( ew->routine_0 ){
	case 0 :
		if( ++ ew->timer0 >= 100 ) ew->timer0 = 0 ;
		obj_uv_begin( obj_stg->obj[ew->type * 2 + 72 + 1], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &px, &py ) ;
			py += ew->timer0 * 0.01f ;
			obj_uv_put( &px, &py ) ;
		}
		break ;
	case 1 :
		work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
void set08_00_scroll_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_scroll_main ;
		ew->obj = *( obj_stg->obj + no * 2 + 72 ) ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set08_00_scroll_pos_tbl[no] ;
		ew->type = no ;
		obj_uv_clear( ew->obj ) ;
	}
}

static void set08_00_block_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set08_00_block_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_block_main ;
		ew->obj = obj_stg->obj[set08_00_block_obj_tbl[no]] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set08_00_block_pos_tbl[no] ;
		ew->type = no ;
	}
}

/*==============================================================*/
/*			A block that waits for Abyss to hit its place,		*/
/*			then drops to the floor								*/
/*==============================================================*/
static void set08_00_block_main( PLAYER_WORK *ew )
{
	VEC3 pos ;
	int i ;

	switch( ew->routine_0 ){
	case 0 :
		if( stage_work.event == 1 ){
			ew->routine_0 ++ ;
			break ;
		}
		for( i = 0 ; i < 2 ; i ++ ){
			if( pl_work[i].pl_type == PL_ABYSS1 ){
				if( PL18_LW( &pl_work[i] )->hit_flag & ( 1 << ew->type ) ){
					ew->routine_0 = 2 ;
					break ;
				}
			}
		}
		break ;
	case 1 :
		if( ++ ew->timer0 >= 40 ) ew->routine_0 ++ ;
		break ;
	case 2 :
		pos = set08_00_block_pos_tbl[ew->type] ;
		pos.y = set08_00_block_y_tbl[ew->type] ;
		land_ef_set( &pos, 0 ) ;
		work_free( ew ) ;
		break ;
	}
}

/****************************************************************/
/*			Where the line from the screen's eye through a		*/
/*			block's place meets the floor						*/
/****************************************************************/
void set08_00_cross_get( Float *x, Float *y, int no )
{
	VEC3 *blk = &set08_00_block_pos_tbl[no] ;
	VEC3 *eye = scr_work.eye ;

	*x = eye->x - ( eye->x - blk->x ) * eye->z / ( eye->z - blk->z ) ;
	*y = set08_00_block_y_tbl[no] ;
}

/*==============================================================*/
/*			A lamp that fades in while it slides to its			*/
/*			second place, waits, and fades out					*/
/*==============================================================*/
static void set08_00_lamp_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->fade.trnsl += 0.01f ;
		ew->pos.x += ( set08_00_lamp_pos_tbl[ew->type + 2].x - set08_00_lamp_pos_tbl[ew->type].x ) / 100.0f ;
		ew->pos.y += ( set08_00_lamp_pos_tbl[ew->type + 2].y - set08_00_lamp_pos_tbl[ew->type].y ) / 100.0f ;
		ew->pos.z += ( set08_00_lamp_pos_tbl[ew->type + 2].z - set08_00_lamp_pos_tbl[ew->type].z ) / 100.0f ;
		if( ++ ew->timer0 >= 100 ){
			ew->routine_0 ++ ;
			ew->fade.trnsl = 1.0f ;
			ew->pos = set08_00_lamp_pos_tbl[ew->type + 2] ;
		}
		break ;
	case 1 :
		if( stage_work.event == 2 ) ew->routine_0 ++ ;
		break ;
	case 2 :
		ew->fade.trnsl -= 0.01f ;
		if( ++ ew->timer0 >= 100 ) work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void set08_00_lamp_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set08_00_lamp_main ;
		ew->obj = *( obj_stg->obj + no + 86 ) ;
		ew->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_BRIGHT ;
		ew->pos = set08_00_lamp_pos_tbl[no] ;
		ew->type = no ;
		ew->fade.trnsl = 0.0f ;
	}
}

/*==============================================================*/
/*			The wall's own work: it makes the parts and			*/
/*			blocks, then the rocks that fall in the two			*/
/*			shaking spells, and the sounds that go with them	*/
/*==============================================================*/
static void set08_00_main( PLAYER_WORK *ew )
{
	VEC3 pos ;
	Float cx, cy ;
	int i, no ;

	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		set08_00_glow_set() ;
		for( i = 0 ; i < 6 ; i ++ ) set08_00_cover_set( i ) ;
		for( i = 0 ; i < 8 ; i ++ ) set08_00_part_set( i ) ;
		for( i = 0 ; i < 4 ; i ++ ) set08_00_block_set( i ) ;
		for( i = 0 ; i < 6 ; i ++ ) set08_00_burst_set( i ) ;
		break ;
	case 1 :
		if( stage_work.event == 1 ){
			ew->routine_0 ++ ;
			s_set08_set() ;
			for( i = 0 ; i < 3 ; i ++ ) set08_00_scroll_set( i ) ;
			pl18_se_req( 31 ) ;
		}
		break ;
	case 2 :
		if( ew->timer0 % 12 == 0 ){
			no = nlRand() % 2 ;
			pos = set08_00_chip_pos_tbl[no] ;
			pos.x = set08_00_chip_pos_tbl[no].x + ( Float )( nlRand() % 2000 - 1000 ) ;
			pos.z = set08_00_chip_pos_tbl[no].z + ( Float )( nlRand() % 500 - 250 ) ;
			set08_00_chip_set( &pos, no ) ;
		}
		if( ew->timer0 + 3 == 0 ) pl18_se_req( 30 ) ;
		if( ( ew->timer0 + 7 ) % 10 == 0 ){
			if( nlRand() % 2 ) pl_se_req( ew, 48 ) ;
		}
		if( ew->timer0 % 100 == 0 ) pl_se_req( ew, 49 ) ;
		if( ++ ew->timer0 >= 180 ) ew->routine_0 ++ ;
		break ;
	case 3 :
		if( ew->timer0 % 12 == 0 ){
			no = nlRand() % 2 ;
			pos = set08_00_chip_pos_tbl[no] ;
			pos.x = set08_00_chip_pos_tbl[no].x + ( Float )( nlRand() % 2000 - 1000 ) ;
			pos.z = set08_00_chip_pos_tbl[no].z + ( Float )( nlRand() % 500 - 250 ) ;
			set08_00_chip_set( &pos, no ) ;
		}
		if( ew->timer0 + 3 == 0 ) pl18_se_req( 30 ) ;
		if( ( ew->timer0 + 7 ) % 20 == 0 ){
			if( nlRand() % 2 ) pl_se_req( ew, 48 ) ;
		}
		if( ew->timer0 % 200 == 0 ) pl_se_req( ew, 49 ) ;
		if( ++ ew->timer0 >= 360 ) ew->routine_0 ++ ;
		break ;
	case 4 :
		if( stage_work.event == 2 ){
			ew->routine_0 ++ ;
			pl18_se_req( 31 ) ;
			na_req( 68 ) ;
		}
		break ;
	}
	set08_00_cross_get( &cx, &cy, 0 ) ;
	set08_00_cross_get( &cx, &cy, 1 ) ;
	set08_00_cross_get( &cx, &cy, 2 ) ;
	set08_00_cross_get( &cx, &cy, 3 ) ;
}

/****************************************************************/
/*			The stage's table entry								*/
/****************************************************************/
void set08_00_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->work_func = set08_00_main ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set08_00.c" End of File	*/
