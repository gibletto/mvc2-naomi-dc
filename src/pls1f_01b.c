/* B.B. Hood's player effect 01, the rest: the three things that circle her and chase after her.
   Each one (types 0 to 2) takes a kind from her animation (cg_flag1 / 2): it wanders about her,
   follows her, or, in the kinds that chase, flies in to a place beside her and holds there. */
#include "player.h"
#include "ef_char.h"
#include "o_sub1.h"
#include "pl1f_def.h"

extern Sint8 pls1f_01_dir_tbl[4] ;
extern Float pls1f_01_spd_tbl0[200] ;
extern Float pls1f_01_spd_tbl1[200] ;
extern Float pls1f_01_spd_tbl2[200] ;

/* an effect's animation frame: the first eight bytes of a player's */
typedef struct EF_CHAR_DATA {
	Sint32	unk_00;		/* the frame's codes; after a last frame, where the animation goes on */
	Uint8	unk_04[0x08 - 0x04];
} EF_CHAR_DATA;

/* what an effect's frame leaves in cg_prm */
typedef struct EF_CHAR_WORK {
	EF_CHAR_DATA data;
	Sint32	unk_08;
	Sint32	unk_0c;
	Sint32	unk_10;
} EF_CHAR_WORK;

#define EF_CG( w )	( *( EF_CHAR_WORK * )&( w )->cg_prm )

/* the effect's own work */
typedef struct PLS1F_01_LOCAL_W {
	Float	*spd_tbl;	/* its type's table: x and y speed, by kind and phase */
	Sint16	dir_spd;		/* the speed dir_move moves it at */
	Uint8	phase;		/* the phase, 0 to 3 */
	Uint8	spd_no;		/* its place in the speed table */
} PLS1F_01_LOCAL_W;
#define PLS1F_01_LW( w )	( ( PLS1F_01_LOCAL_W * )&( w )->obj_flag )

extern void pls1f_01_move( PLAYER_WORK *ew );

/****************************************************************/
void pls1f_01_init( PLAYER_WORK *ew )
{
	static Float *pls1f_01_tbl[] = { pls1f_01_spd_tbl0, pls1f_01_spd_tbl1, pls1f_01_spd_tbl2 } ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PLS1F_01_LOCAL_W *lw = PLS1F_01_LW( ew ) ;
	Float ofs ;

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
	ew->prio = 11 ;
	lw->spd_tbl = pls1f_01_tbl[ew->type] ;
	ew->sub_type = 0xff ;
	ew->pos.y = pl_w->pos.x + DOT_Y( 96.0f ) ;
	ofs = -80.0f ;
	if( pl_w->disp_dir ) ofs = 80.0f ;
	ew->pos.x = ofs + pl_w->pos.x ;
	pls1f_01_move( ew ) ;
}

static void pls1f_01_t0( PLAYER_WORK *ew );
static void pls1f_01_t1( PLAYER_WORK *ew );
static void pls1f_01_t2( PLAYER_WORK *ew );

