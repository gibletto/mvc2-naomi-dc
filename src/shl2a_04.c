/* Storm's effect 04 (character 2a's shot 04): the three bolts that fly off her hands in her
   Lightning Sphere in the air (sp_jsphere), one at a time by the number the setter is given. */
#include "player.h"
#include "pl_char.h"
#include "pl2a_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL2A_04_LOCAL_W {
	Sint32	char_no;	/* 000: the animation Storm was in when it was made */
} SHL2A_04_LOCAL_W;
#define SHL2A_04_LW( w )	( ( SHL2A_04_LOCAL_W * )&( w )->obj_flag )

static void shl2a_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2a_04_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2a_04_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->dir = no ;
		ew->ef_no = 0x2a04 ;
		SHL2A_04_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl2a_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2a_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2a_04_main( PLAYER_WORK *ew )
{
	static void ( *shl2a_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2a_04_init, shl2a_04_move, shl2a_04_free, shl2a_04_free,
	} ;

	shl2a_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl2a_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* where each bolt starts in front of Storm */
	static Sint16 shl2a_04_pos_tbl[3][2] = {
		{ 58, 94 }, { 37, 118 }, { 52, 70 },
	} ;
	/* its speed, x then y */
	static Float shl2a_04_spd_tbl[3][2] = {
		{ -DOT_X( 8.0f ), 0.0f }, { -DOT_X( 8.0f ), DOT_Y( 3.0f ) }, { -DOT_X( 8.0f ), -DOT_Y( 3.0f ) },
	} ;
	Float x, y ;

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
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl2a_at_jsphere ;
	HIT_RESET( ew ) ;
	ew->timer0 = 60 ;
	x = shl2a_04_pos_tbl[ew->dir][0] ;
	y = shl2a_04_pos_tbl[ew->dir][1] ;
	if( ew->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x - x * 1.6666666f ;
	ew->pos.y = pl_w->pos.y + DOT_Y( y ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	x = shl2a_04_spd_tbl[ew->dir][0] ;
	y = shl2a_04_spd_tbl[ew->dir][1] ;
	if( ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	ew->spd.y = y ;
	pl_char_set( ew, 11, 8 ) ;
	shl2a_04_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
	} else if( ew->hit_flag ){
	} else if( ew->hit_box_no ){
	} else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
		return ;
	}
	shl2a_04_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2a_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2a_04.c" End of File	*/
