/* syIntCtrl_: Shinobi's interrupt set-up, as the disc has it (syInt 1.50 of May 1999): the interrupt
   mask call and the copy of the interrupt handlers into the vector table. */
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

const char * const sygIntBuild = "\nsyInt Ver 1.50 Build:" "May 31 1999" " " "16:38:10" "\n";
const SY_VERSION sygIntVer = { 1, 5, 0, 0 };

extern void *syIntMastISR;
extern void *syIntExpMastISR;
extern Sint32 syIntNestCount;
extern const void *syIntIntStack;
extern void syIntMasterISR( void );
extern void syIntMasterISR_2( void );
extern void syIntBrkRteRte( void );
extern Uint32 syIntISRRTE;
extern Uint32 syIntISRRTE_Bottom;
extern Uint32 syIntExpISRTop;
extern Uint32 syIntExpISRBottom;
extern Uint32 syIntISRTop;
extern Uint32 syIntISRBottom;

#define USER_VECT( ev )	( *(Uint32 *)( (Uint32)get_vbr() + 0x200 + ( ( ( ev ) - 0x200 ) >> 3 ) ) )

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

/* copies an interrupt routine to off in the vector table and writes it back from the cache */
#define COPY_ISR( off, top, bottom ) \
	dst = (Uint32 *)( (Uint32)get_vbr() + ( off ) ); \
	for( src = &( top ) ; src <= &( bottom ) ; ) \
		*dst++ = *src++; \
	flushRange( (Uint32 *)get_vbr() + ( off ), (Uint32)dst - (Uint32)get_vbr() - ( off ) )

void syIntInit( Uint32 mode, const void *stack )
{
	Uint32 imask;
	Uint32 *src;
	Uint32 *dst;
	Sint32 ev;

	imask = get_imask();
	set_imask( 15 );
	REG_CCR |= CCR_ICI;
	if( mode == SYD_INT_MODE_INTSTACK ) {
		syIntExpMastISR = (void *)syIntMasterISR_2;
		syIntMastISR = (void *)syIntMasterISR_2;
		syIntMastISR = (void *)syIntMasterISR_2;	/* set twice */
		syIntNestCount = 0;
		syIntIntStack = stack;
	} else {
		syIntExpMastISR = (void *)syIntMasterISR;
		syIntMastISR = (void *)syIntMasterISR;
		syIntNestCount = 0;
		syIntIntStack = NULL;
	}
	COPY_ISR( 0x000, syIntISRRTE, syIntISRRTE_Bottom );
	COPY_ISR( 0x100, syIntExpISRTop, syIntExpISRBottom );
	COPY_ISR( 0x400, syIntExpISRTop, syIntExpISRBottom );
	COPY_ISR( 0x600, syIntISRTop, syIntISRBottom );
	set_imask( imask );
	for( ev = 0x40 ; ev < 0x820 ; ev += 0x20 )
		USER_VECT( ev ) = (Uint32)syIntBrkRteRte;
	clearBL();
}
