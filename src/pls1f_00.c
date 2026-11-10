/* B.B. Hood's player effect 00: the marks and the small things she sheds, ten types, set up by type
   (0 to 9) and moved by type. Some ride on a player, some fly off with a speed and a pull, and some
   are made by another of them (the ef_w it rides on). */
#include "player.h"
#include "ef_char.h"
#include "pl1f_def.h"

/* type 0: the animation and place its mark has, by the animation number its master gives */
const Uint8 pls1f_00_anm_tbl[62][2] = {
	{ 0, 0 }, { 60, 0 }, { 5, 1 }, { 6, 2 }, { 7, 3 }, { 8, 4 }, { 65, 23 }, { 5, 5 },
	{ 6, 6 }, { 7, 7 }, { 8, 8 }, { 5, 13 }, { 8, 16 }, { 65, 9 }, { 11, 0 }, { 12, 0 },
	{ 13, 0 }, { 15, 0 }, { 16, 0 }, { 17, 0 }, { 104, 0 }, { 105, 0 }, { 106, 0 }, { 107, 0 },
	{ 108, 0 }, { 109, 0 }, { 110, 0 }, { 111, 0 }, { 112, 0 }, { 113, 0 }, { 114, 0 }, { 115, 0 },
	{ 116, 0 }, { 117, 0 }, { 118, 0 }, { 119, 0 }, { 120, 0 }, { 121, 0 }, { 5, 26 }, { 115, 0 },
	{ 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 },
	{ 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 }, { 115, 0 },
	{ 28, 20 }, { 29, 21 }, { 30, 22 }, { 28, 17 }, { 29, 18 }, { 30, 19 }
} ;
const Float pls1f_00_pos_tbl[54] = {
	0.0f, 0.0f, -DOT_X( 59.0f ), DOT_Y( 66.0f ), -100.0f, DOT_Y( 60.0f ),
	-100.0f, DOT_Y( 55.0f ), -DOT_X( 55.0f ), DOT_Y( 48.0f ), -DOT_X( 62.0f ), DOT_Y( 39.0f ),
	-DOT_X( 62.0f ), DOT_Y( 34.0f ), -105.0f, DOT_Y( 29.0f ), -DOT_X( 56.0f ), 45.0f,
	-60.0f, DOT_Y( 57.0f ), -110.0f, DOT_Y( 71.0f ), -DOT_X( 67.0f ), DOT_Y( 78.0f ),
	-DOT_X( 68.0f ), DOT_Y( 80.0f ), -DOT_X( 74.0f ), DOT_Y( 71.0f ), -DOT_X( 73.0f ), DOT_Y( 65.0f ),
	-110.0f, DOT_Y( 55.0f ), -DOT_X( 65.0f ), DOT_Y( 55.0f ), -DOT_X( 22.0f ), -15.0f,
	-DOT_X( 31.0f ), -DOT_Y( 6.0f ), -65.0f, 0.0f, -50.0f, -DOT_Y( 3.0f ),
	-65.0f, -DOT_Y( 2.0f ), -75.0f, DOT_Y( 4.0f ), -55.0f, DOT_Y( 66.0f ),
	-55.0f, DOT_Y( 66.0f ), -DOT_X( 41.0f ), DOT_Y( 61.0f ), -DOT_X( 65.0f ), 135.0f
} ;

/* types 4 and 9: the place, and the speed and pull on each axis, by direction */
const Uint8 pls1f_00_dir_anm[8] = { 0xff, 0x01, 0x02, 0x03, 0x04, 0x1f, 0x20, 0xff } ;
const Sint8 pls1f_00_dir_prio[8] = { -24, -18, 8, -18, 8, -18, 8, -24 } ;
const Float pls1f_00_start_tbl[20] = {
	-DOT_X( 22.0f ), DOT_Y( 99.0f ), -DOT_X( 31.0f ), DOT_Y( 96.0f ), -DOT_X( 38.0f ), 195.0f,
	-75.0f, DOT_Y( 88.0f ), -85.0f, DOT_Y( 82.0f ), -100.0f, 75.0f,
	-115.0f, DOT_Y( 32.0f ), -DOT_X( 76.0f ), DOT_Y( 50.0f ), -DOT_X( 56.0f ), DOT_Y( 50.0f ),
	-DOT_X( 89.0f ), DOT_Y( 18.0f )
} ;
const Float pls1f_00_spd_tbl[40] = {
	0.0f, 0.0f, DOT_Y( 6.0f ), -0.4017857f,
	-1.6666666f, 0.052083332f, DOT_Y( 5.0f ), -0.3348214f,
	-DOT_X( 2.0f ), 0.104166664f, DOT_Y( 4.0f ), -DOT_Y( 0.125f ),
	-5.0f, 0.15625f, DOT_Y( 3.0f ), -0.20089285f,
	-DOT_X( 4.0f ), DOT_X( 0.125f ), DOT_Y( 2.0f ), -0.13392857f,
	0.0f, 0.0f, 0.0f, -0.0f,
	-1.6666666f, 0.052083332f, 0.0f, -0.0f,
	-DOT_X( 2.0f ), 0.104166664f, 0.0f, -0.0f,
	-5.0f, 0.15625f, 0.0f, -0.0f,
	-DOT_X( 4.0f ), DOT_X( 0.125f ), 0.0f, -0.0f
} ;

