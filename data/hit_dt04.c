/* Anakaris (character 04): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl04_at_ for its own (pl04_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap, 19 dm_sandbag, 20 dm_curse).
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
     box, box2  boxes of hit_04 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_04[] = {
	/* dmg                 type       kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                  ZU,     A_NOM,     K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,   9,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,                  ZU,     A_NOM,    K_LM,    2,   11,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  11,   12,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,                  ZU,     A_NOM,     K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  15,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                  ZU,     A_NOM,     K_S,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  17,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                  ZU,     A_NOM,     K_M,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  19,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                  ZU,     A_NOM,     K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  21,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                  ZU,     A_NOM,    K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   4,                  ZU,     A_NOM,    K_LM,    1,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  20,  25,   26,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,       BU_O | NO_VEC,     A_NOM,    K_LL,    1,   10,   24,     0, 0x00,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50,  29,   30, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                  ZU,      A_LO,    K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  32,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                  ZU,      A_LO,    K_LM,    2,   11,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  34,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                  AS,      A_LO,    K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90,  36,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,         ZU | NO_VEC,      A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  38,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,         ZU | NO_VEC,      A_HI,    K_LM,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  40,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,         ZU | NO_VEC,      A_HI,    K_LM,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  42,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,         ZU | NO_VEC,      A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  64,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,         ZU | NO_VEC,      A_HI,     K_M,    1,   10,   16,     0, 0x16,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  66,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,         ZU | NO_VEC,      A_HI,     K_S,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  68,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   7,         ZU | NO_VEC,      A_HI,    K_LM,    2,   10,   16,     0, 0x18,   6,  0x60, MARK_N2, SC200, SD_NOR_M,   2,  40,  45,   46,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 12: pl04_at_back_lk, Stand LK with the stick back */
	{   7,         ZU | NO_VEC,      A_HI,    K_LL,    2,   12,   16,     0, 0x18,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   2,  40,  45,   46,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 13 */
	{   7,         ZU | NO_VEC,      A_HI,     K_M,    1,   10,   16,     0, 0x16,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  60,  60,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 14: pl04_at_jdown_k, Jump kick with the stick down (drop) */
	{   7,         ZU | NO_VEC,      A_HI,     K_M,    1,   10,   16,     0, 0x16,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  60,  61,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 15: pl04_at_jdiag_k, Jump kick with the stick down-forward or down-back */
	{   4,         ZU | NO_VEC,      A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20,  70,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 16: pl04_at_sjump_lk, Super jump LK */
	{   7,         ZU | NO_VEC,      A_HI,     K_M,    1,   10,   16,     0, 0x16,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  72,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 17: pl04_at_sjump_mk, Super jump MK */
	{  13,         ZU | NO_VEC,      A_HI,     K_S,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60,  74,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 18: pl04_at_sjump_hk, Super jump HK */
	{   7,         ZU | NO_VEC,      A_HI,     K_M,    1,   10,   16,     0, 0x16,   5,  0x80, MARK_N2, SC200, SD_KIK_L,   0,  60,  59,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 19: pl04_at_jfwd_k, Jump kick with the stick forward only (the dive) */
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
	{  20, ZU | NO_VEC | NO_ZU, A_SPECIAL,     K_L,    0,    6,   20,     0, 0x1a,   8,  0x60,       3, SC200,        8,   0,  50,  57,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl04_at_coffin, Coffin Dance on the ground (Anakaris and the coffin shot) */
	{  20, ZU | NO_VEC | NO_ZU, A_SPECIAL,     K_L,    0,    6,   20,     0, 0x1a,   8,  0x60,       3, SC200,        8,   0,  90,  57,    0,                       0,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 31: pl04_at_jcoffin, Coffin Dance in the air; also the coffin shot as a variable counter */
	{  10,          20 | NO_ZU, A_SPECIAL,     K_L,    0,   10,   20,     0, 0x1c,   8,  0x60,       3, SC200,        0,   0,  90,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 32 */
	{  10,          20 | NO_ZU, A_SPECIAL,     K_L,    0,   10,   20,     0, 0x1c,   8,  0x60,       3, SC200,        0,   0,  90,  58,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 33 */
	{  16,                BU_O, A_SPECIAL,     K_L,    0,    8,   30,     0, 0x1d,   8,  0x80,       3, SC300,        8,   0,  90,  49,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 34 */
	{  18,                BU_O, A_SPECIAL,    K_LL,    0,   10,   30,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0,  90,  51,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 35 */
	{  16,                BU_O, A_SPECIAL,     K_L,    0,    8,   30,     0, 0x1e,   8,  0x80,       3, SC300,        8,   0,  90,  53,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 36 */
	{  18,                BU_O, A_SPECIAL,    K_LL,    0,   10,   30,     0, 0x1e,  10,  0x80,       3, SC300,        8,   0,  90,  55,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 37 */
	{   2,                BU_O, A_SPECIAL,     K_L,    0,    8,   30,     0, 0x0e,   8,  0x80,       3, SC300,        8, 128, 100,  59,    0,                   NO_KO,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 38: pl04_at_throw_hit, Throw's hits on the ground (catch_hit) */
	{   2,        BU_O | NO_ZU, A_SPECIAL,     K_L,    0,   10,   30,     0, 0x0e,  10,  0x80,       3, SC300,       12, 128,  90,  60,    0,                   NO_KO,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 39: pl04_at_air_throw_hit, Air throw's hits (catch_hit) */
	{  14,           2 | NO_ZU, A_SPECIAL,     K_L,    0,   10,    1,     0, 0x04,   8,  0x80,       3, SC300,       12,   0,  90,  60,    0,                       0,  1,     0,  0,     0,    30,   0, YK_GD030, { 0, 0 } },	/* 3a: pl04_at_throw, the throw that ends it on the ground (catch_end) */
	{  14,           2 | NO_ZU, A_SPECIAL,     K_L,    0,   10,    1,     0, 0x0e,  10,  0x80,       3, SC300,       12,   0,  90,  60,    0,                       0,  1,     0,  0,     0,    30,   0, YK_GD030, { 0, 0 } },	/* 3b: pl04_at_air_throw, the throw that ends it in the air (catch_end) */
	{  20,                BU_O, A_SPECIAL,    K_LL,    0,   10,   30,     0, 0x1e,   9,  0x80,       3, SC300,        8,   0,  90,  51,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3c: pl04_at_vc_cobra, Cobra Blow as a variable counter (otherwise 52+level) */
	{ 0 },	/* 3d */
	{ 0 },	/* 3e */
	{ 0 },	/* 3f */
	{  10,                  19,   A_HYPER, K_L | 2,    0,   10,  120,     0, 0x1c,   8,  0x60,       3, SC200,        8,   0,  90,  79,    0,       NO_WEIGHT | NO_KO,  1,     0,  4,     0,     0,   0,        4, { 0, 0 } },	/* 40: pl04_at_magic, Pharaoh Magic (hyper), ground and air */
	{  10,                  19,   A_HYPER, K_L | 1,    0,   30,  120,     0, 0x1c,   8,  0x60,       3, SC200,        8,   0,  90,  80,    0,       NO_WEIGHT | NO_KO,  1,     0,  4,     0,     0,   0,        4, { 0, 0 } },	/* 41 */
	{  10,                  19,   A_HYPER, K_L | 3,    0,   30,  120,     0, 0x1c,   8,  0x60,       3, SC200,        8,   0,  90,  80,    0,       NO_WEIGHT | NO_KO,  1,     0,  4,     0,     0,   0,        4, { 0, 0 } },	/* 42 */
	{  30,  2 | NO_VEC | NO_ZU,   A_HYPER,     K_L,    0,   10,   10,    50, 0x10,  10,  0x60,       3, SC200,        8,   0,  90,  81,    0,               NO_WEIGHT,  1,     0,  9,     0,     0,   0,        4, { 0, 0 } },	/* 43: pl04_at_magic_coffin, the coffin dropped after Pharaoh Magic's fourth shot (shl04_00 type 2) */
	{   3, ZU | NO_VEC | NO_ZU,   A_HYPER,    K_LL,    0,    6,   20,     0, 0x18,   4,  0x60,       3, SC200,        8,   0,  90,  58,    0,               NO_WEIGHT,  1,     0,  7,     0,     0,   5,        3, { 0, 0 } },	/* 44: pl04_at_cobras, Pharaoh Cobra Throw (on Anakaris); its rows' heads and parts one frame in four */
	{   3, ZU | NO_VEC | NO_ZU,   A_HYPER,    K_LL,    0,    6,   20,     0, 0x18,   4,  0x60,       3, SC200,        8,   0,  90,  58,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  7,     0,     0,   5,        3, { 0, 0 } },	/* 45: pl04_at_cobras_2, the rows' heads and parts on the other frames (shl04_03) */
	{ 0 },	/* 46 */
	{ 0 },	/* 47 */
	{  10,       BU_O | NO_VEC,   A_HYPER,     K_L,    0,   10,   20,     0, 0x00,  11,  0x80,       3, SC200,       12,   0,  90,  82,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 48 */
	{  10,          2 | NO_VEC,   A_HYPER,     K_L,    0,   10,   20,     0, 0x1d,  11,  0x80,       3, SC200,        8,   0,  90,  77,    0,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
	{  10,          2 | NO_VEC,   A_HYPER,     K_L,    0,   10,   20,     0, 0x1b,  10,  0x60,       3, SC200,        8,   0,  90,  83,    0,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 4a */
	{  10,          2 | NO_VEC,   A_HYPER,     K_L,    0,   10,   20,     0, 0x19,  10,  0x60,       3, SC200,        8,   0,  90,  77,    0,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 4b */
	{  10,          2 | NO_VEC,   A_HYPER,     K_L,    0,   10,   20,     0, 0x10,   6,  0x60,       3, SC200,        8,   0,  90,  84,    0,               NO_WEIGHT,  1,     0,  4,     0,     0,   0,        3, { 0, 0 } },	/* 4c */
	{  10,         10 | NO_VEC,   A_HYPER,    K_LL,    0,   10,   60,     0, 0x1f,  11,  0x60,       3, SC200,        8,   0,  40,  82,    0,               NO_WEIGHT,  1,     0,  5,     0,     0,   0,        3, { 0, 0 } },	/* 4d: pl04_at_illusion, Pharaoh Illusion (hyper) */
	{ 0 },	/* 4e */
	{ 0 },	/* 4f */
	{  15, ZU | NO_VEC | NO_ZU,   A_HYPER,     K_L,    0,   10,   20,     0, 0x00,   8,  0x60,       3, SC200,        8,   5,  90,  57,    0,                       0,  1,     0,  0,     0,    20,   0,        3, { 0, 0 } },	/* 50: pl04_at_coffins, Coffin Fall (hyper), ground and air, and its coffins (shl04_00 type 1) */
	{  15, ZU | NO_VEC | NO_ZU,   A_HYPER,     K_L,    0,   10,   20,     0, 0x00,   8,  0x60,       3, SC200,        8,   5,  90,  57,    0,                 NO_CHIP,  1,     0,  0,     0,    20,   0,        3, { 0, 0 } },	/* 51 */
	{ 0 },	/* 52 */
	{ 0 },	/* 53 */
	{  10,         10 | NO_VEC, A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  98,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 54: pl04_at_t_kick, Tag-in kick off the wall (t_kick) */
	{  10, 12 | NO_VEC | NO_ZU, A_SPECIAL,     K_L,    1,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64,  90,  45,   46,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 55: pl04_at_snapback, Snapback (sp_snapback) */
	{  16,       BU_O | NO_VEC, A_SPECIAL,     K_M,    0,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_L,   0,  40,  16,   94, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 56: pl04_at_upper, the launcher (pl04_upper) */
};
