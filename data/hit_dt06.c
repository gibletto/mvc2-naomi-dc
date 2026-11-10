/* Cyclops (character 06): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl06_at_ for its own (pl06_def.h).
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
     box, box2  boxes of hit_06 that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_06[] = {
	/* dmg                   type              kind  elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                     flag  13  guard  15  catch  abyss  cnt    answer        1a */
	{   4,                    ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   6,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  14,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 00: ATCK_STAND_LP */
	{   7,         BU_O | NO_VEC,            A_NOM,  K_M,    2,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  16,   94, NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 01: ATCK_STAND_MP */
	{  13,            ZU | NO_ZU,            A_NOM,  K_S,    1,    4,   24,     0, 0x1a,   2,  0x80, MARK_N3, SC300,       17,   0,  50,  64,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 02: ATCK_STAND_HP */
	{   4,                    ZU,            A_NOM,  K_S,    4,   10,   10,     0, 0x18,   2,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  21,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 03: ATCK_STAND_LK */
	{   7,                    ZU,            A_NOM,  K_M,    2,   10,   16,     0, 0x1a,   6,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  23,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 04: ATCK_STAND_MK */
	{  13,           ZU | NO_VEC,            A_NOM,  K_L,    1,   10,   24,     0, 0x1c,   6,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  27,  108,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 05: ATCK_STAND_HK */
	{   4,           ZU | NO_VEC,            A_NOM, K_LS,    4,   10,   10,     0, 0x18,   3,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  33,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 06: ATCK_CROUCH_LP */
	{   7,                    ZU,            A_NOM, K_LM,    2,   10,   16,     0, 0x1e,   2,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  35,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 07: ATCK_CROUCH_MP */
	{  13,            ZU | NO_ZU,            A_NOM,  K_S,    1,    4,   24,     0, 0x1a,   2,  0x80, MARK_N3, SC300,       17,   0,  50,  64,    0,                       0,  0,     0,  0,     0,     0,   0,        4, { 0, 0 } },	/* 08: ATCK_CROUCH_HP */
	{   4,           ZU | NO_VEC,             A_LO, K_LS,    4,   10,   10,     0, 0x1a,   3,  0x70, MARK_N1, SC100, SD_KIK_S,   0,  20,  39,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 09: ATCK_CROUCH_LK */
	{   7,         BU_O | NO_VEC,            A_NOM, K_LM,    2,   10,   16,     0, 0x1f,   6,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  41,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0a: ATCK_CROUCH_MK */
	{  13,                    AS,             A_LO, K_LL,    1,   10,   24,     9, 0x1a,   8,  0x70, MARK_N3, SC300, SD_KIK_L,   0, 100,  42,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0b: ATCK_CROUCH_HK */
	{   4,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   3,  0x70, MARK_N1, SC100, SD_NOR_S,   0,  20,  44,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0c: ATCK_JUMP_LP */
	{   7,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   5,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  93,   46,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0d: ATCK_JUMP_MP */
	{  13,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   24,     0, 0x16,   9,  0x80, MARK_N3, SC300, SD_NOR_L,   0,  60,  48,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0e: ATCK_JUMP_HP */
	{   4,           ZU | NO_VEC,             A_HI,  K_S,    1,   10,   10,     0, 0x00,   3,  0x60, MARK_N1, SC100, SD_KIK_S,   0,  20,  54,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 0f: ATCK_JUMP_LK */
	{   7,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x1f,   5,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  56,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 10: ATCK_JUMP_MK */
	{  13,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   24,     0, 0x16,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  60,  58,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 11: ATCK_JUMP_HK */
	{   7,           ZU | NO_VEC,             A_HI,  K_L,    1,   10,   16,     0, 0x16,   4,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  40,  50,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 12: pl06_at_jdown_mp, Jumping M punch (second press) with the stick down */
	{  13,           ZU | NO_VEC,             A_HI,  K_L,    1,   10,   16,     0, 0x12,   6,  0x60, MARK_N2, SC200, SD_NOR_M,   0,  60,  52,    0,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 13: pl06_at_jdown_hp, Jumping HP with the stick down */
	{   4,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x00,   2,  0x60, MARK_N2, SC200, SD_KIK_S,   0,  20, 106,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 14: pl06_at_jdown_lk, Jumping LK with the stick down */
	{   7,           ZU | NO_VEC,             A_HI,  K_M,    1,   10,   16,     0, 0x16,   4,  0x60, MARK_N2, SC200, SD_KIK_M,   0,  40,  60,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 15: pl06_at_jdown_mk, Jumping M kick (second press) with the stick down */
	{  13,           ZU | NO_VEC,             A_HI,  K_L,    1,   10,   16,     0, 0x16,   6,  0x60, MARK_N2, SC200, SD_KIK_L,   0,  60,  62,   97,           AERIAL_FINISH,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 16: pl06_at_jdown_hk, Jumping HK with the stick down */
	{ 0 },	/* 17 */
	{ 0 },	/* 18 */
	{ 0 },	/* 19 */
	{ 0 },	/* 1a */
	{   7,           ZU | NO_VEC,            A_NOM,  K_L,    1,   10,   24,     0, 0x1c,   8,  0x80, MARK_N3, SC300, SD_KIK_L,   0,  50,  29,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 1b: pl06_at_stand_hk_2, Standing HK's second kick, with the button still held */
	{ 0 },	/* 1c */
	{ 0 },	/* 1d */
	{ 0 },	/* 1e */
	{ 0 },	/* 1f */
	{  20,                     2,        A_SPECIAL,    0,    4,    4,   10,    15, 0x1c,   4,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0,        6, { 0, 0 } },	/* 20: pl06_at_neckbreaker, the Running Neckbreaker's throw, set on the opponent (neckbreaker_catch) */
	{  16,                     2,     A_NOM | 0x40,    0,    4,   16,   10,     0, 0x07,  11,  0x80, MARK_N1, SC300,       76,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 21: pl06_at_crouch_throw_k, Kick throw from a crouch (crouch_k_catch), set on the opponent */
	{  16,         BU_O | NO_VEC,     A_NOM | 0x40,    0,    4,    8,   10,     0, 0x04,   4,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 22: pl06_at_throw_p, the punch throw (nml_p_catch), set on the opponent */
	{  16,         BU_O | NO_VEC,     A_NOM | 0x40,    0,    4,    8,   10,     0, 0x04,   2,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 23: pl06_at_beam_throw, the command throw (half circle back + P, beam_throw_catch) that aims the opponent with the stick; set with ATCK_DIR (0xa3) */
	{  16,                     2,     A_NOM | 0x40,    0,    4,    8,   10,     0, 0x10,   4,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 24: pl06_at_air_throw, the air throw (jump_p_catch), set on the opponent */
	{  16,                     2,     A_NOM | 0x40,    0,    4,    8,   10,     0, 0x1a,   5,  0x80, MARK_N1, SC300, SD_NOR_S,   0,   0,   0,    0,                       0,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 25: pl06_at_throw_k, the standing kick throw (nml_k_catch), a point-blank beam, set on the opponent */
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
	{   6,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   20,     0, 0x00,   9,  0x80,       3, SC300, SD_NOR_L,   0,  90,  68,   69,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 30 */
	{  10,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   0,  10,  88,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 31 */
	{  10,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   0,  10,  88,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 32 */
	{  10,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   0,  10,  88,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 33 */
	{  10,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   0,  10,  88,    0,           AERIAL_FINISH,  1,     0,  0,     0,     0,   2, YK_GD030, { 0, 0 } },	/* 34: pl06_at_jblast, Optic Blast in the air */
	{  14,   AS | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    4,   20,     9, 0x1a,   2,  0x80,       4, SC300,       18,   0,  90,  88,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 35: pl06_at_sweep, Optic Sweep */
	{   6,   ZU | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_L,    0,    4,   20,     0, 0x00,   9,  0x80,       4, SC300,       18,   0,  90,  79,    0,                       0,  1,     0, 12,     0,     0,   4,        3, { 0, 0 } },	/* 36: pl06_at_super, Super Optic Blast (hyper, sp_super, the beam that can be aimed) */
	{   6,   ZU | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_L,    0,    4,   20,     0, 0x00,   9,  0x80,       4, SC300,       18,   0,  90,  79,    0,                 NO_CHIP,  1,     0, 12,     0,     0,   4,        3, { 0, 0 } },	/* 37 */
	{   5,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_L,    0,   10,   20,     0, 0x1a,   6,  0x80,       4, SC300,       18,   0,  90,  85,   90,                       0,  1,     0, 11,     0,     0,   6,        3, { 0, 0 } },	/* 38: pl06_at_mega, Mega Optic Blast (hyper, sp_mega) */
	{   6,    2 | NO_VEC | NO_ZU,   A_HYPER | 0x80,  K_L,    0,   10,   20,     0, 0x1a,   6,  0x80,       4, SC300,       18,   0,  90,  85,   90,                 NO_CHIP,  1,     0, 11,     0,     0,   6,        3, { 0, 0 } },	/* 39 */
	{  10,           10 | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x1f,  10,  0x60,       3, SC200,       12,   0,  90,  98,    0, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3a: pl06_at_t_kick, the jump-in kick when he is tagged in (t_kick) */
	{   6,                    ZU,        A_SPECIAL,  K_M,    0,    6,   10,     0, 0x18,   2,  0x70,       3, SC100, SD_NOR_S,   0,  90, 110,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3b: pl06_at_rapid, Rapid Punches: the first punch, and the repeated hits in each stage */
	{   7,                    ZU,        A_SPECIAL, K_LM,    0,    6,   10,     0, 0x18,   2,  0x70,       3, SC100, SD_KIK_S,   0,  90, 111,    0,                       0,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3c: pl06_at_rapid_2, Rapid Punches, second stage */
	{   8,         BU_O | NO_VEC,        A_SPECIAL,  K_L,    0,   10,   24,     0, 0x1f,   5,  0x70,       3, SC100, SD_NOR_S,   0,  90,  35,    0,               NO_WEIGHT,  1,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 3d: pl06_at_rapid_3, Rapid Punches, third stage */
	{  12,            ZU | NO_ZU,        A_SPECIAL,  K_S,    0,    4,   24,     0, 0x1a,  10,  0x80,       3, SC300,       17,   0,  90,  64,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0, YK_GD060, { 0, 0 } },	/* 3e: pl06_at_rapid_4, Rapid Punches, last punch (it fires the blast) */
	{   6,                    ZU,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x18,   2,  0x70,       3, SC100, SD_KIK_M,   0,  90,  60,    0,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 3f */
	{  14,                    ZU,        A_SPECIAL, K_LL,    0,   10,   10,     0, 0x18,   2,  0x70,       3, SC100, SD_KIK_L,   0,  90, 102,    0,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 40 */
	{  16,   ZU | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_L,    0,    4,   20,     0, 0x1a,   2,  0x80,       4, SC300,       18,   0,  10,  88,    0,                       0,  1,     0,  0,     0,     0,   2,        8, { 0, 0 } },	/* 41 */
	{   4, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_S,    0,    4,   24,     0, 0x1d,   8,  0x80,       3, SC300,       17,   0,  90,  64,    0,                 NO_CHIP,  0,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 42 */
	{  10,   12 | NO_VEC | NO_ZU,        A_SPECIAL,  K_L,    0,    4,   24,     0, 0x1c,  12,  0x80, MARK_N3, SC300,       12,  64,  90,  64,    0,               NO_WEIGHT,  0,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 43: pl06_at_snapback, Snapback */
	{   7,         BU_O | NO_VEC,        A_SPECIAL,  K_M,    0,   10,   16,     0, 0x00,  11,  0x80, MARK_N2, SC200, SD_NOR_M,   0,  40,  16,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 44: pl06_at_upper_p, the launcher, punch, standing */
	{   9,         BU_O | NO_VEC,        A_SPECIAL, K_LM,    0,   10,   16,     0, 0x1f,  11,  0x40, MARK_N2, SC200, SD_KIK_M,   0,  40,  41,    0, NO_CHIP | NO_WEIGHT | BLACK_COMBO,  0,     0,  0,     0,     0,   0, YK_GD030, { 0, 0 } },	/* 45: pl06_at_upper_k, the launcher, kick, crouching */
	{  14,                    ZU,        A_SPECIAL,  K_L,    0,   10,   10,     0, 0x18,   2,  0x70,       3, SC100, SD_KIK_M,   0,  90,  60,    0,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 46 */
	{  10,                    ZU,        A_SPECIAL, K_LL,    0,   10,   10,     0, 0x18,   2,  0x70,       3, SC100, SD_KIK_L,   0,  90, 102,    0,                       0,  1,     0,  0,     0,     0,   0,        7, { 0, 0 } },	/* 47 */
	{   6,         BU_O | NO_VEC, A_SPECIAL | 0x80,  K_L,    0,   10,   20,     0, 0x00,   9,  0x80,       3, SC300, SD_NOR_L,   0,  90,  68,   69,               NO_WEIGHT,  1,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 48 */
	{   8, BU_O | NO_VEC | NO_ZU, A_SPECIAL | 0x80,  K_S,    0,    4,   24,     0, 0x1d,   8,  0x80,       3, SC300,       17,   0,  90,  64,    0,                 NO_CHIP,  0,     0,  0,     0,     0,   0,        3, { 0, 0 } },	/* 49 */
};
