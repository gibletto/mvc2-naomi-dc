/* Blackheart (character 35): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl35_at_ for its own (pl35_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 9 dm_freeze,
             10 dm_away, 12 dm_snap).
             NO_VEC, NO_ZU and FLY_SCREEN change the knock-back
     kind    how it must be guarded (A_NOM either way, A_HI standing, A_LO crouching, A_GUARD_NG
             not at all), and A_SPECIAL or A_HYPER for those moves
     elem    the strength (K_S, K_M, K_L; K_LS, K_LM and K_LL strike low) and the element
             (K_ELEM_MASK)
     piyo    stun points
     stop    the hit stop for both sides
     sand    frames before an air recovery
     sleep   the down time
     vec     the knock-back's direction, in 32 steps
     pow     the knock-back's strength
     range   the attack's reach across (not read)
     mark    the hit mark (MARK_N1 to MARK_N3 are the plain ones), 0x80 and above for none
     score   SC100 to SC300 (not read)
     se      the hit's sound (SD_NOR_ a punch's, SD_KIK_ a kick's)
     shc     the hyper combo gauge it gives
     jug     what it adds to an armour's hit count
     box, box2  boxes of hit_35 that the computer measures the attack's reach by
     flag    extra rules (NO_CHIP to AERIAL_FINISH)
     guard   the guard's frames, 0 to go by the strength
     catch   frames the victim can't be thrown after
     abyss   for the Abyss's armour, in place of jug
     cnt     the hits it counts for, 0 for one
     answer  what a computer opponent does when it sees the attack coming: a record of its own
             yokXX.c (YK_GD030 and YK_GD060 guard)
     13, 15, 1a  bytes the program is not seen to read */
#include "player.h"
#include "hit_equ.h"

