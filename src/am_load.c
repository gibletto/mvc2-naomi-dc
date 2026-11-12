/* Loading from the ROM board: a file read at once, and the fight's data (the players', their
   sounds' and the stage's) read a little each frame through a queue, while the game runs. */
#include "player.h"
#include "naomi.h"
#include "load.h"

/* Shinobi / Kamui */
extern Sint32 nlsndPackLoadStatus( void ) ;
extern Sint32 nlsndPackFreeBG( Sint32 Port ) ;
extern Sint32 nlsndPackLoad_Open( void *PackAddress, Sint32 Port, Sint32 Size ) ;

extern void col_set( void *col, int no, int cnt ) ;
extern void pack_set( Uint8 *src ) ;
extern const Sint32 pl_snd_file_tbl[] ;
extern const Sint32 port_tbl[] ;	/* sound.c: the sound memory port of each player work's sounds */
extern Sint32 stg_file_tbl[] ;	/* tex_load.c: the stage's two files, by stage_no */
extern Sint32 stg_obj_tbl[] ;

/* the start of a block of data, from the table of offsets at its start */
#define DATA_ADRS( top, n )	( ( Uint8 * )( top ) + ( top )[n] )

#define CHAR_LZ_TBL	( ( Sint32 * )0x0dd00000 )	/* the characters' packed animation data */

typedef CATCH_POS CATCH_TBL[128] ;	/* where it holds each character */

/* the queue's state */
typedef struct LOAD_CTRL {
	Uint8	flag;		/* 000: the queue is running */
	Uint8	unk_01;
	Uint8	dma_get;	/* 002 */
	Uint8	dma_put;	/* 003 */
	Uint8	dma_flag;	/* 004: reads from the ROM are waiting */
	Uint8	snd_flag;	/* 005: sound memory requests are waiting */
	Uint8	snd_get;	/* 006 */
	Uint8	snd_put;	/* 007 */
	Uint32	speed;		/* 008: the frame's spare time is divided by it for the 2K blocks to read */
} LOAD_CTRL;

/* a read from the ROM */
typedef struct DMA_REQ {
	Uint8	flag;		/* 000: in use */
	Uint8	unk_01;
	Uint8	kind;		/* 002: 1 the players' sounds, 0, 2 and 3 other data */
	Uint8	no;		/* 003: the player work it is for */
	Uint32	size;		/* 004: the bytes left */
	Uint32	src;		/* 008: the next ROM address */
	Uint8	*dst;		/* 00c: the next RAM address */
	Uint32	src0;		/* 010: where it started */
	Uint8	*dst0;		/* 014 */
} DMA_REQ;

/* a request to the sound memory: 0 frees a bank, 1 sends data there */
typedef struct SND_REQ {
	Uint8	flag;		/* 000: in use */
	Uint8	step;		/* 001: 1 once it is made */
	Uint8	kind;		/* 002 */
	Uint8	unk_03;
	Uint32	size;		/* 004 */
	Uint32	src;		/* 008 */
	Uint32	dst;		/* 00c: the address, or the bank */
} SND_REQ;

/* the module's .bss, in address order */
Uint32 load_cpu ;	/* the CPU time after the last read */
Uint32 load_size ;	/* the size of the last read */
Uint32 load_total ;	/* the bytes read since the start */
Sint32 unk_0C281B44 ;
Sint32 load_log[9] ;	/* the frames run, the sound driver's answer to the last send, the DMA checks */
Uint8 unk_0C281B6C[0x1c] ;
LOAD_CTRL ld_ctrl ;
SND_REQ snd_req_buf[128] ;
DMA_REQ dma_req_buf[128] ;
Uint8 unk_0C282F94[8] ;

