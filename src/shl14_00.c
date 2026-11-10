/* SonSon's Shienbu shots (character 14's shot 00): three monkeys, each thrown on its own path; one
   spins and flies on until it hits or leaves the screen. */
#include "player.h"
#include "pl_char_old.h"
#include "pl14_def.h"

/* a shot's speeds and animation, by path and strength */
typedef struct SHL14_00_DATA {
	Sint16	spd_x, spd_y;
	Sint16	acc_x, acc_y;
	Uint8	char_no;
} SHL14_00_DATA;

/* the effect's own work */
typedef struct SHL14_00_LOCAL_W {
	Uint16	char_no;	/* 000: SonSon's animation when it was thrown */
} SHL14_00_LOCAL_W;
#define SHL14_00_LW( w )	( ( SHL14_00_LOCAL_W * )&( w )->obj_flag )

/* moves the shot by its speed, and its speed by its acceleration */
#define SHL14_00_MOVE( w )	{ ( w )->pos.x += ( w )->spd.x ; ( w )->spd.x += ( w )->acc.x ; \
				  ( w )->pos.y += ( w )->spd.y ; ( w )->spd.y += ( w )->acc.y ; }

const SHL14_00_DATA shl14_00_tbl[6] = {	/* by the path and the strength */
	{ -5, 0, 0, 0, 6 },
	{ -5, 0, 0, 0, 6 },
	{ -5, 6, 0, 0, 7 },
	{ -5, 6, 0, 0, 7 },
	{ -5, -6, 0, 0, 8 },
	{ -5, -6, 0, 0, 8 }
} ;
const Sint32 unk_0C22F55C = 0 ;	/* never used */

static void shl14_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl14_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PL14_LOCAL_W *lw ;
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl14_00_main ;
		ew->type = no ;
		ew->atck_level = pl_w->atck_level ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1400 ;
		SHL14_00_LW( ew )->char_no = pl_w->char_no.w ;
		lw = ( PL14_LOCAL_W * )pl_w->tlocal ;
		lw->shot_cnt ++ ;
	}
	return ew ;
}

