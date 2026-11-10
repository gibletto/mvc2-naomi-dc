/* The select scene's portraits (kind 5 works): the portrait of each of the 56 characters in the
   grid, which fly in from the screen's edges, stay while the characters can be chosen, and fly
   out. The cards, the team tag and the cursor follow, each a file of its own (ef05_card.c,
   ef05_tag.c, ef05_cur.c). */
#include "player.h"
#include "select.h"

static Sint8 unk_0C2FB498;
static Sint8 unk_0C2FB499;
static Uint8 unk_0C2FB49A[6];
extern void move_set_xy( PLAYER_WORK *wk, Sint32 time, Float x, Float y, Sint8 x_type, Sint8 y_type );
extern void move_set_z( PLAYER_WORK *wk, Sint32 time, Float z, Sint8 z_type );
extern void move_z_add( PLAYER_WORK *wk );
extern void sel_obj_pos_add( PLAYER_WORK *wk, Sint16 no, Float x, Float y );
extern PLAYER_WORK *sel_port_set( PLAYER_WORK *ew );
extern void sel_port_fly_set( PLAYER_WORK *ew );

/* where each portrait stands, and the offset of its picture in the texture */
VEC3 ef05_pos_tbl[59] = {
	{ -56.0f, 52.0f, 183.0f },
	{ -57.0f, 17.0f, 189.0f },
	{ -52.0f, 84.0f, 172.0f },
	{ -56.0f, -52.0f, 183.0f },
	{ -57.0f, -17.0f, 189.0f },
	{ -52.0f, -84.0f, 172.0f },
	{ 18.0f, 84.0f, 179.0f },
	{ 56.0f, 52.0f, 183.0f },
	{ 16.0f, 114.0f, 162.0f },
	{ 57.0f, 17.0f, 189.0f },
	{ 47.0f, 114.0f, 156.0f },
	{ 19.0f, -17.0f, 197.0f },
	{ 57.0f, -17.0f, 189.0f },
	{ 52.0f, -84.0f, 172.0f },
	{ 56.0f, -52.0f, 183.0f },
	{ 19.0f, -52.0f, 191.0f },
	{ -9e+01f, 52.0f, 169.0f },
	{ -85.0f, 84.0f, 159.0f },
	{ -19.0f, -17.0f, 197.0f },
	{ -19.0f, 17.0f, 197.0f },
	{ -19.0f, 52.0f, 191.0f },
	{ -18.0f, 84.0f, 179.0f },
	{ 19.0f, 52.0f, 191.0f },
	{ 19.0f, 17.0f, 197.0f },
	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f },
	{ -1.0f, -1.0f, -1.0f },
	{ -16.0f, -114.0f, 162.0f },
	{ -47.0f, 114.0f, 156.0f },
	{ -47.0f, -114.0f, 156.0f },
	{ -93.0f, 17.0f, 174.0f },
	{ -19.0f, -52.0f, 191.0f },
	{ -77.0f, 114.0f, 143.0f },
	{ -125.0f, 17.0f, 153.0f },
	{ -9e+01f, -52.0f, 169.0f },
	{ -122.0f, -52.0f, 148.0f },
	{ -85.0f, -84.0f, 159.0f },
	{ -125.0f, -17.0f, 153.0f },
	{ -16.0f, 114.0f, 162.0f },
	{ -122.0f, 52.0f, 148.0f },
	{ 93.0f, 17.0f, 174.0f },
	{ 93.0f, -17.0f, 174.0f },
	{ 47.0f, -114.0f, 156.0f },
	{ 122.0f, -52.0f, 148.0f },
	{ 52.0f, 84.0f, 172.0f },
	{ 18.0f, -84.0f, 179.0f },
	{ 85.0f, 84.0f, 159.0f },
	{ 9e+01f, 52.0f, 169.0f },
	{ 16.0f, -114.0f, 162.0f },
	{ 125.0f, -17.0f, 153.0f },
	{ 122.0f, 52.0f, 148.0f },
	{ 85.0f, -84.0f, 159.0f },
	{ 77.0f, -114.0f, 143.0f },
	{ 125.0f, 17.0f, 153.0f },
	{ 77.0f, 114.0f, 143.0f },
	{ -93.0f, -17.0f, 174.0f },
	{ -18.0f, -84.0f, 179.0f },
	{ 9e+01f, -52.0f, 169.0f },
	{ -77.0f, -114.0f, 143.0f },
} ;

