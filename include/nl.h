/* The NAOMI graphics library's (Kamui's nl) calls the game makes, and the types they take that
   player.h doesn't hold, as the SDK declares them. player.h includes it. */
#ifndef NL_H
#define NL_H

/* a sprite to draw (nlSprPut) */
typedef struct NLsprarg {
	Sint32	sprno;
	Float	x, y, z;
	Float	zm_x, zm_y;
	Float	u0, v0, u1, v1;
	Sint32	ang;
	Float	trnsl;
	Sint32	listType;
	Sint32	attr;
	Sint32	base_color;
	Sint32	offset_color;
	Sint32	reserved[4];
} NLsprarg;

/* NLsprarg's attr: the place of the texture, its flips and how it is drawn */
#define NLSPR_DISP_LT	0x0005	/* the texture's left, top at the sprite's place */
#define NLSPR_DISP_RT	0x0007	/* its right, top */
#define NLSPR_DISP_CC	0x000a	/* its centre */
#define NLSPR_DISP_LB	0x000d	/* its left, bottom */
#define NLSPR_DISP_RB	0x000f	/* its right, bottom */
#define NLSPR_FLIP_H	0x0010	/* the texture flipped left to right */
#define NLSPR_FLIP_V	0x0020	/* the texture flipped top to bottom */
#define NLSPR_DEPTH_GREATER	0x0400	/* drawn where its depth is greater */
#define NLSPR_FOG_EBL	0x0800	/* the fog acts on it */
#define NLSPR_BILINEAR	0x1000	/* bilinear filtering */
#define NLSPR_FADE	0x2000	/* nlObjPutSetFadeColor acts on it */
#define NLSPR_USERCLIP_INSIDE	0x8000	/* drawn only inside the tile clip */
#define NLSPR_USERCLIP_OUTSIDE	0xc000	/* drawn only outside the tile clip */
#define NLSPR_SUPERSAMPLE	0x10000	/* drawn supersampled */
#define NLSPR_COLORSET	0x20000	/* base_color and offset_color are used */

/* the vertices of a strip: coloured, and textured */
typedef struct NLVERTEX00 {
	Float	x, y, z;
	Uint32	col;
} NLVERTEX00;

typedef struct NLVERTEX03 {
	Float	x, y, z;
	Float	u, v;
	Uint32	col;
	Uint32	ofs;
} NLVERTEX03;

