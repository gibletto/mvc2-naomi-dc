/* The six marks on each door of the select scene (kind 5 works): they light for the side's personal place. */
#include "player.h"
#include "select.h"

extern void sel_dummy_1( PLAYER_WORK *w );

/* where each side's marks stand: the first of each pair for the four flat ones and the lit one, the second for the sixth */
static VEC3 sel_mark_pos_tbl[2][3][2] = {
	{
		{ { -10.0f, 64.0f, 43.9f }, { 10.0f, 58.0f, 43.9f } },
		{ { -30.0f, 67.0f, 43.9f }, { -10.0f, 61.0f, 43.9f } },
		{ { -40.0f, 72.0f, 43.9f }, { -20.0f, 65.0f, 43.9f } },
	},
	{
		{ { 10.0f, 64.0f, 43.9f }, { -10.0f, 58.0f, 43.9f } },
		{ { 30.0f, 67.0f, 43.9f }, { 10.0f, 61.0f, 43.9f } },
		{ { 40.0f, 72.0f, 43.9f }, { 20.0f, 65.0f, 43.9f } },
	},
} ;

static void sel_mark_main( PLAYER_WORK *ew );
static void sel_mark2_main( PLAYER_WORK *ew );

/****************************************************************/
void sel_mark_make( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint8 i ;

	for( i = 0 ; i < 6 ; i ++ ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
		ew->disp_flag = 0 ;
		ew->work_func = sel_mark_main ;
		ew->master_w = pl_w->master_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->ef_w = pl_w->ef_w ;
		ew->type = pl_w->type ;
		ew->sub_type = pl_w->sub_type ;
		ew->dir = i ;
		ew->pos = sel_mark_pos_tbl[ew->type][ew->sub_type][0] ;
		ew->rot[0] = 0xe001 ;
		ew->rot[1] = 0 ;
		ew->timer0 = ew->dir * 3 + 10 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_TRNSL | OBJ_FADE | OBJ_BRIGHT ;
		ew->fade.trnsl = 1.0f ;
		switch( i ){
		case 4:
			ew->obj = obj_dm01->obj[ew->sub_type + 116] ;
			break ;
		case 5:
			ew->work_func = sel_mark2_main ;
			ew->obj = obj_dm01->obj[119] ;
			ew->pos = sel_mark_pos_tbl[ew->type][ew->sub_type][1] ;
			ew->rot[0] = 0 ;
			if( ew->type ) ew->rot[1] = 0x8000 ;
			ew->scale.x = 0.0f ;
			ew->scale.y = 0.0f ;
			ew->scale.z = 0.0f ;
			ew->obj_flag |= OBJ_SCALE ;
			break ;
		default:
			ew->obj = obj_dm01->obj[i + 112] ;
			break ;
		}
		switch( ew->sub_type ){
		case 0:
			ew->fade.r = 1.0f ;
			ew->fade.g = 0.5f ;
			ew->fade.b = 1.0f ;
			break ;
		case 1:
			ew->fade.r = 0.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 0.0f ;
			break ;
		case 2:
			ew->fade.r = 0.2f ;
			ew->fade.g = 0.82f ;
			ew->fade.b = 1.0f ;
			break ;
		}
	}
}

static void sel_mark_in( PLAYER_WORK *ew );
static void sel_mark_hide( PLAYER_WORK *ew );
static void sel_mark_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_mark_main( PLAYER_WORK *ew )
{
	static void ( *sel_mark_jmp[] )( PLAYER_WORK *ew ) = {
		sel_dummy_1, sel_dummy_1, sel_mark_in, sel_mark_in, sel_mark_hide, sel_mark_free,
	} ;

	sel_mark_jmp[ew->master_w->routine_0]( ew ) ;
}

static Uint8 sel_mark_ck( PLAYER_WORK *ew );
static void sel_mark_reset( PLAYER_WORK *ew );

/*==============================================================*/
/*			The mark shows once the side's personal place is	*/
/*			its own, and goes while it is not					*/
/*==============================================================*/
static void sel_mark_in( PLAYER_WORK *ew )
{
	switch( ew->routine_1 ){
	case 0:
		if( sel_mark_ck( ew ) ){
			ew->routine_1 ++ ;
			ew->timer0 = ew->dir * 3 + 10 ;
		}
		break ;
	case 1:
		if( sel_mark_ck( ew ) ){
			ew->timer0 -- ;
			if( ew->timer0 <= 0 ){
				ew->routine_1 ++ ;
				ew->disp_flag = 1 ;
			}
		}
		else sel_mark_reset( ew ) ;
		break ;
	case 2:
		if( !sel_mark_ck( ew ) ) sel_mark_reset( ew ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void sel_mark_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void sel_mark_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

static void sel_mark2_in( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_mark2_main( PLAYER_WORK *ew )
{
	static void ( *sel_mark2_jmp[] )( PLAYER_WORK *ew ) = {
		sel_dummy_1, sel_dummy_1, sel_mark2_in, sel_mark2_in, sel_mark_hide, sel_mark_free,
	} ;

	sel_mark2_jmp[ew->master_w->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The sixth mark grows in								*/
/*==============================================================*/
static void sel_mark2_in( PLAYER_WORK *ew )
{
	switch( ew->routine_1 ){
	case 0:
		if( sel_mark_ck( ew ) ){
			ew->routine_1 ++ ;
			ew->disp_flag = 1 ;
			ew->scale.x = 0.0f ;
			ew->scale.y = 0.0f ;
			ew->scale.z = 0.0f ;
			ew->timer0 = 10 ;
			ew->spd.x = 0.1f ;
		}
		break ;
	case 1:
		if( sel_mark_ck( ew ) ){
			ew->timer0 -- ;
			if( ew->timer0 <= 0 ) ew->routine_1 ++ ;
			else{
				ew->scale.x += ew->spd.x ;
				ew->scale.y += ew->spd.x ;
				ew->scale.z += ew->spd.x ;
			}
		}
		else sel_mark_reset( ew ) ;
		break ;
	case 2:
		if( !sel_mark_ck( ew ) ) sel_mark_reset( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*			The work of the side's personal place is the one	*/
/*			the mark is for										*/
/*==============================================================*/
static Uint8 sel_mark_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *tw = ew->ef_w ;

	if( ew->sub_type == tw->pl_personal ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static void sel_mark_reset( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->routine_1 = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_mark.c" End of File	*/
