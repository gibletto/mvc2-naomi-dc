/* libnlobj: loading, freeing and re-pointing the textures and palettes of a model's groups. */
#include "nlobj.h"
#include "nlobjtex.h"

/* a group's header words: 16-bit texture coordinates (which a group without a texture is given),
   the user clip bits, and the settings every group gets */
#define PCW_UV16	0x00000001
#define PCW_USER_CLIP	0x00030000
#define PCW_GROUP	0x008C0000
#define ISP_UV16	0x00400000

/* the texture control word: the pixel format, of which two have a palette, and the palette */
#define TEX_FORMAT( tex )	( ( ( tex ) >> 27 ) & 7 )
#define TEX_PALETTE4	5
#define TEX_PALETTE8	6
#define TEX_PALETTE_MASK	0x07E00000
#define TEX_PALETTE_SHIFT	21

NLPALETTE_SRC *NLobjput_palList;

void nlObjPutInitTexture( void )
{
	nl_txt_ofs = 0;
	NLobjput_palList = 0;
}

/* loads the textures of a model, from group cont (or the first), until it has loaded a new one;
   *next is where to go on from, or 0 when it has reached the end */
int nlObjPutLoadTextureOnce_Sub( NLOBJ *obj, NLthd *thd, NLMESH *cont, NLMESH **next, int gi )
{
	NLMESH *mesh;
	Uint8 *after;
	Sint32 texno, tex, slot, pal, fmt, size;
	NLTEXPARAM *param;
	NLthd *head;
	NLPALETTE_SRC *src;
	NLPALETTE *dst;
	Uint32 ctrl;
	int loaded = 0;
	int ret = 0;

	*next = 0;
	if( cont == 0 ){
		if( obj->flag < 0 || obj->mesh.pcw == 0 ){
			return 0;
		}
		after = ( Uint8 * )&obj->mesh;
	}else{
		after = ( Uint8 * )cont;
	}
again:
	mesh = ( NLMESH * )after;
	texno = mesh->texno;
	tex = texno;
	after = after + 80;	/* past its header */
	if( tex >= 0 ){
		if( gi == 1 ){
			tex = thd[tex].globl_index;
		}
		tex += nl_txt_ofs;
		if( nl_texture_loadflag[tex].count == 0 ){
			nl_texture_loadflag[tex].index = nlCommonTextureGetNextTextureIndex();
		}
		nl_texture_loadflag[tex].count++;
		tex = nl_texture_loadflag[tex].index;
		param = &nl_texture_param[tex];
		if( param->use == 0 ){
			head = &thd[texno];
			ret |= nlLoadTextureMem( head, tex, head->address );
			nlCreateTextureParam( tex );
			param->index = tex;
			loaded = 1;
		}
		param->use = param->use + 1;
	}
	if( texno < 0 ){
		mesh->pcw |= PCW_UV16;
		mesh->isp |= ISP_UV16;
	}
	mesh->pcw = ( mesh->pcw & ~PCW_USER_CLIP ) | ( ( nl_UserTileClipMode << 16 ) | PCW_GROUP );
	if( tex >= 0 ){
		nlSetTextureParameter( mesh, tex );
	}else{
		nlSetDummyTextureParameter( mesh );
	}
	if( nl_palette_direct_mode == 0 ){
		fmt = TEX_FORMAT( mesh->tex );
		if( NLobjput_palList != 0 && ( fmt == TEX_PALETTE4 || fmt == TEX_PALETTE8 ) ){
			pal = mesh->palno;
			if( pal < NLSystemWork_.palettes ){
				dst = NLSystemWork_.palette;
				src = &NLobjput_palList[pal];
				dst += pal;
				if( dst->use != 0 ){
					dst->use++;
					slot = dst->slot;
				}else{
					dst->type = src->type;
					dst->size = ( src->size + 15 ) >> 4;
					dst->use = 1;
					dst->palno = pal;
					size = src->size;
					slot = nlSearchPalette( src->type, size );
					if( slot >= 0 ){
						dst->slot = slot;
						nlSetPaletteMap( slot, size );
						nlSetPaletteData( slot * 16, src->data, size );
					}else{
						slot = 0;
					}
				}
				mesh->tex = ( mesh->tex & ~TEX_PALETTE_MASK ) | ( slot << TEX_PALETTE_SHIFT );
			}
		}
	}else{
		fmt = TEX_FORMAT( mesh->tex );
		if( fmt == TEX_PALETTE4 || fmt == TEX_PALETTE8 ){
			pal = mesh->palno;
			mesh->tex = ( mesh->tex & ~TEX_PALETTE_MASK ) | ( pal << TEX_PALETTE_SHIFT );
		}
	}
	nlObjPutLoadTextureCallBack( mesh );
	after += mesh->len;
	ctrl = ( ( NLMESH * )after )->pcw;
	if( ctrl == 0 ){
		*next = 0;
		return ret;
	}else if( !loaded ){
		goto again;
	}
	*next = ( NLMESH * )after;
	return ret;
}

