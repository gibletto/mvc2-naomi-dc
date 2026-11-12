/* The scenes' model files: their numbers, and where each is in memory. The board reads every
   file once at start-up, each to a place of its own. The Dreamcast has half the memory: it reads
   a scene's files as the scene starts, the models to one buffer and their textures to another,
   and some it reads ahead to a place they are kept at until their scene copies them. */
#ifndef LOAD_H
#define LOAD_H

#ifdef TARGET_DC

/* a file's number: its place in file_name_tbl */
#define FILE_ENDTEX		0x03e	/* ENDDCTEX */
#define FILE_SE_SYUK	0x07b	/* the attract's sound pack */
#define FILE_KEYBOARD	0x093
#define FILE_SELSTG		0x095
#define FILE_SELTEX		0x096
#define FILE_SELVMJ		0x097
#define FILE_SELVMU		0x098
#define FILE_PL_DAT		0x0d1	/* PLxx_DAT: a character's data, by load_type */
#define FILE_S_PL		0x10c	/* S_PLxxA: a character's program, 59 for each of the six players */
#define FILE_EFKYPOL	0x26e
#define FILE_STG00POL	0x270
#define FILE_DM00POL	0x292
#define FILE_DM01POL	0x294
#define FILE_DM02POL	0x296
#define FILE_DM04POL	0x298
#define FILE_DM05POL	0x29a
#define FILE_DM06POL	0x29c
#define FILE_DM07POL	0x29e
#define FILE_DM08POL	0x2a0
#define FILE_DM09POL	0x2a2
#define FILE_DM0APOL	0x2a4
#define FILE_DM0BPOL	0x2a6
#define FILE_DM0EPOL	0x2a8
#define FILE_DM08CAB	0x2ab
#define FILE_DM08CHR	0x2ac
#define FILE_DM0CPOL	0x2ad
#define FILE_DM0DPOL	0x2af
#define FILE_DM0FPOL	0x2b1
#define FILE_DM10POL	0x2b3
#define FILE_DM11POL	0x2b5
#define FILE_DM12POL	0x2b7
#define FILE_DM13POL	0x2b9
#define FILE_DM14POL	0x2bb
#define FILE_NOWLOAD	0x2bd	/* NOWLOAD0 to NOWLOAD3 */
#define FILE_ASK		0x2c5

/* where a file is read to */
#define TEX_ADRS		0x0cc00000	/* the scene's textures; any file about to be unpacked */
#define SCENE_OBJ_ADRS	0x0ce80000	/* the scene's models */
#define STG_OBJ_ADRS	0x0cea0000	/* the stage's models */
#define EFKY_OBJ_ADRS	0x0ced0000	/* the effects' models */
#define EFKY_TEX_ADRS	0x0cda4000	/* and their textures; in the attract demo at EFKY_TEX2_ADRS */
#define EFKY_TEX2_ADRS	0x0cda0000
#define DM04_OBJ_ADRS	0x0ce08000
#define DM00_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM01_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM02_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM05_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM06_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM07_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM08_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM09_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM0A_OBJ_ADRS	SCENE_OBJ_ADRS
#define DM0B_OBJ_ADRS	EFKY_OBJ_ADRS
#define DM0A_TEX_ADRS	0x0cd00000	/* packed */

/* where a file read ahead is kept (fight_load_start reads them), and its size */
#define DM00_KEEP_ADRS	0x0cd8d000
#define DM00_KEEP_SIZE	0x17000
#define DM00_TEX_ADRS	0x0cd6f000
#define DM01_KEEP_ADRS	0x0cd85000
#define DM01_KEEP_SIZE	0x1f000
#define DM02_KEEP_ADRS	0x0cc49000
#define DM02_KEEP_SIZE	0xbc00
#define DM07_KEEP_ADRS	0x0cd00000
#define DM07_KEEP_SIZE	0x2c000
#define DM08_KEEP_ADRS	0x0ccb7000
#define DM08_KEEP_SIZE	0x27000
#define DM09_KEEP_ADRS	0x0cc40000
#define DM09_KEEP_SIZE	0x14000
#define VS_PIC_ADRS		0x0cdd0000	/* the ranking's three pictures, 0x10000 apart */
#define NOWLOAD_KEEP_ADRS	0x0ce2ea00	/* the "now loading" picture's pack */
#define ASK_ADRS		0x0cd00000	/* the kana-kanji engine */
#define S_PL_ADRS		0x0ce30000	/* the characters' programs, 0x8000 apart */

