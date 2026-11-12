/* adx_tlk2: starts an ADX player on a file of an AFS archive, a named file, or data in memory
   (ADXT 5.66). */
#include "types.h"
#include "adxt.h"

/* a word of the data in memory, which is big-endian */
#define BSWAP32( x )	( ( ( (x) >> 24 ) & 0xFF ) | ( ( (x) >> 8 ) & 0xFF00 ) | ( ( (x) << 8 ) & 0xFF0000 ) | ( (x) << 24 ) )

extern void ADXF_GetFileRange( Sint32 patno, Sint32 fno, Sint32 *ofst, Sint32 *size ) ;
extern void *ADXSTM_OpenRange( Sint32 ofst, Sint32 size, void *sj ) ;
extern void *ADXSTM_OpenFname( char *fname, void *sj ) ;
extern void *SJMEM_Create( void *data, Sint32 size ) ;
extern void adxt_start_stm( ADXT adxt ) ;
extern void adxt_start_sj( ADXT adxt, void *sj ) ;

void ADXT_StartMem2( ADXT adxt, void *adxdat, Sint32 datlen ) ;

const char *adxt_build = "\nADXT Ver.5.66 Build:Aug 26 1999 12:36:27\n" ;

/* plays file fno of the archive that patno names */
void ADXT_StartAfs( ADXT adxt, Sint32 patno, Sint32 fno )
{
	Sint32 ofst, size ;

	ADXT_Stop( adxt ) ;
	ADXCRS_Lock() ;
	ADXF_GetFileRange( patno, fno, &ofst, &size ) ;
	if ( ( adxt->stm = ADXSTM_OpenRange( ofst, size, adxt->sjf ) ) == NULL ) {
		ADXCRS_Unlock() ;
		ADXERR_CallErrFunc( "E8101202: can't open %d, %d (ADXT_StartAfs)\n", patno, fno ) ;
	} else {
		adxt_start_stm( adxt ) ;
	}
	ADXCRS_Unlock() ;
}

void ADXT_StartFname( ADXT adxt, char *fname )
{
	ADXT_Stop( adxt ) ;
	ADXCRS_Lock() ;
	if ( ( adxt->stm = ADXSTM_OpenFname( fname, adxt->sjf ) ) == NULL ) {
		ADXCRS_Unlock() ;
		ADXERR_CallErrFunc( "E8101205: can't open '%s' (ADXT_StartFname)", fname ) ;
	} else {
		adxt_start_stm( adxt ) ;
	}
	ADXCRS_Unlock() ;
}

void ADXT_StartMem( ADXT adxt, void *adxdat )
{
	ADXT_StartMem2( adxt, adxdat, ADXT_MAX_DATASIZE ) ;
}

void ADXT_StartMem2( ADXT adxt, void *adxdat, Sint32 datlen )
{
	void *sj ;

	ADXT_Stop( adxt ) ;
	ADXCRS_Lock() ;
	sj = SJMEM_Create( adxdat, datlen ) ;
	if ( sj == NULL ) {
		ADXCRS_Unlock() ;
		ADXERR_CallErrFunc( "E8101207: can't create sj (ADXT_StartMem)\n" ) ;
	} else {
		adxt_start_sj( adxt, sj ) ;
		adxt->pmode = ADXT_PLY_MEMIDX ;
	}
	ADXCRS_Unlock() ;
}

/* plays entry no of an ACX file in memory: its head is a count, then each entry's offset and size */
void ADXT_StartMemIdx( ADXT adxt, void *acx, Sint32 no )
{
	char *dat, *adr ;
	Sint32 *idx ;
	Sint32 ofst ;
	void *sj ;

	dat = acx ;
	idx = acx ;
	ADXT_Stop( adxt ) ;
	if ( no >= BSWAP32( idx[1] ) || no < 0 ) {
		return ;
	}
	ADXCRS_Lock() ;
	ofst = idx[( no + 1 ) * 2] ;
	ofst = BSWAP32( ofst ) ;
	adr = dat + ofst ;
	sj = SJMEM_Create( adr, ADXT_MAX_DATASIZE ) ;
	if ( sj == NULL ) {
		ADXCRS_Unlock() ;
		ADXERR_CallErrFunc( "E8101207: can't create sj (ADXT_StartMemIdx)" ) ;
	} else {
		adxt_start_sj( adxt, sj ) ;
		adxt->pmode = ADXT_PLY_MEMIDX ;
	}
	ADXCRS_Unlock() ;
}