/* type 2: the place by the animation number its master gives, with the master's cg_type set or not */
const Float pls1f_00_ofs_tbl0[18] = {
	0.0f, 0.0f, DOT_X( 16.0f ), -DOT_Y( 15.0f ), 30.0f, -DOT_Y( 11.0f ),
	35.0f, -DOT_Y( 6.0f ), 35.0f, -2.142857f, DOT_X( 8.0f ), DOT_Y( 16.0f ),
	DOT_X( 11.0f ), DOT_Y( 18.0f ), DOT_X( 16.0f ), DOT_Y( 13.0f ), DOT_X( 10.0f ), 30.0f
} ;
const Float pls1f_00_ofs_tbl1[18] = {
	0.0f, 0.0f, DOT_X( 16.0f ), -DOT_Y( 13.0f ), DOT_X( 19.0f ), -DOT_Y( 10.0f ),
	35.0f, -DOT_Y( 5.0f ), DOT_X( 22.0f ), -2.142857f, DOT_X( 10.0f ), 30.0f,
	DOT_X( 13.0f ), 30.0f, DOT_X( 17.0f ), DOT_Y( 10.0f ), DOT_X( 16.0f ), DOT_Y( 10.0f )
} ;

/* the k-th of the n floats the table keeps for entry i */
#define PLS1F_00_ELEM0( tbl, n, i )	( *( ( tbl ) + ( i ) * ( n ) ) )
#define PLS1F_00_ELEM( tbl, n, i, k )	( *( ( tbl ) + ( i ) * ( n ) + ( k ) ) )

static void pls1f_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls1f_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1f_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->dir = dir ;
		ew->ef_no = 0x1f00 ;
	}
	return ew ;
}

/****************************************************************/
/*			Made by another effect: it rides on that one,		*/
/*			and has its master									*/
/****************************************************************/
PLAYER_WORK *pls1f_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls1f_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = type ;
		ew->dir = dir ;
		ew->ef_no = 0x1f00 ;
	}
	return ew ;
}

static void pls1f_00_init( PLAYER_WORK *ew );
static void pls1f_00_move( PLAYER_WORK *ew );
static void pls1f_00_next( PLAYER_WORK *ew );
static void pls1f_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1f_00_main( PLAYER_WORK *ew )
{
	static void ( *pls1f_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls1f_00_init, pls1f_00_move, pls1f_00_next, pls1f_00_free,
	} ;

	pls1f_00_jmp[ew->routine_0]( ew ) ;
}

static void pls1f_00_t0_init( PLAYER_WORK *ew );
static void pls1f_00_t1_init( PLAYER_WORK *ew );
static void pls1f_00_t2_init( PLAYER_WORK *ew );
static void pls1f_00_t3_init( PLAYER_WORK *ew );
static void pls1f_00_t4_init( PLAYER_WORK *ew );
static void pls1f_00_t5_init( PLAYER_WORK *ew );
static void pls1f_00_t6_init( PLAYER_WORK *ew );
static void pls1f_00_t7_init( PLAYER_WORK *ew );
static void pls1f_00_t8_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1f_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	static void ( *pls1f_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls1f_00_t0_init, pls1f_00_t1_init, pls1f_00_t2_init, pls1f_00_t3_init, pls1f_00_t4_init,
		pls1f_00_t5_init, pls1f_00_t6_init, pls1f_00_t7_init, pls1f_00_t8_init, pls1f_00_t4_init,
	} ;

	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	pl_w = ew->master_w ;
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
	ew->pos = pl_w->pos ;
	pls1f_00_init_jmp[ew->type]( ew ) ;
	pls1f_00_move( ew ) ;
}

/*==============================================================*/
static void pls1f_00_t0_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ef_char_set( ew, 27, 0 ) ;
}

/*==============================================================*/
static void pls1f_00_t1_init( PLAYER_WORK *ew )
{
	ew->sub_prio = pls1f_00_dir_prio[ew->dir] ;
	ef_char_set( ew, 27, pls1f_00_dir_anm[ew->dir] ) ;
}

/*==============================================================*/
static void pls1f_00_t2_init( PLAYER_WORK *ew )
{
	ew->sub_prio = -10 ;
	ef_char_set( ew, 27, 67 ) ;
}

