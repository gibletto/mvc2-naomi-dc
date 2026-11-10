/* Hayato (character 12): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl12_at_ for its own (pl12_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap, 17 dm_slam, 19 dm_sandbag).
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
     box, box2  boxes of hit_12 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_12[] = {
	/* dmg                 type                 kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                  ZU,               A_NOM,  K_S,    4,   10,   10,     0, 0x19,   4,  0x70,      13, SC100,       13,   0,  20,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                  ZU,               A_NOM,  K_M,    2,   11,   16,     0, 0x19,   6,  0x80,      13, SC200,       14,   0,  40,  49,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                  ZU,               A_NOM,  K_L,    1,   12,   24,     0, 0x1c,   8,  0x80,      14, SC300,       15,   0,  50,  50,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,               A_NOM,  K_S,    4,   10,   10,     0, 0x1a,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  51,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                  ZU,               A_NOM, K_LM,    2,   10,   16,     0, 0x19,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  30,  52,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  10, ZU | NO_VEC | NO_ZU,               A_NOM, K_LL,    1,    6,   24,     0, 0x19,   0,  0x80,      13, SC300,       15,   0,  50,  53,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                  ZU,               A_NOM, K_LS,    4,   10,   10,     0, 0x19,   4,  0x70,      13, SC100,       13,   0,  20,  55,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                  ZU,               A_NOM, K_LM,    2,   11,   16,     0, 0x18,   6,  0x80,      13, SC200,       14,   0,  40,  56,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,       BU_O | NO_VEC,               A_NOM,  K_L,    1,   10,   24,     0, 0x00,  11,  0x80,      11, SC300,       15,   0,  50,  57,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                  ZU,                A_LO, K_LS,    4,   10,   10,     0, 0x19,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  58,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                  ZU,                A_LO, K_LM,    2,   11,   16,     0, 0x19,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  59,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                  AS,                A_LO, K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70,      14, SC300,       15,   0,  90,  60,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,         ZU | NO_VEC,                A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x70,      14, SC100,       13,   0,  20,  61,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,         ZU | NO_VEC,                A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x80,      13, SC200,       14,   0,  40,  62,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,         ZU | NO_VEC,                A_HI,  K_L,    1,   12,   32,     0, 0x15,   8,  0x80,      15, SC300,       15,   0,  60,  63,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,                A_HI,  K_S,    1,   10,   10,     0, 0x00,   6,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  64,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,         ZU | NO_VEC,                A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  65,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,         ZU | NO_VEC,                A_HI,  K_L,    1,   12,   32,     0, 0x16,   8,  0x60,      14, SC200,       15,   0,  60,  66,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,       BU_O | NO_VEC,           A_SPECIAL,  K_L,    0,   10,   24,     0, 0x00,  11,  0x80,      11, SC300,       15,   0,  40,  87,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl12_at_upper, the launcher (pl12_upper) */
	{  13,         ZU | NO_VEC,               A_NOM,  K_L,    2,   12,   16,     0, 0x1a,  10,  0x60,      13, SC200,       15,   2,  50,  70,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 13: pl12_at_fwd_hp, Forward + standing HP (stand_punch, Char_Stand_P 3) */
	{  12,         ZU | NO_VEC,                A_HI,  K_L,    1,   12,   16,     0, 0x10,   8,  0x60,      15, SC200,       15,   4,  60,  67,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 14: pl12_at_jdown_hk, Jumping down + HK, the dive (jump_kick) */
	{  10,                  19,               A_NOM,  K_M,    1,    0,   28,     0, 0x1a,   8,  0x60,      13, SC200,       15,   4,  20,  68,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 15: pl12_at_dash_hp, Standing HP that lunges forward (stand_punch_dash, used when atck_type is 3 and jump_dir is 0, Char_Extra 3) */
	{  10,                BU_O,               A_NOM,  K_L,    0,   10,   30,     0, 0x1c,   8,  0x80,      14, SC300,       15,   0,  60,  69,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 16: pl12_at_rush_hp, the third HP's rush-in hit when HP is held (rush_move) */
	{  10,         ZU | NO_VEC,               A_NOM,  K_L,    1,   12,   32,     0, 0x1c,   5,  0x80,      14, SC300,       15,   0,  60,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 17: pl12_at_stand_hk_2, Standing HK's second hit (stand_hk_move) */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{ 0 },	/* 1b */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  16,       BU_O | NO_VEC,        A_NOM | 0x40,  K_L,    0,   10,   20,     0, 0x1e,  11,  0x60,      13, SC200,       15,   0,  40,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl12_at_throw_p, Ground punch throw (nml_p_catch) */
	{ 0 },	/* 21: pl12_at_throw_k, Ground kick throw (nk_land) */
	{  16,         17 | NO_VEC,        A_NOM | 0x40,  K_L,    0,   10,   20,     0, 0x1a,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl12_at_air_throw_p, Air punch throw (jp_land) */
	{  20,                   2,    A_SPECIAL | 0x40,  K_L,    0,    0,   20,     0, 0x1f,  11,  0x40, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,               NO_WEIGHT,  0,     0, 10,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl12_at_air_throw_k, Air kick throw (jump_k_catch) */
	{  60,                   2,      A_HYPER | 0x40,  K_L,    0,    0,   20,     0, 0x1c,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  90,  48,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl12_at_black_end, Black Hayato's grab: last hit on the held opponent (black_cut_end) */
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
	{   8,          ZU | NO_ZU,           A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1a,   4,  0x60,      13, SC200,       16,   0,  60,  88,  161,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 30: pl12_at_shiden, Shiden's first slash, light version (atck_level + 0x30); Byakkou Hou's throwing slash (byakko_init) sets it too */
	{  10,          ZU | NO_ZU,           A_SPECIAL,  K_L,    0,    8,   30,     0, 0x1a,   4,  0x80,      13, SC300,       16,   0,  60,  90,    0,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 31 */
	{  12,                BU_O,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1c,   8,  0x80,      14, SC300,       16,   0,  60,  89,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 32 */
	{  14,                BU_O,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1c,   9,  0x80,      14, SC300,       16,   0,  60,  91,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33 */
	{   6,       BU_O | NO_VEC,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x00,   8,  0x80,      11, SC300,       16,   0,  60,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 34 */
	{   8,       BU_O | NO_VEC,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x00,   8,  0x80,      11, SC300,       16,   0,  60,  93,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 35 */
	{  16,                  10,             A_HYPER,  K_L,    0,    6,   20,     0, 0x1c,   8,  0x60,       3, SC200,        8,   0,  90,  71,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 36: pl12_at_field, Plasma Field (hyper): Hayato and the field (field_init here and in shl12_00) */
	{   5,          ZU | NO_ZU,             A_HYPER,  K_L,    0,    8,   30,     0, 0x1c,   8,  0x80,      13, SC300,       16,   0,  90,  94,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 37: pl12_at_rasetsu, Rasetsu Zan (hyper), the run of slashes (rasetsu_init) */
	{   5,          ZU | NO_ZU,             A_HYPER,  K_L,    0,    8,   30,     0, 0x1c,   8,  0x80,      14, SC300,       16,   0,  90,  95,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 38 */
	{   5,          ZU | NO_ZU,             A_HYPER, K_LM,    0,    8,   30,     0, 0x1c,   8,  0x80,      13, SC300,       16,   0,  90,  96,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39 */
	{   5,          ZU | NO_ZU,             A_HYPER,  K_L,    0,    8,   30,     0, 0x1c,   8,  0x80,      14, SC300,       16,   0,  90,  97,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3a */
	{   5,          ZU | NO_ZU,             A_HYPER, K_LL,    0,    8,   30,     0, 0x1c,   8,  0x80,      14, SC300,       16,   0,  90,  98,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3b */
	{   5,          ZU | NO_ZU,             A_HYPER,  K_L,    0,    8,   30,     0, 0x1c,   8,  0x80,      12, SC300,       16,   0,  90,  99,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3c */
	{  20,                BU_O,             A_HYPER,  K_L,    0,   10,   30,     0, 0x1c,  10,  0x80,      14, SC300,       16,   0,  90, 100,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3d */
	{  10,  2 | NO_VEC | NO_ZU,             A_HYPER,  K_L,    0,   10,   30,     0, 0x19,  12,  0x80,       3, SC300,        8,   0,  90, 101,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   8,        3, { 0, 0 } },	/* 3e: pl12_at_engetsu_2, Engetsu's waves (shl12_00's engetsu_init) */
	{   4,          2 | NO_VEC,             A_HYPER,  K_L,    0,   10,   30,     0, 0x1a,  10,  0x80,      14, SC300,       16,   0,  90, 104,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f: pl12_at_engetsu, Engetsu (hyper): Hayato himself (engetsu_init) */
	{ 0 },	/* 40 */
	{  10,         10 | NO_VEC,           A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  85,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 41: pl12_at_t_kick, the partner's jump-in attack (t_kick) */
	{  10, 12 | NO_VEC | NO_ZU,           A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,      14, SC300,       16,  64,  90,  86,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 42: pl12_at_snapback, Snapback (sp_snapback) */
	{  18, ZU | NO_VEC | NO_ZU,             A_HYPER,  K_L,    0,    8,   30,     0, 0x08,   0,  0x80,       3, SC300,       16,   0,  90, 103,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43: pl12_at_black, Black Hayato (sp_black): the lunging grab */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1c,   6,  0x80,      14, SC200,       16,   0,  60,  71,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 44: pl12_at_back_lp, Back + standing LP, chain opener (Char_Extra 5) */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1c,   6,  0x80,      12, SC200,       16,   0,  60,  72,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 45: pl12_at_chain_06, Chained attack, nml_no 3 (Char_Extra 6) */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1f,   6,  0x80,      12, SC200,       16,   0,  60,  73,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 46: pl12_at_chain_07, Chained attack, nml_no 4 (Char_Extra 7) */
	{   5,       BU_O | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1c,   9,  0x80,      14, SC200,       16,   0,  60,  74,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 47: pl12_at_chain_08, Chained attack, nml_no 5 (Char_Extra 8) */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1a,   6,  0x80,      13, SC200,       16,   0,  60,  75,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 48: pl12_at_chain_09, Chained attack, nml_no 6 (Char_Extra 9) */
	{   5,         ZU | NO_VEC,                A_LO,  K_M,    2,   11,   16,     0, 0x19,   6,  0x80,       3, SC200,       12,   0,  60,  76,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 49: pl12_at_chain_10, Chained kick, nml_no 3 (Char_Extra 10) */
	{   5,                  10,               A_NOM,    3,    0,   20,    0,     0, 0x1f,   6,  0x80,      13, SC100,       16,   5,  60,  77,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 4a: pl12_at_chain_11, Chained attack, nml_no 7 (Char_Extra 11) */
	{  10,                BU_O,      A_HI | A_HYPER,    3,    0,   20,   16,     0, 0x1b,  10,  0x80,      15, SC200,       16,   0,  60,  78,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 4b: pl12_at_chain_12, Chained attack, nml_no 8 (Char_Extra 12) */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1c,   6,  0x80,      13, SC200,       16,   0,  60,  79,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 4c: pl12_at_back_hp, Back + standing HP when no light or medium attack started the string (Char_Extra 13) */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1c,   6,  0x80,      13, SC200,       16,   0,  60,  80,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 4d: pl12_at_chain_14, Chained attack, nml_no 9 (Char_Extra 14) */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1c,   6,  0x80,      14, SC200,       16,   0,  60,  81,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 4e: pl12_at_chain_15, Chained attack, nml_no 10 (Char_Extra 15) */
	{   5,         ZU | NO_VEC,               A_NOM,  K_M,    2,   11,   16,     0, 0x1c,   6,  0x80,      14, SC200,       16,   0,  60,  82,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 4f: pl12_at_chain_16, Chained attack, nml_no 11 (Char_Extra 16) */
	{   5,         ZU | NO_VEC,                A_LO,  K_M,    2,   11,   16,     0, 0x1c,   6,  0x80,       3, SC200,       12,   0,  60,  83,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 50: pl12_at_chain_17, Chained kick, nml_no 4 (Char_Extra 17) */
	{  12,                BU_O, A_SPECIAL | A_HYPER,  K_L,    0,   10,   30,     0, 0x1e,  10,  0x80,      11, SC300,       16,   0,  60,  84,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 51: pl12_at_chain_18, Chained attack, nml_no 12 (Char_Extra 18) */
	{   6,                BU_O,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1e,   8,  0x80,      11, SC300,       16,   0,  60,  91,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 52 */
	{   8,                BU_O,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1e,  10,  0x80,      11, SC300,       16,   0,  60,  92,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 53 */
	{  10,          ZU | NO_ZU,           A_SPECIAL,  K_L,    0,    6,   20,     0, 0x1a,   4,  0x60,      13, SC200,       16,   0,  60,  88,  161,                       0,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 54: pl12_at_shiden_vc, Shiden's first dash as a variable counter (shiden_init, team_type 3) */
	{  14,                BU_O,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1c,   9,  0x80,      14, SC300,       16,   0,  60,  91,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 55: pl12_at_shiden_vc_2, Shiden's second dash as a variable counter (shiden_next, team_type 3) */
	{   8,       BU_O | NO_VEC,           A_SPECIAL,  K_L,    0,   10,   30,     0, 0x1f,  11,  0x80,      11, SC300,       16,   0,  60,  93,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 56: pl12_at_guren_vc, Guren as a variable counter, a rising slash (sp_counter) */
};
