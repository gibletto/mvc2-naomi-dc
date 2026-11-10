/* SonSon's player effect 01 (character 14's player effect 01): the fifteen kinds of piece that she
   throws and that ride on her, each with a first step that sets it up and a second that moves it.
   A piece ends when her animation changes. */
#include "player.h"
#include "ef_char.h"

/* where a piece rides on her, by its animation: x and y */
typedef struct PLS14_01_POS {
	Sint16	x ;
	Sint16	y ;
} PLS14_01_POS ;

const PLS14_01_POS pls14_01_pos_tbl[35] = {
	{ 21, 0 }, { 23, 0 }, { 25, 0 }, { 25, 137 }, { 150, 0 }, { 150, 0 }, { 36, 0 },
	{ 23, 0 }, { 26, 0 }, { 186, 0 }, { 36, 12 }, { 36, 12 }, { 66, 102 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 },
	{ 0, 214 }, { 266, 171 }, { 253, 274 }, { 186, 360 }, { 186, 360 }, { 0, 0 }, { 0, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { -23, 21 }
} ;

/* the effect's own work: the animation she was in when it began */
typedef struct PLS14_01_LOCAL_W {
	CHAR_NO	char_no ;
} PLS14_01_LOCAL_W ;
#define PLS14_01_LW( w )	( ( PLS14_01_LOCAL_W * )&( w )->obj_flag )

static void pls14_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls14_01_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	PLS14_01_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls14_01_main ;
		ew->master_w = pl_w ;
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
		ew->ef_no = 0x1401 ;
		ew->size_top = ew->size_bottom = 32 ;
		ew->size_front = ew->size_back = 36 ;
		ew->type = type ;
		ew->routine_0 = 1 ;
		ew->routine_1 = no ;
		ew->disp_flag = 0 ;
		lw = PLS14_01_LW( ew ) ;
		lw->char_no.w = pl_w->char_no.w ;
		lw->char_no.b.grp = pl_w->char_no.b.grp ;
	}
	return ew ;
}

static void pls14_01_init( PLAYER_WORK *ew );
static void pls14_01_move( PLAYER_WORK *ew );
static void pls14_01_end( PLAYER_WORK *ew );
static void pls14_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_01_main( PLAYER_WORK *ew )
{
	static void ( *pls14_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_init, pls14_01_move, pls14_01_end, pls14_01_free,
	} ;

	pls14_01_jmp[ew->routine_0]( ew ) ;
}

static void pls14_01_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void pls14_01_pos_set2( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static void pls14_01_prio_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew );
static Sint8 pls14_01_chg_ck( PLAYER_WORK *ew );
static Sint8 pls14_01_fly_ck( PLAYER_WORK *ew );
static void pls14_01_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_01_init( PLAYER_WORK *ew )
{
}

static void pls14_01_move0( PLAYER_WORK *ew );
static void pls14_01_move1( PLAYER_WORK *ew );
static void pls14_01_move2( PLAYER_WORK *ew );
static void pls14_01_move3( PLAYER_WORK *ew );
static void pls14_01_move4( PLAYER_WORK *ew );
static void pls14_01_move5( PLAYER_WORK *ew );
static void pls14_01_move6( PLAYER_WORK *ew );
static void pls14_01_move7( PLAYER_WORK *ew );
static void pls14_01_move8( PLAYER_WORK *ew );
static void pls14_01_move9( PLAYER_WORK *ew );
static void pls14_01_move10( PLAYER_WORK *ew );
static void pls14_01_move11( PLAYER_WORK *ew );
static void pls14_01_move12( PLAYER_WORK *ew );
static void pls14_01_move13( PLAYER_WORK *ew );
static void pls14_01_move14( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move0, pls14_01_move1, pls14_01_move2, pls14_01_move3, pls14_01_move4,
		pls14_01_move5, pls14_01_move6, pls14_01_move7, pls14_01_move8, pls14_01_move9,
		pls14_01_move10, pls14_01_move11, pls14_01_move12, pls14_01_move13, pls14_01_move14,
	} ;

	pls14_01_move_jmp[ew->routine_1]( ew ) ;
	pls14_01_debug( ew ) ;
}