static Float ef05_uv_tbl[59][2] = {
	{ 0.0f, 1.0f },
	{ 0.125f, 1.0f },
	{ 0.25f, 1.0f },
	{ 0.375f, 1.0f },
	{ 0.0f, 0.875f },
	{ 0.125f, 0.875f },
	{ 0.25f, 0.875f },
	{ 0.375f, 0.875f },
	{ 0.0f, 0.75f },
	{ 0.125f, 0.75f },
	{ 0.25f, 0.75f },
	{ 0.375f, 0.75f },
	{ 0.0f, DOT_X( 0.375f ) },
	{ 0.125f, DOT_X( 0.375f ) },
	{ 0.25f, DOT_X( 0.375f ) },
	{ 0.375f, DOT_X( 0.375f ) },
	{ 0.0f, 0.5f },
	{ 0.125f, 0.5f },
	{ 0.25f, 0.5f },
	{ 0.375f, 0.5f },
	{ 0.0f, 0.375f },
	{ 0.125f, 0.375f },
	{ 0.25f, 0.375f },
	{ 0.375f, 0.375f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.0f },
	{ 0.0f, 0.25f },
	{ 0.125f, 0.25f },
	{ 0.25f, 0.25f },
	{ 0.375f, 0.25f },
	{ 0.0f, 0.125f },
	{ 0.125f, 0.125f },
	{ 0.25f, 0.125f },
	{ 0.375f, 0.125f },
	{ 0.5f, 1.0f },
	{ DOT_X( 0.375f ), 1.0f },
	{ 0.75f, 1.0f },
	{ 0.875f, 1.0f },
	{ 0.5f, 0.875f },
	{ DOT_X( 0.375f ), 0.875f },
	{ 0.75f, 0.875f },
	{ 0.875f, 0.875f },
	{ 0.5f, 0.75f },
	{ DOT_X( 0.375f ), 0.75f },
	{ 0.75f, 0.75f },
	{ 0.875f, 0.75f },
	{ 0.5f, DOT_X( 0.375f ) },
	{ DOT_X( 0.375f ), DOT_X( 0.375f ) },
	{ 0.75f, DOT_X( 0.375f ) },
	{ 0.875f, DOT_X( 0.375f ) },
	{ 0.5f, 0.5f },
	{ DOT_X( 0.375f ), 0.5f },
	{ 0.75f, 0.5f },
	{ 0.875f, 0.5f },
	{ 0.5f, 0.375f },
	{ DOT_X( 0.375f ), 0.375f },
	{ 0.75f, 0.375f },
	{ 0.875f, 0.375f },
} ;

/* by character: how its portrait is turned and how long it takes to arrive; the second byte is unused here */
const EF05_CELL ef05_cell_tbl[60] = {
	2, 2, 2, 3, 2, 1, 2, 5, 2, 4, 2, 6,
	4, 1, 5, 2, 4, 0, 5, 3, 5, 0, 4, 4,
	5, 4, 5, 6, 5, 5, 4, 5, 1, 2, 1, 1,
	3, 4, 3, 3, 3, 2, 3, 1, 4, 2, 4, 3,
	0, 0, 0, 0, 0, 0, 3, 7, 2, 0, 2, 7,
	1, 3, 3, 5, 1, 0, 0, 3, 1, 5, 0, 5,
	1, 6, 0, 4, 3, 0, 0, 2, 6, 3, 6, 4,
	5, 7, 7, 5, 5, 1, 4, 6, 6, 1, 6, 2,
	4, 7, 7, 4, 7, 2, 6, 6, 6, 7, 7, 3,
	6, 0, 1, 4, 3, 6, 6, 5, 1, 7, 0, 0,
} ;