/*==============================================================*/
static void pls1f_00_t3_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	ew->sub_prio = -8 ;
	if( ew->dir != 2 ){
		ew->pos.x = -DOT_X( 32.0f ) ;
		ew->pos.y = 0.0f ;
	}else{
		ew->pos.x = -DOT_X( 17.0f ) ;
		ew->pos.y = DOT_Y( 20.0f ) ;
	}
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->acc.x = -0.052083332f ;
	ew->acc.y = 0.06696428f ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x = -ew->pos.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	ef_char_set( ew, 27, 59 ) ;
}

/*==============================================================*/
static void pls1f_00_t4_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = PLS1F_00_ELEM0( pls1f_00_start_tbl, 2, ew->dir ) ;
	ew->pos.y = PLS1F_00_ELEM( pls1f_00_start_tbl, 2, ew->dir, 1 ) ;
	ew->spd.x = PLS1F_00_ELEM0( pls1f_00_spd_tbl, 4, ew->dir ) ;
	ew->acc.x = PLS1F_00_ELEM( pls1f_00_spd_tbl, 4, ew->dir, 1 ) ;
	ew->spd.y = PLS1F_00_ELEM( pls1f_00_spd_tbl, 4, ew->dir, 2 ) ;
	ew->acc.y = PLS1F_00_ELEM( pls1f_00_spd_tbl, 4, ew->dir, 3 ) ;
	if( ( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	if( ew->type == 9 ) ef_char_set( ew, 27, 122 ) ;
	else ef_char_set( ew, 27, 10 ) ;
	pl_se_req( pl_w, 75 ) ;
}

/*==============================================================*/
static void pls1f_00_t5_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->timer0 = 129 ;
	ew->sub_prio = 10 ;
	ew->pos.x = DOT_X( 32.0f ) ;
	ew->pos.y = 0.0f ;
	if( ( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = -ew->pos.x ;
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	ef_char_set( ew, 27, 41 ) ;
	pls1f_00_set2( ew, 8, 0 ) ;
}

/*==============================================================*/
static void pls1f_00_t6_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->dir = 1 ;
	ew->timer0 = 24 ;
	ew->timer1 = 16 ;
	ew->sub_prio = -8 ;
	ew->pos.x = -DOT_X( 32.0f ) ;
	ew->pos.y = 0.0f ;
	ew->spd.x = DOT_X( 2.0f ) ;
	ew->acc.x = -0.0065104165f ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	if( ( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	ef_char_set( ew, 27, 43 ) ;
}

/*==============================================================*/
static void pls1f_00_t7_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->dir = 11 ;
	ew->timer0 = 24 ;
	ew->timer1 = 16 ;
	ew->sub_prio = 18 ;
	ew->rot[2] = 0x8000 ;
	ew->pos.x = 225.0f ;
	ew->pos.y = DOT_Y( 333.0f ) ;
	ew->spd.x = -DOT_X( 2.0f ) ;
	ew->acc.x = -0.0065104165f ;
	ew->spd.y = -DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	if( ( ew->disp_dir = pl_w->disp_dir ) ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
	ef_char_set( ew, 27, 43 ) ;
}

/*==============================================================*/
static void pls1f_00_t8_init( PLAYER_WORK *ew )
{
	ew->sub_prio = 8 ;
	ef_char_set( ew, 27, 44 ) ;
}

static void pls1f_00_t0_move( PLAYER_WORK *ew );
static void pls1f_00_t1_move( PLAYER_WORK *ew );
static void pls1f_00_t2_move( PLAYER_WORK *ew );
static void pls1f_00_t3_move( PLAYER_WORK *ew );
static void pls1f_00_t5_move( PLAYER_WORK *ew );
static void pls1f_00_t6_move( PLAYER_WORK *ew );
static void pls1f_00_t7_move( PLAYER_WORK *ew );
static void pls1f_00_t8_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls1f_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	static void ( *pls1f_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls1f_00_t0_move, pls1f_00_t1_move, pls1f_00_t2_move, pls1f_00_t3_move, pls1f_00_t3_move,
		pls1f_00_t5_move, pls1f_00_t6_move, pls1f_00_t7_move, pls1f_00_t8_move, pls1f_00_t3_move,
	} ;

	if( !( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) ){
		ew->prio = pl_w->prio ;
		pls1f_00_move_jmp[ew->type]( ew ) ;
	}
}

/*==============================================================*/
/*			Type 0: her mark, hidden while her animation		*/
/*			gives none											*/
/*==============================================================*/
static void pls1f_00_t0_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint8 no ;
	const Uint8 *anm ;

	ew->disp_flag = 0 ;
	no = HI_BYTE( pl_w->cg_rival ) ;
	if( no ) ew->disp_flag = 1 ;
	anm = pls1f_00_anm_tbl[no] ;
	if( no == ew->dir ){
		ef_char_move( ew ) ;
	}else{
		ew->dir = no ;
		ef_char_set( ew, 27, anm[0] ) ;
	}
	ew->sub_prio = -8 ;
	if( ew->cg_type ) ew->sub_prio = 8 ;
	ew->pos.x = PLS1F_00_ELEM0( pls1f_00_pos_tbl, 2, anm[1] ) ;
	ew->pos.y = PLS1F_00_ELEM( pls1f_00_pos_tbl, 2, anm[1], 1 ) ;
	if( ( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = -ew->pos.x ;
	ew->pos.x += pl_w->pos.x ;
	ew->pos.y += pl_w->pos.y ;
}

/*==============================================================*/
static void pls1f_00_t1_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) ew->routine_0 = 2 ;
}

/*==============================================================*/
static void pls1f_00_t2_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( pl_w->routine_0 < 2 && !pl_w->routine_1 ){
		if( ew->cg_type ){
			ew->pos.x = PLS1F_00_ELEM0( pls1f_00_ofs_tbl0, 2, HI_BYTE( pl_w->cg_rival ) ) ;
			ew->pos.y = PLS1F_00_ELEM( pls1f_00_ofs_tbl0, 2, HI_BYTE( pl_w->cg_rival ), 1 ) ;
		}else{
			ew->pos.x = PLS1F_00_ELEM0( pls1f_00_ofs_tbl1, 2, HI_BYTE( pl_w->cg_rival ) ) ;
			ew->pos.y = PLS1F_00_ELEM( pls1f_00_ofs_tbl1, 2, HI_BYTE( pl_w->cg_rival ), 1 ) ;
		}
		if( ( ew->disp_dir = pl_w->disp_dir ) ) ew->pos.x = -ew->pos.x ;
		ew->pos.x += pl_w->pos.x ;
		ew->pos.y += pl_w->pos.y ;
		ef_char_move( ew ) ;
	}else ew->routine_0 = 2 ;
}

