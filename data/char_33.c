/* Iron Man (character 33): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_33      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_33     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl33.c) says which group
                   and number each move uses.
   pl33_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl33_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl33_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl33_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_33
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_33, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_33 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl33_stand_00[], pl33_stand_02[], pl33_stand_03[], pl33_dash_00[],
	pl33_dash_00_loop[], pl33_dash_02[], pl33_dash_01[], pl33_dash_01_loop[], pl33_dash_03[],
	pl33_jump_00[], pl33_jump_00_loop[], pl33_jump_06[], pl33_jump_06_loop[], pl33_jump_01[],
	pl33_jump_01_loop[], pl33_jump_07[], pl33_jump_07_loop[], pl33_jump_02[], pl33_jump_02_loop[],
	pl33_jump_08[], pl33_jump_08_loop[], pl33_jump_03[], pl33_jump_09[], pl33_crouch_00[],
	pl33_crouch_01[], pl33_crouch_02[], pl33_turn_00[], pl33_turn_01[], pl33_guard_00[],
	pl33_guard_01[], pl33_guard_02[], pl33_guard_04[], pl33_guard_05[], pl33_guard_06[],
	pl33_air_guard_00[], pl33_air_guard_01[], pl33_win_00[], pl33_win_00_loop[], pl33_win_01[],
	pl33_win_01_loop[], pl33_win_02[], pl33_win_02_loop[], pl33_appear_00[], pl33_appear_01[],
	pl33_appear_02[], pl33_appear_03[], pl33_appear_04[], pl33_appear_05[], pl33_appear_06[],
	pl33_appear_07[], pl33_appear_08[], pl33_appear_09[], pl33_get_up_00[], pl33_get_up_01[],
	pl33_get_up_02[], pl33_get_up_03[], pl33_stand_06[], pl33_stand_07[], pl33_stand_p_00[],
	pl33_extra_07[], pl33_stand_p_01[], pl33_stand_p_02[], pl33_stand_p_02_loop[], pl33_extra_00[],
	pl33_stand_k_00[], pl33_stand_k_01[], pl33_stand_k_02[], pl33_extra_06[], pl33_crouch_p_00[],
	pl33_crouch_p_01[], pl33_crouch_p_02[], pl33_crouch_k_00[], pl33_extra_08[], pl33_crouch_k_01[],
	pl33_crouch_k_02[], pl33_jump_p_00[], pl33_jump_p_01[], pl33_jump_p_02[], pl33_jump_p_06[],
	pl33_jump_p_07[], pl33_jump_k_00[], pl33_jump_k_01[], pl33_extra_04[], pl33_extra_04_loop[],
	pl33_extra_05[], pl33_extra_05_loop[], pl33_jump_k_06[], pl33_jump_k_02[], pl33_special_10[],
	pl33_special_11[], pl33_special_00[], pl33_special_00_loop[], pl33_special_01[],
	pl33_special_01_loop[], pl33_special_02[], pl33_special_03[], pl33_special_04[],
	pl33_special_04_loop[], pl33_special_05[], pl33_special_05_loop[], pl33_special_06[],
	pl33_special_07[], pl33_special_08[], pl33_special_09[], pl33_catch_00[], pl33_catch_00_loop[],
	pl33_catch_01[], pl33_catch_01_loop[], pl33_catch_02[], pl33_catch_03[], pl33_catch_04[],
	pl33_catch_04_loop[], pl33_effect_24[], pl33_effect_24_loop[], pl33_effect_25[], pl33_super_00[],
	pl33_super_00_loop[], pl33_super_01[], pl33_super_01_loop[], pl33_super_02[], pl33_unused_00[],
	pl33_unused_01[], pl33_unused_02[], pl33_unused_03[], pl33_unused_04[], pl33_unused_05[],
	pl33_unused_06[], pl33_unused_07[], pl33_unused_08[], pl33_unused_09[], pl33_unused_10[],
	pl33_unused_11[], pl33_unused_12[], pl33_unused_13[], pl33_unused_14[], pl33_unused_15[],
	pl33_unused_16[], pl33_unused_17[], pl33_unused_18[], pl33_unused_19[], pl33_unused_20[],
	pl33_unused_21[], pl33_unused_22[], pl33_unused_23[], pl33_unused_24[], pl33_unused_25[],
	pl33_unused_26[], pl33_unused_27[], pl33_unused_28[], pl33_unused_29[], pl33_unused_30[],
	pl33_unused_31[], pl33_unused_32[], pl33_unused_33[], pl33_unused_34[], pl33_unused_35[],
	pl33_unused_36[], pl33_unused_37[], pl33_unused_38[], pl33_unused_39[], pl33_unused_40[],
	pl33_unused_41[], pl33_unused_42[], pl33_unused_43[], pl33_unused_44[], pl33_unused_45[],
	pl33_unused_46[], pl33_unused_47[], pl33_unused_48[], pl33_unused_49[], pl33_unused_50[],
	pl33_unused_51[], pl33_unused_52[], pl33_unused_53[], pl33_unused_54[], pl33_unused_55[],
	pl33_unused_56[], pl33_unused_57[], pl33_unused_58[], pl33_unused_59[], pl33_unused_60[],
	pl33_unused_61[], pl33_unused_62[], pl33_unused_63[], pl33_unused_64[], pl33_unused_65[],
	pl33_unused_66[], pl33_unused_67[], pl33_unused_68[], pl33_unused_69[], pl33_unused_70[],
	pl33_unused_71[], pl33_unused_72[], pl33_unused_73[], pl33_unused_74[], pl33_unused_75[],
	pl33_unused_76[], pl33_unused_77[], pl33_unused_78[], pl33_unused_79[], pl33_unused_80[],
	pl33_unused_81[], pl33_unused_82[], pl33_unused_83[], pl33_unused_84[], pl33_unused_85[],
	pl33_unused_86[], pl33_unused_87[], pl33_unused_88[], pl33_unused_89[], pl33_unused_90[],
	pl33_unused_91[], pl33_unused_92[], pl33_unused_93[], pl33_unused_94[], pl33_unused_95[],
	pl33_unused_96[], pl33_unused_97[], pl33_unused_98[], pl33_unused_99[], pl33_unused_100[],
	pl33_unused_101[], pl33_unused_102[], pl33_unused_103[], pl33_unused_104[], pl33_unused_105[],
	pl33_unused_106[], pl33_unused_107[], pl33_unused_108[], pl33_unused_109[], pl33_unused_110[],
	pl33_unused_111[], pl33_unused_112[], pl33_unused_113[], pl33_unused_114[], pl33_unused_115[],
	pl33_unused_116[], pl33_unused_117[], pl33_unused_118[], pl33_unused_119[], pl33_unused_120[],
	pl33_unused_121[], pl33_unused_122[], pl33_unused_123[], pl33_unused_124[], pl33_unused_125[],
	pl33_unused_126[], pl33_unused_127[], pl33_unused_128[], pl33_unused_129[], pl33_unused_130[],
	pl33_unused_131[], pl33_unused_132[], pl33_unused_133[], pl33_unused_134[], pl33_unused_135[],
	pl33_unused_136[], pl33_unused_137[], pl33_unused_138[], pl33_unused_139[], pl33_damage_00[],
	pl33_damage_01[], pl33_damage_02[], pl33_damage_03[], pl33_damage_04[], pl33_damage_05[],
	pl33_damage_06[], pl33_damage_07[], pl33_damage_08[], pl33_damage_10[], pl33_damage_11[],
	pl33_damage_12[], pl33_damage_13[], pl33_damage_09[], pl33_damage_09_loop[], pl33_damage_26[],
	pl33_damage_27[], pl33_damage_30[], pl33_damage_33[], pl33_damage_16[], pl33_damage_17[],
	pl33_damage_18[], pl33_damage_19[], pl33_damage_20[], pl33_damage_21[], pl33_damage_22[],
	pl33_damage_23[], pl33_damage_24[], pl33_damage_25[], pl33_damage_28[], pl33_damage_29[],
	pl33_dizzy_00[], pl33_damage_31[], pl33_caught_00[], pl33_caught_01[], pl33_caught_02[],
	pl33_caught_03[], pl33_caught_04[], pl33_caught_05[], pl33_caught_06[], pl33_caught_07[],
	pl33_caught_08[], pl33_caught_09[], pl33_caught_10[], pl33_caught_11[], pl33_caught_12[],
	pl33_caught_13[], pl33_caught_14[], pl33_caught_15[], pl33_caught_16[], pl33_caught_17[],
	pl33_caught_18[], pl33_caught_19[], pl33_caught_20[], pl33_caught_21[], pl33_effect_29[],
	pl33_effect_29_loop[], pl33_effect_30[], pl33_effect_32[], pl33_effect_28[], pl33_effect_27[],
	pl33_effect_31[], pl33_effect_33[], pl33_effect_34[], pl33_effect_07[], pl33_effect_07_loop[],
	pl33_effect_26[], pl33_appear_10[], pl33_effect_00[], pl33_effect_01[], pl33_effect_02[],
	pl33_effect_03[], pl33_effect_04[], pl33_effect_05[], pl33_effect_06[], pl33_effect_12[],
	pl33_effect_10[], pl33_effect_11[], pl33_effect_08[], pl33_effect_09[], pl33_effect_13[],
	pl33_effect_13_loop[], pl33_effect_14[], pl33_effect_15[], pl33_effect_16[], pl33_effect_17[],
	pl33_effect_18[], pl33_effect_19[], pl33_effect_20[], pl33_effect_21[], pl33_effect_22[],
	pl33_effect_23[], pl33_fly_00[], pl33_fly_01[], pl33_fly_02[], pl33_fly_07[], pl33_fly_03[],
	pl33_extra_03[], pl33_extra_03_loop[], pl33_change_04[], pl33_change_00[], pl33_change_01[],
	pl33_change_03[], pl33_change_02[], pl33_extra_01[], pl33_extra_02[], pl33_special_12[],
	pl33_special_12_loop[], pl33_special_13[];
extern const Sint32 *const pl33_stand_tbl[], *const pl33_jump_tbl[], *const pl33_dash_tbl[],
	*const pl33_crouch_tbl[], *const pl33_turn_tbl[], *const pl33_guard_tbl[],
	*const pl33_air_guard_tbl[], *const pl33_stand_p_tbl[], *const pl33_stand_k_tbl[],
	*const pl33_crouch_p_tbl[], *const pl33_crouch_k_tbl[], *const pl33_jump_p_tbl[],
	*const pl33_jump_k_tbl[], *const pl33_damage_tbl[], *const pl33_dizzy_tbl[],
	*const pl33_catch_tbl[], *const pl33_caught_tbl[], *const pl33_get_up_tbl[],
	*const pl33_appear_tbl[], *const pl33_win_tbl[], *const pl33_extra_tbl[],
	*const pl33_special_tbl[], *const pl33_super_tbl[], *const pl33_effect_tbl[],
	*const pl33_change_tbl[], *const pl33_grp25_tbl[], *const pl33_fly_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_33[16] = {
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
const Sint32 *const *const char_tbl_33[] = {
	pl33_stand_tbl, pl33_jump_tbl, pl33_dash_tbl, pl33_crouch_tbl, pl33_turn_tbl, pl33_guard_tbl,
	pl33_air_guard_tbl, pl33_stand_p_tbl, pl33_stand_k_tbl, pl33_crouch_p_tbl, pl33_crouch_k_tbl, pl33_jump_p_tbl,
	pl33_jump_k_tbl, pl33_damage_tbl, pl33_dizzy_tbl, pl33_catch_tbl, pl33_caught_tbl, pl33_get_up_tbl,
	pl33_appear_tbl, pl33_win_tbl, pl33_extra_tbl, pl33_special_tbl, pl33_super_tbl, pl33_effect_tbl,
	pl33_change_tbl, pl33_grp25_tbl, pl33_fly_tbl, pl33_stand_tbl, 0,
};

const Sint32 *const pl33_stand_tbl[] = {
	pl33_stand_00, pl33_stand_00, pl33_stand_02, pl33_stand_03, pl33_stand_00, pl33_stand_00,
	pl33_stand_06, pl33_stand_07, pl33_stand_00, pl33_stand_00, 0,
};

const Sint32 *const pl33_jump_tbl[] = {
	pl33_jump_00, pl33_jump_01, pl33_jump_02, pl33_jump_03, pl33_jump_03, pl33_jump_03,
	pl33_jump_06, pl33_jump_07, pl33_jump_08, pl33_jump_09, pl33_jump_09, pl33_jump_09,
	0,
};

const Sint32 *const pl33_dash_tbl[] = {
	pl33_dash_00, pl33_dash_01, pl33_dash_02, pl33_dash_03, 0,
};

const Sint32 *const pl33_crouch_tbl[] = {
	pl33_crouch_00, pl33_crouch_01, pl33_crouch_02, 0,
};

const Sint32 *const pl33_turn_tbl[] = {
	pl33_turn_00, pl33_turn_01, 0,
};

const Sint32 *const pl33_guard_tbl[] = {
	pl33_guard_00, pl33_guard_01, pl33_guard_02, pl33_guard_01, pl33_guard_04, pl33_guard_05,
	pl33_guard_06, pl33_guard_05, 0,
};

const Sint32 *const pl33_air_guard_tbl[] = {
	pl33_air_guard_00, pl33_air_guard_01, pl33_air_guard_01, 0,
};

const Sint32 *const pl33_stand_p_tbl[] = {
	pl33_stand_p_00, pl33_stand_p_01, pl33_stand_p_02, 0,
};

const Sint32 *const pl33_stand_k_tbl[] = {
	pl33_stand_k_00, pl33_stand_k_01, pl33_stand_k_02, 0,
};

const Sint32 *const pl33_crouch_p_tbl[] = {
	pl33_crouch_p_00, pl33_crouch_p_01, pl33_crouch_p_02, 0,
};

const Sint32 *const pl33_crouch_k_tbl[] = {
	pl33_crouch_k_00, pl33_crouch_k_01, pl33_crouch_k_02, 0,
};

const Sint32 *const pl33_jump_p_tbl[] = {
	pl33_jump_p_00, pl33_jump_p_01, pl33_jump_p_02, pl33_jump_p_00, pl33_jump_p_01, pl33_jump_p_02,
	pl33_jump_p_06, pl33_jump_p_07, 0,
};

const Sint32 *const pl33_jump_k_tbl[] = {
	pl33_jump_k_00, pl33_jump_k_01, pl33_jump_k_02, pl33_jump_k_00, pl33_jump_k_01, pl33_jump_k_02,
	pl33_jump_k_06, 0,
};

const Sint32 *const pl33_damage_tbl[] = {
	pl33_damage_00, pl33_damage_01, pl33_damage_02, pl33_damage_03, pl33_damage_04, pl33_damage_05,
	pl33_damage_06, pl33_damage_07, pl33_damage_08, pl33_damage_09, pl33_damage_10, pl33_damage_11,
	pl33_damage_12, pl33_damage_13, pl33_damage_13, pl33_damage_13, pl33_damage_16, pl33_damage_17,
	pl33_damage_18, pl33_damage_19, pl33_damage_20, pl33_damage_21, pl33_damage_22, pl33_damage_23,
	pl33_damage_24, pl33_damage_25, pl33_damage_26, pl33_damage_27, pl33_damage_28, pl33_damage_29,
	pl33_damage_30, pl33_damage_31, pl33_damage_30, pl33_damage_33, pl33_damage_13, 0,
};

const Sint32 *const pl33_dizzy_tbl[] = {
	pl33_dizzy_00, 0,
};

const Sint32 *const pl33_catch_tbl[] = {
	pl33_catch_00, pl33_catch_01, pl33_catch_02, pl33_catch_03, pl33_catch_04, 0,
};

const Sint32 *const pl33_caught_tbl[] = {
	pl33_caught_00, pl33_caught_01, pl33_caught_02, pl33_caught_03, pl33_caught_04, pl33_caught_05,
	pl33_caught_06, pl33_caught_07, pl33_caught_08, pl33_caught_09, pl33_caught_10, pl33_caught_11,
	pl33_caught_12, pl33_caught_13, pl33_caught_14, pl33_caught_15, pl33_caught_16, pl33_caught_17,
	pl33_caught_18, pl33_caught_19, pl33_caught_20, pl33_caught_21, 0,
};

const Sint32 *const pl33_get_up_tbl[] = {
	pl33_get_up_00, pl33_get_up_01, pl33_get_up_02, pl33_get_up_03, 0,
};

const Sint32 *const pl33_appear_tbl[] = {
	pl33_appear_00, pl33_appear_01, pl33_appear_02, pl33_appear_03, pl33_appear_04, pl33_appear_05,
	pl33_appear_06, pl33_appear_07, pl33_appear_08, pl33_appear_09, pl33_appear_10, 0,
};

const Sint32 *const pl33_win_tbl[] = {
	pl33_win_00, pl33_win_01, pl33_win_02, 0,
};

const Sint32 *const pl33_extra_tbl[] = {
	pl33_extra_00, pl33_extra_01, pl33_extra_02, pl33_extra_03, pl33_extra_04, pl33_extra_05,
	pl33_extra_06, pl33_extra_07, pl33_extra_08, 0,
};

const Sint32 *const pl33_special_tbl[] = {
	pl33_special_00, pl33_special_01, pl33_special_02, pl33_special_03, pl33_special_04, pl33_special_05,
	pl33_special_06, pl33_special_07, pl33_special_08, pl33_special_09, pl33_special_10, pl33_special_11,
	pl33_special_12, pl33_special_13, 0,
};

const Sint32 *const pl33_super_tbl[] = {
	pl33_super_00, pl33_super_01, pl33_super_02, 0,
};

const Sint32 *const pl33_effect_tbl[] = {
	pl33_effect_00, pl33_effect_01, pl33_effect_02, pl33_effect_03, pl33_effect_04, pl33_effect_05,
	pl33_effect_06, pl33_effect_07, pl33_effect_08, pl33_effect_09, pl33_effect_10, pl33_effect_11,
	pl33_effect_12, pl33_effect_13, pl33_effect_14, pl33_effect_15, pl33_effect_16, pl33_effect_17,
	pl33_effect_18, pl33_effect_19, pl33_effect_20, pl33_effect_21, pl33_effect_22, pl33_effect_23,
	pl33_effect_24, pl33_effect_25, pl33_effect_26, pl33_effect_27, pl33_effect_28, pl33_effect_29,
	pl33_effect_30, pl33_effect_31, pl33_effect_32, pl33_effect_33, pl33_effect_34, 0,
};

const Sint32 *const pl33_change_tbl[] = {
	pl33_change_00, pl33_change_01, pl33_change_02, pl33_change_03, pl33_change_04, 0,
};

const Sint32 *const pl33_grp25_tbl[] = {
	pl33_stand_00, 0,
};

const Sint32 *const pl33_fly_tbl[] = {
	pl33_fly_00, pl33_fly_01, pl33_fly_02, pl33_fly_03, pl33_fly_03, pl33_fly_03,
	pl33_fly_01, pl33_fly_07, pl33_fly_07, pl33_fly_07, pl33_fly_01, pl33_fly_01,
	pl33_fly_01, pl33_fly_01, pl33_fly_01, pl33_fly_01, pl33_fly_01, pl33_fly_01,
	0,
};

	/*         prm  type  ctr          end    spr  prio  shadow        atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl33_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    76, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   group 27 2 */
