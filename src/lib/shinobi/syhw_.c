/* syhw_: Shinobi's hardware start-up: the interrupt, store queue, cache and timer set-up, and the
   registers that start from fixed values. */
#include "types.h"
#include "syreg.h"

#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()

/* a register and the value it starts from */
typedef struct HWREGS {
	Uint32	adr;
	Uint32	dat;
} HWREGS;

extern void syIntInit( Uint32 mode, const void *stack );
extern void sySqInit( void );
extern void sySqFinish( void );
extern void syCacheInit_P2_( Uint32 form );
extern void syTmrInit( void );
extern void syChainInit( void );
extern void syG2SysInit( void *work );
extern void syHwCacheFinish( void );

#define REG_CCR		( *(volatile Uint32 *)CCN_CCR )

static void hwInit( void );
static void p2_cache_set( void );

const char sygHwBuild[] = "\nsyHw Ver 1.07am Build:" "Nov 09 1999" " " "18:17:14" "\n";

const Uint32 sygCacheForm = CCR_ICE | CCR_OCE | CCR_CB;
const Uint32 sygIntMode = 0x00000000;
const void *sygIntStack = 0;

void syHwInit( void )
{
	hwInit();
	syIntInit( sygIntMode, sygIntStack );
	sySqInit();
	( (void ( * )( Uint32 ))( (Uint32)syCacheInit_P2_ | P2_AREA ) )( sygCacheForm );
	syTmrInit();
}

/* the NAOMI's start: only the interrupts and the timer */
void syHwInit_NAOMI( void )
{
	syIntInit( sygIntMode, sygIntStack );
	syTmrInit();
}

void syHwInit2( void )
{
	syChainInit();
	syG2SysInit( 0 );
}

void syHwFinish( void )
{
	Uint32 imask;

	imask = get_imask();
	set_imask( 15 );
	sySqFinish();
	syHwCacheFinish();
	set_imask( imask );
}

/* turns the caches off, from the uncached area */
void syHwCacheFinish( void )
{
	Uint32 adrs_dummy;
	void *set_adrs;

	adrs_dummy = (Uint32)&p2_cache_set;
	set_adrs = (void *)( adrs_dummy | P2_AREA );
	( (void ( * )())set_adrs )();
}

/* clears every entry of the operand cache and makes the instruction cache's invalid, with both off */
static void p2_cache_set( void )
{
	register Sint32 i;

	REG_CCR = 0x00000000;
	for( i = 0; i < 512; i++ )
		*(volatile Uint32 *)( CACHE_OC_ADDRESS + i * 0x20 ) = 0x00000000;
	REG_CCR = CCR_ICI;
	REG_CCR = 0x00000000;
}

