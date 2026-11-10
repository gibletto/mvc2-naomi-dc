/* Psylocke's effect 00 (character 08's shot 00): the Psy-Blast, which flies by its kind (three
   strengths from the ground, three from the air) until it hits, lands or leaves the screen. */
#include "player.h"
#include "pl_char.h"
#include "pl08_def.h"

static void shl08_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl08_00_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;
	PL08_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl08_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x800 ;
		lw = ( PL08_LOCAL_W * )pl_w->tlocal ;
		lw->shot_flag = 1 ;
	}
	return ew ;
}

static void shl08_00_init( PLAYER_WORK *ew );
static void shl08_00_move( PLAYER_WORK *ew );
static void shl08_00_end( PLAYER_WORK *ew );
static void shl08_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl08_00_main( PLAYER_WORK *ew )
{
	static void ( *shl08_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl08_00_init, shl08_00_move, shl08_00_end, shl08_00_free,
	} ;

	shl08_00_jmp[ew->routine_0]( ew ) ;
}

/* the speed, x and y, by kind */
static const Float shl08_00_spd[6][2] = {
	{ -9.0625f, -3.6160712f }, { -10.0f, 0.0f }, { -DOT_X( 5.0f ), DOT_Y( 2.5f ) },
	{ -7.5f, -6.5625f }, { -10.0f, 0.0f }, { -DOT_X( 5.0f ), DOT_Y( 2.5f ) },
} ;

/*==============================================================*/
/*			The attack set-up is written in each branch of		*/
/*			the type test										*/
/*==============================================================*/
static void shl08_00_init( PLAYER_WORK *ew )
{
	PL08_LOCAL_W *lw ;
	Float v[2] ;

	ew->routine_0 ++ ;
	lw = ( PL08_LOCAL_W * )ew->master_w->tlocal ;
	ew->size_top = ew->size_bottom = 192 ;
	ew->size_back = ew->size_front = 64 ;
	ew->timer0 = 0 ;
	lw->shot_flag = 1 ;
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
	if( ew->master_w->pl_stance == STANCE_AIR ){
		if( ew->type == 4 ){
			ew->atck_no = pl08_at_blast_h ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl08_at_jblast ;
			HIT_RESET( ew ) ;
		}
	} else ew->atck_no = ew->master_w->atck_level + 59 ;
	HIT_RESET( ew ) ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z + -1.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	v[0] = shl08_00_spd[ew->type][0] ;
	v[1] = -DOT_X( 26.0f ) ;
	if( ew->disp_dir ){
		v[0] = -v[0] ;
		v[1] = -v[1] ;
	}
	ew->spd.x = v[0] ;
	ew->pos.x += v[1] ;
	ew->pos.y += DOT_Y( 80.0f ) ;
	ew->spd.y = shl08_00_spd[ew->type][1] ;
	ew->char_no.b.grp = 21 ;
	ew->char_no.b.no = ew->type + 2 ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
	ew->timer0 = nlRand() ;
	ew->timer0 &= 7 ;
	ew->timer0 ++ ;
	shl08_00_move( ew ) ;
}

static void shl08_00_fall( PLAYER_WORK *ew );
static void shl08_00_fly( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl08_00_move( PLAYER_WORK *ew )
{
	static void ( *shl08_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl08_00_fall, shl08_00_fly, shl08_00_fly,
		shl08_00_fall, shl08_00_fly, shl08_00_fly,
	} ;

	shl08_00_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			The light ones fall and end on the ground			*/
/*==============================================================*/
static void shl08_00_fall( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->hit_flag || ew->pos.y < pl_w->ground_y + DOT_Y( 24.0f ) ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 3 ;
		return ;
	}
	ew->timer0 -- ;
	if( ew->timer0 == 0 ){
		ew->timer0 = nlRand() ;
		ew->timer0 &= 3 ;
		ew->timer0 ++ ;
		pls08_01_set( ew, 0 ) ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl08_00_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->hit_flag ){
		ew->routine_0 ++ ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 3 ;
		return ;
	}
	ew->timer0 -- ;
	if( ew->timer0 == 0 ){
		ew->timer0 = nlRand() ;
		ew->timer0 &= 3 ;
		ew->timer0 ++ ;
		pls08_01_set( ew, 0 ) ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

static void shl08_00_end_init( PLAYER_WORK *ew );
static void shl08_00_end_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl08_00_end( PLAYER_WORK *ew )
{
	static void ( *shl08_00_end_jmp[] )( PLAYER_WORK *ew ) = {
		shl08_00_end_init, shl08_00_end_move,
	} ;

	shl08_00_end_jmp[ew->routine_1]( ew ) ;
}

/* the animation of its end, by kind */
static const Uint8 shl08_00_end_no[6] = { 8, 9, 10, 8, 9, 10 } ;

/*==============================================================*/
static void shl08_00_end_init( PLAYER_WORK *ew )
{
	PL08_LOCAL_W *lw ;

	ew->routine_1 ++ ;
	lw = ( PL08_LOCAL_W * )ew->master_w->tlocal ;
	lw->shot_flag = 0 ;
	ew->char_no.b.grp = 21 ;
	ew->char_no.b.no = shl08_00_end_no[ew->type] ;
	pl_char_set( ew, ew->char_no.b.grp, ew->char_no.b.no ) ;
}

/*--------------------------------------------------------------*/
static void shl08_00_end_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl08_00_free( PLAYER_WORK *ew )
{
	PL08_LOCAL_W *lw = ( PL08_LOCAL_W * )ew->master_w->tlocal ;

	lw->shot_flag = 0 ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl08_00.c" End of File	*/
