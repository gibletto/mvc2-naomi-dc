/* libnlobj's own declarations: the model format as the texture and vertex code reads it, and
   the library state the texture code shares with the rest of the NAOMI library. */
#ifndef NLOBJ_H
#define NLOBJ_H

#include "types.h"

/* a texture's header */
typedef struct NLthd {
	Sint16	width, height;
	Sint32	type;
	void	*address;
	Sint32	globl_index;
} NLthd;

/* one polygon group of a model: this header, then len bytes of vertices */
typedef struct NLMESH {
	Uint32	pcw;		/* 00: parameter control word */
	Uint32	isp;		/* 04 */
	Uint32	tsp;		/* 08 */
	Uint32	tex;		/* 0c: texture control word */
	Sint32	unk_10[4];
	Sint32	texno;		/* 20: its texture, or -1 */
	Sint32	unk_24[6];
	Sint32	palno;		/* 3c: its palette */
	Sint32	unk_40[3];
	Sint32	len;		/* 4c: bytes of vertices after this header */
	Uint8	vtx[1];		/* 50 */
} NLMESH;

/* a model: its flag word, then its groups, the last followed by a zero word */
typedef struct NLOBJ {
	Sint32	flag;
	Sint32	unk_4[5];
	NLMESH	mesh;
} NLOBJ;

#endif
