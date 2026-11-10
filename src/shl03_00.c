/* Morrigan's Soul Fist (character 03's shot 00): thrown straight from the ground, or down and
   forward from the air until it reaches her ground; it flies until it hits or leaves the screen. */
#include "player.h"
#include "pl_char.h"
#include "pl03_def.h"

static void shl03_00_main( PLAYER_WORK *ew );

/****************************************************************/
/*			It returns nothing: its callers test r0, which		*/
/*			is left 0 only when no work was free				*/
/****************************************************************/
int shl03_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl03_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x300 ;
	}
}

static void shl03_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl03_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl03_00_main( PLAYER_WORK *ew )
{
	static void ( *shl03_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_00_init, shl03_00_move, shl03_00_end, shl03_00_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	shl03_00_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
/*			From the ground: straight out at her chest			*/
/*==============================================================*/
static void shl03_00_soul_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint32 shl03_00_spd[] = { 0x50000, 0x60000 } ;	/* by strength, in 16.16 */

	BODY_SIZE_SET( ew, 0x20202424 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( pl_w->team_type == 3 ){	/* as a variable counter: the same attack */
		ew->atck_no = pl03_at_soul ;
		HIT_RESET( ew ) ;
	}
	else {
		ew->atck_no = pl03_at_soul ;
		HIT_RESET( ew ) ;
	}
	ew->pos.x = DOT_X( 32.0f ) ;
	ew->spd.x = shl03_00_spd[ew->atck_level] * 1.6666666f / 65536.0f ;
	if( ew->disp_dir ) ew->pos.x = ew->pos.x + pl_w->pos.x ;
	else {
		ew->pos.x = -ew->pos.x + pl_w->pos.x ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.y = pl_w->pos.y + DOT_Y( 48.0f ) ;
	ew->spd.y = 0.0f ;
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->char_no.b.grp = 21 ;
	ew->char_no.b.no = 11 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
}

/*==============================================================*/
/*			From the air: down and forward						*/
/*==============================================================*/
static void shl03_00_jsoul_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint32 shl03_00_jspd[][2] = { { 0x30000, 0x30000 }, { 0x38000, 0x38000 } } ;	/* x and y, by strength */

	BODY_SIZE_SET( ew, 0x20202424 ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl03_at_jsoul ;
	HIT_RESET( ew ) ;
	ew->pos.x = DOT_X( 32.0f ) ;
	ew->spd.x = shl03_00_jspd[ew->atck_level][0] * 1.6666666f / 65536.0f ;
	ew->spd.y = -( shl03_00_jspd[ew->atck_level][1] * 2.142857f / 65536.0f ) ;
	if( ew->disp_dir ) ew->pos.x = ew->pos.x + pl_w->pos.x ;
	else {
		ew->pos.x = -ew->pos.x + pl_w->pos.x ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.y = pl_w->pos.y + DOT_Y( 64.0f ) ;
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->char_no.b.grp = 21 ;
	ew->char_no.b.no = 13 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
}

/*==============================================================*/
static void shl03_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_00_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_00_soul_init, shl03_00_jsoul_init,
	} ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	shl03_00_init_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_00_soul_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( ew->hit_flag || ew->hit_box_no ){
		lw->shot_flag = 0 ;
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 21 ;
		ew->char_no.b.no = 12 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_00_soul_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*==============================================================*/
static void shl03_00_soul_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_00_soul_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_00_soul_fly, shl03_00_soul_hit,
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	ew->sub_prio = -6 ;
	if( !scr_in_x_ck( ew ) ){
		lw->shot_flag = 0 ;
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl03_00_soul_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl03_00_jsoul_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	if( ew->hit_flag || ew->hit_box_no || ew->pos.y < pl_w->ground_y ){
		lw->shot_flag = 0 ;
		ew->routine_1 ++ ;
		ew->char_no.b.grp = 21 ;
		ew->char_no.b.no = 14 ;
		pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl03_00_jsoul_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*==============================================================*/
static void shl03_00_jsoul_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_00_jsoul_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_00_jsoul_fly, shl03_00_jsoul_hit,
	} ;
	PL03_LOCAL_W *lw = ( PL03_LOCAL_W * )pl_w->tlocal ;

	ew->sub_prio = -6 ;
	if( !scr_in_x_ck( ew ) ){
		lw->shot_flag = 0 ;
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl03_00_jsoul_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl03_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl03_00_soul_move, shl03_00_jsoul_move,
	} ;

	shl03_00_move_jmp[ew->type]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl03_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl03_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl03_00.c" End of File	*/