/* kind 0 */
static void pls14_01_move0_wait( PLAYER_WORK *ew );
static void pls14_01_move0_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_01_move0( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move0_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move0_wait, pls14_01_move0_move,
	} ;

	pls14_01_move0_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move0_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( pl_w->cg_prm ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		pls14_01_pos_set( pl_w, ew ) ;
		if( ew->type == 5 ){
			ew->prio = 7 ;
		} else {
			ew->prio = 0 ;
		}
		ef_char_set( ew, 27, ew->type ) ;
		pl_se_req( ew, 47 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move0_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
}

/* kind 1 */
static void pls14_01_move1_wait( PLAYER_WORK *ew );
static void pls14_01_move1_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move1( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move1_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move1_wait, pls14_01_move1_move,
	} ;

	pls14_01_move1_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move1_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( pl_w->cg_type ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		pls14_01_pos_set( pl_w, ew ) ;
		ew->prio = pl_w->prio ;
		ew->sub_prio = -2 ;
		ef_char_set( ew, 27, ew->type ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move1_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp || !pl_w->cg_type ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
	pls14_01_pos_set( pl_w, ew ) ;
}

/* kind 2 */
static void pls14_01_move2_init( PLAYER_WORK *ew );
static void pls14_01_move2_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move2( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move2_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move2_init, pls14_01_move2_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move2_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move2_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 7 ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move2_move( PLAYER_WORK *ew )
{
	VEC3 pos ;

	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		pos.x = ew->pos.x ;
		pos.y = ew->pos.y ;
		pos.y += 80.0f ;
	}
}

/* kind 3 */
static void pls14_01_move3_wait( PLAYER_WORK *ew );
static void pls14_01_move3_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move3( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move3_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move3_wait, pls14_01_move3_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move3_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move3_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	if( pl_w->cg_type ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		pls14_01_pos_set( pl_w, ew ) ;
		ef_char_set( ew, 27, ew->type ) ;
		ew->prio = 7 ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move3_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
}

/* kind 4 */
static void pls14_01_move4_wait( PLAYER_WORK *ew );
static void pls14_01_move4_fly( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move4( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move4_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move4_wait, pls14_01_move4_fly,
	} ;

	pls14_01_move4_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move4_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		pls14_01_pos_set( pl_w, ew ) ;
		ef_char_set( ew, 27, ew->type ) ;
		ew->prio = 0 ;
		if( ew->spd.x < 0 ){
			ew->disp_dir = 0 ;
		} else {
			ew->disp_dir = 1 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move4_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ef_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
	}
}

/* kind 5 */
static void pls14_01_move5_init( PLAYER_WORK *ew );
static void pls14_01_move5_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move5( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move5_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move5_init, pls14_01_move5_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move5_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move5_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 11 ;
	ef_color_set( ew, 2, 2, 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move5_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
}

/* kind 6 */
static void pls14_01_move6_init( PLAYER_WORK *ew );
static void pls14_01_move6_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move6( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move6_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move6_init, pls14_01_move6_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move6_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move6_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 7 ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move6_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ef_char_move( ew ) ;
	if( !pl_w->cg_type ){
		ew->routine_0 = 2 ;
	}
}

/* kind 7 */
static void pls14_01_move7_init( PLAYER_WORK *ew );
static void pls14_01_move7_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move7( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move7_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move7_init, pls14_01_move7_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move7_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move7_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 8 ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move7_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		pl_w = ew->master_w ;
		shl14_00_set( pl_w, 0 ) ;
		shl14_00_set( pl_w, 1 ) ;
		shl14_00_set( pl_w, 2 ) ;
	}
}

/* kinds 8 and 9: the pieces that turn with her and end by her animation */
static void pls14_01_move8_init( PLAYER_WORK *ew );
static void pls14_01_move8_wait( PLAYER_WORK *ew );
static void pls14_01_move8_ride( PLAYER_WORK *ew );
static void pls14_01_move8_fly( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move8( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move8_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move8_init, pls14_01_move8_wait, pls14_01_move8_ride, pls14_01_move8_fly,
	} ;

	pls14_01_move8_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move8_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_pos_set( pl_w, ew ) ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_prio_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 7 ;
	ew->scale.y = 0.75f ;
	if( pls14_01_chg_ck( ew ) ){
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move8_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *ew2 ;
	PLS14_01_LOCAL_W *lw ;
	PLS14_01_LOCAL_W *lw2 ;

	pl_w = ew->master_w ;
	ef_char_move( ew ) ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_pos_set( pl_w, ew ) ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_prio_set( pl_w, ew ) ;
	if( pls14_01_chg_ck( ew ) ){
		return ;
	}
	if( !ew->cg_type ){
		return ;
	}
	ew->routine_2 ++ ;
	ew->prio = 10 ;
	if( ew2 = pls14_01_set( pl_w, 13, 9 ) ){
		ew2->spd.x = ew->spd.x ;
		ew2->disp_dir = ew->disp_dir ;
		ew2->prio = 11 ;
		ew2->dir = ew->dir ;
		lw = PLS14_01_LW( ew ) ;
		lw2 = PLS14_01_LW( ew2 ) ;
		lw2->char_no.w = lw->char_no.w ;
		lw2->char_no.b.grp = lw->char_no.b.grp ;
	}
	if( ew2 = pls14_01_set( pl_w, 14, 9 ) ){
		ew2->spd.x = ew->spd.x ;
		ew2->disp_dir = ew->disp_dir ;
		ew2->prio = 9 ;
		ew2->dir = ew->dir ;
		lw = PLS14_01_LW( ew ) ;
		lw2 = PLS14_01_LW( ew2 ) ;
		lw2->char_no.w = lw->char_no.w ;
		lw2->char_no.b.grp = lw->char_no.b.grp ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move8_ride( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ef_char_move( ew ) ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_pos_set( pl_w, ew ) ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_prio_set( pl_w, ew ) ;
	if( pls14_01_fly_ck( ew ) ){
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move8_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ef_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
	}
}

/*==============================================================*/
/*			If her animation has changed, the piece ends;		*/
/*			returns 1 then										*/
/*==============================================================*/
static Sint8 pls14_01_fly_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_2 = 3 ;
		switch( ew->type ){
		case 13:
			ew->prio = 12 ;
			break ;
		case 14:
			ew->prio = 7 ;
			break ;
		case 15:
			ew->prio = 7 ;
			break ;
		}
		if( ew->spd.x < 0 ){
			ew->disp_dir = 0 ;
		} else {
			ew->disp_dir = 1 ;
		}
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static Sint8 pls14_01_chg_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void pls14_01_prio_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	ew->prio = pl_w->prio ;
	switch( ew->type ){
	case 13:
		ew->sub_prio = -1 ;
		break ;
	case 14:
		ew->sub_prio = -3 ;
		break ;
	case 15:
		ew->sub_prio = -2 ;
		break ;
	}
}

/* kind 9 */
static void pls14_01_move9_init( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move9( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move9_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move9_init, pls14_01_move8_wait, pls14_01_move8_ride, pls14_01_move8_fly,
	} ;

	pls14_01_move9_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move9_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 = 2 ;
	ew->disp_flag = 1 ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_pos_set( pl_w, ew ) ;
	if( ew->dir & 1 ){
		pl_w->disp_dir ^= 1 ;
	}
	pls14_01_prio_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	if( pls14_01_chg_ck( ew ) ){
	}
}

/* kind 10 */
static void pls14_01_move10_init( PLAYER_WORK *ew );
static void pls14_01_move10_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move10( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move10_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move10_init, pls14_01_move10_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move10_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move10_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 0 ;
	ef_color_set( ew, 2, 2, 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move10_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		pls14_01_set( pl_w, 21, 11 ) ;
	}
}

/* kind 11 */
static void pls14_01_move11_init( PLAYER_WORK *ew );
static void pls14_01_move11_fly( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move11( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move11_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move11_init, pls14_01_move11_fly,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move11_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move11_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 0 ;
	if( pl_w->pl_dir ){
		ew->spd.x = -1.25f ;
	} else {
		ew->spd.x = 1.25f ;
	}
	ew->acc.x = 0 ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = -1.09375f ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move11_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ef_char_move( ew ) ;
	if( !scr_in_ck( ew ) ){
		ew->routine_0 = 2 ;
	}
}

/* kind 12 */
static void pls14_01_move12_init( PLAYER_WORK *ew );
static void pls14_01_move12_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move12( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move12_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move12_init, pls14_01_move12_move,
	} ;

	pls14_01_move12_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move12_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	pls14_01_pos_set2( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move12_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
	pls14_01_pos_set2( pl_w, ew ) ;
}

/*==============================================================*/
static void pls14_01_pos_set2( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	Float x ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += -DOT_Y( 64.0f ) ;
	x = DOT_X( 4.0f ) ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		x = -DOT_X( 4.0f ) ;
	}
	ew->pos.x += x ;
}

/* kind 13 */
static void pls14_01_move13_init( PLAYER_WORK *ew );
static void pls14_01_move13_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move13( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move13_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move13_init, pls14_01_move13_move,
	} ;

	pls14_01_move13_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move13_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	switch( ew->type ){
	case 22:
		ew->rot[2] = 0 ;
		break ;
	case 23:
		ew->rot[2] = 0xe000 ;
		break ;
	case 24:
		ew->rot[2] = 0xd000 ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_01_move13_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	lw = PLS14_01_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ef_char_move( ew ) < 0 ) ew->routine_0 = 2 ;
}

/* kind 14 */
static void pls14_01_move14_init( PLAYER_WORK *ew );
static void pls14_01_move14_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_01_move14( PLAYER_WORK *ew )
{
	static void ( *pls14_01_move14_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_01_move14_init, pls14_01_move14_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_01_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_01_LW( ew ) ;
	if( lw->char_no.b.grp != pl_w->char_no.b.grp ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_01_move14_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_01_move14_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	ew->timer0 = 120 ;
	pls14_01_pos_set( pl_w, ew ) ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 11 ;
}

/*--------------------------------------------------------------*/
static void pls14_01_move14_move( PLAYER_WORK *ew )
{
	if( -- ew->timer0 <= 0 ){
		ew->routine_0 = 2 ;
	}
	ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls14_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls14_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void pls14_01_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	Float ofs ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += pls14_01_pos_tbl[ew->type].y ;
	ofs = pls14_01_pos_tbl[ew->type].x ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ){
		ofs = -ofs ;
	}
	ew->pos.x += ofs ;
}

/*==============================================================*/
static void pls14_01_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls14_01.c" End of File	*/
