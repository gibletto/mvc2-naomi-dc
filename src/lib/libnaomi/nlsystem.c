/* nlsystem: the NAOMI graphics library's system: its set-up and display mode, the vertex mode, the vertex buffers and
   render passes, and the timing figures. In the game it is one module, the multi-pass part (libnaomik2) following. */
#include "nlsystem.h"
#include "nlreg.h"

#define set_imask( mask )	_builtin_set_imask( mask )
#define set_gbr( addr )	_builtin_set_gbr( addr )
#define trapa( n )	_builtin_trapa( n )
#define get_cr()	_builtin_get_cr()
#define set_cr( x )	_builtin_set_cr( x )

/* a display mode: its television system (0 NTSC, 1 VGA, 2 PAL), Kamui2's mode and the screen's size */
typedef struct NLDSPMODE {
	Sint32	tv;
	Sint32	mode;
	Sint16	width, height;
} NLDSPMODE;

/* a vertex mode: the settings it puts in the vertex context */
typedef struct NLVTXMODE {
	Sint32	list_type, color_type, shading, use_alpha, use_specular;
} NLVTXMODE;

/* the four edges of a user clip */
typedef struct NLCLIPRECT {
	Sint32	x0, y0, x1, y1;
} NLCLIPRECT;

/* what kmGetSystemMetrics fills in */
typedef struct KMSYSTEMMETRICS {
	Sint32	unk_00;
	Sint32	time[9];	/* 04; the last render at 0, the last TA at 8 */
	Sint32	unk_28[8];
	Sint32	vertex_timeout;	/* 48 */
	Sint32	render_timeout;	/* 4c */
	Sint32	unk_50[2];
	Sint32	native_use_size;	/* 58 */
	Sint32	unk_5c[2];
	Sint32	native_max_size;	/* 64 */
	Sint32	unk_68[80];
	Sint32	fb_index;	/* 1a8 */
	Sint32	unk_1ac[17];
} KMSYSTEMMETRICS;

/* a vertex of the background plane */
typedef struct KMBGVERTEX {
	Uint32	flags;
	Float	x, y, z;
	Uint32	base;
	Uint32	unk_14;
	Uint32	offset;
	Uint32	unk_1c;
} KMBGVERTEX;

/* Kamui2's private data; the time out count is the only part used here */
typedef struct KMIEXTENSION {
	Uint8	unk_000[0x82C];
	Sint32	time_out_count;
} KMIEXTENSION;

extern KMIEXTENSION kmiExtension;

extern char nl_date_libnaomi_[];
extern KMVERTEXBUFFDESC nl_vertex_buffer;
extern KMSTRIPCONTEXT nl_back_context;
extern KMSTRIPCONTEXT nl_vertex_context[8];
extern KMSTRIPCONTEXT *nl_vertex_context_ptr;
extern Sint32 nl_vertex_context_depth;
extern Sint32 nl_set_txt_num;
extern Sint32 nl_vertex_mode;
extern Sint32 nl_frame_count;
extern Sint32 nl_txt_count;
extern Sint32 nl_counter[4];
extern Float nl_deb_time[4];
extern Float nl_windowSizeWidth, nl_windowSizeHeight, nl_windowSizeLeft, nl_windowSizeTop;
extern Sint32 nl_reg_clip_xmin, nl_reg_clip_xmax, nl_reg_clip_ymin, nl_reg_clip_ymax;
extern Sint32 nl_reg_clip_xmin0, nl_reg_clip_xmax0, nl_reg_clip_ymin0, nl_reg_clip_ymax0;
extern Sint32 nl_reg_clip_start;
extern void nlSetWindowSize( Float left, Float top, Float width, Float height );
extern void nl_action_PixelClipping( void );
extern void nlSetBackColorZ( Float z, Uint32 r, Uint32 g, Uint32 b );

extern void kmInitDevice( Sint32 mode );
extern void kmSetDisplayMode( Sint32 mode, Sint32 bpp, Sint32 interlace, Sint32 vga );
extern void kmChangeDisplayFilterMode( Sint32 interlace, Sint32 vga );
extern Sint32 kmSetSystemConfiguration( KMSYSTEMCONFIGSTRUCT *config );
extern void kmSetCullingRegister( Float value );
extern void kmSetColorClampValue( KMPACKEDARGB min, KMPACKEDARGB max );
extern void kmSetFogVertexColor( KMPACKEDARGB col );
extern void kmSetBorderColor( KMPACKEDARGB col );
extern void kmSetWaitVsyncCount( Sint32 count );
extern void kmSetUserClipping( KMVERTEXBUFFDESC *buff, Sint32 mode, Sint32 tile, NLCLIPRECT *rect );
extern void kmGetCurrentScanline( Sint32 *line );
extern void kmSetEORCallback( void (*func)( void * ), void *arg );
extern void kmSetEndOfVertexCallback( void (*func)( void * ), void *arg );
extern void kmSetVSyncCallback( void (*func)( void * ), void *arg );
extern void kmSetHSyncCallback( void (*func)( void * ), void *arg );
extern void kmSetHSyncLine( Sint32 line );
extern void kmSetTexOverflowCallback( void (*func)( void * ), void *arg );
extern void kmSetStripOverRunCallback( void (*func)( void * ), void *arg );
extern void kmSetPALEXTCallback( void (*func)( Sint32 * ), void *arg );
extern void kmSetFatalErrorCallback( void (*func)( Sint32 ), void *arg );
extern void kmProcessVertexRenderState( KMSTRIPCONTEXT *ctx );
extern void kmSetVertexRenderState( KMVERTEXBUFFDESC *buff, KMSTRIPCONTEXT *ctx );
extern void kmSetBackGroundRenderState( KMSTRIPCONTEXT *ctx );
extern void kmBeginScene( KMSYSTEMCONFIGSTRUCT *config );
extern void kmEndScene( KMSYSTEMCONFIGSTRUCT *config );
extern void kmBeginPass( KMVERTEXBUFFDESC *buff );
extern void kmEndPass( KMVERTEXBUFFDESC *buff );
extern void kmRender( Sint32 mode );
extern Sint32 kmRenderRect( Sint32 mode );
extern void kmSetRegionClipping( Sint32 xmin, Sint32 ymin, Sint32 xmax, Sint32 ymax );
extern void kmChangeLatencyModel( Sint32 model, Sint32 list );
extern void kmDiscardVertexBuffer( KMVERTEXBUFFDESC *buff );
extern void kmGetSystemMetrics( KMSYSTEMMETRICS *metrics );
extern void kmSetBackGroundPlane( KMBGVERTEX **tri, Sint32 offset, Sint32 size );
extern Sint32 nlLoadDummyTexture( void );
extern void kmfSetTimeOutCount( Sint32 render, Sint32 vertex );
extern Sint32 nlSysBeginScene( void );
extern Sint32 nlSysEndScene( void );
extern Sint32 nlSysBeginPass( void );
extern Sint32 nlSysEndPass( void );
extern Sint32 nlSysRender( Sint32 mode );

