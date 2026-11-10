/* Abyss (second form) (character 19): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_19      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_19     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl19.c) says which group
                   and number each move uses.
   pl19_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl19_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl19_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl19_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_19
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_19, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_19 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl19_stand_00[], pl19_stand_02[], pl19_stand_02_loop[], pl19_stand_04[],
	pl19_stand_03[], pl19_stand_03_loop[], pl19_stand_05[], pl19_jump_00[], pl19_jump_00_loop[],
	pl19_crouch_01[], pl19_crouch_02[], pl19_turn_00[], pl19_guard_00[], pl19_damage_01[],
	pl19_damage_02[], pl19_damage_06[], pl19_damage_06_loop[], pl19_win_01[], pl19_win_03[],
	pl19_win_03_loop[], pl19_win_02[], pl19_appear_00[], pl19_extra_00[], pl19_extra_00_loop[],
	pl19_extra_03[], pl19_extra_03_loop[], pl19_extra_04[], pl19_extra_04_loop[], pl19_extra_05[],
	pl19_extra_06[], pl19_extra_06_loop[], pl19_extra_07[], pl19_extra_07_loop[], pl19_extra_08[],
	pl19_extra_09[], pl19_extra_01[], pl19_extra_01_loop[], pl19_extra_02[], pl19_extra_10[],
	pl19_extra_10_loop[], pl19_extra_11[], pl19_super_00[], pl19_super_00_loop[], pl19_super_01[],
	pl19_super_02[], pl19_super_03[], pl19_super_04[], pl19_super_05[], pl19_change_00[],
	pl19_grp25_00[], pl19_grp25_01[], pl19_grp25_02[], pl19_grp25_03[], pl19_grp25_04[],
	pl19_grp25_05[], pl19_grp27_00[], pl19_grp27_01[], pl19_grp27_02[], pl19_grp27_02_loop[],
	pl19_grp27_03[], pl19_grp27_04[], pl19_grp27_05[], pl19_grp27_06[], pl19_grp27_07[],
	pl19_grp27_08[], pl19_grp27_09[], pl19_grp27_10[], pl19_grp27_11[], pl19_grp27_11_loop[],
	pl19_grp27_12[], pl19_grp27_12_loop[];
extern const Sint32 *const pl19_stand_tbl[], *const pl19_jump_tbl[], *const pl19_dash_tbl[],
	*const pl19_crouch_tbl[], *const pl19_turn_tbl[], *const pl19_guard_tbl[],
	*const pl19_air_guard_tbl[], *const pl19_stand_p_tbl[], *const pl19_stand_k_tbl[],
	*const pl19_crouch_p_tbl[], *const pl19_crouch_k_tbl[], *const pl19_jump_p_tbl[],
	*const pl19_jump_k_tbl[], *const pl19_damage_tbl[], *const pl19_dizzy_tbl[],
	*const pl19_catch_tbl[], *const pl19_get_up_tbl[], *const pl19_appear_tbl[],
	*const pl19_win_tbl[], *const pl19_extra_tbl[], *const pl19_super_tbl[],
	*const pl19_change_tbl[], *const pl19_grp25_tbl[], *const pl19_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_19[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_19[] = {
	pl19_stand_tbl, pl19_jump_tbl, pl19_dash_tbl, pl19_crouch_tbl, pl19_turn_tbl, pl19_guard_tbl,
	pl19_air_guard_tbl, pl19_stand_p_tbl, pl19_stand_k_tbl, pl19_crouch_p_tbl, pl19_crouch_k_tbl, pl19_jump_p_tbl,
	pl19_jump_k_tbl, pl19_damage_tbl, pl19_dizzy_tbl, pl19_catch_tbl, pl19_catch_tbl, pl19_get_up_tbl,
	pl19_appear_tbl, pl19_win_tbl, pl19_extra_tbl, pl19_extra_tbl, pl19_super_tbl, pl19_super_tbl,
	pl19_change_tbl, pl19_grp25_tbl, pl19_grp25_tbl, pl19_grp27_tbl, 0,
};

const Sint32 *const pl19_stand_tbl[] = {
	pl19_stand_00, pl19_stand_00, pl19_stand_02, pl19_stand_03, pl19_stand_04, pl19_stand_05,
	pl19_stand_02, pl19_stand_03, 0,
};

const Sint32 *const pl19_jump_tbl[] = {
	pl19_jump_00, pl19_jump_00, pl19_jump_00, pl19_jump_00, pl19_jump_00, pl19_jump_00,
	pl19_jump_00, pl19_jump_00, pl19_jump_00, pl19_jump_00, pl19_jump_00, pl19_jump_00,
};

const Sint32 *const pl19_dash_tbl[] = {
	pl19_jump_00, pl19_jump_00, pl19_jump_00, pl19_jump_00,
};

const Sint32 *const pl19_crouch_tbl[] = {
	pl19_jump_00, pl19_crouch_01, pl19_crouch_02,
};

const Sint32 *const pl19_turn_tbl[] = {
	pl19_turn_00, pl19_turn_00,
};

const Sint32 *const pl19_guard_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00,
	pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_air_guard_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_stand_p_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_stand_k_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_crouch_p_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_crouch_k_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_jump_p_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_jump_k_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_damage_tbl[] = {
	pl19_guard_00, pl19_damage_01, pl19_damage_02, pl19_guard_00, pl19_damage_01, pl19_damage_02,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
};

const Sint32 *const pl19_dizzy_tbl[] = {
	pl19_damage_06,
};

const Sint32 *const pl19_catch_tbl[] = {
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06,
	pl19_damage_06, pl19_damage_06, pl19_damage_06, pl19_damage_06, 0,
};

const Sint32 *const pl19_get_up_tbl[] = {
	pl19_guard_00, pl19_guard_00, pl19_guard_00,
};

const Sint32 *const pl19_appear_tbl[] = {
	pl19_appear_00, 0,
};

const Sint32 *const pl19_win_tbl[] = {
	pl19_damage_06, pl19_win_01, pl19_win_02, pl19_win_03,
};

const Sint32 *const pl19_extra_tbl[] = {
	pl19_extra_00, pl19_extra_01, pl19_extra_02, pl19_extra_03, pl19_extra_04, pl19_extra_05,
	pl19_extra_06, pl19_extra_07, pl19_extra_08, pl19_extra_09, pl19_extra_10, pl19_extra_11,
	0,
};

const Sint32 *const pl19_super_tbl[] = {
	pl19_super_00, pl19_super_01, pl19_super_02, pl19_super_03, pl19_super_04, pl19_super_05,
	0,
};

const Sint32 *const pl19_change_tbl[] = {
	pl19_change_00, pl19_change_00, pl19_change_00, pl19_change_00, pl19_change_00, pl19_change_00,
	0,
};

const Sint32 *const pl19_grp25_tbl[] = {
	pl19_grp25_00, pl19_grp25_01, pl19_grp25_02, pl19_grp25_03, pl19_grp25_04, pl19_grp25_05,
	0,
};

const Sint32 *const pl19_grp27_tbl[] = {
	pl19_grp27_00, pl19_grp27_01, pl19_grp27_02, pl19_grp27_03, pl19_grp27_04, pl19_grp27_05,
	pl19_grp27_06, pl19_grp27_07, pl19_grp27_08, pl19_grp27_09, pl19_grp27_10, pl19_grp27_11,
	pl19_grp27_12, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2  rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1 */
