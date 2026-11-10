/* Cyclops's player effect 01 (character 06's player effect 01): nine kinds of picture, by type: the
   flash of his heavy punches' blast (0 standing, 1 crouching); the thing his entrance throws off
   (2) and the picture at his eyes as it falls (3); the thing a win pose throws, which bounces and
   stays (4); the marks where the beam of shl06_01 reaches the ground (5) and where a throw cuts it
   short (6); the picture of 3 again as he lands when tagged in (7); and the hit flash of shl06_02
   (8). */
#include "player.h"
#include "ef_char.h"

/* the beam's works of shl06_01 keep their own work at obj_mtx; these are the places read here */
typedef struct PLS06_01_TRAIL_W {
	Uint8	unk_00[0x10];
	VEC3	pos_a;			/* 10 */
	VEC3	pos_b;			/* 1c */
	Uint8	unk_28[0x2a - 0x28];
	Uint8	ground_flag;			/* 2a: set while the beam reaches the ground */
} PLS06_01_TRAIL_W;
#define PLS06_01_TRAIL( w )	( ( PLS06_01_TRAIL_W * )&( w )->obj_mtx )

/* this effect's own work, from obj_flag */
typedef struct PLS06_01_LOCAL_W {
	Uint16	char_no;	/* 00: the animation of his that the effect belongs to */
} PLS06_01_LOCAL_W;
#define PLS06_01_LW( w )	( ( PLS06_01_LOCAL_W * )&( w )->obj_flag )

static void pls06_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls06_01_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls06_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x601 ;
		PLS06_01_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

/*==============================================================*/
/*			A piece made by another, which it follows			*/
/*==============================================================*/
static PLAYER_WORK *pls06_01_part_set( PLAYER_WORK *ef_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( ef_w, W_PLS, LIST_BEHIND ) ) ){
		ew->work_func = pls06_01_main ;
		ew->master_w = ef_w->master_w ;
		ew->ef_w = ef_w ;
		ew->pos = ef_w->pos ;
		ew->type = no ;
		ew->ef_no = 0x601 ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl06_01_set2( PLAYER_WORK *ef_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->work_func = pls06_01_main ;
		ew->master_w = ef_w->master_w ;
		ew->ef_w = ef_w ;
		ew->pos = ef_w->pos ;
		ew->type = no ;
		ew->ef_no = 0x601 ;
	}
	return ew ;
}

static void pls06_01_init( PLAYER_WORK *ew );
static void pls06_01_move( PLAYER_WORK *ew );
static void pls06_01_end( PLAYER_WORK *ew );
static void pls06_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls06_01_main( PLAYER_WORK *ew )
{
	static void ( *pls06_01_jmp[] )( PLAYER_WORK *ew ) = {
		pls06_01_init, pls06_01_move, pls06_01_end, pls06_01_free,
	} ;

	pls06_01_jmp[ew->routine_0]( ew ) ;
}

static void pls06_01_flash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_eye_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_cut_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_t_kick_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls06_01_init( PLAYER_WORK *ew )
{
	static void ( *pls06_01_init_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls06_01_flash_init, pls06_01_flash_init, pls06_01_appear_init, pls06_01_eye_init, pls06_01_win_init,
		pls06_01_ground_init, pls06_01_cut_init, pls06_01_t_kick_init, pls06_01_hit_init,
	} ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	pls06_01_init_jmp[ew->type]( ew, pl_w ) ;
	pls06_01_move( ew ) ;
}

/*==============================================================*/
/*			Types 0 and 1: the flash, at his visor				*/
/*==============================================================*/
static void pls06_01_flash_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	if( pl_w->disp_dir ) ew->pos.x += 65.0f ;
	else ew->pos.x -= 65.0f ;
	if( !ew->type ) ew->pos.y += DOT_Y( 89.0f ) ;
	else ew->pos.y += 135.0f ;
	ew->sub_prio = -1 ;
	ef_char_set( ew, 27, 2 ) ;
}

/*==============================================================*/
static void pls06_01_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->sub_prio = -1 ;
	ef_char_set( ew, 27, 3 ) ;
}

/*==============================================================*/
static void pls06_01_eye_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->sub_prio = -1 ;
	ew->pos = pl_w->pos ;
	if( ew->disp_dir ) ew->pos.x += 30.0f ;
	else ew->pos.x -= 30.0f ;
	ew->pos.y += DOT_Y( 99.0f ) ;
	ef_char_set( ew, 27, 4 ) ;
}

/*==============================================================*/
static void pls06_01_win_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs ;

	ew->sub_prio = -1 ;
	ew->pos = pl_w->pos ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ew->spd.x = DOT_X( 2.0f ) ;
	ew->acc.x = 0.0f ;
	ew->pos.y += DOT_Y( 88.0f ) ;
	ofs = 80.0f ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ofs = -80.0f ;
	}
	ew->pos.x += ofs ;
	ef_char_set( ew, 27, 5 ) ;
}

/*==============================================================*/
static void pls06_01_ground_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->sub_prio = -16 ;
	ef_char_set( ew, 27, 6 ) ;
}

