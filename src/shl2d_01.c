/* Shuma-Gorath's effect 01 (character 2d's shot 01): the small eye he lets fall in his Hyper Mystic Smash.
   It drops from his side with a random speed, lands, and flashes when it touches an opponent. */
#include "player.h"
#include "pl_char.h"
#include "pl2d_def.h"

/* the eye's first speed by its kind: x and y, 16.16 */
static Sint32 shl2d_01_spd[4 * 2] = {
	-327680, 786432, -458752, 458752, -589824, 655360, -720896, 327680,
} ;

static void shl2d_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2d_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) != 0 ){
		ew->ef_no = 0x2d01 ;
		ew->work_func = shl2d_01_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void shl2d_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2d_01_main( PLAYER_WORK *ew )
{
	static void ( *shl2d_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2d_01_init, shl2d_01_move, shl2d_01_free,
	} ;

	shl2d_01_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void shl2d_01_spd_set( PLAYER_WORK *ew );
static void shl2d_01_body_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2d_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs_x ;

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
	ew->routine_0 ++ ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->type = Rnd() & 3 ;
	ew->sub_type = 0 ;
	ofs_x = -80.0f ;
	if( ew->disp_dir ) ofs_x = 80.0f ;
	ew->pos.x = pl_w->pos.x + ofs_x ;
	ew->pos.y = pl_w->ground_y ;
	pl_char_set( ew, Char_Effect, 26 ) ;
}

static void shl2d_01_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_01_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_01_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_01_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2d_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2d_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2d_01_start, shl2d_01_fall, shl2d_01_land, shl2d_01_hit,
	} ;

	shl2d_01_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It comes out and is thrown							*/
/*==============================================================*/
static void shl2d_01_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->size_front = 32 ;
		ew->size_back = 32 ;
		pl_char_set( ew, Char_Effect, 28 ) ;
		shl2d_01_spd_set( ew ) ;
		shl2d_01_body_ck( ew, pl_w ) ;
		ew->atck_no = ew->type + 62 ;
		HIT_RESET( ew ) ;
		hit_work_set( ew ) ;
	}
}

/*==============================================================*/
/*			It falls to the floor								*/
/*==============================================================*/
static void shl2d_01_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	shl2d_01_body_ck( ew, pl_w ) ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->pos.y < pl_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		hit_work_set( ew ) ;
		pl_char_set( ew, Char_Effect, 29 ) ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->routine_1 = 3 ;
		pl_char_set( ew, Char_Effect, 30 ) ;
	} else if( ew->hit_flag == 0 ) hit_work_set( ew ) ;
}

/*==============================================================*/
/*			It lies on the floor, where it can be hit and		*/
/*			set off again										*/
/*==============================================================*/
static void shl2d_01_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		shl2d_01_spd_set( ew ) ;
		if( ew->hit_flag ){
			ew->atck_no = ew->type + 62 ;
			HIT_RESET( ew ) ;
		}
		ew->routine_1 -- ;
		pl_char_set( ew, Char_Effect, 28 ) ;
	}
	if( ew->hit_box_no ){
		ew->routine_1 = 3 ;
		pl_char_set( ew, Char_Effect, 30 ) ;
	} else {
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2d_01_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl2d_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Gives the eye its speed from its kind				*/
/*==============================================================*/
static void shl2d_01_spd_set( PLAYER_WORK *ew )
{
	Sint32 *spd = &shl2d_01_spd[ew->type * 2] ;
	Float x ;

	x = *spd ++ * 1.6666666f / 65536.0f ;
	if( ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->acc.x = 0.0f ;
	ew->spd.y = *spd * DOT_Y( 1.0f ) / 65536.0f ;
	ew->acc.y = -1.2053571f ;
}

/*==============================================================*/
/*			The eye lights up while it is inside the body of	*/
/*			one of the opponent's team							*/
/*==============================================================*/
static void shl2d_01_body_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = pl_work ;
	int i ;
	Float rad ;
	Float dx ;
	Float dy ;

	em_w += 1 >> ( pl_w->pl_no & 1 ) ;
	for( i = 0 ; i < 3 ; i ++, em_w += 2 ){
		if( !em_w->be_flag ) continue ;
		rad = ( em_w->size_top + em_w->size_bottom ) * DOT_Y( 1.0f ) ;
		dx = ew->pos.x - em_w->pos.x ;
		dy = ew->pos.y - em_w->pos.y + em_w->size_bottom * DOT_Y( 1.0f ) - rad ;
		if( dx * dx + dy * dy < rad * rad ){
			if( ew->sub_type ) continue ;
			ew->sub_type = 1 ;
			pl_char_set( ew, Char_Effect, 27 ) ;
			return ;
		}
		if( ew->sub_type ){
			ew->sub_type = 0 ;
			pl_char_set( ew, Char_Effect, 28 ) ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2d_01.c" End of File	*/