/****************************************************************/
void pls1f_01_move( PLAYER_WORK *ew )
{
	static void ( *pls1f_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls1f_01_t0, pls1f_01_t1, pls1f_01_t2,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ( pl_w->cg_flag1 >> 1 ) != ew->sub_type ){
		ew->sub_type = pl_w->cg_flag1 >> 1 ;
		ew->routine_1 = 0 ;
	}
	pls1f_01_move_jmp[ew->type]( ew ) ;
	if( pl_w->be_flag && pl_w->disp_flag ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
}

static void pls1f_01_wander( PLAYER_WORK *ew );
static void pls1f_01_follow( PLAYER_WORK *ew );
static void pls1f_01_chase( PLAYER_WORK *ew );

/*==============================================================*/
/*			The kind each animation of hers gives, for each		*/
/*			type												*/
/*==============================================================*/
static void pls1f_01_t0( PLAYER_WORK *ew )
{
	static void ( *pls1f_01_kind0[] )( PLAYER_WORK *ew ) = {
		pls1f_01_wander, pls1f_01_follow, pls1f_01_follow,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_follow,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_follow,
	pls1f_01_chase, pls1f_01_chase, pls1f_01_follow,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_wander, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_follow,
	} ;

	pls1f_01_kind0[ew->sub_type]( ew ) ;
}

/*==============================================================*/
static void pls1f_01_t1( PLAYER_WORK *ew )
{
	static void ( *pls1f_01_kind1[] )( PLAYER_WORK *ew ) = {
		pls1f_01_wander, pls1f_01_wander, pls1f_01_wander,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_follow,
	pls1f_01_follow, pls1f_01_wander, pls1f_01_wander,
	pls1f_01_follow, pls1f_01_wander, pls1f_01_follow,
	pls1f_01_chase, pls1f_01_chase, pls1f_01_follow,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_wander, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_follow,
	} ;

	pls1f_01_kind1[ew->sub_type]( ew ) ;
}

/*==============================================================*/
static void pls1f_01_t2( PLAYER_WORK *ew )
{
	static void ( *pls1f_01_kind2[] )( PLAYER_WORK *ew ) = {
		pls1f_01_wander, pls1f_01_wander, pls1f_01_wander,
	pls1f_01_wander, pls1f_01_wander, pls1f_01_follow,
	pls1f_01_follow, pls1f_01_wander, pls1f_01_wander,
	pls1f_01_follow, pls1f_01_wander, pls1f_01_follow,
	pls1f_01_chase, pls1f_01_chase, pls1f_01_follow,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_wander, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_follow, pls1f_01_follow, pls1f_01_wander,
	pls1f_01_follow,
	} ;

	pls1f_01_kind2[ew->sub_type]( ew ) ;
}

static void pls1f_01_wander_move( PLAYER_WORK *ew );
static void pls1f_01_dir_set( PLAYER_WORK *ew );
static void pls1f_01_spd_set1( PLAYER_WORK *ew );
static void pls1f_01_spd_set2( PLAYER_WORK *ew );
static void pls1f_01_char_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			It moves off at a new speed now and then, and		*/
/*			turns												*/
/*==============================================================*/
static void pls1f_01_wander( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		ew->timer0 = Rnd() & 15 ;
		pls1f_01_spd_set1( ew ) ;
		ew->timer1 = 2 ;
		pls1f_01_dir_set( ew ) ;
	}
	pls1f_01_wander_move( ew ) ;
}

/*==============================================================*/
/*			It draws in to its place at her side				*/
/*==============================================================*/
static void pls1f_01_follow( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_1 == 2 ){
		pls1f_01_wander_move( ew ) ;
		return ;
	}
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		ew->routine_2 = 1 ;
		pls1f_01_spd_set1( ew ) ;
		pls1f_01_dir_set( ew ) ;
		ew->timer0 = 7 ;
	}
	ew->pos.x += ( ew->spd.x + pl_w->pos.x - ew->pos.x ) / 8.0f ;
	ew->pos.y += ( ew->spd.y + pl_w->pos.y - ew->pos.y ) / 8.0f ;
	if( --ew->timer0 < 0 ) ew->routine_1 ++ ;
	pls1f_01_char_move( ew ) ;
}