const FILE_DATA stream_tbl[834] = {
	{ 0x00800000, 59264 }, { 0x0080e780, 73792 }, { 0x008207c0, 3232 }, { 0x00821460, 25792 },
	{ 0x00827920, 27488 }, { 0x0082e480, 26016 }, { 0x00834a20, 27168 }, { 0x0083b440, 26496 },
	{ 0x00841bc0, 26048 }, { 0x00848180, 26528 }, { 0x0084e920, 25248 }, { 0x00854bc0, 27168 },
	{ 0x0085b5e0, 27552 }, { 0x00862180, 26816 }, { 0x00868a40, 26560 }, { 0x0086f200, 26976 },
	{ 0x00875b60, 25920 }, { 0x0087c0a0, 26880 }, { 0x008829a0, 27008 }, { 0x00889320, 29088 },
	{ 0x008904c0, 26816 }, { 0x00896d80, 26528 }, { 0x0089d520, 27616 }, { 0x008a4100, 26304 },
	{ 0x008aa7c0, 26976 }, { 0x008b1120, 26496 }, { 0x008b78a0, 27008 }, { 0x008be220, 25760 },
	{ 0x008c46c0, 0 }, { 0x008c46c0, 0 }, { 0x008c46c0, 27744 }, { 0x008cb320, 30976 },
	{ 0x008d2c20, 25216 }, { 0x008d8ea0, 29888 }, { 0x008e0360, 30112 }, { 0x008e7900, 27744 },
	{ 0x008ee560, 31584 }, { 0x008f60c0, 26304 }, { 0x008fc780, 26208 }, { 0x00902de0, 27040 },
	{ 0x00909780, 25792 }, { 0x0090fc40, 31104 }, { 0x009175c0, 25952 }, { 0x0091db20, 26496 },
	{ 0x009242a0, 25856 }, { 0x0092a7a0, 28672 }, { 0x009317a0, 27168 }, { 0x009381c0, 27072 },
	{ 0x0093eb80, 27648 }, { 0x00945780, 25344 }, { 0x0094ba80, 29216 }, { 0x00952ca0, 28448 },
	{ 0x00959bc0, 28512 }, { 0x00960b20, 26912 }, { 0x00967440, 28352 }, { 0x0096e300, 26240 },
	{ 0x00974980, 28384 }, { 0x0097b860, 26912 }, { 0x00982180, 28064 }, { 0x00988f20, 26848 },
	{ 0x0098f800, 26240 }, { 0x00995e80, 30528 }, { 0x0099d5c0, 1475424 }, { 0x00b05920, 84192 },
	{ 0x00b1a200, 130368 }, { 0x00b39f40, 150112 }, { 0x00b5e9a0, 153440 }, { 0x00b84100, 110720 },
	{ 0x00b9f180, 108704 }, { 0x00bb9a20, 152896 }, { 0x00bdef60, 98112 }, { 0x00bf6ea0, 151840 },
	{ 0x00c1bfc0, 137344 }, { 0x00c3d840, 141568 }, { 0x00c60140, 100192 }, { 0x00c788a0, 144000 },
	{ 0x00c9bb20, 74656 }, { 0x00cadec0, 146400 }, { 0x00cd1aa0, 147904 }, { 0x00cf5c60, 154848 },
	{ 0x00d1b940, 153984 }, { 0x00d412c0, 146656 }, { 0x00d64fa0, 153952 }, { 0x00d8a900, 151232 },
	{ 0x00daf7c0, 152640 }, { 0x00dd4c00, 155328 }, { 0x00dfaac0, 136320 }, { 0x00e1bf40, 151584 },
	{ 0x00e40f60, 153664 }, { 0x00e667a0, 147968 }, { 0x00e8a9a0, 155360 }, { 0x00eb0880, 155072 },
	{ 0x00ed6640, 153728 }, { 0x00efbec0, 145696 }, { 0x00f1f7e0, 154464 }, { 0x00f45340, 152224 },
	{ 0x00f6a5e0, 61408 }, { 0x00f795c0, 147648 }, { 0x00f9d680, 147680 }, { 0x00fc1760, 146272 },
	{ 0x00fe52c0, 148512 }, { 0x010096e0, 147552 }, { 0x0102d740, 140800 }, { 0x0104fd40, 133888 },
	{ 0x01070840, 99776 }, { 0x01088e00, 131104 }, { 0x010a8e20, 109088 }, { 0x010c3840, 130144 },
	{ 0x010e34a0, 109024 }, { 0x010fde80, 125056 }, { 0x0111c700, 87968 }, { 0x01131ea0, 152800 },
	{ 0x01157380, 120416 }, { 0x011749e0, 85216 }, { 0x011896c0, 94720 }, { 0x011a08c0, 130816 },
	{ 0x011c07c0, 146880 }, { 0x011e4580, 141728 }, { 0x01206f20, 147968 }, { 0x0122b120, 148320 },
	{ 0x0124f480, 98112 }, { 0x012673c0, 155424 }, { 0x0128d2e0, 93760 }, { 0x012a4120, 123136 },
	{ 0x012c2220, 1351360 }, { 0x0140c0e0, 2025728 }, { 0x015fa9e0, 1261504 }, { 0x0172e9a0, 1562816 },
	{ 0x018ac260, 1219776 }, { 0x019d5f20, 910400 }, { 0x01ab4360, 1498816 }, { 0x01c22220, 1055232 },
	{ 0x01d23c20, 570944 }, { 0x01daf260, 591680 }, { 0x01e3f9a0, 571392 }, { 0x01ecb1a0, 100160 },
	{ 0x01ee38e0, 772608 }, { 0x01fa02e0, 2390400 }, { 0x021e7c60, 1592000 }, { 0x0236c720, 131072 },
	{ 0x0238c720, 131072 }, { 0x023ac720, 31104 }, { 0x023b40a0, 33632 }, { 0x023bc400, 35040 },
	{ 0x023c4ce0, 41824 }, { 0x023cf040, 39104 }, { 0x023d8900, 35776 }, { 0x023e14c0, 33920 },
	{ 0x023e9940, 27904 }, { 0x023f0640, 34400 }, { 0x023f8ca0, 40512 }, { 0x02402ae0, 30656 },
	{ 0x0240a2a0, 37888 }, { 0x024136a0, 31392 }, { 0x0241b140, 33760 }, { 0x02423520, 41696 },
	{ 0x0242d800, 37792 }, { 0x02436ba0, 29952 }, { 0x0243e0a0, 22208 }, { 0x02443760, 31904 },
	{ 0x0244b400, 40544 }, { 0x02455260, 31456 }, { 0x0245cd40, 50080 }, { 0x024690e0, 33312 },
	{ 0x02471300, 43648 }, { 0x0247bd80, 33696 }, { 0x02484120, 25920 }, { 0x0248a660, 20512 },
	{ 0x0248f680, 24992 }, { 0x02495820, 26944 }, { 0x0249c160, 37664 }, { 0x024a5480, 23264 },
	{ 0x024aaf60, 24864 }, { 0x024b1080, 37792 }, { 0x024ba420, 26592 }, { 0x024c0c00, 27328 },
	{ 0x024c76c0, 35552 }, { 0x024d01a0, 32224 }, { 0x024d7f80, 31008 }, { 0x024df8a0, 35424 },
	{ 0x024e8300, 48576 }, { 0x024f40c0, 42944 }, { 0x024fe880, 43424 }, { 0x02509220, 42880 },
	{ 0x025139a0, 40512 }, { 0x0251d7e0, 53696 }, { 0x0252a9a0, 52832 }, { 0x02537800, 49120 },
	{ 0x025437e0, 42880 }, { 0x0254df60, 47008 }, { 0x02559700, 46080 }, { 0x02564b00, 38912 },
	{ 0x0256e300, 40864 }, { 0x025782a0, 40640 }, { 0x02582160, 41792 }, { 0x0258c4a0, 35328 },
	{ 0x02594ea0, 30464 }, { 0x0259c5a0, 518976 }, { 0x0261b0e0, 884384 }, { 0x026f2f80, 600320 },
	{ 0x02785880, 811616 }, { 0x0284bae0, 1152288 }, { 0x02965000, 883968 }, { 0x02a3cd00, 948672 },
	{ 0x02b246c0, 1027360 }, { 0x02c1f3e0, 1054784 }, { 0x02d20c20, 964832 }, { 0x02e0c500, 943744 },
	{ 0x02ef2b80, 991680 }, { 0x02fe4d40, 896896 }, { 0x030bfcc0, 1154688 }, { 0x031d9b40, 1195264 },
	{ 0x032fd840, 1129984 }, { 0x03411640, 1140768 }, { 0x03527e60, 899680 }, { 0x036038c0, 1038176 },
	{ 0x03701020, 1087008 }, { 0x0380a640, 1153920 }, { 0x039241c0, 1150208 }, { 0x03a3cec0, 1002752 },
	{ 0x03b31bc0, 1097984 }, { 0x03c3dcc0, 966368 }, { 0x03d29ba0, 578656 }, { 0x03db7000, 892032 },
	{ 0x03e90c80, 525312 }, { 0x03f11080, 618784 }, { 0x03fa81a0, 474592 }, { 0x0401bf80, 545920 },
	{ 0x040a1400, 1156032 }, { 0x041bb7c0, 1208288 }, { 0x042e27a0, 446592 }, { 0x0434f820, 814464 },
	{ 0x044165a0, 320064 }, { 0x044647e0, 655712 }, { 0x04504940, 747936 }, { 0x045bb2e0, 542944 },
	{ 0x0463fbc0, 549312 }, { 0x046c5d80, 956864 }, { 0x047af740, 1232544 }, { 0x048dc5e0, 1106912 },
	{ 0x049ea9c0, 1052864 }, { 0x04aeba80, 1142400 }, { 0x04c02900, 960800 }, { 0x04ced220, 1040512 },
	{ 0x04deb2a0, 1170304 }, { 0x04f08e20, 1033536 }, { 0x05005360, 1141888 }, { 0x0511bfe0, 1227136 },
	{ 0x05247960, 1053376 }, { 0x05348c20, 1204544 }, { 0x0546ed60, 1209504 }, { 0x05596200, 976480 },
	{ 0x05684860, 938752 }, { 0x05769b60, 978048 }, { 0x058587e0, 1045248 }, { 0x05957ae0, 417376 },
	{ 0x059bd940, 20928 }, { 0x059c2b00, 24576 }, { 0x059c8b00, 21280 }, { 0x059cde20, 28448 },
	{ 0x059d4d40, 28256 }, { 0x059dbba0, 32160 }, { 0x059e3940, 29248 }, { 0x059eab80, 27040 },
	{ 0x059f1520, 29312 }, { 0x059f87a0, 25792 }, { 0x059fec60, 32352 }, { 0x05a06ac0, 31392 },
	{ 0x05a0e560, 36096 }, { 0x05a17260, 21952 }, { 0x05a1c820, 30048 }, { 0x05a23d80, 22368 },
	{ 0x05a294e0, 33184 }, { 0x05a31680, 26208 }, { 0x05a37ce0, 29536 }, { 0x05a3f040, 28320 },
	{ 0x05a45ee0, 31008 }, { 0x05a4d800, 20960 }, { 0x05a529e0, 21920 }, { 0x05a57f80, 24128 },
	{ 0x05a5ddc0, 10944 }, { 0x05a60880, 10464 }, { 0x05a63160, 14400 }, { 0x05a669a0, 23712 },
	{ 0x05a6c640, 31872 }, { 0x05a742c0, 31968 }, { 0x05a7bfa0, 22784 }, { 0x05a818a0, 33056 },
	{ 0x05a899c0, 27328 }, { 0x05a90480, 19008 }, { 0x05a94ec0, 32032 }, { 0x05a9cbe0, 17056 },
	{ 0x05aa0e80, 22432 }, { 0x05aa6620, 23840 }, { 0x05aac340, 17568 }, { 0x05ab07e0, 22560 },
	{ 0x05ab6000, 25472 }, { 0x05abc380, 19840 }, { 0x05ac1100, 34368 }, { 0x05ac9740, 24032 },
	{ 0x05acf520, 36160 }, { 0x05ad8260, 25088 }, { 0x05ade460, 32608 }, { 0x05ae63c0, 22976 },
	{ 0x05aebd80, 32192 }, { 0x05af3b40, 22272 }, { 0x05af9240, 20064 }, { 0x05afe0a0, 33664 },
	{ 0x05b06420, 20416 }, { 0x05b0b3e0, 31328 }, { 0x05b12e40, 19616 }, { 0x05b17ae0, 29120 },
	{ 0x05b1eca0, 27712 }, { 0x05b258e0, 27936 }, { 0x05b2c600, 30528 }, { 0x05b33d40, 2240 },
	{ 0x05b34600, 1600 }, { 0x05b34c40, 1952 }, { 0x05b353e0, 1600 }, { 0x05b35a20, 1536 },
	{ 0x05b36020, 1568 }, { 0x05b36640, 1664 }, { 0x05b36cc0, 3520 }, { 0x05b37a80, 2368 },
	{ 0x05b383c0, 1120 }, { 0x05b38820, 2112 }, { 0x05b39060, 1824 }, { 0x05b39780, 3104 },
	{ 0x05b3a3a0, 1504 }, { 0x05b3a980, 2688 }, { 0x05b3b400, 1248 }, { 0x05b3b8e0, 1696 },
	{ 0x05b3bf80, 1216 }, { 0x05b3c440, 2240 }, { 0x05b3cd00, 1632 }, { 0x05b3d360, 1792 },
	{ 0x05b3da60, 1088 }, { 0x05b3dea0, 1440 }, { 0x05b3e440, 1856 }, { 0x05b3eb80, 2144 },
	{ 0x05b3f3e0, 2144 }, { 0x05b3fc40, 2144 }, { 0x05b404a0, 1920 }, { 0x05b40c20, 1536 },
	{ 0x05b41220, 1536 }, { 0x05b41820, 1856 }, { 0x05b41f60, 1440 }, { 0x05b42500, 1920 },
	{ 0x05b42c80, 1664 }, { 0x05b43300, 1376 }, { 0x05b43860, 960 }, { 0x05b43c20, 1760 },
	{ 0x05b44300, 1792 }, { 0x05b44a00, 1184 }, { 0x05b44ea0, 1728 }, { 0x05b45560, 1536 },
	{ 0x05b45b60, 2464 }, { 0x05b46500, 1664 }, { 0x05b46b80, 1600 }, { 0x05b471c0, 1888 },
	{ 0x05b47920, 1696 }, { 0x05b47fc0, 1888 }, { 0x05b48720, 2656 }, { 0x05b49180, 1856 },
	{ 0x05b498c0, 1472 }, { 0x05b49e80, 1120 }, { 0x05b4a2e0, 1888 }, { 0x05b4aa40, 1184 },
	{ 0x05b4aee0, 1472 }, { 0x05b4b4a0, 2112 }, { 0x05b4bce0, 1376 }, { 0x05b4c240, 1920 },
	{ 0x05b4c9c0, 3520 }, { 0x05b4d780, 1152 }, { 0x05b4dc00, 2048 }, { 0x05b4e400, 2048 },
	{ 0x05b4ec00, 2048 }, { 0x05b4f400, 2048 }, { 0x05b4fc00, 2048 }, { 0x05b50400, 2048 },
	{ 0x05b50c00, 2048 }, { 0x05b51400, 2048 }, { 0x05b51c00, 2048 }, { 0x05b52400, 2048 },
	{ 0x05b52c00, 2048 }, { 0x05b53400, 2048 }, { 0x05b53c00, 2048 }, { 0x05b54400, 2048 },
	{ 0x05b54c00, 2048 }, { 0x05b55400, 2048 }, { 0x05b55c00, 2048 }, { 0x05b56400, 2048 },
	{ 0x05b56c00, 2048 }, { 0x05b57400, 2048 }, { 0x05b57c00, 2048 }, { 0x05b58400, 2048 },
	{ 0x05b58c00, 2048 }, { 0x05b59400, 2048 }, { 0x05b59c00, 2048 }, { 0x05b5a400, 2048 },
	{ 0x05b5ac00, 2048 }, { 0x05b5b400, 2048 }, { 0x05b5bc00, 2048 }, { 0x05b5c400, 2048 },
	{ 0x05b5cc00, 2048 }, { 0x05b5d400, 2048 }, { 0x05b5dc00, 2048 }, { 0x05b5e400, 2048 },
	{ 0x05b5ec00, 2048 }, { 0x05b5f400, 2048 }, { 0x05b5fc00, 2048 }, { 0x05b60400, 2048 },
	{ 0x05b60c00, 2048 }, { 0x05b61400, 2048 }, { 0x05b61c00, 2048 }, { 0x05b62400, 2048 },
	{ 0x05b62c00, 2048 }, { 0x05b63400, 2048 }, { 0x05b63c00, 2048 }, { 0x05b64400, 2048 },
	{ 0x05b64c00, 2048 }, { 0x05b65400, 2048 }, { 0x05b65c00, 2048 }, { 0x05b66400, 2048 },
	{ 0x05b66c00, 2048 }, { 0x05b67400, 2048 }, { 0x05b67c00, 2048 }, { 0x05b68400, 2048 },
	{ 0x05b68c00, 2048 }, { 0x05b69400, 2048 }, { 0x05b69c00, 2048 }, { 0x05b6a400, 2048 },
	{ 0x05b6ac00, 2048 }, { 0x05b6b400, 2688 }, { 0x05b6be80, 2464 }, { 0x05b6c820, 2240 },
	{ 0x05b6d0e0, 2176 }, { 0x05b6d960, 2464 }, { 0x05b6e300, 2624 }, { 0x05b6ed40, 2080 },
	{ 0x05b6f560, 2816 }, { 0x05b70060, 2464 }, { 0x05b70a00, 2048 }, { 0x05b71200, 3552 },
	{ 0x05b71fe0, 2496 }, { 0x05b729a0, 2912 }, { 0x05b73500, 2496 }, { 0x05b73ec0, 2048 },
	{ 0x05b746c0, 1856 }, { 0x05b74e00, 1920 }, { 0x05b75580, 2400 }, { 0x05b75ee0, 2464 },
	{ 0x05b76880, 2080 }, { 0x05b770a0, 2080 }, { 0x05b778c0, 2016 }, { 0x05b780a0, 2112 },
	{ 0x05b788e0, 2144 }, { 0x05b79140, 3584 }, { 0x05b79f40, 1568 }, { 0x05b7a560, 1664 },
	{ 0x05b7abe0, 2784 }, { 0x05b7b6c0, 2272 }, { 0x05b7bfa0, 2272 }, { 0x05b7c880, 2528 },
	{ 0x05b7d260, 2176 }, { 0x05b7dae0, 2592 }, { 0x05b7e500, 2432 }, { 0x05b7ee80, 3168 },
	{ 0x05b7fae0, 2560 }, { 0x05b804e0, 2688 }, { 0x05b80f60, 2272 }, { 0x05b81840, 2016 },
	{ 0x05b82020, 2592 }, { 0x05b82a40, 2176 }, { 0x05b832c0, 1856 }, { 0x05b83a00, 2752 },
	{ 0x05b844c0, 1984 }, { 0x05b84c80, 1856 }, { 0x05b853c0, 1984 }, { 0x05b85b80, 1856 },
	{ 0x05b862c0, 2624 }, { 0x05b86d00, 2016 }, { 0x05b874e0, 1568 }, { 0x05b87b00, 1568 },
	{ 0x05b88120, 2016 }, { 0x05b88900, 1632 }, { 0x05b88f60, 1920 }, { 0x05b896e0, 2016 },
	{ 0x05b89ec0, 2304 }, { 0x05b8a7c0, 2496 }, { 0x05b8b180, 2912 }, { 0x05b8bce0, 3584 },
	{ 0x05b8cae0, 704 }, { 0x05b8cda0, 768 }, { 0x05b8d0a0, 2048 }, { 0x05b8d8a0, 768 },
	{ 0x05b8dba0, 704 }, { 0x05b8de60, 768 }, { 0x05b8e160, 2048 }, { 0x05b8e960, 1056 },
	{ 0x05b8ed80, 768 }, { 0x05b8f080, 704 }, { 0x05b8f340, 1280 }, { 0x05b8f840, 800 },
	{ 0x05b8fb60, 832 }, { 0x05b8fea0, 1280 }, { 0x05b903a0, 800 }, { 0x05b906c0, 1952 },
	{ 0x05b90e60, 704 }, { 0x05b91120, 704 }, { 0x05b913e0, 704 }, { 0x05b916a0, 704 },
	{ 0x05b91960, 704 }, { 0x05b91c20, 736 }, { 0x05b91f00, 704 }, { 0x05b921c0, 704 },
	{ 0x05b92480, 704 }, { 0x05b92740, 704 }, { 0x05b92a00, 704 }, { 0x05b92cc0, 2048 },
	{ 0x05b934c0, 768 }, { 0x05b937c0, 768 }, { 0x05b93ac0, 768 }, { 0x05b93dc0, 704 },
	{ 0x05b94080, 704 }, { 0x05b94340, 2048 }, { 0x05b94b40, 1952 }, { 0x05b952e0, 1952 },
	{ 0x05b95a80, 2016 }, { 0x05b96260, 2016 }, { 0x05b96a40, 768 }, { 0x05b96d40, 704 },
	{ 0x05b97000, 2272 }, { 0x05b978e0, 640 }, { 0x05b97b60, 800 }, { 0x05b97e80, 960 },
	{ 0x05b98240, 768 }, { 0x05b98540, 800 }, { 0x05b98860, 1280 }, { 0x05b98d60, 704 },
	{ 0x05b99020, 672 }, { 0x05b992c0, 704 }, { 0x05b99580, 704 }, { 0x05b99840, 1280 },
	{ 0x05b99d40, 704 }, { 0x05b9a000, 1280 }, { 0x05b9a500, 704 }, { 0x05b9a7c0, 2016 },
	{ 0x05b9afa0, 1952 }, { 0x05b9b740, 1056 }, { 0x05b9bb60, 704 }, { 0x05b9be20, 1088 },
	{ 0x05b9c260, 1216 }, { 0x05b9c720, 1152 }, { 0x05b9cba0, 1728 }, { 0x05b9d260, 1088 },
	{ 0x05b9d6a0, 928 }, { 0x05b9da40, 1024 }, { 0x05b9de40, 1600 }, { 0x05b9e480, 1376 },
	{ 0x05b9e9e0, 1088 }, { 0x05b9ee20, 1024 }, { 0x05b9f220, 1728 }, { 0x05b9f8e0, 1728 },
	{ 0x05b9ffa0, 1056 }, { 0x05ba03c0, 1728 }, { 0x05ba0a80, 2016 }, { 0x05ba1260, 1088 },
	{ 0x05ba16a0, 1088 }, { 0x05ba1ae0, 1024 }, { 0x05ba1ee0, 1088 }, { 0x05ba2320, 1088 },
	{ 0x05ba2760, 1184 }, { 0x05ba2c00, 1088 }, { 0x05ba3040, 1088 }, { 0x05ba3480, 1088 },
	{ 0x05ba38c0, 1088 }, { 0x05ba3d00, 1088 }, { 0x05ba4140, 1248 }, { 0x05ba4620, 1632 },
	{ 0x05ba4c80, 1728 }, { 0x05ba5340, 1344 }, { 0x05ba5880, 1088 }, { 0x05ba5cc0, 1088 },
	{ 0x05ba6100, 1152 }, { 0x05ba6580, 1056 }, { 0x05ba69a0, 992 }, { 0x05ba6d80, 1152 },
	{ 0x05ba7200, 800 }, { 0x05ba7520, 1184 }, { 0x05ba79c0, 1088 }, { 0x05ba7e00, 1216 },
	{ 0x05ba82c0, 768 }, { 0x05ba85c0, 1248 }, { 0x05ba8aa0, 608 }, { 0x05ba8d00, 1280 },
	{ 0x05ba9200, 1632 }, { 0x05ba9860, 3776 }, { 0x05baa720, 1088 }, { 0x05baab60, 1056 },
	{ 0x05baaf80, 1088 }, { 0x05bab3c0, 1088 }, { 0x05bab800, 3776 }, { 0x05bac6c0, 1088 },
	{ 0x05bacb00, 3424 }, { 0x05bad860, 896 }, { 0x05badbe0, 1408 }, { 0x05bae160, 1440 },
	{ 0x05bae700, 1600 }, { 0x05baed40, 1088 }, { 0x05baf180, 1984 }, { 0x05baf940, 1856 },
	{ 0x05bb0080, 2368 }, { 0x05bb09c0, 1856 }, { 0x05bb1100, 1984 }, { 0x05bb18c0, 2048 },
	{ 0x05bb20c0, 2304 }, { 0x05bb29c0, 1632 }, { 0x05bb3020, 2016 }, { 0x05bb3800, 1984 },
	{ 0x05bb3fc0, 1952 }, { 0x05bb4760, 1888 }, { 0x05bb4ec0, 1856 }, { 0x05bb5600, 1664 },
	{ 0x05bb5c80, 2048 }, { 0x05bb6480, 2048 }, { 0x05bb6c80, 1984 }, { 0x05bb7440, 1984 },
	{ 0x05bb7c00, 1984 }, { 0x05bb83c0, 1984 }, { 0x05bb8b80, 1984 }, { 0x05bb9340, 1984 },
	{ 0x05bb9b00, 1984 }, { 0x05bba2c0, 1984 }, { 0x05bbaa80, 1984 }, { 0x05bbb240, 1984 },
	{ 0x05bbba00, 1984 }, { 0x05bbc1c0, 2016 }, { 0x05bbc9a0, 1792 }, { 0x05bbd0a0, 1792 },
	{ 0x05bbd7a0, 1952 }, { 0x05bbdf40, 1984 }, { 0x05bbe700, 1984 }, { 0x05bbeec0, 2368 },
	{ 0x05bbf800, 1952 }, { 0x05bbffa0, 1952 }, { 0x05bc0740, 2400 }, { 0x05bc10a0, 2432 },
	{ 0x05bc1a20, 1952 }, { 0x05bc21c0, 1984 }, { 0x05bc2980, 1696 }, { 0x05bc3020, 1792 },
	{ 0x05bc3720, 1920 }, { 0x05bc3ea0, 2208 }, { 0x05bc4740, 1952 }, { 0x05bc4ee0, 2016 },
	{ 0x05bc56c0, 1920 }, { 0x05bc5e40, 1984 }, { 0x05bc6600, 1824 }, { 0x05bc6d20, 1984 },
	{ 0x05bc74e0, 1984 }, { 0x05bc7ca0, 1920 }, { 0x05bc8420, 1984 }, { 0x05bc8be0, 1888 },
	{ 0x05bc9340, 1568 }, { 0x05bc9960, 1600 }, { 0x05bc9fa0, 2240 }, { 0x05bca860, 1632 },
	{ 0x05bcaec0, 1984 }, { 0x05bcb680, 3136 }, { 0x05bcc2c0, 3136 }, { 0x05bccf00, 3168 },
	{ 0x05bcdb60, 3520 }, { 0x05bce920, 3136 }, { 0x05bcf560, 2624 }, { 0x05bcffa0, 3264 },
	{ 0x05bd0c60, 3072 }, { 0x05bd1860, 2464 }, { 0x05bd2200, 3136 }, { 0x05bd2e40, 3136 },
	{ 0x05bd3a80, 3296 }, { 0x05bd4760, 2208 }, { 0x05bd5000, 2688 }, { 0x05bd5a80, 2080 },
	{ 0x05bd62a0, 2400 }, { 0x05bd6c00, 3136 }, { 0x05bd7840, 3136 }, { 0x05bd8480, 3136 },
	{ 0x05bd90c0, 3136 }, { 0x05bd9d00, 3136 }, { 0x05bda940, 3136 }, { 0x05bdb580, 3136 },
	{ 0x05bdc1c0, 3136 }, { 0x05bdce00, 3136 }, { 0x05bdda40, 3136 }, { 0x05bde680, 3136 },
	{ 0x05bdf2c0, 3168 }, { 0x05bdff20, 3136 }, { 0x05be0b60, 3136 }, { 0x05be17a0, 3136 },
	{ 0x05be23e0, 3136 }, { 0x05be3020, 3136 }, { 0x05be3c60, 3168 }, { 0x05be48c0, 3136 },
	{ 0x05be5500, 3136 }, { 0x05be6140, 3168 }, { 0x05be6da0, 3200 }, { 0x05be7a20, 3136 },
	{ 0x05be8660, 3136 }, { 0x05be92a0, 2784 }, { 0x05be9d80, 3168 }, { 0x05bea9e0, 3200 },
	{ 0x05beb660, 3200 }, { 0x05bec2e0, 3104 }, { 0x05becf00, 1824 }, { 0x05bed620, 3328 },
	{ 0x05bee320, 3136 }, { 0x05beef60, 3168 }, { 0x05befbc0, 3136 }, { 0x05bf0800, 3136 },
	{ 0x05bf1440, 3328 }, { 0x05bf2140, 3136 }, { 0x05bf2d80, 2464 }, { 0x05bf3720, 1088 },
	{ 0x05bf3b60, 2496 }, { 0x05bf4520, 3200 }, { 0x05bf51a0, 3072 }, { 0x05bf5da0, 3136 },
	{ 0x05bf69e0, 2560 }, { 0x05bf73e0, 2048 }, { 0x05bf7be0, 2944 }, { 0x05bf8760, 2784 },
	{ 0x05bf9240, 2560 }, { 0x05bf9c40, 2528 }, { 0x05bfa620, 2944 }, { 0x05bfb1a0, 1920 },
	{ 0x05bfb920, 1984 }, { 0x05bfc0e0, 2560 }, { 0x05bfcae0, 2816 }, { 0x05bfd5e0, 2656 },
	{ 0x05bfe040, 2752 }, { 0x05bfeb00, 1632 }, { 0x05bff160, 1312 }, { 0x05bff680, 1440 },
	{ 0x05bffc20, 2560 }, { 0x05c00620, 2560 }, { 0x05c01020, 2592 }, { 0x05c01a40, 2560 },
	{ 0x05c02440, 2560 }, { 0x05c02e40, 2528 }, { 0x05c03820, 2560 }, { 0x05c04220, 2560 },
	{ 0x05c04c20, 2560 }, { 0x05c05620, 2560 }, { 0x05c06020, 2560 }, { 0x05c06a20, 3328 },
	{ 0x05c07720, 2176 }, { 0x05c07fa0, 2272 }, { 0x05c08880, 2208 }, { 0x05c09120, 2688 },
	{ 0x05c09ba0, 2560 }, { 0x05c0a5a0, 2944 }, { 0x05c0b120, 2080 }, { 0x05c0b940, 2080 },
	{ 0x05c0c160, 2976 }, { 0x05c0cd00, 1984 }, { 0x05c0d4c0, 2336 }, { 0x05c0dde0, 2560 },
	{ 0x05c0e7e0, 3008 }, { 0x05c0f3a0, 1792 }, { 0x05c0faa0, 1984 }, { 0x05c10260, 2592 },
	{ 0x05c10c80, 1920 }, { 0x05c11400, 2336 }, { 0x05c11d20, 1440 }, { 0x05c122c0, 2560 },
	{ 0x05c12cc0, 2368 }, { 0x05c13600, 2560 }, { 0x05c14000, 2560 }, { 0x05c14a00, 1440 },
	{ 0x05c14fa0, 2560 }, { 0x05c159a0, 2336 }, { 0x05c162c0, 1248 }, { 0x05c167a0, 1248 },
#if ROM_BOARD == 2	/* the M2 board has six of the scenes' model files in the program ROM */
	{ 0x05c16c80, 2688 }, { 0x05c17700, 1920 }, { 0x05c17e80, 2560 }, { 0x80397000, 155360 },
#else
	{ 0x05c16c80, 2688 }, { 0x05c17700, 1920 }, { 0x05c17e80, 2560 }, { 0x88000000, 155360 },
#endif
	{ 0x05c3e760, 374784 }, { 0x05c99f60, 174016 }, { 0x05cc4720, 1703936 }, { 0x05e64720, 116416 },
	{ 0x05e80de0, 1703936 }, { 0x06020de0, 168608 }, { 0x0604a080, 1654784 }, { 0x061de080, 194048 },
	{ 0x0620d680, 1703936 }, { 0x063ad680, 190144 }, { 0x063dbd40, 1703936 }, { 0x0657bd40, 126720 },
	{ 0x0659ac40, 1558528 }, { 0x06717440, 186944 }, { 0x06744e80, 1703936 }, { 0x068e4e80, 168320 },
#if ROM_BOARD == 2
	{ 0x0690e000, 1638400 }, { 0x06a9e000, 183488 }, { 0x06acacc0, 1703936 }, { 0x80340000, 91392 },
	{ 0x06c811c0, 122880 }, { 0x8036d000, 124352 }, { 0x06cbd780, 1499136 }, { 0x06e2b780, 48128 },
	{ 0x06e37380, 262144 }, { 0x80357000, 6016 }, { 0x06e78b00, 32768 }, { 0x06e80b00, 42496 },
	{ 0x06e8b100, 349568 }, { 0x06ee0680, 185536 }, { 0x06f0db40, 242912 }, { 0x80378000, 177536 },
	{ 0x06f745a0, 1047936 }, { 0x07074320, 158432 }, { 0x0709ae00, 748224 }, { 0x80359000, 81600 },
#else
	{ 0x0690e000, 1638400 }, { 0x06a9e000, 183488 }, { 0x06acacc0, 1703936 }, { 0x88026440, 91392 },
	{ 0x06c811c0, 122880 }, { 0x8803bda0, 124352 }, { 0x06cbd780, 1499136 }, { 0x06e2b780, 48128 },
	{ 0x06e37380, 262144 }, { 0x8805a560, 6016 }, { 0x06e78b00, 32768 }, { 0x06e80b00, 42496 },
	{ 0x06e8b100, 349568 }, { 0x06ee0680, 185536 }, { 0x06f0db40, 242912 }, { 0x8805b720, 177536 },
	{ 0x06f745a0, 1047936 }, { 0x07074320, 158432 }, { 0x0709ae00, 748224 }, { 0x8808b7e0, 81568 },
#endif
	{ 0x07165780, 262144 }, { 0x071a5780, 2368 }, { 0x071a60c0, 290944 }, { 0x071ed140, 50784 },
	{ 0x071f97a0, 90112 }, { 0x0720f7a0, 3232 }, { 0x07210440, 317664 }, { 0x0725dd20, 925888 },
	{ 0x0733fde0, 2211680 }, { 0x0755bd40, 1048576 }
} ;

