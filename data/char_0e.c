/* Venom (character 0e): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_0e      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_0e     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl0e.c) says which group
                   and number each move uses.
   pl0e_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl0e_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl0e_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl0e_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_0e
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_0e, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_0e */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl0e_stand_00[], pl0e_stand_02[], pl0e_stand_03[], pl0e_stand_06[],
	pl0e_stand_07[], pl0e_jump_00[], pl0e_jump_00_loop[], pl0e_jump_01[], pl0e_jump_01_loop[],
	pl0e_jump_02[], pl0e_jump_02_loop[], pl0e_jump_03[], pl0e_jump_06[], pl0e_jump_06_loop[],
	pl0e_jump_07[], pl0e_jump_07_loop[], pl0e_jump_08[], pl0e_jump_08_loop[], pl0e_jump_09[],
	pl0e_dash_00[], pl0e_dash_00_loop[], pl0e_dash_02[], pl0e_dash_01[], pl0e_dash_01_loop[],
	pl0e_dash_03[], pl0e_crouch_00[], pl0e_crouch_01[], pl0e_crouch_02[], pl0e_turn_00[],
	pl0e_turn_01[], pl0e_guard_00[], pl0e_guard_01[], pl0e_guard_02[], pl0e_guard_04[],
	pl0e_guard_05[], pl0e_guard_06[], pl0e_air_guard_00[], pl0e_air_guard_01[], pl0e_stand_p_00[],
	pl0e_stand_p_03[], pl0e_stand_p_01[], pl0e_stand_p_02[], pl0e_stand_k_00[], pl0e_stand_k_03[],
	pl0e_stand_k_01[], pl0e_stand_k_02[], pl0e_crouch_p_00[], pl0e_crouch_p_01[], pl0e_crouch_p_02[],
	pl0e_crouch_k_00[], pl0e_crouch_k_01[], pl0e_crouch_k_02[], pl0e_jump_p_00[], pl0e_jump_p_01[],
	pl0e_jump_p_02[], pl0e_jump_k_00[], pl0e_jump_k_01[], pl0e_jump_k_02[], pl0e_damage_00[],
	pl0e_damage_01[], pl0e_damage_02[], pl0e_damage_03[], pl0e_damage_04[], pl0e_damage_05[],
	pl0e_damage_06[], pl0e_damage_07[], pl0e_damage_08[], pl0e_damage_09[], pl0e_damage_09_loop[],
	pl0e_damage_10[], pl0e_damage_11[], pl0e_damage_12[], pl0e_damage_13[], pl0e_damage_16[],
	pl0e_damage_17[], pl0e_damage_18[], pl0e_damage_19[], pl0e_damage_20[], pl0e_damage_21[],
	pl0e_damage_22[], pl0e_damage_23[], pl0e_damage_24[], pl0e_damage_25[], pl0e_damage_26[],
	pl0e_damage_27[], pl0e_damage_27_loop[], pl0e_damage_28[], pl0e_damage_29[], pl0e_damage_30[],
	pl0e_damage_31[], pl0e_damage_33[], pl0e_dizzy_00[], pl0e_catch_00[], pl0e_catch_00_loop[],
	pl0e_catch_01[], pl0e_catch_01_loop[], pl0e_catch_02[], pl0e_catch_02_loop[], pl0e_catch_03[],
	pl0e_catch_04[], pl0e_catch_04_loop[], pl0e_get_up_00[], pl0e_get_up_01[], pl0e_unused_00[],
	pl0e_get_up_02[], pl0e_get_up_03[], pl0e_appear_00[], pl0e_win_00[], pl0e_win_00_loop[],
	pl0e_win_01[], pl0e_win_01_loop[], pl0e_win_02[], pl0e_win_02_loop[], pl0e_extra_02[],
	pl0e_change_04[], pl0e_extra_03[], pl0e_extra_03_loop[], pl0e_extra_00[], pl0e_extra_00_loop[],
	pl0e_extra_01[], pl0e_extra_04[], pl0e_special_00[], pl0e_special_00_loop[], pl0e_special_01[],
	pl0e_special_01_loop[], pl0e_special_02[], pl0e_special_02_loop[], pl0e_special_03[],
	pl0e_special_03_loop[], pl0e_special_04[], pl0e_special_05[], pl0e_special_05_loop[],
	pl0e_special_06[], pl0e_special_06_loop[], pl0e_special_07[], pl0e_special_08[],
	pl0e_special_09[], pl0e_special_10[], pl0e_catch_06[], pl0e_special_12[], pl0e_special_13[],
	pl0e_special_13_loop[], pl0e_special_14[], pl0e_special_15[], pl0e_super_00[],
	pl0e_super_00_loop[], pl0e_super_04[], pl0e_super_04_loop[], pl0e_super_01[],
	pl0e_super_01_loop[], pl0e_super_02[], pl0e_super_02_loop[], pl0e_super_03[], pl0e_super_06[],
	pl0e_super_05[], pl0e_super_07[], pl0e_super_07_loop[], pl0e_super_08[], pl0e_super_09[],
	pl0e_super_09_loop[], pl0e_special_16[], pl0e_special_17[], pl0e_effect_00[], pl0e_effect_01[],
	pl0e_effect_02[], pl0e_effect_03[], pl0e_effect_04[], pl0e_effect_05[], pl0e_effect_06[],
	pl0e_effect_07[], pl0e_effect_08[], pl0e_effect_09[], pl0e_effect_10[], pl0e_effect_10_loop[],
	pl0e_effect_11[], pl0e_effect_11_loop[], pl0e_effect_12[], pl0e_effect_12_loop[],
	pl0e_effect_13[], pl0e_effect_13_loop[], pl0e_effect_14[], pl0e_effect_14_loop[],
	pl0e_effect_15[], pl0e_effect_15_loop[], pl0e_effect_16[], pl0e_effect_16_loop[],
	pl0e_effect_17[], pl0e_effect_17_loop[], pl0e_effect_18[], pl0e_effect_20[], pl0e_effect_19[],
	pl0e_effect_19_loop[], pl0e_effect_21[], pl0e_effect_22[], pl0e_effect_23[], pl0e_effect_24[],
	pl0e_effect_25[], pl0e_effect_26[], pl0e_effect_27[], pl0e_effect_28[], pl0e_effect_88[],
	pl0e_effect_29[], pl0e_effect_29_loop[], pl0e_effect_30[], pl0e_effect_30_loop[],
	pl0e_effect_31[], pl0e_effect_32[], pl0e_effect_33[], pl0e_effect_34[], pl0e_effect_35[],
	pl0e_effect_36[], pl0e_effect_37[], pl0e_effect_38[], pl0e_effect_39[], pl0e_effect_39_loop[],
	pl0e_effect_45[], pl0e_effect_46[], pl0e_effect_47[], pl0e_effect_48[], pl0e_effect_49[],
	pl0e_effect_50[], pl0e_effect_51[], pl0e_effect_52[], pl0e_effect_53[], pl0e_effect_54[],
	pl0e_effect_65[], pl0e_effect_65_loop[], pl0e_effect_66[], pl0e_effect_66_loop[],
	pl0e_effect_67[], pl0e_effect_67_loop[], pl0e_effect_68[], pl0e_effect_68_loop[],
	pl0e_effect_69[], pl0e_effect_69_loop[], pl0e_effect_70[], pl0e_effect_70_loop[],
	pl0e_effect_71[], pl0e_effect_71_loop[], pl0e_effect_72[], pl0e_effect_72_loop[],
	pl0e_effect_73[], pl0e_effect_73_loop[], pl0e_effect_74[], pl0e_effect_74_loop[],
	pl0e_effect_40[], pl0e_effect_40_loop[], pl0e_effect_55[], pl0e_effect_56[], pl0e_effect_57[],
	pl0e_effect_58[], pl0e_effect_59[], pl0e_effect_60[], pl0e_effect_61[], pl0e_effect_62[],
	pl0e_effect_63[], pl0e_effect_64[], pl0e_effect_75[], pl0e_effect_75_loop[], pl0e_effect_76[],
	pl0e_effect_76_loop[], pl0e_effect_77[], pl0e_effect_77_loop[], pl0e_effect_78[],
	pl0e_effect_78_loop[], pl0e_effect_79[], pl0e_effect_79_loop[], pl0e_effect_80[],
	pl0e_effect_80_loop[], pl0e_effect_81[], pl0e_effect_81_loop[], pl0e_effect_82[],
	pl0e_effect_82_loop[], pl0e_effect_83[], pl0e_effect_83_loop[], pl0e_effect_84[],
	pl0e_effect_84_loop[], pl0e_effect_44[], pl0e_effect_85[], pl0e_effect_86[], pl0e_effect_87[],
	pl0e_effect_43[], pl0e_catch_05[], pl0e_change_00[], pl0e_change_00_loop[], pl0e_unused_01[],
	pl0e_change_01[], pl0e_unused_02[], pl0e_change_03[], pl0e_change_03_loop[], pl0e_change_02[],
	pl0e_unused_03[], pl0e_unused_04[], pl0e_caught_00[], pl0e_caught_01[], pl0e_caught_02[],
	pl0e_caught_03[], pl0e_caught_04[], pl0e_caught_05[], pl0e_caught_06[], pl0e_caught_07[],
	pl0e_caught_08[], pl0e_caught_09[], pl0e_caught_10[], pl0e_caught_11[], pl0e_caught_12[],
	pl0e_caught_13[], pl0e_caught_14[], pl0e_caught_15[], pl0e_caught_16[], pl0e_caught_17[],
	pl0e_caught_18[], pl0e_caught_19[], pl0e_caught_20[], pl0e_caught_21[], pl0e_unused_05[];
