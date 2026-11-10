/* syIntCtrl_: Shinobi's interrupt set-up: the interrupt mask call, the start of the interrupt handlers
   and the move of the interrupt stack. */
#include "types.h"
#include "syreg.h"

#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()
#define set_cr( cr )	_builtin_set_cr( cr )
#define get_cr()	_builtin_get_cr()
#define get_vbr()	_builtin_get_vbr()

#define SYD_INT_MODE_INTSTACK	0x80000000
#define REG_CCR		( *(Uint32 *)CCN_CCR )
#define SR_BL		0x10000000
#define VBR_STACK_OFFSET	0x0000019C
#define STACK_FRAME	0x00000100

typedef struct {
	Sint8	major;
	Sint8	minor;
	Sint8	debug;
	Sint8	rsv;
} SY_VERSION;

const char * const sygIntBuild = "\nsyInt Ver 1.50am Build:" "Nov 09 1999" " " "18:17:11" "\n";
const SY_VERSION sygIntVer = { 1, 5, 0, 0 };

static Uint32 intStackPtr = 0;
static Uint32 intStackTop = 0;
static Uint32 intStackSpare = 0;
static Uint32 intStackBase = 0;

static void intStackMove( Uint32 top );

#pragma inline_asm(clearBL)
static void clearBL( void )
{
	STC	SR,R0
	MOV.L	?MASK,R1
	AND	R1,R0
	LDC	R0,SR
	BRA	?END
	NOP
	.ALIGN	4
?MASK:
	.DATA.L	H'EFFFFFFF
?END:
}

#pragma inline_asm(syIntEnableInt)
void syIntEnableInt( Uint32 imask )
{
	SHLL	R4
	SHLL	R4
	SHLL	R4
	SHLL	R4
	STC	SR,R0
	MOV.L	?MASK,R1
	AND	R1,R0
	OR	R4,R0
	LDC	R0,SR
	RTS
	NOP
	.ALIGN	4
?MASK:
	.DATA.L	H'EFFFFF0F
}

#pragma inline_asm(flushRange)
static void flushRange( void *address, Uint32 size )
{
	SHLR2	R5
	SHLR2	R5
	SHLR	R5
	ADD	#1,R5
?LOOP:
	OCBP	@R4
	DT	R5
	BF/S	?LOOP
	ADD	#H'20,R4
}

void syIntInit( Uint32 mode, const void *stack )
{
	Uint32 imask;

	imask = get_imask();
	set_imask( 15 );
	REG_CCR |= CCR_ICI;
	if( mode == SYD_INT_MODE_INTSTACK )
		intStackMove( (Uint32)stack );
	set_imask( imask );
	clearBL();
}

/* moves the interrupt stack to top (0: back to its first place), copying its last frame, and points the
   vector table's stack slot at it */
static void intStackMove( Uint32 top )
{
	Uint32 *src;
	Uint32 *dst;
	Uint32 *s;
	Uint32 *d;
	Uint32 ptr;
	Uint32 base;
	Sint32 i;
	Uint32 sr;

	set_cr( ( sr = get_cr() ) | SR_BL );
	if( top == 0 ) {
		if( intStackTop != 0 )
			top = intStackBase;
		else
			goto done;
	}
	if( intStackTop == 0 ) {
		ptr = *(volatile Uint32 *)( ( (Uint32)get_vbr() | P2_AREA ) + VBR_STACK_OFFSET );
		base = ( ptr + 0xFF ) & ~0xFF;
		intStackBase = base;
	} else {
		ptr = intStackPtr;
		base = intStackTop;
	}
	intStackPtr = ptr + top - base;
	intStackTop = top;
	src = (Uint32 *)( base - STACK_FRAME );
	dst = (Uint32 *)( top - STACK_FRAME );
	flushRange( src, STACK_FRAME );
	flushRange( dst, STACK_FRAME );
	if( src != dst ) {
		s = src;
		d = dst;
		for( i = STACK_FRAME / 4 + 1; --i != 0; )
			*d++ = *s++;
		*(Uint32 *)( ( (Uint32)get_vbr() + VBR_STACK_OFFSET ) | P2_AREA ) = intStackPtr;
	}
done:
	set_cr( sr );
}
