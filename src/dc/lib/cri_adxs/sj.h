/* CRI's stream joint (SJ 5.50): a queue of data that one side puts chunks into and the other gets
   them from, the same calls on each kind (in memory, a ring buffer, a chain of chunks). */
#ifndef SJ_H
#define SJ_H

#include "types.h"

#define SJ_ERR_PRM	(-3)	/* a call was given a wrong value */

/* the two lines of a joint */
#define SJ_LIN_FREE	0	/* the room to put data in */
#define SJ_LIN_DATA	1	/* the data to get */

/* a joint's kind, as COM names it */
typedef struct _UUID {
	Uint32	data1 ;
	Uint16	data2 ;
	Uint16	data3 ;
	Uint8	data4[8] ;
} UUID ;

/* a piece of a line: where it starts and how long it is */
typedef struct {
	Sint8	*data ;
	Sint32	len ;
} SJCK ;

typedef struct _sj_vtbl SJ_IF ;

/* what every kind's object starts with */
typedef struct {
	SJ_IF	*vtbl ;
} SJ_OBJ ;

typedef SJ_OBJ *SJ ;

typedef void ( *SJ_ERRFN )( void *obj, Sint32 ecode ) ;

/* each kind's calls */
struct _sj_vtbl {
	void	( *QueryInterface )() ;	/* COM's, unused */
	void	( *AddRef )() ;
	void	( *Release )() ;
	void	( *Destroy )( SJ sj ) ;
	UUID	*( *GetUuid )( SJ sj ) ;
	void	( *Reset )( SJ sj ) ;
	void	( *GetChunk )( SJ sj, Sint32 id, Sint32 nbyte, SJCK *ck ) ;
	void	( *UngetChunk )( SJ sj, Sint32 id, SJCK *ck ) ;
	void	( *PutChunk )( SJ sj, Sint32 id, SJCK *ck ) ;
	Sint32	( *GetNumData )( SJ sj, Sint32 id ) ;
	Sint32	( *IsGetChunk )( SJ sj, Sint32 id, Sint32 nbyte, Sint32 *rbyte ) ;
	void	( *EntryErrFunc )( SJ sj, SJ_ERRFN func, void *obj ) ;
} ;

/* the smaller of two lengths */
#define SJ_MIN( a, b )	( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()

extern void *memset( void *s, int c, unsigned long n ) ;
extern void *memcpy( void *s1, const void *s2, unsigned long n ) ;

#endif