/*==============================================================*/
/*			Types 3, 4 and 9: fly by their speed and pull		*/
/*==============================================================*/
static void pls1f_00_t3_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ef_char_move( ew ) < 0 ) ew->routine_0 = 2 ;
}

/*==============================================================*/
static void pls1f_00_t5_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_1 == 0 ){
		if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 1 || --ew->timer0 < 0 ){
			ew->routine_1 ++ ;
			ef_char_set( ew, 27, 42 ) ;
		}
		ef_char_move( ew ) ;
	}else{
		if( ef_char_move( ew ) < 0 ) ew->routine_0 = 2 ;
	}
}

/*==============================================================*/
static void pls1f_00_t6_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 1 ) goto fail ;
	ef_char_move( ew ) ;
	if( ew->dir -- == 0 ){
		ew->dir = 0 ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( --ew->timer0 < 0 ){
fail:
			ew->routine_0 = 2 ;
			return ;
		}
		if( ew->timer1 -- == 0 ){
			ew->disp_flag = Sys->game_timer0 & 1 ;
			ew->timer1 = 0 ;
		}
	}
}

/*==============================================================*/
static void pls1f_00_t7_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 1 ) goto fail ;
	ef_char_move( ew ) ;
	if( !ew->routine_1 ){
		if( ew->dir -- == 0 ){
			ew->routine_1 ++ ;
			ew->sub_prio = -8 ;
			ew->pos.x = 140.0f ;
			ew->pos.y = DOT_Y( 241.0f ) ;
			if( pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
			ew->pos.x += pl_w->pos.x ;
			ew->pos.y += pl_w->pos.y ;
		}
	}else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( --ew->timer0 < 0 ){
fail:
			ew->routine_0 = 2 ;
		}else if( ew->timer1 -- == 0 ){
			ew->disp_flag = Sys->game_timer0 & 1 ;
			ew->timer1 = 0 ;
		}
	}
}

/*==============================================================*/
static void pls1f_00_t8_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( ew->master_w->act_st != N_SUPER || ew->master_w->pl_sp_type != 1 || pl_w->cg_type == -1 ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ( ew->disp_flag = pl_w->cg_type & 1 ) ){
		if( Sys->game_timer0 & 1 ){
			ew->pos.x = -DOT_X( 128.0f ) ;
			ew->pos.y = DOT_Y( 118.0f ) ;
		}else{
			ew->pos.x = -DOT_X( 154.0f ) ;
			ew->pos.y = DOT_Y( 113.0f ) ;
		}
		if( pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
		ew->pos.x += pl_w->pos.x ;
		ew->pos.y += pl_w->pos.y ;
		ef_char_move( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls1f_00_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls1f_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1f_00.c" End of File	*/
