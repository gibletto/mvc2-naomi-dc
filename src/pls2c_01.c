/* Magneto's player effect 01: the things that go with his moves, six kinds: ones that
   follow his sprite (0, 1 and 4), one that stands on the ground (2), one that blinks beside him
   (3) and one that shows the frame his animation asks for, from a table (5). */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl2c_def.h"

/* kind 5: what each frame code of his animation shows, by the work's sub_type */
typedef struct PLS2C_01_REC {
	Sint8	disp_flag;
	Uint8	unk_01;
	Uint16	frame;		/* the animation frame */
	Float	scale_x;
	Float	scale_y;
	Float	x;		/* where it stands in front of him, mirrored with his facing */
	Float	y;
} PLS2C_01_REC;

static PLS2C_01_REC pls2c_01_tbl[3][9] = {
	{
		{ 1, 0, 0, 1.0f, 1.0f, DOT_X( 464.0f ), DOT_Y( 80.0f ) },
		{ 1, 0, 0, 1.0f, 1.0f, DOT_X( 464.0f ), DOT_Y( 80.0f ) },
		{ 1, 0, 0, 1.0f, 1.0f, DOT_X( 464.0f ), DOT_Y( 80.0f ) },
		{ 1, 0, 2, 1.0f, 1.0f, DOT_X( 424.0f ), DOT_Y( 80.0f ) },
		{ 1, 0, 4, 1.0f, 1.0f, 480.0f, DOT_Y( 82.0f ) },
		{ 1, 0, 4, 1.0f, 0.6f, 480.0f, DOT_Y( 82.0f ) },
		{ 1, 0, 4, 1.0f, 0.3f, 480.0f, DOT_Y( 82.0f ) },
		{ 1, 0, 4, 1.0f, 1.5f, DOT_X( 352.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 4, 1.0f, 1.0f, DOT_X( 352.0f ), DOT_Y( 82.0f ) },
	},
	{
		{ 1, 0, 1, 15.0f, 1.0f, DOT_X( 320.0f ), DOT_Y( 80.0f ) },
		{ 1, 0, 1, 15.0f, 1.0f, DOT_X( 320.0f ), DOT_Y( 80.0f ) },
		{ 1, 0, 1, 15.0f, 1.0f, DOT_X( 320.0f ), DOT_Y( 80.0f ) },
		{ 1, 0, 3, 4.0f, 1.0f, 600.0f, DOT_Y( 80.0f ) },
		{ 1, 0, 5, 1.0f, 1.0f, DOT_X( 272.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 5, 1.0f, 0.6f, DOT_X( 272.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 5, 1.0f, 0.3f, DOT_X( 272.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 5, 4.1f, 1.5f, DOT_X( 304.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 5, 4.1f, 1.0f, DOT_X( 304.0f ), DOT_Y( 82.0f ) },
	},
	{
		{ 0, 0, 0, 1.0f, 1.0f, 0.0f, DOT_Y( 80.0f ) },
		{ 0, 0, 0, 1.0f, 1.0f, 0.0f, DOT_Y( 80.0f ) },
		{ 0, 0, 0, 1.0f, 1.0f, 0.0f, DOT_Y( 80.0f ) },
		{ 0, 0, 0, 1.0f, 1.0f, 0.0f, DOT_Y( 80.0f ) },
		{ 1, 0, 6, 1.0f, 1.0f, DOT_X( 80.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 6, 1.0f, 0.6f, DOT_X( 80.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 6, 1.0f, 0.3f, DOT_X( 80.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 6, 1.0f, 1.5f, DOT_X( 80.0f ), DOT_Y( 82.0f ) },
		{ 1, 0, 6, 1.0f, 1.0f, DOT_X( 80.0f ), DOT_Y( 82.0f ) },
	},
} ;

static void pls2c_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls2c_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2c_01_main ;
		ew->type = type ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x2c01 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls2c_01_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 kind )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2c_01_main ;
		ew->type = type ;
		ew->sub_type = kind ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x2c01 ;
	}
	return ew ;
}

static void pls2c_01_trail( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2c_01_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2c_01_blink( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2c_01_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2c_01_main( PLAYER_WORK *ew )
{
	static void ( *pls2c_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls2c_01_trail, pls2c_01_trail, pls2c_01_ground, pls2c_01_blink, pls2c_01_trail, pls2c_01_frame,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 ) work_free( ew ) ;
	else pls2c_01_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It follows his place, with his animation group's	*/
/*			own frame by the work's type						*/
/*==============================================================*/
static void pls2c_01_trail( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
		if( pl_w->pl_stance == STANCE_AIR ){
			work_free( ew ) ;
			return ;
		}
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
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ef_char_set( ew, 27, ew->type + 2 ) ;
	}
	if( ef_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*==============================================================*/
static void pls2c_01_ground( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ef_char_set( ew, 27, 4 ) ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->ground_y ;
	if( ef_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			Kind 3: it blinks beside him while his move			*/
/*			lasts												*/
/*==============================================================*/
static void pls2c_01_blink( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 ofs ;

	if( !ew->routine_0 ){
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
		ew->pos.x = pl_w->pos.x ;
		ew->sub_prio = -4 ;
		ef_char_set( ew, 27, 5 ) ;
		pl_voice_req( ew, 33 ) ;
	}
	ew->prio = pl_w->prio ;
	if( pl_w->routine_1 != 1 || !pl_w->cg_prm ) work_free( ew ) ;
	else {
		if( pl_w->disp_dir ) ofs = 72 ;
		else ofs = -72 ;
		ew->pos.x = pl_w->pos.x + DOT_X( ofs ) ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 116.0f ) ;
		ef_char_move( ew ) ;
		if( Sys->game_timer0 & 1 ) ew->disp_flag = 0 ;
		else ew->disp_flag = 1 ;
	}
}

/*==============================================================*/
/*			Kind 5: the frame his animation asks for, from		*/
/*			pls2c_01_tbl										*/
/*==============================================================*/
static void pls2c_01_frame( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS2C_01_REC *rec ;
	Float x ;

	if( !ew->routine_0 ){
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
		ew->disp_flag = 0 ;
		ew->timer1 = 0 ;
	}
	else {
		ew->prio = pl_w->prio ;
		if( pl_w->char_no.w == 0x1500 || pl_w->char_no.w == 0x1510 ){
			if( pl_w->cg_type ){
				if( ew->timer1 != pl_w->cg_type ){
					ew->timer1 = pl_w->cg_type ;
					rec = &pls2c_01_tbl[ew->sub_type][ew->timer1 - 1] ;
					if( ( ew->disp_flag = rec->disp_flag ) ){
						ef_char_set2( ew, 27, 18, rec->frame ) ;
						ew->scale.x = rec->scale_x ;
						ew->scale.y = rec->scale_y ;
						x = rec->x ;
						ew->pos.y = pl_w->pos.y + rec->y ;
						if( !pl_w->disp_dir ) x = -x ;
						ew->pos.x = pl_w->pos.x + x ;
					}
				}
			}
			else {
				ew->timer1 = ew->disp_flag = 0 ;
			}
		}
		else {
			ew->scale.x = ew->scale.y = 1.0f ;
			work_free( ew ) ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2c_01.c" End of File	*/