const Sint32 pl19_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,    37, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    38, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    39, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    40, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    41, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    42, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    43, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    44, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    45, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    46, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    47, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    48, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    49, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    50, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    51, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    52, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl19_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
};

const Sint32 pl19_stand_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   134, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_stand_02_loop ),
};

/* Char_Stand 4 */
const Sint32 pl19_stand_04[] = {
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   142, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_stand_00 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl19_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
};

const Sint32 pl19_stand_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   153, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_stand_03_loop ),
};

/* Char_Stand 5 */
const Sint32 pl19_stand_05[] = {
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   143, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_stand_00 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2
   Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl19_jump_00[] = {
	CG_FRAME(    0,    0,   1,           0,    53, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    54, 0x80,   0xff,    0,  0x00,       0x14,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    55, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    56, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    57, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    58, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    59, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    60, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    61, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    62, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    1,   1,           0,    63, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    64, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    65, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    66, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    67, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    68, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    69, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    70, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    71, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    72, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    73, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    74, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    75, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    76, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    77, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    78, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    79, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    80, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    81, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    82, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    83, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    84, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
};

const Sint32 pl19_jump_00_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,    85, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_jump_00_loop ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl19_crouch_01[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    -1, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_crouch_01 ),
};

/* Char_Crouch 2: bubble_back (bubbles that hold whoever they touch), act_stand_up, act_crouch */
const Sint32 pl19_crouch_02[] = {
	CG_FRAME(    0,    1,   1,           0,    86, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    87, 0x80,   0xff,    0,  0x00,       0x14,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    88, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    89, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    90, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    91, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    92, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    93, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    94, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    95, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    96, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    97, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    98, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    99, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   100, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   101, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   102, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   103, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   104, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   105, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   106, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   107, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   108, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   109, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   110, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   111, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   112, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   113, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   114, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   115, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   116, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_crouch_02 ),
};

/* Char_Turn 0: act_turn
   Char_Turn 1: act_crouch_turn */
const Sint32 pl19_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   212, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   215, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   216, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   217, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   219, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_turn_00 ),
};

