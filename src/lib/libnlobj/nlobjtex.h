/* what the texture code of libnlobj shares with the rest of the NAOMI library */
#ifndef NLOBJTEX_H
#define NLOBJTEX_H

/* how many users a texture has, and the index of its parameters */
typedef struct NLTEXLOAD {
	Uint32	count;
	Sint32	index;
} NLTEXLOAD;

/* a texture's parameters, behind Kamui's surface description */
typedef struct NLTEXPARAM {
	Sint32	unk_00[10];
	Sint32	status;		/* 28: 0 when it has a surface to free */
	Sint32	use;		/* 2c: groups that use it */
	Sint32	unk_30[2];
	Sint32	index;		/* 38 */
} NLTEXPARAM;

/* a palette in a model file's list */
typedef struct NLPALETTE_SRC {
	Sint16	unk_0;
	Sint16	type;
	void	*data;
	Sint32	size;
} NLPALETTE_SRC;

/* a palette loaded into the palette RAM */
typedef struct NLPALETTE {
	Sint16	slot;
	Sint8	type;
	Sint8	size;		/* in 16 colours */
	Sint16	use;
	Sint16	palno;
} NLPALETTE;

/* the library's work: its table of loaded palettes */
typedef struct NLSYSTEMWORK {
	Sint16	unk_0, unk_2;
	Sint16	palettes;	/* 04 */
	Sint16	unk_6;
	Sint32	unk_8[4];
	NLPALETTE *palette;	/* 18 */
} NLSYSTEMWORK;

extern Sint32 nl_txt_ofs;
extern NLTEXLOAD *nl_texture_loadflag;
extern NLTEXPARAM *nl_texture_param;
extern NLSYSTEMWORK NLSystemWork_;
extern Sint32 nl_palette_direct_mode;
extern Sint32 nl_UserTileClipMode;
extern NLPALETTE_SRC *NLobjput_palList;

extern Sint32 nlCommonTextureGetNextTextureIndex( void );
extern void nlCommonTextureClearTextureIndex( Sint32 index );
extern Sint32 nlLoadTextureMem( NLthd *thd, Sint32 index, void *address );
extern void nlCreateTextureParam( Sint32 index );
extern void nlSetTextureParameter( NLMESH *mesh, Sint32 index );
extern void nlSetDummyTextureParameter( NLMESH *mesh );
extern Sint32 nlSearchPalette( Sint32 type, Sint32 size );
extern void nlSetPaletteMap( Sint32 slot, Sint32 size );
extern void nlSetPaletteData( Sint32 ofs, void *data, Sint32 size );
extern void nlFreePalette( Sint32 palno );
extern void nlObjPutLoadTextureCallBack( NLMESH *mesh );
extern Sint32 kmFreeTexture( NLTEXPARAM *param );

#endif