static void shl14_00_init( PLAYER_WORK *ew );
static void shl14_00_move( PLAYER_WORK *ew );
static void shl14_00_end( PLAYER_WORK *ew );
static void shl14_00_free( PLAYER_WORK *ew );
static void shl14_00_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_00_main( PLAYER_WORK *ew )
{
	static void ( *shl14_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_00_init, shl14_00_move, shl14_00_end, shl14_00_free,
	} ;

	shl14_00_debug( ew ) ;
	shl14_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The shot takes its path's speeds and leaves			*/
/*			SonSon's hand										*/
/*==============================================================*/
static void shl14_00_init( PLAYER_WORK *ew )
{
	SHL14_00_LOCAL_W *lw = SHL14_00_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	Float spd, acc, ofs ;
	Uint8 no ;

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
	ew->prio = 0 ;
	if( lw->char_no != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	pl_w->sub_type = 1 ;
	ew->rot[2] = 0 ;
	no = ew->type * 2 + ew->atck_level ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	spd = shl14_00_tbl[no].spd_x ;
	ofs = -120.0f ;
	acc = shl14_00_tbl[no].acc_x ;
	if( ew->disp_dir ){
		ofs = 120.0f ;
		spd = -spd ;
		acc = -acc ;
	}
	ew->pos.x += ofs ;
	ew->spd.x = spd ;
	ew->acc.x = acc ;
	ew->spd.y = shl14_00_tbl[no].spd_y ;
	ew->acc.y = shl14_00_tbl[no].acc_y ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl14_at_shienbu ;
	HIT_RESET( ew ) ;
	pl_w->atck_no = pl14_at_shienbu ;
	HIT_RESET( pl_w ) ;
	ew->disp_flag = 0 ;
	pl_char_set( ew, 21, shl14_00_tbl[no].char_no ) ;
	ew->routine_1 = 2 ;
	ew->routine_2 = 0 ;
}

static void shl14_00_wait( PLAYER_WORK *ew );
static void shl14_00_wait2( PLAYER_WORK *ew );
static void shl14_00_shot( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_00_move( PLAYER_WORK *ew )
{
	static void ( *shl14_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_00_wait, shl14_00_wait2, shl14_00_shot,
	} ;

	ew->prio = 11 ;
	shl14_00_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl14_00_wait( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void shl14_00_wait2( PLAYER_WORK *ew )
{
}

static void shl14_00_hit( PLAYER_WORK *ew );
static void shl14_00_start( PLAYER_WORK *ew );
static void shl14_00_spin( PLAYER_WORK *ew );
static void shl14_00_fly( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_00_shot( PLAYER_WORK *ew )
{
	static void ( *shl14_00_shot_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_00_start, shl14_00_spin, shl14_00_fly,
	} ;

	shl14_00_shot_jmp[ew->routine_2]( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ew->hit_flag ){
		shl14_00_hit( ew ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		shl14_00_hit( ew ) ;
		catch_hitmark( ew ) ;
		return ;
	}
	ew->prio = 11 ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Struck or hit: it drops off the screen				*/
/*==============================================================*/
static void shl14_00_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	ew->spd.x = -1.6666666f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	ew->rot[2] = 0 ;
	if( ew->disp_dir ) ew->spd.x = 1.6666666f ;
	ew->timer0 = 0 ;
	pl_char_set( ew, 21, 9 ) ;
	pl_w = ew->master_w ;
	if( ew->pl_type == pl_w->pl_type ) pl_w->sub_type = 0 ;
}

static void shl14_00_rot( PLAYER_WORK *ew );

/*==============================================================*/
/*			The throw: the path's turn, height and speeds		*/
/*==============================================================*/
static void shl14_00_start( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->routine_2 ++ ;
	if( ew->disp_dir ){
		ew->spd.x = ew->atck_level ? DOT_X( 4.0f ) : 5.0f ;
	} else {
		ew->spd.x = ew->atck_level ? -DOT_X( 4.0f ) : -5.0f ;
	}
	switch( ew->type ){
	case 0 :
		ew->timer0 = 12 ;
		ew->timer1 = ew->timer0 - 7 ;
		ew->rot[2] = 0xe000 ;
		if( ew->disp_dir ){
			ew->pos.y += 120.0f ;
			ew->spd.y = 2.142857f ;
			break ;
		}
		ew->pos.y += 120.0f ;
		ew->spd.y = 2.142857f ;
		break ;
	case 1 :
		ew->timer0 = 12 ;
		ew->timer1 = ew->timer0 - 5 ;
		ew->rot[2] = 0x1000 ;
		if( ew->disp_dir ){
			ew->pos.y += DOT_Y( 24.0f ) ;
			ew->spd.y = -2.142857f ;
			break ;
		}
		ew->pos.y += DOT_Y( 24.0f ) ;
		ew->spd.y = -2.142857f ;
		break ;
	case 2 :
		ew->timer0 = 0 ;
		ew->timer1 = ew->timer0 - 3 ;
		ew->rot[2] = 0x2000 ;
		if( ew->disp_dir ){
			ew->pos.y += DOT_Y( 40.0f ) ;
			break ;
		}
		ew->pos.y += DOT_Y( 40.0f ) ;
		break ;
	}
	SHL14_00_MOVE( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl14_00_spin( PLAYER_WORK *ew )
{
	SHL14_00_MOVE( ew ) ;
	shl14_00_rot( ew ) ;
	if( ew->timer0-- > 0 ) return ;
	ew->routine_2 ++ ;
	if( ew->disp_dir ){
		ew->spd.x = ew->atck_level ? DOT_X( 4.0f ) : 5.0f ;
	} else {
		ew->spd.x = ew->atck_level ? -DOT_X( 4.0f ) : -5.0f ;
	}
	switch( ew->type ){
	case 0 :
		ew->rot[2] = 0x2000 ;
		ew->spd.y = ew->disp_dir ? -DOT_Y( 1.25f ) : -DOT_Y( 1.25f ) ;
		break ;
	case 1 :
		ew->rot[2] = 0xe000 ;
		ew->spd.y = ew->disp_dir ? DOT_Y( 1.25f ) : DOT_Y( 1.25f ) ;
		break ;
	case 2 :
		ew->rot[2] = 0 ;
		ew->spd.y = ew->disp_dir ? 0.0f : 0.0f ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_00_fly( PLAYER_WORK *ew )
{
	SHL14_00_MOVE( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl14_00_end( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	SHL14_00_MOVE( ew ) ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_00_free( PLAYER_WORK *ew )
{
	PL14_LOCAL_W *lw = ( PL14_LOCAL_W * )ew->master_w->tlocal ;

	lw->shot_cnt -- ;
	work_free( ew ) ;
}

/*==============================================================*/
static void shl14_00_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*			The spin: a turn of the shot every other frame,		*/
/*			the way its path bends								*/
/*==============================================================*/
static void shl14_00_rot( PLAYER_WORK *ew )
{
	if( --ew->timer1 > 0 ) return ;
	ew->timer1 = 1 ;
	switch( ew->type ){
	case 0 :
		ew->rot[2] += 0x800 ;
		break ;
	case 1 :
		ew->rot[2] -= 0x800 ;
		break ;
	case 2 :
		ew->rot[2] -= 0x800 ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl14_00.c" End of File	*/
