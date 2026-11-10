/* syreg.h: where the hardware registers that Shinobi's modules use are: the SH-4's own, the system
   bus (Holly) and the graphics chip (PowerVR). Each module casts the address to the access it makes. */
#ifndef SYREG_H
#define SYREG_H

/* the uncached view of memory */
#define P2_AREA		0xA0000000

/* SH-4: memory management, cache, exceptions and the store queues */
#define CCN_MMUCR	0xFF000010
#define CCN_CCR		0xFF00001C
#define CCN_EXPEVT	0xFF000024
#define CCN_INTEVT	0xFF000028
#define CCN_QACR0	0xFF000038
#define CCN_QACR1	0xFF00003C

/* the cache control register's bits: the operand cache on, copy-back, the instruction cache on
   and its entries made invalid */
#define CCR_OCE		0x00000001
#define CCR_CB		0x00000004
#define CCR_ICE		0x00000100
#define CCR_ICI		0x00000800

/* SH-4: the DMA controller, four channels */
#define DMAC_SAR0	0xFFA00000
#define DMAC_DAR0	0xFFA00004
#define DMAC_SAR1	0xFFA00010
#define DMAC_DAR1	0xFFA00014
#define DMAC_CHCR1	0xFFA0001C
#define DMAC_SAR2	0xFFA00020
#define DMAC_DMATCR2	0xFFA00028
#define DMAC_CHCR2	0xFFA0002C
#define DMAC_SAR3	0xFFA00030
#define DMAC_DAR3	0xFFA00034
#define DMAC_CHCR3	0xFFA0003C
#define DMAC_DMAOR	0xFFA00040

/* SH-4: the interrupt controller */
#define INTC_ICR	0xFFD00000
#define INTC_IPRA	0xFFD00004
#define INTC_IPRB	0xFFD00008
#define INTC_IPRC	0xFFD0000C

/* SH-4: the timer unit, three channels */
#define TMU_TOCR	0xFFD80000
#define TMU_TSTR	0xFFD80004
#define TMU_TCOR0	0xFFD80008
#define TMU_TCNT0	0xFFD8000C
#define TMU_TCR0	0xFFD80010
#define TMU_TCOR1	0xFFD80014
#define TMU_TCNT1	0xFFD80018
#define TMU_TCR1	0xFFD8001C
#define TMU_TCOR2	0xFFD80020
#define TMU_TCNT2	0xFFD80024
#define TMU_TCR2	0xFFD80028

/* the operand cache's address array, an entry every 32 bytes */
#define CACHE_OC_ADDRESS	0xF4000000

/* system bus: the DMA to the tile accelerator (channel 2) and the sort DMA */
#define SB_C2DSTAT	0xA05F6800
#define SB_C2DLEN	0xA05F6804
#define SB_C2DST	0xA05F6808
#define SB_SDSTAW	0xA05F6810
#define SB_SDBAAW	0xA05F6814
#define SB_SDWLT	0xA05F6818
#define SB_SDLAS	0xA05F681C
#define SB_SDST		0xA05F6820
#define SB_DBREQM	0xA05F6840
#define SB_BAVLWC	0xA05F6844
#define SB_C2DPRYC	0xA05F6848
#define SB_C2DMAXL	0xA05F684C
#define SB_LMMODE0	0xA05F6884
#define SB_LMMODE1	0xA05F6888
#define SB_RBSPLT	0xA05F68A0

/* system bus: the interrupt status (normal, external, error), the masks of levels 2, 4 and 6,
   and the interrupts that start a DMA */
#define SB_ISTNRM	0xA05F6900
#define SB_ISTEXT	0xA05F6904
#define SB_ISTERR	0xA05F6908
#define SB_IML2NRM	0xA05F6910
#define SB_IML2EXT	0xA05F6914
#define SB_IML2ERR	0xA05F6918
#define SB_IML4NRM	0xA05F6920
#define SB_IML4EXT	0xA05F6924
#define SB_IML4ERR	0xA05F6928
#define SB_IML6NRM	0xA05F6930
#define SB_IML6EXT	0xA05F6934
#define SB_IML6ERR	0xA05F6938
#define SB_PDTNRM	0xA05F6940
#define SB_PDTEXT	0xA05F6944
#define SB_G2DTNRM	0xA05F6950
#define SB_G2DTEXT	0xA05F6954

