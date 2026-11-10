/* Rogue's effect 02 (her pls0a): the trail she leaves in Power Drain and the Dive Punch, while her
   glow_timer runs. A hidden work (type 0) puts out a copy of her in another colour every four frames
   (type 1), which stays 23 frames and blinks out. */
#include "player.h"
#include "ef_char.h"
#include "pl0a_def.h"

/* the effect's own work, from obj_flag */
typedef struct PLS0A_02_LOCAL_W {
	Uint32	unk_00;		/* 000 */
	PLAYER_WORK	*w;	/* 004: the player it rides on */
} PLS0A_02_LOCAL_W;
#define PLS0A_02_LW( w )	( ( PLS0A_02_LOCAL_W * )&( w )->obj_flag )

static void pls0a_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0a_02_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->be_flag = 1 ;
		ew->work_func = pls0a_02_main ;
		ew->master_w = pl_w ;
		PLS0A_02_LW( ew )->w = pl_w ;
		ew->type = 0 ;
		ew->sub_type = no ;
		ew->ef_no = 0xa02 ;
		ew->pl_type = pl_w->pl_type ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls0a_02_set2( PLAYER_WORK *pl_w, PLAYER_WORK *w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pl_w, W_PLS, LIST_BEHIND ) ) ){
		ew->be_flag = 1 ;
		ew->work_func = pls0a_02_main ;
		ew->master_w = pl_w ;
		PLS0A_02_LW( ew )->w = w ;
		ew->type = 1 ;
		ew->ef_no = 0xa02 ;
		ew->pl_type = w->pl_type ;
	}
	return ew ;
}

static void pls0a_02_init( PLAYER_WORK *ew, PLAYER_WORK *w ) ;
static void pls0a_02_move( PLAYER_WORK *ew, PLAYER_WORK *w ) ;
static void pls0a_02_end( PLAYER_WORK *ew, PLAYER_WORK *w ) ;
static void pls0a_02_free( PLAYER_WORK *ew, PLAYER_WORK *w ) ;

/*==============================================================*/
static void pls0a_02_main( PLAYER_WORK *ew )
{
	static void ( *pls0a_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *w ) = {
		pls0a_02_init, pls0a_02_move, pls0a_02_end, pls0a_02_free,
	} ;

	pls0a_02_jmp[ew->routine_0]( ew, PLS0A_02_LW( ew )->w ) ;
}

/*==============================================================*/
static void pls0a_02_init( PLAYER_WORK *ew, PLAYER_WORK *w )
{
	ew->routine_0 ++ ;
	if( !ew->type ){
		ew->disp_flag = 0 ;
		ew->timer0 = 0 ;
	} else {
		CHAR_PART_COPY( ew, w ) ;
		ew->disp_flag = 1 ;
		ew->pl_no = w->pl_no ;
		ew->pl_type = w->pl_type ;
		ew->scale.x = w->scale.x ;
		ew->scale.y = w->scale.y ;
		ew->atck_level = w->atck_level ;
		ew->work_no = w->work_no ;
		ew->col_cnt = w->col_cnt ;
		ew->scale = w->scale ;
		ew->prio = w->prio ;
		ew->disp_flag = 1 ;
		ew->sub_prio = 1 ;
		ew->col_no += 4 ;
		ew->pos = w->pos ;
		ew->timer1 = 23 ;
		if( HI_BYTE( w->cg_rival ) >= 32 ) ef_char_set( ew, 28, HI_BYTE( w->cg_rival ) & 15 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls0a_02_move( PLAYER_WORK *ew, PLAYER_WORK *w )
{
	if( ew->master_w->be_flag == 0 || w->routine_1 ){
		pls0a_02_free( ew, w ) ;
		return ;
	}
	ew->prio = w->prio ;
	if( !ew->type ){
		if( !w->hit_stop ){
			if( ew->sub_type && w->pl_stance != STANCE_AIR ){
				ew->sub_type = 0 ;
				ew->timer0 ++ ;
			}
			if( !( ew->timer0 += 0x4000 ) ) pls0a_02_set2( ew, w ) ;
		}
	} else {
		if( !w->hit_stop ){
			if( -- ew->timer1 == 0 ){
				pls0a_02_free( ew, w ) ;
				return ;
			}
		}
		if( ew->timer1 < 16 ) ew->col_no = w->col_no + 5 ;
		if( ew->timer1 < 6 ) ew->disp_flag ^= 1 ;
	}
	if( PL0A_LW( w )->glow_timer > 0 ) return ;
	pls0a_02_free( ew, w ) ;
}

/*--------------------------------------------------------------*/
static void pls0a_02_end( PLAYER_WORK *ew, PLAYER_WORK *w )
{
}

/*--------------------------------------------------------------*/
static void pls0a_02_free( PLAYER_WORK *ew, PLAYER_WORK *w )
{
	ew->be_flag = 0 ;
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0a_02.c" End of File	*/
