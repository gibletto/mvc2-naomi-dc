/* nlsetgp: the NAOMI library's global parameter settings: tile clipping, volume mode, fog,
   colour clamp, mip map adjust and culling. They are kept in nl_or_GP, which is or-ed into the
   header words of what is drawn next. */
#include "types.h"

/* the bits for a polygon header's parameter control word and its TSP word */
typedef struct NLGP {
	Uint32	pcw;
	Uint32	tsp;
} NLGP;

NLGP nl_or_GP;

void nlSetGP_TileClip( Sint32 mode )
{
	nl_or_GP.pcw &= 0xFFFCFFFF;
	nl_or_GP.pcw |= mode << 16;
}

void nlSetGP_VolumeMode( Sint32 mode )
{
	nl_or_GP.pcw &= 0xFFFFFF3F;
	nl_or_GP.pcw |= mode << 6;
}

void nlSetGP_FogControl( Sint32 mode )
{
	nl_or_GP.tsp &= 0xFF3FFFFF;
	nl_or_GP.tsp |= mode << 22;
}

void nlSetGP_ColorClamp( Sint32 mode )
{
	nl_or_GP.tsp &= 0xFFDFFFFF;
	nl_or_GP.tsp |= mode << 21;
}

void nlSetGP_MipMapAdjust( Sint32 level )
{
	nl_or_GP.tsp &= 0xFFFFF0FF;
	nl_or_GP.tsp |= level << 8;
}

void nlSetGP_CullingReverse( Sint32 mode )
{
	if( mode == 0 )
		nl_or_GP.pcw &= 0x7FFFFFFF;
	else
		nl_or_GP.pcw |= 0x80000000;
}
