/* Sonson (character 14): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl14_at_ for its own (pl14_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 10 dm_away,
             12 dm_snap, 17 dm_slam).
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
     box, box2  boxes of hit_14 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_14[] = {
	/* dmg                   type              kind     elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   3,                    ZU,            A_NOM,     K_S,    4,   10,   10,     0, 0x18,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 128,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   6,                    ZU,            A_NOM,    K_LS,    2,    8,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 130,   63,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  12,                    ZU,            A_NOM,     K_L,    1,   12,   24,     0, 0x1a,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  50, 172,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   3,                    ZU,            A_NOM,    K_LS,    4,   10,   10,     0, 0x18,   4,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20, 131,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   6,                    ZU,            A_NOM,    K_LS,    2,    8,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 132,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  12,                  BU_O,            A_NOM,    K_LL,    1,   12,   24,     0, 0x1b,  10,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50, 134,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   3,                    ZU,            A_NOM,    K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 136,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   6,                    ZU,            A_NOM,    K_LS,    2,    3,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 137,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  12,                    AS,             A_LO,    K_LL,    1,   12,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  90, 139,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   3,                    ZU,             A_LO,    K_LS,    4,   10,   10,     0, 0x1c,   4,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20, 140,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   6,                  BU_O,             A_LO,    K_LM,    2,   10,   16,     0, 0x00,   3,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40, 141,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  12,         BU_O | NO_VEC,            A_NOM,    K_LL,    1,   12,   24,     9, 0x1f,  11,  0x70, MARK_N3, SC300, SD_KIK_L,   0,  50, 142,    0, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   3,           ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20, 143,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   6,           ZU | NO_VEC,             A_HI,    K_LM,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 144,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{   5,           ZU | NO_VEC,             A_HI,    K_LM,    1,    4,   20,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 145,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   3,           ZU | NO_VEC,             A_HI,     K_S,    1,   10,   10,     0, 0x00,   5,  0x60, MARK_N1, SC200, SD_KIK_S,   0,  20, 146,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   6,           ZU | NO_VEC,             A_HI,     K_M,    1,   10,   16,     0, 0x00,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 147,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  12,           ZU | NO_VEC,             A_HI,     K_S,    1,   12,   32,     0, 0x15,   8,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  60, 148,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{  10,                  BU_O,            A_NOM,    K_LL,    1,   18,   24,     0, 0x1b,  10,  0x80, MARK_N3, SC200, SD_NOR_L,   0,  40, 149,    0,                       0,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 12: pl14_at_fwd_hp, Forward + HP: the staff stretched far out (stand_punch, nml_no 2 with forward) */
	{ 0 },	/* 13 */
	{ 0 },	/* 14 */
	{ 0 },	/* 15 */
	{ 0 },	/* 16 */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{   6,                    ZU,            A_NOM,     K_M,    2,   10,   16,     0, 0x00,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 129,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 19: pl14_at_mp_2, Standing MP's second hit, set on cg_type */
	{   6,                    ZU,            A_NOM,     K_M,    2,   10,   16,     0, 0x1a,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40, 133,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1a: pl14_at_mk_2, Standing MK's second hit, set on cg_type */
	{   6,                    ZU,            A_NOM,    K_LM,    2,   11,   16,     0, 0x13,   6,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40, 138,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1b */
	{   5,           ZU | NO_VEC,             A_HI,    K_LM,    1,   12,   32,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60, 138,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 1c: pl14_at_jump_hp_2, Jumping HP's later hit (cg_flag1 2; cg_flag1 1 sets common 0x0e) */
	{  12,         BU_O | NO_VEC,        A_SPECIAL,    K_LL,    0,   10,   24,     0, 0x1f,  11,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  40, 133,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 1d: pl14_at_upper, the launcher */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{   3,                    17,     A_NOM | 0x40,    K_LM,    4,   10,   20,     0, 0x14,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,        NO_COMBO | NO_KO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 20: pl14_at_throw_p, Punch throw's hit while she holds the opponent (p_catch_lift, cg_type 1) */
	{  16,                     2,     A_NOM | 0x40,    K_LM,    4,   10,   20,     0, 0x04,   8,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl14_at_throw_k, Kick throw */
	{   8,                    17,     A_NOM | 0x40,    K_LM,    4,   10,   20,     0, 0x14,  11,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl14_at_throw_p_2, Punch throw's release that sends them off (ground and air throw) */
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
	{   4,            ZU | NO_ZU,        A_SPECIAL,     K_L,    0,    4,   16,     0, 0x1f,   1,  0x80,       3, SC300, SD_NOR_L,   0,  50, 153,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 30: pl14_at_shienbu, Shienbu: SonSon and the three monkey shots (shl14_00) */
	{ 0 },	/* 31 */
	{ 0 },	/* 32 */
	{   5,           ZU | NO_VEC,        A_SPECIAL,     K_M,    0,    6,   30,     0, 0x1e,   1,  0x80,       3, SC300, SD_NOR_M,   0,  90, 151,  152,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 33: pl14_at_fuusetsu, Fuusetsu Zan, each mashed hit, ground or air */
	{ 0 },	/* 34 */
	{ 0 },	/* 35 */
	{  14,          BU_O | NO_ZU,        A_SPECIAL,     K_L,    0,    8,   30,     0, 0x1f,   4,  0x80,       3, SC300,        8,   0,  90, 153,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 36: pl14_at_seiten, Seiten Rengeki's first hit */
	{   4,          BU_O | NO_ZU,        A_SPECIAL,     K_L,    0,   10,   30,     0, 0x1f,   4,  0x80,       3, SC300,        8,   0,  90, 154,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 37: pl14_at_seiten_2, Seiten Rengeki's later hits (up to atck_level + 1) */
	{ 0 },	/* 38 */
	{   2,    2 | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    6,   20,    10, 0x00,  10,  0x60,       3, SC200,       18,   0,  90, 157,    0,     NO_CHIP | NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 39: pl14_at_tenchi, Tenchi Tsuukan: the staff thrown up (SonSon and shl14_01's head) */
	{   4,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,     K_L,    0,   10,   20,    15, 0x00,  10,  0x80,       3, SC300,        4,   0,  90, 158,  169,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3a: pl14_at_tenchi_2, Tenchi Tsuukan: the giant staff's hits as it comes down (shl14_01) */
	{  28,             2 | NO_ZU,   A_HYPER | 0x80,     K_L,    0,    8,   20,    15, 0x18,  10,  0x80,       3, SC300,        4,   0,  90, 158,  169,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3b: pl14_at_tenchi_3, Tenchi Tsuukan: the giant staff's last hit (timer1 4) */
	{   0,                  BU_O,        A_SPECIAL,    K_LM,    0,    0,   20,     0, 0x00,  10,  0x60,       3, SC200, SD_NOR_M,   0,  90,   0,    0,        NO_COMBO | NO_KO,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3c: pl14_at_hisago, Kingin no Hisago: the gourd that catches */
	{  22,                     2, A_SPECIAL | 0x40,    K_LM,    0,   10,   20,     0, 0x00,  10,  0x60,       3, SC200, SD_NOR_M,   0,  90,   0,    0,                       0,  1,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 3d: pl14_at_hisago_2, Kingin no Hisago: the caught opponent sent out (sp_catch_hit, on the opponent) */
	{ 0 },	/* 3e */
	{   4,    2 | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    6,   20,    10, 0x1a,   6,  0x60,       3, SC200,       18,   0,  90, 159,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 3f: pl14_at_pow, POW: SonSon and the flying word (shl14_02) */
	{   5,    2 | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    6,   20,    10, 0x1a,   6,  0x60,       3, SC200,       18,   0,  90, 159,    0,                   NO_KO,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 40: pl14_at_pow_hold, POW: hold on the caught opponent (shl14_02_hold) */
	{  26,                     2,   A_HYPER | 0x40,    K_LM,    0,   10,   20,     0, 0x1a,   6,  0x60,       3, SC200,       18,   0,  90,   0,    0,                       0,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 41: pl14_at_pow_throw, POW: the throw out at the end (shl14_02_throw_rise) */
	{  10,    2 | NO_VEC | NO_ZU,          A_HYPER,     K_L,    0,    6,   20,    10, 0x00,  11,  0x60,       3, SC200,       18,   0,  90, 161,    0,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42: pl14_at_enou, En'Ou: growing into the giant monkey */
	{  20,                    ZU,   A_HYPER | 0x80,     K_L,    0,   15,   20,    10, 0x1c,  12,  0x60,       3, SC200,       18,   0,  90, 162,  170,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 43: pl14_at_enou_lp, En'Ou: the giant monkey's LP downward punch (also sp_enou_auto) */
	{  20, BU_O | NO_VEC | NO_ZU,   A_HYPER | 0x80,     K_L,    0,   15,   20,    10, 0x1f,  11,  0x60,       3, SC200,       18,   0,  90, 163,  171,                 NO_CHIP,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 44: pl14_at_enou_hp, En'Ou: the giant monkey's HP uppercut (also sp_enou_auto) */
	{   7,    2 | NO_VEC | NO_ZU,          A_HYPER, K_L | 2,    0,    4,   20,     0, 0x1e,  10,  0x80,       3, SC300, SD_NOR_L,   2,  90, 168,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,  10,        3, { 0, 0 } },	/* 45: pl14_at_enou_rush, En'Ou: the rush and the attacks riding on it (shl14_04/05/06) */
	{  20,   ZU | NO_VEC | NO_ZU,        A_SPECIAL,     K_L,    0,    6,   30,     0, 0x16,   8,  0x80,       3, SC300,       12,   0,  90,   0,    0,               NO_WEIGHT,  1,     0,  0,     0,    30,   0, YK_GD030, { 0, 0 } },	/* 46: pl14_at_peach, the assist's healing peach (shl14_03, set for the other side) */
	{  10,           10 | NO_VEC,        A_SPECIAL,     K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90, 156,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 47: pl14_at_t_kick, Tag-in kick that leaps off the wall */
	{  10,           12 | NO_VEC,        A_SPECIAL,     K_L,    0,    4,   24,     0, 0x1c,  12,  0x80,       3, SC300,        8,  64,  90, 172,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        2, { 0, 0 } },	/* 48: pl14_at_snapback, Snapback */
	{  10,         BU_O | NO_VEC,             A_HI,     K_L,    0,   10,   24,     0, 0x1a,  12,  0x60, MARK_N3, SC200, SD_KIK_L,   0,  90, 147,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        5, { 0, 0 } },	/* 49: pl14_at_wall, Wall Walk (wall_init) */
};
