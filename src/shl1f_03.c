/* B.B. Hood's effect 03 (character 1f's shot 03): the five dishes of her hyper combo, thrown at
   the one she holds (types 0, 2 and 3), and the scraps of the broken ones that fall (type 1). */
#include "player.h"
#include "pl_char.h"
#include "pl1f_def.h"

static void shl1f_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl1f_03_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1f_03_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x1f03 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl1f_03_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1f_03_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x1f03 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The sound and the splash where it breaks			*/
/*==============================================================*/
static void shl1f_03_se( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 pos ;

	pos.x = ew->pos.x ;
	pos.y = ew->pos.y ;
	burst_set( pl_w, &pos, 1, 0xf9 ) ;
	pl_se_req( pl_w, 7 ) ;
}

static void shl1f_03_dish_main( PLAYER_WORK *ew );
static void shl1f_03_scrap_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1f_03_main( PLAYER_WORK *ew )
{
	static void ( *shl1f_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl1f_03_dish_main, shl1f_03_scrap_main, shl1f_03_dish_main, shl1f_03_dish_main,
	} ;

	shl1f_03_jmp[ew->type]( ew ) ;
}

static void shl1f_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1f_03_dish_main( PLAYER_WORK *ew )
{
	static void ( *shl1f_03_dish_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1f_03_init, shl1f_03_move, shl1f_03_end, shl1f_03_free,
	} ;

	shl1f_03_dish_main_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/* how far from the held one each dish lands, in front of her */
static Float shl1f_03_ofs[] = { DOT_X( 64.0f ), DOT_X( 32.0f ), 0.0f, -DOT_X( 32.0f ), -DOT_X( 64.0f ) } ;

/*==============================================================*/
static void shl1f_03_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *cw ;
	Float ofs, tx, ty ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->prio = pl_w->catch_w->prio ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ofs = shl1f_03_ofs[ew->sub_type] ;
	if( pl_w->disp_dir ) ofs = -ofs ;
	cw = pl_w->catch_w ;
	tx = cw->pos.x - ofs ;
	ty = cw->ground_y + DOT_Y( 24.0f ) ;
	ew->spd.x = ( tx - ew->pos.x ) / 32.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = ( ty - ew->pos.y ) / 32.0f + DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	ew->timer0 = 32 ;
	pl_char_set( ew, Char_Effect, 17 ) ;
	shl1f_03_move( ew, pl_w ) ;
}

static void shl1f_03_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_break( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_dummy( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The dish flies to the held one						*/
/*==============================================================*/
static void shl1f_03_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( -- ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		PL1F_LW( pl_w )->dish_hit = ew->sub_type + 1 ;
		if( ew->sub_type == 4 ) PL1F_LW( pl_w )->dish_hit = -1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1f_03_break( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->routine_1 ++ ;
	ew->disp_flag = 0 ;
	shl1f_03_se( ew, pl_w ) ;
	ew->routine_1 = 0 ;
	shl1f_03_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_03_dummy( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void shl1f_03_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1f_03_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1f_03_fly, shl1f_03_break, shl1f_03_dummy,
	} ;

	if( pl_w->team_no && ew->routine_0 < 2 ){
		shl1f_03_se( ew, pl_w ) ;
		work_free( ew ) ;
		return ;
	}
	shl1f_03_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl1f_03_scrap_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_scrap_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1f_03_scrap_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	ew->sub_prio = -8 ;
	ew->timer0 = 0 ;
	ew->timer1 = Rnd() & 3 ;
	ew->obj_flag = 0 ;
	pl_char_set( ew, Char_Effect, 17 ) ;
	shl1f_03_scrap_move( ew, pl_w ) ;
}

static void shl1f_03_scrap_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_scrap_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1f_03_scrap_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It waits a few frames, shows above the held one		*/
/*			and drops in front or behind her.					*/
/*==============================================================*/
static void shl1f_03_scrap_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint32 n ;
	Float dx, ofs, tx ;

	if( !ew->timer1 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 24.0f ) ;
		ew->acc.x = 0.0f ;
		n = ( Rnd() & 7 ) + 4 ;
		ew->spd.y = ( n << 16 ) * 2.142857f / 65536.0f ;
		ew->acc.y = -DOT_Y( 0.75f ) ;
		dx = 5.0f ;
		ofs = -DOT_X( 56.0f ) ;
		if( pl_w->disp_dir ){
			dx = -5.0f ;
			ofs = DOT_X( 56.0f ) ;
		}
		if( pl_w->disp_dir ) tx = pl_w->pos.x + dx + DOT_X( 160.0f ) ;
		else tx = pl_w->pos.x + dx + -DOT_X( 160.0f ) ;
		ew->pos.x = pl_w->pos.x + ofs ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 24.0f ) ;
		ew->spd.x = ( tx - ew->pos.x ) / 16.0f ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl1f_at_apple_scrap ;
		HIT_RESET( ew ) ;
	} else ew->timer1 -- ;
}

/*==============================================================*/
/*			It stops where it hits, and bounces once on the		*/
/*			floor before it stops there							*/
/*==============================================================*/
static void shl1f_03_scrap_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->hit_flag ){
		ew->obj_flag = 1 ;
		ew->routine_1 ++ ;
		ew->timer0 = ( Rnd() & 7 ) + 16 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	if( pl_w->ground_y + DOT_Y( 24.0f ) > ew->pos.y ){
		if( ew->timer0 ){
			ew->routine_1 ++ ;
			ew->timer0 = ( Rnd() & 7 ) + 16 ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
		} else {
			ew->timer0 ++ ;
			ew->spd.y = ( ew->spd.y < 0.0f ) ? -ew->spd.y : ew->spd.y ;
			ew->spd.y /= 2.0f ;
			ew->acc.y /= 2.0f ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl1f_03_scrap_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->obj_flag ) goto done ;
	pl_char_move( ew ) ;
	if( -- ew->timer0 >= 0 ) return ;
done:
	shl1f_03_se( ew, pl_w ) ;
	ew->routine_0 ++ ;
	ew->routine_1 = 0 ;
	shl1f_03_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_03_scrap_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1f_03_scrap_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1f_03_scrap_wait, shl1f_03_scrap_fall, shl1f_03_scrap_stay,
	} ;

	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	if( pl_w->team_no && ew->routine_0 < 2 ){
		shl1f_03_se( ew, pl_w ) ;
		work_free( ew ) ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl1f_03_scrap_move_jmp[ew->routine_1]( ew, pl_w ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl1f_03_scrap_main( PLAYER_WORK *ew )
{
	static void ( *shl1f_03_scrap_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1f_03_scrap_init, shl1f_03_scrap_move, shl1f_03_end, shl1f_03_free,
	} ;

	shl1f_03_scrap_main_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1f_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1f_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1f_03.c" End of File	*/
