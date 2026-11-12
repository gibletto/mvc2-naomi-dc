/* sh4tmr_: Shinobi's low-level timer, as the disc has it (syTmr 1.04 of April 1999): the
   free-running count on timer 0, a one-shot or timed interrupt on timer 1, and a general-purpose
   timer 2 with its clock, count and interrupt. */
#include "types.h"
#include "syreg.h"

#define USER_VECT( n )	( *(Uint32 *)( (Uint32)get_vbr() + ( n ) ) )
#define get_vbr()	_builtin_get_vbr()

#define REG_TOCR	( *(volatile char *)TMU_TOCR )
#define REG_TSTR	( *(volatile char *)TMU_TSTR )
#define REG_TCOR0	( *(volatile long *)TMU_TCOR0 )
#define REG_TCNT0	( *(volatile long *)TMU_TCNT0 )
#define REG_TCR0	( *(volatile short *)TMU_TCR0 )
#define REG_TCOR1	( *(volatile long *)TMU_TCOR1 )
#define REG_TCNT1	( *(volatile long *)TMU_TCNT1 )
#define REG_TCR1	( *(volatile short *)TMU_TCR1 )
#define REG_TCOR2	( *(volatile long *)TMU_TCOR2 )
#define REG_TCNT2	( *(volatile long *)TMU_TCNT2 )
#define REG_TCR2	( *(volatile short *)TMU_TCR2 )
#define REG_IPRA	( *(volatile short *)INTC_IPRA )

#define TSTR_0		( 1 << 0 )
#define TSTR_1		( 1 << 1 )
#define TSTR_2		( 1 << 2 )
#define TCR_UNIE	( 1 << 5 )
#define TCR_UNF		( 1 << 8 )

#define IPRA_TMU1	0x0F00
#define IPRA_TMU2	0x00F0

#define SYD_TMR_CLOCK_P4	0
#define SYD_TMR_CLOCK_P16	1
#define SYD_TMR_CLOCK_P64	2
#define SYD_TMR_CLOCK_P256	3
#define SYD_TMR_CLOCK_P1024	4
#define SYD_INT_EVENT_TUNI1	0x00000244
#define SYD_INT_EVENT_TUNI2	0x00000248

#define SYD_TMR_ERR_OK		0
#define SYD_TMR_ERR_EXIST	( -1 )

/* microseconds per 100 counts at each clock */
#define COUNT_CLOCK_P4		8
#define COUNT_CLOCK_P16		32
#define COUNT_CLOCK_P64		128
#define COUNT_CLOCK_P256	512
#define COUNT_CLOCK_P1024	2048

static void ( *tmrHandler1 )( void * );
static void *tmrParam1;
static void ( *tmrHandler2 )( void * );
static void *tmrParam2;

static void tmrInt1Handler( void );
static void tmrInt2Handler( void );

const char * const sygTmrBuild = "\nsyTmr Ver 1.04 Build:" "Apr 26 1999" " " "14:43:51" "\n";

void syTmrInit( void )
{
	REG_TOCR = 0;
	REG_TSTR &= ~TSTR_0;
	REG_TCR0 = SYD_TMR_CLOCK_P64;
	REG_TCOR0 = 0xFFFFFFFFL;
	REG_TCNT0 = 0xFFFFFFFFL;
	REG_TSTR |= TSTR_0;
}

Uint32 syTmrGetCount( void )
{
	return 0xFFFFFFFFUL - (unsigned long)REG_TCNT0;
}

Uint32 syTmrDiffCount( unsigned long count1, unsigned long count2 )
{
	return count2 - count1;
}

Uint32 syTmrCountToMicro( unsigned long count )
{
	return ( count / 100 * COUNT_CLOCK_P64 ) + ( count % 100 * COUNT_CLOCK_P64 / 100 );
}

Uint32 syTmrMicroToCount( unsigned long micro )
{
	return ( micro / COUNT_CLOCK_P64 * 100 ) + ( micro % COUNT_CLOCK_P64 * 100 / COUNT_CLOCK_P64 );
}

Sint32 syTmrInt1SetEx( unsigned long count, void ( *callback )( void * ), void *param, Uint16 clock );

Sint32 syTmrInt1Set( unsigned long count, void ( *callback )( void * ), void *param )
{
	return syTmrInt1SetEx( count, callback, param, SYD_TMR_CLOCK_P64 );
}

Sint32 syTmrInt1SetEx( unsigned long count, void ( *callback )( void * ), void *param, Uint16 clock )
{
	if( REG_TSTR & TSTR_1 )
		return SYD_TMR_ERR_EXIST;
	REG_TOCR = 0;
	REG_TSTR &= ~TSTR_1;
	REG_TCR1 = clock;
	if( (void ( * )( void * ))0 != callback ) {
		USER_VECT( SYD_INT_EVENT_TUNI1 ) = (long)tmrInt1Handler;
		tmrHandler1 = callback;
		tmrParam1 = param;
		REG_TCR1 |= TCR_UNIE;
		REG_IPRA = ( REG_IPRA & 0xF0FF ) | IPRA_TMU1;
		REG_TCR1 &= ~TCR_UNF;
	}
	REG_TCOR1 = count;
	REG_TCNT1 = count;
	REG_TSTR |= TSTR_1;
	if( (void ( * )( void * ))0 == callback ) {
		while( !( REG_TCR1 & TCR_UNF ) )
			;
		REG_TSTR &= ~TSTR_1;
		REG_TCR1 &= ~TCR_UNF;
	}
	return SYD_TMR_ERR_OK;
}