const Sint32 pl33_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    87, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    89, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    90, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    92, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    94, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    96, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    97, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_stand_02 ),
};

/* Char_Stand 3
   group 27 3 */
const Sint32 pl33_stand_03[] = {
	CG_FRAME(    0,    0,   6,           0,   109, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   108, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   107, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   106, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   105, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   104, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   103, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   102, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   101, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   100, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    99, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    98, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_stand_03 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl33_dash_00[] = {
	CG_FRAME(    0,    0,   1,           0,   110, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   111, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   3,           0,   112, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   113, 0x80,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   7 ),
};

const Sint32 pl33_dash_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   114, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   115, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   116, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   117, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   118, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   119, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   120, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   121, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl33_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl33_dash_02[] = {
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   123, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl33_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl33_dash_01[] = {
	CG_FRAME(    0,    0,   2,           0,   135, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   2,           0,   136, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   8 ),
};

const Sint32 pl33_dash_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   138, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   139, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   140, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   141, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   142, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   143, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   144, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   145, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl33_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl33_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,   146, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   147, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl33_dash_03 ),
};

/* Char_Jump 0 */
const Sint32 pl33_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   417, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   426, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   429, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   428, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   429, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   430, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   431, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   432, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   433, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   434, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   435, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   436, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   418, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   419, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   420, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  12,           0,   153, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   422, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl33_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   423, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   424, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_jump_00_loop ),
};

