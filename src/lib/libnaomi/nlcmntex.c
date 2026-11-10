/* nlcmntex: the texture slots of the NAOMI graphics library, and the common textures kept in them. */
#include "nl_tex.h"
#include "nlreg.h"

extern Sint32 nl_multi_pass ;
extern Sint32 nl_in_scene ;

Sint32 nl_texture_flagmax = 0;
Sint32 nl_texture_bitmax = 0;
Uint32 *(*nl_tex_load_user_callback)( NLTEXHEAD *thd ) = 0;

NLTEXLOAD *nl_texture_loadflag;
Uint32 *nl_texture_loadbit;
static NLTEXPARAM dummy_param;

void nlInitTextureSurface( NLTEXPARAM *param, Sint32 max );
void nlFreeTextureAll( void );
void nlCreateTextureParamA( NLTEXPARAM *param );

/* makes the surface of slot index and loads the texture into it: 0 when it is loaded */
Sint32 nlLoadTextureMem( NLTEXHEAD *thd, Sint32 index, Uint32 *data )
{
	NLTEXPARAM *param = &nl_texture_param[index];
	Uint32 *user;
	Sint32 ret;

	nl_txt_count++;
	ret = kmCreateTextureSurface( &param->desc, thd->width, thd->height, thd->type );
	param->status = ret;
	if( ret == 0 ){
		if( nl_tex_load_user_callback ){
			user = nl_tex_load_user_callback( thd );
			if( user )
				data = user;
		}
		if( nl_multi_pass == 0 || nl_in_scene == 1 ){
			kmyEnableASyncCopy( 1 );
			kmLoadTextureRom( &nl_texture_param[index].desc, data );
			kmyEnableASyncCopy( 0 );
			while( kmQueryFinishLastTextureDMA() )
				;
		}else
			kmLoadTextureRom( &nl_texture_param[index].desc, data );
		return 0;
	}
	return 1;
}

/* shares the work out: the slots, then each common texture's place (from texture -1) and the bits
   of the slots in use */
void nlInitCommonTexture( Sint32 *work, Sint32 nbLoadTex, Sint32 nbMaxTex )
{
	Sint32 i;
	Uint8 *flag;

	nbLoadTex++;
	nbMaxTex++;
	nlInitTextureSurface( (NLTEXPARAM *)work, nbLoadTex );
	flag = (Uint8 *)work + nbLoadTex * sizeof( NLTEXPARAM ) + 8;
	nl_texture_loadflag = (NLTEXLOAD *)flag;
	nl_texture_loadbit = (Uint32 *)( flag + nbMaxTex * 8 );
	nl_texture_bitmax = ( nbMaxTex + 31 ) / 32;
	nl_texture_flagmax = nbMaxTex - 1;
	for( i = -1; i < nl_texture_flagmax; i++ ){
		nl_texture_loadflag[i].loaded = 0;
		nl_texture_loadflag[i].slot = -1;
	}
	for( i = 0; i < nl_texture_bitmax; i++ )
		nl_texture_loadbit[i] = 0;
	nl_txt_ofs = 0;
}

void nlFreeCommonTextureAll( void )
{
	Sint32 i;

	nlFreeTextureAll();
	if( nl_texture_flagmax ){
		for( i = -1; i < nl_texture_flagmax; i++ ){
			nl_texture_loadflag[i].loaded = 0;
			nl_texture_loadflag[i].slot = -1;
		}
		for( i = 0; i < nl_texture_bitmax; i++ )
			nl_texture_loadbit[i] = 0;
	}
}

void nlSetCommonTextureOffset( Sint32 ofs )
{
	nl_txt_ofs = ofs;
}

/* loads new texels into a common texture that is loaded: 1 when it is done */
Sint32 nlCommonTextureReloadA( Sint32 num, Uint32 *data )
{
	NLTEXPARAM *param;
	Sint32 ret, slot;

	if( num >= 0 )
		num += nl_txt_ofs;
	if( nl_texture_loadflag[num].loaded == 0 )
		return 0;
	slot = nl_texture_loadflag[num].slot;
	param = &nl_texture_param[slot];
	if( param->status != 0 )
		return 0;
	ret = kmLoadTextureRom( &param->desc, data );
	if( ret != 0 )
		return 0;
	return 1;
}

Sint32 nlCommonTextureReloadI( Sint32 num, NLTEXHEAD *thd, Sint32 index )
{
	return nlCommonTextureReloadA( num, thd[index].address );
}

/* copies a common texture's texels out of video memory: 1 when it is done */
Sint32 nlCommonTextureGetRam( Sint32 num, Uint32 *dst )
{
	Uint32 *src, *d;
	Sint32 size, i, slot;

	if( num >= 0 )
		num += nl_txt_ofs;
	if( nl_texture_loadflag[num].loaded == 0 )
		return 0;
	slot = nl_texture_loadflag[num].slot;
	if( nl_texture_param[slot].status != 0 )
		return 0;
	d = dst;
	src = (Uint32 *)( (Uint32)nl_texture_param[slot].desc.pSurface + VRAM_64BIT );
	size = nl_texture_param[slot].desc.VSize * nl_texture_param[slot].desc.USize;
	for( i = 0; i < size; i += 2 )
		*d++ = *src++;
	return 1;
}

Uint32 nlCommonTextureGetFreeMem( void )
{
	Uint32 total, largest;

	kmGetFreeTextureMem( &total, &largest );
	return total;
}

Uint32 nlCommonTextureGetFreeBlock( void )
{
	Uint32 largest, total;

	kmGetFreeTextureMem( &total, &largest );
	return largest;
}

