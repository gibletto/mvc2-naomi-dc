/* nlstrip: the NAOMI library's strip drawing, in its Oct 1999 version: textures and palettes
   of a strip list, and strips sent to the tile accelerator through the store queues. */
#include "nlstrip.h"
#include "nl.h"

/* the store queues' area, which a vertex's first word also names, and the word of a strip's
   last vertex */
#define SQ_AREA		0xE0000000
#define SQ_MASK		0x03FFFFFF
#define STRIP_END	0xF0000000

/* the settings a strip gives the vertex context, and its palette bank */
#define STATE_STRIP	0x600608
#define STATE_PALETTE	0x80000

/* the store queue address registers, and the prefetch that sends a queue */
#define QACR0		( *(Uint32 *)0xFF000038 )
#define QACR1		( *(Uint32 *)0xFF00003C )
#define prefetch( p )	_builtin_prefetch( p )

const char nl_date_libstrip[24] = "Oct 28 1999 18:08:51";

/* how a strip's vertices are laid out: words per vertex, and the one that holds the colour */
static const Sint32 strip_fmt[18][2] = {
	{ 4, 3 }, { 7, 0 }, { 0, 0 }, { 7, 5 }, { 0, 0 }, { 13, 0 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 5, 3 }, { 0, 0 }, { 11, 5 },
	{ 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 9, 0 },
};

Sint32 nl_error_libstrip;
Sint32 nl_disp_palette;
Sint32 nl_disp_coord;

/* the settings of each kind of strip (list, colour type, shading, alpha, offset colour): two
   coloured, two translucent, then textured: opaque, translucent, and the same with an offset colour */
static Sint32 strip_blend[8][5] = {
	{ 0, 0, 1, 0, 0 }, { 0, 0, 1, 0, 0 },
	{ 2, 0, 1, 1, 0 }, { 2, 0, 1, 1, 0 },
	{ 0, 0, 3, 1, 0 }, { 2, 0, 3, 1, 0 },
	{ 0, 0, 3, 1, 1 }, { 2, 0, 3, 1, 1 },
};

static void nlstripSetVertexContext( Sint32 *blend );
static Sint32 nlstripSetVertexContextTex( Sint32 *blend, Sint32 tex );
static void nldispstripSub( Sint32 num, Sint32 mode, Uint32 *vt );
static void nldispstripTexSub( Sint32 num, NLVERTEX03 *vt );
static void nldispstripTexOfsSub( Sint32 num, NLVERTEX03 *vt );

/* the store queue code every strip routine shares, for the header of the list and each vertex */
#define nlstripPut( src, len ) {					\
	Sint32 idx;							\
	Uint32 size, sq, addr;						\
									\
	idx = nl_vertex_buffer.slot->index;				\
	addr = (Uint32)nl_vertex_buffer.write[idx];			\
	size = len;							\
	QACR0 = QACR1 = addr >> 24;					\
	sq = ( addr & SQ_MASK ) | SQ_AREA;				\
	kmyMemCopy( (void *)sq, src, size );				\
	prefetch( (void *)sq );						\
	if( size == 64 )						\
		prefetch( (void *)( sq + 32 ) );			\
	nl_vertex_buffer.write[idx] += size >> 2;			\
}

void nlDispStripInit( void )
{
	nl_error_libstrip = 0;
	nl_disp_coord = 0;
}

void nlDispStripCoordMode( Sint32 mode )
{
	nl_disp_coord = mode;
}

/* loads a strip's texture the first time it is used, and counts its users */
Sint32 nlDispStripLoadTexture( Sint32 index, NLthd *thd )
{
	NLTEXPARAM *param;
	Sint32 slot, err;

	if( nl_texture_loadflag[index].count == 0 )
		nl_texture_loadflag[index].slot = nlCommonTextureGetNextTextureIndex();
	nl_texture_loadflag[index].count++;
	slot = nl_texture_loadflag[index].slot;
	param = &nl_texture_param[slot];
	if( param->users == 0 ){
		err = nlLoadTextureMem( thd, slot, (Sint32)thd->address );
		if( err ){
			nl_error_libstrip = 1;
			return 1;
		}
		nlCreateTextureParam( slot );
		param->index = slot;
		param->users++;
		return 0;
	}
	param->users++;
	return 0;
}

/* frees the texture when its last user lets go of it */
void nlDispStripFreeTexture( Sint32 index )
{
	NLTEXPARAM *param;
	Sint32 slot, err;

	if( nl_texture_loadflag[index].count > 0 ){
		nl_texture_loadflag[index].count--;
		if( nl_texture_loadflag[index].count == 0 ){
			slot = nl_texture_loadflag[index].slot;
			param = &nl_texture_param[slot];
			if( param->status == 0 ){
				err = kmFreeTexture( param );
				if( err ){
					nl_error_libstrip = 2;
					for( ;; ) ;
				}
			}
			param->users = 0;
			param->index = -1;
			nlCommonTextureClearTextureIndex( slot );
			nl_texture_loadflag[index].slot = -1;
		}
	}
}