void syTmrInt1Cancel( void )
{
	REG_TSTR &= ~TSTR_1;
	REG_TCR1 &= ~TCR_UNIE;
}

static void tmrInt1Handler( void )
{
	REG_TSTR &= ~TSTR_1;
	REG_TCR1 &= ~TCR_UNIE;
	REG_TCR1 &= ~TCR_UNF;
	( *tmrHandler1 )( tmrParam1 );
}

static void tmrInt2Handler( void )
{
	REG_TSTR &= ~TSTR_2;
	REG_TCR2 &= ~TCR_UNIE;
	REG_TCR2 &= ~TCR_UNF;
	( *tmrHandler2 )( tmrParam2 );
}

void syTmrGenStart( void )
{
	REG_TOCR = 0;
	REG_TSTR |= TSTR_2;
}

void syTmrGenStop( void )
{
	REG_TSTR &= ~TSTR_2;
}

void syTmrGenSetInt( void ( *callback )( void * ), void *param )
{
	unsigned char saved;

	saved = REG_TSTR;
	REG_TSTR &= ~TSTR_2;
	if( (void ( * )( void * ))0 != callback ) {
		USER_VECT( SYD_INT_EVENT_TUNI2 ) = (long)tmrInt2Handler;
		tmrHandler2 = callback;
		tmrParam2 = param;
		REG_TCR2 |= TCR_UNIE;
		REG_IPRA = ( REG_IPRA & 0xFF0F ) | IPRA_TMU2;
		REG_TCR2 &= ~TCR_UNF;
	}
	REG_TSTR = saved;
	if( (void ( * )( void * ))0 == callback ) {
		REG_TOCR = 0;
		REG_TSTR |= TSTR_2;
		while( !( REG_TCR2 & TCR_UNF ) )
			;
		REG_TSTR &= ~TSTR_2;
		REG_TCR2 &= ~TCR_UNF;
	}
}

void syTmrGenCancelInt( void )
{
	unsigned char saved;

	saved = REG_TSTR;
	REG_TSTR &= ~TSTR_2;
	REG_TCR2 &= ~TCR_UNIE;
	REG_TSTR = saved;
}

void syTmrGenSetClock( Uint16 clock )
{
	unsigned char saved;

	if( ( clock != SYD_TMR_CLOCK_P4 ) &&
	    ( clock != SYD_TMR_CLOCK_P16 ) &&
	    ( clock != SYD_TMR_CLOCK_P64 ) &&
	    ( clock != SYD_TMR_CLOCK_P256 ) &&
	    ( clock != SYD_TMR_CLOCK_P1024 ) ) {
		clock = SYD_TMR_CLOCK_P64;
	}
	saved = REG_TSTR;
	REG_TSTR &= ~TSTR_2;
	REG_TCR2 = ( REG_TCR2 & ~7 ) | clock;
	REG_TSTR = saved;
}

void syTmrGenSetCount( Uint32 count )
{
	unsigned char saved;

	saved = REG_TSTR;
	REG_TSTR &= ~TSTR_2;
	REG_TCOR2 = 0xFFFFFFFFL;
	REG_TCNT2 = 0L - count;
	REG_TSTR = saved;
}

Uint32 syTmrGenGetCount( void )
{
	return 0xFFFFFFFFUL - (unsigned long)REG_TCNT2;
}

Uint32 syTmrGenCountToMicro( Uint32 count, Uint16 clock )
{
	Uint32 per100;

	switch( clock ) {
	case SYD_TMR_CLOCK_P4:
		per100 = COUNT_CLOCK_P4;
		break;
	case SYD_TMR_CLOCK_P16:
		per100 = COUNT_CLOCK_P16;
		break;
	case SYD_TMR_CLOCK_P256:
		per100 = COUNT_CLOCK_P256;
		break;
	case SYD_TMR_CLOCK_P1024:
		per100 = COUNT_CLOCK_P1024;
		break;
	default:
		per100 = COUNT_CLOCK_P64;
		break;
	}
	return ( count / 100 * per100 ) + ( count % 100 * per100 / 100 );
}

Uint32 syTmrGenMicroToCount( Uint32 micro, Uint16 clock )
{
	Uint32 per100;

	switch( clock ) {
	case SYD_TMR_CLOCK_P4:
		per100 = COUNT_CLOCK_P4;
		break;
	case SYD_TMR_CLOCK_P16:
		per100 = COUNT_CLOCK_P16;
		break;
	case SYD_TMR_CLOCK_P256:
		per100 = COUNT_CLOCK_P256;
		break;
	case SYD_TMR_CLOCK_P1024:
		per100 = COUNT_CLOCK_P1024;
		break;
	default:
		per100 = COUNT_CLOCK_P64;
		break;
	}
	return ( micro / per100 * 100 ) + ( micro % per100 * 100 / per100 );
}

Uint32 syTmrGenDiffCount( unsigned long count1, unsigned long count2 )
{
	return syTmrDiffCount( count1, count2 );
}
