/* Sabretooth's effect 01 (character 2b's shot 01): the bits that fly off a work, thrown out with
   a speed picked by the pose number and the type. Each strikes while it flies. */
#include "player.h"
#include "pl_char.h"
#include "pl2b_def.h"

/* where each bit starts from its work, and the speeds it takes (16.16), by the number given */
static Sint16 shl2b_01_pos_tbl[] = {
	-46, 74, -40, 86, -44, 61,
} ;

static Sint32 shl2b_01_spd_0[] = {
	-0x80000, 0, -0x80000, 0x1c000, -0x80000, -0x1c000,
} ;

static Sint32 shl2b_01_spd_1[] = {
	-0x100000, 0, -0x100000, 0x1c000, -0x100000, -0x1c000,
} ;

static Sint32 *shl2b_01_spd_tbl[] = {
	shl2b_01_spd_0, shl2b_01_spd_1,
} ;

static void shl2b_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2b_01_set( PLAYER_WORK *ef_w, PLAYER_WORK *pl_w, int type, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->ef_no = 0x2b01 ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->work_func = shl2b_01_main ;
		ew->ef_w = ef_w ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl2b_01_init( PLAYER_WORK *ew );
static void shl2b_01_move( PLAYER_WORK *ew );
static void shl2b_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl2b_01_main( PLAYER_WORK *ew )
{
	static void ( *shl2b_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl2b_01_init, shl2b_01_move, shl2b_01_free,
	} ;

	shl2b_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl2b_01_init( register PLAYER_WORK *ew )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	Sint32 *spd ;
	Sint16 *pos ;
	Sint32 i ;
	Sint32 atck_no ;
	register Float ofs_x, ofs_y ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ef_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ef_w->pl_no ;
	ew->pl_type = ef_w->pl_type ;
	ew->scale.x = ef_w->scale.x ;
	ew->scale.y = ef_w->scale.y ;
	ew->atck_level = ef_w->atck_level ;
	ew->work_no = ef_w->work_no ;
	ew->col_cnt = ef_w->col_cnt ;
	ew->scale = ef_w->scale ;
	ew->prio = ef_w->prio ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->prio = 0 ;
	ew->disp_dir = ef_w->disp_dir ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->timer0 = 20 ;
	ew->pos = ef_w->pos ;
	i = ew->sub_type * 2 ;
	pos = &shl2b_01_pos_tbl[i] ;
	ofs_x = pos[0] ;
	ofs_y = pos[1] ;
	spd = shl2b_01_spd_tbl[ew->type] + i ;
	ew->spd.x = *spd ++ * 1.6666666f / 65536.0f ;
	ew->spd.y = *spd * 2.142857f / 65536.0f ;
	if( ew->disp_dir ){
		ofs_x = -ofs_x ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.x += DOT_X( ofs_x ) ;
	ew->pos.y += DOT_Y( ofs_y ) ;
	if( !ew->type ) atck_no = 50 ;
	else atck_no = 57 + ( Rnd() & 1 ) ;
	ew->atck_no = atck_no ;
	HIT_RESET( ew ) ;
	hit_work_set( ew ) ;
	pl_char_set( ew, Char_Effect, 9 ) ;
}

/*--------------------------------------------------------------*/
static void shl2b_01_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->timer0 ) ew->timer0 -- ;
	else if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2b_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2b_01.c" End of File	*/
