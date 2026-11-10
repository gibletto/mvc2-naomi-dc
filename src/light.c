/* The light: its direction and ambience, set for the models. */
#include "player.h"

static VEC3 light_dir = { 0.0f, 1.0f, 1.0f } ;

/****************************************************************/
/*			Put the light back to its start						*/
/****************************************************************/
void light_init( void )
{
	memset( &light_work, 0, sizeof( light_work ) ) ;
	light_dir_set( &light_dir ) ;
	light_amb_set( 1.0f ) ;
}

/****************************************************************/
void light_dir_set( VEC3 *dir )
{
	light_work.dir = *dir ;
	nlObjPutSetLightDirectionV( &light_work.dir ) ;
}

/****************************************************************/
void light_amb_set( Float amb )
{
	light_work.amb = amb ;
	nlObjPutSetGlobalAmbience( light_work.amb ) ;
}

/****************************************************************/
/*			Empty in the arcade build							*/
/****************************************************************/
void light_dummy( void )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "light.c" End of File	*/