static void ef05_char_set( Sint32 no );

/****************************************************************/
void ef05_set_all( void )
{
	Sint32 i ;

	unk_0C2FB499 = 0 ;
	unk_0C2FB498 = 0 ;
	for( i = 0 ; i < 59 ; i ++ ) ef05_char_set( i ) ;
}

static void ef05_main( PLAYER_WORK *ew );
static void ef05_timer_set( PLAYER_WORK *ew );
static void ef05_show( PLAYER_WORK *ew );
static void ef05_pic_add( PLAYER_WORK *ew );
static void ef05_pic_sub( PLAYER_WORK *ew );

/*==============================================================*/
/*			A portrait's work for character no (the three		*/
/*			numbers 24 to 26 have none, later ones are three	*/
/*			less)												*/
/*==============================================================*/
static void ef05_char_set( Sint32 no )
{
	PLAYER_WORK *ew ;
	Sint32 rnd ;

	if( no >= 24 && no <= 26 ) return ;
	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	if( no >= 27 ) ew->type = no - 3 ;
	else ew->type = no ;
	ew->disp_flag = 1 ;
	ew->work_func = ef05_main ;
	ew->base_mtx = &sel_back_w->obj_mtx ;
	ew->obj = obj_dm01->obj[ew->type + 10] ;
	ew->sub_type = no ;
	ew->routine_3 = ew->sub_type ;
	switch( ew->sub_type ){
	case 32:
	case 58:
	case 54:
	case 52:
		ew->ef_no = 1 ;
		break ;
	default:
		ew->ef_no = 0 ;
		break ;
	}
	if( ef05_pos_tbl[ew->routine_3].x < 0.0f ) ew->pos.x = ef05_pos_tbl[ew->routine_3].x + -900.0f ;
	else ew->pos.x = ef05_pos_tbl[ew->routine_3].x + 900.0f ;
	if( ef05_pos_tbl[ew->routine_3].y >= 173.0f ) ew->pos.y = ef05_pos_tbl[ew->routine_3].y + 100.0f ;
	else ew->pos.y = ef05_pos_tbl[ew->routine_3].y + -100.0f ;
	ew->pos.z = ef05_pos_tbl[ew->routine_3].z + 800.0f ;
	ef05_timer_set( ew ) ;
	rnd = nlRand() ;
	ew->timer0 += rnd & 3 ;
	ew->rot[0] = 0 ;
	ef05_rot_set( ew, ew->sub_type ) ;
	ew->rot[2] = 0 ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE | OBJ_BRIGHT ;
	ew->fade.r = 1.0f ;
	ew->fade.g = 1.0f ;
	ew->fade.b = 1.0f ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	ew->scale.z = 1.0f ;
	ew->work3 = 0 ;
	ew->dir = 0 ;
	if( ew->ef_no ) ef05_show( ew ) ;
	else ef05_pic_add( ew ) ;
	sel_port_set( ew ) ;
}

/****************************************************************/
/*			It is turned a way that goes with its				*/
/*			character's place in the grid						*/
/****************************************************************/
void ef05_rot_set( PLAYER_WORK *ew, Uint8 no )
{
	Uint8 k = ef05_cell_tbl[no].col ;

	switch( k ){
	case 0:
		ew->rot[1] = 0xe402 ;
		break ;
	case 1:
		ew->rot[1] = 0xec02 ;
		break ;
	case 2:
		ew->rot[1] = 0xf402 ;
		break ;
	case 3:
		ew->rot[1] = 0xfc02 ;
		break ;
	case 4:
		ew->rot[1] = 0x3ff ;
		break ;
	case 5:
		ew->rot[1] = 0xbff ;
		break ;
	case 6:
		ew->rot[1] = 0x13ff ;
		break ;
	case 7:
		ew->rot[1] = 0x1bff ;
		break ;
	}
}

