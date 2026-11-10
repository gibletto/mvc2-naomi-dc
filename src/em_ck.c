/* The computer's checks: which modes an act allows, its level by vitality and time, and the
   other side's attacks, how near they are and whether their boxes reach it. */
#include "player.h"

/* the box numbers in a frame's frame_box record are unsigned; their low bits pick the box */
#define BOX_NUM( n )	( ( Uint16 )( n ) )
#define BOX_NO( n )		( BOX_NUM( n ) & 0x3ff )
#define AREA_NO( n )	( BOX_NUM( n ) & 0xfff )

/* for each act, the event modes it lets start, a bit each */
static Uint16 act_ck_tbl[35] = {
	0x803f, 0x803f, 0x001a, 0x809e, 0x803f, 0x803f, 0x803f, 0x803f,
	0x803f, 0x803f, 0x803f, 0x003d, 0x0000, 0x001a, 0x809e, 0x8012,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x00de, 0x001e, 0x001e, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000,
} ;

/* the level of play by vitality, an entry for each 8 points */
static Uint8 vital_lv_tbl[32] = {
	7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1,
	0, 0, 0, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
} ;

/* and by the time left, an entry for each 5 counts */
static Uint8 time_lv_tbl[32] = {
	7, 7, 7, 6, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
} ;

static int atck_dist_ck( PLAYER_WORK *pl_w, PLAYER_WORK *ew, HIT_DATA *hd, int no );
static int body_box_ck( PLAYER_WORK *pl_w, PLAYER_WORK *ew, int no );
static int body_x_ck( PLAYER_WORK *pl_w, PLAYER_WORK *ew, int pl_no, int ew_no );
static int atck_box_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, int no );
static int box_hit_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, int pl_no, int em_no );
static Float box_pos_get( PLAYER_WORK *w, HIT_BOX *box, int y );
static int area_x_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 *area, HIT_BOX *box );
static int area_y_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 *area, HIT_BOX *box );

/****************************************************************/
/*			Whether the player's act lets event mode no			*/
/*			start												*/
/****************************************************************/
int em_act_ck( PLAYER_WORK *pl_w, int no )
{
	if( pl_w->routine_1 ) return 0 ;
	return act_ck_tbl[pl_w->act_st] & 1 << no ;
}

/****************************************************************/
/*			The same, for the act in old_act_st					*/
/****************************************************************/
int em_old_act_ck( PLAYER_WORK *pl_w, int no )
{
	return act_ck_tbl[pl_w->old_act_st] & 1 << no ;
}

