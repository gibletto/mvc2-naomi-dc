/* Cable (character 17): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl17_at_ for its own (pl17_def.h).
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
     box, box2  boxes of hit_17 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_17[] = {
	/* dmg                   type              kind      elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,            A_NOM,      K_S,    4,    8,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 247,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,         BU_O | NO_VEC,            A_NOM,      K_M,    2,   12,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 246,   94, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                    ZU,            A_NOM,      K_L,    1,   16,   24,     0, 0x1c,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50, 245,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,            A_NOM,      K_S,    4,    8,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20, 244,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                    ZU,            A_NOM,      K_M,    2,   12,   16,     0, 0x1a,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 243,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,                    ZU,            A_NOM,      K_L,    1,   16,   24,     0, 0x1c,  11,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50, 242,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,                    ZU,            A_NOM,     K_LS,    4,    8,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 240,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,            A_NOM,     K_LM,    2,   12,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 239,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,                  BU_O,            A_NOM,     K_LL,    1,   16,   24,     0, 0x1d,  10,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50, 238,   40,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,                    ZU,             A_LO,     K_LS,    4,    8,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20, 237,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,                    ZU,            A_NOM,     K_LM,    2,   12,   16,     0, 0x1d,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40, 236,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                    AS,             A_LO,     K_LL,    1,    8,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100, 235,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,      K_S,    1,    8,   10,     0, 0x00,   7,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 232,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,             A_HI,     K_LM,    1,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 231,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  12,           ZU | NO_VEC,             A_HI,      K_M,    1,    8,   32,     0, 0x17,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50, 241,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,      K_S,    1,    8,   10,     0, 0x00,   7,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20, 229,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,             A_HI,      K_M,    1,   12,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 228,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,             A_HI,      K_L,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60, 227,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  13,           ZU | NO_VEC,            A_NOM,     K_LM,    1,   10,   16,     0, 0x18,   6,  0x60, MARK_N3, SC200, SD_NOR_M,   0,  50, 233,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 12: pl17_at_fwd_hk, Forward + HK */
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
	{  16,                  BU_O,     A_NOM | 0x40,     K_LM,    4,   10,   20,     0, 0x04,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl17_at_throw_k, Kick throw */
	{  16,                    10,     A_NOM | 0x40, K_LM | 3,    4,   10,   20,     0, 0x1a,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21 */
	{   1,                  BU_O,     A_NOM | 0x40, K_LM | 3,    4,   10,   20,     0, 0x18,  10,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 22 */
	{  16,             2 | NO_ZU, A_SPECIAL | 0x40,  K_L | 3,    4,   10,   20,     0, 0x18,  15,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl17_at_psycharge_2, Psy-Charge: the opponent let go at the end */
	{   2,             2 | NO_ZU, A_SPECIAL | 0x40,  K_L | 3,    4,   12,   20,     0, 0x04,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,   36,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl17_at_psycharge_hit, Psy-Charge: the electric hit on the held opponent (cg_type & 8) */
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
	{   4,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,      K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   0,  18, 224,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 30 */
	{  12,            ZU | NO_ZU, A_SPECIAL | 0x80,     K_LS,    0,   10,   20,    60, 0x00,   2,  0x80,       3, SC300,       19,   0,  50, 221,    0,                       0,  1,     0,  0,     1,     0,   0,        5, { 0, 0 } },	/* 31: pl17_at_electrap, Electrap: the grenade (shl17_01_init) */
	{  10,                    ZU,        A_SPECIAL,      K_S,    0,    0,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  90, 219,    0,                NO_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 32: pl17_at_psycharge, Psy-Charge: the grab */
	{  17,                     2,        A_SPECIAL, K_LL | 3,    0,    6,   30,     0, 0x1a,  15,  0x80,       3, SC300,        8,   0, 100, 225,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33: pl17_at_crackdown_l, Crackdown, light */
	{  10,           10 | NO_VEC,        A_SPECIAL,      K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 227,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 34: pl17_at_t_kick, Tag-in kick that leaps off the wall */
	{  10,         BU_O | NO_VEC,        A_SPECIAL,      K_M,    0,   12,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 226,   94, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 35: pl17_at_upper, the launcher, 3HP */
	{   4,                  BU_O,        A_SPECIAL,      K_L,    0,    8,   30,     0, 0x1e,  11,  0x80,       3, SC300,        8,   0,  50, 218,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 36: pl17_at_crackdown_blast, Crackdown: the blast at the end of the charge (shl17_04) */
	{  12,            ZU | NO_ZU, A_SPECIAL | 0x80, K_LS | 3,    0,   10,   20,    60, 0x00,   2,  0x80,       3, SC300,       19,   0,  50, 217,    0,                       0,  1,     0,  0,     8,     0,   0,        4, { 0, 0 } },	/* 37: pl17_at_electrap_burst, Electrap: Cable's throw and the grenade's shock (shl17_01_burst) */
	{   4,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,      K_L,    0,    4,   20,     0, 0x00,   9,  0x80,       4, SC300,       18,   0,  18, 224,    0,                 NO_CHIP,  1,     0, 12,     0,     0,   2,        8, { 0, 0 } },	/* 38 */
	{   4, BU_O | NO_VEC | NO_ZU,   A_HYPER | 0x80,      K_L,    0,   10,   30,     0, 0x1d,  10,  0x80,       4, SC300,       18,   0,  90, 224,    0,                 NO_CHIP,  1,     0, 11,     0,     0,   6,        8, { 0, 0 } },	/* 39 */
	{   3, BU_O | NO_VEC | NO_ZU,   A_HYPER | 0x80,      K_L,    0,   10,   30,     0, 0x1d,  10,  0x80,       4, SC300,       18,   0,  90, 224,    0,                       0,  1,     0, 11,     0,     0,   6,        8, { 0, 0 } },	/* 3a */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 3,    0,   16,   20,     0, 0x1f,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90, 214,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3b: pl17_at_psimitar_l_2, Psimitar, light: the hit cued by the animation */
	{   2,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 3,    0,    4,   20,     0, 0x1e,  11,  0x80,       3, SC300, SD_NOR_L,   0,  50, 222,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3c: pl17_at_psimitar_shot_l, Psimitar, light: the shot fired up (shl17_02) */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 3,    0,   16,   20,     0, 0x1f,  13,  0x80,       3, SC300, SD_NOR_L,   0,  90, 214,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3d: pl17_at_psimitar_h_2, Psimitar, heavy: the hit cued by the animation */
	{   2,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 3,    0,    4,   20,     0, 0x1e,  14,  0x80,       3, SC300, SD_NOR_L,   0,  50, 222,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 3e: pl17_at_psimitar_shot_h, Psimitar, heavy: the shot fired up (shl17_02) */
	{  10,   12 | NO_VEC | NO_ZU,        A_SPECIAL,      K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,       12,  64, 100, 241,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3f: pl17_at_snapback, Snapback (sp_snapback); also shl17_06's flash kind 3, which nothing sets */
	{   4,   ZU | NO_VEC | NO_ZU,          A_HYPER,      K_S,    0,   16,   60,     0, 0x18,   8,  0x60,       3, SC200, SD_NOR_M,   5,  90, 210,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40: pl17_at_timeflip, Time Flip: Cable and the catching shot (shl17_05) */
	{   3,   ZU | NO_VEC | NO_ZU,          A_HYPER,      K_S,    0,   18,   60,     0, 0x18,   8,  0x60,       3, SC200,       12,   0,  90, 209,   57,                 NO_CHIP,  1,     0,  6,     0,     0,   0,        3, { 0, 0 } },	/* 41 */
	{   4,   ZU | NO_VEC | NO_ZU,          A_HYPER,  K_L | 2,    0,   10,   20,     4, 0x12,  10,  0x60,       3, SC200,        8,   0,  90, 206,    0,                       0,  1,     0,  9,     0,     0,   0,        3, { 0, 0 } },	/* 42: pl17_at_timeflip_shot, Time Flip: gunshot on the held opponent (shl17_06 kind 4) */
	{   5,    2 | NO_VEC | NO_ZU,          A_HYPER,  K_L | 2,    0,   10,   20,     4, 0x1e,  11,  0x60,       3, SC200,        8,   0,  90, 206,    0,                       0,  1,     0,  9,     0,     0,   0,        3, { 0, 0 } },	/* 43: pl17_at_timeflip_shot_2, Time Flip: the other gunshot (shl17_06 kind 5) */
	{   3,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,      K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   5,   8, 224,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 44 */
	{   3,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,      K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   5,   8, 224,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 45 */
	{   6,   ZU | NO_VEC | NO_ZU,            A_NOM, K_LL | 2,    0,   12,   32,     0, 0x16,   8,  0x80,       4, SC300,       22,   0,  50, 241,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 46 */
	{   6, BU_O | NO_VEC | NO_ZU,            A_NOM, K_LL | 2,    0,   12,   32,     0, 0x16,   8,  0x80,       4, SC300,       22,   0,  50, 241,    0,                       0,  0,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 47 */
	{   6,           ZU | NO_VEC,             A_HI,  K_M | 2,    1,    8,   32,     0, 0x17,   8,  0x80,       4, SC300,       22,   0,  60, 241,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 48 */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 3,    0,   16,   20,     0, 0x1f,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90, 214,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 49: pl17_at_psimitar_l, Psimitar, light: the rising leap */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,  K_L | 3,    0,   16,   20,     0, 0x1e,  12,  0x80,       3, SC300, SD_NOR_L,   0,  90, 214,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 4a: pl17_at_psimitar_h, Psimitar, heavy: the rising leap */
	{  20,                     2,        A_SPECIAL, K_LL | 3,    0,    6,   30,     0, 0x1a,  15,  0x80,       3, SC300,        8,   0, 100, 225,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 4b: pl17_at_crackdown_h, Crackdown, heavy */
};
