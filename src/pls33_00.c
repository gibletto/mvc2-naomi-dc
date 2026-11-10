/* Iron Man's player effect 00, three kinds by type: the thing he leaves in his entrance, which hops
   and flies off (0); its piece on the ground (1); and the flash where the Unibeam starts, on the
   ground and in the air (2). */
#include "player.h"
#include "pl_char.h"
#include "pl33_def.h"

/* the effect's own work, from obj_flag */
typedef struct PLS33_00_LOCAL_W {
	Uint16	char_no;	/* 00: his animation when it was made; the piece ends when it changes */
} PLS33_00_LOCAL_W;
#define PLS33_00_LW( w )	( ( PLS33_00_LOCAL_W * )&( w )->obj_flag )

static void pls33_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls33_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 kind )
{
	PLAYER_WORK *ew ;
	PLS33_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls33_00_main ;
		ew->ef_no = 0x3300 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = kind ;
		lw = PLS33_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls33_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls33_00_appear_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls33_00_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls33_00_main( PLAYER_WORK *ew )
{
	static void ( *pls33_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls33_00_appear, pls33_00_appear_ground, pls33_00_flash,
	} ;

	pls33_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls33_00_hide( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls33_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

static void pls33_00_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls33_00_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls33_00_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, 18, 4 ) ;
	ew->timer0 = 60 ;
	pls33_00_appear_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls33_00_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 18, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls33_00_appear_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 16 ;
		ew->spd.y = DOT_Y( 1.0f ) ;
		pl_char_set( ew, 18, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls33_00_appear_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->timer0 = 16 ;
		pl_char_set( ew, 18, 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls33_00_appear_up_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )pl_w->tlocal ;

	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->timer0 = 30 ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = DOT_Y( 0.25f ) ;
		pl_char_set( ew, 18, 8 ) ;
		lw->land_flag = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pls33_00_appear_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls33_00_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls33_00_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls33_00_appear_wait, pls33_00_appear_start, pls33_00_appear_up, pls33_00_appear_up_next, pls33_00_appear_fly,
	} ;

	ew->prio = pl_w->prio ;
	pls33_00_appear_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls33_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls33_00_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls33_00_appear_init, pls33_00_appear_move, pls33_00_hide, pls33_00_free,
	} ;

	pls33_00_appear_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls33_00_appear_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls33_00_appear_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls33_00_appear_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ){
		ew->pos.x += DOT_X( 20.0f ) ;
	} else {
		ew->pos.x -= DOT_X( 20.0f ) ;
	}
	pl_char_set( ew, 18, 10 ) ;
	ew->timer0 = 60 ;
	pls33_00_appear_ground_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls33_00_appear_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )&pl_w->tlocal[0] ;

	pl_char_move( ew ) ;
	if( lw->land_flag ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls33_00_appear_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls33_00_appear_ground_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls33_00_appear_ground_init, pls33_00_appear_ground_move, pls33_00_hide, pls33_00_free,
	} ;

	ew->prio = pl_w->prio ;
	pls33_00_appear_ground_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls33_00_flash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls33_00_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls33_00_flash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->sub_prio = -1 ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	pls33_00_flash_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls33_00_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls33_00_flash_tbl[] = {
		-12, 80, -23, 82,
	} ;
	PL33_LOCAL_W *lw = ( PL33_LOCAL_W * )pl_w->tlocal ;
	PLS33_00_LOCAL_W *ew_lw = PLS33_00_LW( ew ) ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.w != ew_lw->char_no ){
		ew->routine_0 ++ ;
		pls33_00_hide( ew, pl_w ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ){
		ew->pos.x += DOT_X( pls33_00_flash_tbl[ew->sub_type * 2] ) ;
	} else {
		ew->pos.x -= DOT_X( pls33_00_flash_tbl[ew->sub_type * 2] ) ;
	}
	ew->pos.y += pls33_00_flash_tbl[ew->sub_type * 2 + 1] * DOT_Y( 1.0f ) ;
	if( lw->beam_end_flag ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls33_00_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls33_00_flash_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls33_00_flash_init, pls33_00_flash_move, pls33_00_hide, pls33_00_free,
	} ;

	pls33_00_flash_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls33_00_hide( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls33_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls33_00.c" End of File	*/
