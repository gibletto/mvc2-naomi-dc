/* Doctor Doom's Molecular Shield (character 0f's shot 05): rocks that circle in front of him and behind while
   he sets them, then fly off in random directions. */
#include "player.h"
#include "pl0f_def.h"

/* the effect's own work */
typedef struct SHL0F_05_LOCAL_W {
	CHAR_NO	char_no;	/* 000: Doom's animation when it was set */
	Uint8	unk_02[2];
	float	spd_x;		/* 004: the circling speed */
	Uint8	back;		/* 008: 1 while it is behind Doom */
} SHL0F_05_LOCAL_W;
#define SHL0F_05_LW( w )	( ( SHL0F_05_LOCAL_W * )&( w )->obj_flag )

/* by the shot's number: its place from Doom in x */
Sint16 shl0f_05_ofs[4] = { 80, 64, 48, 32 } ;

static void shl0f_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0f_05_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	SHL0F_05_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0f_05_main ;
		ew->ef_no = 0x0f01 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		lw = SHL0F_05_LW( ew ) ;
		lw->char_no.w = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl0f_05_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_05_main( PLAYER_WORK *ew )
{
	static void ( *shl0f_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_05_0_main,
	} ;

	shl0f_05_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0f_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0f_05_0_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_05_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_05_init, shl0f_05_move, shl0f_05_end, shl0f_05_free,
	} ;

	shl0f_05_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0f_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL0F_05_LOCAL_W *lw = SHL0F_05_LW( ew ) ;
	Sint16 x ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 16 ;
	ew->atck_no = pl0f_at_shield ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	x = DOT_X( shl0f_05_ofs[ew->sub_type & 3] ) ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->spd.x = -DOT_X( 32.0f ) ;
	ew->acc.x = DOT_X( 4.0f ) ;
	ew->spd.y = DOT_Y( 2.5f ) ;
	ew->acc.y = 0.0f ;
	if( pl_w->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	pl_char_set2( ew, Char_Effect, 22, Rnd() & 15 ) ;
	lw->back = 0 ;
	lw->spd_x = ew->spd.x ;
	shl0f_05_move( ew, pl_w ) ;
}

static void shl0f_05_circle( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_05_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0f_05_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0f_05_move( register PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0f_05_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0f_05_circle, shl0f_05_wait, shl0f_05_fly,
	} ;

	shl0f_05_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl0f_05_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Circles until Doom has set the last of them			*/
/*==============================================================*/
static void shl0f_05_circle( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0F_LOCAL_W *pl_lw = PL0F_LW( pl_w ) ;
	SHL0F_05_LOCAL_W *lw = SHL0F_05_LW( ew ) ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl0f_05_turn( ew, pl_w ) ;
	if( pl_w->char_no.b.grp != lw->char_no.b.grp ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_0 ++ ;
		return ;
	}
	hit_work_set( ew ) ;
	if( pl_lw->shield_fly_flag ){
		ew->routine_1 ++ ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = ew->sub_type * 2 + 30 ;
	}
}

/*==============================================================*/
/*			Circles on the spot for a while, then sets off		*/
/*==============================================================*/
static void shl0f_05_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL0F_05_LOCAL_W *lw = SHL0F_05_LW( ew ) ;
	int rnd ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl0f_05_turn( ew, pl_w ) ;
	if( pl_w->char_no.b.grp != lw->char_no.b.grp ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_0 ++ ;
		return ;
	}
	hit_work_set( ew ) ;
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->atck_no = pl0f_at_shield ;
		HIT_RESET( ew ) ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
		rnd = Rnd() & 3 ;
		if( !pl_w->disp_dir ){
			ew->dir = rnd + 23 ;
		} else {
			ew->dir = rnd + 6 ;
		}
		ew->dir &= 31 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_05_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	dir_spd_move( ew, 800 ) ;
	if( ew->hit_box_no ){
		ew->routine_0 ++ ;
		return ;
	}
	hit_work_set( ew ) ;
	if( ew->hit_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl0f_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl0f_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			At each end of the circle, turns back and passes	*/
/*			to the other side of Doom							*/
/*==============================================================*/
static void shl0f_05_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL0F_05_LOCAL_W *lw = SHL0F_05_LW( ew ) ;

	if( ew->spd.x * ew->acc.x > 0.0f ){
		ew->spd.x = lw->spd_x = -lw->spd_x ;
		ew->acc.x = -ew->acc.x ;
		lw->back ^= 1 ;
	}
	if( !lw->back ){
		ew->prio = 0 ;
	} else {
		ew->prio = 7 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0f_05.c" End of File	*/