/* the coded files' blocks for the M2 board's security chip: each block's passcode, and its size
   coded and decoded; -1 ends a list */
typedef Sint32 PFSB_BLOCK[3] ;

static const Sint32 pfsb_list0[] = {
	15678, 33376, 33372,  26039, 41152, 41148,  22678, 55264, 55260,  5842, 25568, 25564,  -1
} ;
static const Sint32 pfsb_list1[] = { 1170, 50720, 50716,  16714, 40608, 40604,  44953, 52, 48,  -1 } ;
static const Sint32 pfsb_list2[] = { 44428, 60576, 60572,  37437, 38176, 38172,  19045, 25588, 25584,  -1 } ;
static const Sint32 pfsb_list3[] = {
	3757, 58496, 58492,  1170, 50720, 50716,  16714, 40608, 40604,  44953, 27712, 27708,  -1
} ;
static const Sint32 pfsb_list4[] = { 15937, 51488, 51484,  47023, 30088, 30084,  -1 } ;
static const Sint32 pfsb_list5[] = { 15678, 5996, 5992,  -1 } ;

const PFSB_BLOCK *pfsb_list_tbl[6] = {
	( const PFSB_BLOCK * )pfsb_list0, ( const PFSB_BLOCK * )pfsb_list1, ( const PFSB_BLOCK * )pfsb_list2,
	( const PFSB_BLOCK * )pfsb_list3, ( const PFSB_BLOCK * )pfsb_list4, ( const PFSB_BLOCK * )pfsb_list5
} ;

