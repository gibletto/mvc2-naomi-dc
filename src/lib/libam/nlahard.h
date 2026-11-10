/* nlahard.h: the hardware libam works with: the system bus's registers, the ROM board, the clock,
   the backup RAM and what the boot ROM leaves in memory. */
#ifndef NLAHARD_H
#define NLAHARD_H

/* the uncached view of memory */
#define P2_AREA		0xa0000000

/* the system's interrupt flags, and the flag of a Maple DMA's end */
#define SB_ISTNRM	( *( volatile unsigned int * )0xA05F6900 )
#define IST_MAPLE_DMA	0x1000

/* Maple DMA registers */
#define SB_MDSTAR	( *( volatile unsigned int * )0xA05F6C04 )
#define SB_MDEN		( *( volatile unsigned int * )0xA05F6C14 )
#define SB_MDST		( *( volatile unsigned int * )0xA05F6C18 )

/* ROM board registers: the offset and data window, the DMA's offset and count, and the port */
#define ROMB_OFSH	( *( volatile unsigned int * )0xA05F7000 )
#define ROMB_OFSL	( *( volatile unsigned int * )0xA05F7004 )
#define ROMB_DATA	( *( volatile unsigned int * )0xA05F7008 )
#define ROMB_DMAH	( *( volatile unsigned int * )0xA05F700C )
#define ROMB_DMAL	( *( volatile unsigned int * )0xA05F7010 )
#define ROMB_DMALEN	( *( volatile unsigned int * )0xA05F7014 )
#define ROMB_PORT	( *( volatile unsigned int * )0xA05F7068 )

/* G1 DMA registers */
#define SB_GDSTAR	( *( volatile unsigned int * )0xA05F7404 )
#define SB_GDLEN	( *( volatile unsigned int * )0xA05F7408 )
#define SB_GDDIR	( *( volatile unsigned int * )0xA05F740C )
#define SB_GDEN		( *( volatile unsigned int * )0xA05F7414 )
#define SB_GDST		( *( volatile unsigned int * )0xA05F7418 )

/* G1 bus wait registers: the access timing of the ROM board's ROM, flash and GD-ROM window,
   and the range of memory its DMA may write */
#define SB_G1RRC	( *( volatile unsigned int * )0xA05F7480 )
#define SB_G1RWC	( *( volatile unsigned int * )0xA05F7484 )
#define SB_G1FRC	( *( volatile unsigned int * )0xA05F7488 )
#define SB_G1FWC	( *( volatile unsigned int * )0xA05F748C )
#define SB_G1CRC	( *( volatile unsigned int * )0xA05F7490 )
#define SB_G1CWC	( *( volatile unsigned int * )0xA05F7494 )
#define SB_G1GDRC	( *( volatile unsigned int * )0xA05F74A0 )
#define SB_G1GDWC	( *( volatile unsigned int * )0xA05F74A4 )
#define SB_GDAPRO	( *( volatile unsigned int * )0xA05F74B8 )

/* the DMA controller's operation register, and its setting: DMA on, channel 2 first, transfers
   on demand */
#define DMAOR		( *( volatile unsigned int * )0xFFA00040 )
#define DMAOR_SET	0x8201

/* the clock's count of seconds in two halves, read as 16 bits or as 32 */
#define RTC_HIGH	( *( unsigned short * )0xA0710000 )
#define RTC_LOW		( *( unsigned short * )0xA0710004 )
#define RTC_HIGH_L	( *( volatile unsigned int * )0xA0710000 )
#define RTC_LOW_L	( *( volatile unsigned int * )0xA0710004 )

/* the backup RAM, which a battery keeps */
#define SHM_BASE	0xA0200000

/* the boot ROM's service and where it is copied to in RAM, in words */
#define BOOT_SERVICE_ROM	( ( unsigned int * )0xa0060000 )
#define BOOT_SERVICE_RAM	( ( unsigned int * )0xac018000 )
#define BOOT_SERVICE_SIZE	0x1c00

/* the boot ROM's copyright text */
#define BOOT_ROM_COPYRIGHT	( ( const char * )0xa01ffd00 )

/* what the boot ROM leaves in RAM: the clock's count at the start, the two serial numbers, the
   game's description and the defaults for each country, 16 bytes each */
#define BOOT_TIME	( *( unsigned int * )0x0c01f130 )
#define BOOT_SERIAL1	( ( const char * )0x0c01f200 )
#define BOOT_SERIAL2	( ( const char * )0x0c01f220 )
#define BOOT_ID		0x0c01f400
#define BOOT_DEFAULTS	( ( const char * )0x0c01f5e0 )

#endif
