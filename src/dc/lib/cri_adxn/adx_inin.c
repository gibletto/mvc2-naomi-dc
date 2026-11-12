/* adx_inin: starts and ends the ADX player (ADXT 5.66): each part of the library in turn, and the
   players' work. */
#include "types.h"
#include "adxt.h"

extern void *memset( void *s, int c, unsigned long n ) ;
extern void ADXF_Init( void ) ;
extern void ADXF_Finish( void ) ;
extern void SJUNI_Init( void ) ;
extern void SJUNI_Finish( void ) ;
extern void SJRBF_Init( void ) ;
extern void SJRBF_Finish( void ) ;
extern void SJMEM_Init( void ) ;
extern void SJMEM_Finish( void ) ;
extern void ADXCRS_Init( void ) ;
extern void ADXRNA_Init( void ) ;
extern void ADXRNA_Finish( void ) ;
extern void ADXSTM_Init( void ) ;
extern void ADXSTM_Finish( void ) ;
extern void ADXSJD_Init( void ) ;
extern void ADXSJD_Finish( void ) ;
extern void ADXERR_Init( void ) ;
extern void ADXERR_Finish( void ) ;
extern void mwRnaEntryErrFunc( void ( *func )( void *obj, char *msg ), void *obj ) ;

char adxt_obj_mark[16] = "MARK:adxt_obj" ;
ADX_TALK adxt_obj[ADXT_MAX_OBJ] = { 0 } ;

/* the sound output's errors go to the library's error routine */
void adxini_rnaerr_cbfn( void *obj, char *msg )
{
	ADXERR_CallErrFunc( "%s", msg ) ;
}

void ADXT_Init( void )
{
	ADXF_Init() ;
	SJUNI_Init() ;
	SJRBF_Init() ;
	SJMEM_Init() ;
	ADXCRS_Init() ;
	ADXRNA_Init() ;
	ADXSTM_Init() ;
	ADXSJD_Init() ;
	ADXERR_Init() ;
	mwRnaEntryErrFunc( adxini_rnaerr_cbfn, NULL ) ;
	memset( adxt_obj, 0, sizeof( adxt_obj ) ) ;
}

void ADXT_Reset( void )
{
	ADXT_Init() ;
}

void ADXT_Finish( void )
{
	ADXERR_Finish() ;
	ADXSJD_Finish() ;
	ADXSTM_Finish() ;
	ADXRNA_Finish() ;
	SJMEM_Finish() ;
	SJRBF_Finish() ;
	SJUNI_Finish() ;
	ADXF_Finish() ;
}
