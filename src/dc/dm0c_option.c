/* DM0C, OPTION's menu and its pages' headlines: the menu's lines, the game page's items with their
   settings, the cursor, and the numbers and marks the menus draw. */
#include "player.h"
#include "nl.h"
#include "dm.h"

/* a piece of a page: where it is, its model, and whether more pieces follow it */

/* an item of the game page, or one of its parts */
typedef struct DM0C_ITEM {
	VEC3	pos;
	VEC3	scale;
	Uint16	obj;
	Sint8	work;		/* a setting's mark: the setting's value plus one */
	Sint8	next;		/* -1 for a list's last; in headline_tbl, what the item shows: 1 a mark, 0x80 a level */
} DM0C_ITEM;

/* a piece of a headline */
typedef struct DM0C_LABEL {
	VEC3	pos;
	Float	r, g, b;
	Uint16	obj;
	Sint8	unk_1a;
	Sint8	rot;		/* its turn, from label_rot_tbl */
} DM0C_LABEL;

/* a character of a number drawn by the menu */

extern int obj_scale_max() ;	/* menu_obj_disp hands it the VEC3 itself */
extern void scr_2d_matrix_set( void ) ;
extern void dm0c_work_free( PLAYER_WORK *w ) ;
extern void dm0c_work_free2( PLAYER_WORK *w ) ;

void option_menu_move( PLAYER_WORK *w ) ;
void headline_move( PLAYER_WORK *w ) ;
void headline_item_col( PLAYER_WORK *w ) ;
void headline_nop( PLAYER_WORK *w ) ;
void headline_label_init( PLAYER_WORK *w ) ;
void headline_item_init( PLAYER_WORK *w ) ;
void headline_cursor_init( PLAYER_WORK *w ) ;
void headline_title_init( PLAYER_WORK *w ) ;
void headline_level_init( PLAYER_WORK *w ) ;
void headline_select_init( PLAYER_WORK *w ) ;
void headline_init( PLAYER_WORK *w ) ;
void headline_nop2( PLAYER_WORK *w ) ;
void headline_item_main( PLAYER_WORK *w ) ;
void headline_cursor_main( PLAYER_WORK *w ) ;
void headline_level_main( PLAYER_WORK *w ) ;
void headline_select_main( PLAYER_WORK *w ) ;
void headline_main( PLAYER_WORK *w ) ;
void option_menu_init( PLAYER_WORK *w ) ;
void option_menu_main( PLAYER_WORK *w ) ;