#else

/* a file's number: its place in stream_tbl */
#define FILE_ENDTEX		0x03e
#define FILE_SELTEX		0x08a
#define FILE_SELVMJ		0x08b
#define FILE_SELVMU		0x08c
#define FILE_EFKYPOL	0x313
#define FILE_STG00POL	0x315
#define FILE_DM00POL	0x327
#define FILE_DM01POL	0x329
#define FILE_DM02POL	0x32b
#define FILE_DM04POL	0x32d
#define FILE_DM05POL	0x32f
#define FILE_DM06POL	0x331
#define FILE_DM07POL	0x333
#define FILE_DM08POL	0x335
#define FILE_DM09POL	0x337
#define FILE_DM0APOL	0x339
#define FILE_DM0BPOL	0x33b
#define FILE_DM0EPOL	0x33d
#define FILE_DM08CAB	0x341

/* where a file is read to */
#define TEX_ADRS		0x0cc00000	/* any file about to be unpacked */
#if ROM_BOARD == 2
#define LZ_FILE_ADRS	0x0de00000	/* the M2 board's packed files, as read */
#define PFSB_FILE_ADRS	0x0df80000	/* and its coded ones */
#define EFKY_OBJ_ADRS	0x0d000000
#else
#define EFKY_OBJ_ADRS	0x0da00000
#endif
#define EFKY_TEX_ADRS	0x0d026000
#define STG_OBJ_ADRS	0x0d82d000
#define DM00_OBJ_ADRS	0x0d082000
#define DM00_TEX_ADRS	0x0d099000
#define DM01_OBJ_ADRS	0x0d0c6000
#define DM01_TEX_ADRS	0x0d0e5000
#define DM02_OBJ_ADRS	0x0d25c000
#define DM02_TEX_ADRS	0x0d268000
#define DM04_OBJ_ADRS	0x0d2b1000
#define DM04_TEX_ADRS	0x0d2b3000
#define DM05_OBJ_ADRS	0x0d7cc000
#define DM05_TEX_ADRS	0x0d7d7000
#define DM06_OBJ_ADRS	0x0d2bb000
#define DM06_TEX_ADRS	0x0d2e9000
#if ROM_BOARD == 2
#define DM07_OBJ_ADRS	0x0d3a2000
#else
#define DM07_OBJ_ADRS	0x0da28000
#endif
#define DM07_TEX_ADRS	0x0d3ce000
#define DM08_OBJ_ADRS	0x0d4ce000
#define DM08_TEX_ADRS	0x0d4f5000
#define DM09_OBJ_ADRS	0x0d6cc000
#define DM09_TEX_ADRS	0x0d6e0000
#define DM0A_OBJ_ADRS	0x0d720000
#define DM0A_TEX_ADRS	0x0d721000	/* packed */
#define DM0B_OBJ_ADRS	0x0d7a9000
#define DM0B_TEX_ADRS	0x0d7b6000

#endif

/* a model file's textures follow its models' file */
#define FILE_TEX( pol )	( ( pol ) + 1 )

/* each side's picture buffer (Sys->tex_buff) */
#define TEX_BUFF0	0x0c420000
#define TEX_BUFF1	0x0c810000

#define PIC_ADRS	0x0ce60000	/* a picture is unpacked to here */

#endif
