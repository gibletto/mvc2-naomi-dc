/* A part on shl14_04's riding attack (character 14's shot 05): turned and scaled by SonSon's
   animation number from 38, it sets five shl14_06 pieces on itself. */
#include "player.h"
#include "pl_char.h"
#include "pl14_def.h"

/* the part's scale, by SonSon's animation number from 38 */
typedef struct SHL14_05_DATA {
	Float	x, y;
} SHL14_05_DATA;

/* the effect's own work */
typedef struct SHL14_05_LOCAL_W {
	CHAR_NO	char_no;	/* 000: SonSon's animation, kept up with her */
} SHL14_05_LOCAL_W;
#define SHL14_05_LW( w )	( ( SHL14_05_LOCAL_W * )EF_WK( w )->work )

const Sint32 shl14_05_rot[7] = { 0xe200, 0xf400, 0xf400, 0x0000, 0x0c00, 0x0c00, 0x1800 } ;	/* its turn, by SonSon's animation number - 38 */
const SHL14_05_DATA shl14_05_tbl[7] = {	/* by SonSon's animation number - 38 */
	{ 1.8f, 1.5f }, { 1.5f, 1.5f }, { 1.5f, 1.5f }, { 1.49f, 1.5f }, { 1.0f, 1.5f }, { 1.0f, 1.5f }, { 0.8f, 1.5f }
} ;
/* never used: places like shl14_04_tbl's */
const Sint16 unk_0C22F60C[14][2] = {
	{ 30, 259 }, { 30, 259 }, { 46, 250 }, { 46, 250 }, { 46, 250 }, { 48, 257 }, { 48, 257 },
	{ 61, 39 }, { 61, 19 }, { 61, 19 }, { 61, -8 }, { 61, -312 }, { 61, -39 }, { 61, 0 }
} ;

static void shl14_05_main( PLAYER_WORK *ew );
static void shl14_05_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew );

/****************************************************************/
void *shl14_05_set( PLAYER_WORK *pl_w, PLAYER_WORK *hw )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl14_05_main ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1405 ;
		ew->master_w = pl_w ;
		ew->ef_w = hw ;
		SHL14_05_LW( ew )->char_no.w = pl_w->char_no.w ;
		SHL14_05_LW( ew )->char_no.b.grp = pl_w->char_no.b.grp ;
		SHL14_05_LW( ew )->char_no.b.no = pl_w->char_no.b.no ;
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
		ew->size_top = ew->size_bottom = 32 ;
		ew->size_front = ew->size_back = 36 ;
		ew->prio = 0 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->atck_no = pl14_at_enou_rush ;
		HIT_RESET( ew ) ;
		ew->disp_flag = 1 ;
		ew->rot[2] = hw->rot[2] ;
		shl14_05_pos_set( pl_w, ew ) ;
		pl_char_set( ew, 22, 58 ) ;
		shl14_06_set( pl_w, ew, 0 ) ;
		shl14_06_set( pl_w, ew, 1 ) ;
		shl14_06_set( pl_w, ew, 2 ) ;
		shl14_06_set( pl_w, ew, 3 ) ;
		shl14_06_set( pl_w, ew, 4 ) ;
	}
	return ew ;
}

static void shl14_05_init( PLAYER_WORK *ew );
static void shl14_05_move( PLAYER_WORK *ew );
static void shl14_05_end( PLAYER_WORK *ew );
static void shl14_05_free( PLAYER_WORK *ew );
static void shl14_05_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_05_main( PLAYER_WORK *ew )
{
	static void ( *shl14_05_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_05_init, shl14_05_move, shl14_05_end, shl14_05_free,
	} ;

	if( ew->routine_0 < 2 ) shl14_05_debug( ew ) ;
	shl14_05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The setter has done it all							*/
/*==============================================================*/
static void shl14_05_init( PLAYER_WORK *ew )
{
}

static void shl14_05_ride( PLAYER_WORK *ew );
static void shl14_05_after( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_05_move( PLAYER_WORK *ew )
{
	static void ( *shl14_05_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_05_ride, shl14_05_after,
	} ;

	shl14_05_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Follows SonSon's animation; at its last steps it	*/
/*			plays its own end									*/
/*==============================================================*/
static void shl14_05_ride( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL14_05_LOCAL_W *lw = SHL14_05_LW( ew ) ;

	pl_char_move( ew ) ;
	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_w->pl_sp_type == pl14_sp_enou ){
		if( pl_w->routine_2 == 3 ){
			ew->routine_1 ++ ;
			pl_char_set( ew, 22, 60 ) ;
			return ;
		}
		if( pl_w->routine_2 == 4 ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	if( pl_w->pl_sp_type == pl14_sp_enou_auto ){
		if( pl_w->routine_2 == 7 ){
			ew->routine_1 ++ ;
			pl_char_set( ew, 22, 60 ) ;
			return ;
		}
		if( pl_w->routine_2 == 8 ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	if( pl_w->char_no.b.no < 38 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 60 ) ;
		return ;
	}
	lw->char_no.b.no = pl_w->char_no.b.no ;
	ew->rot[2] = shl14_05_rot[pl_w->char_no.b.no - 38] ;
	shl14_05_pos_set( pl_w, ew ) ;
}

/*--------------------------------------------------------------*/
static void shl14_05_after( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) ew->routine_0 = 2 ;
}

/*--------------------------------------------------------------*/
static void shl14_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl14_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl14_05_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*			Puts the part on the riding attack (ef_w),			*/
/*			turned by rot[2], facing SonSon's way				*/
/*==============================================================*/
static void shl14_05_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	PLAYER_WORK *hw ;
	Float x, y, dx, dy ;

	x = -70.0f ;
	y = -DOT_Y( 4.0f ) ;
	dx = x * nlCos( ew->rot[2] ) + y * nlSin( ew->rot[2] ) ;
	dy = x * nlSin( ew->rot[2] ) + y * nlCos( ew->rot[2] ) ;
	hw = ew->ef_w ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	if( pl_w->pl_dir ) dx = -dx ;
	ew->pos.x += dx ;
	ew->pos.y += dy ;
	ew->scale.x = shl14_05_tbl[pl_w->char_no.b.no - 38].x ;
	ew->scale.y = shl14_05_tbl[pl_w->char_no.b.no - 38].y ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl14_05.c" End of File	*/
