/* strt2: the Dreamcast start-up's second module (syStart 1.15, the build for the NAOMI library).
   strt1 jumps to _INIT, which marks the stack's area, clears and copies the sections, registers
   the library handles and calls main. The section bounds are strt1's tables. */
#include "types.h"

#define STACK_BGN	( (Uint32 *)0x8C00C000 )
#define STACK_END	( (Uint32 *)0x8C00F400 )
#define STACK_MARK	0x41474553	/* "SEGA" */

typedef void ( *SY_START_FUNC )( void ) ;

extern Uint32 *_B_BGN, *_B_END ;
extern Uint8 *_D_BGN, *_D_END, *_D_ROM ;
extern Uint8 *_BSG_BGN, *_BSG_END ;
extern Uint8 *_DSG_BGN, *_DSG_END, *_DSG_ROM ;
extern SY_START_FUNC *_PRESG_BGN, *_PRESG_END, *_PRE_BGN, *_PRE_END ;
extern SY_START_FUNC *_POSTSG_BGN, *_POSTSG_END, *_POST_BGN, *_POST_END ;
extern void *_DSGLH_BGN ;
extern const char *__0ShinobiVersion ;	/* Shinobi's version text (an array there) */

extern void main( void ) ;
extern void ovMdlEntryTbl( void *handle ) ;
extern void ovMdlDeleteTbl( void *handle ) ;

void _INITLIBH( void ) ;
void _FINISHLIBH( void ) ;
void _INITSCT( void ) ;

const char *sygStartBuild = "\nsyStart Ver 1.15 Build:Nov 06 1999 23:00:30\n" ;

void _INIT( void )
{
	Uint32 *p ;

	*(volatile const char *)__0ShinobiVersion ;	/* keeps the version text in the link */
	for ( p = STACK_BGN ; p < STACK_END ; p++ ) {
		*p = STACK_MARK ;
	}
	_INITSCT() ;
	_INITLIBH() ;
	main() ;
	_FINISHLIBH() ;
}

void _INITLIBH( void )
{
	ovMdlEntryTbl( _DSGLH_BGN ) ;
}

void _FINISHLIBH( void )
{
	ovMdlDeleteTbl( _DSGLH_BGN ) ;
}

/* clears B and BSG and copies D and DSG from their images */
void _INITSCT( void )
{
	Uint32 *b ;
	Uint8 *p, *q ;

	for ( b = _B_BGN ; b < _B_END ; b++ ) {
		*b = 0 ;
	}
	for ( p = _D_BGN, q = _D_ROM ; p < _D_END ; p++, q++ ) {
		*p = *q ;
	}
	for ( p = _BSG_BGN ; p < _BSG_END ; p++ ) {
		*p = 0 ;
	}
	for ( p = _DSG_BGN, q = _DSG_ROM ; p < _DSG_END ; p++, q++ ) {
		*p = *q ;
	}
}

/* calls the libraries' start routines, then the program's: a table entry of 0 or -1 is empty */
void syStartGlobalConstructor( void )
{
	SY_START_FUNC *p ;

	for ( p = _PRESG_BGN ; p < _PRESG_END ; p++ ) {
		if ( *p != NULL && *(Sint32 *)p != -1 ) {
			( *p )() ;
		}
	}
	for ( p = _PRE_BGN ; p < _PRE_END ; p++ ) {
		if ( *p != NULL && *(Sint32 *)p != -1 ) {
			( *p )() ;
		}
	}
}

void syStartGlobalDestructor( void )
{
	SY_START_FUNC *p ;

	for ( p = _POSTSG_BGN ; p < _POSTSG_END ; p++ ) {
		if ( *p != NULL && *(Sint32 *)p != -1 ) {
			( *p )() ;
		}
	}
	for ( p = _POST_BGN ; p < _POST_END ; p++ ) {
		if ( *p != NULL && *(Sint32 *)p != -1 ) {
			( *p )() ;
		}
	}
}
