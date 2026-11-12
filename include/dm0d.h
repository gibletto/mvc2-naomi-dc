/* DM0D, the network lobby: what its modules share (dm0d_*.c) */
#ifndef DM0D_H
#define DM0D_H

extern void obj_uv_clear( void *obj ) ;
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;
extern void str_put( Float x, Float y, Float z, Uint32 col, char *str ) ;
extern void num_comma_set( int num, char *buf ) ;
extern int str_num_get( char *str ) ;
extern void win_pict_set( int no ) ;

/* the text entry (kbd.c): how kb_init sets the keyboard up */
typedef struct KB_SET {
	Sint16	max;		/* the bytes the text may take */
	Sint16	mode;		/* 002: the input modes it opens in, kept within mode_ok */
	Sint16	mode_ok;	/* 004: the input modes allowed */
	char	*str;		/* 008: the text typed */
	Uint8	size;		/* 00c: the keyboard's size, a row of kb_pos_tbl */
} KB_SET;

extern void kb_init( KB_SET *set ) ;
extern int kb_main( void ) ;	/* each frame: 0 while the entry goes on, else how it ended */

#endif