static void pls1f_01_chase_init( PLAYER_WORK *ew );
static void pls1f_01_chase_fly( PLAYER_WORK *ew );
static void pls1f_01_chase_wait( PLAYER_WORK *ew );
static void pls1f_01_chase_next_init( PLAYER_WORK *ew );
static void pls1f_01_chase_next_fly( PLAYER_WORK *ew );
static void pls1f_01_chase_next_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls1f_01_chase( PLAYER_WORK *ew )
{
	static void ( *pls1f_01_chase_jmp[] )( PLAYER_WORK *ew ) = {
		pls1f_01_chase_init, pls1f_01_chase_fly, pls1f_01_chase_wait, pls1f_01_chase_next_init, pls1f_01_chase_next_fly,
		pls1f_01_chase_next_move,
	} ;

	pls1f_01_chase_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls1f_01_chase_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 1 ;
	pls1f_01_spd_set1( ew ) ;
	pls1f_01_dir_set( ew ) ;
	ew->timer0 = 60 ;
	pls1f_01_chase_fly( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1f_01_chase_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ( ew->spd.x + pl_w->pos.x - ew->pos.x ) / 8.0f ;
	ew->pos.y += ( ew->spd.y + pl_w->pos.y - ew->pos.y ) / 8.0f ;
	pls1f_01_chase_wait( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1f_01_chase_wait( PLAYER_WORK *ew )
{
	if( --ew->timer0 < 0 ) ew->routine_1 ++ ;
	if( ew->timer0 > 73 ) pls1f_01_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1f_01_chase_next_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->routine_2 = 1 ;
	pls1f_01_spd_set2( ew ) ;
	pls1f_01_dir_set( ew ) ;
	ew->timer0 = 1 ;
	pls1f_01_chase_next_fly( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls1f_01_chase_next_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ( ew->spd.x + pl_w->pos.x - ew->pos.x ) / 8.0f ;
	ew->pos.y += ( ew->spd.y + pl_w->pos.y - ew->pos.y ) / 8.0f ;
	pls1f_01_chase_next_move( ew ) ;
}

static void pls1f_01_anm_rate( PLAYER_WORK *ew );
static void pls1f_01_dir_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls1f_01_chase_next_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float d ;

	ew->timer1 = 2 ;
	pls1f_01_dir_set( ew ) ;
	ew->dir = ( pls1f_01_dir_tbl[Rnd() & 3] + ew->dir ) & 31 ;
	if( --ew->timer0 < 0 ) ew->routine_1 = 2 ;
	d = pl_w->pos.x + ew->spd.x - ew->pos.x + DOT_X( 4.0f ) ;
	if( d < 0.0f ) d = -d ;
	if( d < DOT_X( 8.0f ) ) ew->routine_1 = 2 ;
	pls1f_01_anm_rate( ew ) ;
	pls1f_01_dir_move( ew ) ;
	pls1f_01_char_move( ew ) ;
}

/*==============================================================*/
static void pls1f_01_wander_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float d ;

	if( --ew->timer1 < 0 ){
		ew->timer1 = 2 ;
		pls1f_01_dir_set( ew ) ;
		ew->dir = ( pls1f_01_dir_tbl[Rnd() & 3] + ew->dir ) & 31 ;
		if( --ew->timer0 < 0 ) ew->routine_1 = 0 ;
	}
	d = pl_w->pos.x + ew->spd.x - ew->pos.x + DOT_X( 4.0f ) ;
	if( d < 0.0f ) d = -d ;
	if( d < DOT_X( 8.0f ) ) ew->routine_1 = 0 ;
	pls1f_01_anm_rate( ew ) ;
	pls1f_01_dir_move( ew ) ;
	pls1f_01_char_move( ew ) ;
}

/*==============================================================*/
/*			How fast its animation goes, by how far it is		*/
/*			from where it is going								*/
/*==============================================================*/
static void pls1f_01_anm_rate( PLAYER_WORK *ew )
{
	static Sint16 pls1f_01_dir_spd[16] = {
		200, 200, 200, 200, 300, 300, 300, 400, 400, 400, 500, 500, 500, 600, 600, 700,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PLS1F_01_LOCAL_W *lw = PLS1F_01_LW( ew ) ;
	Float d = pl_w->pos.x + ew->spd.x - ew->pos.x ;

	if( d < 0.0f ) d = -d ;
	if( d > DOT_X( 256.0f ) ) d = 425.0f ;
	ew->routine_2 = ( Sint32 )( d / 1.6666666f / 16.0f ) & 15 ;
	lw->dir_spd = pls1f_01_dir_spd[ew->routine_2] ;
}

/*==============================================================*/
/*			The animation's next frame, held for as long as		*/
/*			its speed asks										*/
/*==============================================================*/
static void pls1f_01_char_move( PLAYER_WORK *ew )
{
	static Uint8 pls1f_01_cg_ctr[16] = {
		1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
	} ;

	if( !--ew->cg_ctr ){
		do {
			ew->cg_adrs = ew->cg_adrs + sizeof( EF_CHAR_DATA ) ;
			if( ew->cg_end_flag & CG_END ) ew->cg_adrs = ( Sint8 * )( ew->char_adrs + ( ( EF_CHAR_DATA * )ew->cg_adrs )->unk_00 ) ;
			EF_CG( ew ).data = *( EF_CHAR_DATA * )ew->cg_adrs ;
			EF_CG( ew ).unk_08 = 0 ;
			EF_CG( ew ).unk_0c = 0 ;
			EF_CG( ew ).unk_10 = 0 ;
			ew->cg_ctr = pls1f_01_cg_ctr[ew->routine_2] ;
		} while( !ew->cg_ctr ) ;
	}
}

static void pls1f_01_dir_anm( PLAYER_WORK *ew );

/*==============================================================*/
/*			Turn toward the place it is going: 32 steps to a	*/
/*			circle												*/
/*==============================================================*/
static void pls1f_01_dir_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	VEC3 to ;

	to.x = pl_w->pos.x + ew->spd.x ;
	to.y = pl_w->pos.y + ew->spd.y ;
	ew->dir = ( ( caldir_pos_256( &ew->pos, &to ) + 4 ) >> 3 ) & 31 ;
	pls1f_01_dir_anm( ew ) ;
}

/*==============================================================*/
/*			The animation that shows the way it faces			*/
/*==============================================================*/
static void pls1f_01_dir_anm( PLAYER_WORK *ew )
{
	static Uint8 pls1f_01_anm_tbl[32] = {
		0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
		4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 0,
	} ;

	if( !ew->dir || ew->dir >= 16 ) ew->disp_dir = 0 ;
	else ew->disp_dir = 1 ;
	if( ew->routine_3 != pls1f_01_anm_tbl[ew->dir] ){
		ew->routine_3 = pls1f_01_anm_tbl[ew->dir] ;
		ef_char_set( ew, 27, ew->routine_3 + 19 ) ;
	}
}

/*==============================================================*/
static void pls1f_01_dir_move( PLAYER_WORK *ew )
{
	PLS1F_01_LOCAL_W *lw = PLS1F_01_LW( ew ) ;

	dir_move( ew, lw->dir_spd ) ;
}

static void pls1f_01_spd_set( PLAYER_WORK *ew, Uint8 no );

/*==============================================================*/
/*			Its speed, for the kind after the one its master	*/
/*			shows												*/
/*==============================================================*/
static void pls1f_01_spd_set2( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;

	no = ( pl_w->cg_flag1 + 2 ) >> 1 ;
	pls1f_01_spd_set( ew, no ) ;
}

/*==============================================================*/
static void pls1f_01_spd_set1( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;

	no = pl_w->cg_flag1 >> 1 ;
	pls1f_01_spd_set( ew, no ) ;
}

/*==============================================================*/
/*			The next phase's speed from the type's table,		*/
/*			turned for the way she faces						*/
/*==============================================================*/
static void pls1f_01_spd_set( PLAYER_WORK *ew, Uint8 no )
{
	PLS1F_01_LOCAL_W *lw = PLS1F_01_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;

	lw->phase = ( lw->phase + 1 ) & 3 ;
	lw->spd_no = no * 8 + lw->phase * 2 ;
	ew->spd.x = lw->spd_tbl[lw->spd_no] ;
	ew->spd.y = *( lw->spd_tbl + lw->spd_no + 1 ) ;
	if( pl_w->disp_dir ) ew->spd.x = -ew->spd.x ;
}

/****************************************************************/
void pls1f_01_next( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/****************************************************************/
void pls1f_01_free( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls1f_01b.c" End of File	*/