/* the entry points: one step or all of a model, with the model's own texture numbers or (GI)
   the global ones its texture headers give */
int nlObjPutLoadTextureOnce( NLOBJ *obj, NLthd *thd, NLMESH *cont, NLMESH **next )
{
	return nlObjPutLoadTextureOnce_Sub( obj, thd, cont, next, 0 );
}

int nlObjPutLoadTexture( NLOBJ *obj, NLthd *thd )
{
	NLMESH *cont = 0;
	int ret = 0;

	do{
		ret |= nlObjPutLoadTextureOnce_Sub( obj, thd, cont, &cont, 0 );
	}while( cont != 0 );
	return ret;
}

int nlObjPutLoadTextureOnceGI( NLOBJ *obj, NLthd *thd, NLMESH *cont, NLMESH **next )
{
	return nlObjPutLoadTextureOnce_Sub( obj, thd, cont, next, 1 );
}

int nlObjPutLoadTextureGI( NLOBJ *obj, NLthd *thd )
{
	NLMESH *cont = 0;
	int ret = 0;

	do{
		ret |= nlObjPutLoadTextureOnce_Sub( obj, thd, cont, &cont, 1 );
	}while( cont != 0 );
	return ret;
}

/* points each group at its texture again, or at the dummy texture when its own is gone, and
   says if any was gone */
int nlObjPutSetGarbagedTexture_Sub( NLOBJ *obj, NLthd *thd, int gi )
{
	NLMESH *mesh;
	Uint8 *after;
	Sint32 tex;
	Uint32 keep, ctrl;
	int garbaged = 0;

	if( obj->flag < 0 || obj->mesh.pcw == 0 ){
		return 0;
	}
	after = ( Uint8 * )&obj->mesh;
	if( 0 ) garbaged = ( int )obj;
	if( 0 ) garbaged = ( int )obj + ( int )thd;
	while( 1 ){
		mesh = ( NLMESH * )after;
		tex = mesh->texno;
		after = mesh->vtx;
		if( tex >= 0 ){
			if( gi && thd != 0 ){
				tex = thd[tex].globl_index;
			}
			tex += nl_txt_ofs;
			if( nl_texture_loadflag[tex].count == 0 ){
				garbaged = 1;
				tex = -1;
			}else{
				tex = nl_texture_loadflag[tex].index;
			}
		}
		keep = mesh->tex & TEX_PALETTE_MASK;
		if( 0 ) ctrl = 0;
		if( tex >= 0 ){
			nlSetTextureParameter( mesh, tex );
		}else{
			nlSetDummyTextureParameter( mesh );
		}
		mesh->tex |= keep;
		after += mesh->len;
		ctrl = ( ( NLMESH * )after )->pcw;
		if( ctrl == 0 ) break;
	}
	return garbaged;
}

