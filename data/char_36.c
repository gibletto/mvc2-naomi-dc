/* Thanos (character 36): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_36      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_36     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl36.c) says which group
                   and number each move uses.
   pl36_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl36_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl36_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl36_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_36
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_36, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_36 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl36_super_14[], pl36_stand_00[], pl36_stand_02[], pl36_stand_06[],
	pl36_stand_03[], pl36_stand_07[], pl36_jump_00[], pl36_jump_09[], pl36_change_02[],
	pl36_jump_03[], pl36_dash_00[], pl36_dash_02[], pl36_dash_01[], pl36_dash_01_loop[],
	pl36_dash_03[], pl36_crouch_00[], pl36_crouch_01[], pl36_crouch_02[], pl36_turn_00[],
	pl36_turn_01[], pl36_guard_00[], pl36_guard_01[], pl36_guard_03[], pl36_guard_02[],
	pl36_guard_04[], pl36_guard_05[], pl36_guard_07[], pl36_guard_06[], pl36_air_guard_00[],
	pl36_air_guard_01[], pl36_air_guard_02[], pl36_unused_00[], pl36_stand_p_00[],
	pl36_stand_p_00_loop[], pl36_stand_p_01[], pl36_stand_p_01_loop[], pl36_stand_p_02[],
	pl36_stand_p_02_loop[], pl36_stand_k_00[], pl36_stand_k_00_loop[], pl36_stand_k_01[],
	pl36_stand_k_01_loop[], pl36_stand_k_02[], pl36_stand_k_02_loop[], pl36_extra_04[],
	pl36_extra_04_loop[], pl36_stand_k_03[], pl36_stand_k_03_loop[], pl36_crouch_p_00[],
	pl36_crouch_p_00_loop[], pl36_crouch_p_01[], pl36_crouch_p_01_loop[], pl36_grp27_16[],
	pl36_extra_06[], pl36_extra_06_loop[], pl36_extra_05[], pl36_extra_05_loop[], pl36_crouch_p_02[],
	pl36_crouch_p_02_loop[], pl36_crouch_k_00[], pl36_crouch_k_00_loop[], pl36_crouch_k_01[],
	pl36_crouch_k_01_loop[], pl36_crouch_k_02[], pl36_crouch_k_02_loop[], pl36_jump_p_00[],
	pl36_jump_p_00_loop[], pl36_jump_p_01[], pl36_jump_p_01_loop[], pl36_jump_p_02[],
	pl36_jump_p_02_loop[], pl36_extra_01[], pl36_jump_k_00[], pl36_jump_k_00_loop[], pl36_extra_03[],
	pl36_extra_03_loop[], pl36_jump_k_01[], pl36_jump_k_01_loop[], pl36_jump_k_02[],
	pl36_jump_k_02_loop[], pl36_jump_k_05[], pl36_jump_k_05_loop[], pl36_jump_k_06[],
	pl36_jump_k_06_loop[], pl36_get_up_00[], pl36_get_up_00_loop[], pl36_get_up_01[],
	pl36_get_up_01_loop[], pl36_get_up_02[], pl36_get_up_02_loop[], pl36_get_up_03[],
	pl36_get_up_03_loop[], pl36_damage_00[], pl36_damage_01[], pl36_damage_02[], pl36_damage_03[],
	pl36_damage_04[], pl36_damage_05[], pl36_damage_06[], pl36_damage_07[], pl36_damage_08[],
	pl36_damage_10[], pl36_damage_11[], pl36_damage_12[], pl36_damage_13[], pl36_damage_09[],
	pl36_damage_09_loop[], pl36_damage_16[], pl36_damage_17[], pl36_damage_18[], pl36_damage_19[],
	pl36_damage_20[], pl36_damage_21[], pl36_damage_22[], pl36_damage_23[], pl36_damage_24[],
	pl36_damage_25[], pl36_damage_26[], pl36_damage_26_loop[], pl36_damage_34[], pl36_damage_31[],
	pl36_damage_28[], pl36_damage_29[], pl36_caught_00[], pl36_caught_01[], pl36_caught_02[],
	pl36_caught_03[], pl36_caught_04[], pl36_caught_05[], pl36_caught_06[], pl36_caught_07[],
	pl36_caught_08[], pl36_caught_09[], pl36_caught_10[], pl36_caught_11[], pl36_caught_12[],
	pl36_caught_13[], pl36_caught_14[], pl36_caught_15[], pl36_caught_16[], pl36_caught_17[],
	pl36_caught_18[], pl36_caught_19[], pl36_caught_20[], pl36_caught_21[], pl36_dizzy_00[],
	pl36_catch_00[], pl36_catch_00_loop[], pl36_catch_01[], pl36_catch_01_loop[], pl36_grp27_14[],
	pl36_win_00[], pl36_win_00_loop[], pl36_win_01[], pl36_win_01_loop[], pl36_grp27_13[],
	pl36_grp27_13_loop[], pl36_win_02[], pl36_win_02_loop[], pl36_appear_00[], pl36_special_00[],
	pl36_special_01[], pl36_special_02[], pl36_special_03[], pl36_special_04[], pl36_special_05[],
	pl36_special_06[], pl36_special_07[], pl36_special_08[], pl36_effect_04[], pl36_effect_05[],
	pl36_effect_06[], pl36_effect_07[], pl36_special_09[], pl36_special_09_loop[], pl36_change_01[],
	pl36_change_00[], pl36_change_00_loop[], pl36_change_03[], pl36_change_03_loop[],
	pl36_change_04[], pl36_change_04_loop[], pl36_special_10[], pl36_special_10_loop[],
	pl36_special_11[], pl36_special_11_loop[], pl36_special_12[], pl36_special_12_loop[],
	pl36_special_13[], pl36_special_13_loop[], pl36_special_14[], pl36_special_14_loop[],
	pl36_super_00[], pl36_super_00_loop[], pl36_super_01[], pl36_super_02[], pl36_super_03[],
	pl36_super_03_loop[], pl36_super_04[], pl36_super_05[], pl36_super_06[], pl36_super_07[],
	pl36_super_08[], pl36_super_08_loop[], pl36_super_09[], pl36_super_09_loop[], pl36_super_18[],
	pl36_super_18_loop[], pl36_super_19[], pl36_super_19_loop[], pl36_super_10[],
	pl36_super_10_loop[], pl36_super_12[], pl36_super_13[], pl36_super_20[], pl36_catch_02[],
	pl36_super_22[], pl36_super_23[], pl36_super_24[], pl36_super_25[], pl36_super_26[],
	pl36_grp27_15[], pl36_grp27_00[], pl36_grp27_01[], pl36_grp27_02[], pl36_grp27_03[],
	pl36_grp27_04[], pl36_grp27_05[], pl36_grp27_06[], pl36_grp27_07[], pl36_grp27_09[],
	pl36_grp27_10[], pl36_grp27_11[], pl36_grp27_12[], pl36_super_58[], pl36_super_59[],
	pl36_super_59_loop[], pl36_super_60[], pl36_super_60_loop[], pl36_super_61[],
	pl36_super_61_loop[], pl36_super_62[], pl36_super_62_loop[], pl36_super_63[], pl36_super_64[],
	pl36_super_65[], pl36_super_67[], pl36_super_66[], pl36_super_68[], pl36_super_69[],
	pl36_super_70[], pl36_grp27_17[], pl36_grp27_18[], pl36_grp27_19[], pl36_grp27_20[],
	pl36_grp27_21[], pl36_grp27_22[];
extern const Sint32 *const pl36_stand_tbl[], *const pl36_jump_tbl[], *const pl36_dash_tbl[],
	*const pl36_crouch_tbl[], *const pl36_turn_tbl[], *const pl36_guard_tbl[],
	*const pl36_air_guard_tbl[], *const pl36_stand_p_tbl[], *const pl36_stand_k_tbl[],
	*const pl36_crouch_p_tbl[], *const pl36_crouch_k_tbl[], *const pl36_jump_p_tbl[],
	*const pl36_jump_k_tbl[], *const pl36_damage_tbl[], *const pl36_dizzy_tbl[],
	*const pl36_catch_tbl[], *const pl36_caught_tbl[], *const pl36_get_up_tbl[],
	*const pl36_appear_tbl[], *const pl36_win_tbl[], *const pl36_extra_tbl[],
	*const pl36_special_tbl[], *const pl36_super_tbl[], *const pl36_effect_tbl[],
	*const pl36_change_tbl[], *const pl36_grp25_tbl[], *const pl36_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_36[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -4096, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -8192, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -12288, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -16384, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -20480, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -24576, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -28672, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 28672, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 24576, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 20480, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 16384, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 12288, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 8192, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 4096, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_36[] = {
	pl36_stand_tbl, pl36_jump_tbl, pl36_dash_tbl, pl36_crouch_tbl, pl36_turn_tbl, pl36_guard_tbl,
	pl36_air_guard_tbl, pl36_stand_p_tbl, pl36_stand_k_tbl, pl36_crouch_p_tbl, pl36_crouch_k_tbl, pl36_jump_p_tbl,
	pl36_jump_k_tbl, pl36_damage_tbl, pl36_dizzy_tbl, pl36_catch_tbl, pl36_caught_tbl, pl36_get_up_tbl,
	pl36_appear_tbl, pl36_win_tbl, pl36_extra_tbl, pl36_special_tbl, pl36_super_tbl, pl36_effect_tbl,
	pl36_change_tbl, pl36_grp25_tbl, pl36_grp25_tbl, pl36_grp27_tbl, 0,
};

const Sint32 *const pl36_stand_tbl[] = {
	pl36_stand_00, pl36_stand_00, pl36_stand_02, pl36_stand_03, pl36_stand_00, pl36_stand_00,
	pl36_stand_06, pl36_stand_07, 0,
};

const Sint32 *const pl36_jump_tbl[] = {
	pl36_jump_00, pl36_jump_00, pl36_jump_00, pl36_jump_03, pl36_jump_03, pl36_jump_03,
	pl36_jump_00, pl36_jump_00, pl36_jump_00, pl36_jump_09, pl36_jump_09, pl36_jump_09,
	0,
};

const Sint32 *const pl36_dash_tbl[] = {
	pl36_dash_00, pl36_dash_01, pl36_dash_02, pl36_dash_03, 0,
};

const Sint32 *const pl36_crouch_tbl[] = {
	pl36_crouch_00, pl36_crouch_01, pl36_crouch_02, 0,
};

const Sint32 *const pl36_turn_tbl[] = {
	pl36_turn_00, pl36_turn_01, 0,
};

const Sint32 *const pl36_guard_tbl[] = {
	pl36_guard_00, pl36_guard_01, pl36_guard_02, pl36_guard_03, pl36_guard_04, pl36_guard_05,
	pl36_guard_06, pl36_guard_07, 0,
};

const Sint32 *const pl36_air_guard_tbl[] = {
	pl36_air_guard_00, pl36_air_guard_01, pl36_air_guard_02, 0,
};

const Sint32 *const pl36_stand_p_tbl[] = {
	pl36_stand_p_00, pl36_stand_p_01, pl36_stand_p_02, 0,
};

const Sint32 *const pl36_stand_k_tbl[] = {
	pl36_stand_k_00, pl36_stand_k_01, pl36_stand_k_02, pl36_stand_k_03, 0,
};

const Sint32 *const pl36_crouch_p_tbl[] = {
	pl36_crouch_p_00, pl36_crouch_p_01, pl36_crouch_p_02, 0,
};

const Sint32 *const pl36_crouch_k_tbl[] = {
	pl36_crouch_k_00, pl36_crouch_k_01, pl36_crouch_k_02, 0,
};

const Sint32 *const pl36_jump_p_tbl[] = {
	pl36_jump_p_00, pl36_jump_p_01, pl36_jump_p_02, pl36_jump_p_00, pl36_jump_p_01, pl36_jump_p_02,
	0,
};

const Sint32 *const pl36_jump_k_tbl[] = {
	pl36_jump_k_00, pl36_jump_k_01, pl36_jump_k_02, pl36_jump_k_00, pl36_jump_k_01, pl36_jump_k_05,
	pl36_jump_k_06, 0,
};

const Sint32 *const pl36_damage_tbl[] = {
	pl36_damage_00, pl36_damage_01, pl36_damage_02, pl36_damage_03, pl36_damage_04, pl36_damage_05,
	pl36_damage_06, pl36_damage_07, pl36_damage_08, pl36_damage_09, pl36_damage_10, pl36_damage_11,
	pl36_damage_12, pl36_damage_13, pl36_damage_13, pl36_damage_13, pl36_damage_16, pl36_damage_17,
	pl36_damage_18, pl36_damage_19, pl36_damage_20, pl36_damage_21, pl36_damage_22, pl36_damage_23,
	pl36_damage_24, pl36_damage_25, pl36_damage_26, pl36_jump_09, pl36_damage_28, pl36_damage_29,
	pl36_jump_09, pl36_damage_31, pl36_jump_09, pl36_jump_09, pl36_damage_34, 0,
};

const Sint32 *const pl36_dizzy_tbl[] = {
	pl36_dizzy_00, 0,
};

const Sint32 *const pl36_catch_tbl[] = {
	pl36_catch_00, pl36_catch_01, pl36_catch_02, 0,
};

const Sint32 *const pl36_caught_tbl[] = {
	pl36_caught_00, pl36_caught_01, pl36_caught_02, pl36_caught_03, pl36_caught_04, pl36_caught_05,
	pl36_caught_06, pl36_caught_07, pl36_caught_08, pl36_caught_09, pl36_caught_10, pl36_caught_11,
	pl36_caught_12, pl36_caught_13, pl36_caught_14, pl36_caught_15, pl36_caught_16, pl36_caught_17,
	pl36_caught_18, pl36_caught_19, pl36_caught_20, pl36_caught_21, 0,
};

const Sint32 *const pl36_get_up_tbl[] = {
	pl36_get_up_00, pl36_get_up_01, pl36_get_up_02, pl36_get_up_03, 0,
};

const Sint32 *const pl36_appear_tbl[] = {
	pl36_appear_00, 0,
};

const Sint32 *const pl36_win_tbl[] = {
	pl36_win_00, pl36_win_01, pl36_win_02, 0,
};

const Sint32 *const pl36_extra_tbl[] = {
	pl36_stand_p_00, pl36_extra_01, pl36_jump_k_05, pl36_extra_03, pl36_extra_04, pl36_extra_05,
	pl36_extra_06, 0,
};

const Sint32 *const pl36_special_tbl[] = {
	pl36_special_00, pl36_special_01, pl36_special_02, pl36_special_03, pl36_special_04, pl36_special_05,
	pl36_special_06, pl36_special_07, pl36_special_08, pl36_special_09, pl36_special_10, pl36_special_11,
	pl36_special_12, pl36_special_13, pl36_special_14, 0,
};

const Sint32 *const pl36_super_tbl[] = {
	pl36_super_00, pl36_super_01, pl36_super_02, pl36_super_03, pl36_super_04, pl36_super_05,
	pl36_super_06, pl36_super_07, pl36_super_08, pl36_super_09, pl36_super_10, pl36_super_10,
	pl36_super_12, pl36_super_13, pl36_super_14, pl36_super_14, pl36_super_14, pl36_super_14,
	pl36_super_18, pl36_super_19, pl36_super_20, pl36_catch_02, pl36_super_22, pl36_super_23,
	pl36_super_24, pl36_super_25, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26,
	pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26,
	pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26,
	pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26,
	pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26,
	pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_26, pl36_super_58, pl36_super_59,
	pl36_super_60, pl36_super_61, pl36_super_62, pl36_super_63, pl36_super_64, pl36_super_65,
	pl36_super_66, pl36_super_67, pl36_super_68, pl36_super_69, pl36_super_70, pl36_super_10,
	pl36_super_18, 0,
};

const Sint32 *const pl36_effect_tbl[] = {
	pl36_stand_00, pl36_stand_00, pl36_stand_00, pl36_stand_00, pl36_effect_04, pl36_effect_05,
	pl36_effect_06, pl36_effect_07, 0,
};

const Sint32 *const pl36_change_tbl[] = {
	pl36_change_00, pl36_change_01, pl36_change_02, pl36_change_03, pl36_change_04, 0,
};

const Sint32 *const pl36_grp25_tbl[] = {
	pl36_stand_00, 0,
};

const Sint32 *const pl36_grp27_tbl[] = {
	pl36_grp27_00, pl36_grp27_01, pl36_grp27_02, pl36_grp27_03, pl36_grp27_04, pl36_grp27_05,
	pl36_grp27_06, pl36_grp27_07, pl36_grp27_06, pl36_grp27_09, pl36_grp27_10, pl36_grp27_11,
	pl36_grp27_12, pl36_grp27_13, pl36_grp27_14, pl36_grp27_15, pl36_grp27_16, pl36_grp27_17,
	pl36_grp27_18, pl36_grp27_19, pl36_grp27_20, pl36_grp27_21, pl36_grp27_22, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Super 14
   Char_Super 15
   Char_Super 16
   Char_Super 17 */