static const NLDSPMODE dsp_mode_tbl[28] = {
	{ 1, 0x31, 640, 480 },
	{ 0, 0x0, 320, 240 },
	{ 0, 0x4, 320, 240 },
	{ 0, 0x10, 640, 240 },
	{ 0, 0x14, 640, 240 },
	{ 0, 0x20, 320, 480 },
	{ 0, 0x28, 320, 480 },
	{ 0, 0x24, 320, 480 },
	{ 0, 0x30, 640, 480 },
	{ 0, 0x38, 640, 480 },
	{ 0, 0x34, 640, 480 },
	{ 2, 0x2, 320, 240 },
	{ 2, 0x6, 320, 240 },
	{ 2, 0x12, 640, 240 },
	{ 2, 0x16, 640, 240 },
	{ 2, 0x22, 320, 480 },
	{ 2, 0x2A, 320, 480 },
	{ 2, 0x26, 320, 480 },
	{ 2, 0x32, 640, 480 },
	{ 2, 0x3A, 640, 480 },
	{ 2, 0x36, 640, 480 },
	{ 1, 0x1, 320, 240 },
	{ 1, 0x11, 640, 240 },
	{ 1, 0x21, 320, 480 },
	{ 2, 0x72, 640, 480 },
	{ 2, 0x52, 640, 240 },
	{ 2, 0x7A, 640, 480 },
	{ 2, 0x76, 640, 480 },
};

/* Kamui2's bits per pixel for the low four bits of the library's flags */
static const Sint32 bpp_tbl[6] = { 0, 1, -1, 3, 4, 5 };

/* the vertex modes: list type, colour type, shading, alpha and specular */
static const Sint32 vertex_mode_tbl[48][5] = {
	{ 0, 1, 1, 0, 0 },
	{ 2, 1, 1, 0, 0 },
	{ 0, 0, 1, 0, 0 },
	{ 2, 0, 1, 0, 0 },
	{ 0, 1, 1, 1, 0 },
	{ 2, 1, 1, 1, 0 },
	{ 0, 0, 1, 1, 0 },
	{ 2, 0, 1, 1, 0 },
	{ 0, 1, 1, 0, 1 },
	{ 2, 1, 1, 0, 1 },
	{ 0, 0, 1, 0, 1 },
	{ 2, 0, 1, 0, 1 },
	{ 0, 1, 1, 1, 1 },
	{ 2, 1, 1, 1, 1 },
	{ 0, 0, 1, 1, 1 },
	{ 2, 0, 1, 1, 1 },
	{ 0, 1, 3, 0, 0 },
	{ 2, 1, 3, 0, 0 },
	{ 0, 0, 3, 0, 0 },
	{ 2, 0, 3, 0, 0 },
	{ 0, 1, 3, 1, 0 },
	{ 2, 1, 3, 1, 0 },
	{ 0, 0, 3, 1, 0 },
	{ 2, 0, 3, 1, 0 },
	{ 0, 1, 3, 0, 1 },
	{ 2, 1, 3, 0, 1 },
	{ 0, 0, 3, 0, 1 },
	{ 2, 0, 3, 0, 1 },
	{ 0, 1, 3, 1, 1 },
	{ 2, 1, 3, 1, 1 },
	{ 0, 0, 3, 1, 1 },
	{ 2, 0, 3, 1, 1 },
	{ 0, 1, 2, 0, 0 },
	{ 2, 1, 2, 0, 0 },
	{ 0, 0, 2, 0, 0 },
	{ 2, 0, 2, 0, 0 },
	{ 0, 1, 2, 1, 0 },
	{ 2, 1, 2, 1, 0 },
	{ 0, 0, 2, 1, 0 },
	{ 2, 0, 2, 1, 0 },
	{ 0, 1, 2, 0, 1 },
	{ 2, 1, 2, 0, 1 },
	{ 0, 0, 2, 0, 1 },
	{ 2, 0, 2, 0, 1 },
	{ 0, 1, 2, 1, 1 },
	{ 2, 1, 2, 1, 1 },
	{ 0, 0, 2, 1, 1 },
	{ 2, 0, 2, 1, 1 },
};

static const Sint32 texfilter_tbl[4] = { 0, 1, 2, 3 };
static const Sint32 filter_tbl[4] = { 0, 1, 2, 3 };
static const Sint32 cull_tbl[4] = { 0, 0x08000000, 0x10000000, 0x18000000 };

Sint32 nl_ext_pal_height = 0;
Sint32 nl_UserTileClipMode = 0;
static Uint32 back_color[3] = { 0xFF000000, 0xFF000000, 0xFF000000 };
void (*pnlBlurStart0)( void ) = 0;
void (*pnlBlurStart1)( void ) = 0;
void (*pnlBlurDrawFB)( void ) = 0;
static Float timer_factor[5] = { 0.08f, 0.32f, 1.28f, 5.12f, 20.48f };
Sint32 nl_before_rend_ID = -1;
static Sint32 before_rend_rect = 0;

KMSYSTEMCONFIGSTRUCT nl_system_config;
KMSURFACEDESC *nl_surf[2];
Sint32 nl_latency_mode;
Sint32 nl_size_punch_thru;
Sint32 nl_clx2_flag;
Sint32 nl_trnsl_blend_mode;
static KMSURFACEDESC surface_a;
static KMSURFACEDESC surface_b;
Uint32 *kmiCurrentVertexBuff;
Sint32 nl_multi_pass;
Sint32 nl_in_scene;
Sint32 nl_vint_flag;
Sint32 nl_VGA_flag;
Sint32 nl_PAL_EXT_flag;
Sint32 nl_vwait_count;
NLVTXMODE cmode;
Sint32 nl_bpp;
Float nl_TimerMicrosecFactor0;
Sint32 nl_load_dummy_texture_flag;
Uint32 nlUseVtxBuffSizeWk[5];
static Uint8 texture_work[0x1000 * 24 + 0x400 * 76];
KMSYSTEMMETRICS nl_metrics_debug;
Uint32 nl_timer_data;
Sint32 nl_mp_buf[8][5];

char *__nl_sysver_include__( void )
{
	return nl_date_libnaomi_;
}

void nlSetPalExtMode( Sint32 mode )
{
	nl_ext_pal_height = mode;
}

void nl_pPalExtCallback( Sint32 *p )
{
	p[1] = nl_ext_pal_height;
}

/* starts the DMA channel that feeds the tile accelerator, and waits until the controller is on */
void sys_init( void )
{
	volatile Sint32 i;
	volatile Sint32 v;

	*(volatile Uint32 *)DMAC_SAR2 = 0;
	*(volatile Uint32 *)DMAC_CHCR2 = 0x1201;
	*(volatile Uint32 *)DMAC_DMAOR = 0x8201;
	while( ( v = *(volatile Uint32 *)DMAC_DMAOR ) != 0x8201 ){
		for( i = 0; i < 0x10000; i++ )
			;
		*(volatile Uint32 *)DMAC_DMAOR = 0x8201;
	}
}

void nlWaitVSync( void )
{
	kmEndPass( &nl_vertex_buffer );
	nl_action_PixelClipping();
	kmRender( 0 );
	if( pnlBlurStart0 )
		pnlBlurStart0();
	kmEndScene( &nl_system_config );
	if( pnlBlurStart1 )
		pnlBlurStart1();
	kmBeginScene( &nl_system_config );
	kmBeginPass( &nl_vertex_buffer );
	kmiCurrentVertexBuff = nl_vertex_buffer.pBuffer;
	if( pnlBlurDrawFB )
		pnlBlurDrawFB();
}

