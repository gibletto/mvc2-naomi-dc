/* The Kamui2 calls the NAOMI graphics library makes, and the types they take. */
#ifndef LIBNAOMI_KAMUI_H
#define LIBNAOMI_KAMUI_H

#include "types.h"

typedef struct KMBYTE {
	Uint8	bBlue, bGreen, bRed, bAlpha;
} KMBYTE;

typedef union KMPACKEDARGB {
	Uint32	dwPacked;
	KMBYTE	byte;
} KMPACKEDARGB;

/* a frame buffer or a texture in video memory */
typedef struct KMSURFACEDESC {
	Uint32	SurfaceType;
	Uint32	BitDepth;
	Uint32	PixelFormat;
	Uint32	USize;
	Uint32	VSize;
	Uint32	dwTextureSize;
	Uint32	fSurfaceFlags;
	Uint32	*pSurface;
} KMSURFACEDESC;

/* the vertex buffer the strips are written to */
typedef struct KMVERTEXBUFFDESC {
	Uint32	*pBuffer;
	Uint32	*pCtrl;
} KMVERTEXBUFFDESC;

/* the first word of a vertex: one inside a strip, or the strip's last */
#define KM_VERTEXPARAM_NORMAL		0xE0000000
#define KM_VERTEXPARAM_ENDOFSTRIP	0xF0000000

/* the vertices kmSetVertex takes: types 0 and 1 (coloured), 3 (textured), 9 (two colours for a
   modifier volume), 11 (textured, two sets) and 17 (a modifier volume's triangle) */
typedef struct KMVERTEX_00 {
	Uint32	ParamControlWord;
	Float	fX, fY, fZ;
	Uint32	unk_10, unk_14;
	Uint32	BaseColor;
	Uint32	unk_1c;
} KMVERTEX_00;

typedef struct KMVERTEX_03 {
	Uint32	ParamControlWord;
	Float	fX, fY, fZ;
	Float	fU, fV;
	Uint32	BaseColor;
	Uint32	OffsetColor;
} KMVERTEX_03;

typedef struct KMVERTEX_09 {
	Uint32	ParamControlWord;
	Float	fX, fY, fZ;
	Uint32	BaseColor0;
	Uint32	BaseColor1;
	Uint32	unk_18, unk_1c;
} KMVERTEX_09;

typedef struct KMVERTEX_11 {
	Uint32	ParamControlWord;
	Float	fX, fY, fZ;
	Float	fU0, fV0;
	Uint32	BaseColor0;
	Uint32	OffsetColor0;
	Float	fU1, fV1;
	Uint32	BaseColor1;
	Uint32	OffsetColor1;
	Uint32	unk_30[4];
} KMVERTEX_11;

typedef struct KMVERTEX_17 {
	Uint32	ParamControlWord;
	Float	fAX, fAY, fAZ;
	Float	fBX, fBY, fBZ;
	Float	fCX, fCY, fCZ;
	Uint32	unk_28[6];
} KMVERTEX_17;

extern void kmSetFogTableColor( KMPACKEDARGB argb ) ;
extern void kmSetFogDensity( Uint32 density ) ;
extern void kmSetFogTable( Float *table ) ;
extern Sint32 kmCreateTextureSurface( KMSURFACEDESC *desc, Sint32 width, Sint32 height, Uint32 type ) ;
extern Sint32 kmLoadTexture( KMSURFACEDESC *desc, Uint32 *data ) ;
extern Sint32 kmLoadTextureRom() ;
extern void kmFreeTexture( KMSURFACEDESC *desc ) ;
extern void kmGetFreeTextureMem( Uint32 *total, Uint32 *largest ) ;
extern void kmGarbageCollectTexture( void ) ;
extern void kmyEnableASyncCopy( Sint32 enable ) ;
extern Sint32 kmQueryFinishLastTextureDMA( void ) ;

#endif