/****************************************************************/
/*			Sets the level of play from the vitality and the	*/
/*			time; 1 if it changed								*/
/****************************************************************/
int em_lv_ck( PLAYER_WORK *pl_w )
{
	int lv, lv2, time, half ;

	if( pl_w->cpu_lock_flag ) return 0 ;
	if( Sys->cpu_level_max ) return 0 ;
	lv = vital_lv_tbl[pl_w->vital_new >> 3] ;
	time = vs_sys_work.cpu_time ;
	half = 0 ;
	if( time % 10 > 4 ) half = 8 ;
	lv2 = time_lv_tbl[( time / 10 << 4 | half ) >> 3] ;
	if( lv < lv2 ) lv = lv2 ;
	if( pl_w->cpu_lv != lv ){
		pl_w->cpu_lv = lv ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			The guard types of the other side's attacks			*/
/*			under way, with bit 0 for any						*/
/****************************************************************/
int em_atck_kind( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	HIT_DATA *hd ;
	int i, num, flag ;

	flag = 0 ;
	num = hit_work_old[pl_w->pl_no ^ 1] ;
	for( i = 0 ; i < num ; i ++ ){
		ew = hit_work_w[pl_w->pl_no ^ 1][i] ;
		if( ew->work_kind != W_PL && ew->work_kind != W_SHL && ew->work_kind != W_PLS ) continue ;
		if( ( ew->cg_atck_flag & CG_ATCK_ALL ) < 0x80 ) continue ;
		flag |= 1 ;
		hd = ew->hit_data_tbl ;
		hd += ew->atck_no & ATCK_NO_MASK ;
		flag |= hd->atck_kind & A_GUARD_MASK ;
	}
	return flag ;
}

/****************************************************************/
/*			The nearest of the other side's attacks under		*/
/*			way, with its distance in atck_len					*/
/****************************************************************/
PLAYER_WORK *em_atck_near( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew, *near ;
	HIT_DATA *hd ;
	int i, num, find, ret ;

	pl_w->atck_len = DOT_X( 32767.0f ) ;
	find = 0 ;
	num = hit_work_old[pl_w->pl_no ^ 1] ;
	for( i = 0 ; i < num ; i ++ ){
		ew = hit_work_w[pl_w->pl_no ^ 1][i] ;
		if( ew->work_kind != W_PL && ew->work_kind != W_SHL && ew->work_kind != W_PLS ) continue ;
		if( ( ew->cg_atck_flag & CG_ATCK_ALL ) >= 0x80 ){
			hd = ew->hit_data_tbl ;
			hd += ew->atck_no & ATCK_NO_MASK ;
			ret = atck_dist_ck( pl_w, ew, hd, hd->range_box ) ;
			ret |= atck_dist_ck( pl_w, ew, hd, hd->range_box2 ) ;
			switch( ret ){
			case 1 :
			case 3 :
				pl_w->atck_len = 0.0f ;
				return ew ;
			case 2 :
				near = ew ;
				find = 1 ;
				break ;
			default :
				break ;
			}
		}
	}
	if( find ) ew = near ;
	else ew = 0 ;
	return ew ;
}

/*==============================================================*/
/*			How ew's attack box no stands: 2 in front,			*/
/*			nearer than atck_len; 1 reaching the player			*/
/*==============================================================*/
static int atck_dist_ck( PLAYER_WORK *pl_w, PLAYER_WORK *ew, HIT_DATA *hd, int no )
{
	HIT_BOX *box ;
	Float x, dist, front ;

	if( !no ) return 0 ;
	box = ew->hit_box_tbl ;
	box += no ;
	x = ( box->x - box->w ) * 1.6666666f ;
	if( ew->disp_dir & 1 ) x = -x ;
	x += ew->pos.x ;
	front = pl_w->pos.x ;
	if( pl_w->disp_dir & 1 ){
		front = x ;
		x = pl_w->pos.x ;
	}
	dist = front - x ;
	if( dist <= 0.0f ){
		if( body_box_ck( pl_w, ew, hd->range_box ) || body_box_ck( pl_w, ew, hd->range_box2 ) ) return 1 ;
		return 0 ;
	}
	if( pl_w->atck_len > dist ){
		pl_w->atck_len = dist ;
		return 2 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			Whether any of the player's body boxes is level		*/
/*			with ew's box no									*/
/*==============================================================*/
static int body_box_ck( PLAYER_WORK *pl_w, PLAYER_WORK *ew, int no )
{
	FRAME_BOX *bx = pl_w->frame_box ;

	if( !no ) return 0 ;
	if( body_x_ck( pl_w, ew, BOX_NO( bx->body_box0 ), no ) ) return 1 ;
	if( body_x_ck( pl_w, ew, BOX_NO( bx->body_box1 ), no ) ) return 1 ;
	if( body_x_ck( pl_w, ew, BOX_NO( bx->body_box2 ), no ) ) return 1 ;
	if( body_x_ck( pl_w, ew, BOX_NO( bx->body_box3 ), no ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			Whether the player's box pl_no and ew's box			*/
/*			ew_no overlap across								*/
/*==============================================================*/
static int body_x_ck( PLAYER_WORK *pl_w, PLAYER_WORK *ew, int pl_no, int ew_no )
{
	HIT_BOX *eb, *pb ;
	Float ex, px, dist, len ;

	eb = ew->hit_box_tbl ;
	eb += ew_no ;
	pb = pl_w->hit_box_tbl ;
	pb += pl_no ;

	ex = box_pos_get( ew, eb, 0 ) ;
	px = box_pos_get( pl_w, pb, 0 ) ;
	dist = ex - px ;
	if( dist < 0.0f ) dist = -dist ;
	if( !pb->w ) return 0 ;
	len = ( pb->w + eb->w ) * 1.6666666f ;
	return len > dist ;
}

/****************************************************************/
/*			Whether the player's attack no reaches em's body	*/
/****************************************************************/
int em_atck_hit_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, int no )
{
	HIT_DATA *hd = pl_w->hit_data_tbl ;

	hd += no & 0x7f ;
	if( atck_box_ck( pl_w, em, hd->range_box ) || atck_box_ck( pl_w, em, hd->range_box2 ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			Whether the player's box no reaches any of em's		*/
/*			body boxes											*/
/*==============================================================*/
static int atck_box_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, int no )
{
	FRAME_BOX *bx = em->frame_box ;

	if( !no ) return 0 ;
	if( box_hit_ck( pl_w, em, no, BOX_NUM( bx->body_box0 ) ) ) return 1 ;
	if( box_hit_ck( pl_w, em, no, BOX_NUM( bx->body_box1 ) ) ) return 1 ;
	if( box_hit_ck( pl_w, em, no, BOX_NUM( bx->body_box2 ) ) ) return 1 ;
	if( box_hit_ck( pl_w, em, no, BOX_NUM( bx->body_box3 ) ) ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			Whether the player's box pl_no and em's box			*/
/*			em_no overlap										*/
/*==============================================================*/
static int box_hit_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, int pl_no, int em_no )
{
	HIT_BOX *pb, *eb ;
	Float pos, epos, dist, len ;

	if( !em_no ) return 0 ;
	pb = pl_w->hit_box_tbl ;
	pb += pl_no ;
	eb = em->hit_box_tbl ;
	eb += em_no ;
	pos = box_pos_get( pl_w, pb, 0 ) ;
	epos = box_pos_get( em, eb, 0 ) ;
	if( ( dist = pos - epos ) < 0.0f ) dist = -dist ;
	if( !eb->w ) return 0 ;
	len = ( eb->w + pb->w ) * 1.6666666f ;
	if( dist > len ) return 0 ;
	pos = box_pos_get( pl_w, pb, 1 ) ;
	epos = box_pos_get( em, eb, 1 ) ;
	if( ( dist = pos - epos ) < 0.0f ) dist = -dist ;
	if( !eb->h ) return 0 ;
	len = ( eb->h + pb->h ) * 1.6666666f ;
	if( dist > len ) return 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Where the centre of w's box is on the screen,		*/
/*			across or (y) up									*/
/*==============================================================*/
static Float box_pos_get( PLAYER_WORK *w, HIT_BOX *box, int y )
{
	Float pos ;

	if( !y ){
		pos = DOT_X( box->x ) ;
		if( w->disp_dir & 1 ) pos = -pos ;
		pos += w->pos.x ;
	} else {
		pos = -box->y * 2.142857f ;
		if( w->disp_dir & 2 ) pos = -pos ;
		pos += w->pos.y ;
	}
	return pos ;
}

/* whether the area (half sizes, then the centre's offset) by the player reaches em's body */

/****************************************************************/
int em_area_ck( PLAYER_WORK *pl_w, CPU_PRM *area )
{
	PLAYER_WORK *em ;
	HIT_BOX *tbl ;
	Sint16 box[4], *p ;
	int i ;

	box[0] = area[0].l ;
	box[1] = area[1].l ;
	box[2] = ( pl_w->disp_dir & 1 ) ? area[2].l : -area[2].l ;
	box[3] = ( pl_w->disp_dir & 2 ) ? -area[3].l : area[3].l ;
	em = pl_w->target_w ;
	tbl = em->hit_box_tbl ;
	p = &em->frame_box->atck_box ;
	p += 2 ;	/* past the two attack boxes */
	for( i = 0 ; i < 4 ; i ++, p ++ ){
		if( !AREA_NO( *p ) ) continue ;
		if( !area_x_ck( pl_w, em, &box[0], &tbl[AREA_NO( *p )] ) ) continue ;
		if( !area_y_ck( pl_w, em, &box[0], &tbl[AREA_NO( *p )] ) ) continue ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
/*			Whether the area by the player reaches any of		*/
/*			the other side's attacks under way					*/
/****************************************************************/
int em_atck_area_ck( PLAYER_WORK *pl_w, CPU_PRM *area )
{
	PLAYER_WORK *ew ;
	HIT_BOX *tbl ;
	Sint16 box[4], *p ;
	int i, j, num ;

	box[0] = area[0].l ;
	box[1] = area[1].l ;
	box[2] = ( pl_w->disp_dir & 1 ) ? area[2].l : -area[2].l ;
	box[3] = ( pl_w->disp_dir & 2 ) ? -area[3].l : area[3].l ;
	num = hit_work_old[pl_w->pl_no ^ 1] ;
	for( i = 0 ; i < num ; i ++ ){
		ew = hit_work_w[pl_w->pl_no ^ 1][i] ;
		tbl = ew->hit_box_tbl ;
		p = &ew->frame_box->atck_box ;
		if( ew->work_kind != W_PL && ew->work_kind != W_SHL && ew->work_kind != W_PLS ) continue ;
		for( j = 0 ; j < 2 ; j ++, p ++ ){
			if( !AREA_NO( *p ) ) continue ;
			if( !area_x_ck( pl_w, ew, &box[0], &tbl[AREA_NO( *p )] ) ) continue ;
			if( !area_y_ck( pl_w, ew, &box[0], &tbl[AREA_NO( *p )] ) ) continue ;
			return 1 ;
		}
	}
	return 0 ;
}

/*==============================================================*/
/*			Whether the area and em's box overlap across		*/
/*==============================================================*/
static int area_x_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 *area, HIT_BOX *box )
{
	Float px, ex, dist, len ;

	if( !area[0] ) return 1 ;
	px = pl_w->pos.x + DOT_X( area[2] ) ;
	ex = DOT_X( box->x ) ;
	if( em->disp_dir ) ex = -ex ;
	ex += em->pos.x ;
	if( ( dist = px - ex ) < 0.0f ) dist = -dist ;
	len = DOT_X( area[0] ) + DOT_X( box->w ) ;
	return len > dist ;
}

/*==============================================================*/
/*			Whether the area and em's box overlap up and		*/
/*			down												*/
/*==============================================================*/
static int area_y_ck( PLAYER_WORK *pl_w, PLAYER_WORK *em, Sint16 *area, HIT_BOX *box )
{
	Float py, ey, dist, len ;

	if( !area[1] ) return 1 ;
	py = pl_w->pos.y + DOT_Y( area[3] ) ;
	ey = -box->y * 2.142857f ;
	if( em->disp_dir & 2 ) ey = -ey ;
	ey += em->pos.y ;
	if( ( dist = py - ey ) < 0.0f ) dist = -dist ;
	len = DOT_Y( area[1] ) + DOT_Y( box->h ) ;
	return len > dist ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "em_ck.c" End of File	*/
