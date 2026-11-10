/* The pieces on the select grid's cells (kind 5 works): one for each character the sides have, whose texture
   scrolls and which swells when a side has chosen it. */
#include "player.h"
#include "select.h"

extern void obj_uv_begin( int *src, void *obj );
extern int obj_uv_next( void );
extern void obj_uv_get( Float *x, Float *y );
extern void obj_uv_put( Float *x, Float *y );
extern void obj_uv_clear( void *obj );

/* the watcher's own work: the characters it has made a piece for, a bit each */
typedef struct SEL_CELL_W {
	Sint32	mask[2];
} SEL_CELL_W;
#define SEL_CELL_LW( w )	( ( SEL_CELL_W * )&( w )->obj_flag )

static void sel_cell_watch( PLAYER_WORK *ew );
static void sel_cell_make( Uint8 no );

/****************************************************************/
/*			The watcher, and a piece for each character the		*/
/*			sides have now										*/
/****************************************************************/
void sel_cell_make_all( void )
{
	PLAYER_WORK *ew ;
	Uint8 i ;
	Uint8 j ;
	Sint32 bit ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->work_func = sel_cell_watch ;
	SEL_CELL_LW( ew )->mask[0] = SEL_CELL_LW( ew )->mask[1] = 0 ;
	if( Sys->entry_side & 1 ){
		SEL_CELL_LW( ew )->mask[0] |= sel_w.char_ex[0][0] ;
		SEL_CELL_LW( ew )->mask[1] |= sel_w.char_ex[0][1] ;
	}
	if( Sys->entry_side & 2 ){
		SEL_CELL_LW( ew )->mask[0] |= sel_w.char_ex[1][0] ;
		SEL_CELL_LW( ew )->mask[1] |= sel_w.char_ex[1][1] ;
	}
	for( i = 0 ; i < 2 ; i ++ ){
		for( j = 0 ; j < 32 ; j ++ ){
			bit = 1 << j ;
			if( SEL_CELL_LW( ew )->mask[i] & bit ) sel_cell_make( i * 32 + j ) ;
		}
	}
}

static void sel_cell_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			The piece on the cell of character no				*/
/*==============================================================*/
static void sel_cell_make( Uint8 no )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->base_mtx = &sel_back_w->obj_mtx ;
	ew->work_func = sel_cell_main ;
	ew->sub_type = no ;
	ew->pos = ef05_pos_tbl[no] ;
	ew->timer1 = ef05_cell_tbl[no].row * 2 ;
	ew->pos.z += 0.5f ;
	ew->obj = obj_dm01->obj[ew->timer1 + 157] ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_BRIGHT ;
	ef05_rot_set( ew, ew->sub_type ) ;
	ew->timer0 = 0 ;
	obj_uv_clear( ew->obj ) ;
}

/*==============================================================*/
/*			A piece for each character that has come to the		*/
/*			sides' masks since									*/
/*==============================================================*/
static void sel_cell_watch( PLAYER_WORK *ew )
{
	Sint32 now[2] ;
	Sint8 i ;
	Sint8 j ;
	Sint32 bit ;

	now[0] = now[1] = 0 ;
	if( Sys->entry_side & 1 ){
		now[0] |= sel_w.char_ex[0][0] ;
		now[1] |= sel_w.char_ex[0][1] ;
	}
	if( Sys->entry_side & 2 ){
		now[0] |= sel_w.char_ex[1][0] ;
		now[1] |= sel_w.char_ex[1][1] ;
	}
	for( i = 0 ; i < 2 ; i ++ ){
		now[i] ^= SEL_CELL_LW( ew )->mask[i] ;
		if( now[i] ){
			for( j = 0 ; j < 32 ; j ++ ){
				bit = 1 << j ;
				if( now[i] & bit ){
					SEL_CELL_LW( ew )->mask[i] |= bit ;
					sel_cell_make( i * 32 + j ) ;
				}
			}
		}
	}
}

/*==============================================================*/
/*			The piece scrolls its texture, and swells when a	*/
/*			side has chosen its character						*/
/*==============================================================*/
static void sel_cell_main( PLAYER_WORK *ew )
{
	Float py, px ;
	Sint16 i ;
	Uint8 no ;

	switch( ew->routine_0 ){
	case 0:
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->pos.z = ef05_pos_tbl[ew->sub_type].z + 0.5f ;
		ew->pos.x = ef05_pos_tbl[ew->sub_type].x ;
		if( Sys->exit_flag ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			break ;
		}
		ew->timer0 ++ ;
		if( ew->timer0 >= 500 ) ew->timer0 = 0 ;
		obj_uv_begin( obj_dm01->obj[ew->timer1 + 157 + 1], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &px, &py ) ;
			py -= ew->timer0 * 0.002f ;
			obj_uv_put( &px, &py ) ;
		}
		for( i = 0 ; i < 2 ; i ++ ){
			if( sel_w.cur_char[i] ){
				no = sel_w.cur_char[i] - 1 ;
				if( ew->sub_type == no ){
					if( i ){
						ew->scale.x = 1.15f ;
						ew->scale.y = 1.15f ;
						ew->scale.z = 1.15f ;
						ew->pos.z = ef05_pos_tbl[no].z + 6.5f ;
						if( ew->pos.x < 0.0f ) ew->pos.x = ef05_pos_tbl[no].x + -2.0f ;
						else ew->pos.x = ef05_pos_tbl[no].x + 2.0f ;
					}
					else{
						ew->scale.x = 1.2f ;
						ew->scale.y = 1.2f ;
						ew->scale.z = 1.2f ;
						ew->pos.z = ef05_pos_tbl[no].z + 8.5f ;
						if( ew->pos.x < 0.0f ) ew->pos.x = ef05_pos_tbl[no].x + -2.0f ;
						else ew->pos.x = ef05_pos_tbl[no].x + 2.0f ;
					}
				}
			}
		}
		break ;
	case 1:
		work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_cell.c" End of File	*/
