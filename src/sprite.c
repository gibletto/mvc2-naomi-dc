/* Sprite textures, palettes and drawing: the system's sprite textures, the palette sent to the
   hardware each frame, and the routines that put the works' 2D sprites on the screen. */
#include "player.h"

/* one cell of a sprite: where it goes, its palette and flips, and its number */
typedef struct SPR_PART {
	Sint16	x, y;		/* 000: its offset from the cell before */
	Uint16	attr;		/* 004: its first colour (PART_COL) and its flips */
	Uint16	no;		/* 006: the cell */
} SPR_PART;

/* a cell's attr: its first colour and its flips */
#define PART_COL	0x03ff	/* mask: the cell's first colour, a palette line * 16 */
#define PART_FLIP_V	0x4000	/* the cell flipped top to bottom */
#define PART_FLIP_H	0x8000	/* the cell flipped left to right */

/* a sprite is a count of its cells, then the cells */
#define PART( p )	( ( SPR_PART * )( p ) )

/* one tile of a sprite cut up for this frame's textures */
typedef struct SPR_TILE {
	Uint8	size;		/* 000: its side, 8, 16 or 32 */
	Uint8	cnt;		/* 001: the tiles of its cell after it */
	Uint8	x, y;		/* 002: its place in the cell, in tiles */
} SPR_TILE;

#define SYS_TEX		( ( void * )0x0CE60000 )	/* the system texture */
#define SYS_TEX_LIST	( ( NLthd * )0x0CE60C00 )	/* its list */
#define FONT_TEX	( ( void * )0x0CC00000 )
#define SPR_BUFF	( ( Uint8 * )0x0CE60000 )	/* where the cells are unpacked */

static Sint32 spr_frame_cnt;	/* a frame count */
static Sint32 spr_list_type;	/* the sprites' list type */
static Sint32 spr_attr;	/* the sprites' depth test */
static Float unk_0C283058;
static Uint32 tile_load_top;	/* the textures loaded this frame start here */
static Uint32 tile_disp_top;	/* the textures drawn this frame start here */
static Sint32 tile_cnt;	/* the tiles used this frame */
static Uint16 cell_no_buff[0x100];	/* the cells of a sprite set loaded so far */
static SPR_TILE tile_buff[0x300];	/* the frame's tiles */
static SPR_TILE *tile_p;	/* the next one */
extern const Uint8 *tile_row_tbl[];	/* the places of a cell's tiles, for each row */
extern const Uint8 *tile_col_tbl[];	/* and for each column */
extern Sint16 spr_set_cnt_tbl[];	/* each character's number of sprite sets */

static int sprite_mode_set( int mode ) ;

/* the box display's sprite */
static NLsprarg box_arg = {
	3, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 1.0f, 2, 0x40a, -1, 0
} ;

/* the font texture */
static NLthd font_thd[] = {
	{ 16, 16, 0x502, FONT_TEX, 0 },
	{ -1, -1, -1, ( void * )-1, -1 }
} ;

/* the depths of the sprite layers, 15 to 29 by 2: defined in an earlier file, as they lie before
   message.c's constants */
extern const Float prio_z_tbl[8] ;

/****************************************************************/
/*			Set up the sprite system and its texture			*/
/****************************************************************/
void sprite_init( void )
{
	NLthd *tp ;
	Sint32 mode ;
	int i ;

	if( sprite_keep_flag ) return ;
	spr_frame_cnt = 0 ;
	unk_0C283058 = 1.0f ;
	tile_load_top = 16 ;
	tile_disp_top = 400 ;
	tile_cnt = 0 ;
	tile_p = tile_buff ;
	mode = 0x502 ;
	if( sprite_keep_flag ) mode = 0x101 ;
	memset( SYS_TEX, 0, 0xc00 ) ;
	tp = SYS_TEX_LIST ;
	i = 0x300 ;
	do {
		tp->width = 32 ;
		tp->height = 32 ;
		tp->type = mode ;
		tp->address = SYS_TEX ;
		tp->globl_index = 0 ;
		tp ++ ;
	} while( --i ) ;
	tp->width = -1 ;
	tp->height = -1 ;
	tp->type = -1 ;
	tp->address = ( void * )-1 ;
	tp->globl_index = -1 ;
	nlSetCommonTextureOffset( tile_load_top ) ;
	nlSpriteLoadTexture( SYS_TEX_LIST, SYS_TEX ) ;
	nlSetCommonTextureOffset( 0 ) ;
	sprite_mode_set( 0 ) ;
}