/* Char_Jump 6 */
const Sint32 pl33_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   149, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0x90,   0x74,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   162, 0x90,   0x74,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  12,           0,   153, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl33_jump_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   155, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   156, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_jump_06_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl33_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   417, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   437, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   450, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   451, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   452, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   438, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   442, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  10,           0,   179, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl33_jump_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   180, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_jump_01_loop ),
};

/* Char_Jump 7 */
const Sint32 pl33_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   417, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x90,   0x74,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x90,   0x74,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   169, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl33_jump_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   180, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_jump_07_loop ),
};

/* Char_Jump 2 */
const Sint32 pl33_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   417, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   453, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   464, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   185, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  10,           0,   179, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl33_jump_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   180, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_jump_02_loop ),
};

/* Char_Jump 8 */
const Sint32 pl33_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   417, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x90,   0x74,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x90,   0x74,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   192, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   186, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   187, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   188, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   192, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   185, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl33_jump_08_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   180, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_jump_08_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl33_jump_03[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   157, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_jump_03 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl33_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,   180, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_jump_09 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl33_crouch_00[] = {
	CG_FRAME(    2,    0,   4,           0,   157, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    1,    0,   4,           0,   149, 0x80,   0x0d,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   197, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_crouch_01 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl33_crouch_01[] = {
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    85, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    82, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl33_crouch_02[] = {
	CG_FRAME(    1,    0,   4,           0,   149, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   4,           0,   157, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   123, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl33_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   199, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl33_turn_01[] = {
	CG_FRAME(    0,    0,   8,           0,   201, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   202, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl33_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   205, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl33_guard_01[] = {
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   211, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl33_guard_02[] = {
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   203, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl33_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   215, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   216, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl33_guard_05[] = {
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   221, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl33_guard_06[] = {
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   214, 0x80,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl33_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   224, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl33_air_guard_01[] = {
	CG_FRAME(    0,    0,   2,           0,   227, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   230, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_air_guard_01 ),
};

/* Char_Win 0: win_time */
const Sint32 pl33_win_00[] = {
	CG_FRAME(    0,    0,   2,           0,   234, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   237, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   238, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   240, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   241, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   242, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   243, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   244, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   245, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   246, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   247, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   248, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,          0,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   249, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   256, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   257, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   258, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   259, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   260, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   261, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   262, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   263, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl33_win_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl33_win_01[] = {
	CG_FRAME(    0,    0,  12,           0,   525, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   526, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   265, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  24,           0,   267, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,          0,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl33_win_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   270, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   271, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   272, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_win_01_loop ),
};

/* Char_Win 2: lose_time_ko, draw_time */
const Sint32 pl33_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl33_win_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   289, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   290, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_win_02_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl33_appear_00[] = {
	CG_FRAME(    0,    0,   1,           0,   291, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   292, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   293, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   294, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   298, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_00 ),
};

/* Char_Appear 1: appear_land */
const Sint32 pl33_appear_01[] = {
	CG_FRAME(    0,    0,   1,           0,   299, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   300, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   301, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   302, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   303, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   304, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   305, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   306, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_01 ),
};

/* Char_Appear 2: appear_rise */
const Sint32 pl33_appear_02[] = {
	CG_FRAME(    0,    0,   1,           0,   307, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   308, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   309, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   310, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   311, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   312, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   313, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   314, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_02 ),
};

/* Char_Appear 3: appear_stand */
const Sint32 pl33_appear_03[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   316, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   317, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   318, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   319, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   320, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   321, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   322, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_03 ),
};

/* Char_Appear 4: pls33_00_appear_init */
const Sint32 pl33_appear_04[] = {
	CG_FRAME(    0,    0,   1,           0,   324, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   325, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   326, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   327, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   328, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   329, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   330, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_04 ),
};

/* Char_Appear 5: pls33_00_appear_wait */
const Sint32 pl33_appear_05[] = {
	CG_FRAME(    0,    0,   1,           0,   331, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   332, 0x80,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   333, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   334, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   335, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   336, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_05 ),
};

/* Char_Appear 6: pls33_00_appear_start */
const Sint32 pl33_appear_06[] = {
	CG_FRAME(    0,    0,   1,           0,   339, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   340, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   341, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   342, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   343, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   344, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   345, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   346, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_06 ),
};

/* Char_Appear 7: pls33_00_appear_up */
const Sint32 pl33_appear_07[] = {
	CG_FRAME(    0,    0,   1,           0,   347, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   348, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   349, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   350, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   351, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   352, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   354, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_07 ),
};

/* Char_Appear 8: pls33_00_appear_up_next */
const Sint32 pl33_appear_08[] = {
	CG_FRAME(    0,    0,   1,           0,   355, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   356, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   357, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   358, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   361, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   362, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_08 ),
};

/* Char_Appear 9: appear_hop */
const Sint32 pl33_appear_09[] = {
	CG_FRAME(    0,    0,  16,           0,   153, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   154, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_09 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl33_get_up_00[] = {
	CG_FRAME(    0,    0,   3,           0,   363, 0x40,   0xc9,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   364, 0x40,   0xaa,          0,  0x00, 0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   365, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   366, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   369, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   370, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl33_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   391, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   392, 0x40,   0x09,       0x1c,  0x00, 0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   364, 0x40,   0xaa,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   365, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   366, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   367, 0x40,   0x6e,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   368, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   369, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   370, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl33_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   391, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   392, 0x40,   0x09,       0x1c,  0x00, 0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   392, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   392, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   392, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   364, 0x40,   0xaa,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   365, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   366, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   367, 0x40,   0x6e,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   368, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   369, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   370, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl33_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   395, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   396, 0x40,   0x09,          0,  0x00, 0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   397, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x40,   0x09,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x40,   0xaa,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_get_up_03 ),
};

/* Char_Stand 6
   group 27 6 */
const Sint32 pl33_stand_06[] = {
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   517, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   517, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_stand_06 ),
};

/* Char_Stand 7
   group 27 7 */
const Sint32 pl33_stand_07[] = {
	CG_FRAME(    0,    0,   4,           0,   517, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   517, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_stand_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl33_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   517, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   518, 0xa0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   519, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   520, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_stand_p_00 ),
};

/* Char_Extra 7 */
const Sint32 pl33_extra_07[] = {
	CG_FRAME(    0,    0,   2,           0,   521, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   522, 0xa0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   3,           0,   523, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    2,   2, END_OF_DATA,   524, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_extra_07 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl33_stand_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   525, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   526, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   527, 0xa0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   528, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   529, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   528, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   529, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   528, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   529, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   528, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   517, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl33_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   530, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   533, 0xa0,   0x00,        ATK,  0x00, 0x05,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   2,           0,   534, 0xa0,   0x00,        ATK,  0x00, 0x1e,    0,   0,     0,      0,  14 ),
};

const Sint32 pl33_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   535, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   534, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl33_stand_p_02_loop ),
};

/* Char_Extra 0: stand_hp_shot */
const Sint32 pl33_extra_00[] = {
	CG_FRAME(    0,    0,   5,           0,   536, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   537, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl33_extra_00 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl33_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   659, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   660, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   5,           0,   661, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   3,           0,   662, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   663, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl33_stand_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   538, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,   539, 0xa0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    1,   2,           0,   540, 0xa0,   0x00,        ATK,  0x00, 0x20,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   539, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   539, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   539, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   540, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   540, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   540, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   539, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   540, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   3,           0,   541, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   663, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy): stand_hk_init (standing heavy kick (gamma assist and counter)) */
const Sint32 pl33_stand_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   543, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   556, 0xa0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    1,   3,           0,   557, 0xa0,   0x00,        ATK,  0x00, 0x20,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   558, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   559, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   560, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   561, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   562, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   563, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   544, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   6,           0,   545, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   546, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   547, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   549, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   550, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   551, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   552, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   0,           0,   553, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   0,           0,   554, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   555, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_stand_k_02 ),
};

/* Char_Extra 6: pl33_upper */
const Sint32 pl33_extra_06[] = {
	CG_FRAME(    0,    0,   3,           0,   543, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   556, 0xa0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    1,   3,           0,   557, 0xa0,   0x00,        ATK,  0x00, 0x20,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   558, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   559, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   560, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   561, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   562, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   563, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   544, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   6,           0,   545, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   546, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   547, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   549, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   550, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   551, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   0,           0,   552, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   0,           0,   553, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   0,           0,   554, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   555, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_extra_06 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl33_crouch_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   564, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   565, 0xa0,   0x2e,        ATK,  0x00, 0x03,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   5,           0,   566, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   566, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl33_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl33_crouch_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   197, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   568, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0xa0,   0x2e,        ATK,  0x00, 0x04,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   571, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   197, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl33_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl33_crouch_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   197, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   573, 0xa0,   0x2e,        ATK,  0x00, 0x24,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   574, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   575, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   576, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   577, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   4,           0,   578, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   579, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   580, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   581, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6,           0,   582, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   583, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   584, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   585, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   586, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl33_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl33_crouch_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   587, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   588, 0xa0,   0x2e,        ATK,  0x00, 0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   589, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   589, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    1,   3,           0,   590, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   587, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl33_crouch_k_00 ),
};

/* Char_Extra 8 */
const Sint32 pl33_extra_08[] = {
	CG_FRAME(    0,    0,   2,           0,   591, 0xa0,   0x2e,        ATK,  0x00, 0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   590, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   587, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_extra_08 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl33_crouch_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   201, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   592, 0xa0,   0x2e,        ATK,  0x00, 0x20,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   593, 0xa0,   0x2e,        ATK,  0x00, 0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   4,           0,   594, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   595, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   594, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   595, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   594, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   595, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   594, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   595, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   202, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl33_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl33_crouch_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   596, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   597, 0xa0,   0x2e,        ATK,  0x00, 0x05,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   3,           0,   598, 0xa0,   0x2e,        ATK,  0x00, 0x20,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    1,   6,           0,   599, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   1,           0,   600, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   1,           0,   601, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   1,           0,   602, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2,           0,   603, 0xa0,   0x2e,        ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   1,           0,   604, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   605, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   9,           0,   606, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   6,           0,   607, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   608, 0xa0,   0x2e,        ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl33_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl33_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   738, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   739, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   740, 0xa0,   0x74,        ATK,  0x00, 0x03,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   744, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   741, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   745, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   742, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   746, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   743, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   747, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   737, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   736, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl33_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   611, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    1,   3,           0,   612, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   613, 0xa0,   0x74,        ATK,  0x00, 0x20,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   612, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   613, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   612, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   613, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl33_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl33_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   614, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   622, 0xa0,   0x74,        ATK,  0x00, 0x04,    0,   0,     0,      0, 113 ),
	CG_FRAME( 0x80,    0,   1,           0,   623, 0xa0,   0x74,        ATK,  0x00, 0x1f,    0,   0,     0,      0, 113 ),
	CG_FRAME( 0x80,    0,   1,           0,   624, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0, 113 ),
	CG_FRAME( 0x80,    0,   1,           0,   625, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   626, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   627, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   628, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   629, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   630, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   631, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   632, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   8,           0,   616, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   617, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_p_02 ),
};

/* Char_Jump_P 6 */
const Sint32 pl33_jump_p_06[] = {
	CG_FRAME(    0,    0,   4,           0,   614, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   633, 0xa0,   0x74,        ATK,  0x00, 0x04,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   634, 0xa0,   0x74,        ATK,  0x00, 0x1f,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   635, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   636, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   637, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,   1,           0,   638, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,   1,           0,   639, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   1,           0,   640, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   641, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   642, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   643, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   8,           0,   618, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   617, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_p_06 ),
};

/* Char_Jump_P 7 */
const Sint32 pl33_jump_p_07[] = {
	CG_FRAME(    0,    0,   4,           0,   614, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   644, 0xa0,   0x74,        ATK,  0x00, 0x04,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   645, 0xa0,   0x74,        ATK,  0x00, 0x1f,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   646, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   647, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   648, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   1,           0,   649, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   1,           0,   650, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   1,           0,   651, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   652, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   653, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   654, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   8,           0,   619, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   617, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_p_07 ),
};

/* Char_Jump_K 0 (light): jump_kick
   Char_Jump_K 3 */
const Sint32 pl33_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   153, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   656, 0xa0,   0x74,        ATK,  0x00, 0x03,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2,           0,   657, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   3,           0,   656, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2,           0,   657, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   3,           0,   656, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   655, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_k_00 ),
};

/* Char_Jump_K 1 (medium): jump_kick
   Char_Jump_K 4 */
const Sint32 pl33_jump_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   685, 0xa0,   0x20,        ATK,  0x01,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   685, 0xa0,   0x20, ATK | 0x11,  0x01, 0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   686, 0xa0,   0x20,        ATK,  0x01,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   687, 0xa0,   0x20,        ATK,  0x01,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   5,           0,   688, 0xa0,   0x20,        ATK,  0x01,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4,           0,   688, 0xa0,   0x20,        ATE,  0x01,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   686, 0xa0,   0x20,        ATE,  0x01,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   685, 0xa0,   0x20,        ATE,  0x01,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_k_01 ),
};

/* Char_Extra 4: jump_kick */
const Sint32 pl33_extra_04[] = {
	CG_FRAME(    0,    1,   0,           0,   153, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,  10,           0,   664, 0xa0,   0x74,        ATK,  0x00, 0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   665, 0xa0,   0x74,        ATK,  0x00, 0x22,    0,   0,     0,      0,   6 ),
};

const Sint32 pl33_extra_04_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   665, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   666, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   667, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   668, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   669, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   670, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl33_extra_04_loop ),
};

/* Char_Extra 5: dive_move */
const Sint32 pl33_extra_05[] = {
	CG_FRAME(    0,    0,  13,           0,   674, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   675, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl33_extra_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   155, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_extra_05_loop ),
};

/* Char_Jump_K 6: jump_kick */
const Sint32 pl33_jump_k_06[] = {
	CG_FRAME(    0,    0,   4,           0,   609, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   610, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   681, 0xa0,   0x74,        ATK,  0x00, 0x05,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   681, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   681, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   676, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   677, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   678, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_k_06 ),
};

/* Char_Jump_K 2 (heavy): jump_kick
   Char_Jump_K 5 */
const Sint32 pl33_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   676, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   677, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   678, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   679, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   680, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0xa0,   0x74,        ATK,  0x00, 0x05,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   684, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   684, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   684, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   152, 0xa0,   0x74,        ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_jump_k_02 ),
};

/* Char_Special 10: bomb_init (Smart Bomb) */
const Sint32 pl33_special_10[] = {
	CG_FRAME(    0,    0,   8,           0,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   697, 0xb0,   0x00,        ATK,  0x00, 0x16,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   698, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   699, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   700, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   701, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   702, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   703, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   704, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   705, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   1,           0,   706, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   6,           0,   707, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    1,   3,           0,   708, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,  14,           0,   709, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   712, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   713, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   714, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl33_special_10 ),
};

/* Char_Special 11: bomb_air_init (Smart Bomb) */
const Sint32 pl33_special_11[] = {
	CG_FRAME(    0,    0,   8,           0,   710, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   711, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   710, 0xb0,   0x00,        ATK,  0x00, 0x16,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   698, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   699, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   700, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   701, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   702, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   703, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   704, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   705, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   1,           0,   706, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   6,           0,   707, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    1,   3,           0,   708, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   709, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   712, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   713, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   714, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl33_special_11 ),
};

/* Char_Special 0 */
const Sint32 pl33_special_00[] = {
	CG_FRAME(    0,   19,   8,           0,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   2,           0,   716, 0xb0,   0x00,        ATK,  0x00, 0x17,    0,   0,     0,      0,  84 ),
};

const Sint32 pl33_special_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   717, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   716, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl33_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl33_special_01[] = {
	CG_FRAME(    0,   32,  15,           0,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   2,           0,   716, 0xb0,   0x00,        ATK,  0x00, 0x17,    0,   0,     0,      0,  84 ),
};

const Sint32 pl33_special_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   717, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   716, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl33_special_01_loop ),
};

/* Char_Special 2: unibeam_shot (Unibeam, 2 + atck_level) */
const Sint32 pl33_special_02[] = {
	CG_FRAME(    0,    0,  20,           0,   716, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   718, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl33_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl33_special_03[] = {
	CG_FRAME(    0,    0,  24,           0,   716, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   718, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl33_special_03 ),
};

/* Char_Special 4: unibeam_air_init (Unibeam, 4 + atck_level) */
const Sint32 pl33_special_04[] = {
	CG_FRAME(    0,   19,   8,           0,   611, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   2,           0,   612, 0xb0,   0x00,        ATK,  0x00, 0x17,    0,   0,     0,      0,  85 ),
};

const Sint32 pl33_special_04_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   613, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   612, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl33_special_04_loop ),
};

/* Char_Special 5 */
const Sint32 pl33_special_05[] = {
	CG_FRAME(    0,   32,  14,           0,   611, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   2,           0,   612, 0xb0,   0x00,        ATK,  0x00, 0x17,    0,   0,     0,      0,  85 ),
};

const Sint32 pl33_special_05_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   613, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   612, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl33_special_05_loop ),
};

