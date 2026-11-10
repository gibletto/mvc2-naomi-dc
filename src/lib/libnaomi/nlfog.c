/* nlfog: the fog table and density of the NAOMI graphics library, passed on to Kamui2. */
#include "kamui.h"

void nlFogTableSet( Float *table );

void nlFogTableOff( void )
{
	Float table[128];
	int i;

	for( i = 0; i < 128; i++ )
		table[i] = 0.0f;
	nlFogTableSet( table );
}

void nlFogTableColor( Sint32 argb )
{
	KMPACKEDARGB col;
	Sint8 a = argb >> 24;
	Sint16 r = argb >> 16;
	Sint32 g = argb >> 8;

	col.byte.bAlpha = a;
	col.byte.bRed = r;
	col.byte.bGreen = g;
	col.byte.bBlue = argb;
	kmSetFogTableColor( col );
}

void nlFogDensity( Sint32 density )
{
	kmSetFogDensity( density );
}

void nlFogTableSet( Float *table )
{
	kmSetFogTable( table );
}

/* the density as the hardware takes it: the top eight bits of the mantissa over the exponent */
int nlFogDensityF( Float fdensity )
{
	Sint32 bits = *(Sint32 *)&fdensity;
	Sint32 exp = ( bits >> 23 ) - 127;
	Sint32 mant = (Uint8)( ( (Sint16)( bits >> 16 ) ) | 0x80 );
	Sint32 density;

	if( exp < 0 ){
		mant >>= -exp;
		exp = 0;
	}
	exp &= 0xff;
	density = ( mant << 8 ) | exp;
	kmSetFogDensity( density );
	return density;
}
