/* Loading from the disc: a file read at once, and the scenes' and the fight's files read in the
   background through two queues, with a queue of copies between player buffers and one of sound
   memory requests, while the game runs. */
#include "player.h"
#include "shinobi.h"
#include "adx.h"
#include "load.h"

/* Shinobi / Kamui */
extern Sint32 nlsndPackLoadStatus( void ) ;
extern Sint32 nlsndPackFreeBG( Sint32 Port ) ;
extern Sint32 nlsndPackLoad_Open( void *PackAddress, Sint32 Port, Sint32 Size ) ;
extern void *memcpy( void *dst, const void *src, Uint32 n ) ;

extern void vib_stop( void ) ;
extern void net_vsync_wait( void ) ;
extern void system_exit( void ) ;
extern void pack_set( Uint8 *src ) ;
extern Sint32 gd_drv_req ;	/* system.c: the drive is busy */
extern ADXT adxt ;		/* system.c: the music's player */
extern const Sint32 pl_snd_file_tbl[] ;
extern const Sint32 port_tbl[] ;	/* sound.c: the sound memory port of each player work's sounds */
extern Sint32 stg_file_tbl[] ;	/* tex_load.c: the stage's two files, by stage_no */
extern Sint32 stg_obj_tbl[] ;

#define DMA_COPY	0xffff	/* a request that copies one player's data to another's buffer */

/* a file to read in the background, or a copy */
typedef struct DMA_REQ {
	Uint16	kind;		/* 000: 0 data, 1 to 6 a player work's sounds, DMA_COPY a copy */
	Uint16	no;		/* 002: the file; for a copy, the buffer copied from */
	Uint32	dst;		/* 004: where to; for a copy, the buffer copied to */
} DMA_REQ;

/* a file queue's state */
typedef struct DMA_CTRL {
	GDFS	gdfs;		/* 000: the file open */
	Uint32	dst;		/* 004 */
	Sint32	no;		/* 008 */
	Sint32	size;		/* 00c */
	Sint32	sect;		/* 010: its size in sectors */
	Uint32	wait;		/* 014: the frames the drive has been idle */
	Uint8	kind;		/* 018 */
	Uint8	step;		/* 019 */
	Uint8	get;		/* 01a */
	Uint8	put;		/* 01b */
	Uint8	count;		/* 01c: the requests waiting */
} DMA_CTRL;

/* a request to the sound memory: kind 0 frees a port, any other sends data there */
typedef struct SND_REQ {
	Uint16	kind;		/* 000 */
	Uint16	port;		/* 002 */
	Uint32	adrs;		/* 004 */
} SND_REQ;

typedef struct SND_CTRL {
	Sint32	port;		/* 000 */
	Uint32	adrs;		/* 004 */
	Uint8	kind;		/* 008 */
	Uint8	step;		/* 009 */
	Uint8	get;		/* 00a */
	Uint8	put;		/* 00b */
	Uint8	count;		/* 00c */
} SND_CTRL;

/* a copy made a little each frame */
typedef struct COPY_REQ {
	Uint8	*src;		/* 000 */
	Uint8	*dst;		/* 004 */
	Sint32	size;		/* 008 */
} COPY_REQ;

typedef struct COPY_CTRL {
	Uint8	*dst;		/* 000 */
	Uint8	*src;		/* 004 */
	Sint32	size;		/* 008: the bytes left */
	Sint32	done;		/* 00c: the bytes copied */
	Uint8	unk_10;
	Uint8	step;		/* 011 */
	Uint8	get;		/* 012 */
	Uint8	put;		/* 013 */
	Uint8	count;		/* 014 */
} COPY_CTRL;

static Sint32 file_read( char *name, void *adrs, Sint32 a, Sint32 no ) ;
static void adx_wait( void ) ;
static int snd_main( SND_CTRL *ctrl ) ;
static int dma_main( DMA_CTRL *ctrl ) ;
static int copy_main( COPY_CTRL *ctrl ) ;
void load_init( void ) ;
void load_init2( void ) ;
static void dma_req( Uint16 kind, Uint16 no, Uint32 dst ) ;
static void dma_req2( Uint16 kind, Uint16 no, Uint32 dst ) ;
static void snd_req( Uint16 kind, Uint32 adrs, Uint16 port ) ;
static void copy_req( Uint8 *src, Uint8 *dst, Sint32 size ) ;
static void pl_data_req( PLAYER_WORK *pl_w ) ;
static void pl_data_req2( PLAYER_WORK *pl_w ) ;
static void pl_snd_req( void ) ;

