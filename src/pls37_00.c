/* Jin's player effects: the pieces that stand beside him, follow his animation or fly off
   (types 0 to 16, picked by the type byte), each a copy of his own drawing state. */
#include "player.h"
#include "pl_char.h"
#include "pl_color.h"
#include "pl37_def.h"

/* type 15's place on the screen: how far out it has run, and the edge it runs from */
typedef struct PLS37_00_RUN_W {
	Sint16	dist;
	Sint16	edge;
} PLS37_00_RUN_W;

/* the effect's own work, from obj_flag: the animation it follows; type 15's run record is at +8 */
typedef struct PLS37_00_LOCAL_W {
	CHAR_NO	char_no;
} PLS37_00_LOCAL_W;
#define PLS37_00_LW( w )	( ( PLS37_00_LOCAL_W * )&( w )->obj_flag )
#define PLS37_00_RUN( w )	( ( PLS37_00_RUN_W * )( &( w )->obj_flag + 2 ) )

/* the wobble of type 15, by frame */
static Sint8 pls37_00_wobble[32] = {
	-4, 0, 4, 0, -4, 0, 4, 0, -4, 0, 4, 0, -4, 0, 4, 0,
	-8, 0, 8, 0, -8, 0, 8, 0, -8, 0, 8, 0, -8, 0, 8, 0,
} ;

/* type 8: where each piece goes */
static Sint16 pls37_00_ofs8[] = {
	59, 23, 15, -61, -55, -29, -15, 65,
} ;

/* type 9: where each piece starts, and its animation */
static Sint16 pls37_00_ofs9[] = {
	-33, 119, -27, 111, -129, 57, -131, 79, -137, 75,
	-33, 119, -120, 50, -120, 62, -158, 98, -158, 98,
} ;
static Uint8 pls37_00_no9[10] = { 26, 26, 27, 27, 27, 26, 27, 27, 28, 28 } ;

static void pls37_00_main( PLAYER_WORK *ew );
static void pls37_00_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/****************************************************************/
PLAYER_WORK *pls37_00_set( PLAYER_WORK *w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	PLS37_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls37_00_main ;
		ew->ef_no = 0x3700 ;
		ew->master_w = w ;
		ew->pl_type = w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
		lw = PLS37_00_LW( ew ) ;
		lw->char_no.s = w->char_no.s ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *pls37_00_set2( PLAYER_WORK *w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	PLS37_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls37_00_main ;
		ew->ef_no = 0x3700 ;
		ew->master_w = w ;
		ew->pl_type = w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
		lw = PLS37_00_LW( ew ) ;
		lw->char_no.s = w->char_no.s ;
	}
	return ew ;
}

