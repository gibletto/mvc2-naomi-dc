/* nlintr: the NAOMI library's interrupt layer: the handlers it hangs on Kamui2's and Shinobi's
   callbacks, the table of functions that are called from them, and the interrupt priorities.
   The interrupts are numbered: 0 the end of a render, 1 the V-blank, 2 the H-blank, 3 the end of
   a G1 DMA, 4 the external interrupt, 5 to 11 the timers and the serial port, 12 a texture
   overflow. */
#include "types.h"

#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()
#define set_cr( cr )	_builtin_set_cr( cr )
#define get_cr()	_builtin_get_cr()
#define get_vbr()	_builtin_get_vbr()

/* the status register's bit that blocks interrupts */
#define SR_BL		0x10000000
#define NLI_INTR_MAX	13
#define NLI_CLEAR_MAX	6

/* the system's interrupt flags with the flag of a G1 DMA's end, the external interrupt's
   acknowledge, and the first of the SH-4's priority registers */
#define REG_ISTNRM	( *(volatile Uint32 *)0xA05F6900 )
#define IST_G1_DMA	0x4000
#define REG_ACK		( *(volatile Uint32 *)0xA05F705C )
#define REG_IPR		0xFFD00004
#define VBR_VECTOR	0x000001C0

/* the events of Shinobi's interrupt chain that have a handler here */
#define EVENT_VBLANK	0x900
#define EVENT_HBLANK	0x920
#define EVENT_G1_DMA	0xA40
#define EVENT_EXTERNAL	0xB00

typedef Sint32 ( *NLINTRFUNC )( void );

/* where an interrupt's priority lives: its register, its shift and the bits it takes */
typedef struct NLINTRPRIORITY {
	Sint8	reg;
	Sint8	shift;
	Sint16	mask;
} NLINTRPRIORITY;

/* the block at the vector base: the exception entries, then a handler for each 0x20 of event code */
typedef struct NLVBR {
	Sint8		entry[VBR_VECTOR];
	NLINTRFUNC	vector[0x80];
} NLVBR;

extern void kmSetEORCallback( Sint32 ( *func )( void ), void *arg );
extern void kmSetTexOverflowCallback( Sint32 ( *func )( void ), void *arg );
extern void kmSetWaitVsyncCallback( NLINTRFUNC func, void *arg );
extern void kmSetHSyncLine( Sint32 line );
extern Sint32 syChainAddHandler( Sint32 event, Sint32 ( *func )( void ), Uint32 priority, void *arg );
extern Uint32 sygIntMode;
extern Uint32 sygIntStack;

Sint32 nlSetIntrFunction( Sint32 num, NLINTRFUNC func );
Sint32 nlSetIntrPriority( Sint32 num, Sint32 priority );
Sint32 nlSynchFunction( NLINTRFUNC func );
void nlIntrSetHSynchLine( Sint32 line );

/* the event codes of interrupts 5 to 11: the three timers, and the serial port's four */
static const Sint16 nli_vector[7] = { 0x0400, 0x0420, 0x0440, 0x0700, 0x0720, 0x0740, 0x0760 };
/* the priorities at the start: all off */
static const Sint16 nli_priority_init[12][2] = {
	{ 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 },
	{ 6, 0 }, { 7, 0 }, { 8, 0 }, { 9, 0 }, { 10, 0 }, { 11, 0 }
};
const char * const libintrBuild = "\nlibintr Ver 1.051 Build:" "Oct 28 1999" " " "19:42:51" "\n";

static NLINTRFUNC nli_func[NLI_INTR_MAX];
static Sint32 nli_handle[NLI_INTR_MAX];
Sint32 nliIntrReason;

/* The handlers: each notes which interrupt it is and calls its function, if one is set. */
static Sint32 nli_intr_eor( void )
{
	NLINTRFUNC func;

	nliIntrReason = 0;
	func = nli_func[0];
	if( func != 0 )
		func();
	return 0;
}

static Sint32 nli_intr_1( void )
{
	NLINTRFUNC func;

	nliIntrReason = 1;
	func = nli_func[1];
	if( func != 0 )
		func();
	return 0;
}