int nlObjPutSetGarbagedTexture( NLOBJ *obj )
{
	return nlObjPutSetGarbagedTexture_Sub( obj, 0, 0 );
}

int nlObjPutSetGarbagedTextureGI( NLOBJ *obj, NLthd *thd )
{
	return nlObjPutSetGarbagedTexture_Sub( obj, thd, 1 );
}

/* gives back the textures and palettes of a model, and says if a texture could not be freed */
int nlObjPutFreeTextureOnce_Sub( NLOBJ *obj, NLthd *thd, int gi )
{
	NLMESH *mesh;
	Uint8 *after;
	int has_thd;
	Uint32 ctrl;
	Sint32 tex, err, fmt, idx;
	int ret = 0;

	if( obj->flag < 0 || obj->mesh.pcw == 0 ){
		return 0;
	}
	if( 1 ) has_thd = ( thd != 0 ); else has_thd = nl_texture_param[0].use;
	if( 1 ) after = ( Uint8 * )&obj->mesh; else after = ( Uint8 * )obj;
	do{
		mesh = ( NLMESH * )after;
		tex = mesh->texno;
		after = mesh->vtx;
		if( tex >= 0 ){
			if( gi && has_thd ){
				tex = thd[tex].globl_index;
			}
			tex = nl_txt_ofs + tex;
			if( nl_texture_loadflag[tex].count > 0 ){
				nl_texture_loadflag[tex].count--;
				if( nl_texture_loadflag[tex].count == 0 ){
					idx = nl_texture_loadflag[tex].index;
					if( nl_texture_param[idx].status == 0 ){
						err = kmFreeTexture( &nl_texture_param[idx] );
						if( err ){
							ret = 1;
						}
					}
					nl_texture_param[idx].use = 0;
					nl_texture_param[idx].index = -1;
					if( 1 ) nlCommonTextureClearTextureIndex( idx ); else has_thd = idx;
					nl_texture_loadflag[tex].index = -1;
				}
			}
		}
		if( nl_palette_direct_mode == 0 ){
			fmt = TEX_FORMAT( mesh->tex );
			if( NLobjput_palList != 0 && ( fmt == TEX_PALETTE4 || fmt == TEX_PALETTE8 ) ){
				nlFreePalette( mesh->palno );
			}
		}
		after += mesh->len;
		ctrl = ( ( NLMESH * )after )->pcw;
	}while( ctrl != 0 );
	return ret;
}

int nlObjPutFreeTextureOnce( NLOBJ *obj )
{
	return nlObjPutFreeTextureOnce_Sub( obj, 0, 0 );
}

int nlObjPutFreeTextureOnceGI( NLOBJ *obj, NLthd *thd )
{
	return nlObjPutFreeTextureOnce_Sub( obj, thd, 1 );
}

/* sets the list of palettes the models' groups number into */
void nlObjPutSetPaletteHeaderAddress( NLPALETTE_SRC *list )
{
	NLobjput_palList = list;
}

/* in direct palette mode, moves the palette numbers of a model's groups by ofs */
void nlObjPutSetPaletteOffset( NLOBJ *obj, Sint32 ofs )
{
	NLMESH *mesh;
	Uint8 *p;
	Sint32 fmt;
	Uint32 ctrl;

	if( nl_palette_direct_mode == 1 && obj->flag >= 0 && obj->mesh.pcw != 0 ){
		p = ( Uint8 * )&obj->mesh;
		do{
			mesh = ( NLMESH * )p;
			p = mesh->vtx;
			fmt = TEX_FORMAT( mesh->tex );
			if( fmt == TEX_PALETTE4 || fmt == TEX_PALETTE8 ){
				fmt = mesh->palno + ofs;
				mesh->tex = ( mesh->tex & ~TEX_PALETTE_MASK ) | ( fmt << TEX_PALETTE_SHIFT );
			}
			p += mesh->len;
			ctrl = ( ( NLMESH * )p )->pcw;
		}while( ctrl != 0 );
	}
}
