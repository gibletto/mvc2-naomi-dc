/* The pictures of the players that slide in beside a side's name when a fight starts: the picture
   of the point player (type 0) or of each partner in a team (type 1), works of kind 11. */
#include "player.h"

extern void pl_pic_send( PLAYER_WORK *pl_w ) ;
extern void pl_pic_send2( PLAYER_WORK *pl_w ) ;

/* where a partner's picture starts and where it comes to rest, before it is turned for the side */
typedef struct PL_PIC_POS {
	VEC3	start, stop;
} PL_PIC_POS;

static PL_PIC_POS pl_pic_pos_tbl[] = {
	{ { 50.0f, 0.0f, -30.0f }, { -16.0f, 0.0f, -60.0f } },
	{ { 10.0f, -40.0f, -30.0f }, { -42.0f, 15.0f, -60.0f } },
	{ { 10.0f, 40.0f, -30.0f }, { -42.0f, -15.0f, -60.0f } },
} ;

static void pl_pic_main( PLAYER_WORK *ew );

/****************************************************************/
void pl_pic_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew, *tw ;
	int cnt, i, j ;

	if( Sys->play_mode == 64 ) return ;
	if( ABYSS_CK( pl_w ) ) return ;
	cnt = 1 ;
	if( no ){
		if( ( cnt = pl_w->v_combi_cnt ) == 1 ) no = 0 ;
		else cnt = 3 ;
	}
	if( !no ){
		pl_pic_send( pl_w ) ;
	} else {
		pl_pic_send2( vs_sys_w->team_w[pl_w->pl_no][0] ) ;
		pl_pic_send2( vs_sys_w->team_w[pl_w->pl_no][1] ) ;
		pl_pic_send2( vs_sys_w->team_w[pl_w->pl_no][2] ) ;
	}
	for( i = 0, j = 0 ; i < cnt ; i ++ ){
		tw = vs_sys_w->team_w[pl_w->pl_no][i] ;
		if( ( cnt == 1 && i == 0 ) || ( pl_w->hyper_team_bit & ( 1 << tw->work_no / 2 ) ) ){
			if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
				ew->work_func = pl_pic_main ;
				ew->master_w = pl_w ;
				ew->pl_no = pl_w->pl_no ;
				ew->pl_type = tw->pl_type ;
				ew->dir = tw->team_no ;
				ew->work_no = tw->work_no ;
				ew->ef_w = tw ;
				ew->disp_dir = pl_w->disp_dir ;
				ew->type = no ;
				ew->sub_type = j ;
				ew->ef_no = 6 ;
				j ++ ;
			}
		}
	}
}

static void pl_pic_init( PLAYER_WORK *ew );
static void pl_pic_move( PLAYER_WORK *ew );
static void pl_pic_end( PLAYER_WORK *ew );
static void pl_pic_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pl_pic_main( PLAYER_WORK *ew )
{
	static void ( *pl_pic_jmp[] )( PLAYER_WORK *ew ) = {
		pl_pic_init, pl_pic_move, pl_pic_end, pl_pic_free,
	} ;

	pl_pic_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pl_pic_init( PLAYER_WORK *ew )
{
	Uint8 m ;

	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE ;
	if( !ew->type ){
		ew->obj = obj_dm00->obj[108] ;
		ew->pos.y = -70.0f ;
		ew->pos.z = -200.0f ;
		ew->scale.x = 2.0f ;
		ew->scale.y = 2.0f ;
		ew->scale.z = 1.0f ;
		ew->pos.x = ( 1 - ew->disp_dir * 2 ) * 47.0f ;
		ew->acc.x = ( 1 - ew->disp_dir * 2 ) * 150.0f ;
		ew->rot[1] = ( 1 - ew->disp_dir ) * 0x8000 ;
		ew->timer0 = 9 ;
		ew->spd.x = ( ew->acc.x - ew->pos.x ) / 15.0f ;
		ew->spd.y = 4.6666665f ;
	} else {
		ew->scale.x = 2.0f ;
		ew->scale.y = 2.0f ;
		ew->scale.z = 1.0f ;
		ew->obj = obj_dm00->obj[109 + ew->work_no / 2] ;
		m = 0 ;
		if( ew->master_w->v_combi_cnt != 3 ) m = 1 ;
		ew->pos = pl_pic_pos_tbl[ew->sub_type + m].start ;
		ew->acc = pl_pic_pos_tbl[ew->sub_type + m].stop ;
		if( ew->disp_dir == 0 ){
			ew->pos.x = -ew->pos.x ;
			ew->acc.x = -ew->acc.x ;
		}
		ew->timer0 = 10 ;
		ew->rot[1] = ( 1 - ew->disp_dir ) * 0x8000 ;
		ew->spd.x = ( ew->acc.x - ew->pos.x ) / 10.0f ;
		ew->spd.y = ( ew->acc.y - ew->pos.y ) / 10.0f ;
		ew->spd.z = ( ew->acc.z - ew->pos.z ) / 10.0f ;
	}
	pl_pic_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pl_pic_move( PLAYER_WORK *ew )
{
	if( ew->type == 0 ){
		if( ew->routine_1 == 0 ){
			ew->pos.x += ew->spd.x ;
			ew->pos.y += ew->spd.y ;
			ew->scale.x -= 0.06666667f ;
			ew->scale.y -= 0.06666667f ;
			if( -- ew->timer0 <= 0 ){
				ew->routine_1 ++ ;
				ew->spd.x /= 20.0f ;
				ew->timer0 = 4 ;
			}
		} else {
			if( -- ew->timer0 <= 0 ){
				ew->timer0 = 4 ;
				ew->pos.x += ew->spd.x ;
			}
	if( vs_sys_w->game_stop == 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
		}
	} else {
		if( ew->routine_1 == 0 ){
		ew->pos.x += ew->spd.x ;
		ew->pos.y += ew->spd.y ;
		ew->pos.z += ew->spd.z ;
		ew->scale.x -= 0.1f ;
		ew->scale.y -= 0.1f ;
		if( -- ew->timer0 <= 0 ){
			ew->routine_1 ++ ;
			ew->scale.x = 1.0f ;
			ew->scale.y = 1.0f ;
			ew->pos = ew->acc ;
		}
		}
	if( vs_sys_w->game_stop == 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	}
}

/*--------------------------------------------------------------*/
static void pl_pic_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pl_pic_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_pic_ef.c" End of File	*/