/* the first free texture index, marked as used */
Sint32 nlCommonTextureGetNextTextureIndex( void )
{
	Sint32 i, j;
	Uint32 word, bit;

	for( i = 0; i < nl_texture_bitmax; i++ ){
		word = nl_texture_loadbit[i];
		if( word != -1 ){
			j = 0;
			bit = 1;
			while( 1 ){
				if( !( word & bit ) ){
					nl_texture_loadbit[i] |= bit;
					return i * 32 + j;
				}
				bit <<= 1;
				j++;
			}
		}
	}
	return -1;
}

void nlCommonTextureClearTextureIndex( Sint32 index )
{
	Sint32 bit;

	if( index >= 0 ){
		bit = 1 << ( index & 31 );
		index >>= 5;
		nl_texture_loadbit[index] &= ~bit;
	}
}

/* puts a common texture in a polygon header */
void nlSetCommonTextureParameter( NLTEXCONTEXT *context, Sint32 num )
{
	Sint32 slot = nl_texture_loadflag[num].slot;
	Uint32 tsp = context->tsp, tex;
	NLTEXPARAM *param;

	tsp &= ~0x3f;
	tex = 0;
	param = &nl_texture_param[slot];
	tsp |= param->tsp_size;
	tex |= param->tsp_addr;
	context->tsp = tsp;
	context->tex = tex;
}

void nlCommonTextureGarbageCollection( void )
{
	Sint32 i;
	NLTEXPARAM *param;

	kmGarbageCollectTexture();
	param = nl_texture_param;
	for( i = 0; i < nl_texture_max; i++, param++ ){
		if( param->status == 0 )
			nlCreateTextureParamA( param );
	}
}

void nlCommonTextureSetLoadCallBack( Uint32 *(*callback)( NLTEXHEAD *thd ) )
{
	nl_tex_load_user_callback = callback;
}

/* a side of 8 to 1024 texels as the hardware counts it, 0 to 7 */
static Sint32 tex_size_code( Sint32 size )
{
	if( size == 8 )
		size = 0;
	else if( size == 16 )
		size = 1;
	else if( size == 32 )
		size = 2;
	else if( size == 64 )
		size = 3;
	else if( size == 128 )
		size = 4;
	else if( size == 256 )
		size = 5;
	else if( size == 512 )
		size = 6;
	else if( size == 1024 )
		size = 7;
	return size;
}

/* puts a slot's texture in a polygon header */
void nlSetTextureParameter( NLTEXCONTEXT *context, Sint32 num )
{
	Uint32 tex = 0, tsp = context->tsp;
	NLTEXPARAM *param;

	tsp &= ~0x3f;
	param = &nl_texture_param[num];
	tsp |= param->tsp_size;
	tex |= param->tsp_addr;
	context->tsp = tsp;
	context->tex = tex;
}

void nlInitTextureSurface( NLTEXPARAM *param, Sint32 max )
{
	Sint32 i;

	nl_texture_param = param;
	nl_texture_max = max;
	nl_txt_count = 0;
	nl_txt_ofs = 0;
	for( i = 0; i < max; i++ ){
		param->status = -1;
		param->used = 0;
		param->index = -1;
		param++;
	}
}

/* makes the header words of a slot from its surface */
void nlCreateTextureParamA( NLTEXPARAM *param )
{
	KMSURFACEDESC *desc = &param->desc;
	Sint32 usize = tex_size_code( desc->USize ) << 3;
	Uint32 tsp, tex;

	tsp = ( usize & ~7 ) | tex_size_code( desc->VSize );
	param->tsp_size = tsp;
	tex = ( ( desc->fSurfaceFlags | desc->PixelFormat ) & 0xFF800000 ) | ( (Uint32)desc->pSurface >> 3 );
	param->tsp_addr = tex;
}

/* the same by number; a slot with no surface takes the dummy texture's words */
void nlCreateTextureParam( Sint32 num )
{
	NLTEXPARAM *param = &nl_texture_param[num];

	if( param->status == 0 )
		nlCreateTextureParamA( param );
	else{
		param->tsp_size = dummy_param.tsp_size;
		param->tsp_addr = dummy_param.tsp_addr;
	}
}

void nlFreeTextureAll( void )
{
	NLTEXPARAM *param;
	Sint32 i;

	param = nl_texture_param;
	for( i = 0; i < nl_texture_max; i++ ){
		if( param->used && !param->status )
			kmFreeTexture( &param->desc );
		param++;
	}
	param = nl_texture_param;
	for( i = 0; i < nl_texture_max; i++ ){
		param->status = -1;
		param->used = 0;
		param->index = -1;
		param++;
	}
	nl_txt_ofs = 0;
	nl_txt_count = 0;
}

/* eight by eight white texels */
static const Uint16 dummy_texel[64] = {
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff,
	0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff, 0xffff
};

/* the dummy texture, laid out as a texture's header */
typedef struct DUMMY_TEXTURE {
	Sint16	width, height;
	Sint32	type;
	const Uint16 *texel;
	Sint32	end;
} DUMMY_TEXTURE;

static const DUMMY_TEXTURE dummy_tex = { 8, 8, 0x101, dummy_texel, -1 };

void nlLoadDummyTexture( void )
{
	kmCreateTextureSurface( &dummy_param.desc, dummy_tex.width, dummy_tex.height, dummy_tex.type );
	kmLoadTexture( &dummy_param.desc, (Uint32 *)dummy_tex.texel );
	nlCreateTextureParamA( &dummy_param );
}

/* puts the dummy texture in a polygon header */
void nlSetDummyTextureParameter( NLTEXCONTEXT *context )
{
	Uint32 tsp = context->tsp, tex = context->tex;

	tsp &= ~0x3f;
	tex &= 0xF8000000;
	tsp |= dummy_param.tsp_size;
	tex |= dummy_param.tsp_addr;
	context->tsp = tsp;
	context->tex = tex;
}
