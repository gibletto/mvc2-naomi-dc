/* Mega Man's and Roll's Rock Ball (character 1c's shot 02): type 0 is the ball set beside the
   character, which a kick sends off to bounce off the screen's edges; types 1 to 9 follow it in a
   chain, each placed on the one before. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

static void shl1c_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl1c_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_02_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		lw->ball_end_flag = 0 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The effect breaks up where it is					*/
/*==============================================================*/
static void shl1c_02_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	ew->routine_0 ++ ;
	lw->ball_flag = 0 ;
	if( !ew->disp_dir ) ofs.x = ew->pos.x + 0.0f ;
	else ofs.x = ew->pos.x + -0.0f ;
	ofs.y = ew->pos.y + 0.0f ;
	burst_set( ew, &ofs, 1, 8 ) ;
	pl_se_req( ew, 73 ) ;
}

static void shl1c_02_init( PLAYER_WORK *ew );
static void shl1c_02_move( PLAYER_WORK *ew );
static void shl1c_02_end( PLAYER_WORK *ew );
static void shl1c_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_02_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_02_init, shl1c_02_move, shl1c_02_end, shl1c_02_free,
	} ;

	shl1c_02_jmp[ew->routine_0]( ew ) ;
}

/* the animation of each kind */
static Uint8 shl1c_02_char_tbl[] = {
	20, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
} ;

static void shl1c_02_chain( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c00 ;
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
	if( ew->type ){
		if( ew->type < 10 ) shl1c_02_chain( ew, pl_w ) ;
	}
	pl_char_set( ew, Char_Effect, shl1c_02_char_tbl[ew->type] ) ;
	shl1c_02_move( ew ) ;
}

static void shl1c_02_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_02_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->type ) shl1c_02_b_move( ew, pl_w ) ;
	else shl1c_02_a_move( ew, pl_w ) ;
}

static void shl1c_02_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_02_a_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_02_a_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_02_a_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_02_a_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_02_a_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_02_a_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_02_a_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_02_a_init, shl1c_02_a_fall, shl1c_02_a_wait, shl1c_02_a_fly, shl1c_02_a_out, shl1c_02_a_end,
	} ;

	if( pl_w->routine_0 >= 2 || ew->pl_type != pl_w->pl_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	ew->timer1 ++ ;
	shl1c_02_a_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It starts beside the character						*/
/*==============================================================*/
static void shl1c_02_a_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC2 ofs ;

	ew->routine_1 ++ ;
	ew->size_top = 8 ;
	ew->size_bottom = 8 ;
	ew->size_front = 8 ;
	ew->size_back = 8 ;
	if( pl_w->pl_type == PL_MEGAMAN ){
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 44.0f ) ;
	} else {
		ofs.x = -DOT_X( 32.0f ) ;
		ofs.y = DOT_Y( 37.0f ) ;
	}
	if( !pl_w->disp_dir ){
		ew->pos.x = pl_w->pos.x + ofs.x ;
		ew->spd.x = -2.5f ;
	} else {
		ew->pos.x = pl_w->pos.x - ofs.x ;
		ew->spd.x = 2.5f ;
	}
	ew->pos.y = pl_w->pos.y + ofs.y ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	ew->prio = 11 ;
	shl1c_02_a_fall( ew, pl_w ) ;
}

/*==============================================================*/
/*			It falls to the floor								*/
/*==============================================================*/
static void shl1c_02_a_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= pl_w->ground_y + 8.0f ){
		ew->routine_1 ++ ;
		ew->pos.y = pl_w->ground_y + 8.0f ;
	}
	pl_char_move( ew ) ;
}

/* how fast it is thrown, by the character's stance */
static Float shl1c_02_spd_tbl[][2] = {
	{ -DOT_X( 7.0f ), DOT_Y( 4.0f ) },
	{ -DOT_X( 8.5f ), DOT_Y( 3.0f ) },
} ;

/* and by the strength of the attack that hits it */
static Float shl1c_02_hit_tbl[][2] = {
	{ -DOT_X( 8.0f ), 0.0f },
	{ -DOT_X( 7.0f ), -DOT_Y( 4.0f ) },
	{ -DOT_X( 7.0f ), DOT_Y( 4.0f ) },
} ;

static int shl1c_02_box_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It waits for a hit from the character's attack		*/
/*==============================================================*/
static void shl1c_02_a_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	if( pl_w->team_no || lw->ball_end_flag || vs_sys_work.vs_st >= VS_END_WAIT ){
		shl1c_02_burst( ew, pl_w ) ;
		return ;
	}
	if( pl_w->char_no.b.grp == Char_Stand_K || pl_w->char_no.b.grp == Char_Crouch_K ||
		pl_w->char_no.w == 0x1403 || pl_w->char_no.w == 0x150f ){
		if( shl1c_02_box_ck( ew, pl_w ) ){
			ew->routine_1 ++ ;
			lw->ball_flag = 0xff ;
			ew->sub_type = 0 ;
			ew->timer0 = 8 ;
			ew->spd.x = shl1c_02_spd_tbl[pl_w->pl_stance][0] ;
			ew->spd.y = shl1c_02_spd_tbl[pl_w->pl_stance][1] ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
			if( ( ew->disp_dir = pl_w->disp_dir ) ) ew->spd.x = -ew->spd.x ;
			shl1c_02_chain( ew, pl_w ) ;
			ofs.x = ofs.y = ofs.z = 0.0f ;
			hit_mark_set( ew, &ofs, 0 ) ;
			pl_se_req( ew, 0 ) ;
			ew->atck_flag = HIT_ON | HIT_OBJ ;
			ew->dm_flag = HIT_ON | HIT_OBJ ;
			ew->atck_no = pl1c_at_rockball ;
			HIT_RESET( ew ) ;
			pl_char_set( ew, Char_Effect, 19 ) ;
		}
	}
}

