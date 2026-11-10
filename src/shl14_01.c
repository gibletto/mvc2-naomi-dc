/* SonSon's staff that grows down from above (character 14's shot 01): the head falls with
   twelve parts strung above it, hits as it lands, and the parts fall away after it. */
#include "player.h"
#include "pl_char.h"
#include "pl14_def.h"

/* where each part rides on the head, by its number */
typedef struct SHL14_01_DATA {
	Sint16	x, y;
} SHL14_01_DATA;

/* the effect's own work */
typedef struct SHL14_01_LOCAL_W {
	Uint16	char_no;	/* 000: SonSon's animation when it was set */
	Uint8	unk_02[0x04 - 0x02];
	Float	ground_y;	/* 004: her floor, where the head lands */
} SHL14_01_LOCAL_W;
#define SHL14_01_LW( w )	( ( SHL14_01_LOCAL_W * )&( w )->obj_flag )

/* moves the work by its speed, and its speed by its acceleration */
#define SHL14_01_MOVE( w )	{ ( w )->pos.x += ( w )->spd.x ; ( w )->spd.x += ( w )->acc.x ; \
				  ( w )->pos.y += ( w )->spd.y ; ( w )->spd.y += ( w )->acc.y ; }

#define SHL14_01_PARTS	13	/* the head and twelve parts */

const SHL14_01_DATA shl14_01_tbl[15] = {	/* by the part's number */
	{ 0, 0 }, { 65, 308 }, { 91, 411 }, { 118, 514 }, { 145, 617 },
	{ 171, 720 }, { 198, 822 }, { 225, 925 }, { 251, 1028 }, { 278, 1131 },
	{ 305, 1234 }, { 331, 1337 }, { 358, 1440 }, { 385, 1542 }, { 411, 1645 }
} ;

static void shl14_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl14_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl14_01_main ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1401 ;
		ew->type = no ;
		ew->master_w = pl_w ;
		SHL14_01_LW( ew )->char_no = pl_w->char_no.w ;
		SHL14_01_LW( ew )->ground_y = pl_w->ground_y ;
		ew->timer1 = 0 ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part, set going by the head (hw) as it starts		*/
/*			to fall												*/
/*==============================================================*/
static PLAYER_WORK *shl14_01_part_set( PLAYER_WORK *pl_w, PLAYER_WORK *hw, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ) return ew ;
	else {
		ew->work_func = shl14_01_main ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1401 ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->ef_w = hw ;
		SHL14_01_LW( ew )->char_no = pl_w->char_no.w ;
		SHL14_01_LW( ew )->ground_y = pl_w->ground_y ;
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
		ew->size_top = 48 ;
		ew->size_bottom = 48 ;
		ew->size_front = ew->size_back = 64 ;
		ew->prio = 11 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl14_at_tenchi ;
		HIT_RESET( ew ) ;
		pl_w->atck_no = pl14_at_tenchi ;
		HIT_RESET( pl_w ) ;
		pl_char_set( ew, 22, 4 ) ;
		ew->routine_0 ++ ;
		ew->routine_1 = 1 ;
		ew->disp_flag = 0 ;
		ew->timer1 = 0 ;
	}
	return ew ;
}