static void ef05_come( PLAYER_WORK *ew );
static void ef05_stay( PLAYER_WORK *ew );
static void ef05_go( PLAYER_WORK *ew );
static void ef05_gone( PLAYER_WORK *ew );
static void ef05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void ef05_main( PLAYER_WORK *ew )
{
	static void ( *ef05_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_come, ef05_stay, ef05_go, ef05_gone, ef05_free,
	} ;

	ef05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It sets off for its place, over the frames its		*/
/*			timer holds											*/
/*==============================================================*/
static void ef05_come( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	move_set_xy( ew, ew->timer0, ef05_pos_tbl[ew->routine_3].x, ef05_pos_tbl[ew->routine_3].y, 2, 2 ) ;
	ew->spd.z = ( ef05_pos_tbl[ew->routine_3].z - ew->pos.z ) / ew->timer0 ;
	ew->acc.z = 0.0f ;
}

/*--------------------------------------------------------------*/
static void ef05_stay( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 < 0 ){
		ew->routine_0 ++ ;
		ew->pos = ef05_pos_tbl[ew->routine_3] ;
		sel_w.ready_flag = 1 ;
	}
	else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		ew->pos.z += ew->spd.z ;
		ew->spd.z += ew->acc.z ;
	}
}

/*==============================================================*/
/*			It flies out to the screen's edge, to the side		*/
/*			its place is nearest to								*/
/*==============================================================*/
static void ef05_go( PLAYER_WORK *ew )
{
	VEC3 vec ;
	Sint16 i ;
	Uint8 no ;
	Uint8 pick ;
	Sint32 side ;

	ew->fade.r = 0.7f ;
	ew->fade.g = 0.7f ;
	ew->fade.b = 0.7f ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	ew->scale.z = 1.0f ;
	ew->pos.z = ef05_pos_tbl[ew->sub_type].z ;
	ew->pos.x = ef05_pos_tbl[ew->sub_type].x ;
	if( ew->ef_no ) ef05_show( ew ) ;
	else ef05_pic_sub( ew ) ;
	if( Sys->exit_flag ){
		ew->routine_0 ++ ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
		ef05_timer_set( ew ) ;
		side = pick & 1 ;
		if( ew->pos.x >= 0.0f ){
			if( ew->pos.x > 50.0f ){
				if( side ) vec.x = ew->pos.x + 120.0f ;
				else vec.x = ew->pos.x + 160.0f ;
				vec.z = ew->pos.z + 800.0f ;
			}
			else{
				if( side ) vec.x = ew->pos.x + 30.0f ;
				else vec.x = ew->pos.x + 50.0f ;
				vec.z = ew->pos.z + 1000.0f ;
			}
		}
		else{
			if( ew->pos.x < -50.0f ){
				if( side ) vec.x = ew->pos.x + -120.0f ;
				else vec.x = ew->pos.x + -160.0f ;
				vec.z = ew->pos.z + 800.0f ;
			}
			else{
				if( side ) vec.x = ew->pos.x + -30.0f ;
				else vec.x = ew->pos.x + -50.0f ;
				vec.z = ew->pos.z + 1000.0f ;
			}
		}
		if( ew->pos.y > 150.0f ) vec.y = ew->pos.y + 40.0f ;
		else vec.y = ew->pos.y + -40.0f ;
		move_set_xy( ew, ew->timer0, vec.x, vec.y, 1, 1 ) ;
		move_set_z( ew, ew->timer0, vec.z, 1 ) ;
		sel_port_fly_set( ew ) ;
	}
	else{
		for( i = 0 ; i < 2 ; i ++ ){
			if( sel_w.cur_char[i] ){
				no = sel_w.cur_char[i] - 1 ;
				if( ew->sub_type == no ){
					ew->fade.r = 1.0f ;
					ew->fade.g = 1.0f ;
					ew->fade.b = 1.0f ;
					if( i ){
						ew->scale.x = 1.15f ;
						ew->scale.y = 1.15f ;
						ew->scale.z = 1.15f ;
						ew->pos.z = ef05_pos_tbl[no].z + 6.0f ;
						if( ew->pos.x < 0.0f ) ew->pos.x = ef05_pos_tbl[no].x + -2.0f ;
						else ew->pos.x = ef05_pos_tbl[no].x + 2.0f ;
					}
					else{
						ew->scale.x = 1.2f ;
						ew->scale.y = 1.2f ;
						ew->scale.z = 1.2f ;
						ew->pos.z = ef05_pos_tbl[no].z + 8.0f ;
						if( ew->pos.x < 0.0f ) ew->pos.x = ef05_pos_tbl[no].x + -2.0f ;
						else ew->pos.x = ef05_pos_tbl[no].x + 2.0f ;
					}
				}
			}
		}
	}
}

