/* The select scene's card of a side (kind 5 works), and the cursor's place on the grid
   (sel_cursor_*). */
#include "player.h"
#include "select.h"

extern void sel_spin_set( PLAYER_WORK *ew );
extern void obj_uv_begin( int *src, void *obj );
extern int obj_uv_next( void );
extern void obj_uv_get( Float *x, Float *y );
extern void obj_uv_put( Float *x, Float *y );
extern void obj_uv_clear( void *obj );
extern void look_rotate( VEC3 *from, VEC3 *to, Sint32 roll );

/* the card slots as bits (taken from the table's order) */
static Uint32 ef05_bit_tbl[32] = {
	0x10000000, 0x80, 0x80000000, 0x40000000, 0x8, 0x20000000, 0x8000000, 0x1,
	0x1000000, 0x2000, 0x400000, 0x8, 0x4000, 0x80, 0x8000, 0x800,
	0x2, 0x100, 0x200000, 0x20, 0x800000, 0x200, 0x20000, 0x40,
	0x1000, 0x40000, 0x10, 0x80000, 0x4000000, 0x100000, 0x10000, 0x400,
} ;

/* which character stands at each cell of the grid, -1 for none: rows from the top, then columns */
static Sint8 ef05_grid_tbl[8][8] = {
	{ -1, 0x20, 0x1c, 0x26, 0x08, 0x0a, 0x36, -1 },
	{ -1, 0x11, 0x02, 0x15, 0x06, 0x2c, 0x2e, -1 },
	{ 0x27, 0x10, 0x00, 0x14, 0x16, 0x07, 0x2f, 0x32 },
	{ 0x21, 0x1e, 0x01, 0x13, 0x17, 0x09, 0x28, 0x35 },
	{ 0x25, 0x37, 0x04, 0x12, 0x0b, 0x0c, 0x29, 0x31 },
	{ 0x23, 0x22, 0x03, 0x1f, 0x0f, 0x0e, 0x39, 0x2b },
	{ -1, 0x24, 0x05, 0x38, 0x2d, 0x0d, 0x33, -1 },
	{ -1, 0x3a, 0x1d, 0x1b, 0x30, 0x2a, 0x34, -1 },
} ;

static void ef05_card_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The card that holds a side's picture: it comes		*/
/*			in, follows the chosen character and goes out		*/
/****************************************************************/
void ef05_card_set( Uint8 side, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->type = side ;
	ew->sub_type = no ;
	ew->work_func = ef05_card_main ;
	ew->base_mtx = &sel_back_w->obj_mtx ;
	ew->obj = obj_dm01->obj[side * 2 + 151] ;
	ew->rot[0] = 0 ;
	ew->rot[1] = 0 ;
	ew->rot[2] = 0 ;
	ew->obj_flag = OBJ_SCALE | OBJ_TRNSL | OBJ_BRIGHT | OBJ_MTX ;
	ew->scale.x = 1.2f ;
	ew->scale.y = 1.2f ;
	ew->scale.z = 1.2f ;
	ew->fade.trnsl = 1.0f ;
	ew->pl_type = 0xff ;
	obj_uv_clear( ew->obj ) ;
	if( ew->type ) ew->rot[2] = -0x2000 ;
	else ew->rot[2] = 0x2000 ;
	sel_spin_set( ew ) ;
}

static void ef05_card_in( PLAYER_WORK *ew );
static void ef05_card_move( PLAYER_WORK *ew );
static void ef05_card_out( PLAYER_WORK *ew );
static void ef05_card_free( PLAYER_WORK *ew );
static void ef05_obj_mtx( PLAYER_WORK *ew );
static void ef05_card_pic( PLAYER_WORK *ew );

