/* Cable's effect 02 (character 17's shot 02): the shot of Psimitar, fired up and forward at a steep
   angle until it leaves the stage's top. */
#include "player.h"
#include "pl_char.h"
#include "pl17_def.h"

/* the shot's own work */
typedef struct SHL17_02_LOCAL_W {
	Uint16	char_no;	/* 000: Cable's animation when it was fired */
} SHL17_02_LOCAL_W;
#define SHL17_02_LW( w )	( ( SHL17_02_LOCAL_W * )&( w )->obj_flag )

static SHL17_02_LOCAL_W *lw ;	/* the last shot's */

static void shl17_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl17_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl17_02_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1702 ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->routine_3 = ew->master_w->atck_level ;
		lw = SHL17_02_LW( ew ) ;
		lw->char_no = ew->master_w->char_no.w ;
		return ew ;
	}
}

static void shl17_02_init( PLAYER_WORK *ew );
static void shl17_02_move( PLAYER_WORK *ew );
static void shl17_02_end( PLAYER_WORK *ew );
static void shl17_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl17_02_main( register PLAYER_WORK *ew )
{
	static void ( *shl17_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl17_02_init, shl17_02_move, shl17_02_end, shl17_02_free,
	} ;

	shl17_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl17_02_init( PLAYER_WORK *ew )
{
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
	pl_char_set( ew, Char_Effect, 10 ) ;
	ew->prio = 0 ;
	ew->pos = ew->master_w->pos ;
	ew->pos.x += ew->disp_dir ? 26.0f : -26.0f ;
	ew->pos.y += 90.0f ;
	ew->spd.x = ew->disp_dir ? nlCos( nlArcTan( 3.0f ) ) * 20.0f : -( nlCos( nlArcTan( 3.0f ) ) * 20.0f ) ;
	ew->spd.y = nlSin( nlArcTan( 3.0f ) ) * 20.0f ;
	ew->timer0 = 100 ;
	ew->timer1 = ew->master_w->atck_level ? 7 : 4 ;
	ew->routine_0 = 1 ;
	pl_voice_req( ew, 33 ) ;
	shl17_02_move( ew ) ;
}

static Uint8 shl17_02_top_ck( PLAYER_WORK *ew );

/*==============================================================*/
/*			It hits on each frame its animation marks, seven	*/
/*			times at the higher strength and four at the		*/
/*			lower												*/
/*==============================================================*/
static void shl17_02_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pl_type != ew->master_w->pl_type ){
		ew->routine_0 = 3 ;
		shl17_02_free( ew ) ;
		return ;
	}
	if( -- ew->timer0 < 0 || ew->hit_box_no || shl17_02_top_ck( ew ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type && ew->timer1 ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		if( ew->routine_3 ){
			ew->atck_no = pl17_at_psimitar_shot_h ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl17_at_psimitar_shot_l ;
			HIT_RESET( ew ) ;
		}
		ew->timer1 -- ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Whether the shot has reached the stage's top		*/
/*==============================================================*/
static Uint8 shl17_02_top_ck( PLAYER_WORK *ew )
{
	if( ew->pos.y >= scr_work.stage_top ) return 1 ;
	return 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl17_02.c" End of File	*/
