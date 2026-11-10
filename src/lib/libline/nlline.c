/* libline: the NAOMI library's line drawing calls, in its Oct 1999 version. A line is a quad
   that is as thick as nlLineSetThickness says, cut by the near plane and the screen, and
   sent to the tile accelerator through the store queues. */
#include "nlline.h"

#define sqrt( x )	_builtin_sqrtf( x )

/* the store queue address registers, and where the store queues are */
#define QACR0		( *( Uint32 * )0xFF000038 )
#define QACR1		( *( Uint32 * )0xFF00003C )
#define SQ_AREA		0xE0000000
#define SQ_MASK		0x03FFFFFF
#define STRIP_END	0xF0000000

/* a 4x4 matrix, in columns */
typedef Float NLMATRIX[16];

typedef struct NLMATRIXENTRY {
	Sint32	unk_0, unk_4;
	NLMATRIX	*matrix;	/* 08 */
} NLMATRIXENTRY;

/* a point, and a point with its w after a matrix */
typedef struct NLLINEPOINT {
	Float	x, y, z;
} NLLINEPOINT;

typedef struct NLLINEPOINT4 {
	Float	x, y, z, w;
} NLLINEPOINT4;

/* the library's work: the near plane */
typedef struct NLSYSWORK {
	Uint8	unk_00[28];
	Float	znear;		/* 1c */
} NLSYSWORK;

extern NLSYSWORK NLSystemWork_;
extern NLMATRIXENTRY nlMatrixStack[];
extern Float nl_projectMatrixWidth;
extern Float nl_projectMatrixHeight;
extern Sint32 nl_UserTileClipMode;
extern Uint32 *kmiCurrentVertexBuff;
extern Uint32 nlli_opaque_line[8];
extern Uint32 nlli_transl_line[8];

extern void nlPushMatrix( NLMATRIX *m );
extern void nlPopMatrix( Sint32 n );
extern void nlMultiMatrix8( NLMATRIX *m );
extern void nlMultiPushedMatrix( void );
extern void nlCalcPoint4( const NLLINEPOINT *pos, NLLINEPOINT4 *out );
extern void prefetch( void *p );

/* the settings: the divisions of a line and its thickness */
static Float nlli_div = 64.0f;
static Float nlli_thickness = 1.0f;
static Uint32 nlli_unk_08[4] = { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff };

