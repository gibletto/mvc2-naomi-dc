/* DM0D, the lobby's record panel: its plates, the player's name, record and play time, the five
   items that slide in and turn when chosen, the strip that runs under them, and two messages. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern int sprintf( char *s, const char *fmt, ... ) ;

void rec_rank_move( PLAYER_WORK *w ) ;
void rec_rank_init( PLAYER_WORK *w ) ;
void rec_rank_main( PLAYER_WORK *w ) ;
void rec_back_move( PLAYER_WORK *w ) ;
void rec_back_init( PLAYER_WORK *w ) ;
void rec_back_main( PLAYER_WORK *w ) ;
void rec_bar_move( PLAYER_WORK *w ) ;
void rec_bar_init( PLAYER_WORK *w ) ;
void rec_bar_main( PLAYER_WORK *w ) ;
void rec_text_move( PLAYER_WORK *w ) ;
void rec_text_init( PLAYER_WORK *w ) ;
void rec_text_main( PLAYER_WORK *w ) ;
void rec_frame0_move( PLAYER_WORK *w ) ;
void rec_frame0_init( PLAYER_WORK *w ) ;
void rec_frame0_main( PLAYER_WORK *w ) ;
void rec_frame1_move( PLAYER_WORK *w ) ;
void rec_frame1_init( PLAYER_WORK *w ) ;
void rec_frame1_main( PLAYER_WORK *w ) ;
void rec_item_move( PLAYER_WORK *w ) ;
void rec_item_init( PLAYER_WORK *w ) ;
void rec_item_main( PLAYER_WORK *w ) ;
void rec_item_wait( PLAYER_WORK *w ) ;
void rec_item_slide( PLAYER_WORK *w ) ;
void rec_item_ready( PLAYER_WORK *w ) ;
void rec_item_turn( PLAYER_WORK *w ) ;
void rec_item_chosen( PLAYER_WORK *w ) ;
void rec_item_back( PLAYER_WORK *w ) ;
void rec_head_move( PLAYER_WORK *w ) ;
void rec_head_init( PLAYER_WORK *w ) ;
void rec_head_main( PLAYER_WORK *w ) ;
void rec_head_wait( PLAYER_WORK *w ) ;
void rec_head_slide( PLAYER_WORK *w ) ;
void rec_head_stay( PLAYER_WORK *w ) ;
void rec_flow_set( PLAYER_WORK *mw ) ;
void rec_flow_move( PLAYER_WORK *w ) ;
void rec_flow_init( PLAYER_WORK *w ) ;
void rec_flow_main( PLAYER_WORK *w ) ;
void rec_flow_wait( PLAYER_WORK *w ) ;
void rec_flow_run( PLAYER_WORK *w ) ;
void rec_flow_stop( PLAYER_WORK *w ) ;
void rec_flow_uv( PLAYER_WORK *w ) ;
void rec_part_set( PLAYER_WORK *mw ) ;
void rec_part_move( PLAYER_WORK *w ) ;
void rec_part_init( PLAYER_WORK *w ) ;
void rec_part_main( PLAYER_WORK *w ) ;
void rec_mark_set( PLAYER_WORK *mw ) ;
void rec_mark_move( PLAYER_WORK *w ) ;
void rec_mark_init( PLAYER_WORK *w ) ;
void rec_mark_main( PLAYER_WORK *w ) ;
void rec_glow_set( PLAYER_WORK *mw ) ;
void rec_glow_move( PLAYER_WORK *w ) ;
void rec_glow_init( PLAYER_WORK *w ) ;
void rec_glow_main( PLAYER_WORK *w ) ;
void rec_msg_set( Uint8 no ) ;
void rec_msg_move( PLAYER_WORK *w ) ;
void rec_msg_init( PLAYER_WORK *w ) ;
void rec_msg_main( PLAYER_WORK *w ) ;
void rec_info_put( void ) ;
void rec_next( PLAYER_WORK *w ) ;
void rec_free( PLAYER_WORK *w ) ;

static const VEC3 rec_one = { 1.0f, 1.0f, 1.0f } ;	/* unused */
static FADE_COL rec_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;

