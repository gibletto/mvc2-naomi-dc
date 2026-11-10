/* nlreg.h: where the hardware registers and memory areas that the NAOMI graphics library uses are.
   Each use casts the address to the access it makes. */
#ifndef LIBNAOMI_NLREG_H
#define LIBNAOMI_NLREG_H

/* the uncached view of memory */
#define P2_AREA		0xA0000000

/* video memory: the 64-bit access area, which textures use, and the 32-bit one of the frame buffers */
#define VRAM_64BIT	0xA4000000
#define VRAM_32BIT	0xA5000000

/* SH-4: the store queues' address registers */
#define CCN_QACR0	0xFF000038
#define CCN_QACR1	0xFF00003C

/* SH-4: the DMA controller's channel 2, which feeds the tile accelerator, and its operation register */
#define DMAC_SAR2	0xFFA00020
#define DMAC_CHCR2	0xFFA0002C
#define DMAC_DMAOR	0xFFA00040

/* SH-4: the timer unit's channel 0 */
#define TMU_TOCR	0xFFD80000
#define TMU_TSTR	0xFFD80004
#define TMU_TCOR0	0xFFD80008
#define TMU_TCNT0	0xFFD8000C
#define TMU_TCR0	0xFFD80010

/* system bus: the revision of the chip set */
#define SB_SBREV	0xA05F689C

/* graphics chip: the alpha a punch-through texel is tested against */
#define PVR_PT_ALPHA_REF	0xA05F811C

#endif