/*==============================================================*/
/*			Set the sprites' list type and attributes for a		*/
/*			mode (both list choices are the translucent			*/
/*			list, 2)											*/
/*==============================================================*/
static int sprite_mode_set( int mode )
{
	spr_list_type = ( mode & 1 ) ? 2 : 2 ;
	spr_attr = ( mode & 2 ) ? 0x1400 : 0x400 ;
	return mode & 7 ;
}

/****************************************************************/
/*			Load the font texture								*/
/****************************************************************/
void font_tex_load( void )
{
	nlSetCommonTextureOffset( 14 ) ;
	nlSpriteLoadTexture( font_thd, FONT_TEX ) ;
	nlSetCommonTextureOffset( 0 ) ;
}

/****************************************************************/
/*			Each frame: step the sprite system, and send the	*/
/*			palette lines that changed							*/
/****************************************************************/
void col_send( void )
{
	COL_WORK *cw ;
	Uint16 *src, *dst, *top ;
	int i ;
	Uint16 c ;
	Sint16 a, r, g, b ;
	Uint32 n ;

	if( !Sys->task_stop && !Sys->join_flag ){
		spr_frame_cnt ++ ;
		if( ( unk_0C283058 += 0.01f ) > 1.0f ) unk_0C283058 = 0.5f ;
	}
	if( ( Sys->task_stop + spr_frame_cnt ) & 1 ){
		tile_load_top = 400 ;
		tile_disp_top = 16 ;
	} else {
		tile_load_top = 16 ;
		tile_disp_top = 400 ;
	}
	Sys->spr_cnt = 0 ;
	prio_z[8] = 10.0f ;
	prio_z[9] = 11.0f ;
	prio_z[10] = 12.0f ;
	prio_z[11] = 13.0f ;
	prio_z[12] = 30.0f ;
	prio_z[13] = 31.0f ;
	prio_z[14] = 32.0f ;
	prio_z[15] = 33.0f ;
	for( n = 0 ; n < 8 ; n ++ ) prio_z[n] = prio_z_tbl[n] ;
	tile_cnt = 0 ;
	tile_p = tile_buff ;
	nlSetCommonTextureOffset( 0 ) ;
	cw = col_work ;
	i = 0 ;
	do {
		if( cw->trans_flag ){
			src = cw->col ;
			dst = &col_send_buff[i * 16] ;
			top = dst ;
			n = 16 ;
			do {
				if( cw->trans_flag & 1 ) *dst ++ = *src ++ ;
				else if( cw->trans_flag & 2 ){
					a = ( c = *src ) & 0xf000 ; src ++ ;
					r = ( ( c & 0x0f00 ) >> 1 ) & 0x0f00 ;
					g = ( ( c & 0x00f0 ) >> 1 ) & 0x00f0 ;
					b = ( c & 0x000f ) >> 1 & 0x000f ;
					*dst ++ = a | r | g | b ;
				}
				n -- ;
			} while( n ) ;
			cw->trans_flag = 0 ;
			nlSetPaletteData( cw->pal_no * 16, top, cw->cnt ) ;
		}
		cw ++ ;
		i ++ ;
	} while( i < 0x80 ) ;
}

/****************************************************************/
/*			Copy cnt palette lines to the palette from line		*/
/*			no on; file_load passes a player's colours			*/
/****************************************************************/
void col_set( void *col, int no, int cnt )
{
	COL_WORK *cw = &col_work[no] ;
	Uint16 *adrs = col ;
	Uint16 *dst ;
	int i, j ;

	i = 0 ;
	while( i < cnt ){
		cw->trans_flag = 1 ;
		dst = cw->col ;
		for( j = 0 ; j < 16 ; j ++ ) *dst ++ = *adrs ++ ;
		cw ++ ;
		i ++ ;
	}
}