/* the system bus registers hwInit sets, up to the zero address */
const HWREGS sygHwRegs[91] = {
	{ SB_C2DSTAT, 0x11FF0000 },
	{ SB_C2DLEN, 0x00000020 },
	{ SB_SDSTAW, 0x0CFF0000 },
	{ SB_SDBAAW, 0x0CFF0000 },
	{ SB_SDWLT, 0x00000000 },
	{ SB_SDLAS, 0x00000000 },
	{ SB_DBREQM, 0x00000000 },
	{ SB_BAVLWC, 0x00000000 },
	{ SB_C2DPRYC, 0x00000000 },
	{ SB_C2DMAXL, 0x00000000 },
	{ SB_LMMODE0, 0x00000000 },
	{ SB_LMMODE1, 0x00000000 },
	{ SB_RBSPLT, 0x80000000 },
	{ 0xA05F68A4, 0x00000000 },
	{ 0xA05F68AC, 0x00000000 },
	{ SB_IML2NRM, 0x00000000 },
	{ SB_IML2EXT, 0x00000000 },
	{ SB_IML2ERR, 0x00000000 },
	{ SB_IML4NRM, 0x00000000 },
	{ SB_IML4EXT, 0x00000000 },
	{ SB_IML4ERR, 0x00000000 },
	{ SB_IML6NRM, 0x00000000 },
	{ SB_IML6EXT, 0x00000000 },
	{ SB_IML6ERR, 0x00000000 },
	{ SB_PDTNRM, 0x00000000 },
	{ SB_PDTEXT, 0x00000000 },
	{ SB_G2DTNRM, 0x00000000 },
	{ SB_G2DTEXT, 0x00000000 },
	{ SB_MDSTAR, 0x0CFF0000 },
	{ SB_MDTSEL, 0x00000000 },
	{ SB_MDEN, 0x00000000 },
	{ SB_MSYS, 0xC3500000 },
	{ SB_MDAPRO, 0x61557F00 },
	{ SB_MMSEL, 0x00000001 },
	{ SB_ADSTAG, 0x009F0000 },
	{ SB_ADSTAR, 0x0CFF0000 },
	{ SB_ADLEN, 0x00000020 },
	{ SB_ADDIR, 0x00000000 },
	{ SB_ADTSEL, 0x00000005 },
	{ SB_ADEN, 0x00000000 },
	{ SB_ADSUSP, 0x00000000 },
	{ SB_E1STAG, 0x009F0000 },
	{ SB_E1STAR, 0x0CFF0000 },
	{ SB_E1LEN, 0x00000020 },
	{ SB_E1DIR, 0x00000000 },
	{ SB_E1TSEL, 0x00000005 },
	{ SB_E1EN, 0x00000000 },
	{ SB_E1SUSP, 0x00000000 },
	{ SB_E2STAG, 0x009F0000 },
	{ SB_E2STAR, 0x0CFF0000 },
	{ SB_E2LEN, 0x00000020 },
	{ SB_E2DIR, 0x00000000 },
	{ SB_E2TSEL, 0x00000005 },
	{ SB_E2EN, 0x00000000 },
	{ SB_E2SUSP, 0x00000000 },
	{ SB_G2DSTO, 0x0000001B },
	{ SB_G2TRTO, 0x00000271 },
	{ SB_G2MDMTO, 0x00000000 },
	{ SB_G2MDMW, 0x00000001 },
	{ 0xA05F78A0, 0x00000000 },
	{ 0xA05F78A4, 0x00000000 },
	{ 0xA05F78A8, 0x00000000 },
	{ 0xA05F78AC, 0x00000000 },
	{ 0xA05F78B0, 0x00000000 },
	{ 0xA05F78B4, 0x00000000 },
	{ 0xA05F78B8, 0x00000000 },
	{ SB_G2APRO, 0x46597F00 },
	{ SB_PDSTAP, 0x04FF0000 },
	{ SB_PDSTAR, 0x0CFF0000 },
	{ SB_PDLEN, 0x00000020 },
	{ SB_PDDIR, 0x00000000 },
	{ SB_PDTSEL, 0x00000000 },
	{ SB_PDEN, 0x00000000 },
	{ SB_PDAPRO, 0x67027F00 },
	{ SB_C2DST, 0x00000000 },
	{ SB_SDST, 0x00000000 },
	{ SB_MDST, 0x00000000 },
	{ SB_GDST, 0x00000000 },
	{ SB_ADST, 0x00000000 },
	{ SB_E1ST, 0x00000000 },
	{ SB_E2ST, 0x00000000 },
	{ SB_PDST, 0x00000000 },
	{ SB_ISTNRM, 0xFFFFFFFF },
	{ SB_ISTERR, 0xFFFFFFFF },
	{ 0x00000000, 0x00000000 },
};

/* masks every interrupt priority and sets the system bus registers */
static void hwInit( void )
{
	const HWREGS *reg;

	*(volatile Uint16 *)INTC_ICR = 0;
	*(volatile Uint16 *)INTC_IPRA = 0;
	*(volatile Uint16 *)INTC_IPRB = 0;
	*(volatile Uint16 *)INTC_IPRC = 0;
	reg = sygHwRegs;
	for( ;; ){
		if( reg->adr == 0 )
			break;
		*(volatile Uint32 *)reg->adr = reg->dat;
		reg++;
	}
}
