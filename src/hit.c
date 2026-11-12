/* The hit check: each side's attacks against the other side's works, box against box, the
   throw checks and the push between players. */
#include "player.h"

extern void clash_mark_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/* attacks (atck_flag) against works (dm_flag) by their kinds: 1 where they never meet */
static const Uint8 hit_ng_tbl[4][4] = {
	{ 0, 1, 0, 0 },
	{ 0, 0, 0, 1 },
	{ 0, 0, 0, 1 },
	{ 0, 1, 1, 1 },
} ;

/* how far each of the struck work's four boxes overlaps the attack */
static Float hit_len0, hit_len1, hit_len2, hit_len3;

static void hit_ck_all( void );
static void hit_ck_side( int side );
static void hit_ck_work( PLAYER_WORK *at_w, PLAYER_WORK *dm_w );
static void hit_box_ck_all( PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Uint16 no );
static void hit_set( PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Uint16 no );
static void hit_data_set( PLAYER_WORK *at_w, PLAYER_WORK *dm_w );
static void hit_box_ck( PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Uint16 at_no, Uint16 dm_no, Float *len );
static void hit_work_next( void );
static int catch_stance_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 no );
static int catch_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 no );
static int catch_box_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 at_no, Sint16 dm_no );
static void pl_hit_ck( void );
static void push_ck_all( void );
static void push_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em );

/****************************************************************/
void hit_ck0( void )
{
	push_ck_all() ;
	if( !vs_sys_w->game_stop ){
		hit_ck_all() ;
		pl_hit_ck() ;
	}
	hit_work_next() ;
}

/*==============================================================*/
static void hit_ck_all( void )
{
	hit_ck_side( 0 ) ;
	hit_ck_side( 1 ) ;
}

/*==============================================================*/
static void hit_ck_side( int side )
{
	PLAYER_WORK *at_w, *dm_w ;
	Sint8 i, j ;
	Uint8 at, dm ;

	for( i = hit_work_cnt[side] - 1 ; i >= 0 ; i -- ){
		at_w = hit_work_w[side][i] ;
		if( at_w->hit_flag || !( at = at_w->atck_flag ) ) continue ;
		at = ( at & HIT_KIND_MASK ) >> 1 ;
		for( j = hit_work_cnt[side ^ 1] - 1 ; j >= 0 ; j -- ){
			dm_w = hit_work_w[side ^ 1][j] ;
			if( !dm_w->hit_box_no && ( dm = dm_w->dm_flag ) ){
				dm = ( dm & HIT_KIND_MASK ) >> 1 ;
				if( !hit_ng_tbl[at][dm] ) hit_ck_work( at_w, dm_w ) ;
			}
		}
	}
}

/*==============================================================*/
static void hit_ck_work( PLAYER_WORK *at_w, PLAYER_WORK *dm_w )
{
	FRAME_BOX *ab ;
	Uint16 no ;
	Uint16 mask = 0x1fff ;

	hit_len0 = 0.0f ;
	hit_len1 = 0.0f ;
	hit_len2 = 0.0f ;
	hit_len3 = 0.0f ;
	ab = at_w->frame_box ;
	if( ( no = ab->atck_box & mask ) ){
		hit_box_ck_all( at_w, dm_w, no ) ;
		if( dm_w->hit_box_no ){
			hit_set( at_w, dm_w, no ) ;
			return ;
		}
	}
	if( ab->atck_box2 > 0 ){
		hit_box_ck_all( at_w, dm_w, ab->atck_box2 & mask ) ;
		if( dm_w->hit_box_no ) hit_set( at_w, dm_w, ab->atck_box2 & mask ) ;
	}
}

/*==============================================================*/
/*			The attack box against each of the struck work's	*/
/*			four boxes											*/
/*==============================================================*/
static void hit_box_ck_all( register PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Uint16 no )
{
	FRAME_BOX *db = dm_w->frame_box ;
	Uint16 d ;
	Float *len ;

	if( ( d = db->body_box0 & BOX_MASK ) ){
		len = &hit_len0 ;
		hit_box_ck( at_w, dm_w, no, d, len ) ;
	}
	if( ( d = db->body_box1 & BOX_MASK ) ){
		len = &hit_len1 ;
		hit_box_ck( at_w, dm_w, no, d, len ) ;
	}
	if( ( d = db->body_box2 & BOX_MASK ) ){
		len = &hit_len2 ;
		hit_box_ck( at_w, dm_w, no, d, len ) ;
	}
	if( ( Sint16 )( d = db->body_box3 & BOX_MASK ) > 0 ){
		len = &hit_len3 ;
		hit_box_ck( at_w, dm_w, no, d, len ) ;
	}
	if( 0 ) dm_w->frame_box = db ;
}

