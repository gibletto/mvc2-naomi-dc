/* The home menu's (DM0C to DM12, and the pause's menus): the parts tables its screens are built
   from, and the routines its modules share. */
#ifndef DM_H
#define DM_H

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a piece of a menu object, in a list that a work's work2 points at */
typedef struct DM0C_PARTS {
	VEC3	pos;
	Uint16	obj;
	Sint8	scale;		/* its size, where the list has a size table (key_scale_tbl) */
	Sint8	next;		/* -1 for the last */
} DM0C_PARTS;

/* a character of a number or a row of marks */
typedef struct DM0C_CHAR {
	Sint8	disp;		/* 1 drawn, 0 not, -1 the end */
	Sint8	scale;		/* drawn at the work's size */
	Uint16	code;
	Float	ofs;
} DM0C_CHAR;

/* home_menu.c */
extern Uint16 menu_sw_get( Uint8 side_bit ) ;

/* dm0c_option.c */
extern PLAYER_WORK *option_menu_set( void ) ;
extern PLAYER_WORK *headline_set( int no ) ;

/* the scrolling of a model's texture coordinates */
extern void obj_uv_clear( void *obj ) ;
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;

#endif