void nlSetVertexMode( Sint32 mode )
{
	nl_vertex_context_ptr->state = 0x600608;
	nl_vertex_context_ptr->list_type = vertex_mode_tbl[mode][0];
	nl_vertex_context_ptr->color_type = vertex_mode_tbl[mode][1];
	nl_vertex_context_ptr->shading = vertex_mode_tbl[mode][2];
	nl_vertex_context_ptr->use_alpha = vertex_mode_tbl[mode][3];
	nl_vertex_context_ptr->specular = vertex_mode_tbl[mode][4];
	cmode.list_type = vertex_mode_tbl[mode][0];
	cmode.color_type = vertex_mode_tbl[mode][1];
	cmode.shading = vertex_mode_tbl[mode][2];
	cmode.use_alpha = vertex_mode_tbl[mode][3];
	cmode.use_specular = vertex_mode_tbl[mode][4];
	kmProcessVertexRenderState( nl_vertex_context_ptr );
}

void nlSetShadingMode( Sint32 mode )
{
	Uint32 isp = nl_vertex_context_ptr->isp;
	Uint32 val;

	nl_vertex_context_ptr->shading = vertex_mode_tbl[mode][2];
	val = ( isp & 0xFCFFFFFF ) | ( nl_vertex_context_ptr->shading << 24 );
	nl_vertex_context_ptr->isp = val;
}

void nlSetUseSpecular( Sint32 mode )
{
	Uint32 tsp = nl_vertex_context_ptr->tsp;
	Uint32 val;

	nl_vertex_context_ptr->filter = mode;
	val = ( tsp & 0xFFDFFFFF ) | ( nl_vertex_context_ptr->specular << 21 );
	nl_vertex_context_ptr->tsp = val;
}

void nlSetUseAlpha( Sint32 mode )
{
	Uint32 tsp = nl_vertex_context_ptr->tsp;
	Uint32 val;

	nl_vertex_context_ptr->filter = mode;
	val = ( tsp & 0xFFEFFFFF ) | ( nl_vertex_context_ptr->use_alpha << 20 );
	nl_vertex_context_ptr->tsp = val;
}

void nlSetFilterMode( Sint32 mode )
{
	Uint32 tsp = nl_vertex_context_ptr->tsp;
	Uint32 val;

	nl_vertex_context_ptr->filter = filter_tbl[mode];
	val = ( tsp & 0xFFFF9FFF ) | ( nl_vertex_context_ptr->filter << 13 );
	nl_vertex_context_ptr->tsp = val;
}

static void init_vertex_context( KMSTRIPCONTEXT *ctx )
{
	ctx->state = 0x03F3FFFF;
	ctx->unk_04 = 0;
	ctx->list_type = 0;
	ctx->color_type = 1;
	ctx->specular = 0;
	ctx->unk_10 = 0;
	ctx->depth_mode = 4;
	ctx->unk_18 = 0;
	ctx->shading = 1;
	ctx->unk_24 = 0;
	ctx->unk_28 = 0;
	ctx->src_blend = 8;
	ctx->dst_blend = 6;
	ctx->unk_34 = 0;
	ctx->unk_38 = 0;
	ctx->fog_mode = 2;
	ctx->specular = 0;
	ctx->use_alpha = 0;
	ctx->ignore_tex_alpha = 0;
	ctx->unk_4c = 0;
	ctx->unk_50 = 0;
	ctx->filter = 0;
	ctx->unk_58 = 0;
	ctx->mipmap_adjust = 4;
	ctx->tex_shading = 3;
	ctx->texture = 0;
	ctx->unk_64 = 0;
	ctx->palette_bank = 0;
	ctx->face_a = 1.0f;
	ctx->face_r = 1.0f;
	ctx->face_g = 1.0f;
	ctx->face_b = 1.0f;
	ctx->offset_a = 1.0f;
	ctx->offset_r = 1.0f;
	ctx->offset_g = 1.0f;
	ctx->offset_b = 1.0f;
	kmProcessVertexRenderState( ctx );
	kmSetVertexRenderState( &nl_vertex_buffer, ctx );
}

static void init_back_context( KMSTRIPCONTEXT *ctx )
{
	ctx->state = 0x03F3FFFF;
	ctx->unk_04 = 0;
	ctx->list_type = 0;
	ctx->color_type = 0;
	ctx->specular = 0;
	ctx->unk_10 = 0;
	ctx->depth_mode = 4;
	ctx->unk_18 = 0;
	ctx->shading = 1;
	ctx->unk_24 = 0;
	ctx->unk_28 = 0;
	ctx->src_blend = 10;
	ctx->dst_blend = 11;
	ctx->unk_34 = 0;
	ctx->unk_38 = 0;
	ctx->fog_mode = 2;
	ctx->specular = 0;
	ctx->use_alpha = 0;
	ctx->ignore_tex_alpha = 1;
	ctx->unk_4c = 0;
	ctx->unk_50 = 0;
	ctx->filter = 0;
	ctx->unk_58 = 0;
	ctx->mipmap_adjust = 4;
	ctx->tex_shading = 1;
	ctx->texture = 0;
	ctx->unk_64 = 0;
	ctx->palette_bank = 0;
	ctx->face_a = 1.0f;
	ctx->face_r = 1.0f;
	ctx->face_g = 1.0f;
	ctx->face_b = 1.0f;
	ctx->offset_a = 1.0f;
	ctx->offset_r = 1.0f;
	ctx->offset_g = 1.0f;
	ctx->offset_b = 1.0f;
	kmProcessVertexRenderState( ctx );
	kmSetBackGroundRenderState( ctx );
}

void nlSetBackColor( Uint32 r, Uint32 g, Uint32 b )
{
	nlSetBackColorZ( 100000.0f, r, g, b );
}

Sint32 nlCreateVertexBuffer_clx16( void )
{
	return 1;
}

void nlSetFatalErrorMode( Sint32 mode );
void nl_system_config_init( Sint32 size );