/* every file on the disc, by number */
char *file_name_tbl[712] = {
	"AICADRV.BIN", "FONT.BIN", "DEBUG.BIN", "PL00_FAC.BIN", "PL01_FAC.BIN", "PL02_FAC.BIN",
	"PL03_FAC.BIN", "PL04_FAC.BIN", "PL05_FAC.BIN", "PL06_FAC.BIN", "PL07_FAC.BIN", "PL08_FAC.BIN",
	"PL09_FAC.BIN", "PL0A_FAC.BIN", "PL0B_FAC.BIN", "PL0C_FAC.BIN", "PL0D_FAC.BIN", "PL0E_FAC.BIN",
	"PL0F_FAC.BIN", "PL10_FAC.BIN", "PL11_FAC.BIN", "PL12_FAC.BIN", "PL13_FAC.BIN", "PL14_FAC.BIN",
	"PL15_FAC.BIN", "PL16_FAC.BIN", "PL17_FAC.BIN", "PL18_FAC.BIN", "PL19_FAC.BIN", "PL1A_FAC.BIN",
	"PL1B_FAC.BIN", "PL1C_FAC.BIN", "PL1D_FAC.BIN", "PL1E_FAC.BIN", "PL1F_FAC.BIN", "PL20_FAC.BIN",
	"PL21_FAC.BIN", "PL22_FAC.BIN", "PL23_FAC.BIN", "PL24_FAC.BIN", "PL25_FAC.BIN", "PL26_FAC.BIN",
	"PL27_FAC.BIN", "PL28_FAC.BIN", "PL29_FAC.BIN", "PL2A_FAC.BIN", "PL2B_FAC.BIN", "PL2C_FAC.BIN",
	"PL2D_FAC.BIN", "PL2E_FAC.BIN", "PL2F_FAC.BIN", "PL30_FAC.BIN", "PL31_FAC.BIN", "PL32_FAC.BIN",
	"PL33_FAC.BIN", "PL34_FAC.BIN", "PL35_FAC.BIN", "PL36_FAC.BIN", "PL37_FAC.BIN", "PL38_FAC.BIN",
	"PL39_FAC.BIN", "PL3A_FAC.BIN", "ENDDCTEX.BIN", "PL00_VOI.BIN", "PL01_VOI.BIN", "PL02_VOI.BIN",
	"PL03_VOI.BIN", "PL04_VOI.BIN", "PL05_VOI.BIN", "PL06_VOI.BIN", "PL07_VOI.BIN", "PL08_VOI.BIN",
	"PL09_VOI.BIN", "PL0A_VOI.BIN", "PL0B_VOI.BIN", "PL0C_VOI.BIN", "PL0D_VOI.BIN", "PL0E_VOI.BIN",
	"PL0F_VOI.BIN", "PL10_VOI.BIN", "PL11_VOI.BIN", "PL12_VOI.BIN", "PL13_VOI.BIN", "PL14_VOI.BIN",
	"PL15_VOI.BIN", "PL16_VOI.BIN", "PL17_VOI.BIN", "PL18_VOI.BIN", "PL19_VOI.BIN", "PL1A_VOI.BIN",
	"PL1B_VOI.BIN", "PL1C_VOI.BIN", "PL1D_VOI.BIN", "PL1E_VOI.BIN", "PL1F_VOI.BIN", "PL20_VOI.BIN",
	"PL21_VOI.BIN", "PL22_VOI.BIN", "PL23_VOI.BIN", "PL24_VOI.BIN", "PL25_VOI.BIN", "PL26_VOI.BIN",
	"PL27_VOI.BIN", "PL28_VOI.BIN", "PL29_VOI.BIN", "PL2A_VOI.BIN", "PL2B_VOI.BIN", "PL2C_VOI.BIN",
	"PL2D_VOI.BIN", "PL2E_VOI.BIN", "PL2F_VOI.BIN", "PL30_VOI.BIN", "PL31_VOI.BIN", "PL32_VOI.BIN",
	"PL33_VOI.BIN", "PL34_VOI.BIN", "PL35_VOI.BIN", "PL36_VOI.BIN", "PL37_VOI.BIN", "PL38_VOI.BIN",
	"PL39_VOI.BIN", "PL3A_VOI.BIN", "SE_STAF.BIN", "SE_SYUK.BIN", "ADX_S000.BIN", "ADX_S010.BIN",
	"ADX_S020.BIN", "ADX_S030.BIN", "ADX_S040.BIN", "ADX_S050.BIN", "ADX_S060.BIN", "ADX_S070.BIN",
	"ADX_S080.BIN", "ADX_S090.BIN", "ADX_S0A0.BIN", "ADX_S0B0.BIN", "ADX_CAPL.BIN", "ADX_OPEN.BIN",
	"ADX_STAF.BIN", "ADX_SELC.BIN", "ADX_CONT.BIN", "ADX_HERE.BIN", "ADX_OVER.BIN", "ADX_RANK.BIN",
	"ADX_WINS.BIN", "ADX_MENU.BIN", "ERR_MES.BIN", "KEYBOARD.BIN", "VS4.BIN", "SELSTG.BIN",
	"SELTEX.BIN", "SELVMJ.BIN", "SELVMU.BIN", "PL00_WIN.BIN", "PL01_WIN.BIN", "PL02_WIN.BIN",
	"PL03_WIN.BIN", "PL04_WIN.BIN", "PL05_WIN.BIN", "PL06_WIN.BIN", "PL07_WIN.BIN", "PL08_WIN.BIN",
	"PL09_WIN.BIN", "PL0A_WIN.BIN", "PL0B_WIN.BIN", "PL0C_WIN.BIN", "PL0D_WIN.BIN", "PL0E_WIN.BIN",
	"PL0F_WIN.BIN", "PL10_WIN.BIN", "PL11_WIN.BIN", "PL12_WIN.BIN", "PL13_WIN.BIN", "PL14_WIN.BIN",
	"PL15_WIN.BIN", "PL16_WIN.BIN", "PL17_WIN.BIN", "PL1B_WIN.BIN", "PL1C_WIN.BIN", "PL1D_WIN.BIN",
	"PL1E_WIN.BIN", "PL1F_WIN.BIN", "PL20_WIN.BIN", "PL21_WIN.BIN", "PL22_WIN.BIN", "PL23_WIN.BIN",
	"PL24_WIN.BIN", "PL25_WIN.BIN", "PL26_WIN.BIN", "PL27_WIN.BIN", "PL28_WIN.BIN", "PL29_WIN.BIN",
	"PL2A_WIN.BIN", "PL2B_WIN.BIN", "PL2C_WIN.BIN", "PL2D_WIN.BIN", "PL2E_WIN.BIN", "PL2F_WIN.BIN",
	"PL30_WIN.BIN", "PL31_WIN.BIN", "PL32_WIN.BIN", "PL33_WIN.BIN", "PL34_WIN.BIN", "PL35_WIN.BIN",
	"PL36_WIN.BIN", "PL37_WIN.BIN", "PL38_WIN.BIN", "PL39_WIN.BIN", "PL3A_WIN.BIN", "PL00_DAT.BIN",
	"PL01_DAT.BIN", "PL02_DAT.BIN", "PL03_DAT.BIN", "PL04_DAT.BIN", "PL05_DAT.BIN", "PL06_DAT.BIN",
	"PL07_DAT.BIN", "PL08_DAT.BIN", "PL09_DAT.BIN", "PL0A_DAT.BIN", "PL0B_DAT.BIN", "PL0C_DAT.BIN",
	"PL0D_DAT.BIN", "PL0E_DAT.BIN", "PL0F_DAT.BIN", "PL10_DAT.BIN", "PL11_DAT.BIN", "PL12_DAT.BIN",
	"PL13_DAT.BIN", "PL14_DAT.BIN", "PL15_DAT.BIN", "PL16_DAT.BIN", "PL17_DAT.BIN", "PL18_DAT.BIN",
	"PL19_DAT.BIN", "PL1A_DAT.BIN", "PL1B_DAT.BIN", "PL1C_DAT.BIN", "PL1D_DAT.BIN", "PL1E_DAT.BIN",
	"PL1F_DAT.BIN", "PL20_DAT.BIN", "PL21_DAT.BIN", "PL22_DAT.BIN", "PL23_DAT.BIN", "PL24_DAT.BIN",
	"PL25_DAT.BIN", "PL26_DAT.BIN", "PL27_DAT.BIN", "PL28_DAT.BIN", "PL29_DAT.BIN", "PL2A_DAT.BIN",
	"PL2B_DAT.BIN", "PL2C_DAT.BIN", "PL2D_DAT.BIN", "PL2E_DAT.BIN", "PL2F_DAT.BIN", "PL30_DAT.BIN",
	"PL31_DAT.BIN", "PL32_DAT.BIN", "PL33_DAT.BIN", "PL34_DAT.BIN", "PL35_DAT.BIN", "PL36_DAT.BIN",
	"PL37_DAT.BIN", "PL38_DAT.BIN", "PL39_DAT.BIN", "PL3A_DAT.BIN", "S_PL00A.BIN", "S_PL01A.BIN",
	"S_PL02A.BIN", "S_PL03A.BIN", "S_PL04A.BIN", "S_PL05A.BIN", "S_PL06A.BIN", "S_PL07A.BIN",
	"S_PL08A.BIN", "S_PL09A.BIN", "S_PL0AA.BIN", "S_PL0BA.BIN", "S_PL0CA.BIN", "S_PL0DA.BIN",
	"S_PL0EA.BIN", "S_PL0FA.BIN", "S_PL10A.BIN", "S_PL11A.BIN", "S_PL12A.BIN", "S_PL13A.BIN",
	"S_PL14A.BIN", "S_PL15A.BIN", "S_PL16A.BIN", "S_PL17A.BIN", "S_PL18A.BIN", "S_PL19A.BIN",
	"S_PL1AA.BIN", "S_PL1BA.BIN", "S_PL1CA.BIN", "S_PL1DA.BIN", "S_PL1EA.BIN", "S_PL1FA.BIN",
	"S_PL20A.BIN", "S_PL21A.BIN", "S_PL22A.BIN", "S_PL23A.BIN", "S_PL24A.BIN", "S_PL25A.BIN",
	"S_PL26A.BIN", "S_PL27A.BIN", "S_PL28A.BIN", "S_PL29A.BIN", "S_PL2AA.BIN", "S_PL2BA.BIN",
	"S_PL2CA.BIN", "S_PL2DA.BIN", "S_PL2EA.BIN", "S_PL2FA.BIN", "S_PL30A.BIN", "S_PL31A.BIN",
	"S_PL32A.BIN", "S_PL33A.BIN", "S_PL34A.BIN", "S_PL35A.BIN", "S_PL36A.BIN", "S_PL37A.BIN",
	"S_PL38A.BIN", "S_PL39A.BIN", "S_PL3AA.BIN", "S_PL00B.BIN", "S_PL01B.BIN", "S_PL02B.BIN",
	"S_PL03B.BIN", "S_PL04B.BIN", "S_PL05B.BIN", "S_PL06B.BIN", "S_PL07B.BIN", "S_PL08B.BIN",
	"S_PL09B.BIN", "S_PL0AB.BIN", "S_PL0BB.BIN", "S_PL0CB.BIN", "S_PL0DB.BIN", "S_PL0EB.BIN",
	"S_PL0FB.BIN", "S_PL10B.BIN", "S_PL11B.BIN", "S_PL12B.BIN", "S_PL13B.BIN", "S_PL14B.BIN",
	"S_PL15B.BIN", "S_PL16B.BIN", "S_PL17B.BIN", "S_PL18B.BIN", "S_PL19B.BIN", "S_PL1AB.BIN",
	"S_PL1BB.BIN", "S_PL1CB.BIN", "S_PL1DB.BIN", "S_PL1EB.BIN", "S_PL1FB.BIN", "S_PL20B.BIN",
	"S_PL21B.BIN", "S_PL22B.BIN", "S_PL23B.BIN", "S_PL24B.BIN", "S_PL25B.BIN", "S_PL26B.BIN",
	"S_PL27B.BIN", "S_PL28B.BIN", "S_PL29B.BIN", "S_PL2AB.BIN", "S_PL2BB.BIN", "S_PL2CB.BIN",
	"S_PL2DB.BIN", "S_PL2EB.BIN", "S_PL2FB.BIN", "S_PL30B.BIN", "S_PL31B.BIN", "S_PL32B.BIN",
	"S_PL33B.BIN", "S_PL34B.BIN", "S_PL35B.BIN", "S_PL36B.BIN", "S_PL37B.BIN", "S_PL38B.BIN",
	"S_PL39B.BIN", "S_PL3AB.BIN", "S_PL00C.BIN", "S_PL01C.BIN", "S_PL02C.BIN", "S_PL03C.BIN",
	"S_PL04C.BIN", "S_PL05C.BIN", "S_PL06C.BIN", "S_PL07C.BIN", "S_PL08C.BIN", "S_PL09C.BIN",
	"S_PL0AC.BIN", "S_PL0BC.BIN", "S_PL0CC.BIN", "S_PL0DC.BIN", "S_PL0EC.BIN", "S_PL0FC.BIN",
	"S_PL10C.BIN", "S_PL11C.BIN", "S_PL12C.BIN", "S_PL13C.BIN", "S_PL14C.BIN", "S_PL15C.BIN",
	"S_PL16C.BIN", "S_PL17C.BIN", "S_PL18C.BIN", "S_PL19C.BIN", "S_PL1AC.BIN", "S_PL1BC.BIN",
	"S_PL1CC.BIN", "S_PL1DC.BIN", "S_PL1EC.BIN", "S_PL1FC.BIN", "S_PL20C.BIN", "S_PL21C.BIN",
	"S_PL22C.BIN", "S_PL23C.BIN", "S_PL24C.BIN", "S_PL25C.BIN", "S_PL26C.BIN", "S_PL27C.BIN",
	"S_PL28C.BIN", "S_PL29C.BIN", "S_PL2AC.BIN", "S_PL2BC.BIN", "S_PL2CC.BIN", "S_PL2DC.BIN",
	"S_PL2EC.BIN", "S_PL2FC.BIN", "S_PL30C.BIN", "S_PL31C.BIN", "S_PL32C.BIN", "S_PL33C.BIN",
	"S_PL34C.BIN", "S_PL35C.BIN", "S_PL36C.BIN", "S_PL37C.BIN", "S_PL38C.BIN", "S_PL39C.BIN",
	"S_PL3AC.BIN", "S_PL00D.BIN", "S_PL01D.BIN", "S_PL02D.BIN", "S_PL03D.BIN", "S_PL04D.BIN",
	"S_PL05D.BIN", "S_PL06D.BIN", "S_PL07D.BIN", "S_PL08D.BIN", "S_PL09D.BIN", "S_PL0AD.BIN",
	"S_PL0BD.BIN", "S_PL0CD.BIN", "S_PL0DD.BIN", "S_PL0ED.BIN", "S_PL0FD.BIN", "S_PL10D.BIN",
	"S_PL11D.BIN", "S_PL12D.BIN", "S_PL13D.BIN", "S_PL14D.BIN", "S_PL15D.BIN", "S_PL16D.BIN",
	"S_PL17D.BIN", "S_PL18D.BIN", "S_PL19D.BIN", "S_PL1AD.BIN", "S_PL1BD.BIN", "S_PL1CD.BIN",
	"S_PL1DD.BIN", "S_PL1ED.BIN", "S_PL1FD.BIN", "S_PL20D.BIN", "S_PL21D.BIN", "S_PL22D.BIN",
	"S_PL23D.BIN", "S_PL24D.BIN", "S_PL25D.BIN", "S_PL26D.BIN", "S_PL27D.BIN", "S_PL28D.BIN",
	"S_PL29D.BIN", "S_PL2AD.BIN", "S_PL2BD.BIN", "S_PL2CD.BIN", "S_PL2DD.BIN", "S_PL2ED.BIN",
	"S_PL2FD.BIN", "S_PL30D.BIN", "S_PL31D.BIN", "S_PL32D.BIN", "S_PL33D.BIN", "S_PL34D.BIN",
	"S_PL35D.BIN", "S_PL36D.BIN", "S_PL37D.BIN", "S_PL38D.BIN", "S_PL39D.BIN", "S_PL3AD.BIN",
	"S_PL00E.BIN", "S_PL01E.BIN", "S_PL02E.BIN", "S_PL03E.BIN", "S_PL04E.BIN", "S_PL05E.BIN",
	"S_PL06E.BIN", "S_PL07E.BIN", "S_PL08E.BIN", "S_PL09E.BIN", "S_PL0AE.BIN", "S_PL0BE.BIN",
	"S_PL0CE.BIN", "S_PL0DE.BIN", "S_PL0EE.BIN", "S_PL0FE.BIN", "S_PL10E.BIN", "S_PL11E.BIN",
	"S_PL12E.BIN", "S_PL13E.BIN", "S_PL14E.BIN", "S_PL15E.BIN", "S_PL16E.BIN", "S_PL17E.BIN",
	"S_PL18E.BIN", "S_PL19E.BIN", "S_PL1AE.BIN", "S_PL1BE.BIN", "S_PL1CE.BIN", "S_PL1DE.BIN",
	"S_PL1EE.BIN", "S_PL1FE.BIN", "S_PL20E.BIN", "S_PL21E.BIN", "S_PL22E.BIN", "S_PL23E.BIN",
	"S_PL24E.BIN", "S_PL25E.BIN", "S_PL26E.BIN", "S_PL27E.BIN", "S_PL28E.BIN", "S_PL29E.BIN",
	"S_PL2AE.BIN", "S_PL2BE.BIN", "S_PL2CE.BIN", "S_PL2DE.BIN", "S_PL2EE.BIN", "S_PL2FE.BIN",
	"S_PL30E.BIN", "S_PL31E.BIN", "S_PL32E.BIN", "S_PL33E.BIN", "S_PL34E.BIN", "S_PL35E.BIN",
	"S_PL36E.BIN", "S_PL37E.BIN", "S_PL38E.BIN", "S_PL39E.BIN", "S_PL3AE.BIN", "S_PL00F.BIN",
	"S_PL01F.BIN", "S_PL02F.BIN", "S_PL03F.BIN", "S_PL04F.BIN", "S_PL05F.BIN", "S_PL06F.BIN",
	"S_PL07F.BIN", "S_PL08F.BIN", "S_PL09F.BIN", "S_PL0AF.BIN", "S_PL0BF.BIN", "S_PL0CF.BIN",
	"S_PL0DF.BIN", "S_PL0EF.BIN", "S_PL0FF.BIN", "S_PL10F.BIN", "S_PL11F.BIN", "S_PL12F.BIN",
	"S_PL13F.BIN", "S_PL14F.BIN", "S_PL15F.BIN", "S_PL16F.BIN", "S_PL17F.BIN", "S_PL18F.BIN",
	"S_PL19F.BIN", "S_PL1AF.BIN", "S_PL1BF.BIN", "S_PL1CF.BIN", "S_PL1DF.BIN", "S_PL1EF.BIN",
	"S_PL1FF.BIN", "S_PL20F.BIN", "S_PL21F.BIN", "S_PL22F.BIN", "S_PL23F.BIN", "S_PL24F.BIN",
	"S_PL25F.BIN", "S_PL26F.BIN", "S_PL27F.BIN", "S_PL28F.BIN", "S_PL29F.BIN", "S_PL2AF.BIN",
	"S_PL2BF.BIN", "S_PL2CF.BIN", "S_PL2DF.BIN", "S_PL2EF.BIN", "S_PL2FF.BIN", "S_PL30F.BIN",
	"S_PL31F.BIN", "S_PL32F.BIN", "S_PL33F.BIN", "S_PL34F.BIN", "S_PL35F.BIN", "S_PL36F.BIN",
	"S_PL37F.BIN", "S_PL38F.BIN", "S_PL39F.BIN", "S_PL3AF.BIN", "EFKYPOL.BIN", "EFKYTEX.BIN",
	"STG00POL.BIN", "STG00TEX.BIN", "STG01POL.BIN", "STG01TEX.BIN", "STG02POL.BIN", "STG02TEX.BIN",
	"STG03POL.BIN", "STG03TEX.BIN", "STG04POL.BIN", "STG04TEX.BIN", "STG05POL.BIN", "STG05TEX.BIN",
	"STG06POL.BIN", "STG06TEX.BIN", "STG07POL.BIN", "STG07TEX.BIN", "STG08POL.BIN", "STG08TEX.BIN",
	"STG09POL.BIN", "STG09TEX.BIN", "STG0APOL.BIN", "STG0ATEX.BIN", "STG0BPOL.BIN", "STG0BTEX.BIN",
	"STG0CPOL.BIN", "STG0CTEX.BIN", "STG0DPOL.BIN", "STG0DTEX.BIN", "STG0EPOL.BIN", "STG0ETEX.BIN",
	"STG0FPOL.BIN", "STG0FTEX.BIN", "STG10POL.BIN", "STG10TEX.BIN", "DM00POL.BIN", "DM00TEX.BIN",
	"DM01POL.BIN", "DM01TEX.BIN", "DM02POL.BIN", "DM02TEX.BIN", "DM04POL.BIN", "DM04TEX.BIN",
	"DM05POL.BIN", "DM05TEX.BIN", "DM06POL.BIN", "DM06TEX.BIN", "DM07POL.BIN", "DM07TEX.BIN",
	"DM08POL.BIN", "DM08TEX.BIN", "DM09POL.BIN", "DM09TEX.BIN", "DM0APOL.BIN", "DM0ATEX.BIN",
	"DM0BPOL.BIN", "DM0BTEX.BIN", "DM0EPOL.BIN", "DM0ETEX.BIN", "SE_COMN.BIN", "DM08CAB.BIN",
	"DM08CHR.BIN", "DM0CPOL.BIN", "DM0CTEX.BIN", "DM0DPOL.BIN", "DM0DTEX.BIN", "DM0FPOL.BIN",
	"DM0FTEX.BIN", "DM10POL.BIN", "DM10TEX.BIN", "DM11POL.BIN", "DM11TEX.BIN", "DM12POL.BIN",
	"DM12TEX.BIN", "DM13POL.BIN", "DM13TEX.BIN", "DM14POL.BIN", "DM14TEX.BIN", "NOWLOAD0.BIN",
	"NOWLOAD1.BIN", "NOWLOAD2.BIN", "NOWLOAD3.BIN", "S18RM04.BIN", "S20RM04.BIN", "S24RM04.BIN",
	"S26RM04.BIN", "ASK.BIN", "ADX_NETW.BIN", "PALTEX.BIN"
} ;

