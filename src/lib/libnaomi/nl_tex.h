/* The texture tables of the NAOMI graphics library. */
#ifndef LIBNAOMI_NL_TEX_H
#define LIBNAOMI_NL_TEX_H

#include "kamui.h"

/* a texture's header */
typedef struct NLTEXHEAD {
	Sint16	width, height;
	Sint32	type;
	Uint32	*address;
	Sint32	globl_index;
} NLTEXHEAD;

/* a texture's slot in nl_texture_param: its surface, and the words that select it in a strip */
typedef struct NLTEXPARAM {
	KMSURFACEDESC	desc;
	Sint32	unk_20[2];
	Sint32	status;		/* 28: 0 when its surface was made, -1 when free */
	Sint32	used;		/* 2c: how many use it */
	Uint32	tsp_size;	/* 30: its size, as the TSP word has it */
	Uint32	tsp_addr;	/* 34: its format and address, the texture control word */
	Sint32	index;		/* 38: its own number, -1 when free */
} NLTEXPARAM;

/* a common texture's place in nl_texture_param */
typedef struct NLTEXLOAD {
	Sint32	loaded;
	Sint32	slot;
} NLTEXLOAD;

/* a polygon header's four words: the last two take a texture */
typedef struct NLTEXCONTEXT {
	Uint32	pcw;
	Uint32	isp;
	Uint32	tsp;
	Uint32	tex;
} NLTEXCONTEXT;

extern NLTEXPARAM *nl_texture_param ;
extern Sint32 nl_texture_max ;
extern Sint32 nl_txt_count ;
extern Sint32 nl_txt_ofs ;
extern NLTEXLOAD *nl_texture_loadflag ;
extern Uint32 *nl_texture_loadbit ;
extern Sint32 nl_texture_flagmax ;
extern Sint32 nl_texture_bitmax ;
extern Uint32 *(*nl_tex_load_user_callback)( NLTEXHEAD *thd ) ;

#endif