/*==============================================================*/
/*			It waits out the flight, then is hidden				*/
/*==============================================================*/
static void ef05_gone( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		move_z_add( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void ef05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The frames it takes to come and to go, by its		*/
/*			place in the grid									*/
/*==============================================================*/
static void ef05_timer_set( PLAYER_WORK *ew )
{
	Uint8 k = ef05_cell_tbl[ew->sub_type].col ;

	switch( k ){
	case 0:
	case 7:
		ew->timer0 = 40 ;
		break ;
	case 1:
	case 6:
		ew->timer0 = 38 ;
		break ;
	case 2:
	case 5:
		ew->timer0 = 36 ;
		break ;
	default:
		ew->timer0 = 34 ;
		break ;
	}
}

/*==============================================================*/
/*			The characters that can be chosen: bit and mask		*/
/*			for this one (the second word holds 32 and up)		*/
/*==============================================================*/
static void ef05_bit_get( PLAYER_WORK *ew, Sint8 *bit, Uint32 *mask )
{
	if( ew->sub_type >= 32 ){
		*mask = sel_w.char_open[0][1] | sel_w.char_open[1][1] ;
		*bit = ew->sub_type - 32 ;
	}
	else{
		*mask = sel_w.char_open[0][0] | sel_w.char_open[1][0] ;
		*bit = ew->sub_type ;
	}
}

/*==============================================================*/
/*			Shown while the character is free to be chosen		*/
/*==============================================================*/
static void ef05_show( PLAYER_WORK *ew )
{
	Sint8 bit ;
	Uint32 mask ;

	ef05_bit_get( ew, &bit, &mask ) ;
	if( !( mask & ( 1 << bit ) ) ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
}

/*==============================================================*/
/*			The picture slides along its texture				*/
/*==============================================================*/
static void ef05_pic_add( PLAYER_WORK *ew )
{
	Sint8 bit ;
	Uint32 mask ;

	ef05_bit_get( ew, &bit, &mask ) ;
	if( !( mask & ( 1 << bit ) ) ){
		sel_obj_pos_add( ew, ew->type + 10, 0.5f - ef05_uv_tbl[ew->sub_type][0], ef05_uv_tbl[ew->sub_type][1] + -0.25f ) ;
		ew->dir = 1 ;
	}
}

/*==============================================================*/
/*			And back											*/
/*==============================================================*/
static void ef05_pic_sub( PLAYER_WORK *ew )
{
	Sint8 bit ;
	Uint32 mask ;

	ef05_bit_get( ew, &bit, &mask ) ;
	if( mask & ( 1 << bit ) ){
		if( ew->dir ){
			sel_obj_pos_add( ew, ew->type + 10, ef05_uv_tbl[ew->sub_type][0] + -0.5f, 0.25f - ef05_uv_tbl[ew->sub_type][1] ) ;
			ew->dir = 0 ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ef05.c" End of File	*/