/* Char_Special 6: repulsor_init (Repulsor Blast, 6 + atck_level) */
const Sint32 pl33_special_06[] = {
	CG_FRAME(    0,    0,   3,           0,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl33_special_06 ),
};

/* Char_Special 7 */
const Sint32 pl33_special_07[] = {
	CG_FRAME(    0,    0,   3,           0,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   715, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl33_special_07 ),
};

/* Char_Special 8: repulsor_move (Repulsor Blast) */
const Sint32 pl33_special_08[] = {
	CG_FRAME(    0,    0,   3,           0,   719, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   720, 0xb0,   0x00,        ATK,  0x00, 0x21,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   721, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   722, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   723, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_special_08 ),
};

/* Char_Special 9: repulsor_wait (Repulsor Blast) */
const Sint32 pl33_special_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   724, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_special_09 ),
};

/* Char_Catch 0: nml_k_set */
const Sint32 pl33_catch_00[] = {
	CG_FRAME(    0,    1,   4,           0,   689, 0x80,   0x00,          0,  0x00, 0x03,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    1,   4,           0,   690, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    1,   4,           0,   693, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    1,   4,           0,   692, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,   4,           0,   693, 0x80,   0x00,          0,  0x00, 0x22,    0,   0,     0, 0x0003,   0 ),
};

