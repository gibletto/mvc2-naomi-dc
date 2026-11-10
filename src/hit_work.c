/* The works: the pool every effect and shot takes its work from, the lists that run them each
   frame, and the draw lists that sort them by priority. */
#include "player.h"

#define WORK_KEEP	10	/* works only work_get2 can take */

#define WORK_W( i )	( ( PLAYER_WORK * )&work_buff[i] )

static void work_buff_init( void );
static void work_set_top( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list );
static void work_set_end( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list );
static void work_set_before( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list );
static void work_set_behind( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list );

/****************************************************************/
void work_init( void )
{
	work_buff_init() ;
	memset( &stage_work, 0, sizeof( stage_work ) ) ;
	memset( &scr_work, 0, sizeof( scr_work ) ) ;
	memset( &fog_work, 0, sizeof( fog_work ) ) ;
	memset( &bright_work, 0, sizeof( bright_work ) ) ;
	memset( &flash_work, 0, sizeof( flash_work ) ) ;
	tex_req_flag = 0 ;
	tex_req_no = 0 ;
}

/*==============================================================*/
static void work_buff_init( void )
{
	int i ;

	free_top = WORK_W( 0 ) ;
	free_end = WORK_W( WORK_MAX - 1 ) ;
	for( i = 0 ; i < WORK_MAX ; i ++ ){
		memset( WORK_W( i ), 0, WORK_SIZE ) ;
		if( i + 1 != WORK_MAX ) WORK_W( i )->before_w = WORK_W( i + 1 ) ;
	}
	for( i = 0 ; i < LIST_MAX ; i ++ ){
		work_top[i] = 0 ;
		work_end[i] = 0 ;
		work_cnt[i] = 0 ;
	}
	work_rest_cnt = WORK_MAX - WORK_KEEP ;
	free_cnt = WORK_MAX ;
}

/****************************************************************/
void work_list_kill( Uint8 list )
{
	PLAYER_WORK *w ;

	for( w = work_top[list] ; w ; w = w->behind_w ) work_free( w ) ;
	work_top[list] = 0 ;
	work_end[list] = 0 ;
	work_cnt[list] = 0 ;
}

/****************************************************************/
void work_list_move( Uint8 list )
{
	PLAYER_WORK *w ;

	w = work_top[list] ;
	if( !w ) return ;
	do {
		w->work_func( w ) ;
		w = w->behind_w ;
	} while( w ) ;
}

/****************************************************************/
PLAYER_WORK *work_get( PLAYER_WORK *base, Uint8 list, Uint8 mode )
{
	if( work_rest_cnt <= 0 ) return 0 ;
	else return work_get2( base, list, mode ) ;
}

/****************************************************************/
PLAYER_WORK *work_get2( PLAYER_WORK *base, Uint8 list, Uint8 mode )
{
	static void ( *work_set_tbl[] )( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list ) = {
		work_set_top, work_set_end, work_set_behind, work_set_before
	} ;
	PLAYER_WORK *w ;

	if( !free_cnt ) return 0 ;
	w = free_top ;
	free_top = w->before_w ;
	work_rest_cnt -- ;
	if( !--free_cnt ) free_end = 0 ;
	work_cnt[list] ++ ;
	memset( w, 0, WORK_SIZE ) ;
	w->work_kind = list ;
	work_set_tbl[mode]( base, w, list ) ;
	return w ;
}

/*==============================================================*/
static void work_set_top( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list )
{
	if( !work_top[list] && !work_end[list] ){
		work_top[list] = work_end[list] = w ;
	} else {
		w->before_w = 0 ;
		w->behind_w = work_top[list] ;
		work_top[list]->before_w = w ;
		work_top[list] = w ;
	}
}

/*==============================================================*/
static void work_set_end( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list )
{
	if( !work_top[list] && !work_end[list] ){
		work_top[list] = work_end[list] = w ;
	} else {
		w->behind_w = 0 ;
		w->before_w = work_end[list] ;
		work_end[list]->behind_w = w ;
		work_end[list] = w ;
	}
}

/*==============================================================*/
static void work_set_before( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list )
{
	if( !base->before_w ){
		w->before_w = 0 ;
		w->behind_w = base ;
		base->before_w = w ;
		work_top[list] = w ;
	} else {
		w->before_w = base->before_w ;
		w->behind_w = base ;
		base->before_w->behind_w = w ;
		base->before_w = w ;
	}
}

/*==============================================================*/
static void work_set_behind( PLAYER_WORK *base, PLAYER_WORK *w, Uint8 list )
{
	if( !base->behind_w ){
		w->behind_w = 0 ;
		w->before_w = base ;
		base->behind_w = w ;
		work_end[list] = w ;
	} else {
		w->before_w = base ;
		w->behind_w = base->behind_w ;
		base->behind_w->before_w = w ;
		base->behind_w = w ;
	}
}

/****************************************************************/
void work_list_disp( Uint8 list )
{
	PLAYER_WORK *w ;

	w = work_top[list] ;
	if( !w ) return ;
	do {
		if( w->disp_flag ){
			sprite_tex_set( w ) ;
			disp_prio_set( w ) ;
		}
		w = w->behind_w ;
	} while( w ) ;
}

/****************************************************************/
void work_free( PLAYER_WORK *w )
{
	PLAYER_WORK *ew ;

	w->disp_flag = 0 ;
	if( !w->before_w ){
		ew = w->behind_w ;
		work_top[w->work_kind] = ew ;
		if( !ew ) work_end[w->work_kind] = 0 ;
		else ew->before_w = 0 ;
	} else if( !w->behind_w ){
		ew = w->before_w ;
		work_end[w->work_kind] = ew ;
		ew->behind_w = 0 ;
	} else {
		w->before_w->behind_w = w->behind_w ;
		w->behind_w->before_w = w->before_w ;
	}
	work_cnt[w->work_kind] -- ;
	if( ( ew = free_end ) ) ew->before_w = w ;
	else free_top = w ;
	w->before_w = 0 ;
	free_end = w ;
	work_rest_cnt ++ ;
	free_cnt ++ ;
}

/****************************************************************/
/* puts a work in its priority's draw list, sorted by sub_prio (the last statement has no effect) */
void disp_prio_set( PLAYER_WORK *w )
{
	register PLAYER_WORK *ew ;
	register Sint8 prio ;
	register Sint8 i ;
	register Sint8 j ;
	register int k ;

	if( w->work_kind <= 4 ){
		if( w->disp_flag ){
			prio = w->prio ;
			if( disp_prio_cnt[prio] < PRIO_W_MAX ){
				disp_prio_w[prio][disp_prio_cnt[prio] ++] = w ;
				i = disp_prio_cnt[prio] - 2 ;
				if( i >= 0 ){
					j = disp_prio_cnt[prio] - 1 ;
					do {
						ew = disp_prio_w[prio][( k = j ) - 1] ;
						if( ew->sub_prio > w->sub_prio ){
							disp_prio_w[prio][k - 1] = w ;
							disp_prio_w[prio][k] = ew ;
							j -- ;
						} else break ;
					} while( -- i >= 0 ) ;
				}
			}
		}
	}
	if( 0 ) ew->sub_prio ;
}

/****************************************************************/
void disp_prio_clr( void )
{
	int i ;

	for( i = 0 ; i < PRIO_MAX ; i ++ ) disp_prio_cnt[i] = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "hit_work.c" End of File	*/
