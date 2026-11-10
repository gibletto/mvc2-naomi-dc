/* syIntChkDump_: the address checker's trap handler: it dumps the saved SH-4 registers and the
   memory-mapped hardware registers to the serial port, then waits for the return key. */
#include "types.h"
#include "syreg.h"

typedef enum {
	SYE_CHK_PORT_SCIF,
	SYE_CHK_PORT_SCI
} SYE_CHK_PORT;

#define NUM_OF_MAPREGS	69
#define REG_EXPEVT	( *(Uint32 *)CCN_EXPEVT )

extern SYE_CHK_PORT syIntAdrChk_init( void );
extern void syIntAdrChk_prints( const char *text );
extern void syIntAdrChk_printh( Uint32 value );
extern char syIntAdrChk_getchar( void );
extern void syIntAdrChk_finish( void );

void syIntAdrChk_dumpline( const char *title, int start, int num );

/* the registers the handler found on the stack */
static Uint32 *regs;

const Uint32 * const sygIntAdrChk_adress[NUM_OF_MAPREGS] = {
	(Uint32 *)CCN_MMUCR,
	(Uint32 *)CCN_CCR,
	(Uint32 *)CCN_QACR0,
	(Uint32 *)CCN_QACR1,
	(Uint32 *)DMAC_SAR0,
	(Uint32 *)DMAC_DAR0,
	(Uint32 *)DMAC_SAR1,
	(Uint32 *)DMAC_DAR1,
	(Uint32 *)DMAC_CHCR1,
	(Uint32 *)DMAC_SAR2,
	(Uint32 *)DMAC_DMATCR2,
	(Uint32 *)DMAC_CHCR2,
	(Uint32 *)DMAC_SAR3,
	(Uint32 *)DMAC_DAR3,
	(Uint32 *)DMAC_CHCR3,
	(Uint32 *)DMAC_DMAOR,
	(Uint32 *)SB_C2DSTAT,
	(Uint32 *)SB_C2DLEN,
	(Uint32 *)SB_SDSTAW,
	(Uint32 *)SB_SDBAAW,
	(Uint32 *)SB_MDSTAR,
	(Uint32 *)SB_MDTSEL,
	(Uint32 *)SB_GDSTAR,
	(Uint32 *)SB_GDLEN,
	(Uint32 *)SB_GDDIR,
	(Uint32 *)SB_ADSTAG,
	(Uint32 *)SB_ADSTAR,
	(Uint32 *)SB_ADLEN,
	(Uint32 *)SB_ADDIR,
	(Uint32 *)SB_ADTSEL,
	(Uint32 *)SB_E1STAG,
	(Uint32 *)SB_E1STAR,
	(Uint32 *)SB_E1LEN,
	(Uint32 *)SB_E1DIR,
	(Uint32 *)SB_E1TSEL,
	(Uint32 *)SB_E2STAG,
	(Uint32 *)SB_E2STAR,
	(Uint32 *)SB_E2LEN,
	(Uint32 *)SB_E2DIR,
	(Uint32 *)SB_E2TSEL,
	(Uint32 *)SB_DDSTAG,
	(Uint32 *)SB_DDSTAR,
	(Uint32 *)SB_DDLEN,
	(Uint32 *)SB_DDDIR,
	(Uint32 *)SB_DDTSEL,
	(Uint32 *)SB_PDSTAP,
	(Uint32 *)SB_PDSTAR,
	(Uint32 *)SB_PDLEN,
	(Uint32 *)SB_PDDIR,
	(Uint32 *)SB_PDTSEL,
	(Uint32 *)PVR_PARAM_BASE,
	(Uint32 *)PVR_REGION_BASE,
	(Uint32 *)PVR_FB_R_CTRL,
	(Uint32 *)PVR_FB_W_CTRL,
	(Uint32 *)PVR_FB_R_SOF1,
	(Uint32 *)PVR_FB_R_SOF2,
	(Uint32 *)0xA05F8058,
	(Uint32 *)PVR_FB_R_SIZE,
	(Uint32 *)PVR_FB_W_SOF1,
	(Uint32 *)PVR_FB_W_SOF2,
	(Uint32 *)PVR_TA_OL_BASE,
	(Uint32 *)PVR_TA_ISP_BASE,
	(Uint32 *)PVR_TA_OL_LIMIT,
	(Uint32 *)PVR_TA_ISP_LIMIT,
	(Uint32 *)PVR_TA_NEXT_OPB,
	(Uint32 *)PVR_TA_ITP_CURRENT,
	(Uint32 *)PVR_TA_YUV_TEX_BASE,
	(Uint32 *)PVR_TA_YUV_TEX_CTRL,
	(Uint32 *)PVR_TA_YUV_TEX_CNT
};