/* system bus: Maple */
#define SB_MDSTAR	0xA05F6C04
#define SB_MDTSEL	0xA05F6C10
#define SB_MDEN		0xA05F6C14
#define SB_MDST		0xA05F6C18
#define SB_MSYS		0xA05F6C80
#define SB_MDAPRO	0xA05F6C8C
#define SB_MMSEL	0xA05F6CE8

/* system bus: the G1 bus's DMA */
#define SB_GDSTAR	0xA05F7404
#define SB_GDLEN	0xA05F7408
#define SB_GDDIR	0xA05F740C
#define SB_GDST		0xA05F7418

/* system bus: the G2 bus's DMA channels (the sound chip, two external devices and the debugger),
   and the bus's time-outs */
#define SB_ADSTAG	0xA05F7800
#define SB_ADSTAR	0xA05F7804
#define SB_ADLEN	0xA05F7808
#define SB_ADDIR	0xA05F780C
#define SB_ADTSEL	0xA05F7810
#define SB_ADEN		0xA05F7814
#define SB_ADST		0xA05F7818
#define SB_ADSUSP	0xA05F781C
#define SB_E1STAG	0xA05F7820
#define SB_E1STAR	0xA05F7824
#define SB_E1LEN	0xA05F7828
#define SB_E1DIR	0xA05F782C
#define SB_E1TSEL	0xA05F7830
#define SB_E1EN		0xA05F7834
#define SB_E1ST		0xA05F7838
#define SB_E1SUSP	0xA05F783C
#define SB_E2STAG	0xA05F7840
#define SB_E2STAR	0xA05F7844
#define SB_E2LEN	0xA05F7848
#define SB_E2DIR	0xA05F784C
#define SB_E2TSEL	0xA05F7850
#define SB_E2EN		0xA05F7854
#define SB_E2ST		0xA05F7858
#define SB_E2SUSP	0xA05F785C
#define SB_DDSTAG	0xA05F7860
#define SB_DDSTAR	0xA05F7864
#define SB_DDLEN	0xA05F7868
#define SB_DDDIR	0xA05F786C
#define SB_DDTSEL	0xA05F7870
#define SB_G2DSTO	0xA05F7890
#define SB_G2TRTO	0xA05F7894
#define SB_G2MDMTO	0xA05F7898
#define SB_G2MDMW	0xA05F789C
#define SB_G2APRO	0xA05F78BC

/* system bus: the DMA to the graphics chip */
#define SB_PDSTAP	0xA05F7C00
#define SB_PDSTAR	0xA05F7C04
#define SB_PDLEN	0xA05F7C08
#define SB_PDDIR	0xA05F7C0C
#define SB_PDTSEL	0xA05F7C10
#define SB_PDEN		0xA05F7C14
#define SB_PDST		0xA05F7C18
#define SB_PDAPRO	0xA05F7C80

/* graphics chip: the render's lists, the frame buffers and the sync generator */
#define PVR_PARAM_BASE		0xA05F8020
#define PVR_REGION_BASE		0xA05F802C
#define PVR_VO_BORDER_COL	0xA05F8040
#define PVR_FB_R_CTRL		0xA05F8044
#define PVR_FB_W_CTRL		0xA05F8048
#define PVR_FB_R_SOF1		0xA05F8050
#define PVR_FB_R_SOF2		0xA05F8054
#define PVR_FB_R_SIZE		0xA05F805C
#define PVR_FB_W_SOF1		0xA05F8060
#define PVR_FB_W_SOF2		0xA05F8064
#define PVR_SPG_TRIGGER_POS	0xA05F80C4
#define PVR_SPG_VBLANK		0xA05F80DC
#define PVR_VO_CONTROL		0xA05F80E8
#define PVR_SPG_STATUS		0xA05F810C

/* graphics chip: the tile accelerator */
#define PVR_TA_OL_BASE		0xA05F8124
#define PVR_TA_ISP_BASE		0xA05F8128
#define PVR_TA_OL_LIMIT		0xA05F812C
#define PVR_TA_ISP_LIMIT	0xA05F8130
#define PVR_TA_NEXT_OPB		0xA05F8134
#define PVR_TA_ITP_CURRENT	0xA05F8138
#define PVR_TA_YUV_TEX_BASE	0xA05F8148
#define PVR_TA_YUV_TEX_CTRL	0xA05F814C
#define PVR_TA_YUV_TEX_CNT	0xA05F8150

#endif
