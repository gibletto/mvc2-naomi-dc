/* War Machine's effect 05 (character 2e's shot 05): the War Destroyer's bombs, which go up from him
   out of the screen and come down on the opponent, bursting on the ground or on a hit (type 0); the
   hit beside him of his normal attacks, launcher, snapback and throws, laid out by a table (type
   1); and the Shoulder Cannon's shots, which fly forward from him and burst (type 2). */
#include "player.h"
#include "pl_char.h"
#include "pl2e_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL2E_05_LOCAL_W {
	Uint16	char_no;	/* 000: the master's animation it lasts for (type 1) */
	Sint16	x;		/* 002: where it lands, or its place beside the master */
	Sint16	y;		/* 004 */
} SHL2E_05_LOCAL_W;
#define SHL2E_05_LW( w )	( ( SHL2E_05_LOCAL_W * )&( w )->obj_flag )

/* what each type copies from the player */
#define SHL2E_05_COPY( ew, pl_w ) \
	CHAR_PART_COPY( ew, pl_w ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( pl_w )->pl_no ; \
	( ew )->pl_type = ( pl_w )->pl_type ; \
	( ew )->scale.x = ( pl_w )->scale.x ; \
	( ew )->scale.y = ( pl_w )->scale.y ; \
	( ew )->atck_level = ( pl_w )->atck_level ; \
	( ew )->work_no = ( pl_w )->work_no ; \
	( ew )->col_cnt = ( pl_w )->col_cnt ; \
	( ew )->scale = ( pl_w )->scale

/* where the Shoulder Cannon's shots start from the master: x and y, standing, in the air and crouching */
static Sint16 shl2e_05_pos_tbl[] = {
	3, 87, -10, 94, -23, 56,
} ;

static void shl2e_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl2e_05_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;
	SHL2E_05_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2e_05_main ;
		ew->ef_no = 0x2e05 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
		lw = SHL2E_05_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl2e_05_destroyer_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_hit_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_05_main( PLAYER_WORK *ew )
{
	static void ( *shl2e_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_05_destroyer_main, shl2e_05_hit_main, shl2e_05_shot_main,
	} ;

	shl2e_05_jmp[ew->type]( ew, ew->master_w ) ;
}

/* type 0 */
static void shl2e_05_destroyer_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_destroyer_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_destroyer_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_05_destroyer_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_05_destroyer_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_05_destroyer_init, shl2e_05_destroyer_move, shl2e_05_destroyer_burst, shl2e_05_free,
	} ;

	shl2e_05_destroyer_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/* how far a bomb lands from where it is aimed, by a random 0 to 15 */
static Sint16 shl2e_05_ofs_tbl[] = {
	-64, -48, -32, -16, 16, 32, 48, 64, -8, -56, -40, -24, 24, 40, 56, 8,
} ;

/*==============================================================*/
static void shl2e_05_destroyer_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tw = pl_w->target_w ;
	SHL2E_05_LOCAL_W *lw = SHL2E_05_LW( ew ) ;

	ew->routine_0 ++ ;
	SHL2E_05_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->atck_no = pl2e_at_destroyer ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_top = ew->size_bottom = 68 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->pos.x += shl2e_05_ofs_tbl[Rnd() & 15] ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 16.0f ) ;
	ew->acc.y = DOT_Y( 0.125f ) ;
	lw->x = tw->pos.x ;
	lw->y = pl_w->ground_y + 960.0f ;
	pl_char_set( ew, Char_Effect, ew->sub_type + 32 ) ;
	shl2e_05_destroyer_move( ew, pl_w ) ;
}

static void shl2e_05_destroyer_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_destroyer_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl2e_05_destroyer_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_05_destroyer_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_05_destroyer_up, shl2e_05_destroyer_fall,
	} ;

	if( pl_w->char_no.b.grp != Char_Super || pl_w->routine_1 ) shl2e_05_free( ew, pl_w ) ;
	else {
		ew->prio = pl_w->prio ;
		shl2e_05_destroyer_move_jmp[ew->routine_1]( ew, pl_w ) ;
	}
}