void nlInitLibrary0( Sint32 mode, Uint32 flags, Sint32 vsync, Sint32 dc )
{
	nl_VGA_flag = 0;
	if( mode == 0 || mode == 21 || mode == 21 || mode == 21 )
		nl_VGA_flag = 1;
	nl_PAL_EXT_flag = 0;
	if( mode == 25 || mode == 24 || mode == 26 || mode == 27 )
		nl_PAL_EXT_flag = 1;
	set_imask( 14 );
	if( *(volatile Sint32 *)SB_SBREV >= 8 )
		nl_clx2_flag = 1;
	else
		nl_clx2_flag = 0;
	set_gbr( &NLSystemWork_ );
	sys_init();
	nl_scr.interlace = 0;
	nl_scr.vga = 0;
	if( flags & 0x80 )
		nl_scr.interlace = 1;
	if( flags & 0x8000 )
		nl_scr.vga = 1;
	nl_bpp = bpp_tbl[flags & 15];
	if( dc == 1 )
		kmInitDevice( 0x10000 );
	else
		kmInitDevice( 0 );
	if( nl_PAL_EXT_flag == 1 )
		kmSetPALEXTCallback( nl_pPalExtCallback, 0 );
	kmSetDisplayMode( dsp_mode_tbl[mode].mode, nl_bpp, nl_scr.interlace, nl_scr.vga );
	nl_scr.width = dsp_mode_tbl[mode].width;
	nl_scr.height = dsp_mode_tbl[mode].height;
	nl_set_txt_num = -1;
	nl_vertex_mode = 0;
	nl_frame_count = 0;
	nl_txt_count = 0;
	nl_deb_time[0] = 0.0f;
	nl_deb_time[1] = 0.0f;
	nl_deb_time[2] = 0.0f;
	nl_deb_time[3] = 0.0f;
	nl_counter[0] = 0;
	nl_counter[1] = 0;
	nl_counter[2] = 0;
	nl_counter[3] = 0;
	NLSystemWork_.scale_factor = 1.0f;
	NLSystemWork_.unk_44 = 0;
	*(Uint32 *)CCN_QACR0 = 12;
	*(Uint32 *)CCN_QACR1 = 12;
	nl_vwait_count = vsync;
	nl_system_config_init( 0x200000 );
	nlSetFatalErrorMode( 0 );
	nl_load_dummy_texture_flag = 0;
	nl_trnsl_blend_mode = 104;
	nl_multi_pass = 0;
	nl_in_scene = 0;
	pnlBlurStart0 = 0;
	pnlBlurStart1 = 0;
	pnlBlurDrawFB = 0;
	set_imask( 0 );
}

void nlInitLibrary( Sint32 mode, Uint32 flags, Sint32 vsync )
{
	nlInitLibrary0( mode, flags, vsync, 1 );
}

void nlInitLibraryDC( Sint32 mode, Uint32 flags, Sint32 vsync )
{
	nlInitLibrary0( mode, flags, vsync, 0 );
}

void nlSetVsyncCount( Sint32 count )
{
	nl_vwait_count = count;
	kmSetWaitVsyncCount( count );
}

void nlSetFogColor( Uint32 argb )
{
	KMPACKEDARGB col;

	col.dwPacked = argb;
	kmSetFogTableColor( col );
}

void nlSetFogVertexColor( Uint32 argb )
{
	KMPACKEDARGB col;

	col.dwPacked = argb;
	kmSetFogVertexColor( col );
}

void nlSetFogTable( Float *table )
{
	kmSetFogTable( table );
}

void nlSetCullingMode( Sint32 mode )
{
	Uint32 isp = nl_vertex_context_ptr->isp;

	nl_vertex_context_ptr->isp = ( isp & 0xE7FFFFFF ) | cull_tbl[mode];
}

void nlSetCullingVal( Float value )
{
	kmSetCullingRegister( value );
}

void nlSetBorderColor( Uint32 argb )
{
	KMPACKEDARGB col;

	col.dwPacked = argb | 0xFF000000;
	kmSetBorderColor( col );
}

void nlVtxBufScanInit( void )
{
	Sint32 i;
	Uint32 *src = kmiCurrentVertexBuff;
	Uint32 *dst = nlUseVtxBuffSizeWk;

	for( i = 0; i < 5; i++ )
		*dst++ = *src++;
}

void nlVtxBufScan( Sint32 *size )
{
	Sint32 i;
	Uint32 *base = nlUseVtxBuffSizeWk;
	Uint32 *src = kmiCurrentVertexBuff;

	for( i = 0; i < 5; i++ )
		*size++ = *src++ - *base++;
}

void nlSetUserTileClip( Sint32 x0, Sint32 y0, Sint32 x1, Sint32 y1, register Sint32 mask )
{
	NLCLIPRECT rect;
	Sint32 mode;
	Sint32 i;

	mode = ( *nl_vertex_buffer.pCtrl >> 16 ) & 3;
	rect.x0 = x0;
	rect.y0 = y0;
	rect.x1 = x1;
	rect.y1 = y1;
	for( i = 0; i < 5; i++, mask >>= 1 ){
		if( mask & 1 )
			kmSetUserClipping( &nl_vertex_buffer, mode, i, &rect );
	}
}

void nlSetUserTileClipMode( Sint32 mode )
{
	mode &= 3;
	if( mode == 1 )
		mode = 0;
	nl_UserTileClipMode = mode;
}

void nlFlushTileClip( Sint32 unused, Sint32 mode )
{
	*nl_vertex_buffer.pCtrl = 0x800000 | ( mode << 16 );
}

void nlChangeDisplayFilterMode( Uint32 flags )
{
	nl_scr.interlace = 0;
	if( flags & 0x80 )
		nl_scr.interlace = 1;
	nl_scr.vga = 0;
	if( flags & 0x8000 )
		nl_scr.vga = 1;
	kmChangeDisplayFilterMode( nl_scr.interlace, nl_scr.vga );
	nlSetBackColor( back_color[0], back_color[1], back_color[2] );
	nlSetWindowSize( nl_windowSizeLeft, nl_windowSizeTop, nl_windowSizeWidth, nl_windowSizeHeight );
}

Sint32 nlGetCurrentScanLine( void )
{
	Sint32 line;

	kmGetCurrentScanline( &line );
	return line;
}

void nlSetEORFunction( void (*func)( void * ) )
{
	kmSetEORCallback( func, 0 );
}

void nlSetEOVFunction( void (*func)( void * ) )
{
	kmSetEndOfVertexCallback( func, 0 );
}

void nlSetVSyncFunction( void (*func)( void * ) )
{
	kmSetVSyncCallback( func, 0 );
}

void nlSetHSyncFunction( void (*func)( void * ) )
{
	kmSetHSyncCallback( func, 0 );
}

void nlSetHsyncLine( Sint32 line )
{
	kmSetHSyncLine( line );
}

void nlSetTextureOverFunction( void (*func)( void * ) )
{
	kmSetTexOverflowCallback( func, 0 );
}

void nlSetStripOverFunction( void (*func)( void * ) )
{
	kmSetStripOverRunCallback( func, 0 );
}

void nl_system_config_init( Sint32 size )
{
	nl_system_config.dwSize = sizeof(KMSYSTEMCONFIGSTRUCT);
	nl_system_config.flags = 0;
	nl_surf[0] = &surface_a;
	nl_surf[1] = &surface_b;
	nl_system_config.pSurfaceDesc = nl_surf;
	nl_system_config.nFrameBuffers = 2;
	nl_system_config.flags |= 0x100;
	nl_system_config.texture_size = size << 2;
	nl_system_config.texture_structs = 0x1000;
	nl_system_config.small_vq_structs = 0x400;
	nl_system_config.texture_work = texture_work;
	nl_system_config.buffer_desc = &nl_vertex_buffer;
	nl_system_config.vertex_banks = 2;
	nl_system_config.base = 0xACC00000;
	nl_system_config.vertex_size = 0x400000;
	nl_system_config.passes = 1;
	nl_system_config.pass[0].flags = 0;
	nl_system_config.pass[0].ratio[0] = 70.0f;
	nl_system_config.pass[0].ratio[1] = 5.0f;
	nl_system_config.pass[0].ratio[2] = 25.0f;
	nl_system_config.pass[0].ratio[3] = 0.0f;
	nl_system_config.pass[0].ratio[4] = 0.0f;
	nl_system_config.pass[0].list_type = 0;
	nl_latency_mode = 3;
	nl_size_punch_thru = 0;
}