/* the matrix that takes a point to the screen, on an 8-byte boundary */
#pragma aligndata8( nlli_screen )
static NLMATRIX nlli_screen = {
	777.0f, 0.0f, 0.0f, 0.0f, 0.0f, 777.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

/* the half sizes of the screen and the sides a point is cut at, and the step of a divided line */
static Float nlli_half_w;
static Float nlli_half_h;
static Float nlli_left;
static Float nlli_bottom;
static NLLINEPOINT nlli_step;

const char nl_date_libline_[] = "Oct 28 1999 17:54:23";

/* the routines that draw: a line from one point to another, and a list of them; mode is how
   the entry point was called (view matrix, translucent) */
static Sint32 nlli_single_line( const NLLINEPOINT *from, const NLLINEPOINT *to, Uint32 color, Sint32 mode );
static Sint32 nlli_multi_line( const Uint32 *list, Sint32 mode );

void nlSingleLine( const void *from, const void *to, Uint32 color )
{
	nlli_single_line( from, to, color, NLLINE_OPAQUE );
}

void nlSingleLineUseViewMatrix( const void *from, const void *to, Uint32 color )
{
	nlli_single_line( from, to, color, NLLINE_VIEW );
}

void nlSingleLineTrnsl( const void *from, const void *to, Uint32 color )
{
	nlli_single_line( from, to, color, NLLINE_TRANSL );
}

void nlSingleLineUseViewMatrixTrnsl( const void *from, const void *to, Uint32 color )
{
	nlli_single_line( from, to, color, NLLINE_VIEW_TRANSL );
}

void nlMultiLine( const void *list )
{
	nlli_multi_line( list, NLLINE_OPAQUE );
}

void nlMultiLineUseViewMatrix( const void *list )
{
	nlli_multi_line( list, NLLINE_VIEW );
}

void nlMultiLineTrnsl( const void *list )
{
	nlli_multi_line( list, NLLINE_TRANSL );
}

void nlMultiLineUseViewMatrixTrnsl( const void *list )
{
	nlli_multi_line( list, NLLINE_VIEW_TRANSL );
}

void nlLineSetDiv( float div )
{
	nlli_div = div;
}

void nlLineSetThickness( float thickness )
{
	nlli_thickness = thickness;
}

/* Sets how a translucent line is blended: the source and destination factors. */
void nlLineBlendFunc( Uint32 src, Uint32 dst )
{
	src &= 7;
	dst &= 7;
	nlli_transl_line[2] &= 0x1fffffff;
	nlli_transl_line[2] &= 0xe3ffffff;
	nlli_transl_line[2] |= src << 29;
	nlli_transl_line[2] |= dst << 26;
}

/* Sets the fog of the lines: 1 is none, else it is looked up. */
void nlLineFog( Sint32 mode )
{
	if ( mode == 1 ) {
		nlli_opaque_line[2] &= 0xff3fffff;
		nlli_transl_line[2] &= 0xff3fffff;
		return;
	} else {
		nlli_opaque_line[2] &= 0xff3fffff;
		nlli_transl_line[2] &= 0xff3fffff;
		nlli_opaque_line[2] |= 0x800000;
		nlli_transl_line[2] |= 0x800000;
	}
}

static Uint32 *nlli_start( Sint32 mode );
static Sint32 nlli_clip_near( NLLINEPOINT4 *a, NLLINEPOINT4 *b );
static Sint32 nlli_clip_screen( NLLINEPOINT4 *a, NLLINEPOINT4 *b );
static Uint32 *nlli_line( Uint32 *dst, NLLINEPOINT *p, NLLINEPOINT *q, const Uint32 *color );

/* Draws a line from one point to another. Gives back 0 when none of it is on the screen. */
static Sint32 nlli_single_line( const NLLINEPOINT *from, const NLLINEPOINT *to, Uint32 color, Sint32 mode )
{
	Uint32 *dst;
	Uint32 *slot;
	NLLINEPOINT4 a;
	NLLINEPOINT4 b;
	Float w;

	dst = nlli_start( mode );
	nlPushMatrix( &nlli_screen );
	nlMultiMatrix8( nlMatrixStack[3].matrix );
	if ( mode & 2 )
		nlMultiMatrix8( nlMatrixStack[2].matrix );
	nlMultiPushedMatrix();
	nlCalcPoint4( from, &a );
	nlCalcPoint4( to, &b );
	nlPopMatrix( 1 );
	if ( !nlli_clip_near( &a, &b ) )
		return 0;
	if ( !nlli_clip_screen( &a, &b ) )
		return 0;
	w = 1.0f / a.z;
	a.x = nlli_half_w + w * a.x;
	a.y = nlli_half_h + w * a.y;
	w = 1.0f / b.z;
	b.x = nlli_half_w + w * b.x;
	b.y = nlli_half_h + w * b.y;
	dst = nlli_line( dst, ( NLLINEPOINT * )&a, ( NLLINEPOINT * )&b, &color );
	if ( mode & 1 )
		slot = &kmiCurrentVertexBuff[2];
	else
		slot = &kmiCurrentVertexBuff[0];
	*slot = ( *slot & ~SQ_MASK ) | ( ( Uint32 )dst & SQ_MASK );
	return 1;
}

/* Draws the strips of a list: a strip is its number of segments, its colour and its points. */
static Sint32 nlli_multi_line( const Uint32 *list, Sint32 mode )
{
	Uint32 *dst;
	Uint32 *slot;
	Uint32 color;
	Uint32 count;
	Uint32 i;
	Float w;
	NLLINEPOINT4 a;
	NLLINEPOINT4 b;

	dst = nlli_start( mode );
	nlPushMatrix( &nlli_screen );
	nlMultiMatrix8( nlMatrixStack[3].matrix );
	if ( mode & 2 )
		nlMultiMatrix8( nlMatrixStack[2].matrix );
	nlMultiPushedMatrix();
	while ( *list != 0 ) {
		count = *list++;
		color = *list++;
		for ( i = 0; i < count; i++ ) {
			nlCalcPoint4( ( const NLLINEPOINT * )list, &a );
			nlCalcPoint4( ( const NLLINEPOINT * )list + 1, &b );
			if ( !nlli_clip_near( &a, &b ) )
				goto next;
			if ( !nlli_clip_screen( &a, &b ) )
				goto next;
			w = 1.0f / a.z;
			a.x = nlli_half_w + w * a.x;
			a.y = nlli_half_h + w * a.y;
			w = 1.0f / b.z;
			b.x = nlli_half_w + w * b.x;
			b.y = nlli_half_h + w * b.y;
			dst = nlli_line( dst, ( NLLINEPOINT * )&a, ( NLLINEPOINT * )&b, &color );
		next:
			list += 3;
		}
		list += 3;
	}
	nlPopMatrix( 1 );
	if ( mode & 1 )
		slot = &kmiCurrentVertexBuff[2];
	else
		slot = &kmiCurrentVertexBuff[0];
	*slot = ( *slot & ~SQ_MASK ) | ( ( Uint32 )dst & SQ_MASK );
	return 1;
}

/* Sets up the screen of the lines and the strip header of the kind of line, and gives back
   where in the store queues the quads go. */
static Uint32 *nlli_start( register Sint32 mode )
{
	Uint32 *header;
	Uint32 *vtx;
	Uint32 addr;
	Uint32 *sq;
	Uint32 *out;
	Uint32 *end;

	nlli_half_w = nl_projectMatrixWidth * 0.5f;
	nlli_half_h = nl_projectMatrixHeight * 0.5f;
	nlli_left = -1.0f - nlli_half_w;
	nlli_bottom = -1.0f - nlli_half_h;
	nlli_screen[0] = nlli_half_w;
	nlli_screen[5] = -nlli_half_h;
	vtx = kmiCurrentVertexBuff;
	if ( mode & 1 ) {
		header = nlli_transl_line;
		addr = vtx[2];
	} else {
		header = nlli_opaque_line;
		addr = vtx[0];
	}
	header[0] &= 0xfffcffff;
	header[0] |= ( nl_UserTileClipMode << 16 ) | 0x8c0000;
	QACR1 = QACR0 = addr >> 24;
	sq = ( Uint32 * )( addr & SQ_MASK );
	sq = ( Uint32 * )( ( Uint32 )sq | SQ_AREA );
	out = sq;
	end = out + 8;
	do {
		*out = *header++;
		out++;
	} while ( out < end );
	prefetch( sq );
	sq += 8;
	return sq;
}

/* Cuts the segment a to b where it crosses the near plane, and puts the point in out. */
static void nlli_near_point( const NLLINEPOINT4 *a, const NLLINEPOINT4 *b, NLLINEPOINT4 *out )
{
	Float t;

	t = ( NLSystemWork_.znear - a->z ) / ( b->z - a->z );
	out->z = NLSystemWork_.znear;
	out->x = a->x + t * ( b->x - a->x );
	out->y = a->y + t * ( b->y - a->y );
}

/* Which sides of the screen a point is past: 1 left, 2 right, 4 below and 8 above. */
static Sint32 nlli_outcode( const NLLINEPOINT4 *p )
{
	register Sint32 code;
	Float x, y, z;
	Float right, left, top, bottom;

	code = 0;
	x = p->x;
	y = p->y;
	z = p->z;
	right = nlli_half_w * z;
	top = nlli_half_h * z;
	left = nlli_left * z;
	bottom = nlli_bottom * z;
	if ( x > right )
		code = 2;
	else if ( left > x )
		code = 1;
	if ( y > top )
		code |= 8;
	else if ( bottom > y )
		code |= 4;
	return code;
}

/* Cuts the segment a to b where it crosses the side x = bound * z of the screen, and puts the
   point in out. */
static void nlli_cut_x( const NLLINEPOINT4 *a, const NLLINEPOINT4 *b, NLLINEPOINT4 *out, Float bound )
{
	NLLINEPOINT c;
	Float dx, dy, dz;
	register Float ax, ay, az;

	ax = a->x;
	ay = a->y;
	az = a->z;
	dy = b->y - ay;
	dx = b->x - ax;
	dz = b->z - az;
	if ( dz == 0.0f ) {
		c.z = az;
		c.x = bound * c.z;
		if ( dx == 0.0f )
			c.y = out->y;
		else
			c.y = dy * ( c.x - ax ) / dx + ay;
	} else {
		c.z = ( dz * ax - dx * az ) / ( bound * dz - dx );
		c.x = bound * c.z;
		c.y = dy * ( c.z - az ) / dz + ay;
	}
	out->x = c.x;
	out->y = c.y;
	out->z = c.z;
}

/* The same for the side y = bound * z. */
static void nlli_cut_y( const NLLINEPOINT4 *a, const NLLINEPOINT4 *b, NLLINEPOINT4 *out, Float bound )
{
	NLLINEPOINT c;
	Float dx, dy, dz;
	register Float ax, ay, az;

	ax = a->x;
	ay = a->y;
	az = a->z;
	dy = b->y - ay;
	dx = b->x - ax;
	dz = b->z - az;
	if ( dz == 0.0f ) {
		c.z = az;
		c.y = bound * c.z;
		if ( dy == 0.0f )
			c.x = out->x;
		else
			c.x = dx * ( c.y - ay ) / dy + ax;
	} else {
		c.z = ( dz * ay - dy * az ) / ( bound * dz - dy );
		c.y = bound * c.z;
		c.x = dx * ( c.z - az ) / dz + ax;
	}
	out->x = c.x;
	out->y = c.y;
	out->z = c.z;
}

/* Cuts the segment to the screen: 0 when it is all outside. */
static Sint32 nlli_clip_screen( NLLINEPOINT4 *a, NLLINEPOINT4 *b )
{
	NLLINEPOINT4 *out;
	Sint32 code_a;
	Sint32 code_b;
	Sint32 code;

	for ( ; ; ) {
		code_a = nlli_outcode( a );
		code_b = nlli_outcode( b );
		if ( code_a & code_b )
			return 0;
		if ( code_a == 0 && code_b == 0 )
			break;
		code = code_b;
		if ( code_a )
			code = code_a;
		if ( code_a == 0 )
			out = b;
		else
			out = a;
		if ( code & 1 )
			nlli_cut_x( a, b, out, nlli_left );
		else if ( code & 2 )
			nlli_cut_x( a, b, out, nlli_half_w );
		else if ( code & 4 )
			nlli_cut_y( a, b, out, nlli_bottom );
		else if ( code & 8 )
			nlli_cut_y( a, b, out, nlli_half_h );
		else if ( code == code_a ) {
		}
	}
	return 1;
}

/* Cuts the segment to the near plane: 0 when it is all behind. */
static Sint32 nlli_clip_near( NLLINEPOINT4 *a, NLLINEPOINT4 *b )
{
	if ( NLSystemWork_.znear > a->z ) {
		if ( NLSystemWork_.znear > b->z )
			return 0;
		nlli_near_point( a, b, a );
	} else if ( NLSystemWork_.znear > b->z )
		nlli_near_point( a, b, b );
	return 1;
}

/* Sends a quad of four points (x, y and z words) to the store queues. */
static void nlli_put_quad( Uint32 *dst, const NLLINEPOINT *v, const Uint32 *color )
{
	const Uint32 *point;
	Uint32 c;
	Sint32 i;

	point = ( const Uint32 * )v;
	c = *color;
	for ( i = 0 ; i < 4 ; i++ ) {
		if ( i == 3 )
			*dst++ = STRIP_END;
		else
			*dst++ = SQ_AREA;
		*dst++ = *point++;
		*dst++ = *point++;
		*dst++ = *point++;
		*dst++ = c;
		*dst++ = 0;
		*dst++ = c;
		*dst = 0;
		prefetch( dst );
		dst++;
	}
}

/* The quad of a piece of a line that has an end on both sides: the points p and q are
   widened by the vectors (dx, dy) along the line and across it. */
static void nlli_quad_ends( Uint32 *dst, const NLLINEPOINT *p, const NLLINEPOINT *q, const Uint32 *color, Float dx, Float dy )
{
	NLLINEPOINT v[4];

	v[0].x = p->x + dy + dx;
	v[0].y = p->y - dx + dy;
	v[0].z = p->z;
	v[1].x = p->x - dy + dx;
	v[1].y = p->y + dx + dy;
	v[1].z = p->z;
	v[2].x = q->x + dy - dx;
	v[2].y = q->y - dx - dy;
	v[2].z = q->z;
	v[3].x = q->x - dy - dx;
	v[3].y = q->y + dx - dy;
	v[3].z = q->z;
	nlli_put_quad( dst, v, color );
}

/* The same for the last piece: only the first end is widened. */
static void nlli_quad_last( Uint32 *dst, const NLLINEPOINT *p, const NLLINEPOINT *q, const Uint32 *color, Float dx, Float dy )
{
	NLLINEPOINT v[4];

	v[0].x = p->x + dy + dx;
	v[0].y = p->y - dx + dy;
	v[0].z = p->z;
	v[1].x = p->x - dy + dx;
	v[1].y = p->y + dx + dy;
	v[1].z = p->z;
	v[2].x = q->x + dy;
	v[2].y = q->y - dx;
	v[2].z = q->z;
	v[3].x = q->x - dy;
	v[3].y = q->y + dx;
	v[3].z = q->z;
	nlli_put_quad( dst, v, color );
}

/* The same for a piece in the middle: neither end is widened. */
static void nlli_quad_mid( Uint32 *dst, const NLLINEPOINT *p, const NLLINEPOINT *q, const Uint32 *color, Float dx, Float dy )
{
	NLLINEPOINT v[4];

	v[0].x = p->x + dy;
	v[0].y = p->y - dx;
	v[0].z = p->z;
	v[1].x = p->x - dy;
	v[1].y = p->y + dx;
	v[1].z = p->z;
	v[2].x = q->x + dy;
	v[2].y = q->y - dx;
	v[2].z = q->z;
	v[3].x = q->x - dy;
	v[3].y = q->y + dx;
	v[3].z = q->z;
	nlli_put_quad( dst, v, color );
}

/* The same for the first piece: only the second end is widened. */
static void nlli_quad_first( Uint32 *dst, const NLLINEPOINT *p, const NLLINEPOINT *q, const Uint32 *color, Float dx, Float dy )
{
	NLLINEPOINT v[4];

	v[0].x = p->x + dy;
	v[0].y = p->y - dx;
	v[0].z = p->z;
	v[1].x = p->x - dy;
	v[1].y = p->y + dx;
	v[1].z = p->z;
	v[2].x = q->x + dy - dx;
	v[2].y = q->y - dx - dy;
	v[2].z = q->z;
	v[3].x = q->x - dy - dx;
	v[3].y = q->y + dx - dy;
	v[3].z = q->z;
	nlli_put_quad( dst, v, color );
}

/* The quad of a line that is a point: a square around it. */
static void nlli_quad_point( Uint32 *dst, const NLLINEPOINT *p, const Uint32 *color, Float half )
{
	NLLINEPOINT v[4];

	v[0].x = p->x - half;
	v[0].y = p->y - half;
	v[0].z = p->z;
	v[1].x = p->x + half;
	v[1].y = p->y - half;
	v[1].z = p->z;
	v[2].x = p->x - half;
	v[2].y = p->y + half;
	v[2].z = p->z;
	v[3].x = p->x + half;
	v[3].y = p->y + half;
	v[3].z = p->z;
	nlli_put_quad( dst, v, color );
}

/* Draws the line from p to q, in as many pieces as its length in divisions, and gives back
   where the next quad goes. The z of both points turns into its inverse. */
static Uint32 *nlli_line( Uint32 *dst, NLLINEPOINT *p, NLLINEPOINT *q, const Uint32 *color )
{
	static NLLINEPOINT nlli_p0;
	static NLLINEPOINT nlli_p1;
	Float step;
	Float dx;
	Float dy;
	Float half;
	Float len;
	Float scale;
	Float ex;
	Float ey;
	Sint32 ix;
	Sint32 iy;
	Sint32 n;
	Sint32 i;

	dx = p->x - q->x;
	dy = p->y - q->y;
	len = dx * dx + dy * dy;
	half = nlli_thickness * 0.5f;
	if ( len == 0.0f ) {
		if ( p->z <= q->z )
			nlli_quad_point( dst, p, color, half );
		else
			nlli_quad_point( dst, q, color, half );
		dst += 32;
		return dst;
	} else {
		scale = 1.0f / sqrt( len ) * half;
		ex = dx * scale;
		ey = dy * scale;
	}
	ix = ( Sint32 )( 1.0f / nlli_div * dx );
	iy = ( Sint32 )( 1.0f / nlli_div * dy );
	if ( ix < 0 )
		ix = -ix;
	if ( iy < 0 )
		iy = -iy;
	n = ( ix < iy ) ? ix : iy;
	p->z = 1.0f / p->z;
	q->z = 1.0f / q->z;
	if ( n > 1 ) {
		step = 1.0f / n;
		nlli_step.x = step * dx;
		nlli_step.y = step * dy;
		nlli_step.z = ( p->z - q->z ) * step;
		nlli_p0.x = q->x;
		nlli_p1.x = nlli_p0.x + nlli_step.x;
		nlli_p0.y = q->y;
		nlli_p1.y = nlli_p0.y + nlli_step.y;
		nlli_p0.z = q->z;
		nlli_p1.z = nlli_p0.z + nlli_step.z;
		nlli_quad_first( dst, &nlli_p1, &nlli_p0, color, ex, ey );
		dst += 32;
		for ( i = 1 ; i < n - 1 ; i++ ) {
			nlli_p0.x += nlli_step.x;
			nlli_p1.x += nlli_step.x;
			nlli_p0.y += nlli_step.y;
			nlli_p1.y += nlli_step.y;
			nlli_p0.z += nlli_step.z;
			nlli_p1.z += nlli_step.z;
			nlli_quad_mid( dst, &nlli_p1, &nlli_p0, color, ex, ey );
			dst += 32;
		}
		nlli_p0.x += nlli_step.x;
		nlli_p1.x += nlli_step.x;
		nlli_p0.y += nlli_step.y;
		nlli_p1.y += nlli_step.y;
		nlli_p0.z += nlli_step.z;
		nlli_p1.z += nlli_step.z;
		nlli_quad_last( dst, &nlli_p1, &nlli_p0, color, ex, ey );
		dst += 32;
	} else {
		nlli_quad_ends( dst, p, q, color, ex, ey );
		dst += 32;
	}
	return dst;
}
