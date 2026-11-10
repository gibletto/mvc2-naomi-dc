/* Dhalsim's effect 01 (character 25's shot 01): the yoga flame, a burst of fire at his mouth.
   It follows his position until his animation changes, and tells him when it is done. */
#include "player.h"
#include "pl_char.h"
#include "pl25_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL25_01_LOCAL_W {
	Uint16	char_no;	/* 00: his animation when it was made */
	Uint8	unk_02;
	Uint8	unk_03;
} SHL25_01_LOCAL_W;

static PL25_LOCAL_W *shl25_01_pl_lw ;	/* Dhalsim's tlocal, set each frame by the main routine */
static SHL25_01_LOCAL_W *shl25_01_lw ;	/* the effect's own work */

static void shl25_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl25_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl25_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2501 ;
	}
	return ew ;
}

static void shl25_01_init( PLAYER_WORK *ew );
static void shl25_01_move( PLAYER_WORK *ew );
static void shl25_01_end( PLAYER_WORK *ew );
static void shl25_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl25_01_main( PLAYER_WORK *ew )
{
	static void ( *shl25_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl25_01_init, shl25_01_move, shl25_01_end, shl25_01_free,
	} ;

	shl25_01_pl_lw = PL25_LW( ew->master_w ) ;
	shl25_01_lw = ( SHL25_01_LOCAL_W * )EF_WK( ew )->work ;
	shl25_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl25_01_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	shl25_01_lw->char_no = ew->master_w->char_no.w ;
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 54.0f ) ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 70.0f ) ;
	else ew->pos.x -= DOT_X( 70.0f ) ;
	if( ew->atck_level ){
		ew->timer0 = 60 ;
		ew->atck_no = pl25_at_frei_h ;
		HIT_RESET( ew ) ;
	}
	else {
		ew->timer0 = 20 ;
		ew->atck_no = pl25_at_frei_l ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 20, 2 ) ;
	shl25_01_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_01_move( PLAYER_WORK *ew )
{
	if( scr_in_x_ck( ew ) == 0 ){
		shl25_01_end( ew ) ;
		return ;
	}
	if( shl25_01_lw->char_no != ew->master_w->char_no.w ){
		shl25_01_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 54.0f ) ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 70.0f ) ;
	else ew->pos.x -= DOT_X( 70.0f ) ;
	if( ew->routine_2 ){
		if( pl_char_move( ew ) < 0 ){
			shl25_01_pl_lw->ef_end_flag = 1 ;
			shl25_01_end( ew ) ;
		}
	}
	else {
		if( ew->cg_type ){
			ew->cg_type = 0 ;
			if( ew->atck_level ){
				ew->atck_no = pl25_at_frei_h ;
				HIT_RESET( ew ) ;
			}
			else {
				ew->atck_no = pl25_at_frei_l ;
				HIT_RESET( ew ) ;
			}
		}
		if( ew->hit_box_no ){
			ew->routine_2 ++ ;
			pl_char_set( ew, 20, 3 ) ;
			return ;
		}
		else {
			pl_char_move( ew ) ;
			if( --ew->timer0 < 0 ){
				ew->timer0 = 0 ;
				if( ew->cg_type ){
					ew->routine_2 ++ ;
					pl_char_set( ew, 20, 3 ) ;
				}
			}
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl25_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl25_01.c" End of File	*/