const HIT_DATA hit_dt_35[] = {
	/* dmg                   type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   6,           ZU | NO_VEC,            A_NOM,  K_S,    4,    8,   12,     0, 0x1c,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   8,         BU_O | NO_VEC,            A_NOM,  K_M,    2,   10,   16,     0, 0x1f,  12,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  27,   28, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{   2,                  BU_O,            A_NOM,  K_L,    1,   10,   12,     0, 0x1e,  10,  0x80, MARK_N2, SC200, SD_NOR_L,   0,  50,  20,  123,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   6,           ZU | NO_VEC,            A_NOM,  K_S,    4,    8,   12,     0, 0x1c,   6,  0x60, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   9,                    ZU,            A_NOM,  K_M,    2,   12,   16,     0, 0x1a,   7,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{   4,                    ZU,            A_NOM,  K_L,    1,   10,   12,     0, 0x1a,   7,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  50,  40,   39,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,           ZU | NO_VEC,            A_NOM, K_LS,    4,    6,   12,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   9,                  BU_O,            A_NOM, K_LM,    2,   12,   16,     0, 0x1d,   9,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  81,   82,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{   4,                    ZU,            A_NOM, K_LL,    1,   10,   12,     0, 0x1d,  10,  0x80, MARK_N3, SC200, SD_NOR_L,   0,  50, 116,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,           ZU | NO_VEC,             A_LO, K_LS,    4,    8,   12,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   9,                    ZU,            A_NOM, K_LM,    2,   12,   16,     0, 0x1d,   7,  0x40, MARK_N2, SC200, SD_NOR_M,   0,  40,  46,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  16,                    ZU,            A_NOM, K_LL,    1,   10,   12,     0, 0x1d,   7,  0x40, MARK_N3, SC200, SD_NOR_L,   0,  60, 129,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   6,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   12,     0, 0x00,   2,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  47,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   9,                    ZU,            A_NOM,    3,    1,   16,   12,     0, 0x18,   2,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  49,   50,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  16,                    ZU,            A_NOM,  K_L,    1,    8,   12,     0, 0x18,   4,  0x80, MARK_N3, SC200, SD_NOR_L,   0,  60,  71,   72,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   6,           ZU | NO_VEC,             A_HI,  K_S,    1,   12,   12,     0, 0x00,   2,  0x60, MARK_N1, SC100, SD_NOR_S,   0,  20,  51,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   9,                    ZU,             A_HI,  K_M,    1,   12,   16,     0, 0x16,   2,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  53,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  16,                    ZU,            A_NOM,  K_L,    1,    8,   12,     0, 0x1e,   7,  0x60, MARK_N3, SC200, SD_NOR_L,   0,  20,  77,   78,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   3,          BU_O | NO_ZU,            A_NOM,  K_S,    1,   10,   16,     0, 0x1e,   5,  0x80, MARK_N1, SC300, SD_NOR_S,   0,  10,  94,   59,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 12: pl35_at_fiend_hp, shl35_04 type 0: the five fiends of standing HP */
	{   4,             9 | NO_ZU,            A_NOM,    5,    0,    8,   16,     0, 0x19,  10,  0x80, MARK_N1, SC300, SD_NOR_S,   0,  10,  61,   65,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 13: pl35_at_fiend_hk, shl35_04 type 3: the six fiends of standing HK */
	{   3,          BU_O | NO_ZU,            A_NOM, K_LL,    1,    8,   16,     0, 0x1e,   5,  0x80, MARK_N1, SC300, SD_NOR_S,   0,  10,  68,    0,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 14: pl35_at_fiend_chp, shl35_04 type 1: the three fiends of crouching HP */
	{   4,             9 | NO_ZU,            A_NOM,    5,    1,    8,   16,    15, 0x18,  10,  0x70, MARK_N1, SC300, SD_NOR_S,   0,  50,  70,    0,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 15: pl35_at_fiend_chk, shl35_04 type 4: the fiend of crouching HK */
	{   3,          BU_O | NO_ZU,            A_NOM,  K_S,    1,   10,   16,     0, 0x1e,   5,  0x80, MARK_N1, SC300, SD_NOR_S,   0,  10,  72,   74,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 16: pl35_at_fiend_jhp, shl35_04 type 2: the three fiends of jumping HP */
	{   4,             9 | NO_ZU,            A_NOM,    5,    1,   10,   16,     0, 0x00,   2,  0x80, MARK_N1, SC300, SD_NOR_S,   0,  10,  76,   78,                 NO_CHIP,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 17: pl35_at_fiend_jhk, shl35_04 type 5: the five fiends of jumping HK */
	{ 0 },	/* 18 */
	{   4,           ZU | NO_VEC,            A_NOM,  K_S,    4,   10,    8,     0, 0x1c,  10,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  20,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19 */
	{   4,           ZU | NO_VEC,            A_NOM, K_LS,    2,    6,    8,     0, 0x18,  10,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1a */
	{   4,           ZU | NO_VEC,            A_NOM,  K_S,    1,    8,   12,     0, 0x1c,  12,  0x60, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1b */
	{   4,           ZU | NO_VEC,             A_LO, K_LS,    4,    8,   12,     0, 0x18,  12,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1c */
	{  14,           ZU | NO_VEC,            A_NOM,  K_M,    2,   10,   16,     0, 0x1c,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  27,   28,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                     2,     A_NOM | 0x40,    0,    4,   16,    0,     0, 0x18,  12,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl35_at_throw_p, Punch throw (p_catch_throw) */
	{  16,                     2,     A_NOM | 0x40,    3,    4,   16,    0,     0, 0x19,  10,  0x80, MARK_N1, SC300,       22,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl35_at_throw_k, Kick throw (k_catch_wait) */
	{  16,                     2,     A_NOM | 0x40,    3,    4,    8,    0,     0, 0x18,  10,  0x80, MARK_N1, SC300,       22,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl35_at_air_throw, Air throw (j_catch_wait) */
	{ 0 },	/* 23 */
	{ 0 },	/* 24 */
	{ 0 },	/* 25 */
	{ 0 },	/* 26 */
	{   4,          BU_O | NO_ZU,          A_HYPER,  K_L,    0,   10,   16,    15, 0x1c,   4,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,    0,                 NO_CHIP,  1,     0, 15,     0,     0,   6, YK_GD030, { 0, 0 } },	/* 27: pl35_at_judgement_1, Judgement Day fiend on the ground (shl35_04 type 10), odd frames */
	{   4,            ZU | NO_ZU,          A_HYPER,  K_L,    0,   10,   16,    15, 0x16,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,    0,                 NO_CHIP,  1,     0, 15,     0,     0,   6, YK_GD030, { 0, 0 } },	/* 28: pl35_at_jjudgement_1, Judgement Day fiend in the air (type 11), odd frames */
	{   4,          BU_O | NO_ZU,          A_HYPER,  K_L,    0,   10,   16,    15, 0x1c,   4,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,    0,                       0,  1,     0, 15,     0,     0,   6, YK_GD030, { 0, 0 } },	/* 29: pl35_at_judgement_2, Judgement Day fiend on the ground (type 10), even frames */
	{   4,            ZU | NO_ZU,          A_HYPER,  K_L,    0,   10,   16,    15, 0x16,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90,  80,    0,                       0,  1,     0, 15,     0,     0,   6, YK_GD030, { 0, 0 } },	/* 2a: pl35_at_jjudgement_2, Judgement Day fiend in the air (type 11), even frames */
	{   4,             2 | NO_ZU, A_SPECIAL | 0x80,    3,    0,    8,   16,    15, 0x19,  12,  0x80,       3, SC300,       13,   0,  90,  90,   91,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 2b: pl35_at_thunder_bolt, Dark Thunder: the pieces the fiend hands on to (shl35_01 type 0xff) */
	{   6,             2 | NO_ZU, A_SPECIAL | 0x80,    3,    0,    8,   16,    15, 0x19,  12,  0x80,       3, SC300,       13,   0,  90,  90,   91,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 2c: pl35_at_thunder_bolt_counter, Dark Thunder's pieces as the variable counter (team_type 3) */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  10,   10 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    1,    0,    2,    0,     8, 0x00,   8,  0x80,       3, SC300,       13,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   5,        7, { 0, 0 } },	/* 30: pl35_at_inferno_counter, Inferno fiends as the variable counter (team_type 3) */
	{  10,   10 | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    1,    0,    2,    0,     8, 0x00,   8,  0x80,       3, SC300,       13,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   5,        7, { 0, 0 } },	/* 31: pl35_at_inferno, Inferno fiends (shl35_03 types 3 and 4) */
	{  10, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,    1,    0,    2,    0,     0, 0x00,  11,  0x80,       3, SC300,       13,   0,  90,  79,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   5,        7, { 0, 0 } },	/* 32: pl35_at_inferno_assist, Inferno fiends as the partner's assist (team_type 8) */
	{   6,          BU_O | NO_ZU,   A_HYPER | 0x80,    2,    0,   10,   32,    15, 0x00,   2,  0x80,       3, SC300, SD_NOR_L,   0,  30,  80,    0,                       0,  1,     0, 13,     0,     0,  13,        3, { 0, 0 } },	/* 33: pl35_at_heart, Heart of Darkness (sp_heart): the puffs (shl35_00), set every 5 frames */
	{  14,             2 | NO_ZU, A_SPECIAL | 0x80,    3,    0,    8,   16,    15, 0x19,  12,  0x80,       3, SC300,       13,   0,  90,  85,   93,                       0,  1,     0,  0,     0,     0,   3, YK_GD060, { 0, 0 } },	/* 34 */
	{  14,             2 | NO_ZU, A_SPECIAL | 0x80,    3,    0,    8,   16,    15, 0x19,  12,  0x80,       3, SC300,       13,   0,  90,  85,   93,                       0,  1,     0,  0,     0,     0,   3, YK_GD060, { 0, 0 } },	/* 35: pl35_at_thunder_counter, Dark Thunder's fiend as the counter; otherwise 52 light / 54 heavy */
	{  14,             2 | NO_ZU, A_SPECIAL | 0x80,    3,    0,    8,   16,    15, 0x19,  12,  0x80,       3, SC300,       13,   0,  90,  90,   91,                       0,  1,     0,  0,     0,     0,   3, YK_GD030, { 0, 0 } },	/* 36 */
	{  10,             2 | NO_ZU,   A_HYPER | 0x80,  K_L,    0,   10,   16,    15, 0x00,   2,  0x80,       3, SC300, SD_NOR_L,   0,  90, 117,    0,                       0,  1,     0, 14,     0,     0,   6,        3, { 0, 0 } },	/* 37: pl35_at_armageddon_drop, Armageddon (sp_armageddon): the dropped fiend (shl35_02 type 0) */
	{   4,             2 | NO_ZU,   A_HYPER | 0x80,  K_L,    0,    2,   16,    15, 0x00,   1,  0x80,       3, SC300, SD_NOR_L,   0,  90, 118,    0,       NO_WEIGHT | NO_KO,  1,     0, 14,     0,     0,   6,        3, { 0, 0 } },	/* 38: pl35_at_armageddon_bat_2, Armageddon's bats (shl35_02 type 2), the 1-in-4 record */
	{   4,             2 | NO_ZU,   A_HYPER | 0x80,  K_S,    0,    2,   16,    15, 0x00,   1,  0x80,       3, SC300, SD_NOR_L,   0,  90, 118,    0, NO_CHIP | NO_WEIGHT | NO_KO,  1,     0, 14,     0,     0,   6,        3, { 0, 0 } },	/* 39: pl35_at_armageddon_bat, Armageddon's bats (shl35_02 type 2), the usual record */
	{   9,            10 | NO_ZU,   A_HYPER | 0x80,    1,    0,    2,   20,    15, 0x00,  10,  0x80,       3, SC300,       13,   0,  90,  79,    0,    NO_WEIGHT | NO_SCALE,  1,     0, 14,     0,     0,   0,        3, { 0, 0 } },	/* 3a */
	{  18,        2 | FLY_SCREEN,          A_HYPER,    2,    0,    2,    0,     0, 0x1c,  15,  0x80,       3, SC100, SD_NOR_L,   0,  90,  79,    0,    NO_WEIGHT | NO_SCALE,  1,     0, 14,     0,     0,   6,        3, { 0, 0 } },	/* 3b: pl35_at_armageddon_blast, Armageddon: the blast at the dropped fiend's place as the fiends leave (shl35_02 type 3) */
	{   9,            10 | NO_ZU,   A_HYPER | 0x80,    3,    0,    2,    0,    15, 0x00,  10,  0x80,       3, SC300,       13,   0,  90,  79,    0,    NO_WEIGHT | NO_SCALE,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3c */
	{  14,             2 | NO_ZU, A_SPECIAL | 0x80,    3,    0,   10,   16,    15, 0x19,  12,  0x80,       3, SC300,       13,   0,  90,  85,   93,                       0,  1,     0,  0,     0,     0,   3, YK_GD030, { 0, 0 } },	/* 3d */
	{   0,             2 | NO_ZU, A_SPECIAL | 0x80,  K_S,    0,    0,   12,     0, 0x1c,   6,  0x70,       3, SC100, SD_NOR_L,   0,  20,  20,    0,                   NO_KO,  1,     0,  0,     0,     0,   3, YK_GD030, { 0, 0 } },	/* 3e: pl35_at_judgement, Judgement Day, Blackheart himself (judgement_init) */
	{  10,           10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 132,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3f: pl35_at_t_kick, Partner jump-in attack */
	{  15,         BU_O | NO_VEC,            A_NOM,  K_M,    0,   10,   16,     0, 0x1f,  12,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  90,  27,   28,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 40: pl35_at_counter, the gamma assist's attack (sp_gamma) as the variable counter (team_type 3) */
	{  10,   12 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  81,   82,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 41: pl35_at_snapback, Snapback's fiend (shl35_04 type 7, sub_type 1, set by snapback_move) */
	{  15,         BU_O | NO_VEC,            A_NOM,  K_M,    0,   10,   16,     0, 0x1f,  12,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  90,  27,   28,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 42: pl35_at_gamma, the gamma assist's attack (sp_gamma) */
	{  14,         BU_O | NO_VEC,        A_SPECIAL,  K_M,    0,   10,   16,     0, 0x1f,  12,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  27,   28, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 43: pl35_at_upper, Launcher (pl35_upper) */
};