const Sint32 pl36_super_14[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   118, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_14 ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Effect 0
   Char_Effect 1 */
const Sint32 pl36_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    7,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    9,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   10,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl36_stand_02[] = {
	CG_FRAME(    0,    0,    8,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_stand_02 ),
};

/* Char_Stand 6 */
const Sint32 pl36_stand_06[] = {
	CG_FRAME(    0,    0,    6,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_stand_06 ),
};

/* Char_Stand 3 */
const Sint32 pl36_stand_03[] = {
	CG_FRAME(    0,    0,    8,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_stand_03 ),
};

/* Char_Stand 7 */
const Sint32 pl36_stand_07[] = {
	CG_FRAME(    0,    0,    6,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_stand_07 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2
   Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl36_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    5,           0,   160, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    5,           0,   161, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    5,           0,   162, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    5,           0,   161, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover
   Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl36_jump_09[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   160, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl36_jump_09 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl36_change_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl36_change_02 ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl36_jump_03[] = {
	CG_FRAME(    0,    0,    5,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    5,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_jump_03 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl36_dash_00[] = {
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl36_dash_00 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl36_dash_02[] = {
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl36_dash_01[] = {
	CG_FRAME(    0, 0x80,    8,           0,   164, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,  35 ),
};

const Sint32 pl36_dash_01_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl36_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl36_dash_03[] = {
	CG_FRAME(    0,    0,    4,           0,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   22,           0,   164, 0x80,   0x00,    0,  0x00, 0x34 | CSE,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl36_crouch_00[] = {
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl36_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl36_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl36_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl36_crouch_02[] = {
	CG_FRAME(    0,    0,    6,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    6,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_NEXT( pl36_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl36_turn_00[] = {
	CG_FRAME(    0,    0,    6,           0,   168, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl36_turn_01[] = {
	CG_FRAME(    0,    0,    6,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    6,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl36_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl36_guard_00[] = {
	CG_FRAME(    0,    0,    6,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl36_guard_01[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl36_guard_01 ),
};

/* Char_Guard 3 */
const Sint32 pl36_guard_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl36_guard_03 ),
};

/* Char_Guard 2 */
const Sint32 pl36_guard_02[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl36_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl36_guard_04[] = {
	CG_FRAME(    0,    0,    6,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl36_guard_05[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl36_guard_05 ),
};

/* Char_Guard 7 */
const Sint32 pl36_guard_07[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl36_guard_07 ),
};

/* Char_Guard 6 */
const Sint32 pl36_guard_06[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl36_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl36_air_guard_00[] = {
	CG_FRAME(    0,    0,    5,           0,   180, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl36_air_guard_01[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   181, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl36_air_guard_02[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   181, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_air_guard_02 ),
};

const Sint32 pl36_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    5, END_OF_DATA,   182, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	0x00000d2c,
};

/* Char_Stand_P 0 (light)
   Char_Extra 0 */
const Sint32 pl36_stand_p_00[] = {
	CG_FRAME(    0,    0,    6,           0,   238, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,    2,           0,   239, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x81,    0,    6,           0,   240, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
};

const Sint32 pl36_stand_p_00_loop[] = {
	CG_FRAME(    0, 0x80,    7, END_OF_DATA,   241, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl36_stand_p_00_loop ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl36_stand_p_01[] = {
	CG_FRAME(    0,    0,   10,           0,   234, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,    2,           0,   235, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  62 ),
	CG_FRAME( 0x81,    0,    6,           0,   236, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    1,           0,   237, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    1,    6,           0,   238, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,    2,           0,   239, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x81,    0,   13,           0,   240, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
};

const Sint32 pl36_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,    7, END_OF_DATA,   241, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl36_stand_p_01_loop ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl36_stand_p_02[] = {
	CG_FRAME(    0,    0,   10,           0,   242, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   243, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,    3,           0,   244, 0x80,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,    1,           0,   245, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,    2,           0,   246, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,    2,           0,   247, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,    2,           0,   248, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,    2,           0,   249, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,    2,           0,   250, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,    2,           0,   251, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,    6,           0,   252, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x81,    0,    6,           0,   256, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x81,    0,   12,           0,   164, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x81,    0,    4,           0,   163, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
};

const Sint32 pl36_stand_p_02_loop[] = {
	CG_FRAME( 0x81,    0,    2, END_OF_DATA,   167, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl36_stand_p_02_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl36_stand_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    8,           0,   255, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
};

const Sint32 pl36_stand_k_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   256, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_stand_k_00_loop ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl36_stand_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6,           0,   255, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,   12,           0,   255, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
};

const Sint32 pl36_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   256, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_stand_k_01_loop ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl36_stand_k_02[] = {
	CG_FRAME(    0,    0,    5,           0,   257, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   258, 0x80,   0x08,  ATK,  0x00,       0x05,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   259, 0x80,   0x01,  ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,    5,           0,   262, 0x80,   0x05,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6,           0,   263, 0x80,   0x0b,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    7,           0,   264, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
};

const Sint32 pl36_stand_k_02_loop[] = {
	CG_FRAME(    0,    0,    7, END_OF_DATA,   265, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_stand_k_02_loop ),
};

/* Char_Extra 4: snapback_init (Snapback) */
const Sint32 pl36_extra_04[] = {
	CG_FRAME(    0,    0,    8,           0,   257, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    1,    2,           0,   257, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    1,           0,   257, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   258, 0x80,   0x08,  ATK,  0x00,       0x05,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   259, 0x80,   0x01,  ATK,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,    5,           0,   262, 0x80,   0x05,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6,           0,   263, 0x80,   0x0b,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    7,           0,   264, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
};

const Sint32 pl36_extra_04_loop[] = {
	CG_FRAME(    0,    0,    7, END_OF_DATA,   265, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_extra_04_loop ),
};

/* Char_Stand_K 3: nml_atck */
const Sint32 pl36_stand_k_03[] = {
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   267, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,    5,           0,   270, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,    3,           0,   271, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,    3,           0,   272, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,    3,           0,   273, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
};

const Sint32 pl36_stand_k_03_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   265, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_stand_k_03_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl36_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   275, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    2,           0,   276, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    2,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,    6,           0,   279, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,    3,           0,   280, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 119 ),
};

const Sint32 pl36_crouch_p_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl36_crouch_p_00_loop ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl36_crouch_p_01[] = {
	CG_FRAME(    0,    0,    7,           0,   282, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,    2,           0,   283, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x80,    0,    2,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,    0,    1,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    1,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   14,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
};

const Sint32 pl36_crouch_p_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   285, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl36_crouch_p_01_loop ),
};

/* group 27 16 */
const Sint32 pl36_grp27_16[] = {
	EF_FRAME(    0,    0,    2,           0,   286 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   287 ),
	CG_NEXT( pl36_grp27_16 ),
};

/* Char_Extra 6: pl36_upper */
const Sint32 pl36_extra_06[] = {
	CG_FRAME(    0,    0,    6,           0,   282, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,    2,           0,   283, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x80,    0,    2,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0, 130 ),
	CG_FRAME( 0x80,    0,    1,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    1,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   14,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
};

const Sint32 pl36_extra_06_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   285, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl36_extra_06_loop ),
};

/* Char_Extra 5: sp_upper (Uppercut (assist)) */
const Sint32 pl36_extra_05[] = {
	CG_FRAME(    0,    0,    6,           0,   282, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,    2,           0,   283, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x80,    0,    2,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0, 130 ),
	CG_FRAME( 0x80,    0,    1,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    1,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   14,           0,   284, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  42 ),
};

const Sint32 pl36_extra_05_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   285, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl36_extra_05_loop ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl36_crouch_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   288, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    3,           0,   289, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    1,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    1,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 122 ),
	CG_FRAME(    0,    0,    7,           0,   292, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    3,           0,   293, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 119 ),
};

const Sint32 pl36_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl36_crouch_p_02_loop ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl36_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    6,           0,   295, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
};

const Sint32 pl36_crouch_k_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   296, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl36_crouch_k_00_loop ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl36_crouch_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   126, 0x80,   0x00,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    6,           0,   295, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
};

const Sint32 pl36_crouch_k_01_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   296, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl36_crouch_k_01_loop ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl36_crouch_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   297, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    5,           0,   298, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    2,           0,   299, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    2,           0,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    0,    0,    5,           0,   300, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,    5,           0,   303, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    5,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 119 ),
};

const Sint32 pl36_crouch_k_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   164, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl36_crouch_k_02_loop ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl36_jump_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x40,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   10,           0,   305, 0x80,   0x40,  ATK,  0x00,       0x04,    0,   0,     0,      0,  72 ),
};

const Sint32 pl36_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   306, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl36_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl36_jump_p_01[] = {
	CG_FRAME(    0,    0,    6,           0,   160, 0x80,   0x40,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    8,           0,   305, 0x80,   0x40,  ATK,  0x00,       0x05,    0,   0,     0,      0,  72 ),
};

const Sint32 pl36_jump_p_01_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   306, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl36_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl36_jump_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   307, 0x80,   0x40,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    3,           0,   307, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    1,           0,   308, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    1,           0,   309, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    1,           0,   310, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    1,           0,   311, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    1,           0,   312, 0x80,   0x40,  ATK,  0x00,       0x20,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    2,           0,   313, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   314, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   315, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    2,           0,   316, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2,           0,   317, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   16,           0,   162, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   306, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl36_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   160, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl36_jump_p_02_loop ),
};

/* Char_Extra 1 */
const Sint32 pl36_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0,   318, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   319, 0x80,   0xff,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl36_extra_01 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl36_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x40,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    8,           0,   321, 0x80,   0x40,  ATK,  0x00,       0x03,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
};

const Sint32 pl36_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   322, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl36_jump_k_00_loop ),
};

/* Char_Extra 3: t_kick_init, fly_start5 */
const Sint32 pl36_extra_03[] = {
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x40,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    1,    1,           0,   321, 0x80,   0x40,  ATK,  0x00,       0x05,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    2,           0,   164, 0x80,   0x00,  ATE,  0x00,       0x0a,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   163, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   196, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8,           0,   198, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   199, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   202, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   203, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   202, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   199, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   200, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   201, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
};

const Sint32 pl36_extra_03_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   198, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_extra_03_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl36_jump_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   320, 0x80,   0x40,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    8,           0,   323, 0x80,   0x40,  ATK,  0x00,       0x04,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    8,           0,   323, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
};

const Sint32 pl36_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   306, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl36_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl36_jump_k_02[] = {
	CG_FRAME(    0,    0,    9,           0,   320, 0x80,   0x40,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x40,  ATK,  0x00,       0x05,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   14,           0,   321, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
};

const Sint32 pl36_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   322, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl36_jump_k_02_loop ),
};

/* Char_Jump_K 5: jump_atck
   Char_Extra 2: fly_start5 */
const Sint32 pl36_jump_k_05[] = {
	CG_FRAME(    0,    0,    8,           0,   365, 0x80,   0x40,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,    2,           0,   371, 0x80,   0x40,  ATK,  0x00,       0x05,    0,   0,     0,      0,  15 ),
};

const Sint32 pl36_jump_k_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   366, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   374, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   375, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   376, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   377, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   378, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   380, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   371, 0x80,   0x40,  ATK,  0x00, 0x00 | CSE,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl36_jump_k_05_loop ),
};

/* Char_Jump_K 6 */
const Sint32 pl36_jump_k_06[] = {
	CG_FRAME(    0,    0,    5,           0,   162, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl36_jump_k_06_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   160, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl36_jump_k_06_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl36_get_up_00[] = {
	CG_FRAME(    0,    0,    3,           0,   430, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    0,    3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,    3,           0,   185, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_get_up_00_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_get_up_00_loop ),
};

/* Char_Get_Up 1 */
const Sint32 pl36_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   430, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    2,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    0,    2,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,    2,           0,   185, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_get_up_01_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_get_up_01_loop ),
};

/* Char_Get_Up 2 */
const Sint32 pl36_get_up_02[] = {
	CG_FRAME(    0,    0,   12,           0,   430, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    0,    3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,    3,           0,   185, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_get_up_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_get_up_02_loop ),
};

/* Char_Get_Up 3 */
const Sint32 pl36_get_up_03[] = {
	CG_FRAME(    0,    0,    3,           0,   430, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   183, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    0,    3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,    3,           0,   185, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_get_up_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_get_up_03_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl36_damage_00[] = {
	CG_FRAME(    0,    0,    7,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl36_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl36_damage_01[] = {
	CG_FRAME(    0,    0,    8,           0,   418, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl36_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl36_damage_02[] = {
	CG_FRAME( 0x80,    0,    2,           0,   419, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    2,           0,   420, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   418, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl36_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl36_damage_03[] = {
	CG_FRAME(    0,    0,    6,           0,   421, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl36_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl36_damage_04[] = {
	CG_FRAME(    0,    0,    6,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   421, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl36_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl36_damage_05[] = {
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   424, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   421, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl36_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl36_damage_06[] = {
	CG_FRAME(    0,    0,    7,           0,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl36_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl36_damage_07[] = {
	CG_FRAME(    0,    0,    6,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    8,           0,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl36_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl36_damage_08[] = {
	CG_FRAME(    0,    0,    4,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   428, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    6,           0,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl36_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl36_damage_10[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   421, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl36_damage_11[] = {
	CG_FRAME(    0,    0,    6,           0,   422, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   421, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl36_damage_12[] = {
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   424, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   422, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   421, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_10 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl36_damage_13[] = {
	CG_FRAME(    0,    0,    2,           0,   444, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    3,           0,   429, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   430, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl36_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl36_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   434, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   435, 0x80,   0x8c,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0xac,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_damage_09_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   444, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl36_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl36_damage_16[] = {
	CG_FRAME(    0,    0,    4,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl36_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl36_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl36_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl36_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl36_damage_21[] = {
	CG_FRAME(    0,    0,    4,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
};

/* Char_Damage 22 */
const Sint32 pl36_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

/* Char_Damage 23 */
const Sint32 pl36_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

/* Char_Damage 24 */
const Sint32 pl36_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
};

/* Char_Damage 25 */
const Sint32 pl36_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl36_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl36_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   442, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   443, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
};

const Sint32 pl36_damage_26_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   444, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl36_damage_26_loop ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl36_damage_34[] = {
	CG_FRAME(    0,    0,    6,           0,   435, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   430, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl36_damage_34 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl36_damage_31[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   444, 0x80,   0xcc,    0,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl36_damage_31 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl36_damage_28[] = {
	CG_FRAME(    0,    0,    4,           0,   445, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   447, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   448, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   449, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   450, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl36_damage_29[] = {
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   453, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   454, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   455, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   456, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl36_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl36_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   461, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl36_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl36_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   462, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl36_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl36_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl36_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl36_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   464, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl36_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl36_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl36_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl36_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl36_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl36_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl36_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl36_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl36_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl36_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl36_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl36_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   470, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl36_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl36_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   471, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl36_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl36_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   472, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl36_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl36_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   473, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl36_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl36_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   474, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl36_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl36_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl36_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl36_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl36_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl36_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   477, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl36_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl36_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   478, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl36_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl36_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   479, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl36_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl36_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   480, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl36_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl36_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   481, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_NEXT( pl36_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl36_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   482, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl36_caught_21 ),
};

/* Char_Dizzy 0: shl36_03_catch, act_dizzy */
const Sint32 pl36_dizzy_00[] = {
	CG_FRAME(    0,    0,    8,           0,   457, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    8,           0,   458, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    8,           0,   459, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   460, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl36_dizzy_00 ),
};

/* Char_Catch 0 */
const Sint32 pl36_catch_00[] = {
	CG_FRAME(    0,    0,    1,           0,   326, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    1,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    1,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    1,           0,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    5,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,    5,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    5,           0,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    5,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,    5,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    5,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,    4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,    4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,    4,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,    6,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,    6,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,    2,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,    2,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,    2,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0022,   0 ),
	CG_FRAME(    0,    1,    1,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0023,   0 ),
	CG_FRAME(    0,    0,    3,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_catch_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_catch_00_loop ),
};

/* Char_Catch 1 */
const Sint32 pl36_catch_01[] = {
	CG_FRAME(    0,    0,    4,           0,   355, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    4,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    4,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    4,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    4,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    4,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    4,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    1,    1,           0,   359, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,   23,           0,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_catch_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_catch_01_loop ),
};

/* group 27 14 */
const Sint32 pl36_grp27_14[] = {
	EF_FRAME(    0, 0x80,    4,           0,   363 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   364 ),
	CG_NEXT( pl36_grp27_14 ),
};

/* Char_Win 0: sp_chouhatsu (Taunt) */
const Sint32 pl36_win_00[] = {
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   230, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   231, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    1,    6,           0,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
};

const Sint32 pl36_win_00_loop[] = {
	CG_FRAME(    5,    0,    2,           0,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    4,    0,    2, END_OF_DATA,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl36_win_01[] = {
	CG_FRAME(    0,    0,    1,           0,   213, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_win_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   213, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_win_01_loop ),
};

/* group 27 13 */
const Sint32 pl36_grp27_13[] = {
	EF_FRAME(    1,    0,    4,           0,   215 ),
	EF_FRAME(    0,    0,    4,           0,   226 ),
	EF_FRAME(    0,    0,    4,           0,   227 ),
	EF_FRAME(    0,    0,    4,           0,   228 ),
	EF_FRAME(    0,    0,    4,           0,   229 ),
	EF_FRAME(    0,    0,    4,           0,   216 ),
	EF_FRAME(    0,    0,    4,           0,   217 ),
	EF_FRAME(    0,    0,    4,           0,   218 ),
	EF_FRAME(    1,    0,    2,           0,   219 ),
	EF_FRAME(    2,    0,    2,           0,   219 ),
	EF_FRAME(    3,    0,    2,           0,   220 ),
	EF_FRAME(    4,    0,    2,           0,   221 ),
	EF_FRAME(    5,    0,    2,           0,   221 ),
	EF_FRAME(    0,    0,    4,           0,   222 ),
	EF_FRAME(    0,    0,    4,           0,   223 ),
	EF_FRAME(    0,    0,    4,           0,   224 ),
};

const Sint32 pl36_grp27_13_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   225 ),
	CG_NEXT( pl36_grp27_13_loop ),
};

/* Char_Win 2: lose_time_ko */
const Sint32 pl36_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_win_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_win_02_loop ),
};

/* Char_Appear 0: pl36_appear */
const Sint32 pl36_appear_00[] = {
	CG_FRAME(    0,    0,    8,           0,   213, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   60, END_OF_DATA,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_stand_00 ),
};

/* Char_Special 0 */
const Sint32 pl36_special_00[] = {
	CG_FRAME(    0, 0x80,    8,           0,   395, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   413, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   414, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   415, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl36_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl36_special_01[] = {
	CG_FRAME(    0, 0x80,   10,           0,   395, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   407, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   413, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   414, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   415, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    0,        0x81,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl36_special_01 ),
};

/* Char_Special 2 */
const Sint32 pl36_special_02[] = {
	CG_FRAME(    0, 0x80,   12,           0,   395, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    2,           0,   402, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   405, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   407, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    1,           0,   413, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   414, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   415, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    0,        0x81,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl36_special_02 ),
};

/* Char_Special 3: titan_move (Titan Crush, 3 + atck_level) */
const Sint32 pl36_special_03[] = {
	CG_FRAME(    0,    0,   12,           0,   164, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl36_special_03 ),
};

/* Char_Special 4 */
const Sint32 pl36_special_04[] = {
	CG_FRAME(    0,    0,   16,           0,   164, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl36_special_04 ),
};

/* Char_Special 5 */
const Sint32 pl36_special_05[] = {
	CG_FRAME(    0,    0,   20,           0,   164, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl36_special_05 ),
};

/* Char_Special 6: sp_sphere (Death Sphere, 6 + atck_level) */
const Sint32 pl36_special_06[] = {
	CG_FRAME(    3,    0,    2,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    2,    0,    2,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    2,    0,    2,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    2,    0,    2,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    2,    0,    2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    2,    0,    2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    2,    0,    2,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    0,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    0,    3,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    1,    6,           0,   392, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   18,           0,   393, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    4,    0,    6, END_OF_DATA,   394, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl36_special_06 ),
};

/* Char_Special 7 */
const Sint32 pl36_special_07[] = {
	CG_FRAME(    3,    0,    2,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    2,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    2,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    2,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    0,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    0,    3,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    1,    6,           0,   392, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   24,           0,   393, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    4,    0,    6, END_OF_DATA,   394, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl36_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl36_special_08[] = {
	CG_FRAME(    3,    0,    2,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    3,    0,    3,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    0,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    0,    3,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    1,    6,           0,   392, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   28,           0,   393, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    4,    0,    6, END_OF_DATA,   394, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl36_special_08 ),
};

/* Char_Effect 4: shl36_03_fly */
const Sint32 pl36_effect_04[] = {
	CG_FRAME(    0,    0,    1,           0, 32818, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32819, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_effect_04 ),
};

/* Char_Effect 5: shl36_03_init */
const Sint32 pl36_effect_05[] = {
	CG_FRAME(    0,    0,    1,           0, 32820, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32821, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl36_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl36_effect_06[] = {
	CG_FRAME(    0,    0,    1,           0, 32822, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32823, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_effect_06 ),
};

/* Char_Effect 7: pls36_02_init0 */
const Sint32 pl36_effect_07[] = {
	CG_FRAME(    0,    0,    1,           0, 32824, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32825, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_effect_07 ),
};

/* Char_Special 9: reality_init (Gauntlet Reality (hyper)) */
const Sint32 pl36_special_09[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl36_special_09_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl36_special_09_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl36_change_01[] = {
	CG_FRAME( 0x80,    0,    4,           0,   265, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    4,           0,   193, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,   194, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_change_01 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl36_change_00[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0xc0,   0x00,    0,  0x00,       0x05,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_change_00_loop[] = {
	CG_FRAME(    0,    1,   16, END_OF_DATA,   214, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_change_00_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl36_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   204, 0xc0,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8,           0,   206, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   207, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   206, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   207, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8,           0,   206, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
};

const Sint32 pl36_change_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   204, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl36_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     0,      0,  57 ),
};

const Sint32 pl36_change_04_loop[] = {
	CG_FRAME(    0,    0,   36, END_OF_DATA,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl36_change_04_loop ),
};

/* Char_Special 10: space_init (Gauntlet Space (hyper)) */
const Sint32 pl36_special_10[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    9,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl36_special_10_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl36_special_10_loop ),
};

/* Char_Special 11: power_init (Power Gem (hyper)) */
const Sint32 pl36_special_11[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl36_special_11_loop[] = {
	CG_FRAME(    3,    1,    1,           0,   385, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    3,    1,    1,           0,   386, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    3,    1,    1,           0,   387, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    3,    1,    1,           0,   387, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    3,    1,    1,           0,   388, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    3,    1,    1,           0,   388, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    3,    1,    1, END_OF_DATA,   389, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl36_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl36_special_12[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl36_special_12_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl36_special_12_loop ),
};

/* Char_Special 13: soul_init (Soul Gem (hyper)) */
const Sint32 pl36_special_13[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl36_special_13_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl36_special_13_loop ),
};

/* Char_Special 14 */
const Sint32 pl36_special_14[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,    1,           0,   214, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl36_special_14_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   214, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl36_special_14_loop ),
};

/* Char_Super 0: shl36_01_init */
const Sint32 pl36_super_00[] = {
	CG_FRAME(    0,    0,    4,           0, 32826, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0, 32827, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    1,    2,           0, 32828, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    1,    2,           0, 32829, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    1,    2,           0, 32830, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    1,    2,           0, 32831, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    1,    2,           0, 32832, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,    2,           0, 32833, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
};

const Sint32 pl36_super_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32834, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32835, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl36_super_00_loop ),
};

/* Char_Super 1: shl36_01_rush */
const Sint32 pl36_super_01[] = {
	CG_FRAME(    0,    0,    4,           0, 32836, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32837, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl36_super_01 ),
};

/* Char_Super 2: shl36_01_rush_near */
const Sint32 pl36_super_02[] = {
	CG_FRAME(    0,    0,    4,           0, 32838, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32839, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl36_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl36_super_03[] = {
	CG_FRAME(    0,    0,    4,           0, 32826, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0, 32827, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    2,           0, 32828, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    2,           0, 32829, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,    2,           0, 32830, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    1,    2,           0, 32831, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    1,    2,           0, 32832, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,    2,           0, 32833, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
};

const Sint32 pl36_super_03_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32834, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32835, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl36_super_03_loop ),
};

/* Char_Super 4 */
const Sint32 pl36_super_04[] = {
	CG_FRAME(    0,    0,    4,           0, 32836, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32837, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl36_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl36_super_05[] = {
	CG_FRAME(    0,    0,    4,           0, 32838, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32839, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl36_super_05 ),
};

/* Char_Super 6 */
const Sint32 pl36_super_06[] = {
	CG_FRAME(    0,    0,    1,           0, 32833, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32832, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32831, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32830, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32829, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32828, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32827, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32826, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_06 ),
};

/* Char_Super 7 */
const Sint32 pl36_super_07[] = {
	CG_FRAME(    0,    0,    1,           0, 32833, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32832, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32831, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32830, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32829, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32828, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32827, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32826, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_07 ),
};

/* Char_Super 8 */
const Sint32 pl36_super_08[] = {
	CG_FRAME(    0,    0,    3,           0,   533, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   534, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   535, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_08_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   536, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   537, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   538, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_08_loop ),
};

/* Char_Super 9 */
const Sint32 pl36_super_09[] = {
	CG_FRAME(    0,    0,   12,           0,    -1, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   539, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   540, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   541, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_09_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   542, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_09_loop ),
};

/* Char_Super 18: pls36_01_move0
   Char_Super 72: pls36_01_move1 */
const Sint32 pl36_super_18[] = {
	CG_FRAME(    0,    0,    4,           0,   534, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_18_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   533, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_18_loop ),
};

/* Char_Super 19: pls36_01_move0 */
const Sint32 pl36_super_19[] = {
	CG_FRAME(    0,    0,    4,           0,   541, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   540, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_19_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   539, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_19_loop ),
};

/* Char_Super 10
   Char_Super 11
   Char_Super 71: pls36_01_init1 */
const Sint32 pl36_super_10[] = {
	CG_FRAME(    0,    0,    3,           0,   533, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   534, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_10_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   554, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   555, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_10_loop ),
};

/* Char_Super 12: power_start (Power Gem (hyper)) */
const Sint32 pl36_super_12[] = {
	CG_FRAME(    1,    2,    3,           0,   390, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    3,    2,           0,   391, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0, 0x80,    6,           0,   392, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   48,           0,   393, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    4,    0,   12, END_OF_DATA,   394, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl36_super_12 ),
};

/* Char_Super 13: shl36_00_head_init */
const Sint32 pl36_super_13[] = {
	CG_FRAME(    0,    0,    3,           0, 32817, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA, 32817, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl36_super_13 ),
};

/* Char_Super 20: shl36_04_root_set */
const Sint32 pl36_super_20[] = {
	CG_FRAME(    0,    0,    4,           0, 32840, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32841, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32842, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32843, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32844, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_20 ),
};

/* Char_Catch 2
   Char_Super 21: shl36_04_fly */
const Sint32 pl36_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0, 32845, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001f,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32846, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0020,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32847, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0021,   0 ),
	CG_NEXT( pl36_catch_02 ),
};

/* Char_Super 22: shl36_04_flash_set */
const Sint32 pl36_super_22[] = {
	CG_FRAME(    0,    0,    4,           0, 32883, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32884, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32885, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_22 ),
};

/* Char_Super 23 */
const Sint32 pl36_super_23[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32849, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl36_super_23 ),
};

/* Char_Super 24: shl36_04_init, shl36_04_hold */
const Sint32 pl36_super_24[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32848, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl36_super_24 ),
};

/* Char_Super 25 */
const Sint32 pl36_super_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32850, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl36_super_25 ),
};

/* Char_Super 26
   Char_Super 27
   Char_Super 28
   Char_Super 29
   Char_Super 30
   Char_Super 31 */
const Sint32 pl36_super_26[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32870, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_26 ),
};

/* group 27 15: shl36_04_part_set */
const Sint32 pl36_grp27_15[] = {
	EF_FRAME(    0,    0,    2,           0, 32855 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32871 ),
	CG_NEXT( pl36_grp27_15 ),
};

/* group 27 0 */
const Sint32 pl36_grp27_00[] = {
	EF_FRAME(    0,    0,    2,           0, 32797 ),
	EF_FRAME(    0,    0,    2,           0, 32798 ),
	EF_FRAME(    0,    0,    2,           0, 32799 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32800 ),
	CG_NEXT( pl36_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl36_grp27_01[] = {
	EF_FRAME(    0,    0,    2,           0, 32801 ),
	EF_FRAME(    0,    0,    2,           0, 32802 ),
	EF_FRAME(    0,    0,    2,           0, 32803 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32804 ),
	CG_NEXT( pl36_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl36_grp27_02[] = {
	EF_FRAME(    0,    0,    2,           0, 32805 ),
	EF_FRAME(    0,    0,    2,           0, 32806 ),
	EF_FRAME(    0,    0,    2,           0, 32807 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32808 ),
	CG_NEXT( pl36_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl36_grp27_03[] = {
	EF_FRAME(    0,    0,    2,           0, 32809 ),
	EF_FRAME(    0,    0,    2,           0, 32810 ),
	EF_FRAME(    0,    0,    2,           0, 32811 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32812 ),
	CG_NEXT( pl36_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl36_grp27_04[] = {
	EF_FRAME(    0,    0,    2,           0, 32813 ),
	EF_FRAME(    0,    0,    2,           0, 32814 ),
	EF_FRAME(    0,    0,    2,           0, 32815 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32816 ),
	CG_NEXT( pl36_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl36_grp27_05[] = {
	EF_FRAME(    0,    0,    4,           0, 32780 ),
	EF_FRAME(    0,    0,    4,           0, 32781 ),
	EF_FRAME(    0,    0,    4,           0, 32782 ),
	EF_FRAME(    0,    0,    4,           0, 32783 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32784 ),
	CG_NEXT( pl36_grp27_05 ),
};

/* group 27 6
   group 27 8 */
const Sint32 pl36_grp27_06[] = {
	EF_FRAME(    0,    0,    4,           0, 32785 ),
	EF_FRAME(    0,    0,    4,           0, 32786 ),
	EF_FRAME(    0,    0,    4,           0, 32787 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32788 ),
	CG_NEXT( pl36_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl36_grp27_07[] = {
	EF_FRAME(    0,    0,    4,           0, 32789 ),
	EF_FRAME(    0,    0,    4,           0, 32790 ),
	EF_FRAME(    0,    0,    4,           0, 32791 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32792 ),
	CG_NEXT( pl36_grp27_07 ),
};

/* group 27 9 */
const Sint32 pl36_grp27_09[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32776 ),
	CG_NEXT( pl36_grp27_09 ),
};

/* group 27 10 */
const Sint32 pl36_grp27_10[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32777 ),
	CG_NEXT( pl36_grp27_10 ),
};

/* group 27 11 */
const Sint32 pl36_grp27_11[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32778 ),
	CG_NEXT( pl36_grp27_11 ),
};

/* group 27 12 */
const Sint32 pl36_grp27_12[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32779 ),
	CG_NEXT( pl36_grp27_12 ),
};

/* Char_Super 58: shl36_02_init_2 */
const Sint32 pl36_super_58[] = {
	CG_FRAME(    0,    1,    4,           0, 32793, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32796, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_58 ),
};

/* Char_Super 59 */
const Sint32 pl36_super_59[] = {
	CG_FRAME(    0,    0,    2,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_59_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl36_super_59_loop ),
};

/* Char_Super 60 */
const Sint32 pl36_super_60[] = {
	CG_FRAME(    0,    0,    2,           0, 32769, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_60_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32769, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl36_super_60_loop ),
};

/* Char_Super 61 */
const Sint32 pl36_super_61[] = {
	CG_FRAME(    0,    0,    2,           0, 32770, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_61_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32770, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl36_super_61_loop ),
};

/* Char_Super 62 */
const Sint32 pl36_super_62[] = {
	CG_FRAME(    0,    0,    2,           0, 32771, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl36_super_62_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32771, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl36_super_62_loop ),
};

/* Char_Super 63 */
const Sint32 pl36_super_63[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32772, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_63 ),
};

/* Char_Super 64 */
const Sint32 pl36_super_64[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32773, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_64 ),
};

/* Char_Super 65 */
const Sint32 pl36_super_65[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32774, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_65 ),
};

/* Char_Super 67 */
const Sint32 pl36_super_67[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32775, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl36_super_67 ),
};

/* Char_Super 66 */
const Sint32 pl36_super_66[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32776, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_66 ),
};

/* Char_Super 68 */
const Sint32 pl36_super_68[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32777, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_68 ),
};

/* Char_Super 69 */
const Sint32 pl36_super_69[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32778, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_69 ),
};

/* Char_Super 70 */
const Sint32 pl36_super_70[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32779, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl36_super_70 ),
};

/* group 27 17 */
const Sint32 pl36_grp27_17[] = {
	EF_FRAME(    0,    0,    4,           0,   584 ),
	EF_FRAME(    0,    0,    4,           0,   585 ),
	EF_FRAME(    0,    0,    4,           0,   586 ),
	EF_FRAME(    0,    0,    4,           0,   587 ),
	EF_FRAME(    0,    0,    4,           0,   588 ),
	EF_FRAME(    0,    0,    4,           0,   589 ),
	EF_FRAME(    0,    0,    4,           0,   590 ),
	EF_FRAME(    0,    0,    4,           0,   591 ),
	EF_FRAME(    0,    0,    4,           0,   592 ),
	EF_FRAME(    0,    0,    4,           0,   593 ),
	EF_FRAME(    0,    0,    4,           0,   594 ),
	EF_FRAME(    0,    0,    4,           0,   595 ),
	EF_FRAME(    0,    0,    4,           0,   596 ),
	EF_FRAME(    0,    0,    4,           0,   597 ),
	EF_FRAME(    0,    0,    4,           0,   598 ),
	EF_FRAME(    0,    0,    4,           0,   599 ),
	EF_FRAME(    0,    0,    4,           0,   600 ),
	EF_FRAME(    0,    0,    4,           0,   601 ),
	EF_FRAME(    0,    0,    4,           0,   602 ),
	EF_FRAME(    0,    0,    4,           0,   603 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   604 ),
	CG_NEXT( pl36_grp27_17 ),
};

/* group 27 18 */
const Sint32 pl36_grp27_18[] = {
	EF_FRAME(    0,    0,    4,           0,   605 ),
	EF_FRAME(    0,    0,    4,           0,   606 ),
	EF_FRAME(    0,    0,    4,           0,   607 ),
	EF_FRAME(    0,    0,    4,           0,   608 ),
	EF_FRAME(    0,    0,    4,           0,   609 ),
	EF_FRAME(    0,    0,    4,           0,   610 ),
	EF_FRAME(    0,    0,    4,           0,   611 ),
	EF_FRAME(    0,    0,    4,           0,   612 ),
	EF_FRAME(    0,    0,    4,           0,   613 ),
	EF_FRAME(    0,    0,    4,           0,   614 ),
	EF_FRAME(    0,    0,    4,           0,   615 ),
	EF_FRAME(    0,    0,    4,           0,   616 ),
	EF_FRAME(    0,    0,    4,           0,   617 ),
	EF_FRAME(    0,    0,    4,           0,   618 ),
	EF_FRAME(    0,    0,    4,           0,   619 ),
	EF_FRAME(    0,    0,    4,           0,   620 ),
	EF_FRAME(    0,    0,    4,           0,   621 ),
	EF_FRAME(    0,    0,    4,           0,   622 ),
	EF_FRAME(    0,    0,    4,           0,   623 ),
	EF_FRAME(    0,    0,    4,           0,   624 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   625 ),
	CG_NEXT( pl36_grp27_18 ),
};

/* group 27 19 */
const Sint32 pl36_grp27_19[] = {
	EF_FRAME(    0,    0,    4,           0,   626 ),
	EF_FRAME(    0,    0,    4,           0,   627 ),
	EF_FRAME(    0,    0,    4,           0,   628 ),
	EF_FRAME(    0,    0,    4,           0,   629 ),
	EF_FRAME(    0,    0,    4,           0,   630 ),
	EF_FRAME(    0,    0,    4,           0,   631 ),
	EF_FRAME(    0,    0,    4,           0,   632 ),
	EF_FRAME(    0,    0,    4,           0,   633 ),
	EF_FRAME(    0,    0,    4,           0,   634 ),
	EF_FRAME(    0,    0,    4,           0,   635 ),
	EF_FRAME(    0,    0,    4,           0,   636 ),
	EF_FRAME(    0,    0,    4,           0,   637 ),
	EF_FRAME(    0,    0,    4,           0,   638 ),
	EF_FRAME(    0,    0,    4,           0,   639 ),
	EF_FRAME(    0,    0,    4,           0,   640 ),
	EF_FRAME(    0,    0,    4,           0,   641 ),
	EF_FRAME(    0,    0,    4,           0,   642 ),
	EF_FRAME(    0,    0,    4,           0,   643 ),
	EF_FRAME(    0,    0,    4,           0,   644 ),
	EF_FRAME(    0,    0,    4,           0,   645 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   646 ),
	CG_NEXT( pl36_grp27_19 ),
};

/* group 27 20 */
const Sint32 pl36_grp27_20[] = {
	EF_FRAME(    0,    0,    4,           0,   647 ),
	EF_FRAME(    0,    0,    4,           0,   648 ),
	EF_FRAME(    0,    0,    4,           0,   649 ),
	EF_FRAME(    0,    0,    4,           0,   650 ),
	EF_FRAME(    0,    0,    4,           0,   651 ),
	EF_FRAME(    0,    0,    4,           0,   652 ),
	EF_FRAME(    0,    0,    4,           0,   653 ),
	EF_FRAME(    0,    0,    4,           0,   654 ),
	EF_FRAME(    0,    0,    4,           0,   655 ),
	EF_FRAME(    0,    0,    4,           0,   656 ),
	EF_FRAME(    0,    0,    4,           0,   657 ),
	EF_FRAME(    0,    0,    4,           0,   658 ),
	EF_FRAME(    0,    0,    4,           0,   659 ),
	EF_FRAME(    0,    0,    4,           0,   660 ),
	EF_FRAME(    0,    0,    4,           0,   661 ),
	EF_FRAME(    0,    0,    4,           0,   662 ),
	EF_FRAME(    0,    0,    4,           0,   663 ),
	EF_FRAME(    0,    0,    4,           0,   664 ),
	EF_FRAME(    0,    0,    4,           0,   665 ),
	EF_FRAME(    0,    0,    4,           0,   666 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   667 ),
	CG_NEXT( pl36_grp27_20 ),
};

/* group 27 21 */
const Sint32 pl36_grp27_21[] = {
	EF_FRAME(    0,    0,    4,           0,   668 ),
	EF_FRAME(    0,    0,    4,           0,   669 ),
	EF_FRAME(    0,    0,    4,           0,   670 ),
	EF_FRAME(    0,    0,    4,           0,   671 ),
	EF_FRAME(    0,    0,    4,           0,   672 ),
	EF_FRAME(    0,    0,    4,           0,   673 ),
	EF_FRAME(    0,    0,    4,           0,   674 ),
	EF_FRAME(    0,    0,    4,           0,   675 ),
	EF_FRAME(    0,    0,    4,           0,   676 ),
	EF_FRAME(    0,    0,    4,           0,   677 ),
	EF_FRAME(    0,    0,    4,           0,   678 ),
	EF_FRAME(    0,    0,    4,           0,   679 ),
	EF_FRAME(    0,    0,    4,           0,   680 ),
	EF_FRAME(    0,    0,    4,           0,   681 ),
	EF_FRAME(    0,    0,    4,           0,   682 ),
	EF_FRAME(    0,    0,    4,           0,   683 ),
	EF_FRAME(    0,    0,    4,           0,   684 ),
	EF_FRAME(    0,    0,    4,           0,   685 ),
	EF_FRAME(    0,    0,    4,           0,   686 ),
	EF_FRAME(    0,    0,    4,           0,   687 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   688 ),
	CG_NEXT( pl36_grp27_21 ),
};

/* group 27 22 */
const Sint32 pl36_grp27_22[] = {
	EF_FRAME(    0,    0,    4,           0,   689 ),
	EF_FRAME(    0,    0,    4,           0,   690 ),
	EF_FRAME(    0,    0,    4,           0,   691 ),
	EF_FRAME(    0,    0,    4,           0,   692 ),
	EF_FRAME(    0,    0,    4,           0,   693 ),
	EF_FRAME(    0,    0,    4,           0,   694 ),
	EF_FRAME(    0,    0,    4,           0,   695 ),
	EF_FRAME(    0,    0,    4,           0,   696 ),
	EF_FRAME(    0,    0,    4,           0,   697 ),
	EF_FRAME(    0,    0,    4,           0,   698 ),
	EF_FRAME(    0,    0,    4,           0,   699 ),
	EF_FRAME(    0,    0,    4,           0,   700 ),
	EF_FRAME(    0,    0,    4,           0,   701 ),
	EF_FRAME(    0,    0,    4,           0,   702 ),
	EF_FRAME(    0,    0,    4,           0,   703 ),
	EF_FRAME(    0,    0,    4,           0,   704 ),
	EF_FRAME(    0,    0,    4,           0,   705 ),
	EF_FRAME(    0,    0,    4,           0,   706 ),
	EF_FRAME(    0,    0,    4,           0,   707 ),
	EF_FRAME(    0,    0,    4,           0,   708 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   709 ),
	CG_NEXT( pl36_grp27_22 ),
};