const Sint32 pl33_catch_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   124, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   125, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   126, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   1,           0,   127, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   128, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   129, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   130, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   1,           0,   131, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   132, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   133, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   134, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0006,   0 ),
	CG_NEXT( pl33_catch_00_loop ),
};

/* Char_Catch 1: nml_k_drag */
const Sint32 pl33_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   689, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   2,           0,   694, 0x80,   0x00,          0,  0x00, 0x04,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    2,   2,           0,   695, 0x80,   0x00,          0,  0x00, 0x20,    0,   0,     0, 0x0009,   0 ),
};

const Sint32 pl33_catch_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   694, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   695, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_catch_01_loop ),
};

/* Char_Catch 2 */
const Sint32 pl33_catch_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   123, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_catch_02 ),
};

/* Char_Catch 3: jump_p_set */
const Sint32 pl33_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   689, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   4,           0,   691, 0x80,   0x00,          0,  0x00, 0x03,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   4,           0,   696, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   6,           0,   725, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   4,           0,   726, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   727, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    1,   8,           0,   728, 0x80,   0x00,          0,  0x00, 0x20,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    2,   2,           0,   729, 0x80,   0x00,          0,  0x00, 0x04,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   2,           0,   730, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   731, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   732, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   733, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   734, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   735, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   736, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   737, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_catch_03 ),
};

/* Char_Catch 4: nml_p_set */
const Sint32 pl33_catch_04[] = {
	CG_FRAME(    0,    0,   4,           0,   689, 0x80,   0x00,          0,  0x00, 0x03,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   690, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   8,           0,   693, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   3,           0,   692, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   3,           0,   693, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,  10,           0,   689, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   4,           0,   694, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0,   695, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,   2,           0,   694, 0x80,   0x00,          0,  0x00, 0x20,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    2,   2,           0,   695, 0x80,   0x00,          0,  0x00, 0x04,    0,   0,     0, 0x0009,   0 ),
};

const Sint32 pl33_catch_04_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   694, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   695, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_catch_04_loop ),
};

/* Char_Effect 24: shl33_03_down_init */
const Sint32 pl33_effect_24[] = {
	CG_FRAME(    0,    0,   1,           0,   760, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   759, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   760, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   761, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   759, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   761, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   759, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   763, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   761, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   759, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   763, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   764, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   765, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   766, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   1,           0,   765, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
};

const Sint32 pl33_effect_24_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   766, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl33_effect_24_loop ),
};

/* Char_Effect 25: shl33_03_up_init */
const Sint32 pl33_effect_25[] = {
	CG_FRAME(    0,    0,   2,           0,   766, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   765, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   766, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   765, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   764, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   763, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   759, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   763, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   759, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   759, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   760, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   759, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   760, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   759, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   758, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   762, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   758, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   753, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   758, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   752, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   757, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   751, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   756, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   750, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   755, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   749, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   754, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   767, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_25 ),
};

/* Char_Super 0: proton_init (Proton Cannon) */
const Sint32 pl33_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   198, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   0,           0,   525, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   769, 0xc0,   0x00,        ATK,  0x00, 0x14,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   0,           0,   769, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl33_super_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   768, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   769, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_super_00_loop ),
};

/* Char_Super 1: proton_move (Proton Cannon) */
const Sint32 pl33_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   768, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   769, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   768, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   769, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   768, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   769, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   8,           0,   774, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    2,   2,           0,   770, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl33_super_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   771, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   772, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   773, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   770, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl33_super_01_loop ),
};

/* Char_Super 2: proton_wait (Proton Cannon) */
const Sint32 pl33_super_02[] = {
	CG_FRAME(    0,    0,  48, END_OF_DATA,   774, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_super_02 ),
};

const Sint32 pl33_unused_00[] = {	/* nothing reaches it */
	0x00160000,
};

const Sint32 pl33_unused_01[] = {
	0x00000306,
};

const Sint32 pl33_unused_02[] = {
	0x004000c0,
};

const Sint32 pl33_unused_03[] = {
	0x00000000,
};

const Sint32 pl33_unused_04[] = {
	0x00000000,
};

const Sint32 pl33_unused_05[] = {
	0x00020000,
};