/* where each player work's sounds are read to, before they go to the sound memory */
static Uint32 pl_snd_adrs[6] = {
	0x0db00000, 0x0db30000, 0x0db60000, 0x0db90000, 0x0dbc0000, 0x0dbf0000
} ;

#if ROM_BOARD == 2
/*==============================================================*/
/*			How file no is kept on the M2 board: -1 packed,		*/
/*			0 coded (with its list of blocks), 1 as it is		*/
/*==============================================================*/
static Sint32 file_type_ck( Sint32 no, const PFSB_BLOCK **list )
{
	Sint32 type ;

	type = 0 ;
	switch( no ){
	case FILE_EFKYPOL:
	case FILE_DM01POL:
	case FILE_DM07POL:
		type = -1 ;
		break ;
	case FILE_DM00POL:
		*list = pfsb_list_tbl[1] ;
		break ;
	case FILE_DM09POL:
		*list = pfsb_list_tbl[4] ;
		break ;
	case FILE_DM04POL:
		*list = pfsb_list_tbl[5] ;
		break ;
	default:
		type = 1 ;
		break ;
	}
	return type ;
}

/****************************************************************/
/*			Read a file to adrs, and with a, make the cache		*/
/*			see it; returns its size							*/
/****************************************************************/
Uint32 file_load( Sint32 no, void *adrs, Sint32 a )
{
	const FILE_DATA *file ;
	const PFSB_BLOCK *list ;
	Uint8 *src, *dst ;
	Sint32 size ;
	Sint32 type ;

	rom_dma_flag = 1 ;
	file = &stream_tbl[no] ;
	type = 0 ;
	if( file->adrs & 0x80000000 ){
		dst = adrs ;
		if( type = file_type_ck( no, &list ) ){
		} else {
			/* each block through the security chip's RAM */
			src = ( Uint8 * )PFSB_FILE_ADRS ;
			cache_inv( adrs, file->size ) ;
			nlaReadROMDMA( file->adrs & 0x7fffffff, ( char * )src, file->size, 0 ) ;
			nlaPFSBAccessBank( 0 ) ;
			size = 0 ;
			while( **list != -1 ){
				nlaPFSBDecodeRequest( NLA_PFSB_RAM, ( short * )src, ( ( const Uint32 * )list )[1] ) ;
				nlaPFSBDecodeRead( NLA_PFSB_RAM, ( char * )dst, ( *list )[2], ( *list )[0] ) ;
				dst = dst + ( *list )[2] ;
				src = src + ( *list )[1] ;
				size += ( *list )[2] ;
				list ++ ;
			}
			cache_purge( dst, ( size & ~0x1f ) + 32 ) ;
			rom_dma_flag = 0 ;
			return size ;
		}
	} else {
	}
	if( type < 0 ){
		cache_inv( ( void * )LZ_FILE_ADRS, file->size ) ;
		nlaReadROMDMA( file->adrs & 0x7fffffff, ( char * )LZ_FILE_ADRS, file->size, 0 ) ;
		if( a ) cache_purge( ( void * )LZ_FILE_ADRS, file->size ) ;
		lz_unpack16( ( Sint16 * )LZ_FILE_ADRS, adrs ) ;
		rom_dma_flag = 0 ;
	} else {
		cache_inv( adrs, file->size ) ;
		nlaReadROMDMA( file->adrs & 0x7fffffff, adrs, file->size, 0 ) ;
		if( a ) cache_purge( adrs, file->size ) ;
		rom_dma_flag = 0 ;
	}
	return file->size ;
}