/****************************************************************/
/*			Unpack a work's sprite and load its cells as		*/
/*			this frame's textures, cut into tiles; returns		*/
/*			the tiles used so far, or -1 when they run out		*/
/****************************************************************/
int sprite_tex_set( PLAYER_WORK *w )
{
	Uint16 *p ;
	Uint8 *cell ;
	const Uint8 *pos ;
	Uint8 *dst ;
	Sint16 cw, ch ;
	Uint32 size ;
	int bytes, tile_size, nx, ny, tex, col, cnt, n, i, j ;

	if( w->spr_no == -1 ) return tile_cnt ;
	if( w->spr_no & SPR_NO_SET ) return tile_cnt ;
	w->tile_top = tile_cnt ;
	dst = SPR_BUFF ;
	cache_inv( dst, 0x20000 ) ;
	p = ( Uint16 * )( w->spr_adrs + ( ( Sint32 * )w->spr_adrs )[w->spr_no & SPR_NO_MASK] ) ;
	cnt = *p ;
	p = p + 1 ;
	i = 0 ;
	while( cnt > i ){
		if( w->col_flag == 0 ) col = ( PART( p )->attr & 0x3ff ) >> 4 ;
		else col = ( w->col_no * 16 + PART( p )->attr & 0x3ff ) >> 4 ;
		cell = w->cell_adrs + ( ( Sint32 * )w->cell_adrs )[PART( p )->no] ;
		cell += 2 ;
		cw = *cell ++ << 3 ;
		ch = *cell ++ << 3 ;
		size = cw * ch >> 1 ;
		lz_unpack( ( Sint8 * )cell, size, dst ) ;
		if( cw <= ch ){
			nx = 1 ;
			if( cw == 16 ){
				bytes = 0x80 ;
				tile_size = 16 ;
			} else if( cw == 8 ){
				bytes = 32 ;
				tile_size = 8 ;
			} else {
				bytes = 0x200 ;
				tile_size = 32 ;
				nx = cw / tile_size ;
			}
			ny = ch / tile_size ;
			tex = tile_cnt + tile_load_top ;
			n = size / bytes ;
			pos = tile_col_tbl[nx] ;
		} else {
			ny = 1 ;
			if( ch == 16 ){
				bytes = 0x80 ;
				tile_size = 16 ;
			} else if( ch == 8 ){
				bytes = 32 ;
				tile_size = 8 ;
			} else {
				bytes = 0x200 ;
				tile_size = 32 ;
				ny = ch / tile_size ;
			}
			tex = tile_cnt + tile_load_top ;
			n = size / bytes ;
			pos = tile_row_tbl[ny] ;
		}
		if( tile_cnt + n > 0x180 ) return -1 ;
		for( j = 0 ; j < n ; j ++ ){
			nlSpriteChangePalette( tex, col ) ;
			nlCommonTextureReloadA( tex, dst ) ;
			tile_p->size = tile_size ;
			tile_p->cnt = n - 1 ;
			tile_p->x = *pos ++ ;
			tile_p->y = ny - *pos ++ ;
			tile_p ++ ;
			tex ++ ;
			dst += bytes ;
		}
		tile_cnt += n ;
		p += 4 ;
		i ++ ;
	}
	return tile_cnt ;
}