/*==============================================================*/
/*			Goes up from him until it is 448 dots above the		*/
/*			ground, then is put over the target to come down	*/
/*==============================================================*/
static void shl2e_05_destroyer_up( register PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	register SHL2E_05_LOCAL_W *lw = SHL2E_05_LW( ew ) ;

	if( !scr_in_y_ck( ew ) ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->pos.y > lw->y ){
		ew->routine_1 ++ ;
		ew->atck_no = pl2e_at_destroyer_2 ;
		HIT_RESET( ew ) ;
		ew->pos.x = lw->x ;
		ew->pos.x += shl2e_05_ofs_tbl[Rnd() & 15] ;
		ew->spd.y = -DOT_Y( 16.0f ) ;
		ew->acc.y = -DOT_Y( 0.125f ) ;
		pl_char_set( ew, Char_Effect, ew->sub_type + 34 ) ;
		return ;
	}
	if( ew->hit_box_no ){ ew->routine_0 ++ ; return ; }
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2e_05_destroyer_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !scr_in_y_ck( ew ) ) ew->disp_flag = 0 ;
	else ew->disp_flag = 1 ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->pos.y < pl_w->ground_y ){
		ew->pos.y = pl_w->ground_y ;
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->hit_box_no ){ ew->routine_0 ++ ; return ; }
	hit_work_set( ew ) ;
	if( ew->hit_flag ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl2e_05_destroyer_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	burst_set( ew, &ew->pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/* type 1 */
static void shl2e_05_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_05_hit_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_05_hit_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_05_hit_init, shl2e_05_hit_move, shl2e_05_end, shl2e_05_free,
	} ;

	shl2e_05_hit_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/* each hit: its place beside the master, its attack and its animation */
typedef struct SHL2E_05_HIT {
	Sint16	x, y;
	Sint16	atck_no;
	Sint16	char_no;
} SHL2E_05_HIT;

static SHL2E_05_HIT shl2e_05_hit_tbl[] = {
	{ -39, 63, 2, 38 }, { -76, 6, 10, 39 }, { -116, 55, 4, 40 }, { -20, 82, 13, 43 },
	{ 0, 0, 5, 45 }, { 0, 0, 11, 46 }, { 0, 0, 12, 47 }, { 0, 0, 255, 48 },
	{ -39, 63, 65, 38 }, { -82, 70, 255, 40 },
} ;

/*==============================================================*/
static void shl2e_05_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL2E_05_LOCAL_W *lw = SHL2E_05_LW( ew ) ;

	ew->routine_0 ++ ;
	SHL2E_05_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	lw->x = ( int )shl2e_05_hit_tbl[ew->sub_type].x * 1.6666666f ;
	lw->y = 2.142857f * shl2e_05_hit_tbl[ew->sub_type].y ;
	if( ew->disp_dir ) lw->x = -lw->x ;
	ew->atck_no = shl2e_05_hit_tbl[ew->sub_type].atck_no ;
	if( ew->atck_no != 0xff ){
		ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
	}
	pl_char_set( ew, Char_Effect, shl2e_05_hit_tbl[ew->sub_type].char_no ) ;
	shl2e_05_hit_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2e_05_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL2E_05_LOCAL_W *lw = SHL2E_05_LW( ew ) ;

	if( pl_w->char_no.w != lw->char_no ){ shl2e_05_free( ew, pl_w ) ; return ; }
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.x += lw->x ;
	ew->pos.y += lw->y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl2e_05_end( ew, pl_w ) ;
		return ;
	}
	if( ew->atck_no != 0xff ) hit_work_set( ew ) ;
}

/* type 2 */
static void shl2e_05_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_shot_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2e_05_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2e_05_shot_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2e_05_shot_main_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2e_05_shot_init, shl2e_05_shot_move, shl2e_05_burst, shl2e_05_free,
	} ;

	shl2e_05_shot_main_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2e_05_shot_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 rnd ;

	ew->routine_0 ++ ;
	SHL2E_05_COPY( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->sub_prio = 1 ;
	ew->size_front = ew->size_back = 34 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->disp_dir ) ew->pos.x -= DOT_X( shl2e_05_pos_tbl[ew->sub_type * 2] ) ;
	else ew->pos.x += DOT_X( shl2e_05_pos_tbl[ew->sub_type * 2] ) ;
	ew->pos.y += DOT_Y( shl2e_05_pos_tbl[ew->sub_type * 2 + 1] ) ;
	rnd = Rnd() & 31 ;
	ew->pos.y += 16 - rnd ;
	if( !pl_w->atck_level ) ew->spd.x = -DOT_X( 14.0f ) ;
	else ew->spd.x = -DOT_X( 16.0f ) ;
	ew->acc.x = -DOT_X( 0.125f ) ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	}
	ew->atck_no = pl2e_at_cannon ;
	ew->hit_flag = ew->atck_attr = 0 ;
	ew->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	pl_char_set( ew, Char_Effect, 41 ) ;
	shl2e_05_shot_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2e_05_shot_move( register PLAYER_WORK *ew, register PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	if( ew->hit_box_no ){ ew->routine_0 ++ ; return ; }
	hit_work_set( ew ) ;
	if( ew->hit_flag ){ ew->routine_0 ++ ; return ; }
	if( !scr_in_x_ck( ew ) ) shl2e_05_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2e_05_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	burst_set( ew, &ew->pos, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2e_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2e_05.c" End of File	*/