/****************************************************************/
Uint32 file_load2( Sint32 no, void *adrs, Sint32 a )
{
	const FILE_DATA *file ;
	const PFSB_BLOCK *list ;
	Uint8 *src, *dst ;
	Sint32 size ;
	Sint32 type ;

	rom_dma_flag = 1 ;
	file = &stream_tbl[no] ;
	type = 0 ;
	if( file->adrs & 0x80000000 ){
		dst = adrs ;
		if( type = file_type_ck( no, &list ) ){
		} else {
			/* each block through the security chip's RAM */
			src = ( Uint8 * )PFSB_FILE_ADRS ;
			cache_inv( adrs, file->size ) ;
			nlaReadROMDMA( file->adrs & 0x7fffffff, ( char * )src, file->size, 0 ) ;
			nlaPFSBAccessBank( 0 ) ;
			size = 0 ;
			while( **list != -1 ){
				nlaPFSBDecodeRequest( NLA_PFSB_RAM, ( short * )src, ( ( const Uint32 * )list )[1] ) ;
				nlaPFSBDecodeRead( NLA_PFSB_RAM, ( char * )dst, ( *list )[2], ( *list )[0] ) ;
				dst = dst + ( *list )[2] ;
				src = src + ( *list )[1] ;
				size += ( *list )[2] ;
				list ++ ;
			}
			cache_purge( dst, ( size & ~0x1f ) + 32 ) ;
			rom_dma_flag = 0 ;
			return size ;
		}
	} else {
	}
	if( type < 0 ){
		cache_inv( ( void * )LZ_FILE_ADRS, file->size ) ;
		nlaReadROMDMA( file->adrs & 0x7fffffff, ( char * )LZ_FILE_ADRS, file->size, 0 ) ;
		if( a ) cache_purge( ( void * )LZ_FILE_ADRS, file->size ) ;
		lz_unpack16( ( Sint16 * )LZ_FILE_ADRS, adrs ) ;
		rom_dma_flag = 0 ;
	} else {
		cache_inv( adrs, file->size ) ;
		nlaReadROMDMA( file->adrs & 0x7fffffff, adrs, file->size, 0 ) ;
		if( a ) cache_purge( adrs, file->size ) ;
		rom_dma_flag = 0 ;
	}
	return file->size ;
}
#else
/****************************************************************/
/*			Read a file to adrs, and with a, make the cache		*/
/*			see it; returns its size							*/
/****************************************************************/
Uint32 file_load( Sint32 no, void *adrs, Sint32 a )
{
	const FILE_DATA *file ;

	rom_dma_flag = 1 ;
	file = &stream_tbl[no] ;
	cache_inv( adrs, file->size ) ;
	if( file->adrs & 0x80000000 ) nlaNRSDecodeRead( file->adrs & 0x7fffffff, adrs, ( ( file->size >> 5 ) + 1 ) * 32, 0 ) ;
	else nlaReadROMDMA( file->adrs & 0x7fffffff, adrs, file->size, 0 ) ;
	if( a ) cache_purge( adrs, file->size ) ;
	rom_dma_flag = 0 ;
	return file->size ;
}