/****************************************************************/
/*			Unpack all of a work's sprites and load their		*/
/*			cells, each once, as texture set no; returns the	*/
/*			bytes loaded										*/
/****************************************************************/
int sprite_set_load( PLAYER_WORK *w, int id )
{
	Uint16 *p, *top ;
	Uint8 *buf, *map, *cell, *dst, *adrs ;
	Sint32 *pal_list, *pal ;
	NLthd *thd_list, *thd ;
	int cnt, n, i, j, k, m, used, total, size, add, no ;

	no = id ;
	buf = w->spr_set_buff ;
	top = ( Uint16 * )buf ;
	map = buf + 0x220 ;
	pal_list = ( Sint32 * )( buf + 0xa20 ) ;
	buf += 0xe20 ;
	thd_list = ( NLthd * )buf ;
	pal = pal_list ;
	thd = thd_list ;
	used = 0 ;
	total = 0 ;
	k = 0 ;
	cnt = spr_set_cnt_tbl[w->load_type] ;
	dst = SPR_BUFF ;
	cache_inv( dst, 0x20000 ) ;
	for( i = 0 ; i < cnt ; i ++ ){
		if( 0 ) k = used ;
		adrs = w->spr_adrs ;
		p = ( Uint16 * )( adrs + ( ( Sint32 * )adrs )[i] ) ;
		n = *p ;
		p ++ ;
		top[i] = k ;
		for( j = 0 ; n > j ; j ++ ){
			add = 1 ;
			for( m = 0 ; m < used ; m ++ ){
				if( cell_no_buff[m] == PART( p )->no ){
					add = 0 ;
					break ;
				}
			}
			map[k ++] = m ;
			if( add ){
				cell_no_buff[used] = PART( p )->no ;
				if( w->col_flag == 0 ) *pal = ( PART( p )->attr & 0x3ff ) >> 4 ;
				else *pal = ( w->col_no * 16 + PART( p )->attr & 0x3ff ) >> 4 ;
				cell = w->cell_adrs + ( ( Sint32 * )w->cell_adrs )[PART( p )->no] ;
				cell += 2 ;
				thd->width = *cell ++ << 3 ;
				thd->height = *cell ++ << 3 ;
				thd->type = 0x502 ;
				thd->address = dst ;
				thd->globl_index = 0 ;
				size = thd->width * thd->height >> 1 ;
				lz_unpack( ( Sint8 * )cell, size, dst ) ;
				thd ++ ;
				dst += size ;
				used ++ ;
				total += size ;
				pal ++ ;
			}
			p += 4 ;
		}
	}
	if( cnt ){
		thd->width = -1 ;
		thd->height = -1 ;
		thd->type = -1 ;
		thd->address = ( void * )-1 ;
		thd->globl_index = -1 ;
		nlSetCommonTextureOffset( no * 256 + 0x390 ) ;
		nlSpriteLoadTexture( thd_list, pal_list ) ;
		nlSetCommonTextureOffset( 0 ) ;
	}
	spr_set_tex_cnt[no] = used ;
	spr_set_size[no] = total ;
	return total ;
}

/****************************************************************/
/*			Free texture set no									*/
/****************************************************************/
void sprite_set_free( PLAYER_WORK *w, int no )
{
	Sint32 *pal_list ;
	NLthd *thd_list ;

	if( spr_set_tex_cnt[no] ){
		pal_list = ( Sint32 * )( w->spr_set_buff + 0xa20 ) ;
		thd_list = ( NLthd * )( w->spr_set_buff + 0xe20 ) ;
		nlSetCommonTextureOffset( no * 256 + 0x390 ) ;
		nlSpriteFreeTexture( thd_list, pal_list ) ;
		nlSetCommonTextureOffset( 0 ) ;
	}
}

/****************************************************************/
void sprite_dummy( void )
{
}

/*==============================================================*/
/*			Mark a work's position								*/
/*==============================================================*/
static void pos_mark_disp( PLAYER_WORK *w )
{
	box_arg.sprno = 3 ;
	box_arg.x = w->disp_x ;
	box_arg.y = w->disp_y ;
	box_arg.z -= 0.001f ;
	box_arg.attr = NLSPR_DISP_CC | NLSPR_DEPTH_GREATER ;
	nlSprPut( &box_arg ) ;
}

