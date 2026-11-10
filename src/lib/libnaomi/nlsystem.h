/* The system set-up of the NAOMI graphics library: Kamui2's configuration and vertex context, and
   what nlsystem and nlproj share. */
#ifndef LIBNAOMI_NLSYSTEM_H
#define LIBNAOMI_NLSYSTEM_H

#include "types.h"
#include "kamui.h"

/* a render pass of the system configuration */
typedef struct KMPASSINFO {
	Uint32	flags;		/* 00 */
	Uint32	buffer;		/* 04: where its vertex buffer starts */
	Uint32	size;		/* 08: bytes of vertex buffer */
	Uint32	list_type;	/* 0c: the direct list, 0 to 4 */
	Float	ratio[5];	/* 10 */
	Uint32	unk_24[5];
} KMPASSINFO;

/* what Kamui2's kmSetSystemConfiguration takes */
typedef struct KMSYSTEMCONFIGSTRUCT {
	Uint32	dwSize;		/* 00 */
	Uint32	flags;		/* 04 */
	KMSURFACEDESC	**pSurfaceDesc;	/* 08 */
	Uint32	nFrameBuffers;	/* 0c */
	Uint32	unk_10;
	Uint32	texture_size;	/* 14 */
	Uint32	texture_structs;	/* 18: how many textures */
	Uint32	small_vq_structs;	/* 1c: how many of them small VQ ones */
	void	*texture_work;	/* 20: 24 bytes a texture and 76 a small VQ one */
	void	*buffer_desc;	/* 24: the vertex buffer's description, filled in */
	Uint32	vertex_banks;	/* 28 */
	Uint32	base;		/* 2c: where the vertex buffer starts */
	Uint32	vertex_size;	/* 30 */
	Uint32	passes;		/* 34 */
	KMPASSINFO	pass[8];	/* 38 */
} KMSYSTEMCONFIGSTRUCT;

/* a vertex context: the settings of a strip, and the header words kmProcessVertexRenderState
   makes of them */
typedef struct KMSTRIPCONTEXT {
	Uint32	state;		/* 00: which of the settings to process */
	Uint32	unk_04;
	Uint32	list_type;	/* 08: 0 opaque, 2 translucent */
	Uint32	color_type;	/* 0c: 0 packed, 1 floating */
	Uint32	unk_10;
	Uint32	depth_mode;	/* 14 */
	Uint32	unk_18;
	Uint32	unk_1c;
	Uint32	shading;	/* 20: bit 0 gouraud, bit 1 textured */
	Uint32	unk_24;
	Uint32	unk_28;
	Uint32	src_blend;	/* 2c */
	Uint32	dst_blend;	/* 30 */
	Uint32	unk_34;
	Uint32	unk_38;
	Uint32	fog_mode;	/* 3c */
	Uint32	specular;	/* 40 */
	Uint32	use_alpha;	/* 44 */
	Uint32	ignore_tex_alpha;	/* 48 */
	Uint32	unk_4c;
	Uint32	unk_50;
	Uint32	filter;		/* 54 */
	Uint32	unk_58;
	Uint32	mipmap_adjust;	/* 5c: in quarters */
	Uint32	tex_shading;	/* 60 */
	Uint32	unk_64;
	Uint32	palette_bank;	/* 68 */
	KMSURFACEDESC	*texture;	/* 6c */
	Float	face_a, face_r, face_g, face_b;		/* 70 */
	Float	offset_a, offset_r, offset_g, offset_b;	/* 80 */
	Uint32	pcw;		/* 90: the parameter control word */
	Uint32	isp;		/* 94 */
	Uint32	tsp;		/* 98 */
	Uint32	unk_9c;
	Uint32	unk_a0;
	Uint32	unk_a4[7];
} KMSTRIPCONTEXT;

/* the screen: its size and mode, in nl_scr */
typedef struct NLSCREEN {
	Sint16	unk_0, unk_2;
	Sint16	width;		/* 04 */
	Sint16	height;		/* 06 */
	Sint16	interlace;	/* 08 */
	Sint16	vga;		/* 0a: the VGA flag, which doubles a line */
} NLSCREEN;

extern NLSCREEN nl_scr;
/* the library's work: its near and far planes, how they scale, and the model work areas with
   their stack (nl_vertex and nl_lit hold each level's place) */
typedef struct NLSYSWORK {
	Uint8	unk_00[6];
	Sint16	vertex_depth;	/* 06 */
	Uint8	unk_08[20];
	Float	znear;		/* 1c */
	Float	zfar;		/* 20 */
	Float	znear_inv;	/* 24 */
	Float	scale_factor;	/* 28 */
	Float	*vertex_ptr;	/* 2c */
	Float	*vertex_end_ptr;	/* 30 */
	Float	*vertex_work;	/* 34 */
	Sint32	*lit_ptr;	/* 38 */
	Sint32	*lit_end_ptr;	/* 3c */
	Sint32	*lit_work;	/* 40 */
	Sint32	unk_44;		/* 44 */
} NLSYSWORK;

extern NLSYSWORK NLSystemWork_;

#endif
