/* Strider's effect 00 (character 05's shot 00): two works that circle him while his timer lasts,
   the second showing the timer gauge; each puts out a shl05_01 while a button is held. */
#include "player.h"
#include "pl_char.h"
#include "pl05_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL05_00_LOCAL_W {
	Float	x;		/* 00: where it is from Strider */
	Float	y;		/* 04 */
	Uint8	dir;		/* 08: Strider's facing when it was set */
	Uint8	shot_timer;		/* 09: frames to the next shl05_01 */
	Uint8	blink_timer;		/* 0a: frames it still flickers */
} SHL05_00_LOCAL_W;
#define SHL05_00_LW( w )	( ( SHL05_00_LOCAL_W * )&( w )->obj_flag )

/* the arcade declares the setter as returning a whole effect work, so the call makes room for one */
typedef struct EF_WORK {
	Uint8	unk[0x1d0];
} EF_WORK;
extern EF_WORK shl05_01_set( PLAYER_WORK *pl_w, Uint8 no );

static SHL05_00_LOCAL_W *lw;	/* the running work's */
static PL05_LOCAL_W *pl_lw;	/* Strider's */

static void shl05_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl05_00_set( PLAYER_WORK *pl_w )
{
	PL05_LOCAL_W *plw = ( PL05_LOCAL_W * )&pl_w->tlocal[0] ;
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_00_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		ew->ef_no = 0x500 ;
	}
	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_00_main ;
		ew->master_w = pl_w ;
		ew->type = 1 ;
		ew->ef_no = 0x500 ;
	}
	plw->option_flag = 1 ;
	return ew ;
}

static void shl05_00_init( PLAYER_WORK *ew );
static void shl05_00_move( PLAYER_WORK *ew );
static void shl05_00_end( PLAYER_WORK *ew );
static void shl05_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_00_main( PLAYER_WORK *ew )
{
	static void ( *shl05_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_00_init, shl05_00_move, shl05_00_end, shl05_00_free,
	} ;

	pl_lw = ( PL05_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = SHL05_00_LW( ew ) ;
	shl05_00_jmp[ew->routine_0]( ew ) ;
}

static void shl05_00_char_set( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_00_init( PLAYER_WORK *ew )
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
	ew->disp_dir = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->type ? 87 : 53 ;
	HIT_RESET( ew ) ;
	lw->dir = ew->master_w->pl_dir ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->timer0 = 240 ;
	if( ew->type ) Timer_Gauge_Set( ew->master_w, &ew->timer0, 6 ) ;
	lw->shot_timer = 0 ;
	ew->size_top = 64 ;
	ew->size_bottom = 32 ;
	ew->size_front = 36 ;
	ew->size_back = 36 ;
	lw->x = ew->type ? DOT_X( 64.0f ) : -DOT_X( 64.0f ) ;
	lw->y = DOT_Y( 48.0f ) ;
	ew->dir = ew->type ? 16 : 0 ;
	ew->timer1 = 2 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	lw->blink_timer = 16 ;
	shl05_00_char_set( ew ) ;
}

static void shl05_00_circle( PLAYER_WORK *ew );
static void shl05_00_fall( PLAYER_WORK *ew );
static void shl05_00_land( PLAYER_WORK *ew );
static void shl05_00_fly( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl05_00_move( PLAYER_WORK *ew )
{
	static void ( *shl05_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_00_circle, shl05_00_fall, shl05_00_land, shl05_00_fly,
	} ;

	if( !ew->master_w->disp_flag ){
		if( ew->type ) ew->timer0 = 0 ;
		shl05_00_end( ew ) ;
		return ;
	}
	shl05_00_move_jmp[ew->routine_1]( ew ) ;
}

static void shl05_00_pos( PLAYER_WORK *ew );

/*==============================================================*/
/*			Circling him										*/
/*==============================================================*/
static void shl05_00_circle( PLAYER_WORK *ew )
{
	if( !ew->master_w->vital_new || !--ew->timer0 ){
		ew->routine_1 ++ ;
		if( ew->type ) ew->timer0 = 0 ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = -DOT_Y( 2.0f ) ;
		ew->acc.y = -0.401785702f ;
		pl_char_set( ew, Char_Effect, 3 ) ;
	} else {
		if( ew->hit_box_no ) ew->hit_box_no = 0 ;
		if( !--ew->timer1 ){
			ew->timer1 = 2 ;
			ew->dir = ( ew->dir + 1 ) & 31 ;
		}
		shl05_00_pos( ew ) ;
		shl05_00_char_set( ew ) ;
		if( ew->hit_stop ){
			if( !--ew->hit_stop ) ew->hit_flag = 0 ;
		}
	}
	if( !lw->shot_timer ){
		if( ew->master_w->routine_1 == PL_NORM ){
			if( ew->master_w->pl_now_lvbt & KEY_PUNCH ){
				shl05_01_set( ew, 0 ) ;
				lw->shot_timer = 22 ;
				lw->shot_timer -- ;
			}
		}
	} else lw->shot_timer -- ;
	if( !lw->blink_timer ){
		ew->disp_flag = 1 ;
		hit_work_set( ew ) ;
	} else {
		lw->blink_timer -- ;
		ew->disp_flag = ( Sys->game_timer0 & 1 ) ? 1 : 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_00_fall( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_00_land( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		ew->acc.x = 0.0f ;
		pl_char_set( ew, Char_Effect, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_00_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ) shl05_00_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_00_end( PLAYER_WORK *ew )
{
	if( !ew->type ) pl_lw->option_flag = 0 ;
	ew->disp_flag = 0 ;
	shl05_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Its frame of the circle by its angle, mirrored		*/
/*			when Strider faces left								*/
/*==============================================================*/
static void shl05_00_char_set( PLAYER_WORK *ew )
{
	static int shl05_00_frame[32] = {
		8, 7, 6, 5, 4, 3, 2, 1, 0, 31, 30, 29, 28, 27, 26, 25,
		24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,
	} ;
	int no = ew->dir ;

	if( lw->dir ){
		no ^= 31 ;
		no &= 31 ;
		no ++ ;
		no &= 31 ;
	}
	pl_char_set2( ew, Char_Effect, 6, shl05_00_frame[no] ) ;
}

/*==============================================================*/
/*			Round Strider: the speed from the angle, added		*/
/*			to where it is from him								*/
/*==============================================================*/
static void shl05_00_pos( PLAYER_WORK *ew )
{
	Float x, y ;

	x = nlCos( ( 40 - ew->dir & 31 ) << 11 ) * 10.0f ;
	x += ew->acc.x ;
	ew->spd.x = x ;
	lw->x += x ;
	y = nlSin( ( 40 - ew->dir & 31 ) << 11 ) * DOT_Y( 6.0f ) ;
	if( lw->dir ) y *= -1.0f ;
	y += ew->acc.y ;
	ew->spd.y = y ;
	lw->y += y ;
	ew->pos.x = ew->master_w->pos.x + lw->x ;
	ew->pos.y = ew->master_w->pos.y + lw->y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl05_00.c" End of File	*/