/*==============================================================*/
/*			Draw the four corners of one of a work's boxes		*/
/*==============================================================*/
static void box_disp( PLAYER_WORK *w, HIT_BOX *box, Float scale )
{
	Float sx, sy, x, y, bw, bh ;

	sx = w->scale.x * 1.6666666f / scale ;
	sy = w->scale.y * 2.142857f / scale ;
	if( !w->disp_flip ) x = w->disp_x + box->x * sx ;
	else x = w->disp_x - box->x * sx ;
	bw = box->w * sx ;
	y = w->disp_y + box->y * sy ;
	bh = -box->h * sy ;
	box_arg.x = x + bw ;
	box_arg.y = y + bh ;
	box_arg.z -= 0.001f ;
	box_arg.attr = NLSPR_DISP_RT | NLSPR_FLIP_H | NLSPR_DEPTH_GREATER ;
	nlSprPut( &box_arg ) ;
	box_arg.x = x + bw ;
	box_arg.y = y - bh ;
	box_arg.z -= 0.001f ;
	box_arg.attr = NLSPR_DISP_RB | NLSPR_FLIP_H | NLSPR_FLIP_V | NLSPR_DEPTH_GREATER ;
	nlSprPut( &box_arg ) ;
	box_arg.x = x - bw ;
	box_arg.y = y + bh ;
	box_arg.z -= 0.001f ;
	box_arg.attr = NLSPR_DISP_LT | NLSPR_DEPTH_GREATER ;
	nlSprPut( &box_arg ) ;
	box_arg.x = x - bw ;
	box_arg.y = y - bh ;
	box_arg.z -= 0.001f ;
	box_arg.attr = NLSPR_DISP_LB | NLSPR_FLIP_V | NLSPR_DEPTH_GREATER ;
	nlSprPut( &box_arg ) ;
}

/****************************************************************/
/*			Show a work's boxes for its current frame			*/
/****************************************************************/
void hit_box_disp( PLAYER_WORK *w )
{
	FRAME_BOX *hb ;
	Float scale ;
	HIT_BOX box ;

	hb = &w->frame_box_tbl[( ( CHAR_DATA * )w->cg_adrs )->frame_box_no] ;
	box_arg.z = w->disp_z ;
	box_arg.trnsl = 1.0f ;
	if( w->work_kind != W_PL && w->work_kind != W_SHL ) goto mark ;
	scale = scr_work.eye[1].z / 812.3573f ;
	if( hb->body_box0 ){
		box_arg.sprno = 5 ;
		box_disp( w, &w->hit_box_tbl[( Uint8 )hb->body_box0], scale ) ;
	}
	if( hb->body_box1 ){
		box_arg.sprno = 6 ;
		box_disp( w, &w->hit_box_tbl[( Uint8 )hb->body_box1], scale ) ;
	}
	if( hb->body_box2 ){
		box_arg.sprno = 7 ;
		box_disp( w, &w->hit_box_tbl[( Uint8 )hb->body_box2], scale ) ;
	}
	if( hb->catch_box ){
		box_arg.sprno = 8 ;
		box_disp( w, &w->hit_box_tbl[( Uint8 )hb->catch_box], scale ) ;
	}
	if( hb->atck_box ){
		box_arg.sprno = 4 ;
		box_disp( w, &w->hit_box_tbl[( Uint8 )hb->atck_box], scale ) ;
	}
	if( hb->atck_box2 ){
		box_arg.sprno = 10 ;
		box_disp( w, &w->hit_box_tbl[( Uint8 )hb->atck_box2], scale ) ;
	}
	if( w->work_kind == W_PL ){
		box_arg.sprno = 9 ;
		box = ( w->hit_box_tbl + w->pl_stance )[PUSH_BOX] ;
		box.y = -box.y ;
		box_disp( w, &box, scale ) ;
	}
mark :
	pos_mark_disp( w ) ;
}