static Sint32 nli_intr_2( void )
{
	NLINTRFUNC func;

	nliIntrReason = 2;
	func = nli_func[2];
	if( func != 0 )
		func();
	return 0;
}

static Sint32 nli_intr_3( void )
{
	NLINTRFUNC func;

	nliIntrReason = 3;
	func = nli_func[3];
	if( func != 0 )
		func();
	REG_ISTNRM = IST_G1_DMA;
	return 0;
}

static Sint32 nli_intr_5( void )
{
	NLINTRFUNC func;

	nliIntrReason = 5;
	func = nli_func[4];
	if( func != 0 )
		func();
	REG_ACK = 3;
	return 0;
}

static Sint32 nli_intr_4( void )
{
	NLINTRFUNC func;

	nliIntrReason = 4;
	func = nli_func[12];
	if( func != 0 )
		func();
	return 0;
}

/* Clears the functions and the priorities and hangs the handlers; full adds the two that only
   the NAOMI has. */
static void nli_init( Sint32 full )
{
	Sint32 i;
	Uint32 sr;
	const Sint16 *init;

	set_cr( ( sr = get_cr() ) | SR_BL );
	for( i = 0; i < NLI_INTR_MAX; i++ ){
		nli_func[i] = 0;
		nli_handle[i] = -1;
	}
	init = &nli_priority_init[0][0];
	for( i = 0; i < 12; i++, init += 2 )
		nlSetIntrPriority( init[0], init[1] );
	for( i = 0; i < NLI_INTR_MAX; i++ )
		nlSetIntrFunction( i, 0 );
	nlSynchFunction( 0 );
	kmSetEORCallback( nli_intr_eor, 0 );
	nli_handle[1] = syChainAddHandler( EVENT_VBLANK, nli_intr_1, 0x58, 0 );
	nli_handle[2] = syChainAddHandler( EVENT_HBLANK, nli_intr_2, 0, 0 );
	if( full ){
		nli_handle[3] = syChainAddHandler( EVENT_G1_DMA, nli_intr_3, 0, 0 );
		nli_handle[4] = syChainAddHandler( EVENT_EXTERNAL, nli_intr_5, 0, 0 );
	}
	kmSetTexOverflowCallback( nli_intr_4, 0 );
	nlIntrSetHSynchLine( 0 );
	set_cr( sr );
}

void nlInitInterrupt( void )
{
	nli_init( 1 );
}

void nlInitInterruptDC( void )
{
	nli_init( 0 );
}

/* How each interrupt's function is set: in the table the handlers call from, or straight in
   the vector table. */
static Sint32 nli_set_func0( Sint32 num, NLINTRFUNC func )
{
	nli_func[0] = func;
	return 1;
}

static Sint32 nli_set_func1( Sint32 num, NLINTRFUNC func )
{
	nli_func[1] = func;
	return 1;
}

static Sint32 nli_set_func2( Sint32 num, NLINTRFUNC func )
{
	nli_func[2] = func;
	return 1;
}

static Sint32 nli_set_func3( Sint32 num, NLINTRFUNC func )
{
	nli_func[3] = func;
	return 1;
}

static Sint32 nli_set_func4( Sint32 num, NLINTRFUNC func )
{
	nli_func[4] = func;
	return 1;
}

/* an event code is eight times its handler's offset in the table */
static Sint32 nli_set_vector( Sint32 num, NLINTRFUNC func )
{
	Uint32 code;

	code = nli_vector[num - 5];
	*(NLINTRFUNC *)( (Uint32)( (NLVBR *)get_vbr() )->vector + ( code >> 3 ) ) = func;
	return 1;
}

static Sint32 nli_set_func12( Sint32 num, NLINTRFUNC func )
{
	nli_func[12] = func;
	return 1;
}

static Sint32 nli_set_none( Sint32 num, NLINTRFUNC func )
{
	return 0;
}

static Sint32 ( *nli_set_table[NLI_INTR_MAX] )( Sint32 num, NLINTRFUNC func ) = {
	nli_set_func0, nli_set_func1, nli_set_func2, nli_set_func3, nli_set_func4,
	nli_set_vector, nli_set_vector, nli_set_vector, nli_set_vector, nli_set_vector, nli_set_vector, nli_set_vector,
	nli_set_func12
};