/****************************************************************/
Uint32 file_load2( Sint32 no, void *adrs, Sint32 a )
{
	const FILE_DATA *file ;

	rom_dma_flag = 1 ;
	file = &stream_tbl[no] ;
	cache_inv( adrs, file->size ) ;
	if( file->adrs & 0x80000000 ) nlaNRSDecodeRead( file->adrs & 0x7fffffff, adrs, ( ( file->size >> 5 ) + 1 ) * 32, 0 ) ;
	else nlaReadROMDMA( file->adrs & 0x7fffffff, adrs, file->size, 0 ) ;
	if( a ) cache_purge( adrs, file->size ) ;
	rom_dma_flag = 0 ;
	return file->size ;
}

#endif

/****************************************************************/
/*			Empty the queue: ld_ctrl's first eight bytes are	*/
/*			cleared as two words								*/
/****************************************************************/
void load_init( void )
{
	Uint32 *p ;
	Sint32 i ;

	p = ( Uint32 * )&ld_ctrl ;
	*p++ = 0 ;
	*p = 0 ;
	for( i = 0 ; i < 128 ; i ++ ){
		dma_req_buf[i].flag = 0 ;
		snd_req_buf[i].flag = 0 ;
	}
}

/*==============================================================*/
static void load_start( void )
{
	ld_ctrl.flag = 1 ;
	load_total = 0 ;
	load_log[0] = 0 ;
}

