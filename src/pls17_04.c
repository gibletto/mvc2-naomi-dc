/* Cable's player effect 04: copies of his sprite that follow him or the effect he made, by type, that grow, spin and fade. */
#include "player.h"
#include "ef_char.h"
#include "pl17_def.h"

/* the effect's own work, from obj_flag: the animation of his that it follows, read through one pointer
   set by the setter */
typedef struct PLS17_04_LOCAL_W {
	Uint16	char_no ;
} PLS17_04_LOCAL_W;
#define PLS17_04_LW( w )	( ( PLS17_04_LOCAL_W * )&( w )->obj_flag )

static Uint16 *pls17_04_char_no_p ;

static void pls17_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls17_04_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls17_04_main ;
		ew->master_w = pl_w ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_no = 0x1700 ;
		pls17_04_char_no_p = &PLS17_04_LW( ew )->char_no ;
		*pls17_04_char_no_p = ew->master_w->char_no.w ;
		ew->sub_no = no ;
		return ew ;
	}
}

/****************************************************************/
PLAYER_WORK *pls17_04_set2( PLAYER_WORK *ef_w, PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls17_04_main ;
		ew->master_w = pl_w ;
		ew->ef_w = ef_w ;
		ew->ef_no = 0x1700 ;
		ew->pl_type = pl_w->pl_type ;
		ew->sub_no = no ;
	}
	return ew ;
}

static void pls17_04_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_main2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_main3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_04_main( PLAYER_WORK *ew )
{
	static void ( *pls17_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_04_main0, pls17_04_main1, pls17_04_main2, pls17_04_main3, pls17_04_main1,
	} ;

	pls17_04_jmp[ew->sub_no]( ew, ew->master_w ) ;
}

static void pls17_04_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_04_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_04_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_04_init0, pls17_04_move0, pls17_04_end, pls17_04_free,
	} ;

	pls17_04_jmp0[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_04_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 1 ;
	ew->disp_flag = 1 ;
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
	ew->prio = 0 ;
	ew->pos = pl_w->pos ;
	ew->pos.x += pl_w->disp_dir ? -60.0f : 60.0f ;
	ef_char_set( ew, 27, 6 ) ;
	ew->alpha = 0.2f ;
	pls17_04_move0( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls17_04_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->cg_type == 3 ) ew->alpha += 0.016f ;
	if( ew->pl_type != pl_w->pl_type ){
		pls17_04_free( ew, pl_w ) ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		shl17_05_set( pl_w ) ;
		ew->routine_0 = 2 ;
	}
}

static void pls17_04_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_grow1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_spin1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_stretch1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_blink1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_04_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_04_jmp1[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_04_init1, pls17_04_grow1, pls17_04_spin1, pls17_04_stretch1, pls17_04_blink1, pls17_04_end, pls17_04_free,
	} ;

	pls17_04_jmp1[ew->routine_0]( ew, pl_w ) ;
	if( ew->pl_type != pl_w->pl_type ) pls17_04_free( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_04_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 1 ;
	ew->disp_flag = 1 ;
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
	ew->prio = 7 ;
	ew->pos = pl_w->pos ;
	if( ew->sub_no == 1 ){
		ew->pos.x += pl_w->disp_dir ? -60.0f : 60.0f ;
	}
	ew->pos.y += 160.0f ;
	ef_char_set( ew, 27, 7 ) ;
	ew->timer0 = 10 ;
	ew->scale.x = 0.0f ;
	ew->scale.y = 0.0f ;
	ew->alpha = 1.0f ;
	pl_voice_req( pl_w, 30 ) ;
	pls17_04_grow1( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls17_04_grow1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->rot[2] += 0x2000 ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 = 2 ;
		ew->timer0 = 24 ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		return ;
	}
	ew->disp_flag = ew->timer0 % 2 ? 1 : 0 ;
	ew->scale.x += 0.1f ;
	ew->scale.y += 0.1f ;
}

/*--------------------------------------------------------------*/
static void pls17_04_spin1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->rot[2] += 0x2000 ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 = 3 ;
		ew->timer0 = 9 ;
		ew->rot[2] = 0 ;
		return ;
	}
	ew->disp_flag = ew->timer0 % 2 ? 1 : 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_04_stretch1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->scale.x -= 0.109000005f ;
	ew->scale.y += 0.436000019f ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 = 4 ;
		ew->timer0 = 10 ;
		return ;
	}
	ew->disp_flag = ew->timer0 % 2 ? 1 : 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_04_blink1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 = 5 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = ew->timer0 % 2 ? 1 : 0 ;
}

static void pls17_04_init2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_04_main2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_04_jmp2[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_04_init2, pls17_04_move2, pls17_04_end, pls17_04_free,
	} ;

	pls17_04_jmp2[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_04_init2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->routine_0 = 1 ;
	ew->disp_flag = 1 ;
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
	ew->prio = ef_w->prio ;
	ew->sub_prio = 1 ;
	ew->pos = ef_w->pos ;
	ef_char_set( ew, 27, 8 ) ;
	ew->alpha = 0.7f ;
	PL17_LW( pl_w )->timeflip_wait_timer[0] = 2 ;
	pls17_04_move2( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls17_04_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->alpha -= 0.00952380896f ;
	if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	PL17_LW( pl_w )->timeflip_wait_timer[0] = 2 ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		ew->alpha = 0.0f ;
	}
	else if( ew->cg_type == 2 ){
		ew->cg_type = 0 ;
		pl_voice_req( ew, 31 ) ;
	}
}

static void pls17_04_init3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls17_04_move3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls17_04_main3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls17_04_jmp3[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls17_04_init3, pls17_04_move3, pls17_04_end, pls17_04_free,
	} ;

	pls17_04_jmp3[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls17_04_init3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;

	ew->routine_0 = 1 ;
	ew->disp_flag = 1 ;
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
	ew->prio = ef_w->prio ;
	ew->sub_prio = 1 ;
	ew->pos = ef_w->pos ;
	ef_char_set( ew, 27, 9 ) ;
	ew->alpha = 0.7f ;
	PL17_LW( pl_w )->timeflip_wait_timer[0] = 2 ;
	pls17_04_move3( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls17_04_move3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->alpha -= 0.00952380896f ;
	if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	PL17_LW( pl_w )->timeflip_wait_timer[0] = 2 ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		ew->alpha = 0.0f ;
	}
}

/*--------------------------------------------------------------*/
static void pls17_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls17_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls17_04.c" End of File	*/