/* gives a palette of the list a place in the palette memory, the first time it is used */
Sint32 nlDispStripLoadPalette( Sint32 index, NLtpal *pal )
{
	NLPALETTE *list;
	Sint32 map;

	if( nl_palette_direct_mode != 0 )
		return 0;
	if( index < NLSystemWork_.palettes ){
		list = &NLSystemWork_.palette[index];
		if( list->count != 0 ){
			list->count++;
			return 0;
		}
		map = nlSearchPalette( pal->attr, pal->size );
		if( map >= 0 ){
			list->attr = pal->attr;
			list->size = ( pal->size + 15 ) >> 4;
			list->count = 1;
			list->index = index;
			list->map = map;
			nlSetPaletteMap( map, pal->size );
			nlSetPaletteData( map << 4, pal->colors, pal->size );
			return 0;
		}
	}
	return 1;
}

void nlDispStripFreePalette( Sint32 index )
{
	if( nl_palette_direct_mode )
		return;
	if( index >= NLSystemWork_.palettes )
		return;
	nlFreePalette( index );
}

void nlDispStripSetPalette( Sint32 index )
{
	nl_disp_palette = index;
}

void nlDispStrip( Sint32 num, NLVERTEX00 *vt )
{
	nlstripSetVertexContext( strip_blend[0] );
	nldispstripSub( num, 0, (Uint32 *)vt );
}

void nlDispStripX( Sint32 num, Sint32 mode, NLVERTEX00 *vt )
{
	nlstripSetVertexContext( strip_blend[1] );
	nldispstripSub( num, mode, (Uint32 *)vt );
}

void nlDispStripTrnsl( Sint32 num, NLVERTEX00 *vt )
{
	nlstripSetVertexContext( strip_blend[2] );
	nldispstripSub( num, 0, (Uint32 *)vt );
}

void nlDispStripTrnslX( Sint32 num, Sint32 mode, NLVERTEX00 *vt )
{
	nlstripSetVertexContext( strip_blend[3] );
	nldispstripSub( num, mode, (Uint32 *)vt );
}

void nlDispStripTex( Sint32 num, NLVERTEX03 *vt, Sint32 txtnum )
{
	Sint32 err;

	err = nlstripSetVertexContextTex( strip_blend[4], txtnum );
	if( err == 0 )
		nldispstripTexSub( num, vt );
}

void nlDispStripTexTrnsl( Sint32 num, NLVERTEX03 *vt, Sint32 txtnum )
{
	Sint32 err;

	err = nlstripSetVertexContextTex( strip_blend[5], txtnum );
	if( err == 0 )
		nldispstripTexSub( num, vt );
}

void nlDispStripTexOfs( Sint32 num, NLVERTEX03 *vt, Sint32 txtnum )
{
	Sint32 err;

	err = nlstripSetVertexContextTex( strip_blend[6], txtnum );
	if( err == 0 )
		nldispstripTexOfsSub( num, vt );
}

void nlDispStripTexOfsTrnsl( Sint32 num, NLVERTEX03 *vt, Sint32 txtnum )
{
	Sint32 err;

	err = nlstripSetVertexContextTex( strip_blend[7], txtnum );
	if( err == 0 )
		nldispstripTexOfsSub( num, vt );
}

/* makes the header of a strip of one kind, with the global clip and blend settings */
static void nlstripSetVertexContext( Sint32 *blend )
{
	nl_vertex_context_ptr->state = STATE_STRIP;
	nl_vertex_context_ptr->list_type = *blend++;
	nl_vertex_context_ptr->color_type = *blend++;
	nl_vertex_context_ptr->shading = *blend++;
	nl_vertex_context_ptr->use_alpha = *blend++;
	nl_vertex_context_ptr->specular = *blend;
	kmProcessVertexRenderState( nl_vertex_context_ptr );
	nl_vertex_context_ptr->pcw &= 0xFFFCFF3F;
	nl_vertex_context_ptr->pcw |= nl_or_GP[0];
	nl_vertex_context_ptr->tsp &= 0x031FF0FF;
	nl_vertex_context_ptr->tsp |= ( ( nl_trnsl_blend_mode ^ 0xFC ) << 24 ) | nl_or_GP[1];
	kmSetVertexRenderState( &nl_vertex_buffer, nl_vertex_context_ptr );
}

