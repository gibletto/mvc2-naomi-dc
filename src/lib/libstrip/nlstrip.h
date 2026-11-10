/* What nlstrip.c shares with the rest of the NAOMI library: the texture, palette and vertex
   state it works on, and the routines that load and draw. */
#ifndef NLSTRIP_H
#define NLSTRIP_H

#include "player.h"

/* how often a texture of the game's list is loaded, and the slot that holds it */
typedef struct NLTEXLOAD {
	Uint32	count;
	Sint32	slot;
} NLTEXLOAD;

/* a slot of the loaded textures */
typedef struct NLTEXPARAM {
	Sint32	unk_00[10];
	Sint32	status;		/* 028: 0 when it has a surface to free */
	Sint32	users;		/* 02C */
	Sint32	unk_30;
	Sint32	tcw;		/* 034: texture control word, the pixel format in bits 27-29 */
	Sint32	index;		/* 038: the list's texture, -1 when free */
} NLTEXPARAM;

/* a palette of the list, and how often it is loaded */
typedef struct NLPALETTE {
	Sint16	map;		/* 000: where nlSearchPalette put it */
	Sint8	attr;		/* 002 */
	Sint8	size;		/* 003: in 16 colours */
	Sint16	count;		/* 004 */
	Sint16	index;		/* 006 */
} NLPALETTE;

/* the library's work, as far as nlstrip.c reads it */
typedef struct NLSYSTEMWORK {
	Sint32	unk_00;
	Sint16	palettes;	/* 004: how many the list holds */
	Sint16	unk_06;
	Sint32	unk_08[4];
	NLPALETTE *palette;	/* 018 */
} NLSYSTEMWORK;

/* a palette to load, as the list has it */
typedef struct NLtpal {
	Sint16	unk_00;
	Sint16	attr;
	void	*colors;
	Sint32	size;
} NLtpal;

/* the vertex context: the settings of the strip being drawn, and the header words
   kmProcessVertexRenderState makes of them */
typedef struct NLSTRIPCTX {
	Uint32	state;		/* 000: which of the settings to process */
	Uint32	unk_04;
	Sint32	list_type;	/* 008: 0 opaque, 2 translucent */
	Sint32	color_type;	/* 00C */
	Uint32	unk_10[4];
	Sint32	shading;	/* 020: bit 0 gouraud, bit 1 textured */
	Uint32	unk_24[7];
	Sint32	specular;	/* 040: with an offset colour */
	Sint32	use_alpha;	/* 044 */
	Uint32	unk_48[8];
	Sint32	palette_bank;	/* 068 */
	void	*texture;	/* 06C */
	Uint32	unk_70[8];
	Uint32	pcw;		/* 090: the parameter control word */
	Uint32	isp;		/* 094 */
	Uint32	tsp;		/* 098 */
} NLSTRIPCTX;

/* where the vertex being sent goes: its write pointer and size */
typedef struct NLVERTEXSLOT {
	Sint32	index;
	Sint32	size;
} NLVERTEXSLOT;

/* the list of vertex buffers */
typedef struct NLVERTEXBUF {
	Uint32	**write;	/* 000: the next free word of each buffer */
	void	*head;		/* 004: the strip header to send first */
	NLVERTEXSLOT *slot;	/* 008 */
} NLVERTEXBUF;

/* a vertex as the hardware gets it */
typedef struct NLHWVERTEX {
	Uint32	cmd;
	VEC3	p;
	Float	u, v;
	Uint32	col, ofs;
} NLHWVERTEX;

extern Sint32 nl_palette_direct_mode;
extern NLSYSTEMWORK NLSystemWork_;
extern NLTEXLOAD *nl_texture_loadflag;
extern NLTEXPARAM *nl_texture_param;
extern NLSTRIPCTX *nl_vertex_context_ptr;
extern NLVERTEXBUF nl_vertex_buffer;
extern Uint32 nl_or_GP[2];
extern Sint32 nl_trnsl_blend_mode;

extern Sint32 nlCommonTextureGetNextTextureIndex( void );
extern void nlCommonTextureClearTextureIndex( Sint32 index );
extern Sint32 nlLoadTextureMem( NLthd *thd, Sint32 slot, Sint32 address );
extern void nlCreateTextureParam( Sint32 slot );
extern Sint32 nlSearchPalette( Sint32 attr, Sint32 size );
extern void nlSetPaletteMap( Sint32 map, Sint32 size );
extern void nlSetPaletteData( Sint32 offset, void *colors, Sint32 size );
extern void nlFreePalette( Sint32 index );
extern Sint32 kmFreeTexture( NLTEXPARAM *param );
extern void kmProcessVertexRenderState( NLSTRIPCTX *ctx );
extern void kmSetVertexRenderState( NLVERTEXBUF *dev, NLSTRIPCTX *ctx );
extern void kmyMemCopy( void *dst, void *src, Sint32 size );
extern void nlCalcPoint( VEC3 *src, VEC3 *dst );

#endif