/*==============================================================*/
/* queue a read from the ROM */
static DMA_REQ *dma_req( Uint8 kind, Uint32 src, Uint32 dst, Uint32 size )
{
	DMA_REQ *req ;

	ld_ctrl.dma_flag = 1 ;
	req = &dma_req_buf[ld_ctrl.dma_put ++] ;
	ld_ctrl.dma_put &= 0x7f ;
	req->flag = 1 ;
	req->unk_01 = 0 ;
	req->kind = kind ;
	req->no = 0 ;
	req->size = size ;
	req->src0 = req->src = src & 0x7fffffff ;
	req->dst0 = req->dst = ( Uint8 * )dst ;
	return req ;
}

/*==============================================================*/
/*			Queue a request to the sound memory					*/
/*==============================================================*/
static void snd_req( Uint8 kind, Uint32 src, Uint32 dst, Uint32 size )
{
	SND_REQ *req ;

	ld_ctrl.snd_flag = 1 ;
	req = &snd_req_buf[ld_ctrl.snd_put ++] ;
	ld_ctrl.snd_put &= 0x7f ;
	req->flag = 1 ;
	req->step = 0 ;
	req->kind = kind ;
	req->unk_03 = 0 ;
	req->size = size ;
	req->src = src & 0x7fffffff ;
	req->dst = dst ;
}

/*==============================================================*/
/*			Make the next sound memory request once the			*/
/*			driver is free, and go on to the next				*/
/*==============================================================*/
static void snd_main( void )
{
	LOAD_CTRL *ctrl ;
	SND_REQ *req ;

	ctrl = &ld_ctrl ;
	if( nlsndPackLoadStatus() > 0 ) return ;
	req = &snd_req_buf[ctrl->snd_get] ;
	if( !req->step ){
		if( !req->kind ) nlsndPackFreeBG( req->dst ) ;
		else if( ( load_log[1] = nlsndPackLoad_Open( ( void * )req->src, req->dst, 0x2000 ) ) == -1 ) return ;
		req->step ++ ;
	} else {
		ctrl->snd_get ++ ;
		ctrl->snd_get &= 0x7f ;
		if( ctrl->snd_get == ctrl->snd_put ) ctrl->snd_flag = 0 ;
	}
}