/*==============================================================*/
/*			The struck work's box is the one the attack			*/
/*			overlaps most; the fourth box's upper bits are		*/
/*			read after they are masked off, so its type is		*/
/*			always 4											*/
/*==============================================================*/
static void hit_set( PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Uint16 no )
{
	FRAME_BOX *db = dm_w->frame_box ;
	Uint8 type ;
	Uint16 box ;
	Float len ;
	int flag ;
	Uint16 kind ;

	at_w->at_box_no = no ;
	dm_w->dm_hit_w = at_w ;
	at_w->em_w = dm_w ;
	flag = dm_w->dm_flag ;
	flag &= HIT_KIND_MASK ;
	flag |= HR_HIT ;
	at_w->hit_flag |= flag ;
	type = 1 ;
	box = db->body_box0 & BOX_MASK ;
	len = hit_len0 ;
	if( len < hit_len1 ){
		box = db->body_box1 & BOX_MASK ;
		type = 2 ;
		len = hit_len1 ;
	}
	if( len < hit_len2 ){
		box = db->body_box2 & BOX_MASK ;
		type = 3 ;
		len = hit_len2 ;
	}
	if( len < hit_len3 ){
		box = db->body_box3 & BOX_MASK ;
		kind = box ;
		kind &= 0x6000 ;
		kind = kind >> 13 ;
		kind |= 4 ;
		box &= 0x1fff ;
		type = kind ;
	}
	dm_w->hit_box_no = type ;
	dm_w->dm_box_no = box ;
	hit_data_set( at_w, dm_w ) ;
}

/*==============================================================*/
/*			The hit's data, for the attacker and for the		*/
/*			one it hit											*/
/*==============================================================*/
static void hit_data_set( PLAYER_WORK *at_w, PLAYER_WORK *dm_w )
{
	HIT_DATA *hd ;
	PLAYER_WORK *ow ;
	HIT_DATA *dh ;
	Uint8 no ;

	if( !( at_w->atck_flag & HIT_PL ) ){
		hd = at_w->hit_data_tbl ;
		no = at_w->atck_no ;
		no &= 0x7f ;
		hd = hd + no ;
		at_w->hit_data_adrs = hd ;
		at_w->hit_stop = hd->hit_stop ;
		if( at_w->atck_flag & HIT_OWNER ){
			ow = &pl_work[at_w->work_no] ;
			ow->hit_flag = at_w->hit_flag ;
			ow->em_w = dm_w ;
		}
	}
	if( !( dm_w->dm_flag & HIT_PL ) ){
		hd = at_w->hit_data_tbl ;
		no = at_w->atck_no ;
		no &= 0x7f ;
		hd = hd + no ;
		dm_w->hit_data_adrs = hd ;
#if BUILD_DATE >= 20000529
		dm_w->hit_stop = hd->hit_stop ;
#else
		dh->hit_stop = at_w->hit_stop ;	/* dh is never set: the hit stop is lost */
#endif
		ow = &pl_work[at_w->work_no] ;
		dm_w->dm_at_w = ow ;
		dm_w->dm_atck_no = at_w->atck_no ;
		if( dm_w->dm_flag & HIT_OWNER ){
			ow = &pl_work[dm_w->work_no] ;
			ow->dm_hit_w = at_w ;
			ow->hit_box_no = dm_w->hit_box_no ;
		}
	}
}