/*==============================================================*/
static void ef05_card_main( PLAYER_WORK *ew )
{
	static void ( *ef05_card_jmp[] )( PLAYER_WORK *ew ) = {
		ef05_card_in, ef05_card_move, ef05_card_out, ef05_card_free,
	} ;

	ef05_card_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It shrinks to its size								*/
/*==============================================================*/
static void ef05_card_in( PLAYER_WORK *ew )
{
	ew->scale.x -= 0.04f ;
	ew->scale.y -= 0.04f ;
	ew->scale.z -= 0.04f ;
	if( ew->scale.x <= 1.0f ){
		ew->routine_0 ++ ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
	ef05_card_move( ew ) ;
	ef05_obj_mtx( ew ) ;
}

/*==============================================================*/
/*			It takes the picture of the character the side's	*/
/*			cursor is on, until the side has chosen				*/
/*==============================================================*/
static void ef05_card_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *tw ;

	ef05_card_pic( ew ) ;
	if( sel_w.pick_flag[ew->type] & ( 1 << ew->sub_type ) ) ew->routine_0 = 2 ;
	else{
		tw = pl_work[ew->type].master_w ;
		if( tw->load_type != ew->pl_type ){
			ew->pl_type = tw->load_type ;
			sel_w.cur_char[ew->type] = ew->pl_type + 1 ;
		}
	}
	ef05_obj_mtx( ew ) ;
}

/*==============================================================*/
/*			It grows and fades out								*/
/*==============================================================*/
static void ef05_card_out( PLAYER_WORK *ew )
{
	ew->scale.x += 0.01f ;
	ew->scale.y += 0.01f ;
	ew->fade.trnsl -= 0.05f ;
	if( ew->fade.trnsl <= 0.0f ){
		ew->fade.trnsl = 0.0f ;
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else ef05_obj_mtx( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef05_card_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The model's matrix: its place, turned to face		*/
/*			the character's place in the grid					*/
/*==============================================================*/
static void ef05_obj_mtx( PLAYER_WORK *ew )
{
	nlPushMatrix( 0 ) ;
	nlLoadMatrix( &ew->obj_mtx ) ;
	nlUnitMatrix() ;
	nlTranslateV( &ew->pos ) ;
	look_rotate( &ew->pos, &ef05_pos_tbl[ew->pl_type], ew->rot[2] ) ;
	nlGetMatrix( &ew->obj_mtx ) ;
	nlPopMatrix( 1 ) ;
}

/*==============================================================*/
/*			The picture on the card slides along its texture	*/
/*==============================================================*/
static void ef05_card_pic( PLAYER_WORK *ew )
{
	Float px, py ;

	ew->timer0 ++ ;
	if( ew->timer0 >= 50 ) ew->timer0 = 0 ;
	obj_uv_begin( obj_dm01->obj[ew->type * 2 + 151 + 1], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py += 0.02f * ew->timer0 ;
		obj_uv_put( &px, &py ) ;
	}
}

/****************************************************************/
/*			The cursor's cell from the character, by the		*/
/*			grid												*/
/****************************************************************/
void sel_cursor_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tw = pl_w->master_w ;

	sel_w.cur_col[pl_w->pl_index] = ef05_cell_tbl[tw->load_type].col ;
	sel_w.cur_row[pl_w->pl_index] = ef05_cell_tbl[tw->load_type].row ;
}

/****************************************************************/
/*			The cursor goes round the edges of the grid			*/
/****************************************************************/
void sel_cursor_wrap( Uint8 side )
{
	if( sel_w.cur_row[side] >= 8 ) sel_w.cur_row[side] = 0 ;
	if( sel_w.cur_row[side] < 0 ) sel_w.cur_row[side] = 7 ;
	if( sel_w.cur_col[side] >= 8 ) sel_w.cur_col[side] = 0 ;
	if( sel_w.cur_col[side] < 0 ) sel_w.cur_col[side] = 7 ;
}

/****************************************************************/
/*			The character under a side's cursor, -1 for none	*/
/****************************************************************/
Sint8 sel_cursor_char( Uint8 side )
{
	return ef05_grid_tbl[sel_w.cur_row[side]][sel_w.cur_col[side]] ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ef05_card.c" End of File	*/