/* the same with a texture, and the palette bank when the texture has a palette; 1 when the
   texture is not loaded */
static Sint32 nlstripSetVertexContextTex( Sint32 *blend, Sint32 tex )
{
	NLPALETTE *pal;

	if( nl_texture_loadflag[tex].count == 0 )
		return 1;
	tex = nl_texture_loadflag[tex].slot;
	nl_vertex_context_ptr->texture = &nl_texture_param[tex];
	nl_vertex_context_ptr->state = STATE_STRIP;
	nl_vertex_context_ptr->list_type = *blend++;
	nl_vertex_context_ptr->color_type = *blend++;
	nl_vertex_context_ptr->shading = *blend++;
	nl_vertex_context_ptr->use_alpha = *blend++;
	nl_vertex_context_ptr->specular = *blend;
	switch( ( nl_texture_param[tex].tcw >> 27 ) & 7 ){
	case 5:
	case 6:
		nl_vertex_context_ptr->state |= STATE_PALETTE;
		if( nl_palette_direct_mode ){
			nl_vertex_context_ptr->palette_bank = nl_disp_palette;
		} else if( nl_disp_palette < NLSystemWork_.palettes ){
			pal = &NLSystemWork_.palette[nl_disp_palette];
			if( pal->count != 0 )
				nl_vertex_context_ptr->palette_bank = pal->map;
		}
		break;
	}
	kmProcessVertexRenderState( nl_vertex_context_ptr );
	nl_vertex_context_ptr->pcw &= 0xFFFCFF3F;
	nl_vertex_context_ptr->pcw |= nl_or_GP[0];
	nl_vertex_context_ptr->tsp &= 0x031FF0FF;
	nl_vertex_context_ptr->tsp |= ( ( nl_trnsl_blend_mode ^ 0xFC ) << 24 ) | nl_or_GP[1];
	kmSetVertexRenderState( &nl_vertex_buffer, nl_vertex_context_ptr );
	return 0;
}

/* a strip of any layout, read a word at a time: x, y and z, then the colour where fmt says */
static void nldispstripSub( Sint32 num, Sint32 mode, Uint32 *vt )
{
	NLHWVERTEX v;
	const Sint32 *fmt = strip_fmt[mode];

	nlstripPut( nl_vertex_buffer.head, nl_vertex_buffer.slot->size );
	v.cmd = SQ_AREA;
	for( ; num > 0; num-- ){
		if( num == 1 )
			v.cmd = STRIP_END;
		if( nl_disp_coord ){
			nlCalcPoint( (VEC3 *)vt, &v.p );
			v.p.z = 1.0f / v.p.z;
		} else {
			v.p.x = *(Float *)&vt[0];
			v.p.y = *(Float *)&vt[1];
			v.p.z = 1.0f / *(Float *)&vt[2];
		}
		v.col = vt[fmt[1]];
		nlstripPut( &v, sizeof( v ) );
		vt += fmt[0];
	}
}

/* a textured strip */
static void nldispstripTexSub( Sint32 num, NLVERTEX03 *vt )
{
	NLHWVERTEX v;

	nlstripPut( nl_vertex_buffer.head, nl_vertex_buffer.slot->size );
	v.cmd = SQ_AREA;
	for( ; num > 0; num--, vt++ ){
		if( num == 1 )
			v.cmd = STRIP_END;
		if( nl_disp_coord ){
			nlCalcPoint( (VEC3 *)vt, &v.p );
			v.p.z = 1.0f / v.p.z;
		} else {
			v.p.x = vt->x;
			v.p.y = vt->y;
			v.p.z = 1.0f / vt->z;
		}
		v.u = vt->u;
		v.v = vt->v;
		v.col = vt->col;
		nlstripPut( &v, sizeof( v ) );
	}
}

/* a textured strip with an offset colour */
static void nldispstripTexOfsSub( Sint32 num, NLVERTEX03 *vt )
{
	NLHWVERTEX v;

	nlstripPut( nl_vertex_buffer.head, nl_vertex_buffer.slot->size );
	v.cmd = SQ_AREA;
	for( ; num > 0; num--, vt++ ){
		if( num == 1 )
			v.cmd = STRIP_END;
		if( nl_disp_coord ){
			nlCalcPoint( (VEC3 *)vt, &v.p );
			v.p.z = 1.0f / v.p.z;
		} else {
			v.p.x = vt->x;
			v.p.y = vt->y;
			v.p.z = 1.0f / vt->z;
		}
		v.u = vt->u;
		v.v = vt->v;
		v.col = vt->col;
		v.ofs = vt->ofs;
		nlstripPut( &v, sizeof( v ) );
	}
}