/****************************************************************/
/*			The same at full size, with box sel solid and		*/
/*			the others faint									*/
/****************************************************************/
void hit_box_sel_disp( PLAYER_WORK *w, int sel )
{
	FRAME_BOX *hb ;
	HIT_BOX box ;

	hb = &w->frame_box_tbl[( ( CHAR_DATA * )w->cg_adrs )->frame_box_no] ;
	box_arg.z = w->disp_z ;
	if( w->work_kind == W_PL || w->work_kind == W_SHL ){
		if( hb->body_box0 ){
			if( sel == 2 ) box_arg.trnsl = 1.0f ;
			else box_arg.trnsl = 0.5f ;
			box_arg.sprno = 5 ;
			box_disp( w, &w->hit_box_tbl[( Uint8 )hb->body_box0], 1.0f ) ;
		}
		if( hb->body_box1 ){
			if( sel == 3 ) box_arg.trnsl = 1.0f ;
			else box_arg.trnsl = 0.5f ;
			box_arg.sprno = 6 ;
			box_disp( w, &w->hit_box_tbl[( Uint8 )hb->body_box1], 1.0f ) ;
		}
		if( hb->body_box2 ){
			if( sel == 4 ) box_arg.trnsl = 1.0f ;
			else box_arg.trnsl = 0.5f ;
			box_arg.sprno = 7 ;
			box_disp( w, &w->hit_box_tbl[( Uint8 )hb->body_box2], 1.0f ) ;
		}
		if( hb->body_box3 ){
			if( sel == 5 ) box_arg.trnsl = 1.0f ;
			else box_arg.trnsl = 0.5f ;
			box_arg.sprno = 6 ;
			box_disp( w, &w->hit_box_tbl[( Uint8 )hb->body_box3], 1.0f ) ;
		}
		if( hb->catch_box ){
			if( sel == 6 ) box_arg.trnsl = 1.0f ;
			else box_arg.trnsl = 0.5f ;
			box_arg.sprno = 8 ;
			box_disp( w, &w->hit_box_tbl[( Uint8 )hb->catch_box], 1.0f ) ;
		}
		if( hb->atck_box ){
			if( !sel ) box_arg.trnsl = 1.0f ;
			else box_arg.trnsl = 0.5f ;
			box_arg.sprno = 4 ;
			box_disp( w, &w->hit_box_tbl[( Uint8 )hb->atck_box], 1.0f ) ;
		}
		if( hb->atck_box2 ){
			if( sel == 1 ) box_arg.trnsl = 1.0f ;
			else box_arg.trnsl = 0.5f ;
			box_arg.sprno = 10 ;
			box_disp( w, &w->hit_box_tbl[( Uint8 )hb->atck_box2], 1.0f ) ;
		}
		if( w->work_kind == W_PL ){
			box_arg.sprno = 9 ;
			box = ( w->hit_box_tbl + w->pl_stance )[PUSH_BOX] ;
			box.y = -box.y ;
			box_disp( w, &box, 1.0f ) ;
		}
	}
	pos_mark_disp( w ) ;
}