/* Char_Guard 0
   Char_Guard 1: act_guard_hold
   Char_Guard 2
   Char_Guard 3
   Char_Guard 4
   Char_Guard 5: act_guard_hold */
const Sint32 pl19_guard_00[] = {
	CG_FRAME(    0,    0,   5,           0,   331, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   332, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_guard_00 ),
};

/* Char_Damage 1
   Char_Damage 4 */
const Sint32 pl19_damage_01[] = {
	CG_FRAME(    0,    0,   8,           0,   331, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   332, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_damage_01 ),
};

/* Char_Damage 2
   Char_Damage 5 */
const Sint32 pl19_damage_02[] = {
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_damage_02 ),
};

/* Char_Damage 6
   Char_Damage 7
   Char_Damage 8
   Char_Damage 9: dm_sweep
   Char_Damage 10: dm_ko_blow
   Char_Damage 11 */
const Sint32 pl19_damage_06[] = {
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0xff,    0,  0x00,       0x19,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   233, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
};

const Sint32 pl19_damage_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   234, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   235, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_damage_06_loop ),
};

/* Char_Win 1: fly_init */
const Sint32 pl19_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   155, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   158, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   159, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0xff,    0,  0x00,       0x19,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   242, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_win_01 ),
};

/* Char_Win 3: fly_start */
const Sint32 pl19_win_03[] = {
	CG_FRAME(    0,    0,   1,           0,   246, 0x80,   0xff,    0,  0x00,       0x1a,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   246, 0x80,   0xff,    0,  0x00,       0x0b,    0,   0,     0,     0,   0 ),
};

const Sint32 pl19_win_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   246, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_win_03_loop ),
};

/* Char_Win 2 */
const Sint32 pl19_win_02[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    37, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_stand_00 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl19_appear_00[] = {
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0xff,    0,  0x00,       0x14,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x80,   0xff,    0,  0x00,       0x0a,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   156, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   157, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   158, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   159, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   160, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   159, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   158, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   156, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   155, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   154, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   161, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_appear_00 ),
};

/* Char_Extra 0: t_kick_init
   Char_Special 0: change_init (he fades, to change into the third form) */
const Sint32 pl19_extra_00[] = {
	CG_FRAME(    0,    0,   6,           0,   162, 0x10,   0xff,    0,  0x00,       0x18,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   163, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   164, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   165, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   166, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   167, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   169, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   170, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   171, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   172, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   173, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   174, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   175, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   176, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   177, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   179, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   180, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   181, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   182, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   183, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   184, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   185, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   186, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   187, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   188, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   190, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   191, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   192, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   193, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   194, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   195, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   196, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   197, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   198, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   199, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   200, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   201, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   202, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   203, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   204, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   205, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   206, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   207, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   208, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   209, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
};

const Sint32 pl19_extra_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   210, 0x10,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_extra_00_loop ),
};

/* Char_Extra 3: fly_start5
   Char_Special 3: storm_init (shots in every direction) */