void syIntChkDump( Uint16 event, Uint32 *stack )
{
	int i;
	SYE_CHK_PORT port;

	switch( event ) {
	case 0x300:
	case 0x340:
	case 0x380:
	case 0x3C0:
		break;
	default:
		return;
	}
	regs = stack;
	port = syIntAdrChk_init();
	syIntAdrChk_prints( "\r\n --- ADDRESS CHECKER TRAP --- \r\n" );
	syIntAdrChk_dumpline( "FR0-7:    ", 10, 8 );
	syIntAdrChk_dumpline( "FR8-15:   ", 18, 8 );
	syIntAdrChk_dumpline( "FR0-7(B): ", 26, 8 );
	syIntAdrChk_dumpline( "FR8-15(B):", 34, 8 );
	syIntAdrChk_prints( "FPUL: " );
	syIntAdrChk_printh( regs[42] );
	syIntAdrChk_prints( " FPSCR: " );
	syIntAdrChk_printh( regs[43] );
	syIntAdrChk_dumpline( "\r\nR0-7:     ", 45, 8 );
	{
		Uint32 r15;

		r15 = regs[60];
		regs[60] = regs[1];
		syIntAdrChk_dumpline( "R8-15:    ", 53, 8 );
		regs[60] = r15;
	}
	syIntAdrChk_dumpline( "\r\nR0-7(B):  ", 2, 8 );
	syIntAdrChk_prints( "MACL: " );
	syIntAdrChk_printh( regs[60] );
	syIntAdrChk_prints( " MACH: " );
	syIntAdrChk_printh( regs[61] );
	syIntAdrChk_prints( "\r\nVBR: " );
	syIntAdrChk_printh( regs[62] );
	syIntAdrChk_prints( " GBR: " );
	syIntAdrChk_printh( regs[63] );
	syIntAdrChk_prints( " DBR: " );
	syIntAdrChk_printh( regs[0] );
	syIntAdrChk_prints( "\r\nPR: " );
	syIntAdrChk_printh( regs[64] );
	syIntAdrChk_prints( " PC: " );
	syIntAdrChk_printh( regs[65] );
	syIntAdrChk_prints( " SR: " );
	syIntAdrChk_printh( regs[66] );
	for( i = 0 ; i < NUM_OF_MAPREGS ; i++ ) {
		if( ( i % 4 ) == 0 ) {
			syIntAdrChk_prints( "\r\n" );
		}
		syIntAdrChk_printh( (Uint32)sygIntAdrChk_adress[i] );
		syIntAdrChk_prints( ":" );
		syIntAdrChk_printh( *(Uint32 *)sygIntAdrChk_adress[i] );
		syIntAdrChk_prints( " " );
	}
	syIntAdrChk_prints( "\r\n" );
	switch( port ) {
	case SYE_CHK_PORT_SCIF:
		syIntAdrChk_prints( "Please, cancel the interrupt from ADRESS CHECKER,\r\n then hit RETURN key." );
		break;
	case SYE_CHK_PORT_SCI:
	default:
		syIntAdrChk_prints( "Please, cancel the interrupt from ADRESS CHECKER." );
		break;
	}
	while( syIntAdrChk_getchar() != '\r' )
		;
	REG_EXPEVT = event;
	syIntAdrChk_finish();
}

void syIntAdrChk_dumpline( const char *title, int start, int num )
{
	int i;

	syIntAdrChk_prints( title );
	for( i = start ; i < start + num ; i++ ) {
		syIntAdrChk_printh( regs[i] );
		syIntAdrChk_prints( " " );
	}
	syIntAdrChk_prints( "\r\n" );
}
