/* Servbot's player effect 03: pieces that fly in from the screen's edge to his side, ten of
   one kind (type 0, which turn away again) and ten of another (type 1, which scatter). */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"

static void pls3a_03_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The ten pieces that fly in to his side				*/
/****************************************************************/
PLAYER_WORK *pls3a_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint32 i ;

	for( i = 0 ; i < 10 ; i ++ ){
		if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
			ew->work_func = pls3a_03_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3a03 ;
			ew->type = 0 ;
			ew->sub_type = i ;
		}
	}
	return ew ;
}

/****************************************************************/
/*			The ten pieces that scatter							*/
/****************************************************************/
PLAYER_WORK *pls3a_03_set2( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint32 i ;

	for( i = 0 ; i < 10 ; i ++ ){
		if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
			ew->work_func = pls3a_03_main ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3a03 ;
			ew->type = 1 ;
			ew->sub_type = i ;
		}
	}
	return ew ;
}

static void pls3a_03_gather_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_03_scatter_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_03_main( PLAYER_WORK *ew )
{
	if( !ew->type ) pls3a_03_gather_main( ew, ew->master_w ) ;
	else pls3a_03_scatter_main( ew, ew->master_w ) ;
}

static void pls3a_03_gather_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_03_gather_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_03_gather_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls3a_03_gather_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_03_gather_init, pls3a_03_gather_move, pls3a_03_free, pls3a_03_end,
	} ;

	pls3a_03_gather_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls3a_03_gather_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* where each piece waits past the edge (0 to 9), and where it stops by his side (10 to 19) */
	Sint16 ofs_tbl[20] = {
		0, 32, 64, 96, 128, 16, 48, 80, 112, 144,
		40, -40, 32, -32, 24, -24, 16, -16, 8, -8,
	} ;
	Float x, ofs ;

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
	ew->disp_flag = 0 ;
	ew->prio = 15 ;
	ew->col_no += 6 ;
	x = scr_work.scr_right + DOT_X( 32.0f ) ;
	ofs = DOT_X( ofs_tbl[ew->sub_type] ) ;
	if( pl_w->disp_dir ){
		ofs = -ofs ;
		x = scr_work.scr_left + -DOT_X( 32.0f ) ;
	}
	ew->pos.x = x + ofs ;
	ew->pos.y = pl_w->ground_y ;
	ew->timer0 = 20 ;
	ew->timer1 = Rnd() & 15 ;
	ew->sub_no = 0 ;
	ofs = DOT_X( ofs_tbl[10 + ew->sub_type] ) ;
	ew->spd.x = ( pl_w->pos.x + ofs - ew->pos.x ) / 20.0f ;
	ew->spd.z = ofs ;
	ew->acc.x = 0 ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 16 ;
	ef_char_set( ew, 27, 15 ) ;
	ew->be_flag = 1 ;
	ew->cg_shadow = 0 ;
	pl_shadow_set( ew ) ;
}

static void pls3a_03_gather_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_03_gather_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_03_gather_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls3a_03_gather_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls3a_03_gather_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_03_gather_in, pls3a_03_gather_stay, pls3a_03_gather_out,
	} ;

	pls3a_03_gather_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static int pls3a_03_gather_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Flies in to his side								*/
/*==============================================================*/
static void pls3a_03_gather_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( --ew->timer1 > 0 ){
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( pls3a_03_gather_turn( ew, pl_w ) ) return ;
	ef_char_move( ew ) ;
	ew->cg_shadow = 0 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( ew->timer0 > 0 ){
		x = ew->spd.z ;
		if( pl_w->disp_dir ) x = -x ;
		if( ( x + pl_w->pos.x - ew->pos.x ) * ew->spd.x < 0 ){
			ew->timer0 = 0 ;
			goto next ;
		}
	}
	if( --ew->timer0 ) return ;
next:
	ew->routine_1 ++ ;
	ew->col_no -= 6 ;
	ew->spd.x = 0 ;
	ew->acc.x = 0 ;
	ef_char_set( ew, 27, 16 ) ;
}

/*==============================================================*/
/*			Stays by his side									*/
/*==============================================================*/
static void pls3a_03_gather_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( pls3a_03_gather_turn( ew, pl_w ) ) return ;
	x = ew->spd.z ;
	ew->spd.z = ew->spd.z * 15.0f / 16.0f ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ef_char_move( ew ) ;
	ew->cg_shadow = 0 ;
	if( pl_w->cg_prm < 0 ) pls3a_03_free( ew, pl_w ) ;
}

/*==============================================================*/
/*			Flies off											*/
/*==============================================================*/
static void pls3a_03_gather_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) ) pls3a_03_free( ew, pl_w ) ;
}

/*==============================================================*/
/*			Once he is hit, turns away and starts off; 1		*/
/*			when it did											*/
/*==============================================================*/
static int pls3a_03_gather_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	if( !pl_w->routine_1 && pl_w->act_st == N_SUPER ) return 0 ;
	ew->routine_1 = 2 ;
	ew->disp_dir ^= 1 ;
	x = ( ( Rnd() & 7 ) + 4 ) * 1.6666666f ;
	if( !ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->acc.x = 0 ;
	pl_char_set( ew, Char_Effect, 43 ) ;
	return 1 ;
}

static void pls3a_03_scatter_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_03_scatter_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_03_scatter_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls3a_03_scatter_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_03_scatter_init, pls3a_03_scatter_move, pls3a_03_free, pls3a_03_end,
	} ;

	pls3a_03_scatter_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls3a_03_scatter_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* where each piece starts from him, its speed, its animation and its turn */
	Sint16 pos_tbl[10][2] = {
		{ 64, 0 }, { -64, 0 }, { 56, 48 }, { -56, 48 }, { 48, 80 },
		{ -48, 80 }, { 32, 96 }, { -32, 96 }, { 16, 112 }, { -16, 112 },
	} ;
	Uint8 no_tbl[10] = { 15, 15, 18, 18, 18, 18, 17, 17, 17, 17 } ;
	Sint16 spd_tbl[10][2] = {
		{ 2048, 0 }, { -2048, 0 }, { 1792, 256 }, { -1792, 256 }, { 1536, 768 },
		{ -1536, 768 }, { 896, 1280 }, { -896, 1280 }, { 256, 1536 }, { -256, 1536 },
	} ;
	Sint16 rot_tbl[5] = { 0, 1024, -1024, 0, -4096 } ;
	Float x ;

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
	ew->prio = 15 ;
	ew->col_no += 6 ;
	if( !( ew->sub_type & 1 ) ) ew->disp_dir ^= 1 ;
	x = pos_tbl[ew->sub_type][0] ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->ground_y + pos_tbl[ew->sub_type][1] ;
	x = spd_tbl[ew->sub_type][0] * 1.6666666f / 256.0f ;
	if( pl_w->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->spd.y = spd_tbl[ew->sub_type][1] * 2.142857f / 256.0f ;
	ew->acc.x = ew->acc.y = 0 ;
	ew->rot[0] = ew->rot[1] = 0 ;
	ew->rot[2] = rot_tbl[ew->sub_type >> 1] ;
	ew->size_top = ew->size_bottom = 64 ;
	ew->size_front = ew->size_back = 32 ;
	ef_char_set( ew, 27, no_tbl[ew->sub_type] ) ;
	ew->be_flag = 1 ;
	ew->cg_shadow = 0 ;
	pl_shadow_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_03_scatter_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->cg_shadow = 0 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_ck( ew ) ) pls3a_03_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_03_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 3 ;
	ew->be_flag = 0 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls3a_03_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls3a_03.c" End of File	*/
