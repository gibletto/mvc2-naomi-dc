/* War Machine's player effect 00, eight kinds by type: the thing he leaves in his entrance, which
   hops and flies off (0), and its piece on the ground (1); the flash where the crouching heavy
   punch's beam starts (2); the picture of his flight (3) and of his air dash (4); the War
   Destroyer's column of ten (5); the air Smart Bomb's two (6); and the flight's piece on the ground
   (7). */
#include "player.h"
#include "pl_char.h"
#include "pl2e_def.h"

/* the effect's own work: its state in the speed's place, and the animation it was made in (obj_flag) */
typedef struct PLS2E_00_W {
	Uint8	anim_no;
	Uint8	unk_01;
} PLS2E_00_W;
#define PLS2E_00_LW( w )	( ( PLS2E_00_W * )&( w )->spd )

typedef struct PLS2E_00_CC {
	Uint16	char_no;
} PLS2E_00_CC;
#define PLS2E_00_CC( w )	( ( PLS2E_00_CC * )&( w )->obj_flag )

/* what every kind takes from his work when it starts */
#define PLS2E_00_COPY( ew, pl_w ) \
	CHAR_PART_COPY( ew, pl_w ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( pl_w )->pl_no ; \
	( ew )->pl_type = ( pl_w )->pl_type ; \
	( ew )->scale.x = ( pl_w )->scale.x ; \
	( ew )->scale.y = ( pl_w )->scale.y ; \
	( ew )->atck_level = ( pl_w )->atck_level ; \
	( ew )->work_no = ( pl_w )->work_no ; \
	( ew )->col_cnt = ( pl_w )->col_cnt ; \
	( ew )->scale = ( pl_w )->scale ; \
	( ew )->prio = ( pl_w )->prio

static void pls2e_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2e_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	PLS2E_00_CC *cc ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2e_00_main ;
		ew->ef_no = 0x2e00 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
		cc = PLS2E_00_CC( ew ) ;
		cc->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void pls2e_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_appear_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_air_dash( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_destroyer( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_bomb( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_fly_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_main( PLAYER_WORK *ew )
{
	static void ( *pls2e_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_appear, pls2e_00_appear_ground, pls2e_00_flash, pls2e_00_fly,
		pls2e_00_air_dash, pls2e_00_destroyer, pls2e_00_bomb, pls2e_00_fly_ground,
	} ;

	pls2e_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls2e_00_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
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
	pls2e_00_appear_move( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2e_00_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 18, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls2e_00_appear_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 16 ;
		ew->spd.y = 2.142857f ;
		pl_char_set( ew, 18, 6 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls2e_00_appear_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void pls2e_00_appear_up_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;

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
static void pls2e_00_appear_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !-- ew->timer0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_appear_step_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_appear_wait, pls2e_00_appear_start, pls2e_00_appear_up, pls2e_00_appear_up_next, pls2e_00_appear_fly,
	} ;

	ew->prio = pl_w->prio ;
	pls2e_00_appear_step_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void pls2e_00_hide( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2e_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_appear_init, pls2e_00_appear_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_appear_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls2e_00_appear_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_appear_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs ;

	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 2 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ofs = DOT_X( 20.0f ) ;
	if( !ew->disp_dir ) ew->pos.x += ofs ;
	else ew->pos.x -= ofs ;
	pl_char_set( ew, 18, 10 ) ;
	ew->timer0 = 60 ;
	pls2e_00_appear_ground_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_appear_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	if( lw->land_flag ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls2e_00_appear_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_appear_ground_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_appear_ground_init, pls2e_00_appear_ground_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_appear_ground_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls2e_00_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_flash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	pls2e_00_flash_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 pls2e_00_ofs_tbl[] = {
		-23, 89, -37, 98, -50, 57,
	} ;
	PL2E_LOCAL_W *lw = ( PL2E_LOCAL_W * )pl_w->tlocal ;
	PLS2E_00_CC *cc = PLS2E_00_CC( ew ) ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.w != cc->char_no ){
		ew->routine_0 ++ ;
		pls2e_00_hide( ew, pl_w ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x += DOT_X( pls2e_00_ofs_tbl[ew->sub_type * 2] ) ;
	else ew->pos.x -= DOT_X( pls2e_00_ofs_tbl[ew->sub_type * 2] ) ;
	ew->pos.y += DOT_Y( pls2e_00_ofs_tbl[ew->sub_type * 2 + 1] ) ;
	if( lw->beam_end_flag ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls2e_00_flash( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_flash_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_flash_init, pls2e_00_flash_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_flash_jmp[ew->routine_0]( ew, pl_w ) ;
}

static Uint8 pls2e_00_no_tbl[8] = {
	15, 15, 16, 17, 18, 15, 15, 15,
} ;

static void pls2e_00_fly_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_fly_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2E_00_W *lw = PLS2E_00_LW( ew ) ;

	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 4 ;
	lw->anim_no = 0 ;
	lw->unk_01 = pl_w->char_no.b.no ;
	pl_char_set( ew, Char_Effect, 15 ) ;
	pls2e_00_fly_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_fly_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2E_00_W *lw = PLS2E_00_LW( ew ) ;
	PLS2E_00_CC *cc = PLS2E_00_CC( ew ) ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !pl_w->fly_flag ){
		ew->routine_0 ++ ;
		pls2e_00_hide( ew, pl_w ) ;
		return ;
	}
	if( pl_w->char_no.b.no == 17 ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
	if( !ew->routine_1 ){
		if( pl_w->char_no.b.grp != Char_Fly ){
			lw->anim_no = 1 ;
			pl_char_set( ew, Char_Effect, pls2e_00_no_tbl[lw->anim_no] ) ;
			ew->routine_1 ++ ;
			return ;
		}
	}
	else {
		if( pl_w->char_no.b.grp == Char_Fly ) ew->routine_1 = 0 ;
		return ;
	}
	if( pl_w->cg_type ){
		if( pl_w->cg_type != lw->anim_no ){
			lw->anim_no = pl_w->cg_type ;
			pl_char_set( ew, Char_Effect, pls2e_00_no_tbl[lw->anim_no] ) ;
		}
		if( pl_w->char_no.w != cc->char_no ) ew->disp_dir = pl_w->disp_dir ;
	}
}

/*==============================================================*/
static void pls2e_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_fly_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_fly_init, pls2e_00_fly_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_fly_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls2e_00_air_dash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_air_dash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->sub_type = pl_w->char_no.b.grp ;
	pl_char_set( ew, Char_Effect, pls2e_00_no_tbl[pl_w->cg_type] ) ;
	pls2e_00_air_dash_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_air_dash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( pl_w->char_no.b.grp != ew->sub_type ){
		ew->routine_0 ++ ;
		pls2e_00_hide( ew, pl_w ) ;
	}
}

/*==============================================================*/
static void pls2e_00_air_dash( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_air_dash_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_air_dash_init, pls2e_00_air_dash_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_air_dash_jmp[ew->routine_0]( ew, pl_w ) ;
}

static Sint16 pls2e_00_y_tbl[10] = {
	0, 75, 139, 203, 267, 331, 395, 459, 523, 587,
} ;

static void pls2e_00_destroyer_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_destroyer_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 2 ;
	ew->size_top = ew->size_bottom = 68 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->pos.y += DOT_Y( pls2e_00_y_tbl[ew->sub_type] ) ;
	if( !ew->sub_type ) pl_char_set( ew, Char_Effect, 36 ) ;
	else pl_char_set( ew, Char_Effect, 37 ) ;
	pls2e_00_destroyer_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_destroyer_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	pl_char_move( ew ) ;
	if( !scr_in_y_ck( ew ) ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
	if( pl_w->char_no.b.grp == Char_Super ) return ;
	ew->routine_0 ++ ;
	pls2e_00_hide( ew, pl_w ) ;
}

/*==============================================================*/
static void pls2e_00_destroyer( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_destroyer_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_destroyer_init, pls2e_00_destroyer_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_destroyer_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls2e_00_bomb_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_bomb_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = !ew->sub_type ? -DOT_X( 47.0f ) : DOT_X( 37.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, Char_Effect, 49 ) ;
	pls2e_00_bomb_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_bomb_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2E_00_CC *cc = PLS2E_00_CC( ew ) ;

	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.x += ew->spd.x ;
	if( pl_w->char_no.w != cc->char_no ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls2e_00_bomb( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_bomb_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_bomb_init, pls2e_00_bomb_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_bomb_jmp[ew->routine_0]( ew, pl_w ) ;
}

static void pls2e_00_fly_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2e_00_fly_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	PLS2E_00_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.y = pl_w->ground_y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, 18, 10 ) ;
	pls2e_00_fly_ground_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_fly_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	if( !pl_w->fly_flag ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls2e_00_fly_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls2e_00_fly_ground_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2e_00_fly_ground_init, pls2e_00_fly_ground_move, pls2e_00_hide, pls2e_00_free,
	} ;

	pls2e_00_fly_ground_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_hide( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls2e_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2e_00.c" End of File	*/
