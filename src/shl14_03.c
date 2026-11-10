/* The healing peach of SonSon's assist (character 14's shot 03), set for the other side (pl_no ^ 1)
   so that it meets her own team: it waits for her animation's cue and rides on her; a hit gives
   the work it struck 20 vitality back. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl14_def.h"

/* the effect's own work */
typedef struct SHL14_03_LOCAL_W {
	CHAR_NO	char_no;	/* 000: SonSon's animation when it was set */
} SHL14_03_LOCAL_W;
#define SHL14_03_LW( w )	( ( SHL14_03_LOCAL_W * )&( w )->obj_flag )

static void shl14_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl14_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl14_03_main ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1403 ;
		ew->type = no ;
		ew->master_w = pl_w ;
		SHL14_03_LW( ew )->char_no.w = pl_w->char_no.w ;
		SHL14_03_LW( ew )->char_no.b.grp = pl_w->char_no.b.grp ;
		SHL14_03_LW( ew )->char_no.b.no = pl_w->char_no.b.no ;
	}
	return ew ;
}

static void shl14_03_init( PLAYER_WORK *ew );
static void shl14_03_move( PLAYER_WORK *ew );
static void shl14_03_end( PLAYER_WORK *ew );
static void shl14_03_free( PLAYER_WORK *ew );
static void shl14_03_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_03_main( PLAYER_WORK *ew )
{
	static void ( *shl14_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_03_init, shl14_03_move, shl14_03_end, shl14_03_free,
	} ;

	if( ew->routine_0 > 2 ) shl14_03_debug( ew ) ;
	shl14_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl14_03_init( PLAYER_WORK *ew )
{
	SHL14_03_LOCAL_W *lw = SHL14_03_LW( ew ) ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
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
	ew->size_top = ew->size_bottom = 32 ;
	ew->size_front = ew->size_back = 36 ;
	ew->prio = 11 ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = HIT_ON | HIT_TOUCH | HIT_PL_ONLY ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl14_at_peach ;
	HIT_RESET( ew ) ;
	ew->disp_flag = 0 ;
	pl_char_set( ew, 21, 20 ) ;
	ef_color_set( ew, 2, 2, 2 ) ;
}

static void shl14_03_wait( PLAYER_WORK *ew );
static void shl14_03_hit( PLAYER_WORK *ew );
static void shl14_03_after( PLAYER_WORK *ew );

/*==============================================================*/
/*			Runs while her animation group is the one it was	*/
/*			set in												*/
/*==============================================================*/
static void shl14_03_move( PLAYER_WORK *ew )
{
	static void ( *shl14_03_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_03_wait, shl14_03_hit, shl14_03_after,
	} ;

	shl14_03_move_jmp[ew->routine_1]( ew ) ;
	{
		PLAYER_WORK *pl_w = ew->master_w ;
		SHL14_03_LOCAL_W *lw = SHL14_03_LW( ew ) ;

		if( lw->char_no.b.grp != pl_w->char_no.b.grp ) ew->routine_0 = 2 ;
	}
}

/*==============================================================*/
/*			Waits for her animation to call for it				*/
/*==============================================================*/
static void shl14_03_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->cg_type ){
		ew->routine_1 ++ ;
		pl_w->cg_type = 0 ;
		ew->disp_flag = 1 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
}

/*==============================================================*/
/*			Rides on her; a hit gives the struck work 20		*/
/*			vitality back										*/
/*==============================================================*/
static void shl14_03_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( ew->hit_flag ){
		ew->routine_0 = 2 ;
		vital_add_dmg( ew->em_w, 20 ) ;
		puff_ef_set( ew->em_w, 4 ) ;
		pl_voice_req( ew, 31 ) ;
		return ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	if( pl_w->cg_type ) ew->routine_0 ++ ;
}

/*==============================================================*/
/*			Plays out its animation on her						*/
/*==============================================================*/
static void shl14_03_after( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
}

/*--------------------------------------------------------------*/
static void shl14_03_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pl_w = ew->master_w ;
	pl_color_set( pl_w, 0 ) ;
}

/*--------------------------------------------------------------*/
static void shl14_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl14_03_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl14_03.c" End of File	*/