/* the module's .bss, in address order */
static Uint32 load_speed ;	/* the frame's spare time is divided by it for the 4K blocks to copy */
static Uint32 unk_8C1F8E40 ;
static SND_CTRL snd_ctrl ;
static DMA_CTRL dma_ctrl ;
static COPY_CTRL copy_ctrl ;
static DMA_CTRL dma_ctrl2 ;
static DMA_REQ dma_req_buf[128] ;
static SND_REQ snd_req_buf[16] ;
static COPY_REQ copy_req_buf[8] ;
static DMA_REQ dma_req_buf2[128] ;
static Uint8 load_side ;	/* a bit for each side whose three players' data is in memory */
static Uint8 load_side_req ;	/* the side the second queue is reading */
static Uint32 load_cpu ;	/* the frame's spare time */
/* unused: the arcade's am_load keeps its reads' sizes in the first two */
static Uint32 load_size ;
static Uint32 load_total ;
static Sint32 unk_8C1F979C ;

/*==============================================================*/
/*			Read a whole file to adrs, trying again until		*/
/*			it reads; returns its size							*/
/*==============================================================*/
static Sint32 file_read( char *name, void *adrs, Sint32 a, Sint32 no )
{
	GDFS gdfs ;
	Sint32 size ;
	Sint32 rc ;
	Sint32 sect ;

	do {
		gd_drv_req = 1 ;
		gdfs = gdFsOpen( name, NULL ) ;
		if( gdfs == NULL ) goto err ;
		gdFsGetFileSize( gdfs, &size ) ;
		sect = ( size + 0x7ff ) >> 11 ;
		rc = gdFsRead( gdfs, sect, adrs ) ;
		gdFsClose( gdfs ) ;
		gd_drv_req = 0 ;
		if( rc == GDD_ERR_TRAYOPEND || rc == GDD_ERR_UNITATTENT ){
err :
			vib_stop() ;
			nlWaitVSync() ;
			exit_req = 2 ;
			net_vsync_wait() ;
			system_exit() ;
			return -1 ;
		}
	} while( rc != 0 ) ;
	return size ;
}

