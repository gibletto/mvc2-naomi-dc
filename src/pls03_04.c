/* Morrigan's effect 04 (character 03's pls04): the flash that comes on her (types 0 and 2) or on
   her double (types 1 and 3), in two sizes, and plays its animation once. */
#include "player.h"
#include "ef_char.h"
#include "pl03_def.h"

static void pls03_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls03_04_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls03_04_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x304 ;
	}
	return ew ;
}

/****************************************************************/
/*			On the work of Morrigan's double, which it			*/
/*			follows												*/
/****************************************************************/
PLAYER_WORK *pls03_04_double_set( PLAYER_WORK *ow, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls03_04_main ;
		ew->master_w = ow->master_w ;
		ew->ef_w = ow ;
		ew->pl_type = ow->pl_type ;
		ew->type = type ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x304 ;
	}
	return ew ;
}

static void pls03_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_04_main( PLAYER_WORK *ew )
{
	static void ( *pls03_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_04_init, pls03_04_move, pls03_04_end, pls03_04_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	pls03_04_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls03_04_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_d_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_e_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_f_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_g_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls03_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_04_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_04_a_init, pls03_04_b_init, pls03_04_c_init, pls03_04_d_init,
		pls03_04_e_init, pls03_04_f_init, pls03_04_g_init,
	} ;

	ew->routine_0 ++ ;
	pls03_04_init_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			On Morrigan, small									*/
/*==============================================================*/
static void pls03_04_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ef_char_set( ew, 27, 2 ) ;
}

/*==============================================================*/
/*			On the work it rides, small							*/
/*==============================================================*/
static void pls03_04_b_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ow = ew->ef_w ;

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
	ew->disp_dir = ow->disp_dir ;
	ew->pos.x = ow->pos.x ;
	ew->pos.y = ow->pos.y ;
	ef_char_set( ew, 27, 2 ) ;
}

/*==============================================================*/
/*			On Morrigan, large									*/
/*==============================================================*/
static void pls03_04_c_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ef_char_set( ew, 27, 3 ) ;
}

/*==============================================================*/
/*			On the work it rides, large							*/
/*==============================================================*/
static void pls03_04_d_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ow = ew->ef_w ;

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
	ew->disp_dir = ow->disp_dir ;
	ew->pos.x = ow->pos.x ;
	ew->pos.y = ow->pos.y ;
	ef_char_set( ew, 27, 3 ) ;
}

/*==============================================================*/
/*			The types that nobody sets							*/
/*==============================================================*/
static void pls03_04_e_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls03_04_f_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls03_04_g_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

static void pls03_04_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_e_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_f_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls03_04_g_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls03_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls03_04_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls03_04_a_move, pls03_04_a_move, pls03_04_a_move, pls03_04_a_move,
		pls03_04_e_move, pls03_04_f_move, pls03_04_g_move,
	} ;

	pls03_04_move_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls03_04_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->sub_prio = -4 ;
	if( ef_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*==============================================================*/
static void pls03_04_e_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls03_04_f_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls03_04_g_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls03_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls03_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls03_04.c" End of File	*/
