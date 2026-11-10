/* Dhalsim's effect 03 (character 25's shot 03): the flame of Yoga Blast, a burst of fire up above
   and in front of him. It follows his position until his animation changes, and tells him when it
   is done. */
#include "player.h"
#include "pl_char.h"
#include "pl25_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL25_03_LOCAL_W {
	Uint16	char_no;	/* 00: his animation when it was made */
	Uint8	unk_02;
	Uint8	unk_03;
} SHL25_03_LOCAL_W;

static PL25_LOCAL_W *shl25_03_pl_lw ;	/* Dhalsim's tlocal, set each frame by the main routine */
static SHL25_03_LOCAL_W *shl25_03_lw ;	/* the effect's own work */

static void shl25_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl25_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl25_03_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2503 ;
	}
	return ew ;
}

static void shl25_03_init( PLAYER_WORK *ew );
static void shl25_03_move( PLAYER_WORK *ew );
static void shl25_03_end( PLAYER_WORK *ew );
static void shl25_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl25_03_main( PLAYER_WORK *ew )
{
	static void ( *shl25_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl25_03_init, shl25_03_move, shl25_03_end, shl25_03_free,
	} ;

	shl25_03_pl_lw = PL25_LW( ew->master_w ) ;
	shl25_03_lw = ( SHL25_03_LOCAL_W * )EF_WK( ew )->work ;
	shl25_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl25_03_init( PLAYER_WORK *ew )
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
	shl25_03_lw->char_no = ew->master_w->char_no.w ;
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += 180.0f ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 38.0f ) ;
	else ew->pos.x -= DOT_X( 38.0f ) ;
	if( ew->atck_level ){
		ew->timer0 = 60 ;
		ew->atck_no = pl25_at_brast_h ;
		HIT_RESET( ew ) ;
	}
	else {
		ew->timer0 = 20 ;
		ew->atck_no = pl25_at_brast_l ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 20, 4 ) ;
	shl25_03_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_03_move( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ){
		shl25_03_end( ew ) ;
		return ;
	}
	if( shl25_03_lw->char_no != ew->master_w->char_no.w ){
		shl25_03_end( ew ) ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += 180.0f ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 38.0f ) ;
	else ew->pos.x -= DOT_X( 38.0f ) ;
	if( ew->routine_2 ){
		if( pl_char_move( ew ) < 0 ){
			shl25_03_pl_lw->ef_end_flag = 1 ;
			shl25_03_end( ew ) ;
		}
	}
	else {
		if( ew->cg_type ){
			ew->cg_type = 0 ;
			if( ew->atck_level ){
				ew->atck_no = pl25_at_brast_h ;
				HIT_RESET( ew ) ;
			}
			else {
				ew->atck_no = pl25_at_brast_l ;
				HIT_RESET( ew ) ;
			}
		}
		if( ew->hit_box_no ){
			ew->routine_2 ++ ;
			pl_char_set( ew, 20, 5 ) ;
			return ;
		}
		else {
			pl_char_move( ew ) ;
			if( --ew->timer0 < 0 ){
				ew->timer0 = 0 ;
				if( ew->cg_type ){
					ew->routine_2 ++ ;
					pl_char_set( ew, 20, 5 ) ;
				}
			}
		}
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl25_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl25_03.c" End of File	*/
