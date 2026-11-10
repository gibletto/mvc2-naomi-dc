/* Abyss (second form) (character 19): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl19_at_ for its own (pl19_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng).
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
     box, box2  boxes of hit_19 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_19[] = {
	/* dmg                   type                    kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   6,                    ZU,                  A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{  10,                    ZU,                  A_NOM,     K_LM,    2,   11,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  33,   63,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  16,                    ZU,                  A_NOM,      K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   6,                    ZU,                  A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   8,                    ZU,                  A_NOM,      K_M,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  30,  64,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  17,                    ZU,                  A_NOM,      K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  30,  37,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   6,                    ZU,                  A_NOM,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{  11,                    ZU,                  A_NOM,     K_LM,    2,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  14,         BU_O | NO_VEC,                  A_NOM,     K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   8,   40, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   6,                    ZU,                   A_LO,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{  11,                    ZU,                   A_LO,     K_LM,    2,   11,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  15,                    AS,                   A_LO,     K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   5,           ZU | NO_VEC,                   A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   9,           ZU | NO_VEC,                   A_HI,     K_LM,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  45,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,                   A_HI,     K_LM,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  46,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   5,           ZU | NO_VEC,                   A_HI,      K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  54,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   9,           ZU | NO_VEC,                   A_HI,      K_M,    1,   10,   16,     0, 0x16,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  55,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,                   A_HI,      K_S,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,   9,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{ 0 },	/* 20 */
	{ 0 },	/* 21 */
	{ 0 },	/* 22 */
	{ 0 },	/* 23 */
	{ 0 },	/* 24 */
	{ 0 },	/* 25 */
	{ 0 },	/* 26 */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  10,   ZU | NO_VEC | NO_ZU, A_GUARD_NG | A_SPECIAL,      K_L,    0,    0,    0,     0, 0x1a,   1,  0x80,       3, SC200, SD_NOR_M,   0,   8,  48,    0,                       0,  0,     0,  0,     1,     0,   0, YK_GD030, { 0, 0 } },	/* 30: pl19_at_bubble, the bubble that holds whoever it touches (shl19_00) */
	{   8, BU_O | NO_VEC | NO_ZU,                A_HYPER,      K_S,    0,    6,   20,     0, 0x1e,   5,  0x60,       3, SC200, SD_NOR_S,   0,  90,  58,  161,                       0,  1,     0,  4,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 31: pl19_at_storm, Shots in every direction (hyper), Abyss and each shot (shl19_02) */
	{   3,   ZU | NO_VEC | NO_ZU,                A_HYPER, K_LL | 6,    0,    6,   20,     0, 0x1e,  11,  0x60,       3, SC200,        8,   0,  90,  58,  161,     NO_CHIP | NO_WEIGHT,  1,     0,  7,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 32: pl19_at_t_kick, Tag-in kick that leaps off the wall */
	{   2,            ZU | NO_ZU,              A_SPECIAL,      K_L,    0,   10,   30,     0, 0x18,   8,  0x80,       4, SC200,       17,   0, 100,  51,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 33 */
	{   3,            ZU | NO_ZU,              A_SPECIAL,      K_L,    0,   10,   30,     0, 0x18,   8,  0x80,       4, SC200,       18,   0, 100,  52,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 34 */
	{   2,            ZU | NO_ZU,              A_SPECIAL,      K_L,    0,   10,   30,     0, 0x18,   8,  0x80,       4, SC200,       17,   0, 100,  51,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 35 */
	{   3,            ZU | NO_ZU,              A_SPECIAL,      K_L,    0,   10,   30,     0, 0x18,   8,  0x80,       4, SC200,       18,   0, 100,  52,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 36 */
	{  18,             2 | NO_ZU,              A_SPECIAL, K_LL | 6,    0,   10,   20,     0, 0x1c,  14,  0x60,       3, SC200,        8,   0,  90,  58,  161,               NO_WEIGHT,  1,     0,  7,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 37 */
	{  10,             2 | NO_ZU,              A_SPECIAL, K_LL | 6,    0,   10,   20,     0, 0x1c,  14,  0x60,       3, SC200,        8,   0,  90,  58,  161,               NO_WEIGHT,  1,     0,  7,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 38: pl19_at_blast_h, the heavy blast at his side (55 + atck_level, level 1), struck by shl19_01's mark when its type is not 0 */
};
