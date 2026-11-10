/* Iceman's effect 01 (character 09's shot 01): the ice blast, a work that sends out pieces every
   few frames while his move lasts; each piece flies off and hits on every eighth frame. */
#include "player.h"
#include "pl_char.h"

static void shl09_01_main( PLAYER_WORK *ew );

static Uint8 shl09_01_unk[8] = { 0, 1, 0, 1, 0, 1, 0, 1 } ;	/* not read */
static Sint16 shl09_01_ofs[8] = { 14, 97, 6, 84, 27, 74, 18, 94 } ;	/* a piece's start, x and y, by chance */
static Float shl09_01_spd[4][2] = {
	{ DOT_X( 10.0f ), 15.0f }, { DOT_X( 9.25f ), DOT_Y( 4.0f ) }, { 15.0f, 2.142857f }, { DOT_X( 8.125f ), -DOT_Y( 2.0f ) },
} ;
static Float shl09_01_spd_y[8] = {	/* a piece's rise, by chance */
	DOT_Y( 4.0f ), DOT_Y( 2.5f ), 2.142857f, -DOT_Y( 0.5f ), -DOT_Y( 2.0f ), -7.5f, -DOT_Y( 4.5f ), -DOT_Y( 5.0f ),
} ;

/****************************************************************/
void *shl09_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl09_01_main ;
		ew->type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x901 ;
		ew->timer0 = pl_w->char_no.w ;
	}
	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl09_01_main ;
		ew->type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x901 ;
		ew->timer0 = pl_w->char_no.w ;
	}
	return ew ;
}

/*==============================================================*/
/*			A piece, on the blast's work (ef_w)					*/
/*==============================================================*/
static void shl09_01_part_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pw ;

	if( ( pw = work_get( 0, W_SHL, LIST_TOP ) ) ){
		pw->work_func = shl09_01_main ;
		pw->type = 1 ;
		pw->master_w = ew->master_w ;
		pw->ef_w = ew ;
		pw->pl_type = ew->pl_type ;
		pw->ef_no = 0x901 ;
	}
}

static void shl09_01_init( PLAYER_WORK *ew );
static void shl09_01_move( PLAYER_WORK *ew );
static void shl09_01_end( PLAYER_WORK *ew );
static void shl09_01_free( PLAYER_WORK *ew );
static void shl09_01_ck( PLAYER_WORK *ew );

/*==============================================================*/
static void shl09_01_main( PLAYER_WORK *ew )
{
	static void ( *shl09_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_01_init, shl09_01_move, shl09_01_end, shl09_01_free,
	} ;

	shl09_01_ck( ew ) ;
	shl09_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl09_01_init( PLAYER_WORK *ew )
{
	Uint8 no ;
	Float x, spd ;

	ew->routine_0 ++ ;
	if( !ew->type ){
		ew->disp_flag = 0 ;
		ew->timer1 = 3 ;
		return ;
	}
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
	ew->disp_flag = 1 ;
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->timer1 = 35 ;
	if( !Rnd() & 3 ){	/* as written: almost never true */
		ew->atck_flag = 0 ;
		ew->dm_flag = 0 ;
		ew->sub_type = 1 ;
	} else {
		no = 57 ;
		if( !( Rnd() & 3 ) ) no = 56 ;
		ew->atck_no = no ;
		HIT_RESET( ew ) ;
		ew->sub_type = 0 ;
		ew->atck_flag = HIT_ON | HIT_OBJ ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	no = Rnd() & 3 ;
	x = DOT_X( shl09_01_ofs[no * 2] ) ;
	spd = shl09_01_spd[0][0] ;
	if( !ew->disp_dir ){
		x = -x ;
		spd = -spd ;
	}
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( shl09_01_ofs[no * 2 + 1] ) ;
	ew->spd.x = spd ;
	ew->spd.y = shl09_01_spd_y[Rnd() & 7] ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->sub_type = Rnd() & 7 ;
	pl_char_set2( ew, 22, 4, ( Sint32 )Rnd() % 3 ) ;
}

/*--------------------------------------------------------------*/
static void shl09_01_move( PLAYER_WORK *ew )
{
	if( !ew->type ){
		if( --ew->timer1 <= 0 ){
			ew->timer1 = 3 ;
			shl09_01_part_set( ew ) ;
		}
		if( ew->master_w->routine_2 >= 4 ){
			ew->routine_0 ++ ;
			shl09_01_end( ew ) ;
			return ;
		}
		if( ew->master_w->char_no.w != ew->timer0 ){
			ew->routine_0 ++ ;
			shl09_01_end( ew ) ;
		}
		return ;
	}
	if( ew->hit_flag || ew->hit_box_no || --ew->timer1 <= 0 ){
		ew->routine_0 ++ ;
		pl_char_set( ew, 22, 5 ) ;
		shl09_01_end( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->sub_type = ( ew->sub_type + 1 ) & 7 ;
	if( !ew->sub_type ) hit_work_set( ew ) ;
}

/*==============================================================*/
/*			A piece's break-up, which goes on moving			*/
/*==============================================================*/
static void shl09_01_end( PLAYER_WORK *ew )
{
	if( !ew->type || pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		shl09_01_free( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void shl09_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl09_01_ck( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl09_01.c" End of File	*/
