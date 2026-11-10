/* Effect 05 that Iron Man's moves call (ef_no 0x2e05): a hit beside him, set by his normal and
   special attacks. Types 0 and 1 keep a place beside him for their animation and strike with it
   while he stays in the same animation; type 2 is the shot of his crouching heavy punch, which
   flies out from him and bursts. */
#include "player.h"
#include "pl_char.h"
#include "pl33_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL33_05_LOCAL_W {
	Uint16	char_no;	/* 000: Iron Man's animation when it was set */
	Sint16	x, y;		/* 002: its place from him */
} SHL33_05_LOCAL_W;
#define SHL33_05_LW( w )	( ( SHL33_05_LOCAL_W * )&( w )->obj_flag )

static void shl33_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl33_05_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	SHL33_05_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl33_05_main ;
		ew->ef_no = 0x2e05 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		lw = SHL33_05_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl33_05_hit_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_05_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_05_main( PLAYER_WORK *ew )
{
	static void ( *shl33_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_05_hit_main, shl33_05_hit_main, shl33_05_shot_main,
	} ;

	shl33_05_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl33_05_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_05_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_05_hit_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_05_hit_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_05_hit_init, shl33_05_hit_move, shl33_05_end, shl33_05_free,
	} ;

	shl33_05_hit_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_05_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	/* the hits, four halfwords each: where it sits from Iron Man (x flips with his facing,
	   then y), its attack (0xff for none) and its animation */
	static Sint16 shl33_05_hit_tbl[] = {
		-39, 63, 2, 26,
		-76, 6, 10, 27,
		-116, 55, 4, 28,
		-20, 82, 13, 31,
		0, 0, 5, 33,
		0, 0, 11, 34,
		0, 0, 12, 0,
		0, 0, 11, 0,
		-39, 63, 71, 26,
		-82, 70, 255, 28,
	} ;
	SHL33_05_LOCAL_W *lw = SHL33_05_LW( ew ) ;

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
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	lw->x = DOT_X( shl33_05_hit_tbl[ew->sub_type * 4] ) ;
	lw->y = DOT_Y( shl33_05_hit_tbl[ew->sub_type * 4 + 1] ) ;
	if( ew->disp_dir ) lw->x = -lw->x ;
	ew->atck_no = shl33_05_hit_tbl[ew->sub_type * 4 + 2] ;
	if( ew->atck_no != 0xff ){
		ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
	}
	pl_char_set( ew, Char_Effect, shl33_05_hit_tbl[ew->sub_type * 4 + 3] ) ;
	shl33_05_hit_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl33_05_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL33_05_LOCAL_W *lw = SHL33_05_LW( ew ) ;

	if( pl_w->char_no.w != lw->char_no ){
		shl33_05_free( ew, pl_w ) ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.x += lw->x ;
	ew->pos.y += lw->y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl33_05_end( ew, pl_w ) ;
		return ;
	}
	if( ew->atck_no != 0xff ) hit_work_set( ew ) ;
}

static void shl33_05_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_05_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl33_05_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl33_05_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl33_05_shot_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl33_05_shot_init, shl33_05_shot_move, shl33_05_burst, shl33_05_free,
	} ;

	shl33_05_shot_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl33_05_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->size_front = ew->size_back = 34 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x -= 120.0f ;
	else ew->pos.x += 120.0f ;
	ew->pos.y += 120.0f ;
	ew->spd.x = -DOT_X( 4.0f ) ;
	ew->acc.x = -DOT_X( 0.25f ) ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->atck_no = ATCK_CROUCH_HP ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 29 ) ;
	shl33_05_shot_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl33_05_shot_move( register PLAYER_WORK *ew, register PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( ew->hit_box_no ){ ew->routine_0 ++ ; return ; }
	hit_work_set( ew ) ;
	if( ew->hit_flag ){ ew->routine_0 ++ ; return ; }
	if( !scr_in_x_ck( ew ) ) shl33_05_free( ew, pl_w ) ;
}

/*==============================================================*/
/*			The shot bursts on what it hit						*/
/*==============================================================*/
static void shl33_05_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	burst_set( ew, &ew->pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl33_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl33_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl33_05.c" End of File	*/
