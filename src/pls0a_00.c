/* Rogue's effect 00 (her pls0a): a work that shows her animation's spark by her, at her
   spot or off it at random (type 1), until her animation changes. */
#include "player.h"
#include "ef_char.h"

/* the effect's own work, from obj_flag */
typedef struct PLS0A_00_LOCAL_W {
	Uint32	char_no;	/* 000: the master's animation when it was put out */
} PLS0A_00_LOCAL_W;
#define PLS0A_00_LW( w )	( ( PLS0A_00_LOCAL_W * )&( w )->obj_flag )

static void pls0a_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0a_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls0a_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0xa00 ;
	}
	return ew ;
}

static void pls0a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void pls0a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void pls0a_00_main( PLAYER_WORK *ew )
{
	static void ( *pls0a_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0a_00_init, pls0a_00_move, pls0a_00_end, pls0a_00_free,
	} ;

	pls0a_00_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void pls0a_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 x ;
	Sint16 y ;

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
	ew->sub_prio = -1 ;
	x = y = 0 ;
	if( ew->type ){
		x = 15 ;
		if( ew->disp_dir ) x = -15 ;
		x += Rnd() & 7 ;
		y = ( Rnd() & 7 ) + 37 ;
		ew->col_no -= 3 ;
	}
	ew->pos.x = pl_w->pos.x + 1.6666666f * x ;
	ew->pos.y = pl_w->pos.y + 2.142857f * y ;
	PLS0A_00_LW( ew )->char_no = pl_w->char_no.w ;
	ef_char_set( ew, 27, ew->type + 26 ) ;
}

/*--------------------------------------------------------------*/
static void pls0a_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	if( PLS0A_00_LW( ew )->char_no == pl_w->char_no.w && ef_char_move( ew ) >= 0 ) return ;
	pls0a_00_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls0a_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void pls0a_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0a_00.c" End of File	*/