/*==============================================================*/
/*			Whether the two boxes overlap, and by how much		*/
/*			(box sizes are scaled by 5/3 across and 15/7		*/
/*			down)												*/
/*==============================================================*/
static void hit_box_ck( PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Uint16 at_no, Uint16 dm_no, Float *len )
{
	HIT_BOX *ab, *db ;
	register Float d0 ;
	Float d1 ;

	ab = &at_w->hit_box_tbl[at_no] ;
	d0 = ab->x * at_w->scale.x * 1.6666666f ;
	db = &dm_w->hit_box_tbl[dm_no] ;
	d1 = db->x * dm_w->scale.x * 1.6666666f ;
	if( at_w->disp_dir ) d0 = -d0 ;
	d0 += at_w->pos.x ;
	if( dm_w->disp_dir ) d1 = -d1 ;
	d1 += dm_w->pos.x ;
	d0 -= d1 ;
	if( d0 < 0.0f ) d0 = -d0 ;
	if( ( d1 = db->w * dm_w->scale.x * 1.6666666f ) == 0.0f ) return ;
	d1 += ab->w * at_w->scale.x * 1.6666666f ;
	if( !( d1 > d0 ) ) return ;
	d0 = ab->y * at_w->scale.y * 2.142857f ;
	d0 = -d0 ;
	d0 += at_w->pos.y ;
	d1 = db->y * dm_w->scale.y * 2.142857f ;
	d1 = -d1 ;
	d1 += dm_w->pos.y ;
	d0 -= d1 ;
	if( d0 < 0.0f ) d0 = -d0 ;
	if( ( d1 = db->h * dm_w->scale.y * 2.142857f ) == 0.0f ) return ;
	d1 += ab->h * at_w->scale.y * 2.142857f ;
	if( !( d1 > d0 ) ) return ;
	*len = d1 - d0 ;
	dm_w->hit_box_no = 1 ;
}

/****************************************************************/
void hit_work_init( void )
{
	hit_work_cnt[0] = hit_work_cnt[1] = 0 ;
	hit_work_old[0] = hit_work_old[1] = 0 ;
}

/*==============================================================*/
static void hit_work_next( void )
{
	hit_work_old[0] = hit_work_cnt[0] ;
	hit_work_old[1] = hit_work_cnt[1] ;
	hit_work_cnt[0] = hit_work_cnt[1] = 0 ;
}

/****************************************************************/
void hit_work_set( PLAYER_WORK *w )
{
	if( !vs_sys_w->game_stop ){
		w->hit_box_no = 0 ;
		if( hit_work_cnt[w->pl_no] < HIT_W_MAX ){
			hit_work_w[w->pl_no][hit_work_cnt[w->pl_no] ++] = w ;
		}
	}
}

/****************************************************************/
PLAYER_WORK *pl_catch_ck( PLAYER_WORK *pl_w )
{
	VS_SYS_WORK *vs = &vs_sys_work ;
	Sint16 no ;

	if( vs->ctrl_flag & CTRL_NO_THROW ) return 0 ;
	no = pl_w->frame_box->atck_box2 ;
	if( no >= 0 ) return 0 ;
	if( vs->vs_st != VS_FIGHT ) return 0 ;
	no &= 0x1fff ;
	if( !pl_w->catch_atck_ng_timer && catch_stance_ck( pl_w, pl_w->target_w, no ) ) return pl_w->target_w ;
	return 0 ;
}

/****************************************************************/
PLAYER_WORK *pl_catch_ck2( PLAYER_WORK *pl_w )
{
	VS_SYS_WORK *vs = &vs_sys_work ;
	Sint16 no ;

	if( vs->ctrl_flag & CTRL_NO_THROW ) return 0 ;
	no = pl_w->frame_box->atck_box2 ;
	if( no >= 0 ) return 0 ;
	if( vs->vs_st != VS_FIGHT ) return 0 ;
	no &= 0x1fff ;
	if( !pl_w->catch_atck_ng_timer && catch_ck( pl_w, pl_w->target_w, no ) ) return pl_w->target_w ;
	return 0 ;
}

/*==============================================================*/
static int catch_stance_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 no )
{
	if( em->pl_stance == STANCE_DOWN ) return 0 ;
	if( em->routine_1 == PL_CAUGHT ) return 0 ;
	if( pl_w->pl_stance == STANCE_AIR ){
		if( em->pl_stance != STANCE_AIR ) return 0 ;
	} else {
		if( em->pl_stance == STANCE_AIR ) return 0 ;
		if( !( em->pos.y < em->ground_y + DOT_Y( 15.0f ) ) ) return 0 ;
	}
	return catch_ck( pl_w, em, no ) ;
}