/****************************************************************/
/*			The rank badge										*/
/****************************************************************/
void rec_rank_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_rank_move ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[115 - net_rank] ;
		w->pos.x = 13.0f ;
		w->pos.y = 5.34f ;
		w->pos.z = -15.56f ;
	}
}

void rec_rank_move( PLAYER_WORK *w )
{
	static void ( *rec_rank_jmp[4] )( PLAYER_WORK *w ) = { rec_rank_init, rec_rank_main, rec_next, rec_free } ;

	rec_rank_jmp[w->routine_0]( w ) ;
}

void rec_rank_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

void rec_rank_main( PLAYER_WORK *w )
{
	w->obj = obj_dm0d->obj[115 - net_rank] ;
}

/****************************************************************/
/*			The backdrop										*/
/****************************************************************/
void rec_back_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_back_move ;
		w->obj_flag = OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[142] ;
	}
}

void rec_back_move( PLAYER_WORK *w )
{
	static void ( *rec_back_jmp[4] )( PLAYER_WORK *w ) = { rec_back_init, rec_back_main, rec_next, rec_free } ;

	rec_back_jmp[w->routine_0]( w ) ;
}

void rec_back_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

void rec_back_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The bar, a little narrowed							*/
/****************************************************************/
void rec_bar_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_bar_move ;
		w->obj_flag = OBJ_SCALE | OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[141] ;
		w->scale.x = 0.9f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
	}
}

void rec_bar_move( PLAYER_WORK *w )
{
	static void ( *rec_bar_jmp[4] )( PLAYER_WORK *w ) = { rec_bar_init, rec_bar_main, rec_next, rec_free } ;

	rec_bar_jmp[w->routine_0]( w ) ;
}

void rec_bar_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

void rec_bar_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The plate the record is written on					*/
/****************************************************************/
void rec_text_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_text_move ;
		w->obj_flag = OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[143] ;
	}
}

void rec_text_move( PLAYER_WORK *w )
{
	static void ( *rec_text_jmp[4] )( PLAYER_WORK *w ) = { rec_text_init, rec_text_main, rec_next, rec_free } ;

	rec_text_jmp[w->routine_0]( w ) ;
}

void rec_text_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
	rec_info_put() ;
}

void rec_text_main( PLAYER_WORK *w )
{
	rec_info_put() ;
}

/****************************************************************/
/*			The frame's two pieces								*/
/****************************************************************/
void rec_frame0_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_frame0_move ;
		w->obj_flag = OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[159] ;
	}
}

void rec_frame0_move( PLAYER_WORK *w )
{
	static void ( *rec_frame0_jmp[4] )( PLAYER_WORK *w ) = { rec_frame0_init, rec_frame0_main, rec_next, rec_free } ;

	rec_frame0_jmp[w->routine_0]( w ) ;
}

void rec_frame0_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

void rec_frame0_main( PLAYER_WORK *w )
{
}

void rec_frame1_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_frame1_move ;
		w->obj_flag = OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[160] ;
	}
}

void rec_frame1_move( PLAYER_WORK *w )
{
	static void ( *rec_frame1_jmp[4] )( PLAYER_WORK *w ) = { rec_frame1_init, rec_frame1_main, rec_next, rec_free } ;

	rec_frame1_jmp[w->routine_0]( w ) ;
}

void rec_frame1_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

void rec_frame1_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			An item: it waits its turn, slides in, and turns	*/
/*			round while it is the chosen one					*/
/****************************************************************/
void rec_item_set( Uint8 no )
{
	static const Float rec_item_y[5] = { 3.06f, 1.06f, -0.94f, -3.04f, -5.16f } ;
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_item_move ;
		w->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[144] ;
		w->pos.x = -21.396f ;
		w->pos.y = rec_item_y[no] ;
		w->pos.z = -4.404f ;
		w->sub_no = no ;
		w->rot[1] = 0x238e ;
		w->timer0 = ( w->sub_no + 1 ) * 8 ;
		rec_part_set( w ) ;
	}
}