extern const Sint32 *const pl0e_stand_tbl[], *const pl0e_jump_tbl[], *const pl0e_dash_tbl[],
	*const pl0e_crouch_tbl[], *const pl0e_turn_tbl[], *const pl0e_guard_tbl[],
	*const pl0e_air_guard_tbl[], *const pl0e_stand_p_tbl[], *const pl0e_stand_k_tbl[],
	*const pl0e_crouch_p_tbl[], *const pl0e_crouch_k_tbl[], *const pl0e_jump_p_tbl[],
	*const pl0e_jump_k_tbl[], *const pl0e_damage_tbl[], *const pl0e_dizzy_tbl[],
	*const pl0e_catch_tbl[], *const pl0e_caught_tbl[], *const pl0e_get_up_tbl[],
	*const pl0e_appear_tbl[], *const pl0e_win_tbl[], *const pl0e_extra_tbl[],
	*const pl0e_special_tbl[], *const pl0e_super_tbl[], *const pl0e_effect_tbl[],
	*const pl0e_change_tbl[], *const pl0e_grp25_tbl[], *const pl0e_fly_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_0e[16] = {
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
const Sint32 *const *const char_tbl_0e[] = {
	pl0e_stand_tbl, pl0e_jump_tbl, pl0e_dash_tbl, pl0e_crouch_tbl, pl0e_turn_tbl, pl0e_guard_tbl,
	pl0e_air_guard_tbl, pl0e_stand_p_tbl, pl0e_stand_k_tbl, pl0e_crouch_p_tbl, pl0e_crouch_k_tbl, pl0e_jump_p_tbl,
	pl0e_jump_k_tbl, pl0e_damage_tbl, pl0e_dizzy_tbl, pl0e_catch_tbl, pl0e_caught_tbl, pl0e_get_up_tbl,
	pl0e_appear_tbl, pl0e_win_tbl, pl0e_extra_tbl, pl0e_special_tbl, pl0e_super_tbl, pl0e_effect_tbl,
	pl0e_change_tbl, pl0e_grp25_tbl, pl0e_fly_tbl, 0,
};

const Sint32 *const pl0e_stand_tbl[] = {
	pl0e_stand_00, pl0e_stand_00, pl0e_stand_02, pl0e_stand_03, pl0e_stand_00, pl0e_stand_00,
	pl0e_stand_06, pl0e_stand_07, pl0e_stand_00, pl0e_stand_00, 0,
};

const Sint32 *const pl0e_jump_tbl[] = {
	pl0e_jump_00, pl0e_jump_01, pl0e_jump_02, pl0e_jump_03, pl0e_jump_03, pl0e_jump_03,
	pl0e_jump_06, pl0e_jump_07, pl0e_jump_08, pl0e_jump_09, pl0e_jump_09, pl0e_jump_09,
	0,
};

const Sint32 *const pl0e_dash_tbl[] = {
	pl0e_dash_00, pl0e_dash_01, pl0e_dash_02, pl0e_dash_03, 0,
};

const Sint32 *const pl0e_crouch_tbl[] = {
	pl0e_crouch_00, pl0e_crouch_01, pl0e_crouch_02, 0,
};

const Sint32 *const pl0e_turn_tbl[] = {
	pl0e_turn_00, pl0e_turn_01, 0,
};

const Sint32 *const pl0e_guard_tbl[] = {
	pl0e_guard_00, pl0e_guard_01, pl0e_guard_02, pl0e_guard_01, pl0e_guard_04, pl0e_guard_05,
	pl0e_guard_06, pl0e_guard_05, 0,
};

const Sint32 *const pl0e_air_guard_tbl[] = {
	pl0e_air_guard_00, pl0e_air_guard_01, pl0e_air_guard_01, 0,
};

const Sint32 *const pl0e_stand_p_tbl[] = {
	pl0e_stand_p_00, pl0e_stand_p_01, pl0e_stand_p_02, pl0e_stand_p_03, pl0e_stand_p_01, pl0e_stand_p_02,
	0,
};

const Sint32 *const pl0e_stand_k_tbl[] = {
	pl0e_stand_k_00, pl0e_stand_k_01, pl0e_stand_k_02, pl0e_stand_k_03, pl0e_stand_k_01, pl0e_stand_k_02,
	0,
};

const Sint32 *const pl0e_crouch_p_tbl[] = {
	pl0e_crouch_p_00, pl0e_crouch_p_01, pl0e_crouch_p_02, pl0e_crouch_p_00, pl0e_crouch_p_01, pl0e_crouch_p_02,
	0,
};

const Sint32 *const pl0e_crouch_k_tbl[] = {
	pl0e_crouch_k_00, pl0e_crouch_k_01, pl0e_crouch_k_02, pl0e_crouch_k_00, pl0e_crouch_k_01, pl0e_crouch_k_02,
	0,
};

const Sint32 *const pl0e_jump_p_tbl[] = {
	pl0e_jump_p_00, pl0e_jump_p_01, pl0e_jump_p_02, pl0e_jump_p_00, pl0e_jump_p_01, pl0e_jump_p_02,
	0,
};

const Sint32 *const pl0e_jump_k_tbl[] = {
	pl0e_jump_k_00, pl0e_jump_k_01, pl0e_jump_k_02, pl0e_jump_k_00, pl0e_jump_k_01, pl0e_jump_k_02,
	0,
};

const Sint32 *const pl0e_damage_tbl[] = {
	pl0e_damage_00, pl0e_damage_01, pl0e_damage_02, pl0e_damage_03, pl0e_damage_04, pl0e_damage_05,
	pl0e_damage_06, pl0e_damage_07, pl0e_damage_08, pl0e_damage_09, pl0e_damage_10, pl0e_damage_11,
	pl0e_damage_12, pl0e_damage_13, pl0e_damage_13, pl0e_damage_13, pl0e_damage_16, pl0e_damage_17,
	pl0e_damage_18, pl0e_damage_19, pl0e_damage_20, pl0e_damage_21, pl0e_damage_22, pl0e_damage_23,
	pl0e_damage_24, pl0e_damage_25, pl0e_damage_26, pl0e_damage_27, pl0e_damage_28, pl0e_damage_29,
	pl0e_damage_30, pl0e_damage_31, pl0e_damage_30, pl0e_damage_33, pl0e_damage_13, 0,
};

const Sint32 *const pl0e_dizzy_tbl[] = {
	pl0e_dizzy_00, 0,
};

const Sint32 *const pl0e_catch_tbl[] = {
	pl0e_catch_00, pl0e_catch_01, pl0e_catch_02, pl0e_catch_03, pl0e_catch_04, pl0e_catch_05,
	pl0e_catch_06, 0,
};

const Sint32 *const pl0e_caught_tbl[] = {
	pl0e_caught_00, pl0e_caught_01, pl0e_caught_02, pl0e_caught_03, pl0e_caught_04, pl0e_caught_05,
	pl0e_caught_06, pl0e_caught_07, pl0e_caught_08, pl0e_caught_09, pl0e_caught_10, pl0e_caught_11,
	pl0e_caught_12, pl0e_caught_13, pl0e_caught_14, pl0e_caught_15, pl0e_caught_16, pl0e_caught_17,
	pl0e_caught_18, pl0e_caught_19, pl0e_caught_20, pl0e_caught_21, 0,
};

const Sint32 *const pl0e_get_up_tbl[] = {
	pl0e_get_up_00, pl0e_get_up_01, pl0e_get_up_02, pl0e_get_up_03, 0,
};

const Sint32 *const pl0e_appear_tbl[] = {
	pl0e_appear_00, 0,
};

const Sint32 *const pl0e_win_tbl[] = {
	pl0e_win_00, pl0e_win_01, pl0e_win_02, 0,
};

const Sint32 *const pl0e_extra_tbl[] = {
	pl0e_extra_00, pl0e_extra_01, pl0e_extra_02, pl0e_extra_03, pl0e_extra_04, 0,
};

const Sint32 *const pl0e_special_tbl[] = {
	pl0e_special_00, pl0e_special_01, pl0e_special_02, pl0e_special_03, pl0e_special_04, pl0e_special_05,
	pl0e_special_06, pl0e_special_07, pl0e_special_08, pl0e_special_09, pl0e_special_10, pl0e_catch_06,
	pl0e_special_12, pl0e_special_13, pl0e_special_14, pl0e_special_15, pl0e_special_16, pl0e_special_17,
	0,
};

const Sint32 *const pl0e_super_tbl[] = {
	pl0e_super_00, pl0e_super_01, pl0e_super_02, pl0e_super_03, pl0e_super_04, pl0e_super_05,
	pl0e_super_06, pl0e_super_07, pl0e_super_08, pl0e_super_09, 0,
};

const Sint32 *const pl0e_effect_tbl[] = {
	pl0e_effect_00, pl0e_effect_01, pl0e_effect_02, pl0e_effect_03, pl0e_effect_04, pl0e_effect_05,
	pl0e_effect_06, pl0e_effect_07, pl0e_effect_08, pl0e_effect_09, pl0e_effect_10, pl0e_effect_11,
	pl0e_effect_12, pl0e_effect_13, pl0e_effect_14, pl0e_effect_15, pl0e_effect_16, pl0e_effect_17,
	pl0e_effect_18, pl0e_effect_19, pl0e_effect_20, pl0e_effect_21, pl0e_effect_22, pl0e_effect_23,
	pl0e_effect_24, pl0e_effect_25, pl0e_effect_26, pl0e_effect_27, pl0e_effect_28, pl0e_effect_29,
	pl0e_effect_30, pl0e_effect_31, pl0e_effect_32, pl0e_effect_33, pl0e_effect_34, pl0e_effect_35,
	pl0e_effect_36, pl0e_effect_37, pl0e_effect_38, pl0e_effect_39, pl0e_effect_40, pl0e_effect_40,
	pl0e_catch_05, pl0e_effect_43, pl0e_effect_44, pl0e_effect_45, pl0e_effect_46, pl0e_effect_47,
	pl0e_effect_48, pl0e_effect_49, pl0e_effect_50, pl0e_effect_51, pl0e_effect_52, pl0e_effect_53,
	pl0e_effect_54, pl0e_effect_55, pl0e_effect_56, pl0e_effect_57, pl0e_effect_58, pl0e_effect_59,
	pl0e_effect_60, pl0e_effect_61, pl0e_effect_62, pl0e_effect_63, pl0e_effect_64, pl0e_effect_65,
	pl0e_effect_66, pl0e_effect_67, pl0e_effect_68, pl0e_effect_69, pl0e_effect_70, pl0e_effect_71,
	pl0e_effect_72, pl0e_effect_73, pl0e_effect_74, pl0e_effect_75, pl0e_effect_76, pl0e_effect_77,
	pl0e_effect_78, pl0e_effect_79, pl0e_effect_80, pl0e_effect_81, pl0e_effect_82, pl0e_effect_83,
	pl0e_effect_84, pl0e_effect_85, pl0e_effect_86, pl0e_effect_87, pl0e_effect_88, 0,
};

const Sint32 *const pl0e_change_tbl[] = {
	pl0e_change_00, pl0e_change_01, pl0e_change_02, pl0e_change_03, pl0e_change_04, 0,
};

const Sint32 *const pl0e_grp25_tbl[] = {
	pl0e_stand_00, 0,
};

const Sint32 *const pl0e_fly_tbl[] = {
	pl0e_stand_00, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow        atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: pls0e_04_hold, pl0e_appear, act_stand, appear_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl0e_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    73, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    74, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    75, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    77, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    80, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    70, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    72, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl0e_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    92, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    93, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    94, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    95, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    96, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    97, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    98, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,    99, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   100, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl0e_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl0e_stand_03[] = {
	CG_FRAME(    0,    0,    6,           0,   101, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   102, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   103, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   104, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   105, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   106, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   107, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   108, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6,           0,   109, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   110, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl0e_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl0e_stand_06[] = {
	CG_FRAME(    0,    0,    7,           0,    91, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,    92, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl0e_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl0e_stand_07[] = {
	CG_FRAME(    0,    0,    7,           0,   104, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   105, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl0e_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl0e_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   120, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   123, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0e_jump_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl0e_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,   120, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   131, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0e_jump_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl0e_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   120, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0e_jump_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl0e_jump_03[] = {
	CG_FRAME(    0,    0,    2,           0,   120, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   137, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl0e_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,   120, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   123, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0e_jump_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl0e_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   120, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   123, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0e_jump_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl0e_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   120, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   123, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0e_jump_08_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_jump_08_loop ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl0e_jump_09[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl0e_dash_00[] = {
	CG_FRAME(    0,    1,    4,           0,   119, 0x80,   0x2a,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0e_dash_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   113, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   114, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   115, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   116, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   117, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   111, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   112, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl0e_dash_00_loop ),
};

/* Char_Dash 2 */
const Sint32 pl0e_dash_02[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    70, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl0e_dash_01[] = {
	CG_FRAME(    0,    1,    4,           0,   119, 0x80,   0x2a,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0e_dash_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   117, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   116, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   115, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   114, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   113, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   112, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   111, 0x80,   0xac,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl0e_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl0e_dash_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    70, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl0e_crouch_00[] = {
	CG_FRAME(    0,    1,    4,           0,   136, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   137, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0e_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl0e_crouch_01[] = {
	CG_FRAME(    0,    0,    6,           0,    83, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    84, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    85, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    87, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    88, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    89, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    90, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0e_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl0e_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   137, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   136, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl0e_turn_00[] = {
	CG_FRAME(    0,    0,    6,           0,   138, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   139, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl0e_turn_01[] = {
	CG_FRAME(    0,    0,    6,           0,   140, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   141, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0e_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl0e_guard_00[] = {
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl0e_guard_01[] = {
	CG_FRAME( 0x80,    0,    2,           0,   143, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_FRAME( 0x80,    0,    2,           0,   144, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_FRAME( 0x80,    0,    2,           0,   145, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_FRAME( 0x80,    0,    2,           0,   146, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_FRAME( 0x80,    0,    2,           0,   147, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_FRAME( 0x80,    0,    2,           0,   148, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   149, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_NEXT( pl0e_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl0e_guard_02[] = {
	CG_FRAME(    0,    0,    3,           0,   150, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0, 161 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0x80,   0x95,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   142, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl0e_guard_04[] = {
	CG_FRAME(    0,    0,    3,           0,   152, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl0e_guard_05[] = {
	CG_FRAME( 0x80,    0,    2,           0,   153, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   154, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   155, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   156, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   157, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   158, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   159, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0e_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl0e_guard_06[] = {
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   152, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0e_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl0e_air_guard_00[] = {
	CG_FRAME(    0,    0,    3,           0,   162, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl0e_air_guard_01[] = {
	CG_FRAME( 0x80,    0,    2,           0,   163, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   164, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   165, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   166, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   167, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   168, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   169, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl0e_stand_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   237, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   238, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl0e_stand_p_03[] = {
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   237, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   238, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_p_03 ),
};

/* Char_Stand_P 1 (medium)
   Char_Stand_P 4 */
const Sint32 pl0e_stand_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   239, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x80,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x00, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   243, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   12,           0,   244, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   245, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy)
   Char_Stand_P 5 */
const Sint32 pl0e_stand_p_02[] = {
	CG_FRAME(    0,    0,   10,           0,   246, 0x80,   0x6d,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   247, 0x80,   0x6d,        ATK,  0x00,       0x05,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,    5,           0,   248, 0x80,   0x6d, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,    3,           0,   249, 0x80,   0x6d,        ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   16,           0,   250, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   251, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    5,           0,   252, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   139, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl0e_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   253, 0x80,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   254, 0x80,   0x12, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    1,    8,           0,   254, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   139, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_k_03 ),
};

/* Char_Stand_K 3 */
const Sint32 pl0e_stand_k_03[] = {
	CG_FRAME(    0,    0,    6,           0,   253, 0x80,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   254, 0x80,   0x12, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    8,           0,   254, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   139, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_k_03 ),
};

/* Char_Stand_K 1 (medium)
   Char_Stand_K 4 */
const Sint32 pl0e_stand_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   255, 0x80,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   256, 0x80,   0x12,        ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   257, 0x80,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   258, 0x80,   0x12, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    5,           0,   259, 0x80,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x12,        ATK,  0x01,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    7,           0,   261, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    7,           0,   262, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   263, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy)
   Char_Stand_K 5 */
const Sint32 pl0e_stand_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   264, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   265, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    5,           0,   266, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    2,           0,   267, 0x80,   0x00,        ATK,  0x00,       0x05,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,    2,           0,   268, 0x80,   0x00,        ATK,  0x00,       0x21,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    3,           0,   269, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    3,           0,   270, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,    3,           0,   271, 0x80,   0x00, ATK | 0x1d,  0x01,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    3,           0,   272, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    2,           0,   273, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   274, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   275, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   276, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,   277, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    4,           0,   278, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    5,           0,   279, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   264, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   139, 0x80,   0x52,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_stand_k_02 ),
};

/* Char_Crouch_P 0 (light)
   Char_Crouch_P 3 */
const Sint32 pl0e_crouch_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   280, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   281, 0x80,   0x00, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    4,           0,   281, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   282, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0e_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium)
   Char_Crouch_P 4 */
const Sint32 pl0e_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   283, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x00,        ATK,  0x00,       0x05,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    3,           0,   285, 0x80,   0x00, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,    2,           0,   286, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6,           0,   286, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   287, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   288, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   282, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0e_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy)
   Char_Crouch_P 5 */
const Sint32 pl0e_crouch_p_02[] = {
	CG_FRAME(    0,    0,    7,           0,   289, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   290, 0x80,   0x00,        ATK,  0x00,       0x05,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,    5,           0,   291, 0x80,   0x00, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,    3,           0,   292, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   16,           0,   293, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    6,           0,   294, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4,           0,   295, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    83, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0e_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light)
   Char_Crouch_K 3 */
const Sint32 pl0e_crouch_k_00[] = {
	CG_FRAME(    0,    0,    1,           0,   280, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   296, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    3,           0,   297, 0x80,   0x00, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    1,    8,           0,   297, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   141, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0e_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium)
   Char_Crouch_K 4 */
const Sint32 pl0e_crouch_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   298, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   299, 0x80,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   300, 0x80,   0x00, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    8,           0,   300, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    6,           0,   301, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   298, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0e_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy)
   Char_Crouch_K 5 */
const Sint32 pl0e_crouch_k_02[] = {
	CG_FRAME(    0,    0,    1,           0,   302, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   303, 0x80,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   304, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   306, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   307, 0x50,   0x00, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   308, 0x50,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   309, 0x50,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   310, 0x50,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    3,           0,   311, 0x50,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    3,           0,   312, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   313, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   314, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   302, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0e_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl0e_jump_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   10,           0,   316, 0x80,   0x20, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   126, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl0e_jump_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   318, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   319, 0x80,   0x20,        ATK,  0x00,       0x05,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    5,           0,   320, 0x80,   0x20, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3,           0,   321, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    5,           0,   322, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   126, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl0e_jump_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x20,        ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   325, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    3,           0,   326, 0x80,   0x20, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    3,           0,   327, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x20,        ATK,  0x1b,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    3,           0,   329, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    5,           0,   330, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   325, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   126, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl0e_jump_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   331, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    9,           0,   332, 0x80,   0x20, ATK | 0x1f,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   331, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   333, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl0e_jump_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   334, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x20,        ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    9,           0,   336, 0x80,   0x20, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    5,           0,   337, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   334, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   126, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl0e_jump_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   338, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   339, 0x80,   0x20,        ATK,  0x00,       0x05,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   340, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   343, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   344, 0x80,   0x20, ATK | 0x1d,  0x1c,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   345, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   346, 0x80,   0x20, ATK | 0x1d,  0x1d,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   347, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0,   348, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0,   349, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0,   350, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   351, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   352, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   353, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   338, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl0e_damage_00[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   539, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0e_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl0e_damage_01[] = {
	CG_FRAME(    0,    0,    2,           0,   539, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   12,           0,   540, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   539, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0e_damage_01 ),
};

/* Char_Damage 2: shl0e_01_catch */
const Sint32 pl0e_damage_02[] = {
	CG_FRAME(    0,    0,    2,           0,   540, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   14,           0,   541, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    5,           0,   540, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   539, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0e_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl0e_damage_03[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   545, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0e_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl0e_damage_04[] = {
	CG_FRAME(    0,    0,    2,           0,   545, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   12,           0,   546, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   545, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0e_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl0e_damage_05[] = {
	CG_FRAME(    0,    0,    2,           0,   546, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   14,           0,   547, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    5,           0,   546, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   545, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0e_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl0e_damage_06[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   551, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0e_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl0e_damage_07[] = {
	CG_FRAME(    0,    0,    2,           0,   551, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   12,           0,   552, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   551, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0e_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl0e_damage_08[] = {
	CG_FRAME(    0,    0,    2,           0,   552, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   14,           0,   553, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    5,           0,   552, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   551, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0e_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl0e_damage_09[] = {
	CG_FRAME(    0,    0,    6,           0,   566, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   567, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   568, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   575, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   576, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    3,           0,   577, 0x80,   0x8e,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
};

const Sint32 pl0e_damage_09_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   579, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0e_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl0e_damage_10[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   557, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl0e_damage_11[] = {
	CG_FRAME(    0,    0,    2,           0,   557, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   558, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl0e_damage_12[] = {
	CG_FRAME(    0,    0,    2,           0,   558, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   559, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl0e_damage_13[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   565, 0x80,   0x5e,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl0e_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl0e_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   569, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   574, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl0e_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   570, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl0e_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   571, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl0e_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   572, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl0e_damage_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   573, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl0e_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   569, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   574, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
};

/* Char_Damage 22 */
const Sint32 pl0e_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   570, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
};

/* Char_Damage 23 */
const Sint32 pl0e_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   571, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
};

/* Char_Damage 24 */
const Sint32 pl0e_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   572, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
};

/* Char_Damage 25 */
const Sint32 pl0e_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   573, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl0e_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   575, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2,           0,   576, 0x80,   0x5e,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2,           0,   577, 0x80,   0x5e,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2,           0,   580, 0x80,   0x5e,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2,           0,   578, 0x80,   0x5e,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   579, 0x80,   0x5e,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl0e_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl0e_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   602, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   604, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    4,           0,   603, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl0e_damage_27_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl0e_damage_27_loop ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl0e_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   585, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   586, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   587, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   588, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   589, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   590, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl0e_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   591, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   592, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   593, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   594, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   595, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   596, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl0e_damage_30[] = {
	CG_FRAME(    0,    0,    3,           0,   581, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    3,           0,   584, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    3,           0,   583, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   582, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0e_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl0e_damage_31[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   579, 0x80,   0x8d,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0e_damage_31 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl0e_damage_33[] = {
	CG_FRAME(    0,    0,    3,           0,   599, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   598, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   601, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   600, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   599, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   597, 0x80,   0xaf,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl0e_damage_33 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl0e_dizzy_00[] = {
	CG_FRAME(    0,    0,   12,           0,   563, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   564, 0x80,   0x52,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl0e_catch_00[] = {
	CG_FRAME(    0,    0,   12,           0,   355, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,    5,           0,   356, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,    5,           0,   357, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,    1,           0,   359, 0x80,   0x12,          0,  0x00,       0x1f,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,    1,           0,   360, 0x80,   0x12,          0,  0x00,       0x04,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,    1,           0,   361, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,    1,           0,   362, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    2,   12,           0,   358, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,    5,           0,   356, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   363, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_catch_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   364, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_catch_00_loop ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl0e_catch_01[] = {
	CG_FRAME(    0,    0,    6,           0,   365, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    6,           0,   366, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   10,           0,   367, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,    5,           0,   368, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,    4,           0,   369, 0x80,   0x6e,          0,  0x00,       0x04,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    1,    4,           0,   370, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   371, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   372, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_catch_01_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   373, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_catch_01_loop ),
};

/* Char_Catch 2: jump_p_set */
const Sint32 pl0e_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   443, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
};

const Sint32 pl0e_catch_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   444, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   445, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_NEXT( pl0e_catch_02_loop ),
};

/* Char_Catch 3: jump_p_move */
const Sint32 pl0e_catch_03[] = {
	CG_FRAME(    0,    0,    1,           0,   446, 0x80,   0xa5,          0,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   446, 0x80,   0xa5,          0,  0x00,       0x04,    0,   0,     0, 0x0019,   0 ),
	CG_NEXT( pl0e_catch_03 ),
};

/* Char_Catch 4: jump_p_move */
const Sint32 pl0e_catch_04[] = {
	CG_FRAME(    0,    0,    4,           0,   447, 0x80,   0xa5,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_catch_04_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_catch_04_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl0e_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   212, 0x80,   0x98,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   213, 0x80,   0x97,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   214, 0x80,   0x76,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   215, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   216, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl0e_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   211, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   212, 0x80,   0x98,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   213, 0x80,   0x97,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x76,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   215, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   216, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_get_up_01 ),
};

const Sint32 pl0e_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   217, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   219, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   220, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   212, 0x80,   0x98,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   213, 0x80,   0x97,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x76,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   215, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   216, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00002e6c,
};

/* Char_Get_Up 2 */
const Sint32 pl0e_get_up_02[] = {
	CG_FRAME(    0,    0,    5,           0,   217, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   218, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   219, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   220, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x98,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x97,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   214, 0x80,   0x76,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   215, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   216, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl0e_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   221, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   222, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   223, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   224, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   212, 0x80,   0x98,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   213, 0x80,   0x97,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x76,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   215, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   216, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_get_up_03 ),
};

/* Char_Appear 0: pl0e_appear */
const Sint32 pl0e_appear_00[] = {
	CG_FRAME(    0,    0,   64,           0,   191, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x2d,          0,  0x00,       0x0a,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,   32,           0,   201, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,   16,           0,   207, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    5,           0,   208, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   209, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   210, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_appear_00 ),
};

/* Char_Win 0 */
const Sint32 pl0e_win_00[] = {
	CG_FRAME(    0,    0,    6,           0,   210, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   209, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   208, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,   24,           0,   170, 0x80,   0x2d,          0,  0x00,       0x0f,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   173, 0x80,   0x2d,          0,  0x00,       0x10,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   174, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   175, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
};

const Sint32 pl0e_win_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   176, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_NEXT( pl0e_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl0e_win_01[] = {
	CG_FRAME(    0,    0,    6,           0,   210, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   209, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   208, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    7,           0,   177, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   180, 0x80,   0x2d,          0,  0x00,       0x11,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   181, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
};

const Sint32 pl0e_win_01_loop[] = {
	CG_FRAME(    0,    0,    5,           0,   182, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    5,           0,   183, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    5,           0,   184, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   185, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_NEXT( pl0e_win_01_loop ),
};

/* Char_Win 2: win_init */
const Sint32 pl0e_win_02[] = {
	CG_FRAME(    0,    0,    6,           0,   210, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   209, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   208, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   187, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   188, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    5,           0,   189, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
};

const Sint32 pl0e_win_02_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   190, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_NEXT( pl0e_win_02_loop ),
};

/* Char_Extra 2: chouhatsu_init (Taunt), fly_start5 */
const Sint32 pl0e_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   523, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   524, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   525, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   526, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   527, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   528, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   529, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   530, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   531, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    3,           0,   532, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    1,   20,           0,   533, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    2,    4,           0,   531, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   529, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   528, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   527, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   525, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 151 ),
	CG_FRAME(    0,    0,    4,           0,   524, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   523, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_extra_02 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl0e_change_04[] = {
	CG_FRAME(    0,    0,    3,           0,   120, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   210, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   209, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   229, 0x80,   0x14,          0,  0x00,       0x0e,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    4,           0,   230, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   229, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   230, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   229, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   230, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   209, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   210, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_change_04 ),
};

/* Char_Extra 3: swing_init, swing_move, fly_start5 */
const Sint32 pl0e_extra_03[] = {
	CG_FRAME(    1,   52,    8,           0,   225, 0x80,   0x20,       0x16,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   52,   16,           0,   225, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,   56,    2,           0,   226, 0x80,   0x20,          0,  0x00,       0x04,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,   56,   -1,           0,   226, 0x80,   0x20,       0x1d,  0x00,          0,    0,   0,     0,      0,  62 ),
};

const Sint32 pl0e_extra_03_loop[] = {
	CG_FRAME(    0,   48,   32, END_OF_DATA,   227, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_extra_03_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl0e_extra_00[] = {
	CG_FRAME(    0,    0,    1,           0,   332, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0, 143 ),
	CG_FRAME(    0,    0,    1,           0,   332, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0, 143 ),
};

const Sint32 pl0e_extra_00_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   332, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0, 143 ),
	CG_NEXT( pl0e_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl0e_extra_01[] = {
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    5,           0,   229, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    5,           0,   230, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    5,           0,   229, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    5,           0,   230, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    5,           0,   229, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    5,           0,   230, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    5,           0,   229, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    2,    5,           0,   230, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   210, 0x80,   0x14,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_extra_01 ),
};

/* Char_Extra 4: pl0e_upper */
const Sint32 pl0e_extra_04[] = {
	CG_FRAME(    0,    0,    5,           0,   239, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x80,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x00, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   243, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   12,           0,   244, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   245, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_extra_04 ),
};

/* Char_Special 0 */
const Sint32 pl0e_special_00[] = {
	CG_FRAME(    0,    0,    3,           0,   374, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    7,           0,   375, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x81,    0,    1,           0,   376, 0x80,   0x61,        ATK,  0x00,          0,    0,   0,     0,      0, 149 ),
	CG_FRAME(    1,    1,    4,           0,   376, 0x80,   0x61, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    2,    0,    4,           0,   377, 0x80,   0x61,        ATK,  0x29,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    3,    0,    4,           0,   378, 0x80,   0x65,        ATK,  0x2c,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    4,    0,    3,           0,   379, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    5,    0,    4,           0,   380, 0x80,   0x6f,        ATK,  0x2c,          0,    0,   0,     0,      0,  95 ),
};

const Sint32 pl0e_special_00_loop[] = {
	CG_FRAME(    5,    0,   -1, END_OF_DATA,   380, 0x80,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0e_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl0e_special_01[] = {
	CG_FRAME(    0,    0,    3,           0,   374, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   11,           0,   375, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x81,    0,    1,           0,   376, 0x80,   0x61,        ATK,  0x00,          0,    0,   0,     0,      0, 149 ),
	CG_FRAME(    1,    1,    6,           0,   376, 0x80,   0x61, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    2,    0,    6,           0,   377, 0x80,   0x61,        ATK,  0x29,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    3,    0,    6,           0,   378, 0x80,   0x65,        ATK,  0x2a,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    4,    0,    6,           0,   379, 0x80,   0x60,        ATK,  0x2b,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    5,    0,    6,           0,   380, 0x80,   0x6f,        ATK,  0x2c,          0,    0,   0,     0,      0,  95 ),
};

const Sint32 pl0e_special_01_loop[] = {
	CG_FRAME(    5,    0,   -1, END_OF_DATA,   380, 0x80,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0e_special_01_loop ),
};

/* Char_Special 2 */
const Sint32 pl0e_special_02[] = {
	CG_FRAME(    0,    0,    6,           0,   441, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x81,    0,    1,           0,   377, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 150 ),
	CG_FRAME(    2,    1,    1,           0,   377, 0x80,   0x00, ATK | 0x1d,  0x2e,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    3,    0,    4,           0,   378, 0x80,   0x00,        ATK,  0x2f,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    4,    0,    5,           0,   379, 0x80,   0x00,        ATK,  0x30,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    5,    0,    4,           0,   380, 0x80,   0x00,        ATK,  0x31,          0,    0,   0,     0,      0,  95 ),
};

const Sint32 pl0e_special_02_loop[] = {
	CG_FRAME(    5,    0,   -1, END_OF_DATA,   380, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0e_special_02_loop ),
};

/* Char_Special 3 */
const Sint32 pl0e_special_03[] = {
	CG_FRAME(    0,    0,    6,           0,   441, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x81,    0,    1,           0,   377, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 150 ),
	CG_FRAME(    2,    1,    6,           0,   377, 0x80,   0x00, ATK | 0x1d,  0x2e,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    3,    0,    6,           0,   378, 0x80,   0x00,        ATK,  0x2f,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    4,    0,    6,           0,   379, 0x80,   0x00,        ATK,  0x30,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    5,    0,    6,           0,   380, 0x80,   0x00,        ATK,  0x31,          0,    0,   0,     0,      0,  95 ),
};

const Sint32 pl0e_special_03_loop[] = {
	CG_FRAME(    5,    0,    6, END_OF_DATA,   380, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0e_special_03_loop ),
};

/* Char_Special 4: fang_move (Venom Fang) */
const Sint32 pl0e_special_04[] = {
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   382, 0x80,   0x54,        ATE,  0x00,       0x21,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    3,           0,   384, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   385, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   386, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    83, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0e_special_04 ),
};

/* Char_Special 5: webthrow_init (Web Throw, 5 + atck_level) */
const Sint32 pl0e_special_05[] = {
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x11,        ATK,  0x18,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x11,        ATK,  0x18,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   10,           0,   418, 0x80,   0x11,        ATK,  0x18,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    4,           0,   419, 0x80,   0x13,        ATK,  0x18,       0x16,    0,   0,     0,      0, 100 ),
	CG_FRAME(    1,    0,    2,           0,   420, 0x80,   0x6e,        ATK,  0x18,       0x1e,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2,           0,   421, 0x80,   0x6e,        ATK,  0x18,          0,    0,   0,     0,      0, 102 ),
};

const Sint32 pl0e_special_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   420, 0x80,   0x6e,        ATK,  0x18,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   421, 0x80,   0x6e,        ATK,  0x18,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl0e_special_05_loop ),
};

/* Char_Special 6 */
const Sint32 pl0e_special_06[] = {
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x11,        ATK,  0x1c,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x11,        ATK,  0x1c,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   10,           0,   418, 0x80,   0x11,        ATK,  0x1c,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    4,           0,   419, 0x80,   0x13,        ATK,  0x1c,       0x16,    0,   0,     0,      0, 100 ),
	CG_FRAME(    1,    4,    2,           0,   422, 0x80,   0x6e,        ATK,  0x1c,       0x1e,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    4,    2,           0,   423, 0x80,   0x6e,        ATK,  0x1c,          0,    0,   0,     0,      0, 104 ),
};

const Sint32 pl0e_special_06_loop[] = {
	CG_FRAME(    0,    4,    2,           0,   422, 0x80,   0x6e,        ATK,  0x1c,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    4,    2, END_OF_DATA,   423, 0x80,   0x6e,        ATK,  0x1c,          0,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl0e_special_06_loop ),
};

/* Char_Special 7: webthrow_wait (Web Throw, 7 + atck_level) */
const Sint32 pl0e_special_07[] = {
	CG_FRAME(    0,    0,    2,           0,   420, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2,           0,   421, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2,           0,   420, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2,           0,   421, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    2,           0,   420, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    4,           0,   421, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    7,           0,   426, 0x80,   0x13,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   427, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   210, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl0e_special_08[] = {
	CG_FRAME(    0,    0,    2,           0,   422, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    2,           0,   422, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    2,           0,   422, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    7,           0,   426, 0x80,   0x13,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   427, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   210, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_special_08 ),
};

/* Char_Special 9: webthrow_wait (Web Throw), webthrow_throw (Web Throw) */
const Sint32 pl0e_special_09[] = {
	CG_FRAME(    0,   12,   10,           0,   421, 0x80,   0x6e,          0,  0x18,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   24,    6,           0,   428, 0x80,   0x6e,          0,  0x18,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   28,    4,           0,   429, 0x80,   0x6e,          0,  0x1f,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   32,   16,           0,   430, 0x80,   0x6e,          0,  0x0b,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   32,    4,           0,   430, 0x80,   0x6e,          0,  0x0b, 0x16 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   36,    6,           0,   431, 0x80,   0x6e,          0,  0x0c,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   40,    4,           0,   432, 0x80,   0x6e,          0,  0x03,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   44,   16,           0,   433, 0x80,   0x6e,          0,  0x16,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   44,    4,           0,   433, 0x80,   0x6e,          0,  0x16, 0x16 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   12,    6, END_OF_DATA,   421, 0x80,   0x6e,          0,  0x18,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl0e_special_10[] = {
	CG_FRAME(    0,   16,    8,           0,   423, 0x80,   0x6e,          0,  0x1c,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   28,    4,           0,   429, 0x80,   0x6e,          0,  0x1f,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   32,   16,           0,   430, 0x80,   0x6e,          0,  0x0b,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   32,    4,           0,   430, 0x80,   0x6e,          0,  0x0b, 0x16 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   36,    3,           0,   431, 0x80,   0x6e,          0,  0x0c,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   40,    4,           0,   432, 0x80,   0x6e,          0,  0x03,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   44,   16,           0,   433, 0x80,   0x6e,          0,  0x16,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   44,    4,           0,   433, 0x80,   0x6e,          0,  0x16, 0x16 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   12,    4, END_OF_DATA,   421, 0x80,   0x6e,          0,  0x18,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_special_10 ),
};

/* Char_Catch 6
   Char_Special 11: webthrow_throw (Web Throw) */
const Sint32 pl0e_catch_06[] = {
	CG_FRAME(    0,    0,    1,           0,   434, 0x80,   0x6e,        ATK,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    6,           0,   434, 0x80,   0x6e,        ATK,  0x00,       0x04,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    5,           0,   435, 0x80,   0x6e,        ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    1,   18,           0,   436, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8,           0,   437, 0x80,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    7,           0,   438, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   439, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   440, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_catch_06 ),
};

/* Char_Special 12: rush_init (Venom Rush, 12 + atck_level) */
const Sint32 pl0e_special_12[] = {
	CG_FRAME(    0,    0,    3,           0,   508, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    5,           0,   509, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0,   510, 0x80,   0xff,        ATK,  0x00,       0x15,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA, 32780, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl0e_special_13_loop ),
};

/* Char_Special 13 */
const Sint32 pl0e_special_13[] = {
	CG_FRAME(    0,    0,    3,           0,   508, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    8,           0,   509, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    8,           0,   510, 0x80,   0xff,        ATK,  0x00,       0x15,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    2,           0, 32780, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
};

const Sint32 pl0e_special_13_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32780, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0, 32781, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0, 32782, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0, 32783, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0, 32785, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32786, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl0e_special_13_loop ),
};

/* Char_Special 14: rush_wait (Venom Rush, 14 + atck_level) */
const Sint32 pl0e_special_14[] = {
	CG_FRAME(    0,    0,    3,           0, 32787, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32788, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    5,           0,   520, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   521, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    73, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl0e_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl0e_special_15[] = {
	CG_FRAME(    0,    0,    3,           0, 32787, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32788, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    7,           0,   520, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    6,           0,   521, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    73, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl0e_special_15 ),
};

/* Char_Super 0: web_init (Venom Web (hyper)) */
const Sint32 pl0e_super_00[] = {
	CG_FRAME(    0,    1,    6,           0,   538, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   10,           0,   120, 0x80,   0x11,        ATK,  0x00,       0x17,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   123, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   124, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   -1,           0,   125, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   458, 0x80,   0x20,        ATK,  0x00,       0x18,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   459, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   460, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl0e_super_00_loop[] = {
	CG_FRAME(    0,    1,   16, END_OF_DATA,   461, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_super_00_loop ),
};

/* Char_Super 4 */
const Sint32 pl0e_super_04[] = {
	CG_FRAME(    0,    1,    8,           0,   538, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   18,           0,   120, 0x80,   0x11, ATK | 0x0b,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   123, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   124, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   -1,           0,   125, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   458, 0x80,   0x20, ATK | 0x0b,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   459, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   460, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl0e_super_04_loop[] = {
	CG_FRAME(    0,    1,   16, END_OF_DATA,   461, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_super_04_loop ),
};

/* Char_Super 1: web_aim (Venom Web (hyper)) */
const Sint32 pl0e_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   126, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl0e_super_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0e_super_01_loop ),
};

/* Char_Super 2: web_dive (Venom Web (hyper)) */
const Sint32 pl0e_super_02[] = {
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x20,        ATK,  0x00,       0x03,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    3,           0,   331, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   332, 0x80,   0x20,        ATK,  0x39,       0x03,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x20,        ATK,  0x38,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    3,           0,   335, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   336, 0x80,   0x20,        ATK,  0x39,       0x03,    0,   0,     0,      0, 109 ),
	CG_FRAME( 0x80,    0,    3,           0,   319, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   321, 0x80,   0x20,        ATK,  0x38,       0x03,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    4,           0,   338, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,        ATK,  0x39,       0x03,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    2,           0,   343, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    2,           0,   344, 0x80,   0x20,        ATK,  0x38,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    2,           0,   345, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    0,    2,           0,   346, 0x80,   0x20,        ATK,  0x3a,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    0,    2,           0,   347, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    0,    2,           0,   348, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,    2,           0,   349, 0x80,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,    2,           0,   350, 0x80,   0x20, ATK | 0x1d,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    1,           0,   125, 0x80,   0x00,        ATK,  0x3b,          0,    0,   0,     0,      0, 150 ),
	CG_FRAME(    3,    1,    5,           0,   378, 0x80,   0x00,        ATK,  0x3b,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    4,    0,    3,           0,   379, 0x80,   0x00,        ATK,  0x3c,          0,    0,   0,     0,      0,  94 ),
};

const Sint32 pl0e_super_02_loop[] = {
	CG_FRAME(    5,    0,    3, END_OF_DATA,   380, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0e_super_02_loop ),
};

/* Char_Super 3: web_fall (Venom Web (hyper)) */
const Sint32 pl0e_super_03[] = {
	CG_FRAME(    0,    0,    4,           0,   381, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   382, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   383, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4,           0,   384, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   386, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    83, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_super_03 ),
};

/* Char_Super 6: web_wait (Venom Web (hyper)), web_bite (Venom Web (hyper)) */
const Sint32 pl0e_super_06[] = {
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   128, 0x80,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_super_06 ),
};

/* Char_Super 5: web_end (Venom Web (hyper)) */
const Sint32 pl0e_super_05[] = {
	CG_FRAME(    0,    0,    2,           0,   120, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   137, 0x80,   0x54,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0e_super_05 ),
};

/* Char_Super 7: bite_init (Death Bite (hyper)) */
const Sint32 pl0e_super_07[] = {
	CG_FRAME(    0,    0,    6,           0,   508, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    6,           0,   509, 0x80,   0xff,        ATK,  0x00,       0x11,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    6,           0,   510, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0, 32780, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl0e_super_07_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32780, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    3,           0, 32781, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32782, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32783, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32784, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32785, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32786, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl0e_super_07_loop ),
};

/* Char_Super 8: bite_init (Death Bite (hyper)), bite_wait (Death Bite (hyper)) */
const Sint32 pl0e_super_08[] = {
	CG_FRAME(    0,    0,    3,           0, 32787, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32788, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    6,           0,   520, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    6,           0,   521, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    73, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl0e_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl0e_super_09[] = {
	CG_FRAME(    0,    0,    6,           0,   508, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   14,           0,   509, 0x80,   0xff, ATK | 0x0d,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    6,           0,   510, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    5,           0, 32780, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl0e_super_09_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32780, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    3,           0, 32781, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32782, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32783, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32784, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0, 32785, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32786, 0x80,   0xff,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl0e_super_09_loop ),
};

/* Char_Special 16: gamma_init */
const Sint32 pl0e_special_16[] = {
	CG_FRAME(    0,    0,    5,           0,   239, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x80,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x00, ATK | 0x1e,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   243, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   12,           0,   244, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   245, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_special_16 ),
};

/* Char_Special 17: snapback_init (Snapback) */
const Sint32 pl0e_special_17[] = {
	CG_FRAME(    0,    0,    8,           0,   246, 0x80,   0x6d,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   246, 0x80,   0x6d,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,    2,           0,   247, 0x80,   0x6d,        ATK,  0x00,       0x05,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,    5,           0,   248, 0x80,   0x6d,        ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,    3,           0,   249, 0x80,   0x6d,        ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   16,           0,   250, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   251, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    5,           0,   252, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   139, 0x80,   0x6d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_special_17 ),
};

/* Char_Effect 0 */
const Sint32 pl0e_effect_00[] = {
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   402, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl0e_effect_01[] = {
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   405, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl0e_effect_02[] = {
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   407, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   408, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl0e_effect_03[] = {
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   411, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl0e_effect_04[] = {
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   413, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   414, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl0e_effect_05[] = {
	CG_FRAME(    0,    0,    2,           0,   415, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   417, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl0e_effect_06[] = {
	CG_FRAME(    0,    0,    2,           0,   391, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   393, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl0e_effect_07[] = {
	CG_FRAME(    0,    0,    2,           0,   394, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   395, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   396, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl0e_effect_08[] = {
	CG_FRAME(    0,    0,    2,           0,   397, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   399, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_08 ),
};

/* Char_Effect 9: pls0e_00_ground_init */
const Sint32 pl0e_effect_09[] = {
	CG_FRAME(    0,    0,    3,           0,   387, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   388, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   390, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_09 ),
};

/* Char_Effect 10: shl0e_01_init (10 + type) */
const Sint32 pl0e_effect_10[] = {
	CG_FRAME(    0,    0,    2,           0,   448, 0x80,   0x00, ATK | 0x1a,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   449, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   450, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    6,           0, 32769, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl0e_effect_10_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32769, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_10_loop ),
};

/* Char_Effect 11 */
const Sint32 pl0e_effect_11[] = {
	CG_FRAME(    0,    0,    2,           0,   462, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   463, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   464, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32772, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_11_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32772, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_11_loop ),
};

/* Char_Effect 12 */
const Sint32 pl0e_effect_12[] = {
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32775, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_12_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32775, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_12_loop ),
};

/* Char_Effect 13 */
const Sint32 pl0e_effect_13[] = {
	CG_FRAME(    0,    0,    2,           0,   482, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32777, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32778, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_13_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32778, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_13_loop ),
};

/* Char_Effect 14: shl0e_01_move (14 + type) */
const Sint32 pl0e_effect_14[] = {
	CG_FRAME(    0,    0,    3,           0, 32770, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   454, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   455, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   456, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_14_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   457, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_14_loop ),
};

/* Char_Effect 15 */
const Sint32 pl0e_effect_15[] = {
	CG_FRAME(    0,    0,    3,           0, 32773, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   468, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   469, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   470, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_15_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   471, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_15_loop ),
};

/* Char_Effect 16 */
const Sint32 pl0e_effect_16[] = {
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   478, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   479, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   480, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_16_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   481, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_16_loop ),
};

/* Char_Effect 17 */
const Sint32 pl0e_effect_17[] = {
	CG_FRAME(    0,    0,    3,           0, 32779, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   488, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   489, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   490, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_17_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   491, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_17_loop ),
};

/* Char_Effect 18: shl0e_02_init */
const Sint32 pl0e_effect_18[] = {
	CG_FRAME(    0,    0,    2,           0,   492, 0x80,   0x00,        ATK,  0x3e,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   493, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,        ATK,  0x00,       0x21,    0,   0,     0,      0, 144 ),
	CG_FRAME(    0,    0,    2,           0,   495, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 145 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   497, 0x80,   0x00,        ATK,  0x3f,          0,    0,   0,     0,      0, 146 ),
	CG_FRAME(    0,    0,    2,           0,   498, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 147 ),
	CG_FRAME(    0,    0,    2,           0,   499, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   500, 0x80,   0x00,        ATK,  0x40,          0,    0,   0,     0,      0, 148 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_FRAME(    0,    0,    2,           0,   502, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 160 ),
	CG_FRAME(    0,    0,    2,           0,   503, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   504, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   505, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   506, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   507, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_18 ),
};

/* Char_Effect 20: shl0e_00_init */
const Sint32 pl0e_effect_20[] = {
	CG_FRAME(    0,    0,    2,           0,   492, 0x80,   0x00,        ATK,  0x41,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   493, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,        ATK,  0x00,       0x21,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,    2,           0,   495, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 154 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   497, 0x80,   0x00,        ATK,  0x42,          0,    0,   0,     0,      0, 155 ),
	CG_FRAME(    0,    0,    2,           0,   498, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 156 ),
	CG_FRAME(    0,    0,    2,           0,   499, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 157 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x80,   0x00,        ATK,  0x43,          0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,    2,           0,   502, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,    2,           0,   503, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   504, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   505, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   506, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   507, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_20 ),
};

/* Char_Effect 19: pls0e_01_init, pls0e_02_foot_init, pls0e_02_line_init */
const Sint32 pl0e_effect_19[] = {
	CG_FRAME(    0,    1,    6,           0, 32789, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_19_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32789, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_19_loop ),
};

/* Char_Effect 21 */
const Sint32 pl0e_effect_21[] = {
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x00,          0,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 22 */
const Sint32 pl0e_effect_22[] = {
	CG_FRAME(    0,    0,    1,           0,   232, 0x80,   0x00,          0,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 23 */
const Sint32 pl0e_effect_23[] = {
	CG_FRAME(    0,    0,    1,           0,   234, 0x80,   0x00,          0,  0x00,       0x0e,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 24 */
const Sint32 pl0e_effect_24[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   235, 0x80,   0x00,          0,  0x00,       0x0e,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_21 ),
};

/* Char_Effect 25 */
const Sint32 pl0e_effect_25[] = {
	CG_FRAME(    0,    0,    1,           0,   534, 0x80,   0x00,          0,  0x00,       0x0d,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 26 */
const Sint32 pl0e_effect_26[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   536, 0x80,   0x00,          0,  0x00,       0x0d,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_25 ),
};

/* Char_Effect 27 */
const Sint32 pl0e_effect_27[] = {
	CG_FRAME(    0,    0,    1,           0,   535, 0x80,   0x00,          0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 28 */
const Sint32 pl0e_effect_28[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   537, 0x80,   0x00,          0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_27 ),
};

/* Char_Effect 88 */
const Sint32 pl0e_effect_88[] = {
	CG_FRAME(    0,    0,    4,           0,   605, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   606, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_88 ),
};

/* Char_Effect 29: pls0e_04_grab_init (29 + sub_no) */
const Sint32 pl0e_effect_29[] = {
	CG_FRAME(    0,    0,    4,           0,   877, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   878, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   879, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   880, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   881, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   882, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_29_loop[] = {
	CG_FRAME(    0,    2, -128,           0,   883, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   878, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   879, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   880, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   881, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   882, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   883, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_29_loop ),
};

/* Char_Effect 30 */
const Sint32 pl0e_effect_30[] = {
	CG_FRAME(    0,    0,    4,           0,   884, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   885, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   886, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   887, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   888, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   889, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_30_loop[] = {
	CG_FRAME(    0,    2, -128,           0,   890, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   885, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   886, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   887, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   888, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   889, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   890, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_30_loop ),
};

/* Char_Effect 31 */
const Sint32 pl0e_effect_31[] = {
	CG_FRAME(    0,    0,    4,           0, 32814, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32815, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32816, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32817, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32818, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32819, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_31 ),
};

/* Char_Effect 32 */
const Sint32 pl0e_effect_32[] = {
	CG_FRAME(    0,    0,    4,           0, 32820, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32821, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32822, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32823, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32824, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32825, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_32 ),
};

/* Char_Effect 33 */
const Sint32 pl0e_effect_33[] = {
	CG_FRAME(    0,    0,    4,           0, 32826, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32827, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32828, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32829, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32830, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32831, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_33 ),
};

/* Char_Effect 34 */
const Sint32 pl0e_effect_34[] = {
	CG_FRAME(    0,    0,    4,           0, 32832, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32833, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32834, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32835, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32836, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32837, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_34 ),
};

/* Char_Effect 35 */
const Sint32 pl0e_effect_35[] = {
	CG_FRAME(    0,    0,    4,           0, 32790, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32791, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32792, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32795, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_35 ),
};

/* Char_Effect 36 */
const Sint32 pl0e_effect_36[] = {
	CG_FRAME(    0,    0,    4,           0, 32796, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32798, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32799, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32800, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32801, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_36 ),
};

/* Char_Effect 37 */
const Sint32 pl0e_effect_37[] = {
	CG_FRAME(    0,    0,    4,           0, 32802, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32803, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32804, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32805, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32806, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32807, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_37 ),
};

/* Char_Effect 38 */
const Sint32 pl0e_effect_38[] = {
	CG_FRAME(    0,    0,    4,           0, 32808, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32809, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32810, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32811, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32812, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32813, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_38 ),
};

/* Char_Effect 39: shl0e_03_init (39 + atck_level) */
const Sint32 pl0e_effect_39[] = {
	CG_FRAME(    0,    0,    1,           0,   704, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,    1,           0,   718, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    2,    1,           0,   725, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    3,    1,           0,   732, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    4,    2,           0,   739, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    5,    2,           0,   746, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    6,    2,           0,   753, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    7,    2,           0,   760, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    8,    2,           0,   767, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 120 ),
};

const Sint32 pl0e_effect_39_loop[] = {
	CG_FRAME(    0,    9,    2, END_OF_DATA,   774, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_NEXT( pl0e_effect_39_loop ),
};

/* Char_Effect 45 */
const Sint32 pl0e_effect_45[] = {
	CG_FRAME(    0,    0,    4,           0,   709, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   710, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl0e_effect_45 ),
};

/* Char_Effect 46 */
const Sint32 pl0e_effect_46[] = {
	CG_FRAME(    0,    1,    4,           0,   723, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   724, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_NEXT( pl0e_effect_46 ),
};

/* Char_Effect 47 */
const Sint32 pl0e_effect_47[] = {
	CG_FRAME(    0,    2,    4,           0,   730, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    2,    4, END_OF_DATA,   731, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_NEXT( pl0e_effect_47 ),
};

/* Char_Effect 48 */
const Sint32 pl0e_effect_48[] = {
	CG_FRAME(    0,    3,    4,           0,   737, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA,   738, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl0e_effect_48 ),
};

/* Char_Effect 49 */
const Sint32 pl0e_effect_49[] = {
	CG_FRAME(    0,    4,    4,           0,   744, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    4,    4, END_OF_DATA,   745, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl0e_effect_49 ),
};

/* Char_Effect 50 */
const Sint32 pl0e_effect_50[] = {
	CG_FRAME(    0,    5,    4,           0,   751, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    5,    4, END_OF_DATA,   752, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl0e_effect_50 ),
};

/* Char_Effect 51 */
const Sint32 pl0e_effect_51[] = {
	CG_FRAME(    0,    6,    4,           0,   758, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    6,    4, END_OF_DATA,   759, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_NEXT( pl0e_effect_51 ),
};

/* Char_Effect 52 */
const Sint32 pl0e_effect_52[] = {
	CG_FRAME(    0,    7,    4,           0,   765, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    7,    4, END_OF_DATA,   766, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl0e_effect_52 ),
};

/* Char_Effect 53 */
const Sint32 pl0e_effect_53[] = {
	CG_FRAME(    0,    8,    4,           0,   772, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    8,    4, END_OF_DATA,   773, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 120 ),
	CG_NEXT( pl0e_effect_53 ),
};

/* Char_Effect 54 */
const Sint32 pl0e_effect_54[] = {
	CG_FRAME(    0,    9,    4,           0,   779, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    9,    4, END_OF_DATA,   780, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_NEXT( pl0e_effect_54 ),
};

/* Char_Effect 65 */
const Sint32 pl0e_effect_65[] = {
	CG_FRAME(    0,    0,    4,           0,   705, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   706, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   707, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_65_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   708, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_65_loop ),
};

/* Char_Effect 66 */
const Sint32 pl0e_effect_66[] = {
	CG_FRAME(    0,    0,    4,           0,   719, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   720, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   721, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_66_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   722, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_66_loop ),
};

/* Char_Effect 67 */
const Sint32 pl0e_effect_67[] = {
	CG_FRAME(    0,    0,    4,           0,   726, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   727, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_67_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   729, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_67_loop ),
};

/* Char_Effect 68 */
const Sint32 pl0e_effect_68[] = {
	CG_FRAME(    0,    0,    4,           0,   733, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   734, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   735, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_68_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   736, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_68_loop ),
};

/* Char_Effect 69 */
const Sint32 pl0e_effect_69[] = {
	CG_FRAME(    0,    0,    4,           0,   740, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   741, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   742, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_69_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   743, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_69_loop ),
};

/* Char_Effect 70 */
const Sint32 pl0e_effect_70[] = {
	CG_FRAME(    0,    0,    4,           0,   747, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   748, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   749, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_70_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   750, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_70_loop ),
};

/* Char_Effect 71 */
const Sint32 pl0e_effect_71[] = {
	CG_FRAME(    0,    0,    4,           0,   754, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   755, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   756, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_71_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   757, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_71_loop ),
};

/* Char_Effect 72 */
const Sint32 pl0e_effect_72[] = {
	CG_FRAME(    0,    0,    4,           0,   761, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   762, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   763, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_72_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   764, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_72_loop ),
};

/* Char_Effect 73 */
const Sint32 pl0e_effect_73[] = {
	CG_FRAME(    0,    0,    4,           0,   768, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   769, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   770, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_73_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   771, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_73_loop ),
};

/* Char_Effect 74 */
const Sint32 pl0e_effect_74[] = {
	CG_FRAME(    0,    0,    4,           0,   775, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   776, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   777, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_74_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   778, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_74_loop ),
};

/* Char_Effect 40
   Char_Effect 41 */
const Sint32 pl0e_effect_40[] = {
	CG_FRAME(    0,    0,    1,           0,   781, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 122 ),
	CG_FRAME(    0,    1,    1,           0,   795, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    2,    1,           0,   802, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    0,    3,    1,           0,   809, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    4,    2,           0,   816, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_FRAME(    0,    5,    2,           0,   823, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    6,    2,           0,   830, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    7,    2,           0,   837, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    8,    2,           0,   844, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
};

const Sint32 pl0e_effect_40_loop[] = {
	CG_FRAME(    0,    9,    2, END_OF_DATA,   851, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_NEXT( pl0e_effect_40_loop ),
};

/* Char_Effect 55 */
const Sint32 pl0e_effect_55[] = {
	CG_FRAME(    0,    0,    4,           0,   786, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 122 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   787, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 122 ),
	CG_NEXT( pl0e_effect_55 ),
};

/* Char_Effect 56 */
const Sint32 pl0e_effect_56[] = {
	CG_FRAME(    0,    1,    4,           0,   800, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   801, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_NEXT( pl0e_effect_56 ),
};

/* Char_Effect 57 */
const Sint32 pl0e_effect_57[] = {
	CG_FRAME(    0,    2,    4,           0,   807, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    0,    2,    4, END_OF_DATA,   808, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_NEXT( pl0e_effect_57 ),
};

/* Char_Effect 58 */
const Sint32 pl0e_effect_58[] = {
	CG_FRAME(    0,    3,    4,           0,   814, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA,   815, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_NEXT( pl0e_effect_58 ),
};

/* Char_Effect 59 */
const Sint32 pl0e_effect_59[] = {
	CG_FRAME(    0,    4,    4,           0,   821, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_FRAME(    0,    4,    4, END_OF_DATA,   822, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl0e_effect_59 ),
};

/* Char_Effect 60 */
const Sint32 pl0e_effect_60[] = {
	CG_FRAME(    0,    5,    4,           0,   828, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    5,    4, END_OF_DATA,   829, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl0e_effect_60 ),
};

/* Char_Effect 61 */
const Sint32 pl0e_effect_61[] = {
	CG_FRAME(    0,    6,    4,           0,   835, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    6,    4, END_OF_DATA,   836, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_NEXT( pl0e_effect_61 ),
};

/* Char_Effect 62 */
const Sint32 pl0e_effect_62[] = {
	CG_FRAME(    0,    7,    4,           0,   842, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    7,    4, END_OF_DATA,   843, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl0e_effect_62 ),
};

/* Char_Effect 63 */
const Sint32 pl0e_effect_63[] = {
	CG_FRAME(    0,    8,    4,           0,   849, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    8,    4, END_OF_DATA,   850, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl0e_effect_63 ),
};

/* Char_Effect 64 */
const Sint32 pl0e_effect_64[] = {
	CG_FRAME(    0,    9,    4,           0,   856, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    9,    4, END_OF_DATA,   857, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_NEXT( pl0e_effect_64 ),
};

/* Char_Effect 75: pls0e_04_init1 */
const Sint32 pl0e_effect_75[] = {
	CG_FRAME(    0,    0,    4,           0,   782, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   783, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   784, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_75_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   785, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_75_loop ),
};

/* Char_Effect 76 */
const Sint32 pl0e_effect_76[] = {
	CG_FRAME(    0,    0,    4,           0,   796, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   797, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   798, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_76_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   799, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_76_loop ),
};

/* Char_Effect 77 */
const Sint32 pl0e_effect_77[] = {
	CG_FRAME(    0,    0,    4,           0,   803, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   804, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   805, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_77_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   806, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_77_loop ),
};

/* Char_Effect 78 */
const Sint32 pl0e_effect_78[] = {
	CG_FRAME(    0,    0,    4,           0,   810, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   811, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   812, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_78_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   813, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_78_loop ),
};

/* Char_Effect 79 */
const Sint32 pl0e_effect_79[] = {
	CG_FRAME(    0,    0,    4,           0,   817, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   818, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   819, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_79_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   820, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_79_loop ),
};

/* Char_Effect 80 */
const Sint32 pl0e_effect_80[] = {
	CG_FRAME(    0,    0,    4,           0,   824, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   825, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   826, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_80_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   827, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_80_loop ),
};

/* Char_Effect 81 */
const Sint32 pl0e_effect_81[] = {
	CG_FRAME(    0,    0,    4,           0,   831, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   832, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   833, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_81_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   834, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_81_loop ),
};

/* Char_Effect 82 */
const Sint32 pl0e_effect_82[] = {
	CG_FRAME(    0,    0,    4,           0,   838, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   839, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   840, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_82_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   841, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_82_loop ),
};

/* Char_Effect 83 */
const Sint32 pl0e_effect_83[] = {
	CG_FRAME(    0,    0,    4,           0,   845, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   846, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   847, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_83_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   848, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_83_loop ),
};

/* Char_Effect 84 */
const Sint32 pl0e_effect_84[] = {
	CG_FRAME(    0,    0,    4,           0,   852, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   853, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   854, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0e_effect_84_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   855, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_84_loop ),
};

/* Char_Effect 44: pls0e_06_webthrow_init */
const Sint32 pl0e_effect_44[] = {
	CG_FRAME(    0,    0,    3,           0,   711, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   712, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   713, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   714, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   715, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   716, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   717, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_44 ),
};

/* Char_Effect 85 */
const Sint32 pl0e_effect_85[] = {
	CG_FRAME(    0,    0,    1,           0,   615, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   616, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   617, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   618, 0x00,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   619, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   620, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   621, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   622, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   623, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   624, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   625, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   626, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   627, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   628, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   629, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   630, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   631, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   632, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   633, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   634, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   635, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   636, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   637, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   638, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   607, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   608, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   609, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   610, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   611, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   612, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   613, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   614, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_85 ),
};

/* Char_Effect 86 */
const Sint32 pl0e_effect_86[] = {
	CG_FRAME(    0,    0,    1,           0,   647, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   648, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   649, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   650, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   651, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   652, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   653, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   654, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   655, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   656, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   657, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   658, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   659, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   660, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   661, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   662, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   663, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   664, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   665, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   666, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   667, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   668, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   669, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   670, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   639, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   640, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   641, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   642, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   643, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   644, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   645, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   646, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_86 ),
};

/* Char_Effect 87 */
const Sint32 pl0e_effect_87[] = {
	CG_FRAME(    0,    0,    1,           0,   679, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   680, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   681, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   682, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   683, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   684, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   685, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   686, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   687, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   688, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   689, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   690, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   691, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   692, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   693, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   694, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   695, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   696, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   697, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   698, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   699, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   700, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   701, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   702, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   671, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   672, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   673, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   674, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   675, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   676, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   677, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   678, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_87 ),
};

/* Char_Effect 43: pls0e_05_char_set */
const Sint32 pl0e_effect_43[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   863, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0e_effect_43 ),
};

/* Char_Catch 5: pls0e_03_anim
   Char_Effect 42 */
const Sint32 pl0e_catch_05[] = {
	CG_FRAME(    0,    0,    4,           0,   860, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   861, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    4,           0,   862, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,    4,           0,   870, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,    4,           0,   869, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,    4,           0,   868, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    4,           0,   858, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   859, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_NEXT( pl0e_catch_05 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl0e_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x11,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    8,           0,   228, 0x80,   0x13,          0,  0x00,       0x0e,    0,   0,     0,      0, 142 ),
};

const Sint32 pl0e_change_00_loop[] = {
	CG_FRAME(    0,    0,   26, END_OF_DATA,   228, 0x80,   0x13,          0,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl0e_change_00_loop ),
};

const Sint32 pl0e_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   208, 0x80,   0x13,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   22,           0,   170, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   120, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000070e8,
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl0e_change_01[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   142, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0e_change_01 ),
};

const Sint32 pl0e_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   229, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   230, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00007154,
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl0e_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x80,   0x11,          0,  0x00,       0x11,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   209, 0x80,   0x13,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0e_change_03_loop[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   233, 0x80,   0x14,          0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_NEXT( pl0e_change_03_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl0e_change_02[] = {
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   133, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0e_change_02 ),
};

const Sint32 pl0e_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x11,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   228, 0x80,   0x13,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   228, 0x80,   0x14,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x0000723c,
};

const Sint32 pl0e_unused_04[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   210, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   228, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   228, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00007290,
};

/* Char_Caught 0 */
const Sint32 pl0e_caught_00[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   542, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl0e_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl0e_caught_01[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   543, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl0e_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl0e_caught_02[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   544, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl0e_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl0e_caught_03[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   548, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl0e_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl0e_caught_04[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   549, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0e_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl0e_caught_05[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   550, 0x80,   0x11,          0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl0e_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl0e_caught_06[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   554, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl0e_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl0e_caught_07[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   555, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl0e_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl0e_caught_08[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   556, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl0e_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl0e_caught_09[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   560, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl0e_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl0e_caught_10[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   561, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl0e_caught_09 ),
};

/* Char_Caught 11 */
const Sint32 pl0e_caught_11[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   562, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl0e_caught_09 ),
};

/* Char_Caught 12 */
const Sint32 pl0e_caught_12[] = {
	CG_FRAME(    0,    0,   -1,        0xff,   565, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl0e_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl0e_caught_13[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   569, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl0e_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl0e_caught_14[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   570, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0e_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl0e_caught_15[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   571, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0e_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl0e_caught_16[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   572, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0e_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl0e_caught_17[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   573, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl0e_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl0e_caught_18[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   585, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl0e_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl0e_caught_19[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   591, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl0e_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl0e_caught_20[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   579, 0x80,   0x8e,          0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl0e_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl0e_caught_21[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   566, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl0e_caught_21 ),
};

const Sint32 pl0e_unused_05[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    1, END_OF_DATA,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000074b8,
};
