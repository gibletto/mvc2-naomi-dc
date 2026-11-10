/* Dan (character 23): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl23_at_ for its own (pl23_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
             10 dm_away, 12 dm_snap, 21 dm_buttobi_ukemi_ng).
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
     box, box2  boxes of hit_23 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_23[] = {
	/* dmg                   type            kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,          A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                    ZU,          A_NOM, K_LM,    2,   10,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                    ZU,          A_NOM,  K_L,    1,   10,   24,     0, 0x1a,   4,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   9,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,          A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   4,                    ZU,          A_NOM,  K_M,    2,   10,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  46,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                    ZU,          A_NOM,  K_L,    1,   10,   24,     0, 0x1c,   4,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  13,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,          A_NOM, K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,          A_NOM, K_LM,    2,   10,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         BU_O | NO_VEC,          A_NOM, K_LL,    1,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  16,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,           A_LO, K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  18,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,           A_LO, K_LM,    2,   10,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,            AS | NO_ZU,           A_LO, K_LL,    1,   10,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  20,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,           A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  21,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,           A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  22,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,           A_HI,  K_M,    1,   10,   24,     0, 0x18,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  23,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,           A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  24,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,           A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  25,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,           A_HI,  K_M,    1,   10,   24,     0, 0x18,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  26,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{ 0 },	/* 12 */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   3,                    ZU,          A_NOM,  K_M,    1,   10,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  12,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                     2,   A_NOM | 0x40, K_LM,    4,   10,   20,     0, 0x07,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl23_at_throw_p, Ground punch throw */
	{  16,         BU_O | NO_VEC,   A_NOM | 0x40, K_LM,    4,   10,   20,    16, 0x04,   7,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl23_at_air_throw, Air punch throw */
	{  16,                  BU_O,   A_NOM | 0x40, K_LM,    4,   10,   20,    16, 0x1f,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl23_at_throw_k, Ground kick throw */
	{ 100,                    21,        A_HYPER, K_LM,    4,   12,   20,     0, 0x1e,   8,  0x60,       3, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0, 50,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl23_at_otoko, Otoko Michi: the opponent's record in the blast */
	{ 200,                    21,          A_NOM,  K_L,    2,   10,   24,     0, 0x1c,   0,  0x80,       3, SC300, SD_NOR_L,   0,  50,   9,    0,                   NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl23_at_otoko_self, Otoko Michi: Dan's own record as he knocks himself out (dm_ko_self) */
	{ 150,                    21,        A_HYPER, K_LM,    4,   12,   20,     0, 0x1e,   8,  0x60,       3, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0, 50,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25 */
	{   3,                    ZU,          A_NOM,  K_S,    1,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26 */
	{ 0 },	/* 27 */
	{ 0 },	/* 28 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{  12, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,  K_S,    0,    4,   20,     0, 0x1e,   6,  0x60,       3, SC200, SD_NOR_M,   0,  50,  74,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl23_at_gadou_l, Gadouken, light: his punch, and every light shot (shl23_00 type 0) */
	{  12, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,  K_S,    0,    4,   20,     0, 0x1e,   7,  0x60,       3, SC200, SD_NOR_M,   0,  50,  45,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31: pl23_at_gadou_m, Gadouken, medium: his punch, and the shot of medium and heavy */
	{  12, BU_O | NO_VEC | NO_ZU,      A_SPECIAL,  K_S,    0,    4,   20,     0, 0x1e,   8,  0x60,       3, SC200, SD_NOR_M,  64,  90,  45,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl23_at_gadou_h, Gadouken, heavy punch, also the variable counter's */
	{  16,                  BU_O,      A_SPECIAL, K_LL,    0,   10,   30,     0, 0x1e,   2,  0x80,       3, SC300, SD_NOR_L,   0, 100,  34,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 33: pl23_at_kouryuu_l, Kouryuuken, light */
	{  16,                  BU_O,      A_SPECIAL,  K_L,    0,    8,   30,     0, 0x1c,   2,  0x80,       3, SC300, SD_NOR_L,   0, 100,  38,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 34 */
	{  18,                  BU_O,      A_SPECIAL, K_LL,    0,   10,   30,     0, 0x1e,   4,  0x80,       3, SC300, SD_NOR_L,   0, 100,  34,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 35 */
	{  18,                  BU_O,      A_SPECIAL,  K_L,    0,    8,   30,     0, 0x1e,   4,  0x80,       3, SC300, SD_NOR_L,   0, 100,  38,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 36 */
	{  18,                  BU_O,      A_SPECIAL, K_LL,    0,   10,   30,     0, 0x1e,   6,  0x80,       3, SC300, SD_NOR_L,   0, 100,  34,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 37: pl23_at_kouryuu_h, Kouryuuken, medium and heavy */
	{  18,                  BU_O,      A_SPECIAL,  K_L,    0,    8,   30,     0, 0x1c,   6,  0x80,       3, SC300, SD_NOR_L,   0, 100,  38,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 38 */
	{  14,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_KIK_M,   0, 100,  42,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 39 */
	{  10,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1c,   6,  0x60,       3, SC200, SD_KIK_L,   0,  40,  42,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3a */
	{   5,                     2,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1c,   6,  0x60,       3, SC200, SD_KIK_M,   0, 100,  43,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3b */
	{  10,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_KIK_M,   0, 100,  42,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3c */
	{   5,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_KIK_M,   0, 100,  43,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3d */
	{   1,                     2,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_KIK_M,   0, 100,  44,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3e */
	{   6, BU_O | NO_VEC | NO_ZU,        A_HYPER, 0x40,    0,    4,   20,     0, 0x1c,   3,  0x60,       3, SC200, SD_NOR_M,   0,  20,  74,    0,               NO_WEIGHT,  1,     0, 42,     0,     0,   9,        4, { 0, 0 } },	/* 3f: pl23_at_shinkuu, Shinkuu Gadouken hyper: his record and the many-hit shot (shl23_00 type 1) */
	{  10,   ZU | NO_VEC | NO_ZU,        A_HYPER,  K_S,    0,   18,   20,     0, 0x1d,   2,  0x80,       3, SC300, SD_NOR_L,   0, 100,  75,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 40: pl23_at_rekka, Kouryuu Rekka hyper, the rush */
	{  30,                     2,        A_HYPER, 0x40,    0,   18,   30,     0, 0x1e,  12,  0x80,       3, SC300, SD_NOR_L,   0, 100,  75,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 41: pl23_at_rekka_2, Kouryuu Rekka, the later hit (cg_type) */
	{   4,             5 | NO_ZU,        A_HYPER,  K_S,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_NOR_L,   0,  20,   7,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 42: pl23_at_buraiken, Hisshou Buraiken hyper, first record */
	{   4,             5 | NO_ZU,        A_HYPER,  K_M,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_NOR_L,   0,  20,   8,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 43 */
	{   4,             5 | NO_ZU,        A_HYPER,  K_S,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_KIK_L,   0,  20,  11,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 44 */
	{   4,             5 | NO_ZU,        A_HYPER,  K_L,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_NOR_L,   0,  20,   9,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 45 */
	{   4,             5 | NO_ZU,        A_HYPER,  K_M,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_KIK_L,   0,  20,  46,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 46 */
	{   4,             5 | NO_ZU,        A_HYPER,  K_M,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_KIK_L,   0,  20,  12,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 47 */
	{   4,             5 | NO_ZU,        A_HYPER, K_LL,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_NOR_L,   0,  20,  16,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 48 */
	{   4,             5 | NO_ZU,        A_HYPER, K_LM,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_KIK_L,   0,  20,  19,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{   4,             5 | NO_ZU,        A_HYPER,  K_L,    0,    3,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_KIK_L,   0,  20,  13,    0,       NO_WEIGHT | NO_KO,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{  14,                     2,        A_HYPER, 0x40,    0,   10,   10,     0, 0x1f,   6,  0x70,       3, SC100, SD_NOR_L,   0,  20,  34,    0,               NO_WEIGHT,  1,     0, 44,     0,     0,   0,        3, { 0, 0 } },	/* 4b: pl23_at_buraiken_last, Hisshou Buraiken, the closing leap kick */
	{  10,           10 | NO_VEC,      A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  98,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4c: pl23_at_t_kick, the partner's jumping kick as he comes in */
	{   8,            ZU | NO_ZU, A_HI | A_HYPER, 0x40,    0,    4,   24,     0, 0x01,   1,  0x80,       3, SC300, SD_KIK_L,   0,  60,  30,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4d: pl23_at_jburaiken, Air Hisshou Buraiken (unused hyper), first record */
	{   8,                     2, A_HI | A_HYPER, 0x40,    0,   10,   24,     0, 0x1a,  14,  0x80,       3, SC300, SD_KIK_L,   0,  60,  30,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4e: pl23_at_jburaiken_last, Air Hisshou Buraiken, the last hit */
	{   8,   ZU | NO_VEC | NO_ZU,        A_HYPER,  K_S,    0,    4,   20,     0, 0x1a,   1,  0x60,       3, SC200, SD_NOR_M,   0,  50,  70,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 4f: pl23_at_sign_hit, Premium Sign, the throw (both arms) and the light thrown piece (shl23_00 type 2) */
	{   8,   ZU | NO_VEC | NO_ZU,        A_HYPER,  K_S,    0,    4,   20,     0, 0x1a,   1,  0x60,       3, SC200, SD_NOR_M,   0,  50,  70,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 50 */
	{   9,   ZU | NO_VEC | NO_ZU,        A_HYPER,  K_S,    0,    4,   20,     0, 0x1a,   1,  0x60,       3, SC200, SD_NOR_M,   0,  50,  70,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 51: pl23_at_sign_piece, Premium Sign, the thrown piece of medium and heavy (shl23_00 type 2) */
	{   2,                    ZU,      A_SPECIAL,  K_S,    0,   10,   10,     0, 0x18,   4,  0x70,       3, SC100, SD_NOR_S,   0,  20,  77,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 52: pl23_at_sign, Premium Sign, first record (sign_init) */
	{   8,         BU_O | NO_VEC,      A_SPECIAL, K_LL,    0,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  99,  16,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 53 */
	{  10,           12 | NO_VEC,      A_SPECIAL,  K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64, 100,   9,   10,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 54: pl23_at_snapback, Snapback (sp_snapback) */
	{  14,         BU_O | NO_VEC,      A_SPECIAL, K_LL,    0,   10,   16,     0, 0x1f,  11,  0x40, MARK_N2, SC200, SD_NOR_L,   0,  40,  85,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 55: pl23_at_upper, Crouching HK launcher */
	{   1,                     2,      A_SPECIAL,  K_S,    0,   10,   30,     0, 0x18,   4,  0x60,       3, SC200, SD_KIK_L,   0, 100,  44,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 56 */
	{  18,                  BU_O,      A_SPECIAL, K_LL,    0,   10,   30,     0, 0x1e,   6,  0x80,       3, SC300, SD_NOR_L,  64,  90,  34,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 57: pl23_at_kouryuu_counter, Kouryuuken as a variable counter (team_type 3, not light) */
	{  10,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_KIK_M,  64,  90,  42,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 58 */
	{   5,   ZU | NO_VEC | NO_ZU,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_KIK_M,  64,  90,  43,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 59 */
	{   1,                     2,      A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1a,   8,  0x60,       3, SC200, SD_KIK_M,  64,  90,  44,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 5a */
};
