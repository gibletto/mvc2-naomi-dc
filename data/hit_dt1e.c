/* Akuma (character 1e): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl1e_at_ for its own (pl1e_def.h).
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
     box, box2  boxes of hit_1e that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_1e[] = {
	/* dmg                   type          kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,           ZU | NO_VEC,        A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                    ZU,        A_NOM,     K_LM,    2,   10,   20,     0, 0x1e,   4,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  33,   63,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,           ZU | NO_VEC,        A_NOM,      K_L,    1,   10,   30,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  34,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,           ZU | NO_VEC,        A_NOM,      K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,           ZU | NO_VEC,        A_NOM,      K_M,    2,   10,   20,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   3,  40,  65,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,           ZU | NO_VEC,        A_NOM,      K_L,    1,   10,   30,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  37,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,           ZU | NO_VEC,        A_NOM,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,        A_NOM,     K_LM,    2,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,         BU_O | NO_VEC,        A_NOM,     K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  20,   40, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,           ZU | NO_VEC,         A_LO,     K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,         A_LO,     K_LM,    2,   10,   20,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,            AS | NO_ZU,         A_LO,     K_LL,    1,   10,   30,     9, 0x1a,   6,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  43,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,         A_HI,      K_S,    1,   10,   14,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,         A_HI,     K_LM,    1,   10,   20,     6, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  45,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,         A_HI,     K_LM,    1,   10,   30,     0, 0x16,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  46,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,         A_HI,      K_S,    1,   10,   14,     0, 0x00,   5,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  47,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   30,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  49,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   4,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x18,   6,  0x60, MARK_N2, SC200, SD_NOR_M,   3,  40,  50,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12: pl1e_at_fwd_hp_1, Forward + HP overhead chop (step in), first hit */
	{   6,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x18,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   3,  50,  51,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 13: pl1e_at_fwd_hp_2, Forward + HP overhead chop, second hit on the animation's mark */
	{  13,           ZU | NO_VEC,        A_NOM,      K_M,    1,   10,   20,     0, 0x18,  10,  0x60, MARK_N1, SC200, SD_KIK_M,   0,  50,  72,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 14: pl1e_at_fwd_hk, Forward + HK hop kick */
	{   4,           ZU | NO_VEC,    A_SPECIAL,      K_M,    0,   10,   20,     0, 0x00,   8,  0x60,       3, SC200, SD_KIK_M,   5,  90,  79,   80,                       0,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 15: pl1e_at_kuujin, Tenma Kuujinkyaku (air dive kick, sp_kuujin) */
	{   4,           ZU | NO_VEC,         A_HI,      K_S,    1,   10,   20,     0, 0x00,   8,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 16: pl1e_at_jump_lk, Jump LK (his own animation for every jump) */
	{   7,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  55,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 17: pl1e_at_jump_mk, Jump MK without up held (up+MK uses the common 0x10) */
	{  13,           ZU | NO_VEC,         A_HI,      K_M,    1,   10,   20,     0, 0x16,  12,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  77,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 18: pl1e_at_jump_hk, Jump HK without up held (up+HK uses the common 0x11) */
	{   4,           ZU | NO_VEC,        A_NOM,     K_LM,    0,   10,   20,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   3,  40,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl1e_at_stand_mk_2, Standing MK, second hit on the animation's mark */
	{  13,         BU_O | NO_VEC,    A_SPECIAL,     K_LL,    0,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40,  20,   40, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 1a: pl1e_at_upper, Launcher (pl1e_upper) */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,                  BU_O, A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x04,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl1e_at_throw_k, Ground kick throw, set on the opponent */
	{  16,                     2, A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x06,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl1e_at_throw_p, Ground punch throw, set on the opponent */
	{  70,                     2,      A_HYPER,     K_LM,    4,   12,   20,     0, 0x06,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  90,  35,   36,                NO_SCALE,  0,     0, 10,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl1e_at_shungoku, Shun Goku Satsu (Raging Demon): the opponent's record when the flurry of hits ends */
	{  16,                  BU_O,        A_NOM,     K_LM,    4,   12,   20,     0, 0x04,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl1e_at_guruma, Jigoku Guruma command throw (HCB+HK, catch_com_throw) */
	{  16,                  BU_O, A_NOM | 0x40,     K_LM,    4,   12,   20,     0, 0x04,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl1e_at_air_throw, Air throw: lands with the opponent, then throws */
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
	{  14,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    4,   20,     0, 0x18,   6,  0x60,       3, SC200, SD_NOR_M,   0,  50,  56,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30 */
	{  14,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    4,   20,     0, 0x18,  10,  0x60,       3, SC200, SD_NOR_M,   0,  50,  56,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31 */
	{ 0 },	/* 32 */
	{  19,                  BU_O,    A_SPECIAL,  K_L | 6,    0,   10,   30,     0, 0x1d,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,   59,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33 */
	{  16,                  BU_O,    A_SPECIAL,  K_L | 6,    0,   10,   30,     0, 0x1d,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,   59,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 34 */
	{  16,         BU_O | NO_VEC,    A_SPECIAL,  K_L | 6,    0,   10,   30,     0, 0x1e,   8,  0x80,       3, SC300, SD_NOR_L,   0,  90,  68,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 35 */
	{   4,         BU_O | NO_VEC,    A_SPECIAL,  K_L | 6,    0,   10,   30,     0, 0x1e,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 36 */
	{   4,                  BU_O,    A_SPECIAL,  K_L | 6,    0,   10,   30,     0, 0x1e,   9,  0x80,       3, SC300, SD_NOR_L,   0,  90,  59,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 37 */
	{ 0 },	/* 38 */
	{ 0 },	/* 39 */
	{   6,    2 | NO_VEC | NO_ZU,    A_SPECIAL,  K_S | 3,    0,   10,   30,     0, 0x1c,   3,  0x80,       3, SC300, SD_KIK_L,   0,  90,  60,   61,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3a */
	{   5,    2 | NO_VEC | NO_ZU,    A_SPECIAL,  K_S | 3,    0,   10,   30,     0, 0x1c,   3,  0x80,       3, SC300, SD_KIK_L,   0,  90,  60,   61,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3b */
	{ 0 },	/* 3c */
	{  14,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   10,   30,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,  60,   61,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3d */
	{  17,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,  K_L | 3,    0,   10,   30,     0, 0x1c,   8,  0x80,       3, SC300, SD_KIK_L,   0,  90,  60,   61,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 3e */
	{ 0 },	/* 3f */
	{  13,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    4,   20,     0, 0x18,   6,  0x60,       3, SC200, SD_NOR_M,   0,  50, 101,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 40 */
	{  13,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,      K_L,    0,    4,   20,     0, 0x18,   6,  0x60,       3, SC200, SD_NOR_M,   0,  50, 101,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 41 */
	{ 0 },	/* 42 */
	{  12,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,        1,    0,   10,   20,     0, 0x18,   6,  0x60,       3, SC200, SD_NOR_M,   0,  50,  56,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 43 */
	{  12,   ZU | NO_VEC | NO_ZU,    A_SPECIAL,        3,    0,   10,   20,     0, 0x18,   6,  0x60,       3, SC200, SD_NOR_M,   0,  50,  56,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 44 */
	{ 0 },	/* 45 */
	{ 0 },	/* 46 */
	{ 0 },	/* 47 */
	{ 0 },	/* 48 */
	{   4,   ZU | NO_VEC | NO_ZU,      A_HYPER, K_LL | 6,    0,    6,   20,     0, 0x18,   4,  0x60,       3, SC200, SD_NOR_M,   0,  90, 100,   99,                       0,  1,     0,  7,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{   2,   ZU | NO_VEC | NO_ZU,      A_HYPER, K_LL | 6,    0,    6,   20,     0, 0x1e,  11,  0x60,       3, SC200, SD_NOR_M,   0,  90, 100,   98,                       0,  1,     0,  7,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{  20,                  BU_O,      A_HYPER, K_LL | 6,    0,   20,   30,     0, 0x1b,  11,  0x60,       3, SC200, SD_NOR_L,   0,  90, 100,   99,                       0,  1,     0,  7,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{ 0 },	/* 4c */
	{ 0 },	/* 4d */
	{ 0 },	/* 4e */
	{  10,         BU_O | NO_VEC,      A_HYPER, K_LL | 6,    0,   20,   30,     0, 0x1d,   9,  0x80,       3, SC300, SD_NOR_L,  16,  90, 100,   99,               NO_WEIGHT,  1,     0,  7,     0,     0,   0,        3, { 0, 0 } },	/* 4f: pl1e_at_gou_shouryuu, Messatsu Gou Shouryuu hyper, first record (later hits come from cg_flag1) */
	{ 0 },	/* 50 */
	{ 0 },	/* 51 */
	{  10,           10 | NO_VEC,    A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 104,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 52: pl1e_at_t_kick, the partner's jumping kick as he comes in (t_kick) */
	{   4, BU_O | NO_VEC | NO_ZU,      A_HYPER,  K_L | 3,    0,    3,   30,     0, 0x00,   6,  0x80,       3, SC300, SD_KIK_L,   0,  90,  60,   61,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 53: pl1e_at_gou_rasen, Messatsu Gou Rasen hyper, its repeated rising hits */
	{  16,        2 | FLY_SCREEN,      A_HYPER,      K_L,    0,   15,   30,     0, 0x1a,  15,  0x80,       3, SC300, SD_KIK_L,   0,  90,  58,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 54: pl1e_at_gou_rasen_last, Messatsu Gou Rasen, the last and strongest hit */
	{   4,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_L | 6,    0,    4,   20,    10, 0x1a,   6,  0x60,       6, SC200, SD_NOR_M,   0,  90, 109,  111,                       0,  1,     0,  8,     0,     0,   6,        3, { 0, 0 } },	/* 55: pl1e_at_gou_hadou, Messatsu Gou Hadou hyper beam (Akuma and shl1e_01's beam) */
	{   4,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_L | 6,    0,    4,   20,    10, 0x1a,   6,  0x60,       6, SC200, SD_NOR_M,   0,  90, 109,  111,                 NO_CHIP,  1,     0,  8,     0,     0,   6,        3, { 0, 0 } },	/* 56 */
	{   4,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_L | 3,    0,   10,   20,     4, 0x14,  10,  0x60,       3, SC200, SD_NOR_M,   0,  90, 101,    0,               NO_WEIGHT,  1,     0,  9,     0,     0,   5,        3, { 0, 0 } },	/* 57: pl1e_at_tenma, Tenma Gou Zankuu (air fireball hyper) */
	{   4,    2 | NO_VEC | NO_ZU,      A_HYPER,  K_L | 3,    0,   10,   20,     4, 0x14,  10,  0x60,       3, SC200, SD_NOR_M,   0,  90, 101,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  9,     0,     0,   5,        3, { 0, 0 } },	/* 58 */
	{  10,           12 | NO_VEC,    A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90,  50,   51,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 59: pl1e_at_snapback, Snapback (sp_snapback) */
};