/****************************************************************/
Uint32 file_load( Sint32 no, void *adrs, Sint32 a )
{
	Uint32 size ;

	adx_wait() ;
	size = file_read( file_name_tbl[no], adrs, a, no ) ;
	return size ;
}

/****************************************************************/
Uint32 file_load2( Sint32 no, void *adrs, Sint32 a )
{
	Uint32 size ;

	adx_wait() ;
	size = file_read( file_name_tbl[no], adrs, a, no ) ;
	return size ;
}

/****************************************************************/
/*			Without the wait for the music						*/
/****************************************************************/
Uint32 file_load3( Sint32 no, void *adrs, Sint32 a )
{
	return file_read( file_name_tbl[no], adrs, a, no ) ;
}

/****************************************************************/
/*			While the music plays, wait while its buffer is		*/
/*			full enough											*/
/****************************************************************/
void adx_wait2( void )
{
	Sint32 st ;
	Sint32 i ;

	if( adxt == ( ADXT )-1 ) return ;
	st = ADXT_GetStat( adxt ) ;
	if( st == ADXT_STAT_STOP || st == ADXT_STAT_PLAYEND ) return ;
	for( i = 0 ; i < 0x200000 ; i ++ ){
		if( !ADXT_IsIbufSafety( adxt ) ) return ;
		st = ADXT_GetStat( adxt ) ;
		if( st == ADXT_STAT_STOP || st == ADXT_STAT_PLAYEND ) return ;
	}
}

