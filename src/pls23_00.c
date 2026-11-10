/* Dan's player effect 00: three kinds of work. Type 0 is a picture at his feet that stays while
   his animation lasts, type 1 a piece that is thrown out, bounces on the ground and blinks away,
   type 2 is put at his side and ends at once. */
#include "player.h"
#include "pl_char.h"

/* the effect's own work, from obj_flag */
typedef struct PLS23_00_LOCAL_W {
	Uint16	char_no;	/* 000: his animation when the effect began */
} PLS23_00_LOCAL_W;
#define PLS23_00_LW( w )	( ( PLS23_00_LOCAL_W * )&( w )->obj_flag )

static void pls23_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls23_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 unk )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		PLS23_00_LOCAL_W *lw ;

		ew->work_func = pls23_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = unk ;
		ew->ef_no = 0x2300 ;
		ew->pl_type = pl_w->pl_type ;
		lw = PLS23_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls23_00_init( PLAYER_WORK *ew );
static void pls23_00_move( PLAYER_WORK *ew );
static void pls23_00_end( PLAYER_WORK *ew );
static void pls23_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls23_00_main( PLAYER_WORK *ew )
{
	static void ( *pls23_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls23_00_init, pls23_00_move, pls23_00_end, pls23_00_free,
	} ;

	pls23_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls23_00_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	pls23_00_move( ew ) ;
}

static void pls23_00_t0( PLAYER_WORK *ew );
static void pls23_00_piece( PLAYER_WORK *ew );
static void pls23_00_t2( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls23_00_move( PLAYER_WORK *ew )
{
	static void ( *pls23_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls23_00_t0, pls23_00_piece, pls23_00_t2,
	} ;

	pls23_00_move_jmp[ew->type]( ew ) ;
}

static void pls23_00_t0_init( PLAYER_WORK *ew );
static void pls23_00_t0_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls23_00_t0( PLAYER_WORK *ew )
{
	static void ( *pls23_00_t0_jmp[] )( PLAYER_WORK *ew ) = {
		pls23_00_t0_init, pls23_00_t0_move,
	} ;

	pls23_00_t0_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls23_00_t0_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
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
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y + -DOT_Y( 3.0f ) ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*--------------------------------------------------------------*/
static void pls23_00_t0_move( PLAYER_WORK *ew )
{
	PLS23_00_LOCAL_W *lw = PLS23_00_LW( ew ) ;

	if( lw->char_no != ew->master_w->char_no.w ) ew->routine_0 = 2 ;
}

static void pls23_00_piece_init( PLAYER_WORK *ew );
static void pls23_00_piece_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls23_00_piece( PLAYER_WORK *ew )
{
	static void ( *pls23_00_piece_jmp[] )( PLAYER_WORK *ew ) = {
		pls23_00_piece_init, pls23_00_piece_move,
	} ;

	pls23_00_piece_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls23_00_piece_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
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
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->sub_type ){
		ew->spd.x = -1.6666666f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 6.0f ) ;
	}else{
		ew->spd.x = -DOT_X( 0.5f ) ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 12.0f ) ;
	}
	ew->acc.y = -DOT_Y( 0.25f ) ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	pl_char_set( ew, Char_Effect, 1 ) ;
}

static void pls23_00_piece_bound( PLAYER_WORK *ew );
static void pls23_00_piece_blink( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls23_00_piece_move( PLAYER_WORK *ew )
{
	static void ( *pls23_00_piece_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls23_00_piece_bound, pls23_00_piece_bound, pls23_00_piece_bound, pls23_00_piece_blink,
	} ;

	pls23_00_piece_move_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
/*			It flies, and lands at the ground: the third		*/
/*			landing it blinks									*/
/*==============================================================*/
static void pls23_00_piece_bound( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->master_w->ground_y > ew->pos.y ){
		ew->routine_2 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->timer0 = 60 ;
		ew->spd.y = -ew->spd.y ;
		ew->spd.y = ew->spd.y / 2.0f ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls23_00_piece_blink( PLAYER_WORK *ew )
{
	ew->timer0 -- ;
	if( ew->timer0 >= 30 ) ew->disp_flag = 1 ;
	else if( ew->timer0 < 0 ) ew->routine_0 = 2 ;
	else if( ew->timer0 & 1 ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
}

static void pls23_00_t2_init( PLAYER_WORK *ew );
static void pls23_00_t2_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls23_00_t2( PLAYER_WORK *ew )
{
	static void ( *pls23_00_t2_jmp[] )( PLAYER_WORK *ew ) = {
		pls23_00_t2_init, pls23_00_t2_move,
	} ;

	pls23_00_t2_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls23_00_t2_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 14.0f ) ;
	else ew->pos.x += -DOT_X( 14.0f ) ;
	ew->pos.y += DOT_Y( 79.0f ) ;
}

/*--------------------------------------------------------------*/
static void pls23_00_t2_move( PLAYER_WORK *ew )
{
	ew->routine_0 = 2 ;
}

/*--------------------------------------------------------------*/
static void pls23_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->routine_0 = 3 ;
}

/*--------------------------------------------------------------*/
static void pls23_00_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls23_00.c" End of File	*/
