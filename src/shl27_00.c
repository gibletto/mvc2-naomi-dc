/* Ken's hadouken (character 27's shot 00): type 0 flies along the ground until it hits, is struck
   or slows to a stop; type 1 is the one thrown in the air, which comes to a stop when it lands. */
#include "player.h"
#include "pl_char_old.h"
#include "pl27_def.h"

/* a fireball's speed, its start from Ken, its animation and attack, by strength */
typedef struct SHL27_00_DATA {
	Sint16	spd_x, spd_y;
	Sint16	ofs_x, ofs_y;
	Uint8	char_no;
	Uint8	atck_no;
} SHL27_00_DATA;

/* the effect's own work */
typedef struct SHL27_00_LOCAL_W {
	Uint16	char_no;	/* 000: Ken's animation when it was thrown */
} SHL27_00_LOCAL_W;
#define SHL27_00_LW( w )	( ( SHL27_00_LOCAL_W * )&( w )->obj_flag )

const Uint16 unk_0C22F770[4] = { 4, 16, 4, 16 } ;	/* never used */
const SHL27_00_DATA shl27_00_tbl[4] = {	/* by the strength and type */
	{ -5, 0, -133, 113, 11, 48 },
	{ -7, 0, -133, 113, 12, 49 },
	{ -6, -4, -86, 102, 16, 64 },
	{ -10, -6, -86, 102, 17, 64 }
} ;

static void shl27_00_main( PLAYER_WORK *ew );
static void shl27_00_end( PLAYER_WORK *ew );
static void shl27_00_free( PLAYER_WORK *ew );
static void shl27_00_work_set( PLAYER_WORK *ew );
static void shl27_00_shot_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, const SHL27_00_DATA *dt );
static void shl27_00_debug( PLAYER_WORK *ew );

/****************************************************************/
void *shl27_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl27_00_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x2700 ;
		SHL27_00_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl27_00_init( PLAYER_WORK *ew );
static void shl27_00_move( PLAYER_WORK *ew );
static void shl27_00_air_init( PLAYER_WORK *ew );
static void shl27_00_air_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl27_00_main( PLAYER_WORK *ew )
{
	static void ( *shl27_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl27_00_init, shl27_00_move, shl27_00_end, shl27_00_free,
	} ;
	static void ( *shl27_00_air_jmp[] )( PLAYER_WORK *ew ) = {
		shl27_00_air_init, shl27_00_air_move, shl27_00_end, shl27_00_free,
	} ;

	shl27_00_debug( ew ) ;
	if( !ew->type ){
		shl27_00_jmp[ew->routine_0]( ew ) ;
	} else {
		shl27_00_air_jmp[ew->routine_0]( ew ) ;
	}
}

/*==============================================================*/
static void shl27_00_init( PLAYER_WORK *ew )
{
	SHL27_00_LOCAL_W *lw = SHL27_00_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	PL27_LOCAL_W *plw ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	plw = ( PL27_LOCAL_W * )&pl_w->tlocal[0] ;
	shl27_00_work_set( ew ) ;
	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( !ew->type ) plw->shot_flag = 1 ;
	shl27_00_shot_set( ew, pl_w, &shl27_00_tbl[ew->atck_level + ew->type * 2] ) ;
	ew->timer1 = 32 ;
}

static void shl27_00_fly( PLAYER_WORK *ew );
static void shl27_00_vanish( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl27_00_move( PLAYER_WORK *ew )
{
	static void ( *shl27_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl27_00_fly, shl27_00_vanish,
	} ;

	ew->prio = 11 ;
	shl27_00_move_jmp[ew->routine_1]( ew ) ;
}

static void shl27_00_hit( PLAYER_WORK *ew );
static void shl27_00_out( PLAYER_WORK *ew );

/*==============================================================*/
static void shl27_00_fly( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ){
		shl27_00_out( ew ) ;
		return ;
	}
	if( ew->hit_flag ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl27_00_hit( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl27_00_hit( ew ) ;
		return ;
	}
	if( -- ew->timer1 <= 0 ){
		ew->spd.x /= 2 ;
		ew->spd.y /= 2 ;
		ew->acc.x /= 2 ;
		ew->acc.y /= 2 ;
		shl27_00_hit( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl27_00_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL27_LOCAL_W *plw = ( PL27_LOCAL_W * )pl_w->tlocal ;

	ew->routine_1 ++ ;
	pl_char_set( ew, 21, 13 ) ;
	if( ew->pl_type == pl_w->pl_type ) plw->shot_flag = 0 ;
}

/*==============================================================*/
static void shl27_00_out( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL27_LOCAL_W *plw ;

	ew->routine_0 = 2 ;
	pl_w = ew->master_w ;
	plw = ( PL27_LOCAL_W * )pl_w->tlocal ;
	if( ew->pl_type == pl_w->pl_type ) plw->shot_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl27_00_vanish( PLAYER_WORK *ew )
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
static void shl27_00_air_init( PLAYER_WORK *ew )
{
	SHL27_00_LOCAL_W *lw = SHL27_00_LW( ew ) ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
	shl27_00_work_set( ew ) ;
	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	shl27_00_shot_set( ew, pl_w, &shl27_00_tbl[ew->atck_level + ew->type * 2] ) ;
	ew->timer1 = 24 ;
}

static void shl27_00_air_fly( PLAYER_WORK *ew );
static void shl27_00_air_vanish( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl27_00_air_move( PLAYER_WORK *ew )
{
	static void ( *shl27_00_air_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl27_00_air_fly, shl27_00_air_vanish,
	} ;

	ew->prio = 11 ;
	shl27_00_air_move_jmp[ew->routine_1]( ew ) ;
}

static void shl27_00_air_hit( PLAYER_WORK *ew );
static void shl27_00_air_out( PLAYER_WORK *ew );

/*==============================================================*/
static void shl27_00_air_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !scr_in_x_ck( ew ) ){
		shl27_00_air_out( ew ) ;
		return ;
	}
	if( -- ew->timer1 <= 0 ){
		ew->spd.x /= 2 ;
		ew->spd.y /= 2 ;
		ew->acc.x /= 2 ;
		ew->acc.y /= 2 ;
		shl27_00_air_hit( ew ) ;
		return ;
	}
	if( ew->pos.y <= pl_w->ground_y ){
		ew->pos.y = pl_w->ground_y ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl27_00_air_hit( ew ) ;
	}
	if( ew->hit_flag ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl27_00_air_hit( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		shl27_00_air_hit( ew ) ;
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
static void shl27_00_air_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL27_LOCAL_W *plw = ( PL27_LOCAL_W * )pl_w->tlocal ;

	ew->routine_1 ++ ;
	pl_char_set( ew, 21, 18 ) ;
	if( ew->pl_type == pl_w->pl_type ) plw->shot_flag = 0 ;
}

/*==============================================================*/
static void shl27_00_air_out( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL27_LOCAL_W *plw ;

	ew->routine_0 = 2 ;
	pl_w = ew->master_w ;
	plw = ( PL27_LOCAL_W * )pl_w->tlocal ;
	if( ew->pl_type == pl_w->pl_type ) plw->shot_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl27_00_air_vanish( PLAYER_WORK *ew )
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
static void shl27_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl27_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl27_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The master's drawing, animation and side			*/
/*==============================================================*/
static void shl27_00_work_set( PLAYER_WORK *ew )
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
static void shl27_00_shot_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, const SHL27_00_DATA *dt )
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
static void shl27_00_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl27_00.c" End of File	*/
