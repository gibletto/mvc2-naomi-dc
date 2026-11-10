/* Silver Samurai's effect 04 (character 2f's shot 04): a pair of sparks that fall to the ground
   from his sides, and the sparks they give off on the way. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl2f_def.h"

/* the setter of effect 2f05, in another module */
extern void *pls2f_05_set( PLAYER_WORK *pl_w, int no );

static void shl2f_04_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2f_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	register int i ;
	Float x ;

	for( i = 0 ; i < 4 ; i += 2 ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl2f_04_main ;
			ew->master_w = pl_w ;
			ew->ef_no = 0x2f04 ;
			ew->type = i ;
			x = -DOT_X( 8.0f ) ;
			if( pl_w->disp_dir ) x = DOT_X( 8.0f ) ;
			ew->pos.x = pl_w->pos.x + x ;
			ew->pos.y = pl_w->ground_y + DOT_Y( 48.0f ) ;
		}
	}
	return ew ;
}

/*==============================================================*/
/*			A spark off the one before it, sideways by its		*/
/*			type and down by chance								*/
/*==============================================================*/
static void shl2f_04_next_set( PLAYER_WORK *ew0, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Float x, y ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2f_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2f04 ;
		ew->type = ew0->type ;
		y = ew0->pos.y - DOT_Y( 16.0f ) ;
		x = 0.0f ;
		if( y < pl_w->ground_y ){
			x = DOT_X( 20.0f ) ;
			if( ew0->type ) x = -DOT_X( 20.0f ) ;
			y = pl_w->ground_y + ( Rnd() & 7 ) * 2.142857f ;
			ew->sub_type = 1 ;
		}
		ew->pos.x = ew0->pos.x + x ;
		ew->pos.y = y ;
	}
}

static void shl2f_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2f_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2f_04_main( PLAYER_WORK *ew )
{
	static void ( *shl2f_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2f_04_init, shl2f_04_move, shl2f_04_end, shl2f_04_free,
	} ;

	shl2f_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl2f_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->prio = 8 ;
	ew->timer0 = 32 ;
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl2f_at_hyouga ;
	HIT_RESET( ew ) ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 32 ;
	pl_char_set( ew, 22, ew->sub_type + 8 ) ;
	if( scr_in_x_ck( ew ) ) shl2f_04_next_set( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2f_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		if( -- ew->timer0 <= 0 ){
			ew->routine_1 ++ ;
			if( !ew->sub_type ) ef_char_set( ew, 27, ( Rnd() & 3 ) + 10 ) ;
		}
		else {
			if( !ew->cg_type ) pl_char_move( ew ) ;
			hit_work_set( ew ) ;
		}
		if( !ew->routine_2 && ew->hit_flag ){
			PLAYER_WORK *em_w = ew->em_w ;

			if( em_w->dm_no != 9 ) return ;
			if( pls2f_05_set( pl_w, 1 ) ){
				ew->routine_2 ++ ;
				pl_se_req( ew, 10 ) ;
			}
		}
	} else {
		if( ew->sub_type ){
			if( pl_char_move( ew ) < 0 ) shl2f_04_free( ew, pl_w ) ;
			return ;
		}
		if( ef_char_move( ew ) < 0 ){
			shl2f_04_free( ew, pl_w ) ;
			return ;
		}
		if( ef_char_move( ew ) < 0 ) shl2f_04_free( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2f_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*--------------------------------------------------------------*/
static void shl2f_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2f_04.c" End of File	*/