void nlSysSetTextureSize( Sint32 size )
{
	nl_system_config.texture_size = size << 2;
}

void nlSysUseLatency2V( void )
{
	if( nl_clx2_flag == 1 ){
		nl_system_config.pass[0].list_type = 0;
		nl_system_config.flags |= 0x10;
		nl_latency_mode = 2;
	}else
		kmChangeLatencyModel( 1, 0 );
}

void nlSysUseLatency3V( void )
{
	if( nl_clx2_flag == 1 ){
		nl_system_config.pass[0].list_type = 0;
		nl_system_config.flags &= ~0x10;
		nl_latency_mode = 3;
	}else
		kmChangeLatencyModel( 0, 0 );
}

void nlSysSetDirectListTypeLatency2V( Sint32 type )
{
	Sint32 list = 0;

	switch( type ){
	case 0:
		list = 0;
		break;
	case 2:
		list = 2;
		break;
	case 4:
		list = 4;
		break;
	}
	if( nl_clx2_flag == 1 )
		nl_system_config.pass[0].list_type = list;
	else
		kmChangeLatencyModel( 1, list );
}

void nlSysUseStripBuffer( void )
{
	nl_system_config.flags |= 2;
}

void nlSysCancelStripBuffer( void )
{
	nl_system_config.flags &= ~2;
}

void nlSysNoWaitRenderingOver( void )
{
	nl_vwait_count = 0;
}

void nlSysWaitRenderingOver( void )
{
	kmfSetTimeOutCount( 10, 10 );
}

void nlSysCreatePunchBuffer( Sint32 size )
{
	nl_size_punch_thru = size;
}

void nlSysSetVertexBufferBaseAddress( Uint32 addr )
{
	if( nl_clx2_flag == 1 )
		nl_system_config.base = addr | P2_AREA;
}

/* shares the vertex buffer out between the five lists (opaque, opaque modifier, translucent, translucent
   modifier and punch-through), by the sizes asked for, and sets the configuration */
Sint32 nlCreateVertexBuffer_clx22( Sint32 op, Sint32 om, Sint32 tr, Sint32 tm )
{
	KMSYSTEMCONFIGSTRUCT *sys = &nl_system_config;
	Sint32 pt = nl_size_punch_thru;
	Sint32 n, mask, i;
	Sint32 ret;

	if( nl_latency_mode == 3 ){
		n = op + om + tr + tm + pt;
		n = n + n;
	}else if( sys->pass->list_type == 0 )
		n = om + tr + tm + pt;
	else if( sys->pass->list_type == 2 )
		n = op + om + tm + pt;
	else if( sys->pass->list_type == 4 )
		n = op + om + tr + tm;
	sys->vertex_size = n << 2;
	if( nl_latency_mode == 3 ){
		n /= 2;
		sys->pass->ratio[0] = (Float)op * 100.0f / (Float)n;
		sys->pass->ratio[1] = (Float)om * 100.0f / (Float)n;
		sys->pass->ratio[2] = (Float)tr * 100.0f / (Float)n;
		sys->pass->ratio[3] = (Float)tm * 100.0f / (Float)n;
		sys->pass->ratio[4] = (Float)pt * 100.0f / (Float)n;
	}else if( sys->pass->list_type == 0 ){
		sys->pass->ratio[0] = 0.0f;
		sys->pass->ratio[1] = (Float)om * 100.0f / (Float)n;
		sys->pass->ratio[2] = (Float)tr * 100.0f / (Float)n;
		sys->pass->ratio[3] = (Float)tm * 100.0f / (Float)n;
		sys->pass->ratio[4] = (Float)pt * 100.0f / (Float)n;
	}else if( sys->pass->list_type == 2 ){
		sys->pass->ratio[0] = (Float)op * 100.0f / (Float)n;
		sys->pass->ratio[1] = (Float)om * 100.0f / (Float)n;
		sys->pass->ratio[2] = 0.0f;
		sys->pass->ratio[3] = (Float)tm * 100.0f / (Float)n;
		sys->pass->ratio[4] = (Float)pt * 100.0f / (Float)n;
	}else if( sys->pass->list_type == 4 ){
		sys->pass->ratio[0] = (Float)op * 100.0f / (Float)n;
		sys->pass->ratio[1] = (Float)om * 100.0f / (Float)n;
		sys->pass->ratio[2] = (Float)tr * 100.0f / (Float)n;
		sys->pass->ratio[3] = (Float)tm * 100.0f / (Float)n;
		sys->pass->ratio[4] = 0.0f;
	}
	if( nl_multi_pass != 0 ){
		KMSYSTEMCONFIGSTRUCT *config = &nl_system_config;

		if( nl_latency_mode == 3 ){
			for( i = 1; i < config->passes; i++ ){
				config->pass[i].ratio[0] = config->pass[0].ratio[0];
				config->pass[i].ratio[1] = config->pass[0].ratio[1];
				config->pass[i].ratio[2] = config->pass[0].ratio[2];
				config->pass[i].ratio[3] = config->pass[0].ratio[3];
				config->pass[i].ratio[4] = config->pass[0].ratio[4];
			}
		}else{
			mask = 0;
			for( i = 0; i < config->passes; i++ ){
				if( config->pass[i].list_type == 0 ){
					n = om + tr + tm + pt;
					mask |= 1;
					config->pass[i].ratio[0] = 0.0f;
					config->pass[i].ratio[1] = (Float)om * 100.0f / (Float)n;
					config->pass[i].ratio[2] = (Float)tr * 100.0f / (Float)n;
					config->pass[i].ratio[3] = (Float)tm * 100.0f / (Float)n;
					config->pass[i].ratio[4] = (Float)pt * 100.0f / (Float)n;
				}else if( config->pass[i].list_type == 2 ){
					mask |= 16;
					n = op + om + tm + pt;
					config->pass[i].ratio[0] = (Float)op * 100.0f / (Float)n;
					config->pass[i].ratio[1] = (Float)om * 100.0f / (Float)n;
					config->pass[i].ratio[2] = 0.0f;
					config->pass[i].ratio[3] = (Float)tm * 100.0f / (Float)n;
					config->pass[i].ratio[4] = (Float)pt * 100.0f / (Float)n;
				}else if( config->pass[i].list_type == 4 ){
					mask |= 256;
					n = op + om + tr + tm;
					config->pass[i].ratio[0] = (Float)op * 100.0f / (Float)n;
					config->pass[i].ratio[1] = (Float)om * 100.0f / (Float)n;
					config->pass[i].ratio[2] = (Float)tr * 100.0f / (Float)n;
					config->pass[i].ratio[3] = (Float)tm * 100.0f / (Float)n;
					config->pass[i].ratio[4] = 0.0f;
				}
			}
			if( mask == 1 || mask == 16 || mask == 256 )
				config->vertex_size = n << 2;
			else
				config->vertex_size = ( op + om + tr + tm + pt ) << 2;
		}
	}
	ret = kmSetSystemConfiguration( &nl_system_config );
	if( ret != 0 )
		trapa( 65 );
	*nl_vertex_buffer.pCtrl = 0x800000;
	if( nl_load_dummy_texture_flag == 0 ){
		nlLoadDummyTexture();
		nl_load_dummy_texture_flag = 1;
	}
	return ret;
}

