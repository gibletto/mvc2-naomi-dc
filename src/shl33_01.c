/* Iron Man's effect 01 (character 33's shot 01): one of the blasts his Repulsor Blast fires in a
   row, one for each sub_type (0 to 3), each at its own place beside him and with its own hit. It
   lasts while he is in his firing animation. */
#include "player.h"
#include "pl_char.h"
#include "pl33_def.h"

/* where each blast sits above Iron Man */
static Sint16 shl33_01_ofs_tbl[] = { 0x70, 0x80, 0x80, 0x70 } ;

/* its animation, by his atck_level * 4 + sub_type */
static Uint8 shl33_01_char_tbl[] = { 8, 9, 9, 8, 10, 11, 11, 10 } ;

/* its attack, by sub_type */
static Uint8 shl33_01_atck_tbl[] = { 0x33, 0x3b, 0x3b, 0x33 } ;

static void shl33_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl33_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl33_01_main ;
		ew->ef_no = 0x3301 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl33_01_blast_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl33_01_main( PLAYER_WORK *ew )
{
	static void ( *shl33_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_01_blast_main,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->char_no.b.grp != Char_Special ){
		shl33_01_free( ew ) ;
		return ;
	}
	shl33_01_jmp[ew->type]( ew, pl_w ) ;
}

static void shl33_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_01_blast_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_01_blast_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_01_init, shl33_01_move, shl33_01_end, shl33_01_free,
	} ;

	shl33_01_blast_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->atck_no = shl33_01_atck_tbl[ew->sub_type] ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, shl33_01_char_tbl[ew->sub_type + pl_w->atck_level * 4] ) ;
	if( ew->sub_type & 2 ) ew->disp_dir ^= 1 ;
	shl33_01_move( ew, pl_w ) ;
}

static void shl33_01_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl33_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_01_keep,
	} ;

	shl33_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_01_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += DOT_Y( shl33_01_ofs_tbl[ew->sub_type] ) ;
	hit_work_set( ew ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl33_01_end( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl33_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl33_01_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl33_01.c" End of File	*/
