/* War Machine's effect 01 (character 2e's shot 01): one of the blasts his Repulsor Blast fires in a
   row, one for each place (sub_type 0 to 3), each at its own height beside him and with its own
   hit. It lasts while he is in Char_Special. */
#include "player.h"
#include "pl_char.h"
#include "pl2e_def.h"

/* how far above him each piece stands, by place */
static Sint16 shl2e_01_pos_tbl[] = {
	112, 128, 128, 112,
} ;

/* the piece's animation, by War Machine's level and the place */
static Uint8 shl2e_01_char_tbl[] = {
	8, 9, 9, 8, 10, 11, 11, 10,
} ;

/* the piece's attack, by the place */
static Uint8 shl2e_01_atck_tbl[] = {
	0x33, 0x3b, 0x3b, 0x33,
} ;

static void shl2e_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2e_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2e_01_main ;
		ew->ef_no = 0x2e01 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl2e_01_blast_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_01_main( PLAYER_WORK *ew )
{
	static void ( *shl2e_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_01_blast_main,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp != Char_Special ){
		shl2e_01_free( ew, pl_w ) ;
		return ;
	}
	shl2e_01_jmp[ew->type]( ew, pl_w ) ;
}

static void shl2e_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_01_blast_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_01_blast_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_01_init, shl2e_01_move, shl2e_01_end, shl2e_01_free,
	} ;

	shl2e_01_blast_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->sub_prio = 1 ;
	ew->pos.z = pl_w->pos.z ;
	ew->atck_no = shl2e_01_atck_tbl[ew->sub_type] ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, shl2e_01_char_tbl[ew->sub_type + pl_w->atck_level * 4] ) ;
	if( ew->sub_type & 2 ) ew->disp_dir ^= 1 ;
	shl2e_01_move( ew, pl_w ) ;
}

static void shl2e_01_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_01_keep,
	} ;

	shl2e_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_01_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += DOT_Y( shl2e_01_pos_tbl[ew->sub_type] ) ;
	hit_work_set( ew ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl2e_01_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2e_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2e_01.c" End of File	*/
