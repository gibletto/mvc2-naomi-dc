/* DM0D, a screen's title bar: the title by no, the player's rank mark, and a back whose texture
   scrolls. */
#include "player.h"
#include "dm0d.h"

void title_move( PLAYER_WORK *w ) ;

static VEC3 title_scale = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL title_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 title_pos[6] = {
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.78f }, { 3.7f, 0.47f, -10.4f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }
} ;

/* type is the part; timer0 keeps its first model */
void dm0d_title_set( int no )
{
	static Uint16 title_obj[6] = { 0x103, 0x105, 0x73, 0x5d, 0x5e, 0x3a } ;
	PLAYER_WORK *w ;
	VEC3 *pos ;
	Uint16 *obj ;
	int i ;

	pos = title_pos ;
	obj = title_obj ;
	for( i = 0 ; i < 5 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) break ;
		w->disp_flag = 1 ;
		w->work_func = title_move ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->sub_no = no ;
		w->type = i ;
		w->pos = *pos ++ ;
		w->obj = obj_dm0d->obj[*obj] ;
		w->timer0 = *obj ++ ;
		w->scale = title_scale ;
		w->fade = title_fade ;
	}
}

void title_move( PLAYER_WORK *w )
{
	Float u, v ;

	switch( w->type ){
	case 0 :
		w->obj = obj_dm0d->obj[w->timer0 + w->sub_no] ;
		break ;
	case 1 :
	case 3 :
		break ;
	case 2 :
		w->obj = obj_dm0d->obj[w->timer0 - net_rank] ;
		break ;
	case 4 :
		switch( w->routine_0 ){
		case 0 :
			w->routine_0 ++ ;
			w->timer1 = w->timer0 = 0 ;
			obj_uv_clear( w->obj ) ;
			break ;
		case 1 :
			if( ++ w->timer0 >= 1000 ) w->timer0 = 0 ;
			if( ++ w->timer1 >= 1000 ) w->timer1 = 0 ;
			obj_uv_begin( obj_dm0d->obj[58], w->obj ) ;
			while( !obj_uv_next() ){
				obj_uv_get( &u, &v ) ;
				u -= 1.0f - w->timer0 * 0.001f ;
				v -= 1.0f - w->timer1 * 0.001f ;
				obj_uv_put( &u, &v ) ;
			}
			break ;
		}
		break ;
	}
}