/*==============================================================*/
/*			Draw a work's sprite from the tiles					*/
/*			sprite_tex_set loaded for it this frame; returns	*/
/*			the tiles drawn, or -1 when they run out			*/
/*==============================================================*/
static int sprite_put( PLAYER_WORK *w )
{
	NLsprarg spr ;
	Uint16 *p ;
	SPR_TILE *tile ;
	Uint8 *cell ;
	Float x0, y0, uv, fx, fy ;
	Sint16 ox, oy, px, py, dx, dy, a, b ;
	int cnt, n, i, j, total ;
	Sint32 ang ;

	p = ( Uint16 * )( w->spr_adrs + ( ( Sint32 * )w->spr_adrs )[w->spr_no & SPR_NO_MASK] ) ;
	cnt = *p ++ ;
	tile = &tile_buff[w->tile_top] ;
	if( !w->disp_ang ){
		x0 = nlFloor( w->disp_x ) ;
		y0 = nlFloor( w->disp_y ) ;
		ox = 0 ;
		oy = 0 ;
	} else {
		if( !w->disp_flip ) ox = w->disp_ofs_x ;
		else ox = -w->disp_ofs_x ;
		oy = w->disp_ofs_y ;
		x0 = nlFloor( w->disp_x ) + ox * w->disp_zm_x ;
		y0 = nlFloor( w->disp_y ) + oy * w->disp_zm_y ;
		ox = -ox ;
		oy = -oy ;
	}
	spr.z = w->disp_z ;
	spr.sprno = w->tile_top + tile_disp_top ;
	spr.trnsl = w->alpha ;
	spr.listType = spr_list_type ;
	spr.base_color = -1 ;
	spr.offset_color = 0 ;
	i = 0, total = 0 ;
	while( cnt > i ){
		cell = w->cell_adrs + ( ( Sint32 * )w->cell_adrs )[PART( p )->no] ;
		n = tile->cnt + 1 ;
		uv = tile->size / 32.0f ;
		if( n + w->tile_top > 0x180 ) return -1 ;
		spr.u0 = 0.0f ;
		spr.v0 = 1.0f - uv ;
		spr.u1 = uv ;
		spr.v1 = 1.0f ;
		spr.zm_x = w->disp_zm_x * uv ;
		spr.zm_y = w->disp_zm_y * uv ;
		px = PART( p )->x ;
		py = PART( p )->y ;
		dx = 0 ;
		dy = 0 ;
		if( !( Uint32 )w->disp_flip ){
			ox = ox - px ;
			oy = oy - py ;
			spr.attr = spr_attr | 13 ;
			spr.ang = w->disp_ang ;
			if( PART( p )->attr & 0x4000 ){
				spr.attr = spr_attr | 5 ;
				spr.attr |= NLSPR_FLIP_V ;
				spr.v0 = 0.0f ;
				spr.v1 = uv ;
				dy = cell[1] * 8 ;
			}
			if( PART( p )->attr & 0x8000 ){
				spr.attr |= NLSPR_FLIP_H ;
				spr.u0 = 1.0f - uv ;
				spr.u1 = 1.0f ;
				dx = cell[0] * 8 - tile->size ;
			}
		} else {
			oy = oy - py ;
			spr.attr = spr_attr | 15 ;
			spr.ang = -w->disp_ang ;
			ox = ox + px ;
			dx = cell[0] * 8 - tile->size ;
			if( PART( p )->attr & 0x4000 ){
				spr.attr = spr_attr | 7 ;
				spr.attr |= NLSPR_FLIP_V ;
				spr.v0 = 0.0f ;
				spr.v1 = uv ;
				dy = cell[1] * 8 ;
			}
			if( !( PART( p )->attr & 0x8000 ) ){
				spr.attr |= NLSPR_FLIP_H ;
				spr.u0 = 1.0f - uv ;
				spr.u1 = 1.0f ;
				dx = 0 ;
			}
		}
		for( j = 0 ; j < n ; j ++ ){
			a = tile->x * tile->size - dx ;
			b = tile->y * tile->size - dy ;
			if( spr.attr & NLSPR_FLIP_H ) a = -a ;
			if( spr.attr & NLSPR_FLIP_V ) b = -b ;
			fx = ( ox + a ) * w->disp_zm_x ;
			fy = ( oy + b ) * w->disp_zm_y ;
			ang = spr.ang ;
			if( !ang ){
				spr.x = x0 + fx ;
				spr.y = y0 + fy ;
			} else {
				spr.x = x0 + fx * nlCos( ang ) + fy * nlSin( spr.ang ) ;
				spr.y = y0 - fx * nlSin( spr.ang ) + fy * nlCos( spr.ang ) ;
			}
			spr.z += 0.001f ;
			nlSprPut( &spr ) ;
			spr.sprno ++ ;
			tile ++ ;
		}
		total += n ;
		p = p + 4 ;
		i ++ ;
	}
	return total ;
}