static VEC3 menu_one = { 1.0f, 1.0f, 1.0f } ;
static DM0C_PARTS option_parts_tbl[4] = {	/* the menu's lines */
	{ { 4.0f, 3.0f, -13.0f }, 141, 0, 0 }, { { 1.5f, 0.5f, -13.0f }, 141, 0, 0 },
	{ { -1.0f, -2.0f, -13.0f }, 141, 0, 0 }, { { -3.5f, -4.5f, -13.0f }, 141, 0, 0 }
} ;
static DM0C_PARTS option_parts0[2] = { { { -4.0f, -3.0f, 13.0f }, 143, 0, 0 }, { { 0.0f, 0.0f, 0.02f }, 140, 0, -1 } } ;
static DM0C_PARTS option_parts1[1] = { { { 0.0f, 0.0f, 0.02f }, 140, 0, -1 } } ;
static DM0C_PARTS option_parts2[1] = { { { 0.0f, 0.0f, 0.02f }, 140, 0, -1 } } ;
static DM0C_PARTS option_parts3[1] = { { { 0.0f, 0.0f, 0.02f }, 140, 0, -1 } } ;
static DM0C_PARTS *option_sub_tbl[4] = { option_parts0, option_parts1, option_parts2, option_parts3 } ;
static DM0C_ITEM headline_tbl[7] = {	/* the game page's items */
	{ { -6.0f, 3.5f, -13.0f }, { 1.0f, 1.0f, 1.0f }, 86, 0, -128 },
	{ { -6.5f, 2.0f, -13.0f }, { 1.0f, 1.0f, 1.0f }, 86, 0, 1 },
	{ { -7.0f, 0.5f, -13.0f }, { 1.0f, 1.0f, 1.0f }, 86, 0, -128 },
	{ { -7.0f, -1.0f, -13.0f }, { 1.0f, 1.0f, 1.0f }, 86, 0, 1 },
	{ { -6.5f, -2.5f, -13.0f }, { 1.0f, 1.0f, 1.0f }, 86, 0, 1 },
	{ { -4.21f, -4.0f, -12.99f }, { 1.0f, 1.0f, 1.0f }, 94, 0, 0 },
	{ { -0.5f, -5.5f, -12.98f }, { 1.0f, 1.0f, 1.0f }, 126, 0, 0 }
} ;
static DM0C_ITEM headline_parts0[7] = {
	{ { 0.0f, 0.0f, -0.01f }, { 1.0f, 1.0f, 1.0f }, 87, 0, 0 },
	{ { 0.0f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 88, 0, 0 },
	{ { 4.1f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 95, 0, 0 },
	{ { 8.9f, 0.0f, 0.0f }, { 0.96f, 1.0f, 1.0f }, 96, 0, 0 },
	{ { 13.7f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 97, 0, 0 },
	{ { 3.9f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 98, 0, 0 },
	{ { 13.9f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 99, 0, -1 }
} ;
static DM0C_ITEM headline_parts1[16] = {
	{ { 0.0f, 0.0f, -0.01f }, { 1.0f, 1.0f, 1.0f }, 87, 0, 0 },
	{ { 0.0f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 89, 0, 0 },
	{ { 4.1f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 95, 0, 0 },
	{ { 7.85f, 0.0f, 0.0f }, { 0.75f, 1.0f, 1.0f }, 96, 0, 0 },
	{ { 11.6f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 97, 0, 0 },
	{ { 5.75f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 108, 0, 0 },
	{ { 7.85f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 108, 0, 0 },
	{ { 9.95f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 108, 0, 0 },
	{ { 4.7f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 109, 1, 0 },
	{ { 6.8f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 110, 2, 0 },
	{ { 8.9f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 111, 3, 0 },
	{ { 11.0f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 112, 4, -1 },
	{ { 4.7f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 113, 1, 0 },
	{ { 6.8f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 113, 2, 0 },
	{ { 8.9f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 113, 3, 0 },
	{ { 11.0f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 113, 4, -1 }
} ;
static DM0C_ITEM headline_parts2[7] = {
	{ { 0.0f, 0.0f, -0.01f }, { 1.0f, 1.0f, 1.0f }, 87, 0, 0 },
	{ { 0.0f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 90, 0, 0 },
	{ { 4.1f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 95, 0, 0 },
	{ { 6.5f, 0.0f, 0.0f }, { 0.48f, 1.0f, 1.0f }, 96, 0, 0 },
	{ { 8.9f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 97, 0, 0 },
	{ { 3.9f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 98, 0, 0 },
	{ { 9.1f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 99, 0, -1 }
} ;
static DM0C_ITEM headline_parts3[13] = {
	{ { 0.0f, 0.0f, -0.01f }, { 1.0f, 1.0f, 1.0f }, 87, 0, 0 },
	{ { 0.0f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 91, 0, 0 },
	{ { 4.1f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 95, 0, 0 },
	{ { 9.5f, 0.0f, 0.0f }, { 1.08f, 1.0f, 1.0f }, 96, 0, 0 },
	{ { 14.9f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 97, 0, 0 },
	{ { 7.55f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 108, 0, 0 },
	{ { 11.45f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 108, 0, 0 },
	{ { 5.6f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 114, 1, 0 },
	{ { 9.5f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 115, 2, 0 },
	{ { 13.4f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 116, 3, -1 },
	{ { 5.6f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 117, 1, 0 },
	{ { 9.5f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 117, 2, 0 },
	{ { 13.4f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 117, 3, -1 }
} ;
static DM0C_ITEM headline_parts4[10] = {
	{ { 0.0f, 0.0f, -0.01f }, { 1.0f, 1.0f, 1.0f }, 87, 0, 0 },
	{ { 0.0f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 92, 0, 0 },
	{ { 4.1f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 95, 0, 0 },
	{ { 8.75f, 0.0f, 0.0f }, { 0.93f, 1.0f, 1.0f }, 96, 0, 0 },
	{ { 13.4f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, 97, 0, 0 },
	{ { 8.75f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 108, 0, 0 },
	{ { 6.2f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 119, 1, 0 },
	{ { 11.3f, 0.0f, 0.02f }, { 1.0f, 1.0f, 1.0f }, 118, 2, -1 },
	{ { 6.2f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 120, 1, 0 },
	{ { 11.3f, 0.0f, 0.01f }, { 1.0f, 1.0f, 1.0f }, 120, 2, -1 }
} ;
static DM0C_ITEM headline_parts5[2] = {
	{ { 0.0f, 0.0f, -0.01f }, { 1.7f, 1.0f, 1.0f }, 86, 0, 0 },
	{ { 0.0f, 0.0f, -0.02f }, { 1.7f, 1.0f, 1.0f }, 87, 0, -1 }
} ;
static DM0C_ITEM headline_parts6[3] = {
	{ { -0.9f, 0.0f, -0.02f }, { 1.0f, 1.0f, 1.0f }, 95, 0, 0 },
	{ { 0.0f, 0.0f, -0.02f }, { 0.18f, 1.0f, 1.0f }, 96, 0, 0 },
	{ { 0.9f, 0.0f, -0.02f }, { 1.0f, 1.0f, 1.0f }, 97, 0, -1 }
} ;
static DM0C_ITEM *headline_item_tbl[7] = {
	headline_parts0, headline_parts1, headline_parts2, headline_parts3, headline_parts4, headline_parts5, headline_parts6
} ;
static DM0C_CHAR level_char_tbl[9] = {	/* the difficulty's stars */
	{ 1, 0, 0x64, 0.0f }, { 1, 0, 0x65, 1.2f }, { 1, 0, 0x66, 1.2f }, { 1, 0, 0x67, 1.2f },
	{ 1, 0, 0x68, 1.2f }, { 1, 0, 0x69, 1.2f }, { 1, 0, 0x6a, 1.2f }, { 1, 0, 0x6b, 1.2f },
	{ -1, 0, 0, 0.0f }
} ;
static DM0C_CHAR damage_char_tbl[5] = {	/* and the damage level's */
	{ 1, 0, 0x64, 0.0f }, { 1, 0, 0x66, 1.2f }, { 1, 0, 0x68, 1.2f }, { 1, 0, 0x6a, 1.2f },
	{ -1, 0, 0, 0.0f }
} ;
static Float label_rot_tbl[3] = { 0.0f, 15.0f, 180.0f } ;
static Uint16 headline_label_obj[19] = {	/* a headline's title model, by its page */
	84, 144, 169, 230, 239, 244, 260, 263, 269, 135, 268, 180, 160, 8, 8, 273, 38, 66, 292
} ;
static DM0C_LABEL headline_label0[7] = {
	{ { -9.14f, 6.5f, -13.0f }, 1.0f, 1.0f, 1.0f, 127, 0, 1 },
	{ { -9.14f, 6.5f, -13.01f }, 1.0f, 1.0f, 1.0f, 128, 0, 1 },
	{ { 0.0f, 0.0f, 0.0f }, 0.88f, 0.27f, 0.9f, 81, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 0.79f, 0.18f, 0.81f, 82, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 84, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 83, 0, 0 },
	{ { 11.06f, -1.6f, 0.0f }, 0.88f, 0.27f, 0.9f, 81, 0, 2 }
} ;
static DM0C_LABEL headline_label1[5] = {
	{ { -9.0f, 6.52f, -13.1f }, 1.0f, 1.0f, 1.0f, 3, 0, 1 },
	{ { -9.0f, 6.52f, -13.09f }, 1.0f, 1.0f, 1.0f, 4, 0, 1 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 0.6f, 0.0f, 2, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 0.6f, 0.0f, 6, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 129, 0, 0 }
} ;
static DM0C_LABEL headline_label2[7] = {
	{ { -9.0f, 6.52f, -13.1f }, 1.0f, 1.0f, 1.0f, 3, 0, 1 },
	{ { -9.0f, 6.52f, -13.09f }, 1.0f, 1.0f, 1.0f, 4, 0, 1 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 0.2f, 0.2f, 2, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 0.2f, 0.2f, 6, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 8, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 5, 0, 0 },
	{ { 11.0f, -1.6f, 0.0f }, 1.0f, 0.2f, 0.2f, 2, 0, 2 }
} ;
static DM0C_LABEL headline_label3[5] = {
	{ { -9.0f, 6.52f, -13.1f }, 1.0f, 1.0f, 1.0f, 2, 0, 1 },
	{ { -9.0f, 6.52f, -13.09f }, 1.0f, 1.0f, 1.0f, 3, 0, 1 },
	{ { 0.0f, 6.55f, -13.11f }, 1.0f, 1.0f, 1.0f, 0, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 1, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 38, 0, 0 }
} ;
static DM0C_LABEL headline_label4[5] = {
	{ { -9.14f, 6.5f, -13.0f }, 1.0f, 1.0f, 1.0f, 127, 0, 1 },
	{ { -9.14f, 6.5f, -13.01f }, 1.0f, 1.0f, 1.0f, 128, 0, 1 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 0.1f, 0.1f, 81, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 0.8f, 0.0f, 0.0f, 82, 0, 0 },
	{ { 0.0f, 0.0f, 0.0f }, 1.0f, 1.0f, 1.0f, 292, 0, 0 }
} ;
static DM0C_LABEL *headline_label_tbl[19] = {	/* a page's headline */
	headline_label0, headline_label0, headline_label0, NULL, NULL, NULL, NULL, NULL, NULL,
	headline_label1, headline_label1, headline_label1, headline_label1, headline_label2, headline_label2,
	headline_label1, headline_label3, headline_label3, headline_label4
} ;
static FADE_COL menu_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 select_pos_tbl[9] = {	/* a setting's mark: the round time's, the speed's, the sound's */
	{ -1.8f, 2.0f, -12.99f }, { 0.3f, 2.0f, -12.99f }, { 2.4f, 2.0f, -12.99f }, { 4.5f, 2.0f, -12.99f },
	{ -1.4f, -1.0f, -12.99f }, { 2.5f, -1.0f, -12.99f }, { 6.4f, -1.0f, -12.99f },
	{ -0.3f, -2.5f, -12.99f }, { 4.8f, -2.5f, -12.99f }
} ;
static Uint8 select_tbl[8] = {	/* by the item: its first mark, then the mark's model */
	0, 0, 4, 7, 113, 0, 117, 120
} ;
static Sint8 *headline_conf_tbl[3] = { &conf_round_time, &conf_speed_fix, &conf_stereo } ;
static Sint8 headline_conf_no[7] = { -1, 0, -1, 1, 2, -1, -1 } ;	/* an item's setting */
static VEC3 level_pos_tbl[2] = { { -1.3f, 3.5f, -12.99f }, { -2.3f, 0.5f, -12.99f } } ;
static void ( *headline_init_jmp[7] )( PLAYER_WORK *w ) = {
	headline_label_init, headline_item_init, headline_cursor_init, headline_nop,
	headline_title_init, headline_level_init, headline_select_init
} ;
static void ( *headline_main_jmp[7] )( PLAYER_WORK *w ) = {
	headline_nop2, headline_item_main, headline_cursor_main, headline_item_main,
	headline_nop, headline_level_main, headline_select_main
} ;
static void ( *headline_jmp[4] )( PLAYER_WORK *w ) = {
	headline_init, headline_main, dm0c_work_free, dm0c_work_free2
} ;
static void ( *option_menu_jmp[7] )( PLAYER_WORK *w ) = {
	option_menu_init, option_menu_main, dm0c_work_free, dm0c_work_free2
} ;

/****************************************************************/
/*			OPTION's menu: its four lines and the cursor		*/
/****************************************************************/
PLAYER_WORK *option_menu_set( void )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 4 ; i ++ ){
		if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
			w->disp_flag = 0 ;
			w->work_func = option_menu_move ;
			w->sub_no = 0 ;
			w->type = i ;
			w->scale = menu_one ;
			w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		}
	}
	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = option_menu_move ;
		w->sub_no = 1 ;
		w->type = 0 ;
		w->scale = menu_one ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	}
	return w ;
}

/****************************************************************/
/*			The pieces that ride on a line						*/
/****************************************************************/
PLAYER_WORK *option_menu_sub_set( PLAYER_WORK *mw, Uint8 kind, int no )
{
	PLAYER_WORK *w ;
	DM0C_PARTS *p ;
	int i ;

	w = NULL ;
	p = option_sub_tbl[no] ;
	i = 1 ;
	for( ; ; p ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) break ;
		w->disp_flag = 1 ;
		w->work_func = option_menu_move ;
		w->sub_no = kind ;
		w->type = mw->type ;
		w->sub_type = i ++ ;
		w->base_mtx = &mw->obj_mtx ;
		w->obj = obj_dm0c->obj[p->obj] ;
		w->pos = p->pos ;
		w->scale = mw->scale ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = menu_fade ;
		if( p->next < 0 ) break ;
	}
	return w ;
}

/****************************************************************/
/*			A piece of a headline or a page, by its kind		*/
/****************************************************************/
PLAYER_WORK *headline_obj_set( Uint8 kind, Uint8 no )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = headline_move ;
		w->sub_no = kind ;
		w->type = no ;
		w->scale = menu_one ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	}
	return w ;
}

/****************************************************************/
/*			An item's parts, from headline_item_tbl				*/
/****************************************************************/
PLAYER_WORK *headline_item_set( PLAYER_WORK *mw, Uint8 kind, int no )
{
	PLAYER_WORK *w ;
	DM0C_ITEM *p ;
	int i ;

	w = NULL ;
	p = headline_item_tbl[no] ;
	i = 1 ;
	for( ; ; p ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) break ;
		w->disp_flag = 1 ;
		w->work_func = headline_move ;
		w->sub_no = kind ;
		w->type = mw->type ;
		w->sub_type = i ++ ;
		w->base_mtx = &mw->obj_mtx ;
		w->obj = obj_dm0c->obj[p->obj] ;
		w->pos = p->pos ;
		w->scale = p->scale ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = menu_fade ;
		w->routine_0 = 1 ;
		w->work1 = p->work ;
		headline_item_col( w ) ;
		if( p->next < 0 ) break ;
	}
	return w ;
}

/****************************************************************/
/*			A piece drawn in front: a number					*/
/****************************************************************/
PLAYER_WORK *headline_front_set( Uint8 kind, Uint8 no )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_FRONT, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = headline_move ;
		w->sub_no = kind ;
		w->type = no ;
		w->scale = menu_one ;
		w->work2 = 0 ;
		w->fade = menu_fade ;
	}
	return w ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void headline_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A headline's piece									*/
/****************************************************************/
void headline_label_init( PLAYER_WORK *w )
{
	DM0C_LABEL *p ;
	Uint16 no ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	if( !( p = headline_label_tbl[w->sub_type] ) ){
		work_free( w ) ;
		return ;
	}
	p += w->type ;
	if( w->type == 4 ) no = headline_label_obj[w->sub_type] ;
	else no = p->obj ;
	w->obj = obj_dm0c->obj[no] ;
	w->pos = p->pos ;
	w->scale = menu_one ;
	w->rot[2] = DEG_ANG( label_rot_tbl[p->rot] ) ;
	w->obj_flag = ( OBJ_ROT_Z | OBJ_TRNSL | OBJ_FADE ) | w->obj_flag ;
	w->fade.trnsl = 1.0f ;
	w->fade.r = p->r ;
	w->fade.g = p->g ;
	w->fade.b = p->b ;
	w->work3 = Sys->routine_0 << 8 | Sys->routine_1 ;
}

/****************************************************************/
/*			An item's colour: the cursor's item and its			*/
/*			setting lit											*/
/****************************************************************/
void headline_item_col( PLAYER_WORK *w )
{
	int no ;
	Sint8 sel ;
	Float col ;

	if( w->work1 ){
		no = headline_conf_no[w->type] ;
		sel = *headline_conf_tbl[no] ;
		if( no == 2 ) sel ^= 1 ;
		if( w->type == Sys->timer0 ){
			if( no >= 0 && sel == w->work1 - 1 ) col = 1.0f ;
			else col = 0.5f ;
		} else if( no >= 0 && sel == w->work1 - 1 ){
			col = 0.7f ;
			w->fade.r = 0.0f ;
			w->fade.g = w->fade.b = col ;
			return ;
		} else col = 0.19999999f ;
		goto set ;
	} else if( w->type == Sys->timer0 ) col = 1.0f ;
	else col = 0.7f ;
set :
	w->fade.r = w->fade.g = w->fade.b = col ;
}

/****************************************************************/
/*			An item of the game page, its parts and its			*/
/*			setting												*/
/****************************************************************/
void headline_item_init( PLAYER_WORK *w )
{
	DM0C_ITEM *p ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	p = &headline_tbl[w->type] ;
	w->obj = obj_dm0c->obj[p->obj] ;
	w->pos = p->pos ;
	w->scale = p->scale ;
	w->obj_flag = ( OBJ_TRNSL | OBJ_FADE ) | w->obj_flag ;
	w->work1 = 0 ;
	w->fade.trnsl = 1.0f ;
	headline_item_col( w ) ;
	headline_item_set( w, 3, w->type ) ;
	if( p->next ){
		if( p->next & 0x80 ) headline_front_set( 5, w->type ) ;
		else headline_obj_set( 6, w->type ) ;
	}
}

/****************************************************************/
/*			The cursor											*/
/****************************************************************/
void headline_cursor_init( PLAYER_WORK *w )
{
	DM0C_ITEM *p ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	p = &headline_tbl[Sys->timer0] ;
	w->obj = obj_dm0c->obj[87] ;
	w->pos = p->pos ;
	w->scale = p->scale ;
	w->obj_flag |= OBJ_TRNSL | OBJ_FADE ;
	w->fade = menu_fade ;
	w->work2 = Sys->timer0 ;
}

/****************************************************************/
/*			A page's title										*/
/****************************************************************/
void headline_title_init( PLAYER_WORK *w )
{
	DM0C_LABEL *p ;
	Uint16 no ;

	w->routine_0 ++ ;
	w->routine_1 = 2 ;
	w->disp_flag = 1 ;
	if( !( p = headline_label_tbl[w->sub_type] ) ){
		work_free( w ) ;
		return ;
	}
	p += w->type ;
	if( w->type == 4 ) no = headline_label_obj[w->sub_type] ;
	else no = p->obj ;
	if( w->sub_type == 18 ) w->obj = obj_dm0c->obj[no] ;
	else if( w->sub_type >= 16 ) w->obj = obj_dm10->obj[no] ;
	else w->obj = obj_dm0f->obj[no] ;
	w->pos = p->pos ;
	w->scale = menu_one ;
	w->rot[2] = DEG_ANG( label_rot_tbl[p->rot] ) ;
	w->obj_flag = ( OBJ_ROT_Z | OBJ_TRNSL | OBJ_FADE ) | w->obj_flag ;
	w->fade.trnsl = 1.0f ;
	w->fade.r = p->r ;
	w->fade.g = p->g ;
	w->fade.b = p->b ;
}

/****************************************************************/
/*			A level: the difficulty's or the damage's stars		*/
/****************************************************************/
void headline_level_init( PLAYER_WORK *w )
{
	register DM0C_CHAR *c, *p ;
	int i, n ;
	int lv ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos = level_pos_tbl[w->type >> 1] ;
	if( w->type == Sys->timer0 ){
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 1.0f ;
	} else {
		w->fade.r = 0.7f ;
		w->fade.g = 0.7f ;
		w->fade.b = 0.7f ;
	}
	w->fade.trnsl = 1.0f ;
	if( !w->type ){
		n = 8 ;
		lv = conf_difficulty ;
		c = level_char_tbl ;
	} else {
		n = 4 ;
		lv = conf_damage_level ;
		c = damage_char_tbl ;
	}
	w->work2 = ( Sint32 )c ;
	p = c ;
	for( i = 0 ; i < n ; p ++, i ++ ){
		if( i <= lv ) p->disp = 1 ;
		else p->disp = 0 ;
	}
}

/****************************************************************/
/*			A setting's mark									*/
/****************************************************************/
void headline_select_init( PLAYER_WORK *w )
{
	int sel ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	if( w->type == 1 ) sel = conf_round_time ;
	else if( w->type == 3 ) sel = conf_speed_fix ;
	else sel = conf_stereo ^ 1 ;
	w->pos = select_pos_tbl[select_tbl[w->type - 1] + sel] ;
	w->obj = obj_dm0c->obj[select_tbl[( Sint32 )( w->type + 3 )]] ;
	w->scale = menu_one ;
	w->obj_flag = ( OBJ_TRNSL | OBJ_FADE ) | w->obj_flag ;
	w->fade.r = 0.7f ;
	w->fade.g = 0.7f ;
	w->fade.b = 0.7f ;
	w->fade.trnsl = 1.0f ;
	w->timer0 = 0 ;
	w->timer1 = 0 ;
}

/****************************************************************/
/*			A piece's start, by its kind						*/
/****************************************************************/
void headline_init( PLAYER_WORK *w )
{
	headline_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void headline_nop2( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			An item each frame									*/
/****************************************************************/
void headline_item_main( PLAYER_WORK *w )
{
	headline_item_col( w ) ;
}

/****************************************************************/
/*			The cursor each frame: on its item, blinking		*/
/****************************************************************/
void headline_cursor_main( PLAYER_WORK *w )
{
	DM0C_ITEM *p ;

	p = &headline_tbl[Sys->timer0] ;
	w->pos = p->pos ;
	if( Sys->timer0 == 6 ){
		w->obj = obj_dm0c->obj[125] ;
		w->pos.z = -12.99f ;
		w->scale.x = 1.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
	} else {
		w->obj = obj_dm0c->obj[205] ;
		w->pos.z -= 0.01f ;
		w->scale.y = 0.65f ;
		if( Sys->timer0 == 5 ) w->scale.x = 3.28f ;
		else w->scale.x = 1.93f ;
	}
	if( w->work2 != Sys->timer0 ){
		w->work2 = Sys->timer0 ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}

/****************************************************************/
/*			A level each frame									*/
/****************************************************************/
void headline_level_main( PLAYER_WORK *w )
{
	DM0C_CHAR *c, *p ;
	int i, n ;
	int lv ;

	if( w->type == Sys->timer0 ){
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 1.0f ;
	} else {
		w->fade.r = 0.7f ;
		w->fade.g = 0.7f ;
		w->fade.b = 0.7f ;
	}
	if( !w->type ){
		n = 8 ;
		lv = conf_difficulty ;
		c = level_char_tbl ;
	} else {
		n = 4 ;
		lv = conf_damage_level ;
		c = damage_char_tbl ;
	}
	for( i = 0 ; i < n ; c ++, i ++ ){
		if( i <= lv ) c->disp = 1 ;
		else c->disp = 0 ;
	}
}

/****************************************************************/
/*			A setting's mark each frame							*/
/****************************************************************/
void headline_select_main( PLAYER_WORK *w )
{
	int sel ;

	if( w->type == Sys->timer0 ){
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
		w->fade.r = 1.0f ;
		w->fade.g = 1.0f ;
		w->fade.b = 1.0f ;
	} else {
		w->timer1 = 0 ;
		w->fade.r = 0.7f ;
		w->fade.g = 0.7f ;
		w->fade.b = 0.7f ;
		w->fade.trnsl = 1.0f ;
	}
	if( w->type == 1 ) sel = conf_round_time ;
	else if( w->type == 3 ) sel = conf_speed_fix ;
	else sel = conf_stereo ^ 1 ;
	w->pos = select_pos_tbl[select_tbl[w->type - 1] + sel] ;
}

/****************************************************************/
/*			A piece each frame, by its kind						*/
/****************************************************************/
void headline_main( PLAYER_WORK *w )
{
	headline_main_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			A piece of a headline or a page, each frame			*/
/****************************************************************/
void headline_move( PLAYER_WORK *w )
{
	headline_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			A line of the menu, or its cursor					*/
/****************************************************************/
void option_menu_init( PLAYER_WORK *w )
{
	DM0C_PARTS *p ;

	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->obj_flag |= OBJ_TRNSL | OBJ_FADE ;
	w->fade = menu_fade ;
	if( !w->sub_no ){
		p = &option_parts_tbl[w->type] ;
		w->obj = obj_dm0c->obj[p->obj] ;
		w->pos = p->pos ;
		option_menu_sub_set( w, 2, w->type ) ;
	} else if( w->sub_no == 1 ){
		Sys->timer0 ;	/* read, not used */
		w->obj = obj_dm0c->obj[142] ;
		w->work2 = Sys->timer0 ;
		w->timer1 = 0 ;
	}
}

/****************************************************************/
/*			The menu each frame: the cursor on its line,		*/
/*			blinking											*/
/****************************************************************/
void option_menu_main( PLAYER_WORK *w )
{
	DM0C_PARTS *p ;

	if( !w->sub_no ) w->disp_flag = 1 ;
	else if( w->sub_no == 1 ){
		p = &option_parts_tbl[Sys->timer0] ;
		w->pos = p->pos ;
		w->pos.z += 0.01f ;
		if( w->work2 != Sys->timer0 ){
			w->work2 = Sys->timer0 ;
			w->timer1 = 0 ;
		}
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	}
}

/****************************************************************/
/*			A piece of the menu, each frame						*/
/****************************************************************/
void option_menu_move( PLAYER_WORK *w )
{
	option_menu_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			A fade in over 30 frames: its start					*/
/****************************************************************/
void menu_fade_in_set( PLAYER_WORK *w )
{
	w->routine_1 ++ ;
	w->acc.x = 1.0f / 30.0f ;
	w->timer0 = 30 ;
	w->fade.trnsl = 0.0f ;
}

/****************************************************************/
/*			And each frame										*/
/****************************************************************/
void menu_fade_in( PLAYER_WORK *w )
{
	w->fade.trnsl += w->acc.x ;
	if( -- w->timer0 < 0 ){
		w->fade.trnsl = 1.0f ;
		w->routine_1 ++ ;
	}
}

/****************************************************************/
/*			Nothing to do										*/
/****************************************************************/
void menu_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			A piece's end										*/
/****************************************************************/
void dm0c_work_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

void dm0c_work_free2( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/****************************************************************/
/*			The game page's items and its cursor				*/
/****************************************************************/
void option_game_set( void )
{
	int i ;

	for( i = 0 ; i < 7 ; i ++ ) headline_obj_set( 1, i ) ;
	headline_obj_set( 2, 0 ) ;
}

/****************************************************************/
/*			A page's headline									*/
/****************************************************************/
PLAYER_WORK *headline_set( int no )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 5 ; i ++ ){
		if( no <= 8 ) w = headline_obj_set( 0, i ) ;
		else w = headline_obj_set( 4, i ) ;
		w->sub_type = no ;
	}
	return w ;
}

/****************************************************************/
/*			And its last two pieces								*/
/****************************************************************/
PLAYER_WORK *headline_sub_set( int no )
{
	PLAYER_WORK *w ;

	w = NULL ;
	if( no == 13 ){
		w = headline_obj_set( 4, 5 ) ;
		w->sub_type = no ;
		w = headline_obj_set( 4, 6 ) ;
		w->sub_type = no ;
	} else if( no <= 8 ){
		w = headline_obj_set( 0, 5 ) ;
		w->sub_type = no ;
		w = headline_obj_set( 0, 6 ) ;
		w->sub_type = no ;
	}
	return w ;
}

/****************************************************************/
/*			A number of the menu: its characters, from work2	*/
/****************************************************************/
void menu_obj_disp( PLAYER_WORK *w )
{
	DM0C_CHAR *c ;
	Float x ;

	if( !w->disp_flag ) return ;
	if( !( c = ( DM0C_CHAR * )w->work2 ) ) return ;
	nlPushMatrix( 0 ) ;
	nlUnitMatrix() ;
	nlTranslateV( &w->pos ) ;
	nlGetMatrix( &w->obj_mtx ) ;
	nlPopMatrix( 1 ) ;
	scr_2d_matrix_set() ;
	nlPushMatrix( 0 ) ;
	nlLoadMatrix( &w->obj_mtx ) ;
	x = 0.0f ;
	for( ; c->disp >= 0 ; c ++ ){
		if( c->disp ){
			nlTranslate( x + c->ofs, 0.0f, 0.0f ) ;
			if( c->scale ){
				nlScaleV( &menu_one ) ;
				nlSetScaleFactor( obj_scale_max( menu_one ) ) ;
			}
			nlObjPutSetFadeColor( w->fade.r, w->fade.g, w->fade.b ) ;
			nlObjPutTrnsl( obj_dm0c->obj[c->code], w->fade.trnsl ) ;
			x = 0.0f ;
		} else x = c->ofs ;
	}
	nlPopMatrix( 1 ) ;
}