/*==============================================================*/
static int catch_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 no )
{
	if( ABYSS_CK( em ) ) return 0 ;
	if( !em->be_flag ) return 0 ;
	if( !em->vital_new || em->nml_catch_ng_timer
	 || !em->dm_flag || em->change_req || em->team_no || em->catch_cnt >= 2 || em->routine_1
	 || em->act_st == N_GUARD || em->act_st == N_AIR_GUARD ) return 0 ;
	else if( em->frame_box->catch_box ){
		if( catch_box_ck( pl_w, em, no, em->frame_box->catch_box ) ) return 1 ;
	}
	if( em->frame_box->unk_0e > 0 ){
		if( catch_box_ck( pl_w, em, no, em->frame_box->unk_0e ) ) return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static int catch_box_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 at_no, Sint16 dm_no )
{
	return box_ck( pl_w, em, at_no, dm_no ) ;
}

/****************************************************************/
int box_ck( PLAYER_WORK *at_w, PLAYER_WORK *dm_w, Sint16 at_no, Sint16 dm_no )
{
	HIT_BOX *ab = &at_w->hit_box_tbl[at_no] ;
	HIT_BOX *db = &dm_w->hit_box_tbl[dm_no] ;
	Float d[2] ;

	d[0] = ab->x * at_w->scale.x * 1.6666666f ;
	if( at_w->disp_dir ) d[0] = -d[0] ;
	d[0] += at_w->pos.x ;
	d[1] = db->x * dm_w->scale.x * 1.6666666f ;
	if( dm_w->disp_dir ) d[1] = -d[1] ;
	d[1] += dm_w->pos.x ;
	d[0] -= d[1] ;
	if( d[0] < 0.0f ) d[0] = -d[0] ;
	if( ( d[1] = ab->w * at_w->scale.x * 1.6666666f ) == 0.0f ) return 0 ;
	d[1] += db->w * dm_w->scale.x * 1.6666666f ;
	if( d[0] > d[1] ) return 0 ;
	d[0] = -( ab->y * at_w->scale.y * 2.142857f ) + at_w->pos.y ;
	d[1] = -( db->y * dm_w->scale.y * 2.142857f ) + dm_w->pos.y ;
	d[0] -= d[1] ;
	if( d[0] < 0.0f ) d[0] = -d[0] ;
	if( ( d[1] = ab->h * at_w->scale.y * 2.142857f ) == 0.0f ) return 0 ;
	d[1] += db->h * dm_w->scale.y * 2.142857f ;
	if( d[0] > d[1] ) return 0 ;
	return 1 ;
}

/*==============================================================*/
static void pl_hit_ck( void )
{
	register PLAYER_WORK *pl_w, *at_w, *em ;
	int i ;

	if( vs_sys_work.vs_st >= VS_FIGHT ){
		for( i = 0 ; i < 6 ; i ++ ){
			pl_w = &pl_work[i] ;
			if( pl_w->be_flag && pl_w->hit_box_no ){
				at_w = pl_w->dm_hit_w ;
				if( at_w->atck_flag & HIT_TOUCH ) pl_w->hit_box_no |= 0x40 ;
				else {
					pl_w->dm_atck_no = at_w->atck_no ;
					pl_w->dm_at_w = em = &pl_work[at_w->work_no] ;
					pl_w->dm_at_dir = em->pl_dir ;
					pl_w->dm_pl_dir = pl_w->pl_dir ;
					pl_damage( pl_w, at_w ) ;
				}
			}
		}
	}
}

/*==============================================================*/
static void push_ck_all( void )
{
	int i, j, k ;

	for( i = 0 ; i < 2 ; i ++ ){
		for( j = 0 ; j < 3 ; j ++ ){
			for( k = 0 ; k < 3 ; k ++ ) push_ck( vs_sys_work.team_w[i][j], vs_sys_work.team_w[i ^ 1][k] ) ;
		}
	}
}

/*==============================================================*/
static void push_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	Sint16 no, dm ;
	int hit ;

	if( !pl_w->be_flag || !( pl_w->dm_attr & ATR_COUNTER ) || ( no = pl_w->frame_box->atck_box2 ) > 0 || !em->be_flag ) return ;
	hit = 0 ;
	no &= 0x1fff ;
	if( ( dm = em->frame_box->atck_box ) > 0 ){
		hit = box_ck( pl_w, em, no, dm ) ;
		em->at_box_no = dm ;
	}
	if( !hit && ( dm = em->frame_box->atck_box2 ) > 0 ){
		hit = box_ck( pl_w, em, no, dm ) ;
		em->at_box_no = dm ;
	}
	if( !hit ) return ;
	pl_w->hit_stop = 10 ;
	em->hit_stop = 11 ;
	pl_w->dm_flag = 0 ;
	pl_w->catch_ng_timer = 8 ;
	pl_w->dm_attr &= ATR_ELEM_MASK | ATR_BACK_POS | ATR_BACK_TURN | ATR_DOWN_LIFT ;
	pl_w->dm_box_no = no ;
	clash_mark_set( pl_w, em ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit.c" End of File	*/