static void shl1c_02_bounce( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It is thrown, and bounces							*/
/*==============================================================*/
static void shl1c_02_a_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
	VEC3 ofs ;

	if( lw->ball_end_flag || ew->hit_box_no ){
		shl1c_02_burst( ew, pl_w ) ;
		return ;
	}
	if( ew->hit_flag && ew->sub_type < 3 ) ew->sub_type = 2 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->timer0 ) ew->timer0 -- ;
	else if( pl_w->char_no.b.grp == Char_Jump_K && shl1c_02_box_ck( ew, pl_w ) ){
		ew->sub_type ++ ;
		ew->timer0 = 8 ;
		ew->spd.x = shl1c_02_hit_tbl[pl_w->atck_level][0] ;
		ew->spd.y = shl1c_02_hit_tbl[pl_w->atck_level][1] ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		if( ( ew->disp_dir = pl_w->disp_dir ) ) ew->spd.x = -ew->spd.x ;
		ofs.x = ofs.y = ofs.z = 0.0f ;
		hit_mark_set( ew, &ofs, 0 ) ;
		pl_voice_req( ew, 31 ) ;
		ew->atck_no = pl1c_at_rockball ;
		HIT_RESET( ew ) ;
		if( ew->sub_type >= 3 ){
			ew->routine_1 ++ ;
			goto move ;
		}
	}
	shl1c_02_bounce( ew, pl_w ) ;
	if( ew->sub_type >= 3 ) ew->routine_1 ++ ;
move:
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			It flies on until it leaves the screen				*/
/*==============================================================*/
static void shl1c_02_a_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;

	if( ew->hit_box_no ){
		shl1c_02_burst( ew, pl_w ) ;
		return ;
	}
	if( !scr_in_ck( ew ) ){
		ew->routine_1 ++ ;
		lw->ball_flag = 0 ;
		ew->timer0 = 7 ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_02_a_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void shl1c_02_b_follow( PLAYER_WORK *ew );

/*==============================================================*/
/*			A link of the chain rides on the one before			*/
/*==============================================================*/
static void shl1c_02_b_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *bw = ew->ef_w ;

	if( pl_w->routine_0 >= 2 || bw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1c_02_b_follow( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The next link of the chain							*/
/*==============================================================*/
static void shl1c_02_chain( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *cw ;

	if( ( cw = work_get( 0, W_SHL, LIST_TOP ) ) ){
		cw->work_func = shl1c_02_main ;
		cw->master_w = pl_w ;
		cw->type = ew->type + 1 ;
		cw->ef_w = ew ;
		cw->sub_prio = ew->sub_prio + 1 ;
		ew->timer1 = 0 ;
	}
}

/*==============================================================*/
/*			It turns round at the screen's edges and the		*/
/*			floor												*/
/*==============================================================*/
static void shl1c_02_bounce( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int n = 0 ;

	if( ew->spd.x < 0.0f && 1 ){
		if( scr_work.scr_left + DOT_X( 16.0f ) > ew->pos.x ){
			ew->spd.x = -ew->spd.x ;
			n ++ ;
		}
	} else if( ew->pos.x > scr_work.scr_right - DOT_X( 16.0f ) ){
		ew->spd.x = -ew->spd.x ;
		n ++ ;
	}
	if( ew->spd.y >= 0.0f && 1 ){
		if( ew->pos.y > scr_work.scr_top - DOT_Y( 12.0f ) ){
			ew->spd.y = -ew->spd.y ;
			n ++ ;
		}
	} else if( pl_w->ground_y + DOT_Y( 8.0f ) > ew->pos.y ){
		ew->spd.y = -ew->spd.y ;
		n ++ ;
	}
	if( n ){
		ew->sub_type ++ ;
		ew->atck_no = pl1c_at_rockball ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_voice_req( ew, 31 ) ;
	}
}

/*==============================================================*/
static void shl1c_02_b_follow( PLAYER_WORK *ew )
{
	PLAYER_WORK *bw = ew->ef_w ;

	ew->pos.x = bw->pos.x ;
	ew->pos.y = bw->pos.y ;
}

/*==============================================================*/
/*			Whether the effect's box and the character's		*/
/*			overlap												*/
/*==============================================================*/
static int shl1c_02_box_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	HIT_BOX *pb ;
	HIT_BOX *eb ;
	Float dx ;
	Float px ;
	Float dy ;
	Float py ;

	eb = &ew->hit_box_tbl[( Uint16 )ew->frame_box->body_box0] ;
	dx = DOT_X( eb->x ) ;
	pb = &pl_w->hit_box_tbl[( Uint16 )pl_w->frame_box->atck_box] ;
	px = DOT_X( pb->x ) ;
	if( ew->disp_dir ) dx = -dx ;
	dx += ew->pos.x ;
	if( pl_w->disp_dir ) px = -px ;
	px = px + pl_w->pos.x ;
	dx -= px ;
	if( dx < 0.0f ) dx = -dx ;
	if( ( px = DOT_X( pb->w ) ) == 0.0f ) return 0 ;
	px += DOT_X( eb->w ) ;
	if( dx >= px ) return 0 ;
	dy = DOT_Y( eb->y ) ;
	dy = -dy ;
	dy += ew->pos.y ;
	py = DOT_Y( pb->y ) ;
	py = -py ;
	py += pl_w->pos.y ;
	dy -= py ;
	if( dy < 0.0f ) dy = -dy ;
	if( ( py = DOT_Y( pb->h ) ) == 0.0f ) return 0 ;
	py += DOT_Y( eb->h ) ;
	if( dy < py ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_02.c" End of File	*/