extern Sint32 nlArcTan( Float n ) ;
extern Sint32 nlArcTan2( Float y, Float x ) ;
extern Sint32 nlAtan256( Float y, Float x ) ;
extern void nlCalcPoint( VEC3 *src, VEC3 *dst ) ;
extern void nlCalcVector( VEC3 *src, VEC3 *dst ) ;
extern Uint32 nlCommonTextureGetFreeMem( void ) ;
extern Float nlCos( int ang ) ;
extern int nlCreateVertexBuffer( int opSz, int opMdSz, int trSz, int trMdSz ) ;
extern void nlDispStripCoordMode( Sint32 mode ) ;
extern void nlDispStripInit( void ) ;
extern Sint32 nlDispStripLoadTexture( Sint32 index, NLthd *thd ) ;
extern void nlDispStripTexTrnsl( Sint32 num, NLVERTEX03 *vt, Sint32 txtnum ) ;
extern void nlDispStripTrnsl( Sint32 num, NLVERTEX00 *vt ) ;
extern Float nlFloor( Float n ) ;
extern void nlFogDensity( Sint32 density ) ;
extern int nlFogDensityF( Float fdensity ) ;
extern void nlFogTableColor( Uint32 argb ) ;
extern void nlFogTableOff( void ) ;
extern void nlFogTableSet( Float *table ) ;
extern void nlFreeCommonTextureAll( void ) ;
extern void nlFreePaletteAll( void ) ;
extern MATRIX *nlGetMatrix( MATRIX *md ) ;
extern void nlInitCommonTexture( Sint32 *work, Sint32 nbLoadTex, Sint32 nbMaxTex ) ;
extern void nlInitDrawModel( Float *draw_vertex_work, Sint32 *draw_light_work ) ;
extern void nlInitInterrupt( void ) ;
extern void nlInitLibrary( int tvmode, int fbmode, int vwait ) ;
extern MATRIX *nlInitMatrix( MATRIX *m, Sint32 n ) ;
extern void nlInitPalette( void *pb, int np ) ;
extern void nlInitPrint( void ) ;
extern void nlInitSprite( Sint32 *work, Sint32 nbLoadSpr, Sint32 nbMaxSpr ) ;
extern void nlLightInit( void ) ;
extern void nlLineSetThickness( Float t ) ;
extern void nlLoadCorePaletteData( void ) ;
extern void nlLoadMatrix( MATRIX *ms ) ;
extern void nlLocate( Sint32 x, Sint32 y ) ;
extern void nlLookAt( VEC3 *eye, VEC3 *obj, Sint32 z ) ;
extern void nlMakeModelViewProjectMatrix( Float width, Float height ) ;
extern void nlMatrixMode( int mode ) ;
extern void nlMultiMatrix( MATRIX *ms ) ;
extern void nlObjPut( int *objname ) ;
extern void nlObjPutCheapEnvMap( Sint32 *objname ) ;
extern void nlObjPutInit( void ) ;
extern void nlObjPutInitTexture( void ) ;
extern int nlObjPutLoadTexture( int *objname, NLthd *thdbase ) ;
extern void nlObjPutMvClosedVolumeUseViewMatrix( Sint32 *objname ) ;
extern void nlObjPutMvInit( int intensity ) ;
extern void nlObjPutMvSetIntensity( int intensity ) ;
extern void nlObjPutMvSetMode( int mode ) ;
extern void nlObjPutSetFadeColor( Float r, Float g, Float b ) ;
extern void nlObjPutSetGlobalAmbience( Float gamb ) ;
extern void nlObjPutSetLightDirectionV( VEC3 *lv ) ;
extern void nlObjPutTrnsl( Sint32 *objname, Float trs ) ;
extern void nlObjPutUseViewMatrix( int *objname ) ;
extern void nlObjPutUseViewMatrixTrnsl( Sint32 *objname, Float trs ) ;
extern void nlPaletteDirectMode( int mode ) ;
extern void nlPerspective( Sint32 fovy, Float aspect, Float znear, Float zfar ) ;
extern void nlPerspectiveX( Sint32 fovx, Float aspect, Float znear, Float zfar ) ;
extern int nlPopMatrix( int n ) ;
extern Float nlPow( Float x, Float y ) ;
extern void nlPrintColor( Uint32 c ) ;
extern int nlPrintf( char *fmt, ... ) ;
extern void nlPrnIndentOn( void ) ;
extern void nlProjectScreen( VEC3 *pos, Float *scr ) ;
extern int nlPushMatrix( MATRIX *m ) ;
extern void nlPuts( char *s ) ;
extern Sint32 nlRand( void ) ;
extern Float nlRandom( void ) ;
extern void nlRotateAX( Sint32 ang ) ;
extern void nlRotateAY( Sint32 ang ) ;
extern void nlRotateAZ( Sint32 ang ) ;
extern void nlScaleV( VEC3 *v ) ;
extern void nlScreenPoint( Float x, Float y, Float z, Float *dx, Float *dy, Float *dz ) ;
extern void nlSetBackColor( Uint32 col1, Uint32 col2, Uint32 col3 ) ;
extern void nlSetBackColorZ( Uint32 col1, Uint32 col2, Uint32 col3, Float z ) ;
extern void nlSetBorderColor( Uint32 color ) ;
extern void nlSetCommonTextureOffset( Sint32 ofs ) ;
extern void nlSetFontSize( Sint32 xs, Sint32 ys ) ;
extern Sint32 nlSetIntrFunction( Sint32 code, Sint32 ( *func )( void ) ) ;
extern void nlSetPaletteMode( int mode ) ;
extern void nlSetPaletteData( Sint32 offset, void *colors, Sint32 size ) ;
extern void nlSetScaleFactor( Float s ) ;
extern void nlShiftPerspective( Float x, Float y ) ;
extern Float nlSin( int ang ) ;
extern void nlSingleLineUseViewMatrix( VEC3 *p0, VEC3 *p1, Uint32 col ) ;
extern int nlSpriteChangePalette( int index, int pal ) ;
extern int nlSpriteFreeTexture( NLthd *thd, Sint32 *pal ) ;
extern int nlSpriteLoadTexture( NLthd *thd, Sint32 *pal ) ;
extern int nlSpriteLoadTextureOnce( NLthd *thd, Sint32 *pal, int index ) ;
extern void nlSpriteSetImaginaryScreenSize( Float width, Float height ) ;
extern int nlSprPut( NLsprarg *arg ) ;
extern void nlSysCreatePunchBuffer( int punch_size ) ;
extern int nlSysMetricsCpuPowerGet( void ) ;
extern void nlSysMetricsCpuPowerStart( void ) ;
extern void nlSysMetricsInit( void ) ;
extern void nlSysSetAutoSortMode( Sint32 enable ) ;
extern void nlSysSetDirectListTypeLatency2V( int ListType ) ;
extern void nlSysSetTextureSize( int tex_size ) ;
extern void nlSysSetTimeOutCount( int VtxTimeOut, int RndTimeOut ) ;
extern void nlSysSetVertexBufferBaseAddress( void *base_address ) ;
extern void nlSysUseLatency2V( void ) ;
extern Float nlTan( Sint32 ang ) ;
extern void nlTranslate( Float x, Float y, Float z ) ;
extern void nlTranslateV( VEC3 *v ) ;
extern void nlUnitMatrix( void ) ;
extern Float nlUnitVector( VEC3 *v ) ;
extern void nlWaitVSync( void ) ;

#endif