/*==============================================================*/
/*			The cut's turn, by the way the beam points			*/
/*==============================================================*/
static void pls06_01_cut_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 pls06_01_turn_no[] = {
		0, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 2, 2, 1, 1, 0,
	} ;
	static Uint16 pls06_01_turn_tbl[] = {
		0x4000, 0x1f00, 0, 0xe100, 0xc000, 0,
	} ;

	ew->sub_prio = -120 ;
	ew->disp_dir ^= 1 ;
	ew->rot[2] = pls06_01_turn_tbl[pls06_01_turn_no[ew->ef_w->dir]] ;
	ef_char_set( ew, 27, 7 ) ;
}

/*==============================================================*/
static void pls06_01_t_kick_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->sub_prio = -1 ;
	ew->pos = pl_w->pos ;
	ew->spd.y = DOT_Y( 97.0f ) ;
	ew->pos.y += ew->spd.y ;
	ef_char_set( ew, 27, 4 ) ;
}

/*==============================================================*/
static void pls06_01_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->sub_prio = -16 ;
	ef_char_set( ew, 27, 8 ) ;
}

static void pls06_01_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_eye_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_cut_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_t_kick_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls06_01_move( PLAYER_WORK *ew )
{
	static void ( *pls06_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls06_01_flash_move, pls06_01_flash_move, pls06_01_appear_move, pls06_01_eye_move, pls06_01_win_move,
		pls06_01_ground_move, pls06_01_cut_move, pls06_01_t_kick_move, pls06_01_hit_move,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	pls06_01_move_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls06_01_flash_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls06_01_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_appear_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_appear_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The entrance's thing: shown by the animation,		*/
/*			thrown out, and falling to the ground				*/
/*==============================================================*/
static void pls06_01_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls06_01_appear_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls06_01_appear_wait, pls06_01_appear_up, pls06_01_appear_fall,
	} ;

	pls06_01_appear_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls06_01_appear_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs ;

	ew->disp_flag = 1 ;
	switch( pl_w->cg_type ){
	case 0 :
		ew->disp_flag = 0 ;
		break ;
	case 3 :
		ew->routine_1 ++ ;
		ew->acc.x = 0.0f ;
		ew->spd.x = 1.6666666f ;
		if( !ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		ew->spd.y = DOT_Y( 6.0f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
		break ;
	default :
		ew->pos = pl_w->pos ;
		if( pl_w->cg_type == 1 ){
			ofs = 0.0f ;
			ew->pos.y += 240.0f ;
		}
		else{
			ofs = -DOT_X( 8.0f ) ;
			ew->pos.y += DOT_Y( 128.0f ) ;
		}
		if( !ew->disp_dir ) ofs = -ofs ;
		ew->pos.x += ofs ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void pls06_01_appear_up( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	if( ( ew->spd.y += ew->acc.y ) > 0.0f ) return ;
	ew->routine_1 ++ ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -2.142857f ;
	ew->acc.y = -DOT_Y( 0.125f ) ;
	ef_char_move( ew ) ;
	ew->timer0 = 0 ;
}

/*--------------------------------------------------------------*/
static void pls06_01_appear_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->timer0 && pl_w->ground_y + DOT_Y( 96.0f ) > ew->pos.y ){
		ew->timer0 = 1 ;
		pls06_01_part_set( ew, 3 ) ;
	}
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void pls06_01_eye_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void pls06_01_win_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_win_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls06_01_win_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The win pose's thing, while he is in Char_Win		*/
/*==============================================================*/
static void pls06_01_win_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls06_01_win_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls06_01_win_fall, pls06_01_win_bound, pls06_01_win_stay,
	} ;

	if( pl_w->char_no.b.grp != Char_Win ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls06_01_win_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls06_01_win_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ew->spd.y = DOT_Y( 2.0f ) ;
		ew->acc.y = -DOT_Y( 0.25f ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls06_01_win_bound( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y ;
		ef_char_set( ew, 27, 5 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls06_01_win_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
/*			The mark where the beam reaches the ground			*/
/*==============================================================*/
static void pls06_01_ground_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w ;
	PLS06_01_TRAIL_W *tw ;

	ew->disp_flag = 0 ;
	ef_w = ew->ef_w ;
	if( ef_w->work_kind != W_SHL || ef_w->ef_no != 0x601 || ef_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	tw = PLS06_01_TRAIL( ef_w ) ;
	if( tw->ground_flag ){
		ew->pos = tw->pos_a ;
		ef_char_move( ew ) ;
		ew->disp_flag = 1 ;
	}
}

/*==============================================================*/
static void pls06_01_cut_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w ;
	PLS06_01_TRAIL_W *tw ;
	Float ofs ;

	ew->disp_flag = 0 ;
	ef_w = ew->ef_w ;
	tw = PLS06_01_TRAIL( ef_w ) ;
	if( ef_w->work_kind != W_SHL || ef_w->ef_no != 0x601 || ef_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos = tw->pos_b ;
	if( pl_w->catch_no == 5 ){
		ofs = DOT_X( 32.0f ) ;
		if( pl_w->disp_dir ) ofs = -DOT_X( 32.0f ) ;
		ew->pos.x += ofs ;
	}
	ef_char_move( ew ) ;
	ew->disp_flag = 1 ;
}

/*==============================================================*/
static void pls06_01_t_kick_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( PLS06_01_LW( ew )->char_no == pl_w->char_no.w ){
		ew->pos = pl_w->pos ;
		ew->pos.y += ew->spd.y ;
		if( ef_char_move( ew ) >= 0 ) return ;
	}
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void pls06_01_hit_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls06_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls06_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls06_01.c" End of File	*/
