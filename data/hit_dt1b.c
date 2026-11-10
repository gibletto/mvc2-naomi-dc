/* Chun Li (character 1b): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl1b_at_ for its own (pl1b_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap).
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
     box, box2  boxes of hit_1b that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_1b[] = {
	/* dmg                   type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                    ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   1,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   6,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,                    ZU,            A_NOM, K_LM,    2,   11,   16,     0, 0x1d,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,   7,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                    ZU,            A_NOM, 0x40,    1,   12,   24,     0, 0x1a,   4,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,   8,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,                    ZU,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,                    ZU,            A_NOM,  K_M,    2,   11,   16,     0, 0x1a,   3,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  10,  119,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,         BU_O | NO_VEC,            A_NOM,  K_L,    1,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  11,  111, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,           ZU | NO_VEC,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  12,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                    ZU,            A_NOM, K_LM,    2,   11,   16,     0, 0x18,   8,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  13,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,                    ZU,            A_NOM, 0xc0,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,           ZU | NO_VEC,             A_LO, K_LS,    4,   10,   10,     0, 0x18,   8,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,                    ZU,             A_LO, K_LM,    2,   11,   16,     0, 0x18,   8,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  16,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,                    AS,             A_LO, K_LL,    1,   12,   24,     5, 0x18,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  18,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  19,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,           ZU | NO_VEC,             A_HI,  K_M,    1,   12,   32,     0, 0x16,  12,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  20,    0, NO_WEIGHT | AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  21,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x1d,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  52,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{ 0 },	/* 11: ATCK_JUMP_HK */
	{  12,         BU_O | NO_VEC,             A_HI,  K_M,    1,   12,   24,     0, 0x1c,   6,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl1b_at_downback_hk, Down-back + HK, the axe kick (Kaku Kyaku Raku) */
	{  10,                    ZU,             A_HI, K_LM,    1,   12,   24,     0, 0x18,   0,  0x80, MARK_N3, SC200, SD_KIK_M,   0,  50,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl1b_at_jdown_hk, Jumping down + HK, the head stomp */
	{  12,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   24,     0, 0x16,   8,  0x60, MARK_N3, SC300, SD_KIK_L,   0,  60,  23,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl1b_at_jfwd_hk, Jumping forward + HK (Kaku Yoku Da) */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   6,                    ZU,             A_HI,  K_M,    1,   12,   32,     0, 0x17,   8,  0x60, MARK_N3, SC300, SD_KIK_L,   3,  60,  53,   54,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19 */
	{   8,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   32,     0, 0x17,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a: pl1b_at_jump_hk, Her own jumping HK, used where others use the common 0x11 */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                     2,     A_NOM | 0x40,    0,    0,    4,   20,     0, 0x1c,   4,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl1b_at_throw, Ground throw */
	{  16,                     2,     A_NOM | 0x40,    0,    0,   16,   20,     0, 0x14,  10,  0x80, MARK_N1, SC300, SD_NOR_L,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl1b_at_air_throw, Air throw */
	{  32,        2 | FLY_SCREEN,   A_HYPER | 0x40,    0,    0,    8,   20,     0, 0x04,  12,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,  32,   35,    NO_WEIGHT | NO_COMBO,  1,     0, 33,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl1b_at_shichisei_2, Shichisei Ranka: the caught opponent thrown away (shichisei_throw) */
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
	{  12,            ZU | NO_ZU,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1a,   2,  0x80,       3, SC300,        8,   0,  50, 107,   24,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl1b_at_kikou, Kikou Ken, Chun-Li and the shot (shl1b_00 kind 0) */
	{ 0 },	/* 31 */
	{ 0 },	/* 32 */
	{   6, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,    8,   30,     0, 0x1d,   3,  0x80,       3, SC300, SD_KIK_L,   3,  90,  28,   29,               NO_WEIGHT,  1,     0,  0,     0,    40,   0,        5, { 0, 0 } },	/* 33 */
	{   7, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,   12,   30,     0, 0x1e,  12,  0x80,       3, SC300,       12,   6,  90,  28,   29,               NO_WEIGHT,  1,     0,  0,     0,    40,   0,        5, { 0, 0 } },	/* 34 */
	{ 0 },	/* 35 */
	{ 0 },	/* 36 */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,    8,   30,     0, 0x1e,   9,  0x80,       3, SC300, SD_KIK_L,   3,  90,  28,   29,               NO_WEIGHT,  1,     0,  0,     0,    40,   0,        5, { 0, 0 } },	/* 37 */
	{   5, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,   12,   30,     0, 0x1e,  12,  0x80,       3, SC300,       12,   3,  90,  28,   29,               NO_WEIGHT,  1,     0,  0,     0,    40,   0,        5, { 0, 0 } },	/* 38 */
	{  16,                     2, A_HI | A_SPECIAL,  K_S,    0,   10,   30,     0, 0x14,   3,  0x80,       3, SC300, SD_KIK_L,   0,  90,  31,   90,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{ 0 },	/* 3a */
	{  16,                     2, A_HI | A_SPECIAL,  K_L,    0,   10,   30,     0, 0x14,   5,  0x80,       3, SC300, SD_KIK_L,   0,  90,  31,   90,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{ 0 },	/* 3c */
	{ 0 },	/* 3d */
	{ 0 },	/* 3e */
	{ 0 },	/* 3f */
	{   4,            ZU | NO_ZU,          A_HYPER,  K_L,    0,    4,   20,     0, 0x1e,   0,  0x80,       3, SC300, SD_NOR_L,   0,  90,  34,   35,               NO_WEIGHT,  1,     0, 30,     0,     0,   6,        3, { 0, 0 } },	/* 40: pl1b_at_kikoushou, Kikoushou (hyper), Chun-Li and the ball (shl1b_00 kind 1) */
	{  20,             2 | NO_ZU,          A_HYPER,  K_L,    0,    8,   20,     0, 0x1d,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90,  34,   35,               NO_WEIGHT,  1,     0, 30,     0,     0,   6,        3, { 0, 0 } },	/* 41 */
	{ 0 },	/* 42 */
	{   4,   ZU | NO_VEC | NO_ZU,          A_HYPER,  K_L,    0,    2,   20,     0, 0x1b,   4,  0x80,       3, SC300, SD_KIK_L,   0,  90,  45,   47,               NO_WEIGHT,  1,     0, 31,     0,     0,   4,        3, { 0, 0 } },	/* 43 */
	{   4,   ZU | NO_VEC | NO_ZU,          A_HYPER, K_LL,    0,    2,   20,     0, 0x19,   7,  0x80,       3, SC300, SD_KIK_L,   0,  90,  44,   46,     NO_CHIP | NO_WEIGHT,  1,     0, 31,     0,     0,   4,        3, { 0, 0 } },	/* 44 */
	{   4,    2 | NO_VEC | NO_ZU,          A_HYPER,  K_L,    0,    2,   20,     0, 0x1c,   0,  0x80,       3, SC300, SD_KIK_L,   0,  90,  44,    0,               NO_WEIGHT,  1,     0, 31,     0,     0,   4,        3, { 0, 0 } },	/* 45: pl1b_at_senretsu, Senretsu Kyaku (hyper): the kicks */
	{  18,        2 | FLY_SCREEN,          A_HYPER, 0x40,    0,   16,   20,     0, 0x19,  15,  0x80,       3, SC300,       12,   0,  90, 112,    0,               NO_WEIGHT,  1,     0, 31,     0,     0,   4,        3, { 0, 0 } },	/* 46: pl1b_at_senretsu_end, Senretsu Kyaku: the last kick after the mashing */
	{ 0 },	/* 47 */
	{ 0 },	/* 48 */
	{ 0 },	/* 49 */
	{ 0 },	/* 4a */
	{   6,         BU_O | NO_VEC,          A_HYPER,  K_L,    0,    4,   30,     0, 0x00,   5,  0x80,       3, SC300, SD_KIK_L,   0,  90,  92,   93,               NO_WEIGHT,  1,     0, 32,     0,     0,   0,        3, { 0, 0 } },	/* 4b: pl1b_at_hazan, Hazan Tenshou Kyaku (hyper) */
	{  10,            2 | NO_VEC,          A_HYPER,  K_L,    0,   12,   30,     0, 0x1c,   8,  0x80,       3, SC300,       12,   0,  90,  92,   93,               NO_WEIGHT,  1,     0, 32,     0,     0,   0,        3, { 0, 0 } },	/* 4c */
	{   8,                  BU_O,        A_SPECIAL,  K_S,    0,    6,   20,     0, 0x1a,   4,  0x80,       3, SC100, SD_KIK_L,   2,  10,  45,   46,               NO_WEIGHT,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 4d: pl1b_at_hyakuretsu, Hyakuretsu Kyaku */
	{ 0 },	/* 4e */
	{   6,                  BU_O,        A_SPECIAL,  K_S,    0,    6,   20,     0, 0x1a,   6,  0x80,       3, SC100, SD_KIK_L,   2,  10,  45,   46,               NO_WEIGHT,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 4f */
	{  10,           10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  84,   85, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 50: pl1b_at_t_kick, Tag-in kick that leaps off the wall */
	{ 0 },	/* 51 */
	{  14,          BU_O | NO_ZU,        A_SPECIAL,  K_L,    0,    4,   20,     0, 0x1a,   8,  0x80,       3, SC300,        8,   0,  90,  24,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 52: pl1b_at_kikou_counter, Kikou Ken's shot as a variable counter (team_type 3) */
	{   7,                  BU_O,        A_SPECIAL,  K_S,    0,    6,   20,     0, 0x1a,   4,  0x80,       3, SC100, SD_KIK_L,   2,  10,  45,   46,               NO_WEIGHT,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 53 */
	{ 0 },	/* 54 */
	{   6,                  BU_O,        A_SPECIAL,  K_S,    0,    6,   20,     0, 0x1a,   6,  0x80,       3, SC100, SD_KIK_L,   2,  10,  45,   46,               NO_WEIGHT,  1,     0,  0,     0,    30,   0,        6, { 0, 0 } },	/* 55 */
	{   1, BU_O | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1a,  13,  0x80,       3, SC300, SD_NOR_L,   2,  90, 106,    0, NO_CHIP | NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 56: pl1b_at_chouhatsu, the taunt, which can hit */
	{ 0 },	/* 57: pl1b_at_fwd_hp, Forward + HP, Reishiki Kikou Ken */
	{ 0 },	/* 58 */
	{  12,                    ZU,        A_SPECIAL,  K_M,    0,    6,   20,     0, 0x1b,   4,  0x80,       3, SC300, SD_NOR_L,   2,  50, 108,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 59 */
	{   8,                  BU_O,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1e,   6,  0x80,       3, SC300, SD_NOR_L,   2,  60, 108,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5a */
	{ 0 },	/* 5b */
	{ 0 },	/* 5c */
	{ 0 },	/* 5d: pl1b_at_shichisei, Shichisei Ranka (air hyper) */
	{ 0 },	/* 5e */
	{ 0 },	/* 5f */
	{ 0 },	/* 60 */
	{  10,           12 | NO_VEC,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90, 108,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 61: pl1b_at_snapback, Snapback, Chun-Li and its ball (shl1b_00 kind 6) */
	{  13,         BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  40, 144,  145, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 62: pl1b_at_upper, the launcher */
	{   6,         BU_O | NO_VEC,          A_HYPER,  K_L,    0,    4,   30,     0, 0x00,  10,  0x80,       3, SC300, SD_KIK_L,   0,  90,  92,   93,               NO_WEIGHT,  1,     0, 32,     0,     0,   0,        3, { 0, 0 } },	/* 63 */
};