/*==============================================================*/
/*			While the music plays, wait until its buffer is		*/
/*			full enough, before the drive is taken for a file	*/
/*==============================================================*/
static void adx_wait( void )
{
	Sint32 st ;
	Sint32 i ;

	if( adxt == ( ADXT )-1 ) return ;
	st = ADXT_GetStat( adxt ) ;
	if( st == ADXT_STAT_STOP || st == ADXT_STAT_PLAYEND ) return ;
	for( i = 0 ; i < 0x200000 ; i ++ ){
		if( ADXT_IsIbufSafety( adxt ) ) return ;
		st = ADXT_GetStat( adxt ) ;
		if( st == ADXT_STAT_STOP || st == ADXT_STAT_PLAYEND ) return ;
	}
}

/****************************************************************/
/*			Each frame: one step of the first queue that has	*/
/*			work, the copies when the others are empty			*/
/****************************************************************/
void load_main( void )
{
	DMA_CTRL *ctrl ;
	int rc ;

	if( snd_main( &snd_ctrl ) ) return ;
	if( dma_main( &dma_ctrl ) ) return ;
	ctrl = &dma_ctrl2 ;
	if( ( rc = dma_main( ctrl ) ) ){
		if( rc >= 0 ) return ;
		if( ctrl->count ) return ;
		load_side |= load_side_req ;
		return ;
	}
	if( dma_ctrl.get != dma_ctrl.put ) return ;
	if( snd_ctrl.get != snd_ctrl.put ) return ;
	if( ctrl->get != ctrl->put ) return ;
	copy_main( &copy_ctrl ) ;
}

/*==============================================================*/
/*			The sound memory queue: 1 while it works			*/
/*==============================================================*/
static int snd_main( SND_CTRL *ctrl )
{
	SND_REQ *req ;

	if( nlsndPackLoadStatus() > 0 ) return 0 ;
	switch( snd_ctrl.step ){
	case 0:
		if( ctrl->get == ctrl->put ) return 0 ;
		ctrl->step ++ ;
		req = &snd_req_buf[ctrl->get] ;
		ctrl->kind = req->kind ;
		ctrl->port = req->port ;
		ctrl->adrs = req->adrs ;
	case 1:
		if( !ctrl->kind ) nlsndPackFreeBG( ctrl->port ) ;
		else if( nlsndPackLoad_Open( ( void * )ctrl->adrs, ctrl->port, 0x4000 ) == -1 ) return 0 ;
		ctrl->step = 2 ;
		break ;
	case 2:
		ctrl->step = 0 ;
		ctrl->count -- ;
		ctrl->get ++ ;
		ctrl->get &= 15 ;
		break ;
	}
	return 1 ;
}

