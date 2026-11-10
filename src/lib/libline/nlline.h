/* libline's declarations: the line drawing calls of the NAOMI library, in its Oct 1999 version. */
#ifndef NLLINE_H
#define NLLINE_H

#include "types.h"

/* the modes of a line: what the entry points pass to the routines they share */
#define NLLINE_OPAQUE		0
#define NLLINE_TRANSL		1
#define NLLINE_VIEW		2
#define NLLINE_VIEW_TRANSL	3

extern void nlSingleLine( const void *from, const void *to, Uint32 color );
extern void nlSingleLineUseViewMatrix( const void *from, const void *to, Uint32 color );
extern void nlSingleLineTrnsl( const void *from, const void *to, Uint32 color );
extern void nlSingleLineUseViewMatrixTrnsl( const void *from, const void *to, Uint32 color );
extern void nlMultiLine( const void *list );
extern void nlMultiLineUseViewMatrix( const void *list );
extern void nlMultiLineTrnsl( const void *list );
extern void nlMultiLineUseViewMatrixTrnsl( const void *list );
extern void nlLineSetDiv( float div );
extern void nlLineSetThickness( float thickness );
extern void nlLineBlendFunc( Uint32 src, Uint32 dst );
extern void nlLineFog( Sint32 mode );

#endif