void rec_item_move( PLAYER_WORK *w )
{
	static void ( *rec_item_jmp[4] )( PLAYER_WORK *w ) = { rec_item_init, rec_item_main, rec_next, rec_free } ;

	rec_item_jmp[w->routine_0]( w ) ;
}

void rec_item_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

void rec_item_main( PLAYER_WORK *w )
{
	static void ( *rec_item_main_jmp[6] )( PLAYER_WORK *w ) = {
		rec_item_wait, rec_item_slide, rec_item_ready, rec_item_turn, rec_item_chosen, rec_item_back
	} ;

	rec_item_main_jmp[w->routine_1]( w ) ;
}

void rec_item_wait( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		return ;
	}
	w->routine_1 ++ ;
	w->timer0 = 15 ;
	w->spd.x = ( -11.396f - w->pos.x ) / w->timer0 ;
	w->spd.z = ( -14.404f - w->pos.z ) / w->timer0 ;
}

void rec_item_slide( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		w->pos.x += w->spd.x ;
		w->pos.z += w->spd.z ;
		return ;
	}
	w->routine_1 ++ ;
	w->timer0 = 0 ;
	w->spd.x = -10.09f ;
	w->spd.z = -11.5f ;
}

void rec_item_ready( PLAYER_WORK *w )
{
	if( rec_item_no == w->sub_no ){
		w->routine_1 ++ ;
		w->timer0 = 10 ;
		w->spd.y = -5.0f ;
	}
}

void rec_item_turn( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		w->rot[1] += DEG_ANG( w->spd.y ) ;
		return ;
	}
	w->routine_1 ++ ;
	w->rot[1] = 0 ;
	w->obj = obj_dm0d->obj[145] ;
	unk_8C212F20 = 0 ;
}

void rec_item_chosen( PLAYER_WORK *w )
{
	if( rec_item_no != w->sub_no ){
		w->routine_1 ++ ;
		w->timer0 = 10 ;
		w->spd.y = 5.0f ;
		w->obj = obj_dm0d->obj[144] ;
	}
}

void rec_item_back( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		w->rot[1] += DEG_ANG( w->spd.y ) ;
		return ;
	}
	w->routine_1 = 2 ;
	w->rot[1] = 0x238e ;
}

/****************************************************************/
/*			The heading, which slides in from the right			*/
/****************************************************************/
void rec_head_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_head_move ;
		w->obj_flag = OBJ_POS | OBJ_FADE ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[146] ;
		w->pos.x = 40.0f ;
		w->pos.y = 0.0f ;
		w->pos.z = 0.0f ;
		w->timer0 = 40 ;
		rec_flow_set( w ) ;
	}
}

void rec_head_move( PLAYER_WORK *w )
{
	static void ( *rec_head_jmp[4] )( PLAYER_WORK *w ) = { rec_head_init, rec_head_main, rec_next, rec_free } ;

	rec_head_jmp[w->routine_0]( w ) ;
}

void rec_head_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

void rec_head_main( PLAYER_WORK *w )
{
	static void ( *rec_head_main_jmp[3] )( PLAYER_WORK *w ) = { rec_head_wait, rec_head_slide, rec_head_stay } ;

	rec_head_main_jmp[w->routine_1]( w ) ;
}

void rec_head_wait( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		return ;
	}
	w->routine_1 ++ ;
	w->timer0 = 15 ;
	w->spd.x = ( 0.0f - w->pos.x ) / w->timer0 ;
}

void rec_head_slide( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		w->pos.x += w->spd.x ;
		return ;
	}
	w->routine_1 ++ ;
	w->timer0 = 0 ;
	w->spd.x = 0.0f ;
}

void rec_head_stay( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The strip that runs under the heading, with its		*/
/*			mark and glow										*/
/****************************************************************/
void rec_flow_set( PLAYER_WORK *mw )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_flow_move ;
		w->obj_flag = OBJ_POS ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[147] ;
		w->master_w = mw ;
		rec_mark_set( w ) ;
		rec_glow_set( w ) ;
	}
}