/*==============================================================*/
/*			A file queue: 1 while it works, -1 as a file is		*/
/*			read												*/
/*==============================================================*/
static int dma_main( DMA_CTRL *ctrl )
{
	static Uint8 gd_stat = 0 ;	/* the last read's state */
	static Uint8 *pl_adrs_tbl[6] = {
		( Uint8 * )0x0c420000, ( Uint8 * )0x0c810000, ( Uint8 * )0x0c570000,
		( Uint8 * )0x0c960000, ( Uint8 * )0x0c6c0000, ( Uint8 * )0x0cab0000
	} ;
	DMA_REQ *req ;
	Sint32 rc ;

	switch( ctrl->step ){
	case 0:
		if( ctrl->get == ctrl->put ) return 0 ;
		ctrl->step ++ ;
		req = &dma_req_buf[ctrl->get] ;
		if( ctrl != &dma_ctrl ) req = &dma_req_buf2[ctrl->get] ;
		ctrl->kind = req->kind ;
		ctrl->dst = req->dst ;
		ctrl->no = req->no ;
		if( req->kind & 0x80 ){
			copy_req( pl_adrs_tbl[req->no], pl_adrs_tbl[req->dst], 0x150000 ) ;
			ctrl->step = 0 ;
			ctrl->wait = 0 ;
			ctrl->count -- ;
			ctrl->get ++ ;
			ctrl->get &= 0x7f ;
			return 0 ;
		}
	case 1:
		if( ( ctrl->gdfs = gdFsOpen( file_name_tbl[ctrl->no], NULL ) ) == NULL ){
			ctrl->step = 3 ;
			return 0 ;
		}
		if( !gdFsGetFileSize( ctrl->gdfs, &ctrl->size ) ){
			gdFsClose( ctrl->gdfs ) ;
			ctrl->gdfs = NULL ;
			ctrl->step = 3 ;
			return 0 ;
		}
		ctrl->sect = ( ctrl->size + 0x7ff ) >> 11 ;
	case 4:
		rc = gdFsReqRd32( ctrl->gdfs, ctrl->sect, ( void * )ctrl->dst ) ;
		if( rc < 0 ){
			gdFsClose( ctrl->gdfs ) ;
			ctrl->gdfs = NULL ;
			ctrl->step = 3 ;
			return 0 ;
		} else if( rc == 0 ) ctrl->step = 4 ;
		else {
			ctrl->step = 2 ;
			ctrl->wait = 0 ;
		}
		break ;
	case 2:
		gd_stat = rc = gdFsGetStat( ctrl->gdfs ) ;
		switch( rc ){
		case GDD_STAT_IDLE:
			ctrl->wait ++ ;
			if( ctrl->wait > 60 ){
				gdFsClose( ctrl->gdfs ) ;
				ctrl->gdfs = NULL ;
				ctrl->step = 3 ;
				ctrl->wait = 0 ;
			}
			break ;
		case GDD_STAT_COMPLETE:
			gdFsClose( ctrl->gdfs ) ;
			ctrl->gdfs = NULL ;
			ctrl->step = 0 ;
			ctrl->wait = 0 ;
			ctrl->count -- ;
			ctrl->get ++ ;
			ctrl->get &= 0x7f ;
			if( ctrl->kind & 0x7f ) snd_req( ctrl->kind, ctrl->dst, port_tbl[ctrl->kind - 1] ) ;
			return -1 ;
		case GDD_STAT_READ:
			ctrl->wait = 0 ;
			break ;
		case GDD_STAT_ERR:
		default:
			gdFsClose( ctrl->gdfs ) ;
			ctrl->gdfs = NULL ;
			ctrl->step = 3 ;
			break ;
		}
		break ;
	case 3:
		ctrl->step = 0 ;
	default:
		return 0 ;
	}
	return 1 ;
}

/*==============================================================*/
/*			The copy queue: as many 4K blocks as the frame's	*/
/*			spare time allows; 1 while it works					*/
/*==============================================================*/
static int copy_main( COPY_CTRL *ctrl )
{
	COPY_REQ *req ;
	Sint32 size ;
	Sint32 rest ;

	if( stream_start_flag ) return 0 ;
	switch( ctrl->step ){
	case 0:
		if( ctrl->get == ctrl->put ) return 0 ;
		else {
			ctrl->step ++ ;
			req = &copy_req_buf[ctrl->get] ;
			ctrl->dst = req->dst ;
			ctrl->src = req->src ;
			ctrl->size = req->size ;
			ctrl->done = 0 ;
		}
	case 1:
		rest = 15000 - nlSysMetricsCpuPowerGet() ;
		load_cpu = rest ;
		goto calc ;	/* nothing is between them in this build */
	calc:
		size = rest / load_speed ;
		if( size <= 0 ) return 0 ;
		size *= 0x1000 ;
		if( ctrl->size <= size ){
			size = ctrl->size ;
			ctrl->size = 0 ;
			ctrl->step = 0 ;
			ctrl->count -- ;
			ctrl->get ++ ;
			ctrl->get &= 7 ;
		} else ctrl->size -= size ;
		ctrl->done += size ;
		memcpy( ctrl->dst, ctrl->src, size ) ;
		ctrl->src += size ;
		ctrl->dst += size ;
		break ;
	default:
		return 0 ;
	}
	return 1 ;
}

/****************************************************************/
/*			Empty the first file queue, the sound queue and		*/
/*			the copies											*/
/****************************************************************/
void load_init( void )
{
	Uint32 *p ;
	int i ;

	unk_8C1F8E40 = 0 ;
	dma_ctrl.gdfs = NULL ;
	p = &dma_ctrl.dst ;
	for( i = 0 ; i < 7 ; i ++ ) *p++ = 0 ;
	p = ( Uint32 * )&snd_ctrl ;
	for( i = 0 ; i < 4 ; i ++ ) *p++ = 0 ;
	p = ( Uint32 * )&copy_ctrl ;
	for( i = 0 ; i < 6 ; i ++ ) *p++ = 0 ;
	p = ( Uint32 * )dma_req_buf ;
	for( i = 0 ; i < 128 ; i ++ ){
		*p++ = 0 ;
		*p++ = 0 ;
	}
	p = ( Uint32 * )snd_req_buf ;
	for( i = 0 ; i < 16 ; i ++ ){
		*p++ = 0 ;
		*p++ = 0 ;
	}
	p = ( Uint32 * )copy_req_buf ;
	for( i = 0 ; i < 8 ; i ++ ){
		*p++ = 0 ;
		*p++ = 0 ;
		*p++ = 0 ;
	}
	gdFsEntryErrFuncAll( NULL, NULL ) ;
}

/****************************************************************/
/*			Empty the second file queue							*/
/****************************************************************/
void load_init2( void )
{
	Uint32 *p ;
	int i ;

	dma_ctrl2.gdfs = NULL ;
	p = &dma_ctrl2.dst ;
	for( i = 0 ; i < 7 ; i ++ ) *p++ = 0 ;
	p = ( Uint32 * )dma_req_buf2 ;
	for( i = 0 ; i < 128 ; i ++ ){
		*p++ = 0 ;
		*p++ = 0 ;
	}
}

/****************************************************************/
/*			1 when every queue is empty							*/
/****************************************************************/
Sint32 load_ck( void )
{
	if( dma_ctrl.get != dma_ctrl.put ) return 0 ;
	if( dma_ctrl2.get != dma_ctrl2.put ) return 0 ;
	if( snd_ctrl.get != snd_ctrl.put ) return 0 ;
	if( copy_ctrl.get != copy_ctrl.put ) return 0 ;
	return 1 ;
}