const Sint32 pl19_extra_03[] = {
	CG_FRAME(    0,    0,   4,           0,   293, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   295, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   296, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   4 ),
};

const Sint32 pl19_extra_03_loop[] = {
	CG_FRAME( 0x80,    0,   4,           0,   299, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   4 ),
	CG_FRAME( 0x80,    0,   4, END_OF_DATA,   300, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   4 ),
	CG_NEXT( pl19_extra_03_loop ),
};

/* Char_Extra 4
   Char_Special 4: storm_set (shots in every direction) */
const Sint32 pl19_extra_04[] = {
	CG_FRAME(    0,    0,   1,           0,   301, 0x80,   0xff,  ATK,  0x00,       0x03,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0xff,  ATK,  0x00, 0x18 | CSE,    0,   0,     0,     0,   5 ),
};

const Sint32 pl19_extra_04_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   302, 0x80,   0xff,  ATK,  0x00,       0x17,    0,   0,     0,     0,   5 ),
	CG_FRAME(    1,    1,   4,           0,   303, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    2,    1,   4,           0,   304, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    3,    1,   4, END_OF_DATA,   305, 0x80,   0xff,  ATK,  0x00, 0x18 | CSE,    0,   0,     0,     0,   5 ),
	CG_NEXT( pl19_extra_04_loop ),
};

/* Char_Extra 5
   Char_Special 5: storm_move (shots in every direction) */
const Sint32 pl19_extra_05[] = {
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   309, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   311, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   143, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_extra_05 ),
};

/* Char_Extra 6
   Char_Special 6: beam_init (a beam, longer for pressed buttons, 6 + atck_level) */
const Sint32 pl19_extra_06[] = {
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    1,    0,  32,           0,   316, 0x80,   0xff,  ATK,  0x00,       0x0b,    0,   0,     0,     0,   3 ),
	CG_FRAME(    1,    1,   4,           0,   316, 0x80,   0xff,  ATK,  0x00,       0x15,    0,   0,     0,     0,   3 ),
};

const Sint32 pl19_extra_06_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   321, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    1,    1,   2, END_OF_DATA,   322, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_NEXT( pl19_extra_06_loop ),
};

/* Char_Extra 7
   Char_Special 7 */
const Sint32 pl19_extra_07[] = {
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    1,    0,  32,           0,   316, 0x80,   0xff,  ATK,  0x00,       0x0b,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    1,   4,           0,   316, 0x80,   0xff,  ATK,  0x00,       0x1b,    0,   0,     0,     0,   3 ),
};

const Sint32 pl19_extra_07_loop[] = {
	CG_FRAME(    2,    1,   2,           0,   317, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    3,    1,   2,           0,   318, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    4,    1,   2,           0,   319, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    5,    1,   2, END_OF_DATA,   320, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_NEXT( pl19_extra_07_loop ),
};

/* Char_Extra 8
   Char_Special 8: beam_hold (a beam, longer for pressed buttons, 8 + atck_level) */
const Sint32 pl19_extra_08[] = {
	CG_FRAME(    0,    0,  32,           0,   316, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   312, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_extra_08 ),
};

/* Char_Extra 9
   Char_Special 9 */
const Sint32 pl19_extra_09[] = {
	CG_FRAME(    0,    0,   6,           0,   317, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,  48,           0,   316, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   312, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_extra_09 ),
};

/* Char_Extra 1: t_kick_jump
   Char_Special 1 */
const Sint32 pl19_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0xff,  ATK,  0x00,       0x0b,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0xff,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,     0,   3 ),
};

const Sint32 pl19_extra_01_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   329, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   330, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_NEXT( pl19_extra_01_loop ),
};

/* Char_Extra 2: fly_start5
   Char_Special 2 */
const Sint32 pl19_extra_02[] = {
	CG_FRAME(    0,    0,   9,           0,   326, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,   8,           0,   325, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   7,           0,   324, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   323, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_extra_02 ),
};

/* Char_Extra 10
   Char_Special 10 */
const Sint32 pl19_extra_10[] = {
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0xff,  ATK,  0x00,       0x0b,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0xff,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,     0,   3 ),
};

