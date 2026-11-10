/* Cyclops's effect 00 (character 06's shot 00): the optic blast of his heavy punches, standing
   (type 0) and crouching (type 1), and its other forms (types 2 to 4). While it flies it flashes
   Cyclops's colours. */
#include "player.h"
#include "pl_char_old.h"
#include "pl06_def.h"

/* the flash's timer and step, kept in the two bytes of timer1 */
#define SHL06_00_FLASH( w )	( ( Sint8 * )&( w )->timer1 )

static void shl06_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl06_00_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl06_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x600 ;
	}
	return ew ;
}

static void shl06_00_init( PLAYER_WORK *ew );
static void shl06_00_move( PLAYER_WORK *ew );
static void shl06_00_end( PLAYER_WORK *ew );
static void shl06_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl06_00_main( PLAYER_WORK *ew )
{
	static void ( *shl06_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl06_00_init, shl06_00_move, shl06_00_end, shl06_00_free,
	} ;

	shl06_00_jmp[ew->routine_0]( ew ) ;
}

static void shl06_00_shot_init( PLAYER_WORK *ew );
static void shl06_00_up_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl06_00_init( PLAYER_WORK *ew )
{
	static void ( *shl06_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl06_00_shot_init, shl06_00_shot_init, shl06_00_up_init, shl06_00_shot_init,
		shl06_00_shot_init,
	} ;

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
	ew->timer1 = 0x101 ;
	shl06_00_init_jmp[ew->type]( ew ) ;
	shl06_00_move( ew ) ;
}

/*==============================================================*/
/*			Forward from his visor								*/
/*==============================================================*/
static void shl06_00_shot_init( PLAYER_WORK *ew )
{
	Float ofs[2] ;
	Uint8 no ;

	ew->disp_flag = 1 ;
	ew->pos = ew->master_w->pos ;
	ofs[0] = -DOT_X( 8.0f ) ;
	ofs[1] = DOT_X( 16.0f ) ;
	if( ew->disp_dir ){
		ofs[0] = -ofs[0] ;
		ofs[1] = -ofs[1] ;
	}
	ew->spd.x = ofs[0] ;
	ew->pos.x += ofs[1] ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = 0.0f ;
	switch( ew->type ){
	case 1:
		ew->pos.y += DOT_Y( 64.0f ) ;
		break ;
	default:
		ew->pos.y += DOT_Y( 88.0f ) ;
		break ;
	}
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->timer0 = 60 ;
	ew->size_back = ew->size_front = 48 ;
	ew->size_top = ew->size_bottom = 16 ;
	if( !ew->type ) no = 2 ;
	else if( ew->type == 1 ) no = 2 ;
	else if( ew->type == 3 ) no = 62 ;
	else no = 67 ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 20, 0 ) ;
}

/*==============================================================*/
/*			Straight up											*/
/*==============================================================*/
static void shl06_00_up_init( PLAYER_WORK *ew )
{
	Uint8 no ;

	ew->disp_flag = 1 ;
	ew->pos = ew->master_w->pos ;
	ew->rot[2] = 0x8000 ;
	ew->spd.y = DOT_Y( 8.0f ) ;
	ew->pos.y += DOT_Y( 80.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = 0.0f ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->timer0 = 120 ;
	ew->size_back = ew->size_front = 16 ;
	ew->size_top = ew->size_bottom = 48 ;
	no = 66 ;
	if( ew->master_w->team_type == 3 ) no = 73 ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 20, 1 ) ;
}

/*--------------------------------------------------------------*/
static void shl06_00_move( PLAYER_WORK *ew )
{
	shl06_01_flash( ew, &SHL06_00_FLASH( ew )[0], &SHL06_00_FLASH( ew )[1] ) ;
	if( --ew->timer0 < 0 || ew->hit_flag || ew->hit_box_no || !scr_in_x_ck( ew ) || !ground_ck( ew ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		pl_color_trans( ew->master_w, 2, 2, 1 ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl06_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl06_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl06_00.c" End of File	*/