/*==============================================================*/
/*			Read as much of the next file as the frame has		*/
/*			time for											*/
/*==============================================================*/
static void dma_main( void )
{
	DMA_REQ *req ;
	Sint32 rest ;
	Sint32 size ;

	req = &dma_req_buf[ld_ctrl.dma_get] ;
	rest = 15000 - nlSysMetricsCpuPowerGet() ;
	size = rest / ld_ctrl.speed ;
	if( size <= 0 ) return ;
	size *= 0x800 ;
	if( req->size <= size ){
		size = req->size ;
		req->size = 0 ;
		req->flag = 0 ;
		ld_ctrl.dma_get ++ ;
		ld_ctrl.dma_get &= 0x7f ;
		if( req->kind == 1 ) snd_req( 1, ( Uint32 )req->dst0, port_tbl[req->no], 0 ) ;
		if( ld_ctrl.dma_get == ld_ctrl.dma_put ) ld_ctrl.dma_flag = 0 ;
	} else req->size -= size ;
	cache_purge( req->dst, size ) ;
	nlaReadROMDMA( req->src, ( char * )req->dst, size, 0 ) ;
	cache_inv( req->dst, size ) ;
	req->src += size ;
	req->dst += size ;
	load_size = size ;
	load_cpu = nlSysMetricsCpuPowerGet() ;
	load_total += load_size ;
}

/****************************************************************/
/* each frame: run the queue while the ROM's DMA is free */
void load_main( void )
{
	if( !ld_ctrl.flag ) return ;
	unk_0C281B44 = 300 ;
	load_log[0] ++ ;
	if( !( load_log[2] = nlaCheckWaitROMDMA() ) ) return ;
	if( ld_ctrl.snd_flag ) snd_main() ;
	if( !( load_log[3] = nlaCheckWaitROMDMA() ) ) return ;
	if( ld_ctrl.dma_flag ) dma_main() ;
	if( !ld_ctrl.snd_flag && !ld_ctrl.dma_flag ) ld_ctrl.flag = 0 ;
}

/****************************************************************/
/*			The queue is still running							*/
/****************************************************************/
Sint32 load_ck( void )
{
	return ld_ctrl.flag ;
}

/*==============================================================*/
/*			Queue the six player works' data					*/
/*==============================================================*/
static void pl_data_req( void )
{
	int i ;
	const FILE_DATA *file ;
	PLAYER_WORK *pl_w ;
	Uint8 *adrs ;

	pl_w = pl_work ;
	for( i = 0 ; i < 6 ; i ++, pl_w ++ ){
		file = &stream_tbl[pl_file_tbl[pl_w->load_type]] ;
		adrs = pl_data_adrs_tbl[i] ;
		dma_req( 0, file->adrs, ( Uint32 )adrs, file->size ) ;
	}
}

/****************************************************************/
/*			Set up a player work from its loaded data: the		*/
/*			sprites, colours, animations and the computer's		*/
/*			data												*/
/****************************************************************/
void pl_data_set( PLAYER_WORK *pl_w, int no )
{
	Sint32 *dat ;
	Sint32 *anm ;
	Sint16 *src ;

	dat = ( Sint32 * )pl_data_adrs_tbl[no] ;
	pl_w->pl_data_adrs = dat ;
	pl_w->cell_adrs = DATA_ADRS( dat, 0 ) ;
	pl_w->spr_adrs = DATA_ADRS( dat, 1 ) ;
	pl_w->col_adrs = DATA_ADRS( dat, 2 ) ;
	pl_w->catch_pos_tbl = ( CATCH_TBL * )DATA_ADRS( dat, 3 ) ;
	pl_w->ef_pos_tbl = DATA_ADRS( dat, 4 ) ;
	pl_w->spr_set_buff = spr_set_buff_tbl[no] ;
	pl_w->col_flag = 1 ;
	pl_w->col_no = col_no_tbl[no] ;
	pl_w->col_cnt = col_cnt_tbl[pl_w->load_type] ;
	pl_w->char_ofs = 0x100 ;
	if( pl_w->load_type == 52 ) pl_w->char_ofs = 0x200 ;
	col_set( ( Uint16 * )pl_w->col_adrs + pl_w->load_color * 128, pl_w->col_no, 8 ) ;
	/* the animation data is unpacked 0x130000 bytes on */
	src = ( Sint16 * )DATA_ADRS( CHAR_LZ_TBL, pl_w->load_type ) ;
	lz_unpack16( src, ( Uint16 * )( anm = dat + 0x130000 / 4 ) ) ;
	pl_w->char_adrs = DATA_ADRS( anm, 0 ) ;
	pl_w->frame_box_tbl = ( FRAME_BOX * )DATA_ADRS( anm, 1 ) ;
	pl_w->hit_box_tbl = ( HIT_BOX * )DATA_ADRS( anm, 2 ) ;
	pl_w->hit_data_tbl = ( HIT_DATA * )DATA_ADRS( anm, 3 ) ;
	pl_w->cpu_adrs[0] = ( struct CPU_DATA * )DATA_ADRS( anm, 4 ) ;
	pl_w->cpu_adrs[1] = ( struct CPU_DATA * )DATA_ADRS( anm, 5 ) ;
	pl_w->cpu_answer_adrs = ( struct CPU_DATA * )DATA_ADRS( anm, 7 ) ;
	pl_w->cpu_adrs[2] = ( struct CPU_DATA * )DATA_ADRS( anm, 6 ) ;
	pl_w->cpu_ex_adrs = ( Sint32 * )DATA_ADRS( anm, 8 ) ;
}

/*==============================================================*/
/*			Free the players' sound banks and queue their		*/
/*			sounds, once for each character in the fight		*/
/*==============================================================*/
static void pl_snd_req( void )
{
	Uint8 same[6] ;
	DMA_REQ *req ;
	int i ;
	int j ;

	for( i = 5 ; i >= 0 ; i -- ) snd_req( 0, 0, i + 2, 0 ) ;	/* banks 7 down to 2 */
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
		if( same[i] == i ){
			if( ( req = dma_req( 1, stream_tbl[pl_snd_file_tbl[pl_work[i].load_type]].adrs, pl_snd_adrs[i],
					stream_tbl[pl_snd_file_tbl[pl_work[i].load_type]].size ) ) ) req->no = i ;
		}
	}
	pack_set( same ) ;
}

/*==============================================================*/
/*			Queue the stage's two files							*/
/*==============================================================*/
static void stg_load_req( void )
{
	const FILE_DATA *file ;

	file = &stream_tbl[stg_obj_tbl[stage_no]] ;
	dma_req( 2, file->adrs, 0x0d82d000, file->size ) ;
	file = &stream_tbl[stg_file_tbl[stage_no]] ;
	dma_req( 3, file->adrs, 0x0d85d000, file->size ) ;
}

/****************************************************************/
void load_dummy( void )
{
}

/****************************************************************/
void load_log_put( Sint32 n, int i )
{
	load_log[i] = n ;
}

/****************************************************************/
/*			The queue's reading speed, 0 for the usual			*/
/****************************************************************/
void load_speed_set( Uint32 spd )
{
	if( !spd ) spd = 0x100 ;
	ld_ctrl.speed = spd ;
}

/****************************************************************/
/*			Start the queue, and unless only is set, with		*/
/*			the fight's data									*/
/****************************************************************/
void fight_load_start( int only )
{
	load_init() ;
	load_start() ;
	switch( only ){
	case 0:
		pl_snd_req() ;
		pl_data_req() ;
		stg_load_req() ;
	}
	load_log[8] = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "am_load.c" End of File	*/