void rec_flow_move( PLAYER_WORK *w )
{
	static void ( *rec_flow_jmp[4] )( PLAYER_WORK *w ) = { rec_flow_init, rec_flow_main, rec_next, rec_free } ;

	rec_flow_jmp[w->routine_0]( w ) ;
}

void rec_flow_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
}

void rec_flow_main( PLAYER_WORK *w )
{
	static void ( *rec_flow_main_jmp[3] )( PLAYER_WORK *w ) = { rec_flow_wait, rec_flow_run, rec_flow_stop } ;

	rec_flow_main_jmp[w->routine_1]( w ) ;
}

void rec_flow_wait( PLAYER_WORK *w )
{
	if( w->master_w->routine_1 == 2 ){
		w->routine_1 ++ ;
		w->disp_flag = 1 ;
		obj_uv_clear( w->obj ) ;
	}
}

void rec_flow_run( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	rec_flow_uv( w ) ;
	if( unk_8C212F20 == 0 ){
		w->disp_flag = 0 ;
		w->routine_1 ++ ;
	}
}

void rec_flow_stop( PLAYER_WORK *w )
{
	if( unk_8C212F20 == 1 ) w->routine_1 = 1 ;
}

/* the strip's texture runs a step each frame, four steps round */
void rec_flow_uv( PLAYER_WORK *w )
{
	static Float rec_flow_step[12] = {
		3.0f, 2.0f, 0.0f, 3.0f, -2.0f, 0.0f, -3.0f, 2.0f, 0.0f, -3.0f, -2.0f, 0.0f
	} ;
	Float v, u ;

	if( ++ w->timer0 >= 4 ) w->timer0 = 0 ;
	obj_uv_begin( obj_dm0d->obj[148], w->obj ) ;
	while( !obj_uv_next() ){
		obj_uv_get( &u, &v ) ;
		u -= 1.0f - rec_flow_step[w->timer0 * 2] * 0.01f ;
		v -= 1.0f - rec_flow_step[w->timer0 * 2 + 1] * 0.01f ;
		obj_uv_put( &u, &v ) ;
	}
}

/****************************************************************/
/*			An item's label, drawn on the item					*/
/****************************************************************/
void rec_part_set( PLAYER_WORK *mw )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_part_move ;
		w->obj_flag = OBJ_POS ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[mw->sub_no + 149] ;
		w->base_mtx = &mw->obj_mtx ;
		w->master_w = mw ;
	}
}

void rec_part_move( PLAYER_WORK *w )
{
	static void ( *rec_part_jmp[2] )( PLAYER_WORK *w ) = { rec_part_init, rec_part_main } ;

	rec_part_jmp[w->routine_0]( w ) ;
}

void rec_part_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
}

void rec_part_main( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The strip's mark, for the chosen item				*/
/****************************************************************/
void rec_mark_set( PLAYER_WORK *mw )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_mark_move ;
		w->obj_flag = OBJ_POS ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[154] ;
		w->master_w = mw ;
	}
}

void rec_mark_move( PLAYER_WORK *w )
{
	static void ( *rec_mark_jmp[2] )( PLAYER_WORK *w ) = { rec_mark_init, rec_mark_main } ;

	rec_mark_jmp[w->routine_0]( w ) ;
}

void rec_mark_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
}

void rec_mark_main( PLAYER_WORK *w )
{
	if( w->master_w->routine_1 == 2 ) w->disp_flag = 1 ;
	else w->disp_flag = 0 ;
	w->obj = obj_dm0d->obj[rec_item_no + 154] ;
}

/****************************************************************/
/*			The strip's glow									*/
/****************************************************************/
void rec_glow_set( PLAYER_WORK *mw )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = rec_glow_move ;
		w->obj_flag = OBJ_POS ;
		w->fade = rec_fade ;
		w->obj = obj_dm0d->obj[161] ;
		w->master_w = mw ;
	}
}

