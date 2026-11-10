/* Dhalsim's effect 04 (character 25's shot 04): one of the flames of Yoga Inferno. It starts
   at a place picked by the number he gives it, and flies along the turn that place sets. */
#include "player.h"
#include "pl_char.h"
#include "pl25_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL25_04_LOCAL_W {
	Uint16	char_no;	/* 00: his animation when it was made */
	Uint8	flame_no;		/* 02: the flame's number, then which of the two attack numbers it has */
	Uint8	pos_no;		/* 03: its place in the tables, two for each place */
} SHL25_04_LOCAL_W;

/* where each flame starts from him: x, y */
static Float shl25_04_pos_tbl[] = {
	-DOT_X( 52.0f ), DOT_Y( 48.0f ), -DOT_X( 56.0f ), DOT_Y( 52.0f ), -100.0f, 120.0f, -DOT_X( 68.0f ), DOT_Y( 60.0f ),
	-120.0f, DOT_Y( 64.0f ), -DOT_X( 64.0f ), DOT_Y( 68.0f ), -DOT_X( 68.0f ), DOT_Y( 72.0f ), -DOT_X( 76.0f ), DOT_Y( 76.0f ),
	-DOT_X( 58.0f ), DOT_Y( 80.0f ), -100.0f, 180.0f,
} ;

/* the way it faces from there, facing one way and the other */
static Uint8 shl25_04_dir_tbl[] = {
	0x28, 0x18, 0x2a, 0x16, 0x2c, 0x14, 0x2e, 0x12, 0x30, 0x10,
	0x32, 0x0e, 0x35, 0x0c, 0x37, 0x0a, 0x3a, 0x08, 0x3c, 0x06,
} ;

static PL25_LOCAL_W *shl25_04_pl_lw ;	/* Dhalsim's tlocal, set each frame by the main routine */
static SHL25_04_LOCAL_W *shl25_04_lw ;	/* the effect's own work */

static void shl25_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl25_04_set( PLAYER_WORK *pl_w, Sint8 no, Sint8 flame_no )
{
	PLAYER_WORK *ew ;
	SHL25_04_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl25_04_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2504 ;
		lw = ( SHL25_04_LOCAL_W * )&ew->obj_flag ;
		lw->flame_no = flame_no ;
		lw->pos_no = no ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl25_04_init( PLAYER_WORK *ew );
static void shl25_04_move( PLAYER_WORK *ew );
static void shl25_04_end( PLAYER_WORK *ew );
static void shl25_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl25_04_main( PLAYER_WORK *ew )
{
	static void ( *shl25_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl25_04_init, shl25_04_move, shl25_04_end, shl25_04_free,
	} ;

	shl25_04_pl_lw = PL25_LW( ew->master_w ) ;
	shl25_04_lw = ( SHL25_04_LOCAL_W * )&ew->obj_flag ;
	shl25_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl25_04_init( PLAYER_WORK *ew )
{
	Float *pos ;
	Uint8 *dir ;
	int no ;

	ew->routine_0 ++ ;
	if( !shl25_04_lw->flame_no ){
		ew->atck_no = pl25_at_inferno_1 ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl25_at_inferno_2 ;
		HIT_RESET( ew ) ;
	}
	ew->atck_attr = ATR_BACK_POS ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	if( shl25_04_lw->char_no != ew->master_w->char_no.w ){
		shl25_04_end( ew ) ;
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
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->spd.x = 0 ;
	ew->spd.y = 0 ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	shl25_04_lw->flame_no = shl25_04_pl_lw->flame_no ;
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	pos = shl25_04_pos_tbl ;
	dir = shl25_04_dir_tbl ;
	no = shl25_04_lw->pos_no ;
	pos += no ;
	dir += no ;
	ew->pos.y += pos[1] ;
	if( !ew->disp_dir ){
		ew->pos.x += pos[0] ;
		ew->dir = dir[0] ;
	}
	else {
		ew->pos.x -= pos[0] ;
		ew->dir = dir[1] ;
	}
	pl_char_set( ew, 20, 6 ) ;
	shl25_04_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl25_04_move( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ) goto out ;
	hit_work_set( ew ) ;
	if( pl_char_move( ew ) < 0 ){
out :
		shl25_04_end( ew ) ;
		return ;
	}
	ew->spd.x = nlCos( ( ( 80 - ew->dir ) & 63 ) << 10 ) * DOT_X( 5.0f ) ;
	ew->spd.y = nlSin( ( ( 80 - ew->dir ) & 63 ) << 10 ) * DOT_Y( 5.0f ) ;
	ew->acc.x = 0 ;
	ew->acc.y = 0 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void shl25_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl25_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl25_04.c" End of File	*/
