/* Gambit (character 28): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl28_at_ for its own (pl28_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap, 14 dm_nop, 15 dm_spark).
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
     box, box2  boxes of hit_28 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_28[] = {
	/* dmg                 type          kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score  se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                  ZU,        A_NOM,  K_S,    4,   10,   10,     0, 0x12,   1,  0x70, MARK_N1, SC100,  1,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                  ZU,        A_NOM, K_LM,    2,   11,   16,     0, 0x11,   1,  0x80, MARK_N2, SC200,  2,   0,  40,   8,   51,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                BU_O,        A_NOM,  K_L,    1,   12,   24,     0, 0x1a,  12,  0x80, MARK_N3, SC300,  3,   0,  50,   9,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100,  1,   0,  20,  10,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                  ZU,        A_NOM, K_LM,    2,   11,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200,  2,   0,  40,  11,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                  ZU,        A_NOM,  K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300,  3,   0,  50,  12,   70,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                  ZU,        A_NOM, K_LS,    4,   10,   10,     0, 0x12,   1,  0x70, MARK_N1, SC100,  1,   0,  20,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  ZU,        A_NOM,  K_M,    2,   11,   16,     0, 0x11,   1,  0x80, MARK_N2, SC200,  2,   0,  40,  14,   53,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,       BU_O | NO_VEC,        A_NOM, K_LL,    1,   10,   24,     0, 0x00,  12,  0x80, MARK_N3, SC300,  3,   0,  50,  15,   61, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                  ZU,         A_LO, K_LS,    4,   10,   10,     0, 0x00,   3,  0x70, MARK_N1, SC100,  1,   0,  20,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                  ZU,         A_LO, K_LM,    2,   11,   16,     0, 0x00,   3,  0x40, MARK_N2, SC200,  2,   0,  40,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                  AS,         A_LO, K_LL,    1,   12,   24,     9, 0x18,   8,  0x70, MARK_N3, SC300,  3,   0, 100,  18,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   3,  0x70, MARK_N1, SC100,  1,   0,  20,  19,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x1c,   4,  0x80, MARK_N2, SC200,  2,   0,  40,  20,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,         ZU | NO_VEC,         A_HI,  K_M,    1,   12,   32,     0, 0x14,   8,  0x80, MARK_N3, SC300,  3,   0,  60,  21,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,         A_HI,  K_S,    1,   10,   10,     0, 0x00,   3,  0x60, MARK_N1, SC100,  1,   0,  20,  22,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,         ZU | NO_VEC,         A_HI,  K_M,    1,   10,   16,     0, 0x00,   3,  0x60, MARK_N2, SC200,  2,   0,  40,  23,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,         ZU | NO_VEC,         A_HI,  K_M,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300,  3,   0,  60,  24,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,       BU_O | NO_VEC,    A_SPECIAL, K_LL,    1,   10,   24,     0, 0x00,  12,  0x80, MARK_N3, SC300,  3,   0,  40,  15,   61, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12 */
	{  13,         ZU | NO_VEC,        A_NOM, K_LL,    1,   10,   24,     0, 0x1c,   5,  0x80, MARK_N3, SC300,  3,   0,  50,  15,   61,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19: pl28_at_stand_mp_2, Second hit of the standing MP */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                   2, A_NOM | 0x40,    0,    4,    4,    0,    15, 0x1a,   9,  0x80, MARK_N1, SC300, 76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl28_at_throw_p, P throw */
	{   4,                   2, A_NOM | 0x40,    0,    4,   16,    0,    64, 0x1f,  10,  0x80, MARK_N1, SC300, 76,   2,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl28_at_throw_k, K throw: the final throw out of the pin */
	{  16,                   2, A_NOM | 0x40,    0,    4,    8,    0,    15, 0x14,   4,  0x80, MARK_N1, SC300,  1,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl28_at_air_throw, K air throw */
	{   4,                   2, A_NOM | 0x40,    0,    4,    8,    0,    15, 0x14,   4,  0x80, MARK_N1, SC300,  1,   2,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl28_at_throw_k_hit, K throw: each extra hit while he holds the opponent down */
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
	{  15, 15 | NO_VEC | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x02,   8,  0x80,    0xff, SC300, 20,   0,  50,  57,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl28_at_card, Kinetic Card, light (player and card); also the player's record for Trick Card */
	{  15,          15 | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x00,   8,  0x80,    0xff, SC300, 20,  64,  90,  57,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31: pl28_at_card_vc, Kinetic Card as a variable counter (team_type 3) */
	{  15, 15 | NO_VEC | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x00,   8,  0x80,    0xff, SC300, 20,   0,  90,  57,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32: pl28_at_card_h, Kinetic Card, heavy (card kinds 5 and 7) */
	{  10,         10 | NO_VEC,    A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,  4,   0,  90, 104,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 33: pl28_at_t_kick, Partner's jump-in attack (pl28_t_kick); also the unused sp_t_kick hop */
	{  18,                   2,    A_SPECIAL,  K_L,    0,   11,   20,     0, 0x1a,   8,  0x80,       3, SC300,  4,  64,  90,  46,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 34: pl28_at_slash_vc, Cajun Slash as a variable counter (team_type 3) */
	{  18,                   2,    A_SPECIAL,  K_L,    0,   11,   20,     0, 0x1a,   8,  0x80,       3, SC300,  4,   4,  90,  46,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 35 */
	{  10, ZU | NO_VEC | NO_ZU,    A_SPECIAL, K_LL,    0,    4,   20,     0, 0x1a,   8,  0x80,       3, SC300,  4,   3,  90,  44,   97,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 36 */
	{   6, ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   20,     0, 0x1a,   8,  0x80,       3, SC300,  3,   3,  90,  78,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 37 */
	{  10,          2 | NO_VEC,    A_SPECIAL, K_LL,    0,   11,   20,     0, 0x1a,   8,  0x80,       3, SC300,  4,   3,  90,  87,   83,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 38 */
	{   5,                  ZU,        A_NOM,  K_S,    4,   10,   10,     0, 0x12,   1,  0x70, MARK_N1, SC100,  1,   0,  20,   7,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 39 */
	{   4, ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L,    0,    4,   20,     0, 0x1a,   8,  0x80,       3, SC300,  4,   3,  90,  78,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3a */
	{   4, ZU | NO_VEC | NO_ZU,    A_SPECIAL, K_LL,    0,    4,   20,     0, 0x1a,   8,  0x80,       3, SC300,  3,   3,  90,  90,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3b */
	{  14,          2 | NO_VEC,    A_SPECIAL,  K_L,    0,   12,   20,     0, 0x1a,  10,  0x80,       3, SC300,  4,   3,  90,  94,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3c */
	{  20,                   2,    A_SPECIAL,  K_L,    0,   12,   20,     0, 0x14,  10,  0x80,       3, SC300,  4,   0,  90, 101,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3d: pl28_at_escape, Cajun Escape dive */
	{  18,                   2,    A_SPECIAL,  K_L,    0,   12,   20,     0, 0x14,  10,  0x80,       3, SC300,  4,   0,  90,  98,   99,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3e: pl28_at_strike, Cajun Strike */
	{   2,          15 | NO_ZU,      A_HYPER,  K_S,    0,    0,   20,     0, 0x02,   0,  0x80,    0xff, SC300, 20,   0,  90,  57,    0,               NO_WEIGHT,  1,     0, 36,     0,     0,   6,        4, { 0, 0 } },	/* 3f: pl28_at_royal_card8, Royal Flush: every eighth card of the stream (sub_type 0) */
	{   3,          15 | NO_ZU,      A_HYPER,  K_S,    0,    0,   20,     0, 0x02,   0,  0x80,    0xff, SC300, 20,   0,  90,  57,    0,     NO_CHIP | NO_WEIGHT,  1,     0, 36,     0,     0,   6,        4, { 0, 0 } },	/* 40: pl28_at_royal, Royal Flush hyper: his record and the other streamed cards */
	{  13,          15 | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x02,   8,  0x80,    0xff, SC300, 20,   0,  50,  57,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 41 */
	{  13,          15 | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x00,   8,  0x80,    0xff, SC300, 20,   0,  90,  57,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 42 */
	{  13,          15 | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x00,   8,  0x80,    0xff, SC300, 20,   0,  90,  57,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 43 */
	{  15,          15 | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x02,   8,  0x80,    0xff, SC300, 20,   0,  50,  57,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44 */
	{  15,          15 | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x00,   8,  0x80,    0xff, SC300, 20,   0,  90,  57,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 45 */
	{  15,          15 | NO_ZU,    A_SPECIAL,  K_S,    0,    2,   20,     0, 0x00,   8,  0x80,    0xff, SC300, 20,   0,  90,  57,    0, NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 46 */
	{  15,           2 | NO_ZU,      A_HYPER,  K_S,    0,    2,   20,     0, 0x14,   8,  0x80,       3, SC300,  3,   0,  90, 107,  108,                       0,  1,     0, 36,     0,     0,   0,        2, { 0, 0 } },	/* 47: pl28_at_royal_shower, Royal Flush: the shower of cards (shl28_00 kind 9) */
	{   6, 2 | NO_VEC | NO_ZU | FLY_SCREEN,      A_HYPER,  K_L,    0,    1,   20,     0, 0x1a,  11,  0x80,       3, SC300,  4,   0,  90, 113,  115,                       0,  1,     0, 37,     0,     0,  16,        2, { 0, 0 } },	/* 48: pl28_at_explosion, Cajun Explosion hyper (player and shl28_01) */
	{  10,         14 | NO_VEC,    A_SPECIAL,  K_L,    0,   16,   10,    30, 0x02,   9,  0x60,       3, SC200, 12,   0,  90, 104,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 49 */
	{  10,         12 | NO_VEC,    A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,  8,  64, 100,   8,   51,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4a: pl28_at_snapback, Snapback (sp_snapback) */
	{  13,       BU_O | NO_VEC,    A_SPECIAL, K_LL,    0,   10,   16,     0, 0x00,  12,  0x40, MARK_N2, SC200,  3,   0,  40, 125,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4b: pl28_at_upper, Launcher (pl28_upper, Char_Extra 5) */
	{  15,       BU_O | NO_VEC,        A_NOM, K_LL,    1,   10,   24,     0, 0x00,  12,  0x80, MARK_N3, SC300,  3,   0,  50,  15,   61, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 4c: pl28_at_crouch_hp_vc, Crouching HP (sp_crouch_hp) as a variable counter; otherwise it uses the common 8 */
};