const Sint32 pl19_extra_10_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   329, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   330, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_NEXT( pl19_extra_10_loop ),
};

/* Char_Extra 11
   Char_Special 11 */
const Sint32 pl19_extra_11[] = {
	CG_FRAME(    0,    0,  16,           0,   326, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   3 ),
	CG_FRAME(    0,    0,   8,           0,   325, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   7,           0,   324, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   323, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,     0,   1 ),
	CG_NEXT( pl19_extra_11 ),
};

/* Char_Super 0
   Char_Effect 0: shl19_01_init */
const Sint32 pl19_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   363, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  23 ),
};

const Sint32 pl19_super_00_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   367, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  24 ),
	CG_FRAME(    0,    1,   2,           0,   368, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  24 ),
	CG_FRAME(    1,    1,   2,           0,   369, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  24 ),
	CG_FRAME(    0,    1,   2,           0,   370, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  24 ),
	CG_FRAME(    0,    1,   2,           0,   371, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  24 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   372, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  24 ),
	CG_NEXT( pl19_super_00_loop ),
};

/* Char_Super 1
   Char_Effect 1: shl19_01_move */
const Sint32 pl19_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   373, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_super_01 ),
};

/* Char_Super 2
   Char_Effect 2: shl19_02_init */
const Sint32 pl19_super_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  20 ),
	CG_NEXT( pl19_super_02 ),
};

/* Char_Super 3
   Char_Effect 3: shl19_00_init */
const Sint32 pl19_super_03[] = {
	CG_FRAME(    0,    0,   1,           0,   248, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   249, 0x80,   0x00,    0,  0x00,       0x16,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32777, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32778, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32779, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_super_03 ),
};

/* Char_Super 4
   Char_Effect 4: shl19_00_start */
const Sint32 pl19_super_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32780, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,  21 ),
	CG_NEXT( pl19_super_04 ),
};

/* Char_Super 5
   Char_Effect 5: shl19_00_out */
const Sint32 pl19_super_05[] = {
	CG_FRAME(    0,    0,   2,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_super_05 ),
};

/* Char_Change 0: act_change_tag_init
   Char_Change 1: act_change_vcounter_init
   Char_Change 2: act_change_out_bound
   Char_Change 3: act_stand, act_change_call
   Char_Change 4: act_change_assist_end
   Char_Change 5 */
const Sint32 pl19_change_00[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    37, 0x00,   0xff,    0,  0x00,          0,    0,   0,     0,     0,   0 ),
	CG_NEXT( pl19_change_00 ),
};

/* group 25 0
   Char_Fly 0 */
const Sint32 pl19_grp25_00[] = {
	CG_FRAME(    0,    0,   4,           0, 32794, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  18 ),
};

/* group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl19_grp25_01[] = {
	CG_FRAME(    0,    0,   4,           0, 32797, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  19 ),
};

/* group 25 2
   Char_Fly 2 */
const Sint32 pl19_grp25_02[] = {
	CG_FRAME(    0,    0,   4,           0, 32795, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  18 ),
};

/* group 25 3
   Char_Fly 3 */
const Sint32 pl19_grp25_03[] = {
	CG_FRAME(    0,    0,   4,           0, 32798, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  19 ),
};

/* group 25 4
   Char_Fly 4 */
const Sint32 pl19_grp25_04[] = {
	CG_FRAME(    0,    0,   4,           0, 32793, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  18 ),
};

/* group 25 5
   Char_Fly 5 */
const Sint32 pl19_grp25_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32796, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,     0,  19 ),
	CG_NEXT( pl19_grp25_00 ),
};

/* group 27 0 */
const Sint32 pl19_grp27_00[] = {
	EF_FRAME(    0,    0,   4,           0,   339 ),
	EF_FRAME(    0,    0,   4,           0,   340 ),
	EF_FRAME(    0,    0,   4,           0,   341 ),
	EF_FRAME(    0,    0,   4,           0,   342 ),
	EF_FRAME(    0,    0,   4,           0,   343 ),
	EF_FRAME(    0,    0,   4,           0,   344 ),
	EF_FRAME(    0,    0,   4,           0,   345 ),
	EF_FRAME(    0,    0,   4,           0,   346 ),
	EF_FRAME(    0,    0,   4,           0,   347 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   348 ),
	CG_NEXT( pl19_grp27_00 ),
};