Sint32 nlCreateVertexBuffer( Sint32 op, Sint32 om, Sint32 tr, Sint32 tm )
{
	Sint32 ret;
	Sint32 hh;

	if( nl_clx2_flag == 1 )
		ret = nlCreateVertexBuffer_clx22( op, om, tr, tm );
	else
		ret = nlCreateVertexBuffer_clx16();
	init_back_context( &nl_back_context );
	init_vertex_context( &nl_vertex_context[0] );
	nl_vertex_context_ptr = &nl_vertex_context[0];
	nl_vertex_context_depth = 0;
	if( nl_vertex_context_depth ){
		hh = nl_scr.height;
		if( nl_scr.vga == 1 ){
			hh = hh + hh;
			if( hh > 480 )
				hh = 480;
		}
		nlSetWindowSize( 0.0f, 0.0f, nl_scr.width, hh );
	}else
		nlSetWindowSize( 0.0f, 0.0f, nl_scr.width, nl_scr.height );
	kmSetCullingRegister( 0.02f );
	*(Uint32 *)CCN_QACR0 = 12;
	*(Uint32 *)CCN_QACR1 = 12;
	kmSetWaitVsyncCount( nl_vwait_count );
	if( nl_multi_pass == 0 ){
		kmBeginScene( &nl_system_config );
		kmBeginPass( &nl_vertex_buffer );
		kmiCurrentVertexBuff = nl_vertex_buffer.pBuffer;
	}
	return ret;
}

void nlSysTextureWaitDMA( void )
{
	nl_system_config.flags &= ~0x100000;
}

void nlSysTextureNoWaitDMA( void )
{
	nl_system_config.flags |= 0x100000;
}

/* starts timer 0 counting down from the top, a count every 1.28 microseconds */
void nlSysMetricsInit( void )
{
	Sint32 tstr;

	set_imask( 14 );
	tstr = *(Sint8 *)TMU_TSTR;
	*(Sint8 *)TMU_TSTR = tstr & ~1;
	*(Sint8 *)TMU_TOCR = 1;
	*(Uint16 *)TMU_TCR0 = 2;
	*(Uint32 *)TMU_TCOR0 = -1;
	*(Uint32 *)TMU_TCNT0 = -1;
	*(Sint8 *)TMU_TSTR = tstr | 1;
	nl_TimerMicrosecFactor0 = 1.28f;
	set_imask( 0 );
	nl_metrics_debug.unk_00 = 30;
}

/* the same at one of the timer's five clocks */
void nlSysMetricsInitSetClock( Sint32 clock )
{
	Sint32 tstr;

	set_imask( 14 );
	if( clock < 0 || clock > 4 )
		clock = 2;
	tstr = *(Sint8 *)TMU_TSTR;
	*(Sint8 *)TMU_TSTR = tstr & ~1;
	*(Sint8 *)TMU_TOCR = 1;
	*(Uint16 *)TMU_TCR0 = clock;
	*(Uint32 *)TMU_TCOR0 = -1;
	*(Uint32 *)TMU_TCNT0 = -1;
	*(Sint8 *)TMU_TSTR = tstr | 1;
	nl_TimerMicrosecFactor0 = timer_factor[clock];
	set_imask( 0 );
	nl_metrics_debug.unk_00 = 14;
}

void nlSysMetricsStart( void )
{
	kmGetSystemMetrics( &nl_metrics_debug );
}

Sint32 nlSysMetricsGetLastTATime( void )
{
	return nl_metrics_debug.time[8];
}

Sint32 nlSysMetricsGetLastRenderTime( void )
{
	return nl_metrics_debug.time[0];
}

Sint32 nlSysMetricsGetNativeUseSize( void )
{
	return nl_metrics_debug.native_use_size;
}

Sint32 nlSysMetricsGetNativeMaxSize( void )
{
	return nl_metrics_debug.native_max_size;
}

/* copies the frame buffer that is shown */
void nlSysMetricsGetFrameBuffer( void *buf )
{
	Uint32 *dst = buf;
	Uint32 *src;
	Sint32 *fb;
	Sint32 w, x, y;

	w = nl_scr.width;
	fb = &nl_metrics_debug.fb_index;
	if( *fb >= 0 ){
		src = (Uint32 *)( (Uint32)nl_system_config.pSurfaceDesc[*fb]->pSurface + VRAM_32BIT );
		if( nl_bpp <= 3 )
			w *= 2;
		else if( nl_bpp <= 4 )
			w *= 3;
		else if( nl_bpp <= 5 )
			w *= 4;
		else
			for( ;; )
				;
		w /= 4;
		for( y = 0; y < nl_scr.height; y++ ){
			for( x = 0; x < w; x++ )
				*dst++ = *src++;
		}
	}
}

/* copies every fifth pixel of every fifth line of it */
void nlSysMetricsGetFrameBufferUser( void *buf )
{
	Uint16 *dst = buf;
	Uint16 *src;
	Sint32 *fb;
	Sint32 w, x, y;

	w = nl_scr.width;
	fb = &nl_metrics_debug.fb_index;
	if( *fb >= 0 ){
		src = (Uint16 *)( (Uint32)nl_system_config.pSurfaceDesc[*fb]->pSurface + VRAM_32BIT );
		if( nl_bpp <= 3 )
			w *= 2;
		else if( nl_bpp <= 4 )
			w *= 3;
		else if( nl_bpp <= 5 )
			w *= 4;
		else
			for( ;; )
				;
		w /= 2;
		dst += 0x800;
		for( y = 0; y < nl_scr.height; y += 5 ){
			for( x = 0; x < w; x += 5 ){
				*dst++ = *src;
				src += 5;
			}
			src += 0xA00;
		}
	}
}

static Uint32 get_tcnt0( void )
{
	return *(volatile Uint32 *)TMU_TCNT0;
}

static Sint32 ticks_to_us( Sint32 ticks )
{
	return (Sint32)( (Float)ticks * nl_TimerMicrosecFactor0 );
}

static Sint32 tick_diff( Sint32 a, Sint32 b )
{
	return a - b;
}

void nlSysMetricsCpuPowerStart( void )
{
	nl_timer_data = get_tcnt0();
}

Sint32 nlSysMetricsCpuPowerGet( void )
{
	return ticks_to_us( tick_diff( nl_timer_data, get_tcnt0() ) );
}

Sint32 nlSysMetricsCpuPowerEnd( void )
{
	return ticks_to_us( tick_diff( nl_timer_data, get_tcnt0() ) );
}

void nlSetColorClampValue( Uint32 min, Uint32 max )
{
	KMPACKEDARGB a, b;

	a.dwPacked = min;
	b.dwPacked = max;
	kmSetColorClampValue( a, b );
}

void nlSetTrnslMode( Sint32 src, Sint32 dst )
{
	nl_trnsl_blend_mode = ( ( ~src & 7 ) << 5 ) + ( ( ~dst & 7 ) << 2 );
}

