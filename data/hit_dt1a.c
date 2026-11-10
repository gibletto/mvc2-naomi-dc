/* Abyss (third form) (character 1a): the attacks (HIT_DATA, 0x1c bytes).
   What each hit does. The character's module sets the attack's number (atck_no) with the move,
   and the row is read when the attack box meets a body. After a row is its number, in
   hexadecimal, and the program's name for it:
   ATCK_ for the normals every character has (player.h), pl1a_at_ for its own (pl1a_def.h).
   The values' names are Capcom's own for these tables (hit_equ.h) and the program's (player.h).

     dmg     the damage
     type    the reaction: ZU pushed back, BU_O knocked off the feet, AS swept,
             or the number of another in pl_dmg.c's dm_tbl (here 2 dm_buttobi_ng, 6 dm_hold, 7 dm_07,
             9 dm_freeze, 12 dm_snap, 13 dm_zuri, 14 dm_nop, 15 dm_spark, 16 dm_dizzy, 17 dm_slam,
             20 dm_curse, 21 dm_buttobi_ukemi_ng).
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
     box, box2  boxes of hit_1a that the computer measures the attack's reach by
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

const HIT_DATA hit_dt_1a[] = {
	/* dmg                      type                      kind                   elem  piyo  stop  sand  sleep   vec  pow  range     mark  score        se  shc  jug  box  box2                      flag   13  guard   15  catch  abyss  cnt    answer            1a */
	{   8,               12 | NO_ZU,                        3, K_LS | K_BACK_MAX | 2,   95,  120,  118,   148, 0x8f,  69,  0xbf,      73,   227,      150,   0, 192, 136,  157, NO_WEIGHT | NO_SCALE | NO_COMBO | BLACK_COMBO | AERIAL_FINISH | 1, 192,   109,  54,    96,   223,  72,       93, { 173, 247 } },	/* 00: ATCK_STAND_LP */
	{  70,      17 | NO_VEC | NO_ZU, A_HI | A_SPECIAL | A_HYPER,               K_M | 4,   56,   41,  149,   205, 0xad,  40,  0xd2,    0xa2,   220,      137, 243,  87, 217,   33, NO_CHIP | NO_WEIGHT | NO_SCALE | BLACK_COMBO | AERIAL_FINISH | 1, 222,   223, 142,    31,   211,  48,       62,  { 134, 25 } },	/* 01: ATCK_STAND_MP */
	{ 100,                       30,    12 | A_SPECIAL | 0x80,  K_M | K_BACK_MAX | 7,  149,  180,  216,    54, 0x55, 223,  0x4b,      77,   205,       36, 251,  49, 232,  184, NO_CHIP | NO_KO | BLACK_COMBO | AERIAL_FINISH | 1, 191,    13, 255,   109,    67,  50,        2,  { 100, 67 } },	/* 02: ATCK_STAND_HP */
	{ 180,  9 | NO_VEC | FLY_SCREEN,         A_HI | A_SPECIAL,              K_LM | 7,  244,   83,  204,   218, 0x02, 235,  0x60,    0xf7,    99,      221, 208, 104,  27,  151, NO_SCALE | NO_COMBO | NO_KO | AERIAL_FINISH,  42,    59, 124,   201,    20,   4,      239,  { 215, 68 } },	/* 03: ATCK_STAND_LK */
	{ 205,  2 | NO_VEC | FLY_SCREEN, 3 | A_SPECIAL | A_HYPER | 0x80,               K_L | 4,  218,   84,  115,   130, 0x3f,  93,  0x58,    0xdd,    20,       44, 236,  71, 239,  193, NO_CHIP | NO_WEIGHT | NO_SCALE | AERIAL_FINISH | 1,  16,     0, 141,    24,   160, 229,      237, { 165, 152 } },	/* 04: ATCK_STAND_MK */
	{  67,               26 | NO_ZU,                11 | 0x40,                  0x5a,  125,  123,  225,   108, 0x8a, 132,  0x5b,    0xcf,    26,        3,  38, 203,  84,   23, NO_CHIP | NO_WEIGHT | NO_SCALE | NO_COMBO | BLACK_COMBO | AERIAL_FINISH,  58,     9, 140,   193,   228, 234, YK_GD030, { 118, 201 } },	/* 05: ATCK_STAND_HK */
	{ 122,                6 | NO_ZU,             A_NOM | 0x40,  K_M | K_BACK_MAX | 6,   89,  150,   86,    40, 0x9c, 132,  0x86,      90,   183,       98, 200, 219, 226,  122, NO_WEIGHT | NO_KO | BLACK_COMBO | AERIAL_FINISH | 1, 220,    86,  49,   229,     6,  83,      171,   { 99, 33 } },	/* 06: ATCK_CROUCH_LP */
	{  72,      15 | NO_VEC | NO_ZU, A_SPECIAL | A_HYPER | 0x40,                     4,   24,   45,   44,   100, 0xdf,  90,  0xc9,     122,     9,       28,   7,  20,  88,   68, NO_CHIP | NO_SCALE | NO_COMBO | NO_KO | BLACK_COMBO | AERIAL_FINISH | 1,  25,   115,  87,     4,   244, 182,      234,   { 248, 8 } },	/* 07: ATCK_CROUCH_MP */
	{ 241, 16 | NO_VEC | NO_ZU | FLY_SCREEN, 14 | A_SPECIAL | A_HYPER, K_LL | K_BACK_MAX | 2,  158,  196,  239,   223, 0x69,  48,  0xc2,     111,   112,       73, 188,  63,  47,   76, NO_COMBO | AERIAL_FINISH,  93,   113, 103,   127,    40,  59,      157, { 199, 154 } },	/* 08: ATCK_CROUCH_HP */
	{  20,               20 | NO_ZU,     3 | A_SPECIAL | 0x40,                  0xca,  138,  149,  225,    60, 0x6d, 140,  0x1f,       3,   151,       67,  32,  46, 184,  175, NO_COMBO | AERIAL_FINISH | 1, 149,   253, 129,    16,   155,   9,      238,  { 42, 205 } },	/* 09: ATCK_CROUCH_LK */
	{  45,  21 | NO_ZU | FLY_SCREEN,             12 | A_HYPER,                     1,   31,  111,  126,   246, 0x57,  63,  0xa0,    0xe8,    68,      195, 125,  80,  90,  132,     NO_COMBO | NO_KO | 1,  37,    99, 187,   245,   189, 234,      147,  { 70, 221 } },	/* 0a: ATCK_CROUCH_MK */
	{  52, AS | NO_VEC | NO_ZU | FLY_SCREEN,   A_GUARD_NG | A_SPECIAL,                  0xf3,  195,  111,    4,     3, 0x8a, 162,  0x45,      99,    19,      250, 156, 158, 185,  231, NO_CHIP | NO_WEIGHT | BLACK_COMBO | AERIAL_FINISH, 196,   242, 119,   167,   131,  42,      135,  { 214, 33 } },	/* 0b: ATCK_CROUCH_HK */
	{ 247,       2 | NO_VEC | NO_ZU, 3 | A_SPECIAL | A_HYPER | 0x80,                  0xda,   10,   61,  174,   173, 0xfe,   5,  0xe5,      34,    13,       58, 191,  18,   3,   52, NO_CHIP | NO_SCALE | NO_COMBO | NO_KO | AERIAL_FINISH | 1, 235,   103,  94,     1,   133,  33,        9,  { 249, 28 } },	/* 0c: ATCK_JUMP_LP */
	{ 206,                       23,      15 | A_HYPER | 0x40,                  0x55,  171,  178,  161,   244, 0xac, 217,  0x95,      56,   103,       24,  38, 134, 112,   45, NO_SCALE | BLACK_COMBO | 1,  41,   244, 207,   173,     0, 247,      124, { 128, 231 } },	/* 0d: ATCK_JUMP_MP */
	{  58,               16 | NO_ZU,       5 | A_HYPER | 0x40,                  0x6f,   17,   86,   67,    18, 0x19, 117,  0x22,      46,    67,      119, 194, 253,  19,  179, NO_CHIP | NO_COMBO | NO_KO | BLACK_COMBO | AERIAL_FINISH | 1,   0,   124,  90,   137,   248,  54,        7, { 231, 190 } },	/* 0e: ATCK_JUMP_HP */
	{  17, 23 | NO_VEC | FLY_SCREEN,    11 | A_SPECIAL | 0xc0,                  0x5f,   17,  201,  251,   161, 0xc9,  19,  0x1a,      61,   218,      129,  20,  61,   0,  199, NO_COMBO | NO_KO | BLACK_COMBO, 131,   157,  66,    51,    12,   2,      135, { 109, 242 } },	/* 0f: ATCK_JUMP_LK */
	{ 170,               14 | NO_ZU,       9 | A_HYPER | 0x80,              K_LM | 4,  112,  111,  239,   145, 0x04, 216,  0x84,     127,   201, SD_KIK_M,   8, 223,   9,  181,      NO_SCALE | NO_COMBO, 118,    30,  89,    35,   130,  36,      164, { 158, 204 } },	/* 10: ATCK_JUMP_MK */
	{ 227, BU_O | NO_VEC | FLY_SCREEN,             10 | A_HYPER,              K_LL | 6,  178,  114,  250,   224, 0xce, 108,  0xe6,      67,   253,       74, 159, 127, 241,  222, NO_CHIP | NO_WEIGHT | NO_KO | BLACK_COMBO,  40,    84,  88,   153,   217, 223,      173, { 170, 214 } },	/* 11: ATCK_JUMP_HK */
	{  85,                       20, A_SPECIAL | A_HYPER | 0x80,                     4,  254,  127,   64,    65, 0x08, 140,  0x03,    0x9a,    95,      120, 118, 148, 143,   69, NO_CHIP | NO_WEIGHT | NO_SCALE | NO_COMBO | NO_KO | AERIAL_FINISH | 1,  73,   227, 150,     0,   192, 136,      157, { 221, 192 } },	/* 12 */
	{ 109,          22 | FLY_SCREEN,           A_HYPER | 0x40,                  0xdf,   72,   93,  173,   247, 0x46, 209,  0x32,      36,    56,       41, 149, 205, 173,   40, NO_CHIP | NO_COMBO | BLACK_COMBO | AERIAL_FINISH, 162,   220, 137,   243,    87, 217,       33, { 207, 222 } },	/* 13 */
	{ 223,               14 | NO_ZU,           15 | A_SPECIAL,                  0xd3,   48,   62,  134,    25, 0x64,  30,  0x9c,      47,   149,      180, 216,  54,  85,  223, NO_CHIP | NO_SCALE | BLACK_COMBO | 1,  77,   205,  36,   251,    49, 232,      184, { 227, 191 } },	/* 14 */
	{  13, 31 | NO_VEC | NO_ZU | FLY_SCREEN,      13 | A_HYPER | 0x40,                  0x43,   50,    2,  100,    67, 0xb4, 105,  0x12,    0xa7,   244,       83, 204, 218,   2,  235,      NO_KO | BLACK_COMBO, 247,    99, 221,   208,   104,  27,      151,  { 184, 42 } },	/* 15 */
	{  59, 28 | NO_VEC | FLY_SCREEN,                 9 | 0xc0,               K_S | 4,    4,  239,  215,    68, 0xcd,  98,  0xb3,      52,   218,       84, 115, 130,  63,   93, NO_SCALE | NO_COMBO | BLACK_COMBO, 221,    20,  44,   236,    71, 239,      193,  { 143, 16 } },	/* 16 */
	{   0,               13 | NO_ZU,   A_GUARD_NG | A_SPECIAL,                  K_LM,  229,  237,  165,   152, 0x43, 154,  0x4b,      90,   125,      123, 225, 108, 138,  132, NO_CHIP | NO_SCALE | NO_COMBO | BLACK_COMBO | 1, 207,    26,   3,    38,   203,  84,       23,  { 222, 58 } },	/* 17 */
	{   9,               12 | NO_ZU,                 1 | 0xc0,                  0xe4,  234,    0,  118,   201, 0x7a, 134,  0x40,      46,    89,      150,  86,  40, 156,  132, NO_CHIP | NO_WEIGHT | AERIAL_FINISH,  90,   183,  98,   200,   219, 226,      122, { 229, 220 } },	/* 18 */
	{  86,          17 | FLY_SCREEN,       5 | A_HYPER | 0xc0,                     6,   83,  171,   99,    33, 0x48, 207,  0x70,       4,    24,       45,  44, 100, 223,   90, NO_SCALE | BLACK_COMBO | AERIAL_FINISH | 1, 122,     9,  28,     7,    20,  88,       68,  { 251, 25 } },	/* 19 */
	{ 115,              23 | NO_VEC,                     A_LO,                  0xf4,  182,  234,  248,     8, 0xf1, 240,  0x3e,    0xba,   158,      196, 239, 223, 105,   48, NO_CHIP | BLACK_COMBO | AERIAL_FINISH, 111,   112,  73,   188,    63,  47,       76,  { 144, 93 } },	/* 1a */
	{ 113,  7 | NO_VEC | FLY_SCREEN, 15 | A_SPECIAL | A_HYPER | 0x40,      K_M | K_BACK_MAX,   59,  157,  199,   154, 0x14, 148,  0x53,    0xca,   138,      149, 225,  60, 109,  140, NO_CHIP | NO_WEIGHT | NO_SCALE | NO_COMBO | 1,   3,   151,  67,    32,    46, 184,      175, { 145, 149 } },	/* 1b */
	{ 253,             BU_O | NO_ZU,                A_SPECIAL, K_LS | K_BACK_MAX | 3,    9,  238,   42,   205, 0x2d, 181,  0x2c, MARK_N2,    31,      111, 126, 246,  87,   63,    NO_KO | AERIAL_FINISH, 232,    68, 195,   125,    80,  90,      132,   { 49, 37 } },	/* 1c */
	{  99,  27 | NO_ZU | FLY_SCREEN, 5 | A_SPECIAL | A_HYPER | 0xc0, K_LL | K_BACK_MAX | 5,  234,  147,   70,   221, 0x34, 228,  0x18,    0xf3,   195,      111,   4,   3, 138,  162, NO_WEIGHT | BLACK_COMBO | 1,  99,    19, 250,   156,   158, 185,      231, { 198, 196 } },	/* 1d */
	{ 242, 23 | NO_VEC | FLY_SCREEN,       7 | A_HYPER | 0x80,                  0x83,   42,  135,  214,    33, 0xf7, 194,  0xb3,    0xda,    10,       61, 174, 173, 254,    5, NO_WEIGHT | NO_KO | BLACK_COMBO | AERIAL_FINISH | 1,  34,    13,  58,   191,    18,   3,       52, { 187, 235 } },	/* 1e */
	{ 103,              30 | NO_VEC,                        1,                  0x85,   33,    9,  249,    28, 0xce,  23,  0x6f,      85,   171,      178, 161, 244, 172,  217, NO_WEIGHT | NO_COMBO | AERIAL_FINISH | 1,  56,   103,  24,    38,   134, 112,       45,   { 73, 41 } },	/* 1f */
	{ 244,      15 | NO_VEC | NO_ZU,      13 | A_HYPER | 0x80,                     0,  247,  124,  128,   231, 0x3a, 144,  0x65,     111,    17,       86,  67,  18,  25,  117,          NO_CHIP | NO_KO,  46,    67, 119,   194,   253,  19,      179,   { 243, 0 } },	/* 20 */
	{ 124,              26 | NO_VEC,                 9 | 0x80,                  0xf8,   54,    7,  231,   190, 0x11, 119,  0xdb,      95,    17,      201, 251, 161, 201,   19, NO_CHIP | NO_SCALE | NO_COMBO,  61,   218, 129,    20,    61,   0,      199, { 112, 131 } },	/* 21 */
	{ 157,               2 | NO_VEC,  3 | A_SPECIAL | A_HYPER,        K_BACK_MAX | 4,    2,  135,  109,   242, 0xaa, 142,  0xa9,    0xa4,   112,      111, 239, 145,   4,  216, NO_WEIGHT | AERIAL_FINISH, 127,   201,  10,     8,   223,   9,      181,  { 24, 118 } },	/* 22 */
	{  30,              25 | NO_VEC,              3 | A_HYPER,                  0x82,   36,  164,  158,   204, 0xe3,  97,  0x2a,    0xb6,   178,      114, 250, 224, 206,  108, NO_CHIP | NO_WEIGHT | NO_KO | BLACK_COMBO | AERIAL_FINISH,  67,   253,  74,   159,   127, 241,      222,  { 102, 40 } },	/* 23 */
	{  84,              24 | NO_VEC,     9 | A_SPECIAL | 0x80,                  0xd9,  223,  173,  170,   214, 0x55,  20,  0xb0,       4,   254,      127,  64,  65,   0,    0,                        0,   0,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 24 */
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
	{  14,    BU_O | NO_VEC | NO_ZU,                A_SPECIAL,                   K_L,    0,    6,   20,     0, 0x1c,   8,  0x60,       3, SC200,        8,   0, 100,  21,    0,                        0,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 30: pl1a_at_wave, the shot along the floor: Abyss himself */
	{  10,       2 | NO_VEC | NO_ZU,                A_SPECIAL,              K_LM | 2,    0,    6,   20,     0, 0x1f,  13,  0x60,       3, SC200,        8,   0, 100,  15,    0,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 31: pl1a_at_wave_shot, the shot along the floor and its seven pieces (shl1a_00_wave and shl1a_00_wave_part) */
	{   8,    BU_O | NO_VEC | NO_ZU,                A_SPECIAL,                  K_LL,    0,    6,   20,     0, 0x1c,  10,  0x60,       3, SC200,        8,   0, 100,  23,   25,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 32: pl1a_at_stomp, the leap that comes down on the opponent */
	{  12, 2 | NO_VEC | NO_ZU | FLY_SCREEN,                A_SPECIAL,                  K_LL,    0,   10,   30,     0, 0x1a,  13,  0x80,       3, SC300,        8,   0, 100,  28,    0,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 33 */
	{  12,       2 | NO_VEC | NO_ZU,                A_SPECIAL,               K_L | 2,    0,    8,   30,     0, 0x1f,  11,  0x80,       3, SC300,        8,   0, 100,  33,    0,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 34: pl1a_at_jump, Jumps at the opponent, firing */
	{  12,       2 | NO_VEC | NO_ZU,                A_SPECIAL,               K_L | 2,    0,    8,   30,     0, 0x1f,  11,  0x80,       3, SC300,        8,   0, 100,  33,    0,      NO_CHIP | NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 35 */
	{  10,       2 | NO_VEC | NO_ZU,                A_SPECIAL,              K_LL | 2,    0,    8,   30,     0, 0x1d,   9,  0x80,       3, SC300,        8,   0, 100,  35,    0,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 36: pl1a_at_spray_shot, Each shot sprayed out by the storm and the spray (shl1a_00_spray and shl1a_00_shot) */
	{  10,       2 | NO_VEC | NO_ZU,                A_SPECIAL,              K_LL | 2,    0,    8,   30,     0, 0x1d,   9,  0x80,       3, SC300,        8,   0, 100,  35,    0,      NO_CHIP | NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 37 */
	{  10,       2 | NO_VEC | NO_ZU,                A_SPECIAL,               K_L | 2,    0,    2,   30,     0, 0x1c,  10,  0x80,       3, SC300,        8,   0, 100,  47,   49,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 38: pl1a_at_spray, the random spray: Abyss himself; also the storm's opening effect (shl1a_00_storm) */
	{  10,       2 | NO_VEC | NO_ZU,                A_SPECIAL,               K_L | 2,    0,    2,   30,     0, 0x1c,  10,  0x80,       3, SC300,        8,   0, 100,  47,   49,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 39 */
	{  10, 2 | NO_VEC | NO_ZU | FLY_SCREEN,                A_SPECIAL,               K_L | 2,    0,    8,   30,     0, 0x1c,  10,  0x80,       3, SC300,        8,   0, 100,  47,   49,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 3a */
	{   8,             BU_O | NO_ZU,                A_SPECIAL,               K_L | 2,    0,   10,   40,     0, 0x1f,   4,  0x80,       3, SC300,       12,   0,  90,  52,    0,                        0,   1,     0,   0,     0,    30,   0, YK_GD030,     { 0, 0 } },	/* 3b: pl1a_at_blast, the lasting blast in front of him, Abyss and the blast (shl1a_00_blast) */
	{   8,             BU_O | NO_ZU,                A_SPECIAL,               K_L | 2,    0,   10,   40,     0, 0x1f,   4,  0x80,       3, SC300,       12,   0,  90,  52,    0,      NO_CHIP | NO_WEIGHT,   1,     0,   0,     0,    30,   0, YK_GD030,     { 0, 0 } },	/* 3c */
	{   0,       2 | NO_VEC | NO_ZU,                A_SPECIAL,               K_L | 2,    0,    6,   20,     0, 0x01,  14,  0x60,       3, SC200,        8,   0, 100,  21,    0,                NO_WEIGHT,   1,     0,   0,     0,     0,   0, YK_GD030,     { 0, 0 } },	/* 3d: pl1a_at_throw, Grabs, leaps and drops on the opponent (sp_throw) */
};