/* group 27 1: pls19_02_init */
const Sint32 pl19_grp27_01[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA,    37 ),
	CG_NEXT( pl19_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl19_grp27_02[] = {
	EF_FRAME(    0,    0,   2,           0, 32785 ),
	EF_FRAME(    0,    0,   2,           0, 32786 ),
	EF_FRAME(    0,    0,   2,           0, 32787 ),
	EF_FRAME(    0,    0,   2,           0, 32788 ),
	EF_FRAME(    0,    0,   2,           0, 32789 ),
	EF_FRAME(    0,    0,   2,           0, 32790 ),
};

const Sint32 pl19_grp27_02_loop[] = {
	EF_FRAME(    0,    0,   2,           0, 32791 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32792 ),
	CG_NEXT( pl19_grp27_02_loop ),
};

/* group 27 3 */
const Sint32 pl19_grp27_03[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32804 ),
	CG_NEXT( pl19_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl19_grp27_04[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32803 ),
	CG_NEXT( pl19_grp27_03 ),
};

/* group 27 5 */
const Sint32 pl19_grp27_05[] = {
	EF_FRAME(    0,    0,   4,           0, 32801 ),
	EF_FRAME(    0,    0,   4,           0, 32802 ),
	EF_FRAME(    0,    0,   4,           0, 32799 ),
	EF_FRAME(    0,    0,   4,           0, 32800 ),
	EF_FRAME(    0,    0,   4,           0, 32799 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA, 32800 ),
	CG_NEXT( pl19_grp27_04 ),
};

/* group 27 6 */
const Sint32 pl19_grp27_06[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0, 32782 ),
	EF_FRAME(    0,    0,   1,           0, 32783 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32782 ),
	CG_NEXT( pl19_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl19_grp27_07[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0, 32783 ),
	EF_FRAME(    0,    0,   4,           0, 32784 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32783 ),
	CG_NEXT( pl19_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl19_grp27_08[] = {
	EF_FRAME(    0,    0,   1,           0,   244 ),
};

/* group 27 9 */
const Sint32 pl19_grp27_09[] = {
	EF_FRAME(    0,    0,   1,           0,   245 ),
};

/* group 27 10 */
const Sint32 pl19_grp27_10[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   243 ),
	CG_NEXT( pl19_grp27_10 ),
};

/* group 27 11 */
const Sint32 pl19_grp27_11[] = {
	EF_FRAME(    0,    0,   4,           0,   363 ),
	EF_FRAME(    0,    0,   4,           0,   364 ),
	EF_FRAME(    0,    0,   4,           0,   365 ),
	EF_FRAME(    0,    0,   4,           0,   366 ),
};

const Sint32 pl19_grp27_11_loop[] = {
	EF_FRAME(    0,    0,   4,           0,   367 ),
	EF_FRAME(    0,    0,   4,           0,   368 ),
	EF_FRAME(    0,    0,   4,           0,   369 ),
	EF_FRAME(    0,    0,   4,           0,   370 ),
	EF_FRAME(    0,    0,   4,           0,   371 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   372 ),
	CG_NEXT( pl19_grp27_11_loop ),
};

/* group 27 12 */
const Sint32 pl19_grp27_12[] = {
	EF_FRAME(    0,    0,   5,           0,   363 ),
	EF_FRAME(    0,    0,   5,           0,   364 ),
	EF_FRAME(    0,    0,   5,           0,   365 ),
	EF_FRAME(    0,    0,   5,           0,   366 ),
};

const Sint32 pl19_grp27_12_loop[] = {
	EF_FRAME(    0,    0,   5,           0,   367 ),
	EF_FRAME(    0,    0,   5,           0,   368 ),
	EF_FRAME(    0,    0,   5,           0,   369 ),
	EF_FRAME(    0,    0,   5,           0,   370 ),
	EF_FRAME(    0,    0,   5,           0,   371 ),
	EF_FRAME(    0,    0,   5, END_OF_DATA,   372 ),
	CG_NEXT( pl19_grp27_12_loop ),
};