/*==============================================================*/
/*			Queue a file to the first queue						*/
/*==============================================================*/
static void dma_req( Uint16 kind, Uint16 no, Uint32 dst )
{
	DMA_REQ *req ;

	dma_ctrl.count ++ ;
	req = &dma_req_buf[dma_ctrl.put ++] ;
	dma_ctrl.put &= 0x7f ;
	req->kind = kind ;
	req->dst = dst ;
	req->no = no ;
}

/*==============================================================*/
/*			And to the second									*/
/*==============================================================*/
static void dma_req2( Uint16 kind, Uint16 no, Uint32 dst )
{
	DMA_REQ *req ;

	dma_ctrl2.count ++ ;
	req = &dma_req_buf2[dma_ctrl2.put ++] ;
	dma_ctrl2.put &= 0x7f ;
	req->kind = kind ;
	req->dst = dst ;
	req->no = no ;
}

/*==============================================================*/
/*			Queue a request to the sound memory					*/
/*==============================================================*/
static void snd_req( Uint16 kind, Uint32 adrs, Uint16 port )
{
	SND_REQ *req ;

	snd_ctrl.count ++ ;
	req = &snd_req_buf[snd_ctrl.put ++] ;
	snd_ctrl.put &= 15 ;
	req->kind = kind ;
	req->port = port ;
	req->adrs = adrs ;
}

/*==============================================================*/
/*			Queue a copy										*/
/*==============================================================*/
static void copy_req( Uint8 *src, Uint8 *dst, Sint32 size )
{
	COPY_REQ *req ;

	copy_ctrl.count ++ ;
	req = &copy_req_buf[copy_ctrl.put ++] ;
	copy_ctrl.put &= 7 ;
	req->src = src ;
	req->dst = dst ;
	req->size = size ;
}

/****************************************************************/
/*			Stop both file queues, closing their files			*/
/****************************************************************/
void load_stop( void )
{
	if( dma_ctrl.gdfs ) gdFsClose( dma_ctrl.gdfs ) ;
	if( dma_ctrl2.gdfs ) gdFsClose( dma_ctrl2.gdfs ) ;
	load_init() ;
	load_init2() ;
}

/****************************************************************/
/*			With no human playing, stop the second queue		*/
/****************************************************************/
void load_stop2( void )
{
	if( Sys->play_side ) return ;
	load_side = load_side_req = 0 ;
	if( dma_ctrl2.gdfs ) gdFsClose( dma_ctrl2.gdfs ) ;
	load_init2() ;
}

/****************************************************************/
void load_side_set( Uint8 side )
{
	load_side = side ;
}