/* Kamui2's fatal error: shows the error as a border colour and stops in a trap */
void nlFatalErrorTrap( Sint32 code )
{
	KMPACKEDARGB col;

	switch( code ){
	case 1:
		col.dwPacked = 0xFF0000FF;
		break;
	case 2:
		col.dwPacked = 0xFFFF0000;
		break;
	case 3:
		col.dwPacked = 0xFF00FF00;
		break;
	case 4:
		col.dwPacked = 0xFFFF00FF;
		break;
	}
	if( code != 3 ){
		kmSetBorderColor( col );
		set_cr( get_cr() & 0xEFFFFF0F );
		switch( code ){
		case 1:
			trapa( 48 );
		case 2:
			trapa( 49 );
		case 3:
			trapa( 50 );
		case 4:
			trapa( 51 );
		}
	}
}

void nlFatalErrorReturn( Sint32 code )
{
}

void nlSetFatalErrorMode( Sint32 mode )
{
	if( mode == 0 )
		kmSetFatalErrorCallback( nlFatalErrorTrap, 0 );
	else
		kmSetFatalErrorCallback( nlFatalErrorReturn, 0 );
}

void nlUseSameModifier( void )
{
	nl_system_config.pass[0].flags |= 0xA0;
}

void nlSetPunchAlphaThreshold( Sint32 value )
{
	*(volatile Uint32 *)PVR_PT_ALPHA_REF = value;
}

Uint32 nlSysGetVertexBufferSize( void )
{
	return ( nl_system_config.vertex_size << 2 ) >> 2;
}

Sint32 nlSysMetricsGetRenderTimeOutCount( void )
{
	return nl_metrics_debug.render_timeout;
}

Sint32 nlSysMetricsGetVertexTimeOutCount( void )
{
	return nl_metrics_debug.vertex_timeout;
}

void nlSysSetTimeOutCount( Sint32 render, Sint32 vertex )
{
	kmfSetTimeOutCount( render, vertex );
}

void nlExitSystem( void )
{
	kmDiscardVertexBuffer( &nl_vertex_buffer );
}

/* the background plane: a triangle over the screen at depth z, a colour at each corner */
void nlSetBackColorZ( register Float z, Uint32 r, Uint32 g, Uint32 b )
{
	KMBGVERTEX v[3];
	KMBGVERTEX *tri[3];
	Float invz = 1.0f / z;

	back_color[0] = r;
	back_color[1] = g;
	back_color[2] = b;
	v[0].flags = KM_VERTEXPARAM_NORMAL;
	v[0].x = 0.0f;
	v[0].y = 0.0f;
	v[0].z = invz;
	v[0].base = r;
	v[0].offset = r;
	v[1].flags = KM_VERTEXPARAM_NORMAL;
	if( nl_scr.vga )
		v[1].x = nl_scr.width * 2;
	else
		v[1].x = nl_scr.width;
	v[1].y = 0.0f;
	v[1].z = invz;
	v[1].base = g;
	v[1].offset = g;
	v[2].flags = KM_VERTEXPARAM_ENDOFSTRIP;
	v[2].x = 0.0f;
	v[2].y = nl_scr.height;
	v[2].z = invz;
	v[2].base = b;
	v[2].offset = b;
	tri[0] = &v[0];
	tri[1] = &v[1];
	tri[2] = &v[2];
	kmSetBackGroundPlane( tri, 0, 32 );
}

void nlSysSetAutoSortMode( Sint32 mode )
{
	if( mode == 1 )
		;
}

Sint32 nlChangeDisplayMode( Sint32 mode, Uint32 flags )
{
	Sint32 ret;
	Sint32 clamp_height;
	Sint32 hh;

	clamp_height = 0;
	nl_VGA_flag = 0;
	if( mode == 0 || mode == 21 || mode == 21 || mode == 21 )
		nl_VGA_flag = 1;
	nl_PAL_EXT_flag = 0;
	if( mode == 25 || mode == 24 || mode == 26 || mode == 27 )
		nl_PAL_EXT_flag = 1;
	if( nl_multi_pass == 0 ){
		nlSysEndPass();
		nlSysRender( 0 );
		nlSysEndScene();
	}
	nl_scr.interlace = 0;
	nl_scr.vga = 0;
	if( flags & 0x80 )
		nl_scr.interlace = 1;
	if( flags & 0x8000 )
		nl_scr.vga = 1;
	ret = bpp_tbl[flags & 15];
	if( nl_PAL_EXT_flag == 1 )
		kmSetPALEXTCallback( nl_pPalExtCallback, 0 );
	kmSetDisplayMode( dsp_mode_tbl[mode].mode, ret, nl_scr.interlace, nl_scr.vga );
	nl_scr.width = dsp_mode_tbl[mode].width;
	nl_scr.height = dsp_mode_tbl[mode].height;
	ret = kmSetSystemConfiguration( &nl_system_config );
	nlLoadDummyTexture();
	nl_load_dummy_texture_flag = 1;
	nlSetBackColor( back_color[0], back_color[1], back_color[2] );
	if( clamp_height ){
		hh = nl_scr.height;
		if( nl_scr.vga == 1 ){
			hh = hh + hh;
			if( hh > 480 )
				hh = 480;
		}
		nlSetWindowSize( 0.0f, 0.0f, nl_scr.width, hh );
	}else
		nlSetWindowSize( 0.0f, 0.0f, nl_scr.width, nl_scr.height );
	if( nl_multi_pass == 0 ){
		nlSysBeginScene();
		nlSysBeginPass();
	}
	return ret;
}

void kmfSetTimeOutCount( Sint32 render, Sint32 vertex )
{
	kmiExtension.time_out_count = vertex;
}

/* the same for one pass of several: its buffer follows the pass before */
Sint32 nlCreateVertexBuffer_multi_pass( Sint32 pass, Sint32 op, Sint32 om, Sint32 tr, Sint32 tm, Sint32 pt )
{
	KMSYSTEMCONFIGSTRUCT *config = &nl_system_config;
	Sint32 n, ret;

	if( nl_latency_mode == 3 ){
		n = op + om + tr + tm + pt;
		n = n + n;
	}else if( config->pass[pass].list_type == 0 )
		n = om + tr + tm + pt;
	else if( config->pass[pass].list_type == 2 )
		n = op + om + tm + pt;
	else if( config->pass[pass].list_type == 4 )
		n = op + om + tr + tm;
	ret = n;
	config->pass[pass].size = ret << 2;
	if( pass == 0 )
		config->pass[pass].buffer = config->base;
	else
		config->pass[pass].buffer = config->pass[pass - 1].buffer + config->pass[pass - 1].size;
	if( nl_latency_mode == 3 ){
		n /= 2;
		config->pass[pass].ratio[0] = (Float)op * 100.0f / (Float)n;
		config->pass[pass].ratio[1] = (Float)om * 100.0f / (Float)n;
		config->pass[pass].ratio[2] = (Float)tr * 100.0f / (Float)n;
		config->pass[pass].ratio[3] = (Float)tm * 100.0f / (Float)n;
		config->pass[pass].ratio[4] = (Float)pt * 100.0f / (Float)n;
	}else if( config->pass[pass].list_type == 0 ){
		config->pass[pass].ratio[0] = 0.0f;
		config->pass[pass].ratio[1] = (Float)om * 100.0f / (Float)n;
		config->pass[pass].ratio[2] = (Float)tr * 100.0f / (Float)n;
		config->pass[pass].ratio[3] = (Float)tm * 100.0f / (Float)n;
		config->pass[pass].ratio[4] = (Float)pt * 100.0f / (Float)n;
	}else if( config->pass[pass].list_type == 2 ){
		config->pass[pass].ratio[0] = (Float)op * 100.0f / (Float)n;
		config->pass[pass].ratio[1] = (Float)om * 100.0f / (Float)n;
		config->pass[pass].ratio[2] = 0.0f;
		config->pass[pass].ratio[3] = (Float)tm * 100.0f / (Float)n;
		config->pass[pass].ratio[4] = (Float)pt * 100.0f / (Float)n;
	}else if( config->pass[pass].list_type == 4 ){
		config->pass[pass].ratio[0] = (Float)op * 100.0f / (Float)n;
		config->pass[pass].ratio[1] = (Float)om * 100.0f / (Float)n;
		config->pass[pass].ratio[2] = (Float)tr * 100.0f / (Float)n;
		config->pass[pass].ratio[3] = (Float)tm * 100.0f / (Float)n;
		config->pass[pass].ratio[4] = 0.0f;
	}
	return ret;
}