const Sint32 pl33_unused_06[] = {
	0x000000ec,
};

const Sint32 pl33_unused_07[] = {
	0x00000080,
};

const Sint32 pl33_unused_08[] = {
	0x00000000,
};

const Sint32 pl33_unused_09[] = {
	0x00000000,
};

const Sint32 pl33_unused_10[] = {
	0x00020000,
};

const Sint32 pl33_unused_11[] = {
	0x000000fa,
};

const Sint32 pl33_unused_12[] = {
	0x00000080,
};

const Sint32 pl33_unused_13[] = {
	0x00000000,
};

const Sint32 pl33_unused_14[] = {
	0x00000000,
};

const Sint32 pl33_unused_15[] = {
	0x00020000,
};

const Sint32 pl33_unused_16[] = {
	0x000000fb,
};

const Sint32 pl33_unused_17[] = {
	0x00000080,
};

const Sint32 pl33_unused_18[] = {
	0x00000000,
};

const Sint32 pl33_unused_19[] = {
	0x00000000,
};

const Sint32 pl33_unused_20[] = {
	0x00020000,
};

const Sint32 pl33_unused_21[] = {
	0x000000fc,
};

const Sint32 pl33_unused_22[] = {
	0x00000080,
};

const Sint32 pl33_unused_23[] = {
	0x00000000,
};

const Sint32 pl33_unused_24[] = {
	0x00000000,
};

const Sint32 pl33_unused_25[] = {
	0x00020000,
};

const Sint32 pl33_unused_26[] = {
	0x000000fd,
};

const Sint32 pl33_unused_27[] = {
	0x00000080,
};

const Sint32 pl33_unused_28[] = {
	0x00000000,
};

const Sint32 pl33_unused_29[] = {
	0x00000000,
};

const Sint32 pl33_unused_30[] = {
	0x00020000,
};

const Sint32 pl33_unused_31[] = {
	0x000000fe,
};

const Sint32 pl33_unused_32[] = {
	0x00000080,
};

const Sint32 pl33_unused_33[] = {
	0x00000000,
};

const Sint32 pl33_unused_34[] = {
	0x00000000,
};

const Sint32 pl33_unused_35[] = {
	0x00020000,
};

const Sint32 pl33_unused_36[] = {
	0x000000ff,
};

const Sint32 pl33_unused_37[] = {
	0x00000080,
};

const Sint32 pl33_unused_38[] = {
	0x00000000,
};

const Sint32 pl33_unused_39[] = {
	0x00000000,
};

const Sint32 pl33_unused_40[] = {
	0x00020500,
};

const Sint32 pl33_unused_41[] = {
	0x00000100,
};

const Sint32 pl33_unused_42[] = {
	0x00000080,
};

const Sint32 pl33_unused_43[] = {
	0x00000000,
};

const Sint32 pl33_unused_44[] = {
	0x00000000,
};

const Sint32 pl33_unused_45[] = {
	0x00020300,
};

const Sint32 pl33_unused_46[] = {
	0x00000101,
};

const Sint32 pl33_unused_47[] = {
	0x00000080,
};

const Sint32 pl33_unused_48[] = {
	0x00000000,
};

const Sint32 pl33_unused_49[] = {
	0x00000000,
};

const Sint32 pl33_unused_50[] = {
	0x00020500,
};

const Sint32 pl33_unused_51[] = {
	0x000000fc,
};

const Sint32 pl33_unused_52[] = {
	0x00000080,
};

const Sint32 pl33_unused_53[] = {
	0x00000000,
};

const Sint32 pl33_unused_54[] = {
	0x00000000,
};

const Sint32 pl33_unused_55[] = {
	0x00020100,
};

const Sint32 pl33_unused_56[] = {
	0x000000fc,
};

const Sint32 pl33_unused_57[] = {
	0x00000080,
};

const Sint32 pl33_unused_58[] = {
	0x00000000,
};

const Sint32 pl33_unused_59[] = {
	0x00000000,
};

const Sint32 pl33_unused_60[] = {
	0x00020300,
};

const Sint32 pl33_unused_61[] = {
	0x00000102,
};

const Sint32 pl33_unused_62[] = {
	0x00000080,
};

const Sint32 pl33_unused_63[] = {
	0x00000000,
};

const Sint32 pl33_unused_64[] = {
	0x00000000,
};

const Sint32 pl33_unused_65[] = {
	0x00020500,
};

const Sint32 pl33_unused_66[] = {
	0x000000fc,
};

const Sint32 pl33_unused_67[] = {
	0x00000080,
};

const Sint32 pl33_unused_68[] = {
	0x00000000,
};

const Sint32 pl33_unused_69[] = {
	0x00000000,
};

const Sint32 pl33_unused_70[] = {
	0x00020100,
};

const Sint32 pl33_unused_71[] = {
	0x000000fc,
};

const Sint32 pl33_unused_72[] = {
	0x00000080,
};

const Sint32 pl33_unused_73[] = {
	0x00000000,
};

const Sint32 pl33_unused_74[] = {
	0x00000000,
};

const Sint32 pl33_unused_75[] = {
	0x00020300,
};

const Sint32 pl33_unused_76[] = {
	0x00000103,
};

const Sint32 pl33_unused_77[] = {
	0x00000080,
};

const Sint32 pl33_unused_78[] = {
	0x00000000,
};

const Sint32 pl33_unused_79[] = {
	0x00000000,
};

const Sint32 pl33_unused_80[] = {
	0x00020500,
};

const Sint32 pl33_unused_81[] = {
	0x000000fc,
};

const Sint32 pl33_unused_82[] = {
	0x00000080,
};

const Sint32 pl33_unused_83[] = {
	0x00000000,
};

const Sint32 pl33_unused_84[] = {
	0x00000000,
};

const Sint32 pl33_unused_85[] = {
	0x00020100,
};

const Sint32 pl33_unused_86[] = {
	0x000000fc,
};

const Sint32 pl33_unused_87[] = {
	0x00000080,
};

const Sint32 pl33_unused_88[] = {
	0x00000000,
};

const Sint32 pl33_unused_89[] = {
	0x00000000,
};

const Sint32 pl33_unused_90[] = {
	0x00020300,
};

const Sint32 pl33_unused_91[] = {
	0x00000104,
};

const Sint32 pl33_unused_92[] = {
	0x00000080,
};

const Sint32 pl33_unused_93[] = {
	0x00000000,
};

const Sint32 pl33_unused_94[] = {
	0x00000000,
};

const Sint32 pl33_unused_95[] = {
	0x00020500,
};

const Sint32 pl33_unused_96[] = {
	0x000000fc,
};

const Sint32 pl33_unused_97[] = {
	0x00000080,
};

const Sint32 pl33_unused_98[] = {
	0x00000000,
};

const Sint32 pl33_unused_99[] = {
	0x00000000,
};

const Sint32 pl33_unused_100[] = {
	0x00020100,
};

const Sint32 pl33_unused_101[] = {
	0x000000fc,
};

const Sint32 pl33_unused_102[] = {
	0x00000080,
};

const Sint32 pl33_unused_103[] = {
	0x00000000,
};

const Sint32 pl33_unused_104[] = {
	0x00000000,
};

const Sint32 pl33_unused_105[] = {
	0x00020300,
};

const Sint32 pl33_unused_106[] = {
	0x00000105,
};

const Sint32 pl33_unused_107[] = {
	0x00000080,
};

const Sint32 pl33_unused_108[] = {
	0x00000000,
};

const Sint32 pl33_unused_109[] = {
	0x00000000,
};

const Sint32 pl33_unused_110[] = {
	0x00020500,
};

const Sint32 pl33_unused_111[] = {
	0x000000fc,
};

const Sint32 pl33_unused_112[] = {
	0x00000080,
};

const Sint32 pl33_unused_113[] = {
	0x00000000,
};

const Sint32 pl33_unused_114[] = {
	0x00000000,
};

const Sint32 pl33_unused_115[] = {
	0x00020100,
};

const Sint32 pl33_unused_116[] = {
	0x000000fc,
};

const Sint32 pl33_unused_117[] = {
	0x00000080,
};

const Sint32 pl33_unused_118[] = {
	0x00000000,
};

const Sint32 pl33_unused_119[] = {
	0x00000000,
};

const Sint32 pl33_unused_120[] = {
	0x00020300,
};

const Sint32 pl33_unused_121[] = {
	0x00000106,
};

const Sint32 pl33_unused_122[] = {
	0x00000080,
};

const Sint32 pl33_unused_123[] = {
	0x00000000,
};

const Sint32 pl33_unused_124[] = {
	0x00000000,
};

const Sint32 pl33_unused_125[] = {
	0x00030500,
};

const Sint32 pl33_unused_126[] = {
	0x000000fc,
};

