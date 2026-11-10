/* Psylocke's effect 01 (character 08's shot 01): a blade that turns as it flies, speeding up a
   step each quarter turn, until she leaves her move. */
#include "player.h"
#include "pl08_def.h"

/* the blade's own work, from obj_flag: pos is where it would be in whole dots, the master's place
   moved by each new speed; step picks the speed */
typedef struct SHL08_01_LOCAL_W {
	Sint32	obj_flag;	/* 000: its animation, read as the work's own obj_flag */
	Sint32	pos[2];		/* 004: x, y */
	Sint32	step;		/* 00c: 0 to 8 (shl08_01_spd) */
} SHL08_01_LOCAL_W ;
#define SHL08_01_LW( w )	( ( SHL08_01_LOCAL_W * )&( w )->obj_flag )

static void shl08_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl08_01_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl08_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->ef_no = 0x801 ;
	}
	return ew ;
}

static void shl08_01_init( PLAYER_WORK *ew );
static void shl08_01_move( PLAYER_WORK *ew );
static void shl08_01_fly( PLAYER_WORK *ew );
static void shl08_01_free( PLAYER_WORK *ew );
static void shl08_01_char_set( PLAYER_WORK *ew );
static void shl08_01_pos_move( PLAYER_WORK *ew );
static void shl08_01_spd_set( PLAYER_WORK *ew );
static void shl08_01_nop( PLAYER_WORK *ew );

/*==============================================================*/
static void shl08_01_main( PLAYER_WORK *ew )
{
	static void ( *shl08_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl08_01_init, shl08_01_move, shl08_01_fly, shl08_01_free,
	} ;

	shl08_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl08_01_init( PLAYER_WORK *ew )
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
	ew->dm_flag = HIT_ON ;
	if( ew->type & 1 ){
		ew->atck_no = pl08_at_butterfly_2 ;
		HIT_RESET( ew ) ;
	}
	else {
		ew->atck_no = pl08_at_butterfly ;
		HIT_RESET( ew ) ;
	}
	SHL08_01_LW( ew )->pos[0] = ew->master_w->pos.x ;
	SHL08_01_LW( ew )->pos[1] = ew->master_w->pos.y ;
	ew->size_top = ew->size_bottom = ew->size_back = ew->size_front = 32 ;
	SHL08_01_LW( ew )->step = ew->timer1 ;
	shl08_01_spd_set( ew ) ;
	ew->timer0 = 0 ;
	ew->obj_flag = 22 ;
	shl08_01_char_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl08_01_move( PLAYER_WORK *ew )
{
	ew->prio = ew->master_w->prio ;
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->timer0 = 3 ;
		if( ew->disp_dir & 1 ) ew->dir += 2 ;
		else ew->dir -= 2 ;
		ew->dir &= 31 ;
		if( ew->master_w->char_no.w == 0x1510 ){
			ew->obj_flag = 17 ;
			ew->routine_0 ++ ;
		} else if( ew->master_w->act_st == N_SUPER ){
			/* still held: it goes on turning */
		} else {
			ew->obj_flag = 17 ;
			ew->routine_0 ++ ;
		}
		shl08_01_char_set( ew ) ;
		if( !( ew->dir & 3 ) ){
			if( ew->obj_flag != 17 ) ew->obj_flag -- ;
			ew->timer1 ++ ;
			if( ew->timer1 > 8 ) ew->timer1 = 8 ;
			SHL08_01_LW( ew )->step = ew->timer1 ;
			shl08_01_spd_set( ew ) ;
		} else shl08_01_pos_move( ew ) ;
	} else shl08_01_pos_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Let go: it flies on until it hits or leaves the		*/
/*			screen												*/
/*==============================================================*/
static void shl08_01_fly( PLAYER_WORK *ew )
{
	if( ew->hit_flag || !scr_in_ck( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl08_01_pos_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl08_01_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			Its animation: obj_flag, at the frame of its		*/
/*			turn (dir, mirrored by the facing)					*/
/*==============================================================*/
static void shl08_01_char_set( PLAYER_WORK *ew )
{
	int dir = ew->dir ;

	if( ew->disp_dir & 1 ) dir ^= 16 ;
	dir /= 2 ;
	pl_char_set2( ew, 21, ew->obj_flag, dir ) ;
}

/*==============================================================*/
static void shl08_01_pos_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl08_01_nop( ew ) ;
}

/* the speed by step, in the direction of its turn */
static const Sint16 shl08_01_spd[9] = { 400, 500, 600, 700, 800, 900, 1000, 1100, 1200 } ;

/*==============================================================*/
static void shl08_01_spd_set( PLAYER_WORK *ew )
{
	dir_spd_move( ew, shl08_01_spd[SHL08_01_LW( ew )->step] ) ;
	SHL08_01_LW( ew )->pos[0] += ( Sint32 )ew->spd.x ;
	SHL08_01_LW( ew )->pos[1] += ( Sint32 )ew->spd.y ;
	shl08_01_nop( ew ) ;
}

/*==============================================================*/
/*			Empty: both moves end in it							*/
/*==============================================================*/
static void shl08_01_nop( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl08_01.c" End of File	*/
