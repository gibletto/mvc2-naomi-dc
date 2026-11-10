/* The fog: a table that fades the far polygons into a colour, made each frame from fog_work and
   sent in the vertical blank. */
#include "player.h"

/****************************************************************/
/*			Clear the fog										*/
/****************************************************************/
void fog_init()
{
	memset( &fog_work, 0, sizeof( fog_work ) ) ;
}

/****************************************************************/
/*			The fog table, its density and its colour,			*/
/*			darkened with the screen							*/
/****************************************************************/
void fog_set( void )
{
	Sint32 i ;
	Uint8 r, g, b ;

	i = 0 ;
	while( i < fog_work.len ){
		fog_work.table[i] = 1.0f - ( Float )i / fog_work.len ;
		i ++ ;
	}
	for( ; i < FOG_TABLE ; i ++ ) fog_work.table[i] = 0.0f ;
	nlFogDensityF( fog_work.density ) ;
	r = bright_work.rate[1] * fog_work.r ;
	g = bright_work.rate[2] * fog_work.g ;
	b = bright_work.rate[3] * fog_work.b ;
	nlFogTableColor( r << 16 | g << 8 | b ) ;
}

/****************************************************************/
/*			Send the table, or turn the fog off					*/
/****************************************************************/
void fog_disp( void )
{
	if( fog_work.on ) nlFogTableSet( fog_work.table ) ;
	else nlFogTableOff() ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "fog.c" End of File	*/