/****************************************************************/
/*			Queue a scene's files								*/
/****************************************************************/
void fight_load_start( int no )
{
	static Uint32 pic_adrs0_tbl[6] = {
		0x0c568000, 0x0cc55000, 0x0c6b8000,
		0x0cbf0000, 0x0c808000, 0x0cbf8000
	} ;
	static Uint32 pic_adrs1_tbl[6] = {
		0x0cc55000, 0x0c958000, 0x0c800000,
		0x0caa8000, 0x0c808000, 0x0cbf8000
	} ;
	static Uint32 pic_adrs2_tbl[6] = {
		0x0c568000, 0x0c958000, 0x0c6b8000,
		0x0caa8000, 0x0c808000, 0x0cbf8000
	} ;
	static Uint8 order_tbl[18] = {
		0, 1, 2, 3, 4, 5,  1, 3, 5, 0, 2, 4,  0, 2, 4, 1, 3, 5
	} ;
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *cp_w ;
	Uint8 *p ;
	Uint32 adrs ;
	int i ;
	int side ;

	switch( no ){
	case 0:
		load_init() ;
		dma_req( 0, FILE_TEX( FILE_DM02POL ), TEX_ADRS ) ;
		dma_req( 0, FILE_DM02POL, DM02_KEEP_ADRS ) ;
		dma_req( 0, FILE_TEX( FILE_DM00POL ), DM00_TEX_ADRS ) ;
		dma_req( 0, FILE_DM00POL, DM00_KEEP_ADRS ) ;
		dma_req( 0, FILE_EFKYPOL, EFKY_OBJ_ADRS ) ;
		snd_req( 0, 0, 7 ) ;
		snd_req( 0, 0, 5 ) ;
		snd_req( 0, 0, 3 ) ;
		snd_req( 0, 0, 6 ) ;
		snd_req( 0, 0, 4 ) ;
		snd_req( 0, 0, 2 ) ;
		break ;
	case 1:
		break ;
	case 2:
		dma_req( 0, FILE_TEX( FILE_EFKYPOL ), EFKY_TEX_ADRS ) ;
		break ;
	case 3:
		for( i = 0 ; i < 6 ; i ++ ){
			pl_w = &pl_work[i] ;
			if( load_side & 1 && !( i & 1 ) ) continue ;
			if( load_side & 2 && i & 1 ) continue ;
			p = order_tbl ;
			if( !( i & 1 ) ){
				if( load_side & 2 ) p += 6 ;
				else if( load_side & 1 ) p += 12 ;
			}
			for( ; ; p ++ ){
				if( *p == i ) break ;
				cp_w = &pl_work[*p] ;
				if( pl_w->load_type == cp_w->load_type ){
					dma_req( DMA_COPY, cp_w->pic_buff_no, pl_w->pic_buff_no ) ;
					goto next ;
				}
			}
			pl_data_req( pl_w ) ;
		next: ;
		}
		break ;
	case 4:
		dma_req( 0, stg_file_tbl[stage_no], TEX_ADRS ) ;
		dma_req( 0, FILE_EFKYPOL, EFKY_OBJ_ADRS ) ;
		dma_req( 0, FILE_TEX( FILE_EFKYPOL ), EFKY_TEX2_ADRS ) ;
		for( i = 0 ; i < 6 ; i += 2 ) pl_data_req( &pl_work[i] ) ;
		break ;
	case 5:
		load_init() ;
		dma_req( 0, FILE_TEX( FILE_DM07POL ), TEX_ADRS ) ;
		dma_req( 0, FILE_DM07POL, DM07_KEEP_ADRS ) ;
		for( i = 1 ; i < 6 ; i += 2 ) pl_data_req( &pl_work[i] ) ;
		break ;
	case 6:
		load_init() ;
		dma_req( 0, FILE_TEX( FILE_DM09POL ), TEX_ADRS ) ;
		dma_req( 0, FILE_DM09POL, DM09_KEEP_ADRS ) ;
		break ;
	case 7:
		load_init() ;
		dma_req( 0, FILE_TEX( FILE_DM08POL ), TEX_ADRS ) ;
		dma_req( 0, FILE_DM08POL, DM08_KEEP_ADRS ) ;
		dma_req( 0, FILE_DM08CAB, 0x0c420000 ) ;
		dma_req( 0, FILE_DM08CHR, 0x0c520000 ) ;
		dma_req( 0, FILE_SE_SYUK, 0x0ccde000 ) ;
		break ;
	case 8:
		load_init() ;
	case 12:
		dma_req( 0, FILE_SELTEX, TEX_ADRS ) ;
		dma_req( 0, FILE_DM01POL, DM01_KEEP_ADRS ) ;
		break ;
	case 9:
		dma_req( 0, stg_obj_tbl[stage_no & 31], STG_OBJ_ADRS ) ;
		break ;
	case 10:
		load_init() ;
		pl_snd_req() ;
		break ;
	case 11:
		load_init() ;
		if( load_side & Sys->play_side ) break ;
		side = 0 ;
		if( Sys->play_side == 2 ) side = 1 ;
		for( i = side ; i < 6 ; i += 2 ){
			pl_w = &pl_work[i] ;
			pl_data_req( pl_w ) ;
			dma_req( 0, pic_file_tbl[pl_w->load_type], pic_adrs2_tbl[pl_w->pic_buff_no] ) ;
		}
		break ;
	case 13:
		side = 0 ;
		goto pic_load ;
	case 14:
		side = 1 ;
	pic_load:
		for( i = side ; i < 6 ; i += 2 ){
			pl_w = &pl_work[i] ;
			if( 0 ) pl_w->pic_buff_no ;	/* left out of the build */
			if( Sys->tex_buff == ( Uint8 * )TEX_BUFF0 ) adrs = pic_adrs1_tbl[pl_w->pic_buff_no] ;
			else adrs = pic_adrs0_tbl[pl_w->pic_buff_no] ;
			dma_req( 0, pic_file_tbl[pl_w->load_type], adrs ) ;
		}
		break ;
	case 15:
	case 16:
		for( i = no - 15 ; i < 6 ; i += 2 ){
			pl_w = &pl_work[i] ;
			pl_data_req2( pl_w ) ;
		}
	case 24:
		load_side_req = 1 ;
		if( Sys->tex_buff == ( Uint8 * )TEX_BUFF0 ) load_side_req = 2 ;
		break ;
	case 17:
		if( Sys->join_side & 1 ) load_side_req = 1 ;
		else load_side_req = 2 ;
		break ;
	case 18:
		if( load_side_req & 1 ) pl_data_req( &pl_work[0] ) ;
		break ;
	case 20:
		if( load_side_req & 1 ){
			pl_w = &pl_work[2] ;
			if( pl_w->load_type == pl_work[0].load_type ) dma_req( DMA_COPY, pl_work[0].pic_buff_no, pl_w->pic_buff_no ) ;
			else pl_data_req( pl_w ) ;
		}
		break ;
	case 22:
		if( load_side_req & 1 ){
			pl_w = &pl_work[4] ;
			if( pl_w->load_type == pl_work[0].load_type ){
				dma_req( DMA_COPY, pl_work[0].pic_buff_no, pl_w->pic_buff_no ) ;
				load_side |= load_side_req ;
			} else if( pl_w->load_type == pl_work[2].load_type ){
				dma_req( DMA_COPY, pl_work[2].pic_buff_no, pl_w->pic_buff_no ) ;
				load_side |= load_side_req ;
			} else pl_data_req2( pl_w ) ;
		}
		break ;
	case 19:
		if( load_side_req & 2 ) pl_data_req( &pl_work[1] ) ;
		break ;
	case 21:
		if( load_side_req & 2 ){
			pl_w = &pl_work[3] ;
			if( pl_w->load_type == pl_work[1].load_type ) dma_req( DMA_COPY, pl_work[1].pic_buff_no, pl_w->pic_buff_no ) ;
			else pl_data_req( pl_w ) ;
		}
		break ;
	case 23:
		if( load_side_req & 2 ){
			pl_w = &pl_work[5] ;
			if( pl_w->load_type == pl_work[1].load_type ){
				dma_req( DMA_COPY, pl_work[1].pic_buff_no, pl_w->pic_buff_no ) ;
				load_side |= load_side_req ;
			} else if( pl_w->load_type == pl_work[3].load_type ){
				dma_req( DMA_COPY, pl_work[3].pic_buff_no, pl_w->pic_buff_no ) ;
				load_side |= load_side_req ;
			} else pl_data_req2( pl_w ) ;
		}
		break ;
	}
}

/*==============================================================*/
/*			Queue a player's data to its buffer					*/
/*==============================================================*/
static void pl_data_req( PLAYER_WORK *pl_w )
{
	dma_req( 0, pl_w->load_type + FILE_PL_DAT, ( Uint32 )pl_data_adrs_tbl[pl_w->pic_buff_no] ) ;
}

/*==============================================================*/
/*			The same through the second queue					*/
/*==============================================================*/
static void pl_data_req2( PLAYER_WORK *pl_w )
{
	dma_req2( 0, pl_w->load_type + FILE_PL_DAT, ( Uint32 )pl_data_adrs_tbl[pl_w->pic_buff_no] ) ;
}

/*==============================================================*/
/*			Queue the players' sounds, once for each			*/
/*			character in the fight								*/
/*==============================================================*/
static void pl_snd_req( void )
{
	static Uint32 pl_snd_adrs[6] = {
		0x0cc00000, 0x0cc30000, 0x0cc60000, 0x0cc90000, 0x0ccc0000, 0x0ccf0000
	} ;
	Uint8 same[6] ;
	int i ;
	int j ;

	for( i = 0 ; i < 6 ; i ++ ){
		same[i] = i ;
		for( j = 0 ; j < i ; j ++ ){
			if( pl_work[i].load_type == pl_work[j].load_type ){
				same[i] = j ;
				break ;
			}
		}
	}
	for( i = 0 ; i < 6 ; i ++ ){
		if( same[i] == i ) dma_req( i + 1, pl_snd_file_tbl[pl_work[i].load_type], pl_snd_adrs[i] ) ;
	}
	pack_set( same ) ;
}

/****************************************************************/
/*			The copies' speed, 0 for the usual					*/
/****************************************************************/
void load_speed_set( Uint32 spd )
{
	if( !spd ) spd = 0x400 ;
	load_speed = spd ;
}

/****************************************************************/
/*			Each of a side's players uses its own buffer		*/
/****************************************************************/
void pl_data_no_init( int side )
{
	PLAYER_WORK *pl_w ;
	int i ;

	pl_w = &pl_work[side] ;
	for( i = side ; i < 6 ; i += 2 ){
		pl_w->pic_buff_no = i ;
		pl_w += 2 ;
	}
}

/****************************************************************/
void load_dummy( void )
{
}

/****************************************************************/
void load_log_put( Sint32 n, int i )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "am_load.c" End of File	*/
