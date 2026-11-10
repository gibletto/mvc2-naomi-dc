/* The select scene's team tag (a kind 5 work) that grows and shrinks. */
#include "player.h"
#include "select.h"

static void ef05_tag_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The tag of the team member being chosen: it			*/
/*			stands at its side's edge until the side picks		*/
/****************************************************************/
void ef05_tag_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->master_w = pl_w ;
	ew->type = pl_w->pl_index ;
	ew->sub_type = pl_w->timer1 ;
	ew->work_func = ef05_tag_main ;
	ew->obj = obj_dm01->obj[ew->type + 4] ;
	if( pl_w->pl_index ){
		ew->pos.x = 146.0f ;
		ew->rot[2] = 0x11c7 ;
	}
	else{
		ew->pos.x = -146.0f ;
		ew->rot[2] = 0xee3a ;
	}
	ew->pos.y = 36.0f ;
	ew->pos.z = 260.0f ;
	ew->rot[0] = 0xd556 ;
	ew->rot[1] = 0 ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	ew->scale.z = 1.0f ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Z | OBJ_SCALE | OBJ_BRIGHT ;
}

static void ef05_tag_wait( PLAYER_WORK *ew );
static void ef05_tag_grow( PLAYER_WORK *ew );
static void ef05_tag_shrink( PLAYER_WORK *ew );
static void ef05_tag_end( PLAYER_WORK *ew );
static void ef05_tag_free( PLAYER_WORK *ew );

/*==============================================================*/
static void ef05_tag_main( PLAYER_WORK *ew )
{
	static void ( *ef05_tag_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_tag_wait, ef05_tag_grow, ef05_tag_shrink, ef05_tag_end, ef05_tag_free,
	} ;

	ef05_tag_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void ef05_tag_wait( PLAYER_WORK *ew )
{
	if( sel_w.pick_flag[ew->type] & ( 1 << ew->sub_type ) ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void ef05_tag_grow( PLAYER_WORK *ew )
{
	ew->scale.x += 0.1f ;
	ew->scale.y += 0.1f ;
	if( ew->scale.x >= 1.3f ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void ef05_tag_shrink( PLAYER_WORK *ew )
{
	ew->scale.x -= 0.1f ;
	ew->scale.y -= 0.1f ;
	if( ew->scale.x <= 1.0f ){
		ew->scale.y = 1.0f ;
		ew->scale.x = 1.0f ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void ef05_tag_end( PLAYER_WORK *ew )
{
	if( sel_w.pick_flag[ew->type] & ( 16 << ew->sub_type ) ){
		if( sel_w.work_step[ew->type] == 3 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void ef05_tag_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ef05_tag.c" End of File	*/