void nlSysMultiPassMode( Sint32 passes )
{
	KMSYSTEMCONFIGSTRUCT *config = &nl_system_config;
	Sint32 i;

	nlSysUseLatency2V();
	nl_multi_pass = 1;
	config->passes = passes;
	for( i = 0; i < passes; i++ ){
		config->pass[i].list_type = 0;
		config->pass[i].flags = 0;
	}
}

Sint32 nlSysMultiPassCreateVertexBuffer( Sint32 pass, Sint32 op, Sint32 om, Sint32 tr, Sint32 tm, Sint32 pt )
{
	Sint32 ret;
	Sint32 i;
	Sint32 hh;

	nl_mp_buf[pass][0] = op;
	nl_mp_buf[pass][1] = om;
	nl_mp_buf[pass][2] = tr;
	nl_mp_buf[pass][3] = tm;
	nl_mp_buf[pass][4] = pt;
	if( pass >= nl_system_config.passes - 1 ){
		{
			KMSYSTEMCONFIGSTRUCT *sys = &nl_system_config;

			sys->flags |= 0x40;
		}
		for( i = 0; i < nl_system_config.passes; i++ )
			nlCreateVertexBuffer_multi_pass( i, nl_mp_buf[i][0], nl_mp_buf[i][1], nl_mp_buf[i][2], nl_mp_buf[i][3], nl_mp_buf[i][4] );
		ret = kmSetSystemConfiguration( &nl_system_config );
		if( ret != 0 )
			trapa( 65 );
		*nl_vertex_buffer.pCtrl = 0x800000;
		if( nl_load_dummy_texture_flag == 0 ){
			nlLoadDummyTexture();
			nl_load_dummy_texture_flag = 1;
		}
		init_back_context( &nl_back_context );
		init_vertex_context( &nl_vertex_context[0] );
		nl_vertex_context_ptr = &nl_vertex_context[0];
		nl_vertex_context_depth = 0;
		if( nl_vertex_context_depth ){
			hh = nl_scr.height;
			if( nl_scr.vga == 1 ){
				hh = hh + hh;
				if( hh > 480 )
					hh = 480;
			}
			nlSetWindowSize( 0.0f, 0.0f, nl_scr.width, hh );
		}else
			nlSetWindowSize( 0.0f, 0.0f, nl_scr.width, nl_scr.height );
		kmSetCullingRegister( 0.02f );
		*(Uint32 *)CCN_QACR0 = 12;
		*(Uint32 *)CCN_QACR1 = 12;
		kmSetWaitVsyncCount( nl_vwait_count );
		return ret;
	}
	return 0;
}

Sint32 nlSysBeginScene( void )
{
	nl_in_scene = 1;
	kmBeginScene( &nl_system_config );
	return 0;
}

Sint32 nlSysEndScene( void )
{
	kmEndScene( &nl_system_config );
	return 0;
}

Sint32 nlSysBeginPass( void )
{
	kmBeginPass( &nl_vertex_buffer );
	kmiCurrentVertexBuff = nl_vertex_buffer.pBuffer;
	return 0;
}

Sint32 nlSysEndPass( void )
{
	kmEndPass( &nl_vertex_buffer );
	return 0;
}

/* renders the scene, in the region's tiles alone when a pixel clip has changed them */
Sint32 nlSysRender( Sint32 mode )
{
	Sint32 xmin, ymin, xmax, ymax;
	Sint32 tx, ty;

	if( nl_reg_clip_start != 0 || before_rend_rect != 0 ){
		xmin = nl_reg_clip_xmin;
		xmax = nl_reg_clip_xmax;
		ymin = nl_reg_clip_ymin;
		ymax = nl_reg_clip_ymax;
		if( nl_scr.vga ){
			tx = ( ( nl_scr.width + 15 ) >> 4 ) - 1;
			if( nl_scr.height * 2 <= 480 )
				ty = ( ( nl_scr.height + 15 ) >> 4 ) - 1;
			else
				ty = ( ( nl_scr.height + 31 ) >> 5 ) - 1;
		}else{
			tx = ( ( nl_scr.width + 31 ) >> 5 ) - 1;
			ty = ( ( nl_scr.height + 31 ) >> 5 ) - 1;
		}
		if( nl_reg_clip_start != 0 )
			before_rend_rect = 1;
		else
			before_rend_rect = 0;
		nl_reg_clip_xmin0 = xmin;
		nl_reg_clip_xmax0 = xmax;
		nl_reg_clip_ymin0 = ymin;
		nl_reg_clip_ymax0 = ymax;
		if( xmax > tx )
			xmax = tx;
		if( ymax >= ty )
			ymax = ty;
		kmSetRegionClipping( xmin, ymin, xmax, ymax );
		nl_action_PixelClipping();
		if( mode == 1 )
			nl_before_rend_ID = kmRenderRect( 0 );
		else
			nl_before_rend_ID = kmRenderRect( 1 );
		nl_reg_clip_start = 0;
	}else{
		kmRender( mode == 1 ? 0 : 1 );
		nl_before_rend_ID = -1;
	}
	nl_in_scene = 0;
	return 0;
}

void nlSysWaitVSyncMode( void )
{
	nl_system_config.flags &= ~0x20;
}

void nlSysNoWaitVSyncMode( void )
{
	nl_system_config.flags |= 0x20;
}

void nlSysMultiPassSetAutoSortMode( Sint32 pass, Sint32 mode )
{
	if( mode == 1 )
		;
}

void nlSysMultiPassUseSameModifier( Sint32 pass )
{
	nl_system_config.pass[pass].flags |= 0xA0;
}

void nlSysMultiPassNoUseSameModifier( Sint32 pass )
{
	nl_system_config.pass[pass].flags &= ~0xA0;
}

void nlSysMultiPassSetDirectListTypeLatency2V( Sint32 pass, Sint32 type )
{
	Sint32 list = 0;

	switch( type ){
	case 0:
		list = 0;
		break;
	case 2:
		list = 2;
		break;
	case 4:
		list = 4;
		break;
	}
	nl_system_config.pass[pass].list_type = list;
}
