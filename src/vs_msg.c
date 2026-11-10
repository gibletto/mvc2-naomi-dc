/* The big words the fight shows, as works of kind 11: type 0 at its start, 1 at its call to fight
   and 2 at its end. Each waits, flies in along three points, shakes and flies out, with
   fading copies behind it, and says its sound. */
#include "player.h"

extern void vs_res_set( int no ) ;

/* the object of each word */
static Sint32 vs_msg_obj_tbl[] = { 0x2f, 0x30, 0x31 } ;
/* the three points of each word's way */
static VEC3 vs_msg_pos_tbl[] = {
	{ -32.06f, 0.0f, -21.99f }, { -1.33f, 0.0f, -8.75f }, { 10.98f, 0.0f, -3.0f },
	{ 32.33f, 0.0f, -21.99f }, { 1.14f, 0.0f, -7.45f }, { -11.18f, 0.0f, -1.7f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -4.6f }, { 0.0f, 0.0f, 0.0f },
} ;
/* its turn, and the frames it stays shaking */
static Sint32 vs_msg_rot_tbl[] = { 0xee3a, 0x11c7, 0 } ;
static Sint32 vs_msg_wait_tbl[] = { 30, 30, 60 } ;

static void vs_msg_se( int no );
static void vs_msg_main( PLAYER_WORK *ew );

/****************************************************************/
void vs_msg_set( int no )
{
	PLAYER_WORK *ew ;

	vs_msg_se( no ) ;
	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = vs_msg_main ;
		ew->obj = obj_dm00->obj[vs_msg_obj_tbl[no]] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y ;
		ew->pos = vs_msg_pos_tbl[no * 3] ;
		ew->rot[1] = vs_msg_rot_tbl[no] ;
		ew->type = no ;
		if( ew->type == 0 ) ew->work1 = 80 ;
		else ew->work1 = 0 ;
	}
}

/*==============================================================*/
static void vs_msg_se( int no )
{
	switch( no ){
	case 0:
		na_req( ( int )Rnd() % 3 + 6 ) ;
		break ;
	case 1:
		na_req( 9 ) ;
		break ;
	case 2:
		break ;
	}
}

static void vs_msg_ghost_set( PLAYER_WORK *mw );
static void vs_msg_ghost_main( PLAYER_WORK *ew );

/*==============================================================*/
static void vs_msg_main( PLAYER_WORK *ew )
{
	VEC3 *p = &vs_msg_pos_tbl[ew->type * 3 + ew->timer1] ;
	VEC3 *q = p + 1 ;

	switch( ew->routine_0 ){
	case 0:
		if( flash_flag_get() ) return ;
		if( ( ew->work1 = ew->work1 - 1 ) > 0 ) return ;
		ew->disp_flag = 1 ;
		ew->routine_0 ++ ;
		if( ew->type == 2 ){
			if( vs_sys_w->winner_id < 0 ){
				vs_res_set( 1 ) ;
				work_free( ew ) ;
				return ;
			}
			else na_req( 40 ) ;
		}
	case 1:
		{
			int no = ew->type ;
			if( no == 0 || no == 1 ){
				vs_msg_ghost_set( ew ) ;
			}
		}
		ew->pos.x = p->x + ( q->x - p->x ) / 15.0f * ew->timer0 ;
		ew->pos.z = p->z + ( q->z - p->z ) / 15.0f * ew->timer0 ;
		if( ++ ew->timer0 >= 15 ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			ew->timer1 ++ ;
			ew->pos = *q ;
		}
		break ;
	case 2:
		if( ew->type < 2 && ew->timer0 < 15 ){
			ew->pos.x = p->x + 0.2f * ( ew->timer0 % 2 ) ;
		}
		if( ++ ew->timer0 >= vs_msg_wait_tbl[ew->type] ){
			ew->routine_0 ++ ;
			ew->timer0 = 0 ;
			if( ew->type == 1 ){
				flash_white_set() ;
			}
		}
		break ;
	case 3:
		{
			int no = ew->type ;
			if( no == 0 || no == 1 ){
				vs_msg_ghost_set( ew ) ;
			}
		}
		ew->pos.x = p->x + ( q->x - p->x ) / 15.0f * ew->timer0 ;
		ew->pos.z = p->z + ( q->z - p->z ) / 15.0f * ew->timer0 ;
		if( ++ ew->timer0 >= 15 ){
			work_free( ew ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*			A copy that stays where the word was and fades		*/
/*==============================================================*/
static void vs_msg_ghost_set( PLAYER_WORK *mw )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = vs_msg_ghost_main ;
		ew->obj = mw->obj ;
		ew->obj_flag = mw->obj_flag | OBJ_TRNSL ;
		ew->pos = mw->pos ;
		ew->rot[1] = mw->rot[1] ;
		ew->fade.trnsl = 0.5f ;
	}
}

/*==============================================================*/
static void vs_msg_ghost_main( PLAYER_WORK *ew )
{
	ew->fade.trnsl -= 0.1f ;
	if( ew->fade.trnsl > 0.0f ) return ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "vs_msg.c" End of File	*/