/* Sets the function of an interrupt; 0 when there is no such interrupt. */
Sint32 nlSetIntrFunction( Sint32 num, NLINTRFUNC func )
{
	Sint32 imask;
	Sint32 ret;

	imask = get_imask();
	set_imask( 14 );
	if( num < NLI_INTR_MAX )
		ret = nli_set_table[num]( num, func );
	else
		ret = nli_set_none( num, func );
	set_imask( imask );
	return ret;
}

Sint32 nlGetIntrFunction( Sint32 num, NLINTRFUNC *func )
{
	Sint32 ret;

	if( num < NLI_INTR_MAX ){
		*func = nli_func[num];
		ret = 1;
	}else{
		*func = 0;
		ret = 0;
	}
	return ret;
}

/* Which interrupt the last handler was for. */
Sint32 nlIntrGetEvent( void )
{
	return nliIntrReason;
}

/* How each interrupt is acknowledged, for those that need it. */
static void nli_clear_none( Sint32 num )
{
}

static void nli_clear_istnrm( Sint32 num )
{
	REG_ISTNRM = IST_G1_DMA;
}

static void nli_clear_ack( Sint32 num )
{
	REG_ACK = 3;
}

static void ( *nli_clear_table[NLI_CLEAR_MAX] )( Sint32 num ) = {
	nli_clear_none, nli_clear_none, nli_clear_none, nli_clear_istnrm, nli_clear_none, nli_clear_ack
};

void nlIntrClearEvent( Sint32 num )
{
	if( num < NLI_CLEAR_MAX )
		nli_clear_table[num]( num );
}

/* each priority's place in the three priority registers */
static NLINTRPRIORITY nli_priority[12] = {
	{ 8, 0, 0x000F }, { 8, 12, 0xF000 }, { 8, 8, 0x0F00 }, { 0, 12, 0xF000 }, { 0, 8, 0x0F00 }, { 0, 4, 0x00F0 },
	{ 0, 0, 0x000F }, { 4, 4, 0x00F0 }, { 8, 4, 0x00F0 }, { 4, 12, 0xF000 }, { 4, 8, 0x0F00 }, { 0, 0, 0 }
};

/* Sets one of the SH-4's interrupt priorities, 0 to 15. */
Sint32 nlSetIntrPriority( Sint32 num, Sint32 priority )
{
	Uint32 sr;
	Sint32 mask;
	volatile Uint16 *reg;

	if( num > 12 )
		return 0;
	if( priority < 0 || priority > 15 )
		return 0;
	set_cr( ( sr = get_cr() ) | SR_BL );
	reg = (volatile Uint16 *)( REG_IPR + nli_priority[num].reg );
	mask = nli_priority[num].mask;
	*reg = ( *reg & ~mask ) | ( priority << nli_priority[num].shift );
	*reg;
	set_cr( sr );
	return 1;
}

Sint32 nlGetIntrPriority( Sint32 num, Sint32 *priority )
{
	volatile Uint16 *reg;

	if( num > 12 )
		return 0;
	reg = (volatile Uint16 *)( REG_IPR + nli_priority[num].reg );
	*priority = ( *reg >> nli_priority[num].shift ) & 15;
	return 1;
}

/* Sets the function Kamui2 calls while it waits for the V-blank. */
Sint32 nlSynchFunction( NLINTRFUNC func )
{
	Uint32 sr;

	set_cr( ( sr = get_cr() ) | SR_BL );
	kmSetWaitVsyncCallback( func, 0 );
	set_cr( sr );
	return 0;
}

/* Not there: it stops. */
void nlSetHSynchMode( void )
{
	for( ;; )
		;
}

void nlIntrSetHSynchLine( Sint32 line )
{
	kmSetHSyncLine( line );
}

/* Gives the interrupts a stack of their own. */
void nlSetIntrStack( Uint32 top )
{
	sygIntMode = 0x80000000;
	sygIntStack = top;
}