static void shl14_01_init( PLAYER_WORK *ew );
static void shl14_01_move( PLAYER_WORK *ew );
static void shl14_01_end( PLAYER_WORK *ew );
static void shl14_01_free( PLAYER_WORK *ew );
static void shl14_01_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_01_main( PLAYER_WORK *ew )
{
	static void ( *shl14_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_01_init, shl14_01_move, shl14_01_end, shl14_01_free,
	} ;

	if( ew->routine_0 < 2 ) shl14_01_debug( ew ) ;
	shl14_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl14_01_init( PLAYER_WORK *ew )
{
	SHL14_01_LOCAL_W *lw = SHL14_01_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	Float x, spd, acc ;

	ew->routine_0 ++ ;
	ew->timer0 = 20 ;
	pl_w = ew->master_w ;
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
	ew->size_top = 64 ;
	ew->size_bottom = 0 ;
	ew->size_front = ew->size_back = 64 ;
	ew->prio = 11 ;
	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	x = 0.0f ;
	spd = 0.0f ;
	acc = 0.0f ;
	if( pl_w->disp_dir ){
		x = -x ;
		acc = -acc ;
		spd = -spd ;
	}
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( 80.0f ) ;
	if( pl_w->pl_stance == STANCE_AIR ) ew->pos.y += DOT_Y( 80.0f ) ;
	ew->spd.x = spd ;
	ew->acc.x = acc ;
	ew->spd.y = DOT_Y( 24.0f ) ;
	ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl14_at_tenchi ;
	HIT_RESET( ew ) ;
	pl_w->atck_no = pl14_at_tenchi ;
	HIT_RESET( pl_w ) ;
	pl_char_set( ew, 22, 2 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
}

static void shl14_01_wait( PLAYER_WORK *ew );
static void shl14_01_fall( PLAYER_WORK *ew );
static void shl14_01_land( PLAYER_WORK *ew );
static void shl14_01_drop( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_01_move( PLAYER_WORK *ew )
{
	static void ( *shl14_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_01_wait, shl14_01_fall, shl14_01_land, shl14_01_drop,
	} ;

	ew->prio = 11 ;
	shl14_01_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			The head waits above, then starts down with its		*/
/*			parts												*/
/*==============================================================*/
static void shl14_01_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL14_LOCAL_W *plw ;
	Float x, spd, acc ;
	Sint8 i ;

	if( --ew->timer0 <= 0 ){
		ew->routine_1 ++ ;
		pl_w = ew->master_w ;
		x = 120.0f ;
		spd = -DOT_X( 10.0f ) ;
		acc = 0.0f ;
		if( pl_w->disp_dir ){
			x = -x ;
			acc = -acc ;
			spd = -spd ;
		}
		ew->pos.x += x ;
		ew->pos.y += 0.0f ;
		ew->spd.x = spd ;
		ew->acc.x = acc ;
		ew->spd.y = 0.0f ;
		ew->acc.y = -DOT_Y( 2.5f ) ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl14_at_tenchi_2 ;
		HIT_RESET( ew ) ;
		pl_w->atck_no = pl14_at_tenchi_2 ;
		HIT_RESET( pl_w ) ;
		pl_char_set( ew, 22, 3 ) ;
		plw = ( PL14_LOCAL_W * )pl_w->tlocal ;
		for( i = 1 ; i < SHL14_01_PARTS ; i ++ ) plw->part_w[i] = 0 ;
		for( i = 1 ; i < SHL14_01_PARTS ; i ++ ){
			if( !( plw->part_w[i] = shl14_01_part_set( pl_w, ew, i ) ) ) return ;
		}
	}
	SHL14_01_MOVE( ew ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	{
		SHL14_01_LOCAL_W *lw = SHL14_01_LW( ew ) ;

		pl_w = ew->master_w ;
		if( lw->char_no != pl_w->char_no.w ) ew->routine_0 = 2 ;
	}
}

/*==============================================================*/
/*			The head falls to the floor, and the parts			*/
/*			follow it											*/
/*==============================================================*/
static void shl14_01_fall( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL14_LOCAL_W *plw ;
	SHL14_01_LOCAL_W *lw ;
	PLAYER_WORK *hw, *w ;
	Float div = 2.0f ;
	Sint8 i ;

	if( !ew->type ){
		if( SHL14_01_LW( ew )->ground_y >= ew->pos.y ){
			ew->pos.y = SHL14_01_LW( ew )->ground_y ;
			ew->routine_1 ++ ;
			ew->timer0 = 10 ;
			scr_work.quake_level = 3 ;
			scr_work.quake_flag = 1 ;
			land_dust_set( &ew->pos, -1 ) ;
			pl_se_req( ew, 49 ) ;
			return ;
		}
		lw = SHL14_01_LW( ew ) ;
		if( lw->char_no != pl_w->char_no.w ){
			ew->routine_1 ++ ;
			ew->timer0 = 1 ;
			return ;
		}
		if( ew->hit_flag ){
			ew->hit_flag = 0 ;
			ew->spd.y /= div ;
			ew->timer1 ++ ;
			ew->dm_flag = ew->atck_flag = 0 ;
			plw = ( PL14_LOCAL_W * )pl_w->tlocal ;
			for( i = 1 ; i < SHL14_01_PARTS ; i ++ ){
				if( ( w = plw->part_w[i] ) ){
					w->atck_flag = 0 ;
					w->dm_flag = 0 ;
				}
			}
		}
		if( ew->hit_stop ){
			pl_w->hit_stop = ew->hit_stop ;
			if( -- ew->hit_stop ) return ;
			if( ew->timer1 == 5 ) return ;
			ew->atck_flag = HIT_ON | HIT_SHOT ;
			ew->dm_flag = HIT_ON | HIT_SHOT ;
			if( ew->timer1 == 4 ){
				ew->atck_no = pl14_at_tenchi_3 ;
				HIT_RESET( ew ) ;
				pl_w->atck_no = pl14_at_tenchi_3 ;
				HIT_RESET( pl_w ) ;
			} else {
				ew->atck_no = pl14_at_tenchi_2 ;
				HIT_RESET( ew ) ;
				pl_w->atck_no = pl14_at_tenchi_2 ;
				HIT_RESET( pl_w ) ;
			}
			plw = ( PL14_LOCAL_W * )pl_w->tlocal ;
			for( i = 1 ; i < SHL14_01_PARTS ; i ++ ){
				if( ( w = plw->part_w[i] ) ){
					w->atck_flag = HIT_ON | HIT_SHOT ;
					w->dm_flag = HIT_ON | HIT_SHOT ;
					if( ew->timer1 == 4 ){
						w->atck_no = pl14_at_tenchi_3 ;
						HIT_RESET( w ) ;
						pl_w->atck_no = pl14_at_tenchi_3 ;
						HIT_RESET( pl_w ) ;
					} else {
						w->atck_no = pl14_at_tenchi_2 ;
						HIT_RESET( w ) ;
						pl_w->atck_no = pl14_at_tenchi_2 ;
						HIT_RESET( pl_w ) ;
					}
				}
			}
		}
		SHL14_01_MOVE( ew ) ;
		if( ew->timer1 <= 4 ) hit_work_set( ew ) ;
		return ;
	}
	hw = ew->ef_w ;
	if( hw->routine_0 > 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( scr_in_y_ck( ew ) ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->pos.y += shl14_01_tbl[ew->type].y ;
	ew->pos.x += pl_w->disp_dir ? -shl14_01_tbl[ew->type].x : shl14_01_tbl[ew->type].x ;
	if( ew->hit_flag ){
		ew->hit_flag = 0 ;
		ew->spd.y /= div ;
		hw->timer1 ++ ;
		hw->dm_flag = hw->atck_flag = 0 ;
		plw = ( PL14_LOCAL_W * )pl_w->tlocal ;
		for( i = 1 ; i < SHL14_01_PARTS ; i ++ ){
			if( ( w = plw->part_w[i] ) ){
				w->dm_flag = w->atck_flag = 0 ;
			}
		}
	}
	if( ew->hit_stop ){
		pl_w->hit_stop = ew->hit_stop ;
		hw->hit_stop = ew->hit_stop ;
		ew->hit_stop = 0 ;
		return ;
	}
	if( hw->hit_stop ) return ;
	if( hw->timer1 > 4 ) return ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Landed: the head stays a moment, then is thrown		*/
/*			up and away											*/
/*==============================================================*/
static void shl14_01_land( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float x, spd, acc ;

	if( --ew->timer0 > 0 ) return ;
	ew->routine_1 ++ ;
	ew->timer0 = 30 ;
	x = 0.0f ;
	spd = 5.0f ;
	acc = 0.0f ;
	pl_w = ew->master_w ;
	if( pl_w->disp_dir ){
		x = -x ;
		acc = -acc ;
		spd = -spd ;
	}
	ew->pos.x += x ;
	ew->pos.y += 0.0f ;
	ew->spd.x = spd ;
	ew->acc.x = acc ;
	ew->spd.y = DOT_Y( 16.0f ) ;
	ew->acc.y = 0.0f ;
}

/*--------------------------------------------------------------*/
static void shl14_01_drop( PLAYER_WORK *ew )
{
	if( --ew->timer0 <= 0 ) ew->routine_0 ++ ;
	SHL14_01_MOVE( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl14_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl14_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl14_01_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl14_01.c" End of File	*/
