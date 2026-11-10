/* Akuma's effect 00 (character 1e's shot 00): his two fireballs, picked by the setter. Kind 0 is
   the Gou Hadouken, which flies on its speed; kind 1 is the Zankuu Hadouken, which falls to the
   ground; each stops where it hits and plays its burst. */
#include "player.h"
#include "pl_char_old.h"
#include "pl1e_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL1E_00_LOCAL_W {
	Uint16	char_no;	/* 000: Akuma's animation when it was set; the effect ends when it changes */
} SHL1E_00_LOCAL_W;
#define SHL1E_00_LW( w )	( ( SHL1E_00_LOCAL_W * )&( w )->obj_flag )

/* one kind's start: its speed and its place from Akuma (both turned by his side), its animation
   and its attack */
typedef struct SHL1E_00_DATA {
	Sint16	spd_x;
	Sint16	spd_y;
	Sint16	x;
	Sint16	y;
	Uint8	char_no;
	Uint8	atck_no;
} SHL1E_00_DATA;

static void shl1e_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1e_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1e_00_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1e00 ;
		SHL1E_00_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl1e_00_dummy( PLAYER_WORK *ew );
static void shl1e_00_hadou_init( PLAYER_WORK *ew );
static void shl1e_00_hadou_move( PLAYER_WORK *ew );
static void shl1e_00_zankuu_init( PLAYER_WORK *ew );
static void shl1e_00_zankuu_move( PLAYER_WORK *ew );
static void shl1e_00_end( PLAYER_WORK *ew );
static void shl1e_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1e_00_main( PLAYER_WORK *ew )
{
	static void ( *shl1e_00_hadou_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_00_hadou_init, shl1e_00_hadou_move, shl1e_00_end, shl1e_00_free,
	} ;
	static void ( *shl1e_00_zankuu_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_00_zankuu_init, shl1e_00_zankuu_move, shl1e_00_end, shl1e_00_free,
	} ;

	shl1e_00_dummy( ew ) ;
	if( !ew->type ) shl1e_00_hadou_jmp[ew->routine_0]( ew ) ;
	else shl1e_00_zankuu_jmp[ew->routine_0]( ew ) ;
}

static void shl1e_00_part_set( PLAYER_WORK *ew );
static void shl1e_00_hit_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, const SHL1E_00_DATA *dp );

static const SHL1E_00_DATA shl1e_00_data[4] = {
	{ -6, 0, -133, 113, 11, 48 },
	{ -8, 0, -133, 113, 12, 49 },
	{ -6, -4, -86, 102, 16, 64 },
	{ -10, -6, -86, 102, 17, 64 },
} ;

/*==============================================================*/
static void shl1e_00_hadou_init( PLAYER_WORK *ew )
{
	SHL1E_00_LOCAL_W *lw = SHL1E_00_LW( ew ) ;
	PL1E_LOCAL_W *plw ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	plw = ( PL1E_LOCAL_W * )&pl_w->tlocal[0] ;
	shl1e_00_part_set( ew ) ;
	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( !ew->type ) plw->shot_flag = 1 ;
	shl1e_00_hit_set( ew, pl_w, &shl1e_00_data[ew->atck_level + ew->type * 2] ) ;
}

static void shl1e_00_hadou_fly( PLAYER_WORK *ew );
static void shl1e_00_hadou_burst( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl1e_00_hadou_move( PLAYER_WORK *ew )
{
	static void ( *shl1e_00_hadou_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_00_hadou_fly, shl1e_00_hadou_burst,
	} ;

	ew->prio = 11 ;
	shl1e_00_hadou_move_jmp[ew->routine_1]( ew ) ;
}

static void shl1e_00_hadou_gone( PLAYER_WORK *ew );
static void shl1e_00_hadou_hit( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1e_00_hadou_fly( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ){
		shl1e_00_hadou_gone( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl1e_00_hadou_hit( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl1e_00_hadou_hit( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl1e_00_hadou_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1E_LOCAL_W *lw = PL1E_LW( pl_w ) ;

	ew->routine_1 ++ ;
	pl_char_set( ew, 21, 13 ) ;
	if( ew->pl_type == pl_w->pl_type ) lw->shot_flag = 0 ;
}

/*==============================================================*/
static void shl1e_00_hadou_gone( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL1E_LOCAL_W *lw ;

	ew->routine_0 = 2 ;
	pl_w = ew->master_w ;
	lw = PL1E_LW( pl_w ) ;
	if( ew->pl_type == pl_w->pl_type ) lw->shot_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1e_00_hadou_burst( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl1e_00_zankuu_init( PLAYER_WORK *ew )
{
	SHL1E_00_LOCAL_W *lw = SHL1E_00_LW( ew ) ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	shl1e_00_part_set( ew ) ;
	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	shl1e_00_hit_set( ew, pl_w, &shl1e_00_data[ew->atck_level + ew->type * 2] ) ;
}

static void shl1e_00_zankuu_fly( PLAYER_WORK *ew );
static void shl1e_00_zankuu_burst( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl1e_00_zankuu_move( PLAYER_WORK *ew )
{
	static void ( *shl1e_00_zankuu_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_00_zankuu_fly, shl1e_00_zankuu_burst,
	} ;

	ew->prio = 11 ;
	shl1e_00_zankuu_move_jmp[ew->routine_1]( ew ) ;
}

static void shl1e_00_zankuu_gone( PLAYER_WORK *ew );
static void shl1e_00_zankuu_hit( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1e_00_zankuu_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !scr_in_x_ck( ew ) ){
		shl1e_00_zankuu_gone( ew ) ;
		return ;
	}
	if( ew->pos.y <= pl_w->ground_y ){
		ew->pos.y = pl_w->ground_y ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl1e_00_zankuu_hit( ew ) ;
	}
	if( ew->hit_flag ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl1e_00_zankuu_hit( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl1e_00_zankuu_hit( ew ) ;
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
static void shl1e_00_zankuu_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1E_LOCAL_W *lw = PL1E_LW( pl_w ) ;

	ew->routine_1 ++ ;
	pl_char_set( ew, 21, 18 ) ;
	if( ew->pl_type == pl_w->pl_type ) lw->shot_flag = 0 ;
}

/*==============================================================*/
static void shl1e_00_zankuu_gone( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL1E_LOCAL_W *lw ;

	ew->routine_0 = 2 ;
	pl_w = ew->master_w ;
	lw = PL1E_LW( pl_w ) ;
	if( ew->pl_type == pl_w->pl_type ) lw->shot_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1e_00_zankuu_burst( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1e_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl1e_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1e_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1e_00_part_set( PLAYER_WORK *ew )
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
static void shl1e_00_hit_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, const SHL1E_00_DATA *dp )
{
	Float x ;
	Float spd_x ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	spd_x = dp->spd_x ;
	x = dp->x ;
	if( pl_w->disp_dir ){
		x = -x ;
		spd_x = -spd_x ;
	}
	ew->pos.x += x ;
	ew->pos.y += dp->y ;
	ew->spd.x = spd_x ;
	ew->spd.y = dp->spd_y ;
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = dp->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 21, dp->char_no ) ;
}

/*==============================================================*/
static void shl1e_00_dummy( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1e_00.c" End of File	*/