void rec_glow_move( PLAYER_WORK *w )
{
	static void ( *rec_glow_jmp[2] )( PLAYER_WORK *w ) = { rec_glow_init, rec_glow_main } ;

	rec_glow_jmp[w->routine_0]( w ) ;
}

void rec_glow_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
}

void rec_glow_main( PLAYER_WORK *w )
{
	w->disp_flag = 0 ;
	if( w->master_w->routine_1 == 2 ) w->disp_flag = 1 ;
}

/****************************************************************/
/*			The two messages, shown for three seconds			*/
/****************************************************************/
void rec_msg_all_set( void )
{
	rec_msg_set( 0 ) ;
	rec_msg_set( 1 ) ;
}

void rec_msg_set( Uint8 no )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 1 ;
		w->work_func = rec_msg_move ;
		w->obj_flag = OBJ_POS ;
		w->fade = rec_fade ;
		w->timer0 = 180 ;
		w->obj = no ? obj_dm0d->obj[269] : obj_dm0d->obj[309] ;
		w->sub_no = no ;
	}
}

void rec_msg_move( PLAYER_WORK *w )
{
	static void ( *rec_msg_jmp[4] )( PLAYER_WORK *w ) = { rec_msg_init, rec_msg_main, rec_next, rec_free } ;

	rec_msg_jmp[w->routine_0]( w ) ;
}

void rec_msg_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	if( !w->sub_no ) unk_8C212F20 = 1 ;
}

void rec_msg_main( PLAYER_WORK *w )
{
	if( w->timer0 ) w->timer0 -- ;
	if( !w->timer0 ){
		w->routine_0 ++ ;
		if( !w->sub_no ) unk_8C212F20 = 0 ;
	}
}

/****************************************************************/
/*			The record's lines: the name, the two counts, the	*/
/*			two scores and the play time's hours and minutes	*/
/****************************************************************/
void rec_info_put( void )
{
	char buf[32] ;
	int n ;

	sprintf( buf, "%s", net_handle ) ;
	str_put( 20.0f, 120.0f, 14.45f, 0xffffffff, buf ) ;
	n = ( Uint16 )unk_8C213B0C ;
	sprintf( buf, "%8d", n ) ;
	str_put( 240.0f, 121.0f, 11.91f, 0xffffffff, buf ) ;
	n = ( Uint16 )( unk_8C213B0C >> 16 ) ;
	sprintf( buf, "%8d", n ) ;
	str_put( 400.0f, 121.0f, 11.91f, 0xffffffff, buf ) ;
	n = unk_8C213AF8 ;
	sprintf( buf, "%7d", n ) ;
	str_put( 220.0f, 395.0f, 11.91f, 0xffffffff, buf ) ;
	n = net_entry_num ;
	sprintf( buf, "%7d", n ) ;
	str_put( 370.0f, 395.0f, 11.91f, 0xffffffff, buf ) ;
	n = net_play_time ;
	n = n / 60 / 60 ;
	sprintf( buf, "%2d", n ) ;
	str_put( 500.0f, 395.0f, 11.91f, 0xffffffff, buf ) ;
	n = net_play_time ;
	n = n / 60 % 60 ;
	sprintf( buf, "%2d", n ) ;
	str_put( 570.0f, 395.0f, 11.91f, 0xffffffff, buf ) ;
}

/****************************************************************/
/*			A piece is done										*/
/****************************************************************/
void rec_next( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
}

void rec_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/****************************************************************/
/*			The record panel									*/
/****************************************************************/
void dm0d_rec_all_set( void )
{
	int i ;

	rec_rank_set() ;
	rec_back_set() ;
	rec_bar_set() ;
	rec_text_set() ;
	rec_frame0_set() ;
	rec_frame1_set() ;
	for( i = 0 ; i < 5 ; i ++ ) rec_item_set( i ) ;
	rec_head_set() ;
	win_pict_set( 1 ) ;
}
