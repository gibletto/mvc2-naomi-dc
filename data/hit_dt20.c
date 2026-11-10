/* Felicia (character 20): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl20_at_ for its own (pl20_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 5 dm_hold,
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
     box, box2  boxes of hit_20 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_20[] = {
	/* dmg                   type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                    ZU,            A_NOM, K_LS,    4,   10,   10,     0, 0x15,   4,  0x70, MARK_N1, SC100,       26,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,                    ZU,            A_NOM, K_LM,    2,   11,   16,     0, 0x15,   6,  0x80, MARK_N2, SC200,       27,   0,  40,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{   8,                    ZU,            A_NOM, K_LL,    1,   12,   24,     0, 0x00,  10,  0x80, MARK_N3, SC300,       28,   0,  50,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,                    ZU,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,                    ZU,            A_NOM, K_LM,    2,   11,   16,     0, 0x1c,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,         BU_O | NO_VEC,            A_NOM,  K_L,    1,   12,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  46,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,                    ZU,            A_NOM, K_LS,    4,   10,   10,     0, 0x16,   4,  0x70, MARK_N1, SC100,       26,   0,  20,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                    ZU,            A_NOM, K_LM,    2,   11,   16,     0, 0x19,   6,  0x80, MARK_N2, SC200,       27,   0,  40,  50,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,                  BU_O,            A_NOM,  K_L,    1,   10,   24,     0, 0x1e,   9,  0x80, MARK_N3, SC300,       28,   0,  50,  51,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,                    ZU,             A_LO, K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,                    ZU,             A_LO, K_LM,    2,   11,   16,     0, 0x19,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  53,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,                    AS,             A_LO, K_LL,    1,   12,   24,     4, 0x19,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100,       26,   0,  20,  55,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200,       27,   0,  40,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,           ZU | NO_VEC,             A_HI,  K_L,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300,       28,   0,  60,  57,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  58,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  59,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,           ZU | NO_VEC,             A_HI,  K_L,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC300, SD_KIK_L,   0,  60,  60,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   3,                    ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 12: pl20_at_stand_lp, Standing LP (her own record, not the common 0x00) */
	{   6,                    ZU,            A_NOM,  K_M,    2,   11,   16,     0, 0x18,   6,  0x80, MARK_N2, SC200,       27,   0,  40,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl20_at_stand_mp, Standing MP (her own record) */
	{   6,                    ZU,            A_NOM, K_LL,    1,   12,   24,     0, 0x18,   8,  0x80, MARK_N3, SC300,       28,   0,  50,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl20_at_stand_hp_1, Standing HP, a chain of hits: first */
	{   3,                    ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  43,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 15 */
	{   6,                    ZU,            A_NOM,  K_M,    2,   11,   16,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  45,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl20_at_stand_mk, Standing MK (her own record) */
	{   4,                    ZU,            A_NOM, K_LL,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  47,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 17: pl20_at_back_hk_1, Back + HK, first hit */
	{ 0 },	/* 18 */
	{   6,                    ZU,            A_NOM, K_LL,    1,   12,   24,     0, 0x00,  10,  0x80, MARK_N3, SC300,       27,   0,  50,  37,   38,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl20_at_stand_hp_alt, Standing HP hit for routine_2 0, which stand_punch never leaves, so it looks unreachable */
	{   6,                    ZU,            A_NOM, K_LL,    1,   12,   24,     0, 0x17,   8,  0x80, MARK_N3, SC300,       28,   0,  50,  40,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a: pl20_at_stand_hp_2, Standing HP, second hit (cg_type 1) */
	{   6,                    ZU,            A_NOM, K_LL,    1,   12,   24,     0, 0x17,   8,  0x80, MARK_N3, SC300,       28,   0,  50,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1b: pl20_at_stand_hp_3, Standing HP, third hit (cg_type 2) */
	{   6,                    ZU,            A_NOM, K_LL,    1,   12,   24,     0, 0x19,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1c: pl20_at_back_hk_2, Back + HK, second hit */
	{   5,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   1,  0x80, MARK_N2, SC200,       27,   0,  40,  56,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1d: pl20_at_jump_mp_2, Jump MP, second hit */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   0,                  BU_O,     A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x1d,   4,  0x60, MARK_N2, SC200,       26,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl20_at_throw_p, Punch throw: the final throw after the hits, set on the opponent */
	{   2,                  BU_O,     A_NOM | 0x40,  K_S,    0,   10,   20,     0, 0x1d,   4,  0x60, MARK_N2, SC200,       26, 128,   0,   0,    0,                   NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{   2,                  BU_O,     A_NOM | 0x40,  K_S,    0,   10,   20,     0, 0x1d,   4,  0x60, MARK_N2, SC200,       26, 128,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22 */
	{  16,                  BU_O,     A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x03,   4,  0x60, MARK_N2, SC200, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl20_at_throw_k, Kick throw (a leap with the opponent) */
	{  16,                  BU_O,     A_NOM | 0x40, K_LM,    0,   10,   20,     0, 0x1d,   6,  0x60, MARK_N2, SC200, SD_NOR_M,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl20_at_air_throw, Air throw */
	{  20,                  BU_O, A_SPECIAL | 0x40, K_LM,    0,   12,   20,     0, 0x1d,   8,  0x60, MARK_N2, SC200,       27,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25: pl20_at_hellcat_throw, Hell Cat command throw, the final throw */
	{   2,                  BU_O, A_SPECIAL | 0x40, K_LM,    0,   12,   20,     0, 0x1b,   8,  0x60, MARK_N2, SC200,       27, 128,   0,   0,    0,                   NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 26: pl20_at_hellcat_hit, Hell Cat, each hit on the held opponent */
	{   1,             5 | NO_ZU,   A_HYPER | 0x40, K_LL,    0,    1,   20,     0, 0x18,   0,  0x80,       3, SC300,        8, 128,   0,   0,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 27: pl20_at_help_hit, Please Help Me, the hits while shl20_01's grab carries the opponent */
	{  60,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x40,  K_L,    0,   10,   20,     4, 0x1c,   2,  0x60,       3, SC200,        8,   0,   0,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 28: pl20_at_help_last, Please Help Me, the last throw from shl20_01 */
	{ 0 },	/* 29 */
	{ 0 },	/* 2a */
	{ 0 },	/* 2b */
	{ 0 },	/* 2c */
	{ 0 },	/* 2d */
	{ 0 },	/* 2e */
	{ 0 },	/* 2f */
	{   3,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x00,   6,  0x60,       3, SC200,       12,   0,  20,  64,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 30 */
	{   2,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x00,   6,  0x60,       3, SC200,       12,   0,  20,  65,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 31 */
	{   3,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x00,   8,  0x60,       3, SC200,       12,   0,  20,  64,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 32 */
	{   2,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x00,   8,  0x41,       3, SC200,       12,   0,  20,  65,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 33 */
	{   8,   ZU | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,   10,   20,     0, 0x00,   6,  0x60,       3, SC200,       12,   0,  20,  66,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 34 */
	{   8,            2 | NO_VEC,        A_SPECIAL, K_LL,    0,   12,   20,    10, 0x1c,  11,  0x60,       3, SC200,       12,   0,  20,  67,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 35 */
	{   8,   ZU | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,   10,   20,     0, 0x00,   8,  0x60,       3, SC200,       12,   0,  20,  66,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 36 */
	{   8,            2 | NO_VEC,        A_SPECIAL, K_LL,    0,   14,   20,    10, 0x1c,  12,  0x60,       3, SC200,       12,   0,  20,  67,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 37 */
	{   2,           ZU | NO_VEC,        A_SPECIAL, K_LL,    0,   10,   20,     0, 0x1c,   8,  0x60,       3, SC200,        8,   0,  20,  68,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 38 */
	{   2,           ZU | NO_VEC,        A_SPECIAL, K_LL,    0,   10,   20,     0, 0x1c,  10,  0x80,       3, SC300,        8,   0,  20,  68,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 39 */
	{  17,            2 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1d,   8,  0x80,       3, SC300,        8,   0,  20,  69,   70,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{  19,            2 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1d,  10,  0x80,       3, SC300,        8,   0,  20,  69,   70,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{   3,            2 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1d,   8,  0x80,       3, SC300,        8,   0,  20, 108,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3c */
	{   4,            2 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1d,  12,  0x80,       3, SC300,        8,   0,  90, 108,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3d */
	{  16,         BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   20,     0, 0x1c,  11,  0x80,       3, SC300,        8,   0,  90,  71,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{  16,         BU_O | NO_VEC,        A_SPECIAL, K_LL,    0,   10,   20,     0, 0x1c,  10,  0x80,       3, SC300,        8,   0,  90,  71,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3f */
	{  15,                    AS, A_LO | A_SPECIAL,  K_L,    0,   10,   20,    15, 0x18,   8,  0x80,       3, SC300,       12,   0,  90,  72,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 40 */
	{  15,                    AS, A_LO | A_SPECIAL,  K_L,    0,   10,   20,    15, 0x18,  10,  0x80,       3, SC300,       12,   0,  90,  72,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 41 */
	{  13, BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_LM,    0,   10,   20,     0, 0x1b,   8,  0x80,       3, SC300,        8,   0,  50,  98,   99,                       0,  1,     0,  0,     0,     0,   4,        4, { 0, 0 } },	/* 42 */
	{  16, BU_O | NO_VEC | NO_ZU,        A_SPECIAL, K_LM,    0,   10,   20,     0, 0x1b,  10,  0x80,       3, SC300,        8,   0,  50,  98,   99,                       0,  1,     0,  0,     0,     0,   4,        4, { 0, 0 } },	/* 43 */
	{  13,            2 | NO_VEC,        A_SPECIAL,  K_M,    0,    6,   20,     4, 0x1a,  10,  0x60,       3, SC200,       28,   0,  90,  73,    0,               NO_WEIGHT,  1,     0,  7,     0,     0,   0,        6, { 0, 0 } },	/* 44 */
	{  16,            2 | NO_VEC,        A_SPECIAL,  K_M,    0,    6,   20,     4, 0x10,   0,  0x60,       3, SC200,       28,   0,  90,  74,   75,               NO_WEIGHT,  1,     0,  7,     0,     0,   0,        6, { 0, 0 } },	/* 45 */
	{  10,           10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  89,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 46: pl20_at_t_kick, the partner's jumping kick as she comes in */
	{  10,           12 | NO_VEC,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90,  53,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 47: pl20_at_snapback, Snapback */
	{  14,         BU_O | NO_VEC,        A_SPECIAL, K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40, 112,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 48: pl20_at_upper, Launcher */
	{   2,           ZU | NO_VEC,          A_HYPER,  K_L,    0,    6,   20,     0, 0x18,   0,  0x60,       3, SC200,        8,   0,  90,  78,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 49: pl20_at_dancing, Dancing Flash hyper, first hit (later hits are cg_flag1 + 73 / + 77) */
	{   3,                     5,          A_HYPER,  K_L,    0,    4,   20,     0, 0x18,   0,  0x60,       3, SC200,       28,   0,  90,  79,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{   4,                     5,          A_HYPER, K_LL,    0,    4,   20,     0, 0x18,   0,  0x60,       3, SC200,       28,   0,  90,  80,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{   3,                     5,          A_HYPER,  K_L,    0,    4,   20,     0, 0x18,   0,  0x60,       3, SC200,       12,   0,  90,  83,   85,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4c */
	{   4,            2 | NO_VEC,          A_HYPER,  K_L,    0,   11,   20,     0, 0x1f,   8,  0x60,       3, SC200,       12,   0,  90,  86,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4d */
	{  10,            2 | NO_VEC,          A_HYPER, K_LL,    0,   11,   20,     0, 0x1e,   8,  0x60,       3, SC200,       28,   0,  40,  87,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4e */
	{  20,   ZU | NO_VEC | NO_ZU,          A_HYPER, K_LL,    0,   11,   20,     0, 0x1c,  12,  0x60,       3, SC200,       28,   0,  90,  88,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 4f */
	{   8,             5 | NO_ZU,          A_HYPER, K_LL,    0,   10,   20,     0, 0x1c,   8,  0x60,       3, SC200,        8,   0,  90,  96,    0,       NO_WEIGHT | NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 50: pl20_at_help, Please Help Me hyper: her own record and shl20_01's grab */
	{ 0 },	/* 51 */
	{   2,           ZU | NO_VEC,          A_HYPER, K_LL,    0,    0,   20,     0, 0x1a,   8,  0x60,       3, SC200,       12,   0,  90, 110,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 52: pl20_at_ssand, Super Sand Splash hyper */
	{   4,   ZU | NO_VEC | NO_ZU,          A_HYPER, K_LL,    0,    4,   20,     0, 0x1a,   8,  0x60,       3, SC200,        8,   0,  90, 100,  101,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 53: pl20_at_ssand_hit, Super Sand Splash, the hits of the row of shots (shl20_00 types 4 and 5) */
	{   2,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1d,   4,  0x60,       3, SC200,       12,   0,  20,  64,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 54 */
	{   2,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1d,   4,  0x60,       3, SC200,       12,   0,  20,  65,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 55 */
	{   2,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1d,   6,  0x60,       3, SC200,       12,   0,  20,  64,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 56 */
	{   2,           ZU | NO_VEC,        A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1d,   6,  0x41,       3, SC200,       12,   0,  20,  65,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 57 */
	{   8,   ZU | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,    6,   20,     0, 0x17,   4,  0x60,       3, SC200,       12,   0,  20,  66,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 58 */
	{  10,            2 | NO_VEC,        A_SPECIAL, K_LL,    0,    6,   20,    10, 0x17,   6,  0x60,       3, SC200,       12,   0,  20,  67,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 59 */
	{   8,   ZU | NO_VEC | NO_ZU,        A_SPECIAL, K_LL,    0,    6,   20,     0, 0x17,   4,  0x60,       3, SC200,       12,   0,  20,  66,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 5a */
	{  10,            2 | NO_VEC,        A_SPECIAL, K_LL,    0,    6,   20,    10, 0x17,   6,  0x60,       3, SC200,       12,   0,  20,  67,    0,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 5b */
	{   2,           ZU | NO_VEC,        A_SPECIAL, K_LL,    0,    0,   20,     0, 0x19,   6,  0x60,       3, SC200,       12,   0,  90, 111,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5c */
	{   2,           ZU | NO_VEC,        A_SPECIAL, K_LL,    0,    0,   20,     0, 0x19,   6,  0x60,       3, SC200,       12,   0,  90, 111,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 5d */
	{  10, 2 | NO_VEC | NO_ZU | FLY_SCREEN,          A_HYPER, K_LL,    0,   10,   20,     0, 0x1a,  15,  0x60,       3, SC200,        8,   0,  90, 100,  101,               NO_WEIGHT,  1,     0,  0,     0,     0,   6,        3, { 0, 0 } },	/* 5e: pl20_at_ssand_last, Super Sand Splash, the last shot's hit */
};
