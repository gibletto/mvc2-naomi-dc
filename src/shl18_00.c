/* Abyss's effect 00 (character 18's shot 00): a dark spark that drops from above the opponent, at a
   random place and in one of four sizes, and strikes where it lands. */
#include "player.h"
#include "pl_char.h"
#include "pl18_def.h"

static PLAYER_WORK *shl18_00_set( PLAYER_WORK *pl_w, Uint8 no, Uint8 pos );

/****************************************************************/
void *shl18_00_set_all( PLAYER_WORK *pl_w, Uint8 no, Uint8 pos )
{
	static PLAYER_WORK *( *shl18_00_set_jmp[] )( PLAYER_WORK *pl_w, Uint8 no, Uint8 pos ) = {
		shl18_00_set,
	} ;

	return shl18_00_set_jmp[no]( pl_w, no, pos ) ;
}

static void shl18_00_main( PLAYER_WORK *ew );

/*==============================================================*/
static PLAYER_WORK *shl18_00_set( PLAYER_WORK *pl_w, Uint8 no, Uint8 pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl18_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = pos ;
		ew->sub_no = 0 ;
	}
	return ew ;
}

static void shl18_00_init( PLAYER_WORK *ew );
static void shl18_00_move( PLAYER_WORK *ew );
static void shl18_00_end( PLAYER_WORK *ew );
static void shl18_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl18_00_main( PLAYER_WORK *ew )
{
	static void ( *shl18_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl18_00_init, shl18_00_move, shl18_00_end, shl18_00_free,
	} ;

	shl18_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl18_00_init( PLAYER_WORK *ew )
{
	static const Float shl18_00_scale_tbl[] = { 1.0f, 0.75f, 1.25f, 1.5f } ;
	PLAYER_WORK *pl_w = ew->master_w ;
	const Float *tbl = shl18_00_scale_tbl ;
	Float x ;
	int n ;

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
	ew->prio = 9 ;
	n = Rnd() ;
	x = ( ( n - 128 ) * 640 ) >> 8 ;
	ew->pos.x = pl_w->target_w->pos.x + x ;
	ew->pos.y = pl_w->target_w->pos.y + 300.0f ;
	ew->pos.z = pl_w->target_w->pos.z ;
	ew->rot[2] = 0xb000 ;
	n = Rnd() ;
	n = n % 4 ;
	ew->scale.x = tbl[n] ;
	ew->scale.y = tbl[n] ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl18_at_thunder ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 1 ) ;
	shl18_00_move( ew ) ;
}

static void shl18_00_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl18_00_over( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl18_00_move( PLAYER_WORK *ew )
{
	static void ( *shl18_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl18_00_fall, shl18_00_over,
	} ;

	shl18_00_move_jmp[ew->routine_1]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl18_00_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		if( ew->disp_dir ) ew->pos.x -= DOT_X( 64.0f ) ;
		else ew->pos.x -= DOT_X( 64.0f ) ;
		ew->pos.y = pl_w->ground_y ;
		ew->rot[2] = 0 ;
		pl_char_set( ew, Char_Effect, 2 ) ;
		ew->scale.x *= 0.25f ;
		ew->scale.y *= 0.375f ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl18_00_over( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl18_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl18_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl18_00.c" End of File	*/
