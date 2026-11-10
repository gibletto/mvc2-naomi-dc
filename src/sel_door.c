/* The select scene's door works (kind 5): the two doors that turn in over the screen when a side has
   chosen, and out again. */
#include "player.h"
#include "select.h"

extern void sel_door_follow_set( PLAYER_WORK *pl_w, Uint8 flag );
extern void sel_light_make( PLAYER_WORK *ew );
extern void sel_glow_make( PLAYER_WORK *ew );

/* where each side's door stands and how it starts turned */
static VEC3 sel_door_pos_tbl[2] = {
	{ -115.0f, 392.0f, 310.0f },
	{ 115.0f, 392.0f, 310.0f },
} ;

static Sint32 sel_door_rot_tbl[2][3] = {
	{ 0, 0, 0xe001 },
	{ 0, 0, 0x2000 },
} ;

static void sel_door_main( PLAYER_WORK *ew );

/****************************************************************/
void sel_door_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_door_main ;
	ew->master_w = pl_w ;
	ew->ef_w = &pl_work[pl_w->timer1 * 2 + pl_w->pl_index] ;
	ew->type = pl_w->pl_index ;
	ew->sub_type = pl_w->timer1 ;
	if( ew->type ) ew->obj = obj_dm01->obj[105] ;
	else ew->obj = obj_dm01->obj[98] ;
	ew->pos = sel_door_pos_tbl[ew->type] ;
	ew->rot[0] = sel_door_rot_tbl[ew->type][0] ;
	ew->rot[1] = sel_door_rot_tbl[ew->type][1] ;
	ew->rot[2] = sel_door_rot_tbl[ew->type][2] ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
	sel_light_make( ew ) ;
	sel_door_follow_set( ew, 0 ) ;
	na_req( 76 + ew->type ) ;
}

static void sel_door_speed_main( PLAYER_WORK *ew );

/****************************************************************/
void sel_door_speed_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = sel_door_speed_main ;
	ew->master_w = pl_w ;
	ew->ef_w = &pl_work[pl_w->timer1 * 2 + pl_w->pl_index] ;
	ew->type = pl_w->pl_index ;
	ew->sub_type = pl_w->timer1 ;
	if( ew->type ) ew->obj = obj_dm01->obj[105] ;
	else ew->obj = obj_dm01->obj[98] ;
	ew->pos = sel_door_pos_tbl[ew->type] ;
	ew->rot[0] = sel_door_rot_tbl[ew->type][0] ;
	ew->rot[1] = sel_door_rot_tbl[ew->type][1] ;
	ew->rot[2] = sel_door_rot_tbl[ew->type][2] ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
	sel_door_follow_set( ew, 1 ) ;
	sel_glow_make( ew ) ;
	na_req( 76 + ew->type ) ;
}

static void sel_door_turn( PLAYER_WORK *ew );
static void sel_door_ready( PLAYER_WORK *ew );
static void sel_door_wait( PLAYER_WORK *ew );
static void sel_door_delay( PLAYER_WORK *ew );
static void sel_door_close( PLAYER_WORK *ew );
static void sel_door_hide( PLAYER_WORK *ew );
static void sel_door_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_door_main( PLAYER_WORK *ew )
{
	static void ( *ef05_door_jmp[] )( PLAYER_WORK *ew ) = {
		sel_door_turn, sel_door_ready, sel_door_wait, sel_door_delay,
		sel_door_close, sel_door_hide, sel_door_free,
	} ;

	ef05_door_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void sel_door_turn( PLAYER_WORK *ew )
{
	if( ew->type ){
		ew->rot[2] -= 0x200 ;
		if( ew->rot[2] <= 0 ){
			ew->routine_0 ++ ;
			ew->rot[2] = 0 ;
		}
	}
	else{
		ew->rot[2] += 0x200 ;
		if( ew->rot[2] >= 0x10000 ){
			ew->routine_0 ++ ;
			ew->rot[2] = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sel_door_ready( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	sel_w.work_step[ew->type] = 2 ;
}

/*--------------------------------------------------------------*/
static void sel_door_wait( PLAYER_WORK *ew )
{
	if( sel_w.pick_flag[ew->type] & ( 16 << ew->sub_type ) ){
		ew->routine_0 ++ ;
		ew->timer0 = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_door_delay( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_door_close( PLAYER_WORK *ew )
{
	if( ew->type ){
		ew->rot[2] += 0x200 ;
		if( ew->rot[2] >= 0x2000 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
	else{
		ew->rot[2] -= 0x200 ;
		if( ew->rot[2] <= -0x2000 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void sel_door_hide( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void sel_door_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

static void sel_door_speed_ready( PLAYER_WORK *ew );
static void sel_door_speed_wait( PLAYER_WORK *ew );
static void sel_door_speed_end( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_door_speed_main( PLAYER_WORK *ew )
{
	static void ( *ef05_door2_jmp[] )( PLAYER_WORK *ew ) = {
		sel_door_turn, sel_door_speed_ready, sel_door_speed_wait, sel_door_delay, sel_door_close, sel_door_speed_end,
	} ;

	ef05_door2_jmp[ew->routine_0]( ew ) ;
}

/*--------------------------------------------------------------*/
static void sel_door_speed_ready( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	sel_w.work_step[ew->type] = 4 ;
}

/*--------------------------------------------------------------*/
static void sel_door_speed_wait( PLAYER_WORK *ew )
{
	if( sel_w.speed_flag ){
		ew->routine_0 ++ ;
		ew->timer0 = 10 ;
	}
}

/*--------------------------------------------------------------*/
static void sel_door_speed_end( PLAYER_WORK *ew )
{
	sel_w.work_step[ew->type] = 5 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_door.c" End of File	*/