const Sint32 pl33_unused_127[] = {
	0x00000080,
};

const Sint32 pl33_unused_128[] = {
	0x00000000,
};

const Sint32 pl33_unused_129[] = {
	0x00000000,
};

const Sint32 pl33_unused_130[] = {
	0x00030100,
};

const Sint32 pl33_unused_131[] = {
	0x000000fc,
};

const Sint32 pl33_unused_132[] = {
	0x00000080,
};

const Sint32 pl33_unused_133[] = {
	0x00000000,
};

const Sint32 pl33_unused_134[] = {
	0x00000000,
};

const Sint32 pl33_unused_135[] = {
	0x80060300,
};

const Sint32 pl33_unused_136[] = {
	0x00000107,
};

const Sint32 pl33_unused_137[] = {
	0x00000080,
};

const Sint32 pl33_unused_138[] = {
	0x00000000,
};

const Sint32 pl33_unused_139[] = {
	0x00000000,
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl33_damage_00[] = {
	CG_FRAME(    0,    0,   5,           0,   776, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   777, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   778, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl33_damage_01[] = {
	CG_FRAME(    0,    0,   6,           0,   780, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   781, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   782, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   778, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl33_damage_02[] = {
	CG_FRAME(    0,    0,   8,           0,   784, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   785, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   781, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   782, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   778, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl33_damage_03[] = {
	CG_FRAME(    0,    0,   5,           0,   787, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   788, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   789, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl33_damage_04[] = {
	CG_FRAME(    0,    0,  10,           0,   791, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   792, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   793, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl33_damage_05[] = {
	CG_FRAME(    0,    0,   8,           0,   795, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   796, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   797, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   798, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   799, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl33_damage_06[] = {
	CG_FRAME(    0,    0,   5,           0,   801, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   802, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   801, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   802, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl33_damage_07[] = {
	CG_FRAME(    0,    0,   6,           0,   804, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   805, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   202, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl33_damage_08[] = {
	CG_FRAME(    0,    0,   8,           0,   807, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   808, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   809, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   810, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   811, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl33_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl33_damage_10[] = {
	CG_FRAME(    0,    0,   6,           0,   791, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   792, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl33_damage_11[] = {
	CG_FRAME(    0,    0,   4,           0,   795, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   796, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   798, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl33_damage_12[] = {
	CG_FRAME(    0,    0,   4,           0,   813, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   814, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   815, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl33_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   817, 0x40,   0xa9,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   818, 0x40,   0xa9,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl33_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl33_damage_09[] = {
	CG_FRAME(    0,    0,   3,           0,   787, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   836, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   835, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   837, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   833, 0x40,   0x8b,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   838, 0x40,   0xc9,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl33_damage_09_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   839, 0x40,   0xc9,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl33_damage_09_loop ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl33_damage_26[] = {
	CG_FRAME(    0,    0,   3,           0,   831, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,   832, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   833, 0x40,   0x8c,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   887, 0x60,   0xc8,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl33_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl33_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,   840, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   848, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   849, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   850, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   844, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   845, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   846, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   847, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   154, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl33_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   840, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   841, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   842, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   843, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   844, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   845, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   846, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   847, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   154, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_30 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl33_damage_33[] = {
	CG_FRAME(    0,    0,   3,           0,   367, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   369, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   370, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_damage_33 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl33_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   821, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   822, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl33_damage_17[] = {
	CG_FRAME(    0,    0,   4,           0,   823, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   824, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl33_damage_18[] = {
	CG_FRAME(    0,    0,   4,           0,   825, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   826, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl33_damage_19[] = {
	CG_FRAME(    0,    0,   4,           0,   827, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   828, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl33_damage_20[] = {
	CG_FRAME(    0,    0,   4,           0,   829, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   830, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl33_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   821, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   822, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl33_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   823, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   824, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl33_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   825, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   826, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl33_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   827, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   828, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl33_damage_25[] = {
	CG_FRAME(    0,    0,   4,           0,   829, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   830, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl33_damage_25 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl33_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   851, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   852, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   853, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   854, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   855, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   856, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl33_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   857, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   858, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   859, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   860, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   861, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   862, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl33_damage_29 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl33_dizzy_00[] = {
	CG_FRAME(    0,    0,   4,           0,   863, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   864, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   865, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   866, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   867, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   868, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   869, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   870, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   871, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   872, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   873, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   874, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   875, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   876, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   877, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   878, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   879, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   880, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   881, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   882, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   883, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   884, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   885, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   886, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl33_dizzy_00 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl33_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   887, 0x60,   0xc8,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl33_damage_31 ),
};

/* Char_Caught 0 */
const Sint32 pl33_caught_00[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   776, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl33_caught_01[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   780, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl33_caught_02[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   784, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl33_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl33_caught_03[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   787, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl33_caught_04[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   791, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl33_caught_05[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   795, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl33_caught_06[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   801, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl33_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl33_caught_07[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   804, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl33_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl33_caught_08[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   807, 0x40,   0x2e,          0,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl33_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl33_caught_09[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   791, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl33_caught_10[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   795, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl33_caught_11[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   813, 0x40,   0x74,          0,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl33_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl33_caught_12[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   817, 0x40,   0xc8,          0,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl33_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl33_caught_13[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   821, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl33_caught_14[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   823, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl33_caught_15[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   825, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl33_caught_16[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   827, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl33_caught_17[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   829, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl33_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl33_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   851, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl33_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl33_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   857, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl33_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl33_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   887, 0x60,   0xc8,          0,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl33_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl33_caught_21[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   835, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl33_caught_21 ),
};

/* Char_Effect 29: shl33_05_shot_init */
const Sint32 pl33_effect_29[] = {
	CG_FRAME(    0,    0,   1,           0,   894, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   895, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 104 ),
};

const Sint32 pl33_effect_29_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   896, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   888, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   889, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   890, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   891, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   892, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   893, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   894, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   895, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl33_effect_29_loop ),
};

/* Char_Effect 30 */
const Sint32 pl33_effect_30[] = {
	CG_FRAME(    0,    0,   4,           0,   897, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   898, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   899, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   900, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_30 ),
};

/* Char_Effect 32 */
const Sint32 pl33_effect_32[] = {
	CG_FRAME(    0,    0,   2,           0,   901, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   902, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   903, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   904, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   905, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   906, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   907, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   908, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_32 ),
};

/* Char_Effect 28 */
const Sint32 pl33_effect_28[] = {
	CG_FRAME(    0,    0,   2,           0,   901, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   902, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   903, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   1,           0,   904, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   1,           0,   905, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   1,           0,   906, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0,   907, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   908, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_28 ),
};

/* Char_Effect 27 */
const Sint32 pl33_effect_27[] = {
	CG_FRAME(    0,    0,   2,           0,   909, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   910, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   911, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   912, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   913, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   914, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   915, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   916, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_27 ),
};

/* Char_Effect 31 */
const Sint32 pl33_effect_31[] = {
	CG_FRAME(    0,    0,   2,           0,   917, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   918, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   919, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   920, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   921, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   922, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   923, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   924, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   925, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   926, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_31 ),
};

/* Char_Effect 33 */
const Sint32 pl33_effect_33[] = {
	CG_FRAME(    0,    0,   2,           0,  1065, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1066, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1067, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1068, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1069, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1070, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1071, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1072, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_33 ),
};

/* Char_Effect 34 */
const Sint32 pl33_effect_34[] = {
	CG_FRAME(    0,    0,   2,           0,  1073, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,  1074, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,  1075, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,  1076, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,  1077, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,  1078, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,  1079, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1080, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_34 ),
};

/* Char_Effect 7: pls33_00_flash_init */
const Sint32 pl33_effect_07[] = {
	CG_FRAME(    0,    0,   4,           0, 32768, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32769, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32770, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32771, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32772, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32773, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32774, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32775, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32776, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32777, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32778, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl33_effect_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32779, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32785, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32790, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_07_loop ),
};

/* Char_Effect 26 */
const Sint32 pl33_effect_26[] = {
	CG_FRAME(    0,    0,   6,           0,   953, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0,   954, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   955, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   956, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   957, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   958, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   959, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   960, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   961, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   962, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   963, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   964, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_26 ),
};

/* Char_Appear 10: pls33_00_appear_ground_init */
const Sint32 pl33_appear_10[] = {
	CG_FRAME(    0,    0,   2,           0,   965, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   966, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   967, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   968, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_appear_10 ),
};

/* Char_Effect 0: shl33_00_part_init */
const Sint32 pl33_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0, 32792, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32791, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl33_effect_00 ),
};

/* Char_Effect 1: shl33_00_beam_move */
const Sint32 pl33_effect_01[] = {
	CG_FRAME(    0,    0,   1,           0, 32797, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32798, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32803, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32804, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32809, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32810, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32815, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32816, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_01 ),
};

/* Char_Effect 2: shl33_00_part_init */
const Sint32 pl33_effect_02[] = {
	CG_FRAME(    0,    0,   1,           0, 32793, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
};

/* Char_Effect 3: shl33_00_part_init */
const Sint32 pl33_effect_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32794, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl33_effect_02 ),
};

/* Char_Effect 4: shl33_00_beam_move */
const Sint32 pl33_effect_04[] = {
	CG_FRAME(    0,    0,   1,           0, 32799, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32800, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32805, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32806, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32811, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32812, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32817, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32818, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_04 ),
};

/* Char_Effect 5: shl33_00_head_init */
const Sint32 pl33_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0, 32795, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32796, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl33_effect_05 ),
};

/* Char_Effect 6: shl33_00_fire */
const Sint32 pl33_effect_06[] = {
	CG_FRAME(    0,    0,   1,           0, 32801, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32802, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32807, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32808, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32813, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32814, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32819, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32820, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_06 ),
};

/* Char_Effect 12: shl33_02_bomb_init, shl33_02_trail_init */
const Sint32 pl33_effect_12[] = {
	CG_FRAME(    0,    0,   2,           0, 32821, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0, 32822, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0, 32823, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32824, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl33_effect_12 ),
};

/* Char_Effect 10 */
const Sint32 pl33_effect_10[] = {
	CG_FRAME(    0,    0,   1,           0,   981, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   982, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   983, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   984, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   969, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   972, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   973, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   0,           0,   974, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   975, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   0,           0,   976, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   977, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   1,           0,   977, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   979, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   980, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   979, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   977, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   977, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   976, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   975, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   974, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   973, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   972, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   984, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   983, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   982, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   981, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl33_effect_11[] = {
	CG_FRAME(    0,    0,   1,           0,   997, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   998, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   999, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1000, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   985, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   988, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   1,           0,   989, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   0,           0,   990, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   1,           0,   991, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   0,           0,   992, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   1,           0,   993, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   1,           0,   993, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   995, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   996, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   995, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   993, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   993, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   992, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   991, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   990, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   989, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   988, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1000, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   999, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   998, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   997, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_11 ),
};

/* Char_Effect 8 */
const Sint32 pl33_effect_08[] = {
	CG_FRAME(    0,    0,   1,           0,  1045, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1046, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1047, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1048, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1035, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1036, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,  1037, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   0,           0,  1038, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,  1039, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   0,           0,  1040, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,  1041, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,  1042, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1041, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1042, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1043, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1044, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1043, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1042, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1041, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1042, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1041, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1040, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  1039, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1038, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1037, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,  1036, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1048, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1047, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1046, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1045, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_10 ),
};

/* Char_Effect 9 */
const Sint32 pl33_effect_09[] = {
	CG_FRAME(    0,    0,   1,           0,  1061, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1062, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1063, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1064, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1051, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1052, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,  1053, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   0,           0,  1054, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,  1055, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   0,           0,  1056, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,  1057, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,  1058, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,  1057, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,  1058, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1059, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1060, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1059, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1058, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1057, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1058, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1057, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1056, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  1055, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1054, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1053, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,  1052, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1064, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1063, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1062, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1061, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_11 ),
};

/* Char_Effect 13: shl33_04_head_init */
const Sint32 pl33_effect_13[] = {
	CG_FRAME(    0,    0,   1,           0, 32825, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32826, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32827, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32828, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl33_effect_13_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32829, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0, 32830, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0, 32831, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32832, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl33_effect_13_loop ),
};

/* Char_Effect 14: shl33_04_fire */
const Sint32 pl33_effect_14[] = {
	CG_FRAME(    0,    0,   2,           0, 32833, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32827, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32826, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32825, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_14 ),
};

/* Char_Effect 15: shl33_04_part_init (15 + sub_type) */
const Sint32 pl33_effect_15[] = {
	CG_FRAME(    0,    0,   1,           0, 32834, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 16 */
const Sint32 pl33_effect_16[] = {
	CG_FRAME(    0,    0,   1,           0, 32834, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 17 */
const Sint32 pl33_effect_17[] = {
	CG_FRAME(    0,    0,   1,           0, 32835, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 18 */
const Sint32 pl33_effect_18[] = {
	CG_FRAME(    0,    0,   1,           0, 32835, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 19 */
const Sint32 pl33_effect_19[] = {
	CG_FRAME(    0,    0,   1,           0, 32836, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 20 */
const Sint32 pl33_effect_20[] = {
	CG_FRAME(    0,    0,   1,           0, 32836, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 21 */
const Sint32 pl33_effect_21[] = {
	CG_FRAME(    0,    0,   1,           0, 32837, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 22 */
const Sint32 pl33_effect_22[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32837, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl33_effect_15 ),
};

/* Char_Effect 23: shl33_04_beam */
const Sint32 pl33_effect_23[] = {
	CG_FRAME(    0,    0,   2,           0, 32838, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32839, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32840, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32841, 0xc0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32842, 0xc0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_effect_23 ),
};

/* Char_Fly 0: fly_init (Flight) */
const Sint32 pl33_fly_00[] = {
	CG_FRAME(    0,    0,   1,           0,   158, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   159, 0x90,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   160, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   161, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   162, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   163, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   164, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   165, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   166, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   167, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl33_fly_00 ),
};

/* Char_Fly 1: act_fly
   Char_Fly 6
   Char_Fly 10
   Char_Fly 11
   Char_Fly 12
   Char_Fly 13 */
const Sint32 pl33_fly_01[] = {
	CG_FRAME(    0,    0,   1,           0,   468, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   469, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   470, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   471, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   472, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   473, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   474, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   475, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl33_fly_01 ),
};

/* Char_Fly 2 */
const Sint32 pl33_fly_02[] = {
	CG_FRAME(    0,    0,   1,           0,   161, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   162, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   163, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   164, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   165, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   166, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   167, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl33_fly_02 ),
};

/* Char_Fly 7
   Char_Fly 8
   Char_Fly 9 */
const Sint32 pl33_fly_07[] = {
	CG_FRAME(    0,    1,   1,           0,   170, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   173, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   174, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   175, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   176, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   177, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl33_fly_07 ),
};

/* Char_Fly 3
   Char_Fly 4
   Char_Fly 5 */
const Sint32 pl33_fly_03[] = {
	CG_FRAME(    0,    0,   1,           0,   186, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   190, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   192, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   193, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl33_fly_03 ),
};

/* Char_Extra 3: chouhatsu_init (the taunt), fly_start5 */
const Sint32 pl33_extra_03[] = {
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,          0,  0x00, 0x0f,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   2,           0,   256, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    3,   2,           0,   257, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    3,   2,           0,   258, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    3,   2,           0,   259, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    3,   2,           0,   260, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    3,   2,           0,   261, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    3,   2,           0,   262, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   3,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   3,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl33_extra_03_loop[] = {
	CG_FRAME(    0,    3,   6, END_OF_DATA,   263, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_extra_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl33_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x00,          0,  0x00, 0x0f,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,   4,           0,   256, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    5,  12, END_OF_DATA,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_change_04 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl33_change_00[] = {
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,          0,  0x00, 0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   257, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   261, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   262, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   263, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl33_change_01[] = {
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,          0,  0x00, 0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   257, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   261, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   262, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   263, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_change_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl33_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,          0,  0x00, 0x04,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   257, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   261, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   1,           0,   262, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   252, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   263, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_change_03 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl33_change_02[] = {
	CG_FRAME(    0,    0,   2,           0,   180, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x90,   0x74,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl33_change_02 ),
};

/* Char_Extra 1: t_kick_init */
const Sint32 pl33_extra_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   657, 0xa0,   0x74,        ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl33_extra_01 ),
};

/* Char_Extra 2: t_kick_move, fly_start5 */
const Sint32 pl33_extra_02[] = {
	CG_FRAME(    0,    0,  12,           0,   525, 0x80,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x00,        ATE,  0x00, 0x0a,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x80,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   267, 0x80,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl33_extra_02 ),
};

/* Char_Special 12: snapback_init (the snapback) */
const Sint32 pl33_special_12[] = {
	CG_FRAME(    0,    0,   5,           0,   530, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    2,   2,           0,   533, 0xa0,   0x00,        ATK,  0x00, 0x05,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   2,           0,   534, 0xa0,   0x00,        ATK,  0x00, 0x1e,    0,   0,     0,      0,  14 ),
};

const Sint32 pl33_special_12_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   535, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   534, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl33_special_12_loop ),
};

/* Char_Special 13: snapback_move (the snapback) */
const Sint32 pl33_special_13[] = {
	CG_FRAME(    0,    0,   5,           0,   536, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   537, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl33_special_13 ),
};
