/* Ryu's fireball (character 00's shot 00): it flies straight until it hits, is struck or leaves
   the screen. */
#include "player.h"
#include "pl_char_old.h"
#include "pl00_def.h"

/* a fireball's speed, its start from Ryu, its animation and attack, by strength */
typedef struct SHL00_00_DATA {
	Sint16	spd_x, spd_y;
	Sint16	ofs_x, ofs_y;
	Uint8	char_no;
	Uint8	atck_no;
} SHL00_00_DATA;

/* the effect's own work */
typedef struct SHL00_00_LOCAL_W {
	Uint16	char_no;	/* 000: Ryu's animation when it was thrown */
} SHL00_00_LOCAL_W;
#define SHL00_00_LW( w )	( ( SHL00_00_LOCAL_W * )&( w )->obj_flag )

const SHL00_00_DATA shl00_00_tbl[4] = {	/* by the setter's kind and the strength */
	{ -6, 0, -96, 113, 11, 48 },
	{ -9, 0, -96, 113, 12, 48 },
	{ -5, 0, -91, 141, 16, 63 },
	{ -7, 0, -91, 141, 16, 63 }
} ;

static void shl00_00_main( PLAYER_WORK *ew );
static void shl00_00_hit( PLAYER_WORK *ew );
static void shl00_00_out( PLAYER_WORK *ew );
static void shl00_00_free( PLAYER_WORK *ew );
static void shl00_00_work_set( PLAYER_WORK *ew );
static void shl00_00_shot_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, const SHL00_00_DATA *dt );
static void shl00_00_debug( PLAYER_WORK *ew );

/****************************************************************/
void *shl00_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl00_00_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0 ;
		SHL00_00_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl00_00_init( PLAYER_WORK *ew );
static void shl00_00_move( PLAYER_WORK *ew );
static void shl00_00_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl00_00_main( PLAYER_WORK *ew )
{
	static void ( *shl00_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl00_00_init, shl00_00_move, shl00_00_end, shl00_00_free,
	} ;

	shl00_00_debug( ew ) ;
	shl00_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl00_00_init( PLAYER_WORK *ew )
{
	SHL00_00_LOCAL_W *lw = SHL00_00_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	PL00_LOCAL_W *plw ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	plw = ( PL00_LOCAL_W * )&pl_w->tlocal[0] ;
	shl00_00_work_set( ew ) ;
	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( !ew->type ) plw->shot_flag = 1 ;
	shl00_00_shot_set( ew, pl_w, &shl00_00_tbl[ew->atck_level + ew->type * 2] ) ;
	if( pl_w->team_type == 3 ){
		ew->atck_no = pl00_at_vc_hadou ;
		HIT_RESET( ew ) ;
	}
}

static void shl00_00_fly( PLAYER_WORK *ew );
static void shl00_00_vanish( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl00_00_move( PLAYER_WORK *ew )
{
	static void ( *shl00_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl00_00_fly, shl00_00_vanish,
	} ;

	ew->prio = 11 ;
	shl00_00_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl00_00_fly( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ){
		shl00_00_out( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		shl00_00_hit( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		shl00_00_hit( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl00_00_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL00_LOCAL_W *plw = ( PL00_LOCAL_W * )pl_w->tlocal ;

	ew->routine_1 ++ ;
	pl_char_set( ew, 21, 13 ) ;
	if( !ew->type ) plw->shot_flag = 0 ;
}

/*==============================================================*/
static void shl00_00_out( PLAYER_WORK *ew )
{
	PL00_LOCAL_W *plw ;

	ew->routine_0 = 2 ;
	plw = ( PL00_LOCAL_W * )ew->master_w->tlocal ;
	if( !ew->type ) plw->shot_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl00_00_vanish( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl00_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl00_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl00_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The master's drawing, animation and side			*/
/*==============================================================*/
static void shl00_00_work_set( PLAYER_WORK *ew )
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
	ew->size_top = ew->size_bottom = 32 ;
	ew->size_front = ew->size_back = 36 ;
	ew->prio = 11 ;
}

/*==============================================================*/
static void shl00_00_shot_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, const SHL00_00_DATA *dt )
{
	Float spd ;
	Float ofs ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	spd = dt->spd_x ;
	ofs = dt->ofs_x ;
	if( pl_w->disp_dir ){
		ofs = -ofs ;
		spd = -spd ;
	}
	ew->pos.x += ofs ;
	ew->pos.y += dt->ofs_y ;
	ew->spd.x = spd ;
	ew->spd.y = dt->spd_y ;
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = dt->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 21, dt->char_no ) ;
}

/*==============================================================*/
static void shl00_00_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl00_00.c" End of File	*/