static void pls37_00_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main5( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main6( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main8( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main9( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main13( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main15( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_main16( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_main( PLAYER_WORK *ew )
{
	static void ( *pls37_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_main0, pls37_00_main1, pls37_00_main2, pls37_00_main3,
		pls37_00_nop, pls37_00_main5, pls37_00_main6, pls37_00_main7,
		pls37_00_main8, pls37_00_main9, pls37_00_main9, pls37_00_main9,
		pls37_00_main0, pls37_00_main13, pls37_00_main0, pls37_00_main15,
		pls37_00_main16,
	} ;

	pls37_00_jmp[ew->type]( ew, ew->master_w ) ;
}

/* what every type copies from the player it rides on */
#define PLS37_00_COPY( ew, pl_w ) \
	ew->routine_0 ++ ; \
	CHAR_PART_COPY( ew, pl_w ) ; \
	ew->disp_flag = 1 ; \
	ew->pl_no = pl_w->pl_no ; \
	ew->pl_type = pl_w->pl_type ; \
	ew->scale.x = pl_w->scale.x ; \
	ew->scale.y = pl_w->scale.y ; \
	ew->atck_level = pl_w->atck_level ; \
	ew->work_no = pl_w->work_no ; \
	ew->col_cnt = pl_w->col_cnt ; \
	ew->scale = pl_w->scale

static void pls37_00_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	ew->sub_prio = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 4.4791665f ;
	ew->spd.y = DOT_Y( 2.5f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, 18, 2 ) ;
	pls37_00_move0( ew, pl_w ) ;
}

/*==============================================================*/
static void pls37_00_wait0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = PL37_LW( pl_w ) ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( lw->appear_ef_flag ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->timer0 = 56 ;
	}
}

/*--------------------------------------------------------------*/
static void pls37_00_fly0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ! -- ew->timer0 ) ew->routine_0 ++ ;
}

static void pls37_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls37_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls37_00_move0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_wait0, pls37_00_fly0,
	} ;

	ew->prio = pl_w->prio ;
	pls37_00_jmp0[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls37_00_main0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp0[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init0, pls37_00_move0, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp0[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 0 ) ;
	pls37_00_move1( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->char_no.b.grp != Char_Special ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls37_00_main1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp1[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init1, pls37_00_move1, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp1[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 1 ) ;
	pls37_00_move2( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->char_no.b.grp != Char_Special ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls37_00_main2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp2[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init2, pls37_00_move2, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp2[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 2 ) ;
	pls37_00_move3( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->char_no.b.grp != Char_Special ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls37_00_main3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp3[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init3, pls37_00_move3, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp3[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls37_00_nop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

static void pls37_00_move5( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 8 ) ;
	pls37_00_move5( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_LOCAL_W *lw = PLS37_00_LW( ew ) ;

	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	pl_char_move( ew ) ;
	if( pl_w->char_no.s != lw->char_no.s ) ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls37_00_main5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp5[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init5, pls37_00_move5, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp5[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move6( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x -= DOT_X( 10.0f ) ;
	else ew->pos.x += DOT_X( 10.0f ) ;
	ew->pos.y += DOT_Y( 36.0f ) ;
	pl_char_set( ew, Char_Effect, 21 ) ;
	pls37_00_move6( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Special ){
		ew->routine_0 ++ ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls37_00_main6( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp6[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init6, pls37_00_move6, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp6[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, 7 ) ;
	pls37_00_move7( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_LOCAL_W *lw = PLS37_00_LW( ew ) ;

	if( pl_w->char_no.s != lw->char_no.s ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void pls37_00_main7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp7[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init7, pls37_00_move7, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp7[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move8( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init8( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, Char_Effect, ew->sub_type + 22 ) ;
	pls37_00_move8( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move8( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_LOCAL_W *lw = PLS37_00_LW( ew ) ;
	Sint16 x ;

	if( pl_w->char_no.s != lw->char_no.s ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	x = DOT_X( pls37_00_ofs8[ew->sub_type * 2] ) ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( pls37_00_ofs8[ew->sub_type * 2 + 1] ) ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
static void pls37_00_main8( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp8[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init8, pls37_00_move8, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp8[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move9( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = DOT_X( pls37_00_ofs9[ew->sub_type * 2] ) ;
	ew->pos.y += DOT_Y( pls37_00_ofs9[ew->sub_type * 2 + 1] ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->pos.x += ew->spd.x ;
	pl_char_set( ew, Char_Effect, pls37_00_no9[ew->sub_type] ) ;
	pls37_00_move9( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
/* the goto is the one shared routine_0 step for its two ways out */
static void pls37_00_move9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_LOCAL_W *lw = PLS37_00_LW( ew ) ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.s != lw->char_no.s ) goto done ;
	if( pl_char_move( ew ) >= 0 ) return ;
	ew->disp_flag = 0 ;
done:
	ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls37_00_main9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp9[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init9, pls37_00_move9, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp9[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move13( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init13( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = -65.0f ;
	ew->spd.y = DOT_Y( 101.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, Char_Effect, 29 ) ;
	pls37_00_move13( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move13( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_LOCAL_W *lw = PLS37_00_LW( ew ) ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.s != lw->char_no.s ) goto done13 ;
	ew->pos.x = pl_w->pos.x + ew->spd.x ;
	ew->pos.y = pl_w->pos.y + ew->spd.y ;
	if( pl_char_move( ew ) >= 0 ) return ;
	ew->disp_flag = 0 ;
done13:
	ew->routine_0 ++ ;
}

/*==============================================================*/
static void pls37_00_main13( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp13[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init13, pls37_00_move13, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp13[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move15( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_RUN_W *p = PLS37_00_RUN( ew ) ;

	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->disp_dir ^= 1 ;
	p->dist = 0 ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = DOT_X( 8.0f ) ;
	pl_char_set( ew, 22, 10 ) ;
	pls37_00_move15( ew, pl_w ) ;
}

/*==============================================================*/
static void pls37_00_edge15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_RUN_W *p = PLS37_00_RUN( ew ) ;

	if( !ew->disp_dir ) p->edge = scr_work.scr_right ;
	else p->edge = scr_work.scr_left ;
	if( p->dist > DOT_X( 352.0f ) ) p->dist = 586 ;
}

/*==============================================================*/
static void pls37_00_run15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_RUN_W *p = PLS37_00_RUN( ew ) ;

	p->dist += ew->spd.x ;
}

/*==============================================================*/
static void pls37_00_pos15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_RUN_W *p = PLS37_00_RUN( ew ) ;
	Sint16 x ;

	x = p->dist ;
	if( !ew->disp_dir ) x = -x ;
	ew->pos.x = x + p->edge ;
}

/*==============================================================*/
static void pls37_00_wobble15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint8 n ;

	n = pls37_00_wobble[ew->timer0 & 31] ;
	if( ew->disp_dir ) n = -n ;
	ew->pos.x += n ;
}

extern void wall_dust_pos_set( VEC3 *pos, Uint8 dir );

/*==============================================================*/
static void pls37_00_come15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = PL37_LW( pl_w ) ;
	VEC3 pos ;
	Sint16 d ;

	pls37_00_edge15( ew, pl_w ) ;
	pls37_00_run15( ew, pl_w ) ;
	pls37_00_pos15( ew, pl_w ) ;
	d = ew->pos.x - pl_w->pos.x ;
	if( d < 0 ) d = -d ;
	if( d < 16 ){
		lw->edge_flag = 1 ;
		if( !ew->disp_dir ) pos.x = ew->pos.x + DOT_Y( 48.0f ) ;
		else pos.x = ew->pos.x - DOT_Y( 48.0f ) ;
		pos.y = ew->pos.y + DOT_Y( 48.0f ) ;
		pos.z = ew->pos.z + 0.0f ;
		wall_dust_pos_set( &pos, ew->disp_dir ) ;
		ew->timer0 = 8 ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls37_00_wait15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls37_00_edge15( ew, pl_w ) ;
	pls37_00_pos15( ew, pl_w ) ;
	if( !ew->timer0 -- ){
		ew->timer0 = 32 ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls37_00_shake15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pls37_00_edge15( ew, pl_w ) ;
	pls37_00_pos15( ew, pl_w ) ;
	pls37_00_wobble15( ew, pl_w ) ;
	if( !ew->timer0 -- ){
		ew->spd.x = -ew->spd.x ;
		ew->timer0 = 16 ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void pls37_00_back15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_RUN_W *p = PLS37_00_RUN( ew ) ;

	if( p->dist <= 0 ) ew->timer0 = 0 ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	} else {
		pls37_00_edge15( ew, pl_w ) ;
		pls37_00_run15( ew, pl_w ) ;
		pls37_00_pos15( ew, pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls37_00_move15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp15[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_come15, pls37_00_wait15, pls37_00_shake15, pls37_00_back15,
	} ;
	PLS37_00_LOCAL_W *lw = PLS37_00_LW( ew ) ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != lw->char_no.b.grp ){
		ew->routine_0 ++ ;
		return ;
	}
	pls37_00_jmp15[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls37_00_main15( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp15[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init15, pls37_00_move15, pls37_00_end, pls37_00_free,
	} ;

	pls37_00_jmp15[ew->routine_0]( ew, pl_w ) ;
}

static void pls37_00_move16( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls37_00_init16( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS37_00_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	pls37_00_move16( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_move16( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 pls37_00_col16[12] = { 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0, 0 } ;
	PL37_LOCAL_W *lw = PL37_LW( pl_w ) ;

	pl_w->armor_timer = 2 ;
	if( !pl_w->vital_new ){
		ew->routine_0 ++ ;
		return ;
	}
	{
		VS_SYS_WORK *vs = &vs_sys_work ;
		PLAYER_WORK *em = pl_w->target_w ;

		if( ( em->pl_type == PL_ABYSS1 || em->pl_type == PL_ABYSS2 ) && ( vs->time_over || vs->vs_st == VS_WIN ) ){
			ew->routine_0 ++ ;
			return ;
		}
		if( vs->game_stop & ( 1 << ( ew->pl_no ^ 1 ) ) ) return ;
	}
	if( !lw->burning_flag ) return ;
	if( lw->burning_timer -- ) return ;
	lw->burning_timer = 4 ;
	if( -- lw->burning_col < 0 ) lw->burning_col = 9 ;
	ef_color_set( pl_w, 0, pl_w->pl_color * 6 + pls37_00_col16[lw->burning_col], 1 ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_end16( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *lw = PL37_LW( pl_w ) ;

	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	lw->burning_flag = 0 ;
	lw->burning_end_flag = 1 ;
	pl_color_set( pl_w, 0 ) ;
}

/*==============================================================*/
static void pls37_00_main16( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls37_00_jmp16[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls37_00_init16, pls37_00_move16, pls37_00_end16, pls37_00_free,
	} ;

	pls37_00_jmp16[ew->routine_0]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls37_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls37_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls37_00.c" End of File	*/
