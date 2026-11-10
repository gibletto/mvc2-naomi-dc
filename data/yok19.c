/* Abyss (second form) (character 19): the computer's answers.
   What it does about an attack that is coming. PLAYER_WORK's cpu_answer_adrs points here;
   em_answer_sel picks a list from it.

   yok19_tbl        a record (CPU_DATA) for each answer. The row of the attack in its owner's hit
                    table (hit_dtXX.c) names the answer; YK_GD030 and YK_GD060 are the first two.
                    A record is the offsets of its sel (twice: no rate is read) and its cmd.
   yok19_sel_AA     32 picks; one is taken at random.
   yok19_cmd_AA     the answer's lists: a pick is a number in this table.
   yok19_cmd_AA_NN  a list, in the commands of cpu_cmd.h. CMD_JUMP and the other jumps name
                    a list of lib19.c. */
#include "player.h"
#include "cpu_cmd.h"

extern const Uint8 *const yok19_cmd_00[], *const yok19_cmd_01[], *const yok19_cmd_02[],
	*const yok19_cmd_03[], *const yok19_cmd_04[], *const yok19_cmd_05[], *const yok19_cmd_06[],
	*const yok19_cmd_07[], *const yok19_cmd_08[];
extern const Uint8 yok19_sel_00[], yok19_sel_01[], yok19_sel_02[], yok19_sel_03[], yok19_sel_04[],
	yok19_sel_05[], yok19_sel_06[], yok19_sel_07[], yok19_sel_08[], yok19_cmd_00_00[],
	yok19_cmd_01_00[], yok19_cmd_02_00[], yok19_cmd_03_00[], yok19_cmd_04_00[], yok19_cmd_05_00[],
	yok19_cmd_06_00[], yok19_cmd_07_00[], yok19_cmd_08_00[];

const CPU_DATA yok19_tbl[] = {
	{ ( Sint32 )yok19_sel_00, ( Sint32 )yok19_sel_00, ( Sint32 )yok19_cmd_00 },	/* YK_GD030 */
	{ ( Sint32 )yok19_sel_01, ( Sint32 )yok19_sel_01, ( Sint32 )yok19_cmd_01 },	/* YK_GD060 */
	{ ( Sint32 )yok19_sel_02, ( Sint32 )yok19_sel_02, ( Sint32 )yok19_cmd_02 },	/* 2 */
	{ ( Sint32 )yok19_sel_03, ( Sint32 )yok19_sel_03, ( Sint32 )yok19_cmd_03 },	/* 3 */
	{ ( Sint32 )yok19_sel_04, ( Sint32 )yok19_sel_04, ( Sint32 )yok19_cmd_04 },	/* 4 */
	{ ( Sint32 )yok19_sel_05, ( Sint32 )yok19_sel_05, ( Sint32 )yok19_cmd_05 },	/* 5 */
	{ ( Sint32 )yok19_sel_06, ( Sint32 )yok19_sel_06, ( Sint32 )yok19_cmd_06 },	/* 6 */
	{ ( Sint32 )yok19_sel_07, ( Sint32 )yok19_sel_07, ( Sint32 )yok19_cmd_07 },	/* 7 */
	{ ( Sint32 )yok19_sel_08, ( Sint32 )yok19_sel_08, ( Sint32 )yok19_cmd_08 },	/* 8 */
};

const Uint8 yok19_sel_00[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_00[] = {
	yok19_cmd_00_00,
};

const Uint8 yok19_sel_01[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_01[] = {
	yok19_cmd_01_00,
};

const Uint8 yok19_sel_02[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_02[] = {
	yok19_cmd_02_00,
};

const Uint8 yok19_sel_03[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_03[] = {
	yok19_cmd_03_00,
};

const Uint8 yok19_sel_04[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_04[] = {
	yok19_cmd_04_00,
};

const Uint8 yok19_sel_05[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_05[] = {
	yok19_cmd_05_00,
};

const Uint8 yok19_sel_06[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_06[] = {
	yok19_cmd_06_00,
};

const Uint8 yok19_sel_07[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_07[] = {
	yok19_cmd_07_00,
};

const Uint8 yok19_sel_08[] = {
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	  0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

const Uint8 *const yok19_cmd_08[] = {
	yok19_cmd_08_00,
};

const Uint8 yok19_cmd_00_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_01_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_02_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_03_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_04_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_05_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_06_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_07_00[] = {
	CMD_EXIT,
};

const Uint8 yok19_cmd_08_00[] = {
	CMD_EXIT,
};