/*==============================================================*/
/*			Draw a work's sprite from the texture set			*/
/*			sprite_set_load loaded for it; returns the cells	*/
/*			drawn												*/
/*==============================================================*/
static int sprite_set_put( PLAYER_WORK *w )
{
	NLsprarg spr ;
	Uint16 *p ;
	Uint8 *map, *cell ;
	Float x0, y0, fx, fy ;
	Sint16 ox, oy, px, py ;
	int cnt, i, total ;
	Sint32 ang ;

	p = ( Uint16 * )( w->spr_adrs + ( ( Sint32 * )w->spr_adrs )[w->spr_no & SPR_NO_MASK] ) ;
	cnt = *p ++ ;
	if( !w->disp_ang ){
		x0 = nlFloor( w->disp_x ) ;
		y0 = nlFloor( w->disp_y ) ;
		ox = 0 ;
		oy = 0 ;
	} else {
		if( !w->disp_flip ) ox = w->disp_ofs_x ;
		else ox = -w->disp_ofs_x ;
		oy = w->disp_ofs_y ;
		x0 = nlFloor( w->disp_x ) + ox * w->disp_zm_x ;
		y0 = nlFloor( w->disp_y ) + oy * w->disp_zm_y ;
		ox = -ox ;
		oy = -oy ;
	}
	spr.z = w->disp_z ;
	spr.trnsl = w->alpha ;
	spr.listType = spr_list_type ;
	spr.base_color = -1 ;
	spr.offset_color = 0 ;
	i = ( ( Uint16 * )w->spr_set_buff )[w->spr_no & 0x1ff] ;
	map = ( Uint8 * )( ( Uint16 * )w->spr_set_buff + 0x110 ) + i ;
	i = 0, total = 0 ;
	while( cnt > i ){
		spr.sprno = *map ++ + w->work_no * 256 + 0x390 ;
		cell = w->cell_adrs + ( ( Sint32 * )w->cell_adrs )[PART( p )->no] ;
		spr.u0 = 0.0f ;
		spr.v0 = 0.0f ;
		spr.u1 = ( Float )cell[0] / cell[2] ;
		spr.v1 = ( Float )cell[1] / cell[3] ;
		spr.zm_x = w->disp_zm_x * spr.u1 ;
		spr.zm_y = w->disp_zm_y * spr.v1 ;
		px = PART( p )->x ;
		py = PART( p )->y ;
		if( !w->disp_flip ){
			ox = ox - px ;
			oy = oy - py ;
			spr.ang = w->disp_ang ;
			spr.attr = spr_attr | 5 ;
			if( PART( p )->attr & 0x4000 ){
				spr.attr |= NLSPR_FLIP_V ;
				spr.v0 = 1.0f - spr.v1 ;
				spr.v1 = 1.0f ;
			}
			if( PART( p )->attr & 0x8000 ){
				spr.attr |= NLSPR_FLIP_H ;
				spr.u0 = 1.0f - spr.u1 ;
				spr.u1 = 1.0f ;
			}
		} else {
			ox = ox + px ;
			oy = oy - py ;
			spr.ang = -w->disp_ang ;
			spr.attr = spr_attr | 7 ;
			if( PART( p )->attr & 0x4000 ){
				spr.attr |= NLSPR_FLIP_V ;
				spr.v0 = 1.0f - spr.v1 ;
				spr.v1 = 1.0f ;
			}
			if( !( PART( p )->attr & 0x8000 ) ){
				spr.attr |= NLSPR_FLIP_H ;
				spr.u0 = 1.0f - spr.u1 ;
				spr.u1 = 1.0f ;
			}
		}
		fx = ox * w->disp_zm_x ;
		fy = oy * w->disp_zm_y ;
		ang = spr.ang ;
		if( !ang ){
			spr.x = x0 + fx ;
			spr.y = y0 + fy ;
		} else {
			spr.x = x0 + fx * nlCos( ang ) + fy * nlSin( spr.ang ) ;
			spr.y = y0 - fx * nlSin( spr.ang ) + fy * nlCos( spr.ang ) ;
		}
		spr.z += 0.001f ;
		nlSprPut( &spr ) ;
		total ++ ;
		p = p + 4 ;
		i ++ ;
	}
	return total ;
}

/****************************************************************/
/*			Draw a work's sprite								*/
/****************************************************************/
int sprite_disp( PLAYER_WORK *w )
{
	int n ;

	if( w->spr_no == -1 ) return 0 ;
	if( w->spr_no & SPR_NO_SET ) n = sprite_set_put( w ) ;
	else n = sprite_put( w ) ;
	return n ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sprite.c" End of File	*/
