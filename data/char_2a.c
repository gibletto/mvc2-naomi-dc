/* Storm (character 2a): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_2a      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_2a     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl2a.c) says which group
                   and number each move uses.
   pl2a_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl2a_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl2a_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl2a_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_2a
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_2a, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_2a */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl2a_stand_00[], pl2a_stand_02[], pl2a_stand_02_loop[], pl2a_stand_06[],
	pl2a_stand_04[], pl2a_stand_03[], pl2a_stand_03_loop[], pl2a_stand_05[], pl2a_jump_00[],
	pl2a_jump_00_loop[], pl2a_jump_06[], pl2a_jump_09[], pl2a_jump_01[], pl2a_jump_01_loop[],
	pl2a_jump_07[], pl2a_jump_10[], pl2a_jump_02[], pl2a_jump_02_loop[], pl2a_jump_08[],
	pl2a_jump_11[], pl2a_jump_03[], pl2a_jump_04[], pl2a_jump_05[], pl2a_dash_00[],
	pl2a_dash_00_loop[], pl2a_dash_02[], pl2a_dash_01[], pl2a_dash_01_loop[], pl2a_dash_03[],
	pl2a_crouch_00[], pl2a_crouch_01[], pl2a_crouch_02[], pl2a_turn_00[], pl2a_turn_01[],
	pl2a_guard_00[], pl2a_guard_03[], pl2a_guard_01[], pl2a_guard_02[], pl2a_guard_04[],
	pl2a_guard_07[], pl2a_guard_05[], pl2a_guard_06[], pl2a_air_guard_00[], pl2a_air_guard_02[],
	pl2a_air_guard_01[], pl2a_grp27_17[], pl2a_grp27_18[], pl2a_grp27_19[], pl2a_stand_p_00[],
	pl2a_stand_p_00_loop[], pl2a_grp27_00[], pl2a_stand_p_01[], pl2a_stand_p_01_loop[],
	pl2a_grp27_02[], pl2a_stand_p_02[], pl2a_stand_p_02_loop[], pl2a_grp27_03[], pl2a_extra_04[],
	pl2a_extra_04_loop[], pl2a_stand_k_00[], pl2a_stand_k_00_loop[], pl2a_grp27_04[],
	pl2a_stand_k_01[], pl2a_stand_k_01_loop[], pl2a_grp27_05[], pl2a_stand_k_02[],
	pl2a_stand_k_02_loop[], pl2a_grp27_06[], pl2a_crouch_p_00[], pl2a_crouch_p_00_loop[],
	pl2a_grp27_07[], pl2a_crouch_p_01[], pl2a_crouch_p_01_loop[], pl2a_grp27_08[],
	pl2a_crouch_p_02[], pl2a_crouch_p_02_loop[], pl2a_grp27_09[], pl2a_crouch_k_00[],
	pl2a_crouch_k_00_loop[], pl2a_grp27_10[], pl2a_crouch_k_01[], pl2a_crouch_k_01_loop[],
	pl2a_grp27_11[], pl2a_crouch_k_02[], pl2a_crouch_k_02_loop[], pl2a_grp27_12[], pl2a_jump_p_00[],
	pl2a_jump_p_00_loop[], pl2a_grp27_13[], pl2a_jump_p_02[], pl2a_jump_p_02_loop[], pl2a_grp27_14[],
	pl2a_grp27_20[], pl2a_jump_p_06[], pl2a_jump_p_01[], pl2a_jump_p_07[], pl2a_extra_07[],
	pl2a_extra_07_loop[], pl2a_extra_08[], pl2a_extra_08_loop[], pl2a_extra_09[],
	pl2a_extra_09_loop[], pl2a_jump_p_08[], pl2a_jump_p_09[], pl2a_jump_k_00[],
	pl2a_jump_k_00_loop[], pl2a_grp27_15[], pl2a_jump_k_01[], pl2a_jump_k_01_loop[],
	pl2a_jump_k_02[], pl2a_jump_k_02_loop[], pl2a_grp27_16[], pl2a_get_up_00[], pl2a_get_up_01[],
	pl2a_get_up_02[], pl2a_get_up_03[], pl2a_damage_00[], pl2a_damage_01[], pl2a_damage_02[],
	pl2a_damage_03[], pl2a_damage_04[], pl2a_damage_05[], pl2a_damage_06[], pl2a_damage_07[],
	pl2a_damage_08[], pl2a_damage_10[], pl2a_damage_11[], pl2a_damage_12[], pl2a_damage_13[],
	pl2a_damage_13_loop[], pl2a_damage_09[], pl2a_damage_09_loop[], pl2a_damage_16[],
	pl2a_damage_17[], pl2a_damage_18[], pl2a_damage_19[], pl2a_damage_20[], pl2a_damage_21[],
	pl2a_damage_22[], pl2a_damage_23[], pl2a_damage_24[], pl2a_damage_25[], pl2a_damage_26[],
	pl2a_damage_27[], pl2a_damage_28[], pl2a_damage_29[], pl2a_damage_31[], pl2a_dizzy_00[],
	pl2a_win_00[], pl2a_win_00_loop[], pl2a_win_01[], pl2a_win_01_loop[], pl2a_special_23[],
	pl2a_special_23_loop[], pl2a_special_24[], pl2a_special_24_loop[], pl2a_special_25[],
	pl2a_appear_00[], pl2a_appear_00_loop[], pl2a_special_00[], pl2a_grp27_21[], pl2a_grp27_22[],
	pl2a_grp27_23[], pl2a_grp27_24[], pl2a_special_01[], pl2a_special_02[], pl2a_special_03[],
	pl2a_special_03_loop[], pl2a_special_26[], pl2a_special_26_loop[], pl2a_special_21[],
	pl2a_special_22[], pl2a_special_22_loop[], pl2a_special_04[], pl2a_special_04_loop[],
	pl2a_special_27[], pl2a_special_05[], pl2a_special_06[], pl2a_special_07[], pl2a_special_08[],
	pl2a_special_09[], pl2a_special_10[], pl2a_special_11[], pl2a_special_12[], pl2a_special_13[],
	pl2a_special_14[], pl2a_special_15[], pl2a_special_16[], pl2a_special_17[], pl2a_special_18[],
	pl2a_special_19[], pl2a_unused_00[], pl2a_special_28[], pl2a_special_29[], pl2a_special_30[],
	pl2a_special_31[], pl2a_special_32[], pl2a_special_33[], pl2a_special_34[], pl2a_special_35[],
	pl2a_super_00[], pl2a_super_01[], pl2a_super_02[], pl2a_super_03[], pl2a_super_04[],
	pl2a_super_05[], pl2a_super_06[], pl2a_super_07[], pl2a_super_08[], pl2a_super_09[],
	pl2a_super_10[], pl2a_special_20[], pl2a_super_11[], pl2a_catch_00[], pl2a_catch_00_loop[],
	pl2a_catch_01[], pl2a_catch_01_loop[], pl2a_catch_02[], pl2a_caught_00[], pl2a_caught_01[],
	pl2a_caught_02[], pl2a_caught_03[], pl2a_caught_04[], pl2a_caught_05[], pl2a_caught_06[],
	pl2a_caught_07[], pl2a_caught_08[], pl2a_caught_09[], pl2a_caught_10[], pl2a_caught_11[],
	pl2a_caught_12[], pl2a_caught_13[], pl2a_caught_14[], pl2a_caught_15[], pl2a_caught_16[],
	pl2a_caught_17[], pl2a_caught_18[], pl2a_caught_19[], pl2a_caught_20[], pl2a_caught_21[],
	pl2a_fly_00[], pl2a_fly_02[], pl2a_unused_01[], pl2a_change_01[], pl2a_change_01_loop[],
	pl2a_change_00[], pl2a_change_00_loop[], pl2a_change_02[], pl2a_extra_00[], pl2a_extra_02[],
	pl2a_effect_01[], pl2a_effect_02[], pl2a_effect_03[], pl2a_effect_04[], pl2a_effect_05[],
	pl2a_effect_06[], pl2a_effect_06_loop[], pl2a_effect_07[], pl2a_effect_08[], pl2a_effect_09[],
	pl2a_effect_19[], pl2a_effect_20[], pl2a_effect_21[], pl2a_effect_10[], pl2a_effect_11[],
	pl2a_effect_12[], pl2a_effect_13[], pl2a_effect_14[], pl2a_effect_15[], pl2a_effect_16[],
	pl2a_effect_17[], pl2a_effect_18[], pl2a_unused_02[], pl2a_unused_03[], pl2a_unused_04[],
	pl2a_unused_05[], pl2a_unused_06[], pl2a_unused_07[], pl2a_unused_08[], pl2a_unused_09[],
	pl2a_unused_10[], pl2a_unused_11[], pl2a_unused_12[], pl2a_unused_13[], pl2a_unused_14[],
	pl2a_unused_15[], pl2a_unused_16[], pl2a_unused_17[], pl2a_unused_18[], pl2a_unused_19[],
	pl2a_unused_20[], pl2a_unused_21[], pl2a_unused_22[], pl2a_unused_23[], pl2a_unused_24[],
	pl2a_unused_25[], pl2a_unused_26[], pl2a_unused_27[], pl2a_unused_28[], pl2a_unused_29[],
	pl2a_unused_30[], pl2a_unused_31[], pl2a_unused_32[], pl2a_unused_33[], pl2a_unused_34[],
	pl2a_unused_35[], pl2a_unused_36[], pl2a_unused_37[], pl2a_unused_38[], pl2a_unused_39[],
	pl2a_unused_40[], pl2a_unused_41[], pl2a_unused_42[], pl2a_unused_43[], pl2a_unused_44[],
	pl2a_unused_45[], pl2a_unused_46[], pl2a_unused_47[], pl2a_unused_48[], pl2a_unused_49[],
	pl2a_unused_50[], pl2a_unused_51[], pl2a_unused_52[], pl2a_unused_53[], pl2a_unused_54[],
	pl2a_unused_55[], pl2a_unused_56[], pl2a_unused_57[], pl2a_unused_58[], pl2a_unused_59[],
	pl2a_unused_60[], pl2a_unused_61[], pl2a_unused_62[], pl2a_unused_63[], pl2a_unused_64[],
	pl2a_unused_65[], pl2a_unused_66[], pl2a_unused_67[], pl2a_unused_68[], pl2a_unused_69[],
	pl2a_unused_70[], pl2a_unused_71[], pl2a_unused_72[], pl2a_unused_73[], pl2a_unused_74[],
	pl2a_unused_75[], pl2a_unused_76[], pl2a_unused_77[], pl2a_unused_78[], pl2a_unused_79[],
	pl2a_unused_80[], pl2a_unused_81[], pl2a_unused_82[], pl2a_unused_83[], pl2a_unused_84[],
	pl2a_grp27_26[], pl2a_grp27_27[], pl2a_grp27_28[], pl2a_grp27_29[], pl2a_grp27_30[],
	pl2a_grp27_31[], pl2a_grp27_32[], pl2a_grp27_33[], pl2a_grp27_34[], pl2a_grp27_35[],
	pl2a_grp27_36[], pl2a_grp27_37[], pl2a_grp27_38[], pl2a_unused_85[], pl2a_unused_86[],
	pl2a_unused_87[], pl2a_unused_88[], pl2a_unused_89[], pl2a_unused_90[], pl2a_unused_91[],
	pl2a_unused_92[], pl2a_grp27_39[], pl2a_grp27_40[], pl2a_grp27_41[], pl2a_grp27_42[],
	pl2a_grp27_43[], pl2a_grp27_44[], pl2a_unused_93[], pl2a_unused_94[], pl2a_unused_95[],
	pl2a_grp27_45[], pl2a_grp27_46[], pl2a_grp27_47[], pl2a_grp27_48[], pl2a_grp27_49[],
	pl2a_grp27_50[], pl2a_grp27_25[], pl2a_grp27_25_loop[], pl2a_fly_01[], pl2a_fly_07[],
	pl2a_fly_07_loop[], pl2a_fly_15[], pl2a_fly_03[], pl2a_fly_03_loop[], pl2a_fly_11[],
	pl2a_change_03[], pl2a_change_03_loop[], pl2a_change_04[], pl2a_extra_06[],
	pl2a_extra_06_loop[];
extern const Sint32 *const pl2a_stand_tbl[], *const pl2a_jump_tbl[], *const pl2a_dash_tbl[],
	*const pl2a_crouch_tbl[], *const pl2a_turn_tbl[], *const pl2a_guard_tbl[],
	*const pl2a_air_guard_tbl[], *const pl2a_stand_p_tbl[], *const pl2a_stand_k_tbl[],
	*const pl2a_crouch_p_tbl[], *const pl2a_crouch_k_tbl[], *const pl2a_jump_p_tbl[],
	*const pl2a_jump_k_tbl[], *const pl2a_damage_tbl[], *const pl2a_dizzy_tbl[],
	*const pl2a_catch_tbl[], *const pl2a_caught_tbl[], *const pl2a_get_up_tbl[],
	*const pl2a_appear_tbl[], *const pl2a_win_tbl[], *const pl2a_extra_tbl[],
	*const pl2a_special_tbl[], *const pl2a_super_tbl[], *const pl2a_effect_tbl[],
	*const pl2a_change_tbl[], *const pl2a_grp25_tbl[], *const pl2a_fly_tbl[],
	*const pl2a_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_2a[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_2a[] = {
	pl2a_stand_tbl, pl2a_jump_tbl, pl2a_dash_tbl, pl2a_crouch_tbl, pl2a_turn_tbl, pl2a_guard_tbl,
	pl2a_air_guard_tbl, pl2a_stand_p_tbl, pl2a_stand_k_tbl, pl2a_crouch_p_tbl, pl2a_crouch_k_tbl, pl2a_jump_p_tbl,
	pl2a_jump_k_tbl, pl2a_damage_tbl, pl2a_dizzy_tbl, pl2a_catch_tbl, pl2a_caught_tbl, pl2a_get_up_tbl,
	pl2a_appear_tbl, pl2a_win_tbl, pl2a_extra_tbl, pl2a_special_tbl, pl2a_super_tbl, pl2a_effect_tbl,
	pl2a_change_tbl, pl2a_grp25_tbl, pl2a_fly_tbl, pl2a_grp27_tbl, 0,
};

const Sint32 *const pl2a_stand_tbl[] = {
	pl2a_stand_00, pl2a_stand_00, pl2a_stand_02, pl2a_stand_03, pl2a_stand_04, pl2a_stand_05,
	pl2a_stand_06, pl2a_stand_06, 0,
};

const Sint32 *const pl2a_jump_tbl[] = {
	pl2a_jump_00, pl2a_jump_01, pl2a_jump_02, pl2a_jump_03, pl2a_jump_04, pl2a_jump_05,
	pl2a_jump_06, pl2a_jump_07, pl2a_jump_08, pl2a_jump_09, pl2a_jump_10, pl2a_jump_11,
	0,
};

const Sint32 *const pl2a_dash_tbl[] = {
	pl2a_dash_00, pl2a_dash_01, pl2a_dash_02, pl2a_dash_03, 0,
};

const Sint32 *const pl2a_crouch_tbl[] = {
	pl2a_crouch_00, pl2a_crouch_01, pl2a_crouch_02, 0,
};

const Sint32 *const pl2a_turn_tbl[] = {
	pl2a_turn_00, pl2a_turn_01, 0,
};

const Sint32 *const pl2a_guard_tbl[] = {
	pl2a_guard_00, pl2a_guard_01, pl2a_guard_02, pl2a_guard_03, pl2a_guard_04, pl2a_guard_05,
	pl2a_guard_06, pl2a_guard_07, 0,
};

const Sint32 *const pl2a_air_guard_tbl[] = {
	pl2a_air_guard_00, pl2a_air_guard_01, pl2a_air_guard_02, 0,
};

const Sint32 *const pl2a_stand_p_tbl[] = {
	pl2a_stand_p_00, pl2a_stand_p_01, pl2a_stand_p_02, 0,
};

const Sint32 *const pl2a_stand_k_tbl[] = {
	pl2a_stand_k_00, pl2a_stand_k_01, pl2a_stand_k_02, 0,
};

const Sint32 *const pl2a_crouch_p_tbl[] = {
	pl2a_crouch_p_00, pl2a_crouch_p_01, pl2a_crouch_p_02, 0,
};

const Sint32 *const pl2a_crouch_k_tbl[] = {
	pl2a_crouch_k_00, pl2a_crouch_k_01, pl2a_crouch_k_02, 0,
};

const Sint32 *const pl2a_jump_p_tbl[] = {
	pl2a_jump_p_00, pl2a_jump_p_01, pl2a_jump_p_02, pl2a_jump_p_00, pl2a_jump_p_01, pl2a_jump_p_02,
	pl2a_jump_p_06, pl2a_jump_p_07, pl2a_jump_p_08, pl2a_jump_p_09, 0,
};

const Sint32 *const pl2a_jump_k_tbl[] = {
	pl2a_jump_k_00, pl2a_jump_k_01, pl2a_jump_k_02, pl2a_jump_k_00, pl2a_jump_k_01, pl2a_jump_k_02,
	0,
};

const Sint32 *const pl2a_damage_tbl[] = {
	pl2a_damage_00, pl2a_damage_01, pl2a_damage_02, pl2a_damage_03, pl2a_damage_04, pl2a_damage_05,
	pl2a_damage_06, pl2a_damage_07, pl2a_damage_08, pl2a_damage_09, pl2a_damage_10, pl2a_damage_11,
	pl2a_damage_12, pl2a_damage_13, pl2a_damage_13, pl2a_damage_13, pl2a_damage_16, pl2a_damage_17,
	pl2a_damage_18, pl2a_damage_19, pl2a_damage_20, pl2a_damage_21, pl2a_damage_22, pl2a_damage_23,
	pl2a_damage_24, pl2a_damage_25, pl2a_damage_26, pl2a_damage_27, pl2a_damage_28, pl2a_damage_29,
	pl2a_damage_27, pl2a_damage_31, pl2a_damage_27, pl2a_damage_27, pl2a_damage_13, 0,
};

const Sint32 *const pl2a_dizzy_tbl[] = {
	pl2a_dizzy_00, 0,
};

const Sint32 *const pl2a_catch_tbl[] = {
	pl2a_catch_00, pl2a_catch_01, pl2a_catch_02, 0,
};

const Sint32 *const pl2a_caught_tbl[] = {
	pl2a_caught_00, pl2a_caught_01, pl2a_caught_02, pl2a_caught_03, pl2a_caught_04, pl2a_caught_05,
	pl2a_caught_06, pl2a_caught_07, pl2a_caught_08, pl2a_caught_09, pl2a_caught_10, pl2a_caught_11,
	pl2a_caught_12, pl2a_caught_13, pl2a_caught_14, pl2a_caught_15, pl2a_caught_16, pl2a_caught_17,
	pl2a_caught_18, pl2a_caught_19, pl2a_caught_20, pl2a_caught_21, 0,
};

const Sint32 *const pl2a_get_up_tbl[] = {
	pl2a_get_up_00, pl2a_get_up_01, pl2a_get_up_02, pl2a_get_up_03, 0,
};

const Sint32 *const pl2a_appear_tbl[] = {
	pl2a_appear_00, 0,
};

const Sint32 *const pl2a_win_tbl[] = {
	pl2a_win_00, pl2a_win_01, 0,
};

const Sint32 *const pl2a_extra_tbl[] = {
	pl2a_extra_00, pl2a_extra_00, pl2a_extra_02, pl2a_extra_00, pl2a_extra_04, pl2a_extra_04,
	pl2a_extra_06, pl2a_extra_07, pl2a_extra_08, pl2a_extra_09, 0,
};

const Sint32 *const pl2a_special_tbl[] = {
	pl2a_special_00, pl2a_special_01, pl2a_special_02, pl2a_special_03, pl2a_special_04, pl2a_special_05,
	pl2a_special_06, pl2a_special_07, pl2a_special_08, pl2a_special_09, pl2a_special_10, pl2a_special_11,
	pl2a_special_12, pl2a_special_13, pl2a_special_14, pl2a_special_15, pl2a_special_16, pl2a_special_17,
	pl2a_special_18, pl2a_special_19, pl2a_special_20, pl2a_special_21, pl2a_special_22, pl2a_special_23,
	pl2a_special_24, pl2a_special_25, pl2a_special_26, pl2a_special_27, pl2a_special_28, pl2a_special_29,
	pl2a_special_30, pl2a_special_31, pl2a_special_32, pl2a_special_33, pl2a_special_34, pl2a_special_35,
	0,
};

const Sint32 *const pl2a_super_tbl[] = {
	pl2a_super_00, pl2a_super_01, pl2a_super_02, pl2a_super_03, pl2a_super_04, pl2a_super_05,
	pl2a_super_06, pl2a_super_07, pl2a_super_08, pl2a_super_09, pl2a_super_10, pl2a_super_11,
	0,
};

const Sint32 *const pl2a_effect_tbl[] = {
	pl2a_extra_02, pl2a_effect_01, pl2a_effect_02, pl2a_effect_03, pl2a_effect_04, pl2a_effect_05,
	pl2a_effect_06, pl2a_effect_07, pl2a_effect_08, pl2a_effect_09, pl2a_effect_10, pl2a_effect_11,
	pl2a_effect_12, pl2a_effect_13, pl2a_effect_14, pl2a_effect_15, pl2a_effect_16, pl2a_effect_17,
	pl2a_effect_18, pl2a_effect_19, pl2a_effect_20, pl2a_effect_21, 0,
};

const Sint32 *const pl2a_change_tbl[] = {
	pl2a_change_00, pl2a_change_01, pl2a_change_02, pl2a_change_03, pl2a_change_04, 0,
};

const Sint32 *const pl2a_grp25_tbl[] = {
	pl2a_stand_00, pl2a_stand_00, 0,
};

const Sint32 *const pl2a_fly_tbl[] = {
	pl2a_fly_00, pl2a_fly_01, pl2a_fly_02, pl2a_fly_03, pl2a_fly_03, pl2a_fly_03,
	pl2a_jump_09, pl2a_fly_07, pl2a_fly_07, pl2a_fly_07, pl2a_fly_01, pl2a_fly_11,
	pl2a_fly_11, pl2a_fly_11, pl2a_fly_01, pl2a_fly_15, pl2a_fly_15, pl2a_fly_15,
	0,
};

const Sint32 *const pl2a_grp27_tbl[] = {
	pl2a_grp27_00, pl2a_grp27_00, pl2a_grp27_02, pl2a_grp27_03, pl2a_grp27_04, pl2a_grp27_05,
	pl2a_grp27_06, pl2a_grp27_07, pl2a_grp27_08, pl2a_grp27_09, pl2a_grp27_10, pl2a_grp27_11,
	pl2a_grp27_12, pl2a_grp27_13, pl2a_grp27_14, pl2a_grp27_15, pl2a_grp27_16, pl2a_grp27_17,
	pl2a_grp27_18, pl2a_grp27_19, pl2a_grp27_20, pl2a_grp27_21, pl2a_grp27_22, pl2a_grp27_23,
	pl2a_grp27_24, pl2a_grp27_25, pl2a_grp27_26, pl2a_grp27_27, pl2a_grp27_28, pl2a_grp27_29,
	pl2a_grp27_30, pl2a_grp27_31, pl2a_grp27_32, pl2a_grp27_33, pl2a_grp27_34, pl2a_grp27_35,
	pl2a_grp27_36, pl2a_grp27_37, pl2a_grp27_38, pl2a_grp27_39, pl2a_grp27_40, pl2a_grp27_41,
	pl2a_grp27_42, pl2a_grp27_43, pl2a_grp27_44, pl2a_grp27_45, pl2a_grp27_46, pl2a_grp27_47,
	pl2a_grp27_48, pl2a_grp27_49, pl2a_grp27_50, pl2a_grp27_25, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_move, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   group 25 0
   group 25 1 */
const Sint32 pl2a_stand_00[] = {
	CG_FRAME(    0,    0,   6,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl2a_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl2a_stand_02[] = {
	CG_FRAME(    0,    0,   1,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2a_stand_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_NEXT( pl2a_stand_02_loop ),
};

/* Char_Stand 6
   Char_Stand 7 */
const Sint32 pl2a_stand_06[] = {
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_06 ),
};

/* Char_Stand 4 */
const Sint32 pl2a_stand_04[] = {
	CG_FRAME(    0,    0,   1,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_00 ),
};

/* Char_Stand 3 */
const Sint32 pl2a_stand_03[] = {
	CG_FRAME(    0,    0,   1,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2a_stand_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_NEXT( pl2a_stand_03_loop ),
};

/* Char_Stand 5 */
const Sint32 pl2a_stand_05[] = {
	CG_FRAME(    0,    0,   1,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_00 ),
};

/* Char_Jump 0 */
const Sint32 pl2a_jump_00[] = {
	CG_FRAME(    0,    1,   2,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    1,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   1,           0,   121, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   1,           0,   123, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   124, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   1,           0,   125, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   126, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   1,           0,   127, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
};

const Sint32 pl2a_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl2a_jump_00_loop ),
};

/* Char_Jump 6 */
const Sint32 pl2a_jump_06[] = {
	CG_FRAME(    0,    1,   2,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Fly 6 */
const Sint32 pl2a_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_09 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl2a_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   6,           0,   156, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   159, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   162, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
};

const Sint32 pl2a_jump_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   169, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   176, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl2a_jump_01_loop ),
};

/* Char_Jump 7 */
const Sint32 pl2a_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   156, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   167, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   168, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   167, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   168, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   167, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   168, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   157, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   158, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   159, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   160, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   161, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   162, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   163, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   164, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   165, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   166, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl2a_jump_10[] = {
	CG_FRAME(    0,    0,   2,           0,   169, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   176, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_10 ),
};

/* Char_Jump 2 */
const Sint32 pl2a_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
};

const Sint32 pl2a_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   199, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   200, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl2a_jump_02_loop ),
};

/* Char_Jump 8 */
const Sint32 pl2a_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl2a_jump_11[] = {
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   202, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   204, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_11 ),
};

/* Char_Jump 3 */
const Sint32 pl2a_jump_03[] = {
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_jump_03 ),
};

/* Char_Jump 4 */
const Sint32 pl2a_jump_04[] = {
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_jump_04 ),
};

/* Char_Jump 5 */
const Sint32 pl2a_jump_05[] = {
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_jump_05 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl2a_dash_00[] = {
	CG_FRAME(    0,    1,   3,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2a_dash_00_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0400,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0400,   1 ),
	CG_NEXT( pl2a_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl2a_dash_02[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl2a_dash_01[] = {
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2a_dash_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0500,   1 ),
	CG_NEXT( pl2a_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl2a_dash_03[] = {
	CG_FRAME(    0,    0,   1,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl2a_crouch_00[] = {
	CG_FRAME(    4,    0,   3,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    3,    0,   2,           0,   213, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    1,    0,   3,           0,   214, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   215, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl2a_crouch_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    74, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl2a_crouch_02[] = {
	CG_FRAME(    4,    0,   3,           0,   216, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   2,           0,   217, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   1,           0,   218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   219, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl2a_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   222, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl2a_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   228, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   229, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   230, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   231, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl2a_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 3 */
const Sint32 pl2a_guard_03[] = {
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl2a_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   237, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl2a_guard_02[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl2a_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,   238, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 7 */
const Sint32 pl2a_guard_07[] = {
	CG_FRAME(    0,    0,   3,           0,   239, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl2a_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   239, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl2a_guard_06[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   238, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl2a_air_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0600,   3 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl2a_air_guard_02[] = {
	CG_FRAME(    0,    0,   3,           0,   242, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0600,   3 ),
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0600,   3 ),
	CG_FRAME(    0,    0,   3,           0,   244, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0600,   3 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0600,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl2a_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   246, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0600,   3 ),
	CG_NEXT( pl2a_air_guard_01 ),
};

/* group 27 17 */
const Sint32 pl2a_grp27_17[] = {
	EF_FRAME(    0,    0,   2,           0,  1096 ),
	EF_FRAME(    0,    0,   2,           0,  1097 ),
	EF_FRAME(    0,    0,   2,           0,  1098 ),
	EF_FRAME(    0,    0,   2,           0,  1099 ),
	EF_FRAME(    0,    0,   2,           0,  1100 ),
	EF_FRAME(    0,    0,   2,           0,  1101 ),
	EF_FRAME(    0,    0,   2,           0,  1102 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,  1103 ),
	CG_NEXT( pl2a_grp27_17 ),
};

/* group 27 18 */
const Sint32 pl2a_grp27_18[] = {
	EF_FRAME(    0,    0,   2,           0,  1104 ),
	EF_FRAME(    0,    0,   2,           0,  1105 ),
	EF_FRAME(    0,    0,   2,           0,  1106 ),
	EF_FRAME(    0,    0,   2,           0,  1107 ),
	EF_FRAME(    0,    0,   2,           0,  1108 ),
	EF_FRAME(    0,    0,   2,           0,  1109 ),
	EF_FRAME(    0,    0,   2,           0,  1110 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,  1111 ),
	CG_NEXT( pl2a_grp27_18 ),
};

/* group 27 19 */
const Sint32 pl2a_grp27_19[] = {
	EF_FRAME(    0,    0,   2,           0,  1112 ),
	EF_FRAME(    0,    0,   2,           0,  1113 ),
	EF_FRAME(    0,    0,   2,           0,  1114 ),
	EF_FRAME(    0,    0,   2,           0,  1115 ),
	EF_FRAME(    0,    0,   2,           0,  1116 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,  1117 ),
	CG_NEXT( pl2a_grp27_19 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl2a_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   368, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0, 0x1000,   1 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   370, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl2a_stand_p_00_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   368, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_p_00_loop ),
};

/* group 27 0
   group 27 1 */
const Sint32 pl2a_grp27_00[] = {
	EF_FRAME(    0,    0,   3,           0,   927 ),
	EF_FRAME(    0,    0,   2,           0,   928 ),
	EF_FRAME(    0,    0,   2,           0,   929 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   930 ),
	CG_NEXT( pl2a_grp27_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl2a_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   371, 0x80,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0, 0x1100,   1 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   4,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   8,           0,   374, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
};

const Sint32 pl2a_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   375, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_p_01_loop ),
};

/* group 27 2 */
const Sint32 pl2a_grp27_02[] = {
	EF_FRAME(    0,    0,   4,           0,   931 ),
	EF_FRAME(    0,    0,   2,           0,   932 ),
	EF_FRAME(    0,    0,   1,           0,   933 ),
	EF_FRAME(    0,    0,   1,           0,   934 ),
	EF_FRAME(    0,    0,   1,           0,   935 ),
	EF_FRAME(    0,    0,   1,           0,   936 ),
	EF_FRAME(    0,    0,   1,           0,   937 ),
	EF_FRAME(    0,    0,   1,           0,   938 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   939 ),
	CG_NEXT( pl2a_grp27_02 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl2a_stand_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   376, 0x80,   0x00,  ATK,  0x00,       0x20,    0,   0,     0, 0x1200,   1 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0, 0x0700,   1 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,  12 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,  12 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,  12 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  13 ),
};

const Sint32 pl2a_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_p_02_loop ),
};

/* group 27 3 */
const Sint32 pl2a_grp27_03[] = {
	EF_FRAME(    0,    0,   2,           0,   940 ),
	EF_FRAME(    0,    0,   2,           0,   941 ),
	EF_FRAME(    0,    0,   2,           0,   942 ),
	EF_FRAME(    0,    0,   2,           0,   943 ),
	EF_FRAME(    0,    0,   2,           0,   944 ),
	EF_FRAME(    0,    0,   2,           0,   945 ),
	EF_FRAME(    0,    0,   2,           0,   946 ),
	EF_FRAME(    0,    0,   2,           0,   947 ),
	EF_FRAME(    0,    0,   2,           0,   948 ),
	EF_FRAME(    0,    0,   2,           0,   949 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   950 ),
	CG_NEXT( pl2a_grp27_03 ),
};

/* Char_Extra 4
   Char_Extra 5: snapback_init (Snapback) */
const Sint32 pl2a_extra_04[] = {
	CG_FRAME(    0,    0,   6,           0,   376, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1200,   1 ),
	CG_FRAME(    0,    1,   3,           0,   376, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0, 0x0700,   1 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,  75 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,  75 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,  75 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  13 ),
};

const Sint32 pl2a_extra_04_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_extra_04_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl2a_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0, 0x1300,   1 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   382, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
};

const Sint32 pl2a_stand_k_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   383, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_k_00_loop ),
};

/* group 27 4 */
const Sint32 pl2a_grp27_04[] = {
	EF_FRAME(    0,    0,   2,           0,   951 ),
	EF_FRAME(    0,    0,   2,           0,   952 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   953 ),
	CG_NEXT( pl2a_grp27_04 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl2a_stand_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   384, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0, 0x1400,   1 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
};

const Sint32 pl2a_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,   7, END_OF_DATA,   387, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_k_01_loop ),
};

/* group 27 5 */
const Sint32 pl2a_grp27_05[] = {
	EF_FRAME(    0,    0,   3,           0,   954 ),
	EF_FRAME(    0,    0,   2,           0,   955 ),
	EF_FRAME(    0,    0,   2,           0,   956 ),
	EF_FRAME(    0,    0,   2,           0,   957 ),
	EF_FRAME(    0,    0,   2,           0,   958 ),
	EF_FRAME(    0,    0,   2,           0,   959 ),
	EF_FRAME(    0,    0,   2,           0,   960 ),
	EF_FRAME(    0,    0,   2,           0,   961 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   962 ),
	CG_NEXT( pl2a_grp27_05 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl2a_stand_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x00,  ATK,  0x00,       0x25,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1500,   1 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   393, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   394, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,  10,           0,   395, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
};

const Sint32 pl2a_stand_k_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_stand_k_02_loop ),
};

/* group 27 6 */
const Sint32 pl2a_grp27_06[] = {
	EF_FRAME(    0,    0,   2,           0,   963 ),
	EF_FRAME(    0,    0,   2,           0,   964 ),
	EF_FRAME(    0,    0,   2,           0,   965 ),
	EF_FRAME(    0,    0,   2,           0,   966 ),
	EF_FRAME(    0,    0,   2,           0,   967 ),
	EF_FRAME(    0,    0,   1,           0,   968 ),
	EF_FRAME(    0,    0,   2,           0,   969 ),
	EF_FRAME(    0,    0,   1,           0,   970 ),
	EF_FRAME(    0,    0,   2,           0,   971 ),
	EF_FRAME(    0,    0,   1,           0,   972 ),
	EF_FRAME(    0,    0,   2,           0,   973 ),
	EF_FRAME(    0,    0,   1,           0,   974 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   975 ),
	CG_NEXT( pl2a_grp27_06 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl2a_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   397, 0x80,   0x40,  ATK,  0x00,       0x1e,    0,   0,     0, 0x1600,   2 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
};

const Sint32 pl2a_crouch_p_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   397, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_crouch_p_00_loop ),
};

/* group 27 7 */
const Sint32 pl2a_grp27_07[] = {
	EF_FRAME(    0,    0,   3,           0,   976 ),
	EF_FRAME(    0,    0,   2,           0,   977 ),
	EF_FRAME(    0,    0,   2,           0,   978 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   979 ),
	CG_NEXT( pl2a_grp27_07 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl2a_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x40,  ATK,  0x00,       0x1f,    0,   0,     0, 0x1700,   2 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   2,           0,   402, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,   2,           0,   403, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,   2,           0,   402, 0x80,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   6,           0,   403, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
};

const Sint32 pl2a_crouch_p_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   404, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_crouch_p_01_loop ),
};

/* group 27 8 */
const Sint32 pl2a_grp27_08[] = {
	EF_FRAME(    0,    0,   4,           0,   980 ),
	EF_FRAME(    0,    0,   2,           0,   981 ),
	EF_FRAME(    0,    0,   2,           0,   982 ),
	EF_FRAME(    0,    0,   2,           0,   983 ),
	EF_FRAME(    0,    0,   1,           0,   984 ),
	EF_FRAME(    0,    0,   1,           0,   985 ),
	EF_FRAME(    0,    0,   1,           0,   986 ),
	EF_FRAME(    0,    0,   1,           0,   987 ),
	EF_FRAME(    0,    0,   1,           0,   988 ),
	EF_FRAME(    0,    0,   1,           0,   989 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   990 ),
	CG_NEXT( pl2a_grp27_08 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl2a_crouch_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   405, 0x80,   0x40,  ATK,  0x00,       0x20,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x80,   0x40,  ATK,  0x00,       0x03,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   408, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0, 0x1800,  24 ),
	CG_FRAME(    0,    0,   4,           0,   409, 0x80,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
};

const Sint32 pl2a_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   409, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl2a_crouch_p_02_loop ),
};

/* group 27 9 */
const Sint32 pl2a_grp27_09[] = {
	EF_FRAME(    0,    0,   1,           0,   991 ),
	EF_FRAME(    0,    0,   2,           0,   992 ),
	EF_FRAME(    0,    0,   2,           0,   993 ),
	EF_FRAME(    0,    0,   2,           0,   994 ),
	EF_FRAME(    0,    0,   2,           0,   995 ),
	EF_FRAME(    0,    0,   2,           0,   996 ),
	EF_FRAME(    0,    0,   2,           0,   997 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   998 ),
	CG_NEXT( pl2a_grp27_09 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl2a_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x40,  ATK,  0x00, 0x14 | CSE,    0,   0,     0, 0x1900,   2 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   412, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
};

const Sint32 pl2a_crouch_k_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   410, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_crouch_k_00_loop ),
};

/* group 27 10 */
const Sint32 pl2a_grp27_10[] = {
	EF_FRAME(    0,    0,   2,           0,   999 ),
	EF_FRAME(    0,    0,   2,           0,  1000 ),
	EF_FRAME(    0,    0,   2,           0,  1001 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,  1002 ),
	CG_NEXT( pl2a_grp27_10 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl2a_crouch_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x4e,  ATK,  0x00,       0x24,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0x80,   0x4e,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   415, 0x80,   0x85,  ATK,  0x00,          0,    0,   0,     0, 0x1a00,  28 ),
	CG_FRAME(    0,    0,  22,           0,   416, 0x80,   0x85,  ATE,  0x00,          0,    0,   0,     0, 0x1a00,  29 ),
};

const Sint32 pl2a_crouch_k_01_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   414, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_NEXT( pl2a_crouch_k_01_loop ),
};

/* group 27 11 */
const Sint32 pl2a_grp27_11[] = {
	EF_FRAME(    0,    0,   1,           0,  1003 ),
	EF_FRAME(    0,    0,   1,           0,  1004 ),
	EF_FRAME(    0,    0,   1,           0,  1005 ),
	EF_FRAME(    0,    0,   1,           0,  1006 ),
	EF_FRAME(    0,    0,   1,           0,  1007 ),
	EF_FRAME(    0,    0,   1,           0,  1008 ),
	EF_FRAME(    0,    0,   1,           0,  1009 ),
	EF_FRAME(    0,    0,   1,           0,  1010 ),
	EF_FRAME(    0,    0,   1,           0,  1011 ),
	EF_FRAME(    0,    0,   1,           0,  1012 ),
	EF_FRAME(    0,    0,   1,           0,  1013 ),
	EF_FRAME(    0,    0,   1,           0,  1014 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,  1015 ),
	CG_NEXT( pl2a_grp27_11 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl2a_crouch_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   417, 0x80,   0x40,  ATK,  0x00,       0x25,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   418, 0x80,   0x6b,  ATK,  0x00,       0x03,    0,   0,     0, 0x1b00,   2 ),
	CG_FRAME(    0,    0,   4,           0,   419, 0x80,   0x6b,  ATK,  0x00,          0,    0,   0,     0, 0x0800,  30 ),
	CG_FRAME(    0,    0,  16,           0,   420, 0x80,   0x6b,  ATK,  0x00,          0,    0,   0,     0, 0x0800,  31 ),
};

const Sint32 pl2a_crouch_k_02_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   417, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl2a_crouch_k_02_loop ),
};

/* group 27 12 */
const Sint32 pl2a_grp27_12[] = {
	EF_FRAME(    0,    0,   2,           0,  1016 ),
	EF_FRAME(    0,    0,   2,           0,  1017 ),
	EF_FRAME(    0,    0,   2,           0,  1018 ),
	EF_FRAME(    0,    0,   2,           0,  1019 ),
	EF_FRAME(    0,    0,   2,           0,  1020 ),
	EF_FRAME(    0,    0,   2,           0,  1021 ),
	EF_FRAME(    0,    0,   2,           0,  1022 ),
	EF_FRAME(    0,    0,   2,           0,  1023 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,  1024 ),
	CG_NEXT( pl2a_grp27_12 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl2a_jump_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x20,  ATK,  0x00,       0x1e,    0,   0,     0, 0x1c00,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   422, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x0900,  32 ),
	CG_FRAME( 0x80,    0,   2,           0,   423, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x0900,  32 ),
	CG_FRAME( 0x82,    0,   2,           0,   424, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x0900,  32 ),
	CG_FRAME(    0,    0,   2,           0,   423, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x0900,   3 ),
	CG_FRAME(    0,    0,   2,           0,   424, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0, 0x0900,   3 ),
};

const Sint32 pl2a_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   421, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_p_00_loop ),
};

/* group 27 13 */
const Sint32 pl2a_grp27_13[] = {
	EF_FRAME(    0,    0,   4,           0,  1025 ),
	EF_FRAME(    0,    0,   2,           0,  1026 ),
	EF_FRAME(    0,    0,   2,           0,  1027 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,  1028 ),
	CG_NEXT( pl2a_grp27_13 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl2a_jump_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   425, 0x80,   0x20,  ATK,  0x00,       0x20,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   426, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0, 0x1d00,   3 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   428, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2a_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   425, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_p_02_loop ),
};

/* group 27 14 */
const Sint32 pl2a_grp27_14[] = {
	EF_FRAME(    0,    0,   6,           0,  1029 ),
	EF_FRAME(    0,    0,   2,           0,  1030 ),
	EF_FRAME(    0,    0,   2,           0,  1031 ),
	EF_FRAME(    0,    0,   2,           0,  1032 ),
	EF_FRAME(    0,    0,   2,           0,  1033 ),
	EF_FRAME(    0,    0,   2,           0,  1034 ),
	EF_FRAME(    0,    0,   2,           0,  1035 ),
	EF_FRAME(    0,    0,   1,           0,  1036 ),
	EF_FRAME(    0,    0,   2,           0,  1037 ),
	EF_FRAME(    0,    0,   1,           0,  1038 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,  1039 ),
	CG_NEXT( pl2a_grp27_14 ),
};

/* group 27 20 */
const Sint32 pl2a_grp27_20[] = {
	EF_FRAME(    0,    0,   2,           0,  1032 ),
	EF_FRAME(    0,    0,   2,           0,  1033 ),
	EF_FRAME(    0,    0,   2,           0,  1034 ),
	EF_FRAME(    0,    0,   2,           0,  1035 ),
	EF_FRAME(    0,    0,   1,           0,  1036 ),
	EF_FRAME(    0,    0,   2,           0,  1037 ),
	EF_FRAME(    0,    0,   1,           0,  1038 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,  1039 ),
	CG_NEXT( pl2a_grp27_20 ),
};

/* Char_Jump_P 6 */
const Sint32 pl2a_jump_p_06[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x20,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    3,   4,           0,   432, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   434, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   449, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_extra_07_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl2a_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   4,           0,   437, 0x80,   0x20,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    1,   4,           0,   438, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   439, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   440, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   449, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_extra_08_loop ),
};

/* Char_Jump_P 7 */
const Sint32 pl2a_jump_p_07[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   443, 0x80,   0x20,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    2,   4,           0,   444, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   3,           0,   445, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   446, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   449, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_extra_09_loop ),
};

/* Char_Extra 7 */
const Sint32 pl2a_extra_07[] = {
	CG_FRAME(    0,    0,  10,           0,   429, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   430, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x20,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    3,   4,           0,   432, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   434, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
};

const Sint32 pl2a_extra_07_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   449, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_extra_07_loop ),
};

/* Char_Extra 8 */
const Sint32 pl2a_extra_08[] = {
	CG_FRAME(    0,    0,  10,           0,   429, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   430, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x20,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    1,   4,           0,   438, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   439, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   440, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
};

const Sint32 pl2a_extra_08_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   449, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_extra_08_loop ),
};

/* Char_Extra 9 */
const Sint32 pl2a_extra_09[] = {
	CG_FRAME(    0,    0,  10,           0,   429, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   430, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   443, 0x80,   0x20,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    2,   4,           0,   444, 0x80,   0x20,  ATK,  0x00,       0x03,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   445, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   446, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
};

const Sint32 pl2a_extra_09_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   449, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_extra_09_loop ),
};

/* Char_Jump_P 8: pls2a_04_init, shl2a_04_init */
const Sint32 pl2a_jump_p_08[] = {
	CG_FRAME(    0,    0,   2,           0,  1040, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,  1041, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,  1042, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,  1043, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,  1044, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,  1045, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,  1046, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1047, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl2a_jump_p_08 ),
};

/* Char_Jump_P 9 */
const Sint32 pl2a_jump_p_09[] = {
	CG_FRAME(    0,    0,   2,           0,  1048, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1049, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1050, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1051, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_jump_p_09 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl2a_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   450, 0x80,   0x20,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   451, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x1e00,  42 ),
	CG_FRAME(    0,    0,   5,           0,   452, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x0a00,  42 ),
	CG_FRAME(    0,    0,   4,           0,   453, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0, 0x0a00,   3 ),
};

const Sint32 pl2a_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   450, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_k_00_loop ),
};

/* group 27 15 */
const Sint32 pl2a_grp27_15[] = {
	EF_FRAME(    0,    0,   1,           0,  1052 ),
	EF_FRAME(    0,    0,   1,           0,  1053 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,  1054 ),
	CG_NEXT( pl2a_grp27_15 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl2a_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   454, 0x80,   0x20,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   458, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   459, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2a_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   425, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl2a_jump_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   460, 0x80,   0x20,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x1f00,  46 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   464, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   5,           0,   465, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   6,           0,   466, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl2a_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,   7, END_OF_DATA,   467, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_jump_k_02_loop ),
};

/* group 27 16 */
const Sint32 pl2a_grp27_16[] = {
	EF_FRAME(    0,    0,   2,           0,  1055 ),
	EF_FRAME(    0,    0,   2,           0,  1056 ),
	EF_FRAME(    0,    0,   4,           0,  1057 ),
	EF_FRAME(    0,    0,   5,           0,  1058 ),
	EF_FRAME(    0,    0,   6,           0,  1059 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,  1060 ),
	CG_NEXT( pl2a_grp27_16 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl2a_get_up_00[] = {
	CG_FRAME(    0,    0,   1,           0,   288, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   289, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   290, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   291, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   292, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   293, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl2a_get_up_01[] = {
	CG_FRAME(    0,    0,   1,           0,   311, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   312, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0, 0x2800,   0 ),
	CG_FRAME(    0,    0,   1,           0,   313, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   315, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl2a_get_up_02[] = {
	CG_FRAME(    0,    0,   1,           0,   307, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   307, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0, 0x2800,   0 ),
	CG_FRAME(    0,    0,   3,           0,   308, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   309, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   310, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   311, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   312, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   313, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   315, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   317, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl2a_get_up_03[] = {
	CG_FRAME(    0,    0,   1,           0,   326, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   327, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   328, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   329, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   330, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   331, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   332, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   333, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   334, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl2a_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   614, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   613, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl2a_damage_01[] = {
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   616, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   615, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl2a_damage_02[] = {
	CG_FRAME(    0,    0,   2,           0,   622, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   623, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   624, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   625, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   626, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl2a_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   628, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   627, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl2a_damage_04[] = {
	CG_FRAME(    0,    0,   4,           0,   631, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   630, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   629, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl2a_damage_05[] = {
	CG_FRAME( 0x80,    0,   2,           0,   632, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   2,           0,   633, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   634, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   635, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl2a_damage_06[] = {
	CG_FRAME(    0,    0,   7,           0,   638, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   638, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl2a_damage_07[] = {
	CG_FRAME(    0,    0,  13,           0,   639, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   638, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl2a_damage_08[] = {
	CG_FRAME( 0x80,    0,   2,           0,   640, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   2,           0,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   4,           0,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   4,           0,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   4,           0,   644, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   4,           0,   645, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   646, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2a_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl2a_damage_10[] = {
	CG_FRAME( 0x80,    0,   6,           0,   628, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   627, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl2a_damage_11[] = {
	CG_FRAME( 0x80,    0,   6,           0,   631, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,   2,           0,   630, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   629, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl2a_damage_12[] = {
	CG_FRAME(    0,    0,   3,           0,   632, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   634, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl2a_damage_13[] = {
	CG_FRAME( 0x80,    0,   2,           0,   653, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x82,    0,   4,           0,   654, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl2a_damage_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   655, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2a_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl2a_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   656, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   657, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   663, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   664, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl2a_damage_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   665, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2a_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl2a_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   666, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   667, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl2a_damage_17[] = {
	CG_FRAME(    0,    0,   2,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl2a_damage_18[] = {
	CG_FRAME(    0,    0,   2,           0,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl2a_damage_19[] = {
	CG_FRAME(    0,    0,   2,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl2a_damage_20[] = {
	CG_FRAME(    0,    0,   2,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl2a_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   666, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   667, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl2a_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl2a_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl2a_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl2a_damage_25[] = {
	CG_FRAME(    0,    0,   2,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl2a_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   672, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   673, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   675, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2a_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl2a_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   679, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   684, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   685, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   686, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   687, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   688, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl2a_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   689, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   692, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   693, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl2a_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   702, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl2a_damage_31[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   655, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2a_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl2a_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   704, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   705, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   706, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_dizzy_00 ),
};

/* Char_Win 0: win_time_ko */
const Sint32 pl2a_win_00[] = {
	CG_FRAME(    0,    0,   5,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   248, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   257, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   257, 0x80,   0x00,    0,  0x00,       0x27,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_win_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_win_00_loop ),
};

/* Char_Win 1: lose_time_ko */
const Sint32 pl2a_win_01[] = {
	CG_FRAME(    0,    0,   8,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2a_win_01_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_win_01_loop ),
};

/* Char_Special 23: hail_init (Hail Storm) */
const Sint32 pl2a_special_23[] = {
	CG_FRAME(    0,    0,   1,           0,    82, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    83, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    84, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    85, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    86, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2a_special_23_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    87, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    88, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    88, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_NEXT( pl2a_special_23_loop ),
};

/* Char_Special 24: t_kick_init */
const Sint32 pl2a_special_24[] = {
	CG_FRAME(    0,    0,   6,           0,   426, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1d00,   3 ),
};

const Sint32 pl2a_special_24_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   427, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   428, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl2a_special_24_loop ),
};

/* Char_Special 25: t_kick_move */
const Sint32 pl2a_special_25[] = {
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,    0,  0x00,       0x27,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2a_special_25 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl2a_appear_00[] = {
	CG_FRAME(    0,    1,   3,           0,   270, 0x80,   0x20,    0,  0x00,       0x26,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   3,           0,   271, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   272, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0,   273, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   4,           0,   274, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   5,           0,   275, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   5,           0,   276, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   5,           0,   277, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    9,   6,           0,   278, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,   6,           0,   279, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   11,   8,           0,   280, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   12,   8,           0,   281, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   13,   8,           0,   282, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   14,   8,           0,   283, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   15,   8,           0,   284, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,   8,           0,   285, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   17,   8,           0,   286, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_appear_00_loop[] = {
	CG_FRAME(    0,   18,   8, END_OF_DATA,   287, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_appear_00_loop ),
};

/* Char_Special 0 */
const Sint32 pl2a_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   485, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2400,  49 ),
	CG_FRAME(    0,    0,   5,           0,   485, 0x00,   0x00,  ATK,  0x00,       0x16,    0,   0,     0, 0x2400,  49 ),
	CG_FRAME(    0,    0,   3,           0,   486, 0x00,   0x00,  ATK,  0x00,       0x25,    0,   0,     0, 0x2500,  49 ),
	CG_FRAME(    0,    0,   1,           0,   487, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2600,  49 ),
	CG_FRAME(    0,    0,   2,           0,   487, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2700,  49 ),
	CG_FRAME(    0,    1,   4,           0,   487, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   489, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl2a_special_00 ),
};

/* group 27 21 */
const Sint32 pl2a_grp27_21[] = {
	EF_FRAME(    0,    0,  16, END_OF_DATA,  1079 ),
	CG_NEXT( pl2a_grp27_21 ),
};

/* group 27 22 */
const Sint32 pl2a_grp27_22[] = {
	EF_FRAME(    0,    0,  16, END_OF_DATA,  1080 ),
	CG_NEXT( pl2a_grp27_22 ),
};

/* group 27 23 */
const Sint32 pl2a_grp27_23[] = {
	EF_FRAME(    0,    0,  16, END_OF_DATA,  1081 ),
	CG_NEXT( pl2a_grp27_23 ),
};

/* group 27 24 */
const Sint32 pl2a_grp27_24[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,  1082 ),
	CG_NEXT( pl2a_grp27_24 ),
};

/* Char_Special 1 */
const Sint32 pl2a_special_01[] = {
	CG_FRAME(    0,    0,   4,           0,   485, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2400,  49 ),
	CG_FRAME(    0,    0,  11,           0,   485, 0x00,   0x00,  ATK,  0x00,       0x16,    0,   0,     0, 0x2400,  49 ),
	CG_FRAME(    0,    0,   6,           0,   486, 0x00,   0x00,  ATK,  0x00,       0x25,    0,   0,     0, 0x2500,  49 ),
	CG_FRAME(    0,    0,   1,           0,   487, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2600,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2700,  49 ),
	CG_FRAME(    0,    1,   4,           0,   488, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   489, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl2a_special_01 ),
};

/* Char_Special 2 */
const Sint32 pl2a_special_02[] = {
	CG_FRAME(    0,    0,   4,           0,   485, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2400,  49 ),
	CG_FRAME(    0,    0,   4,           0,   485, 0x00,   0x00,  ATK,  0x00,       0x16,    0,   0,     0, 0x2400,  49 ),
	CG_FRAME(    0,    0,   6,           0,   486, 0x00,   0x00,  ATK,  0x00,       0x25,    0,   0,     0, 0x2500,  49 ),
	CG_FRAME(    0,    0,   1,           0,   487, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2600,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x2700,  49 ),
	CG_FRAME(    0,    1,   4,           0,   488, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   487, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   488, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   489, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl2a_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl2a_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,  1079, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1080, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1081, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_special_03_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1082, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl2a_special_03_loop ),
};

/* Char_Special 26: shl2a_00_init */
const Sint32 pl2a_special_26[] = {
	CG_FRAME(    0,    0,   2,           0,  1083, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
};

const Sint32 pl2a_special_26_loop[] = {
	CG_FRAME(    0,    0,   2,           0,  1084, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,  1085, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,  1086, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,  1087, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1088, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1089, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1090, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1091, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1092, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1093, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1094, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1095, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_special_26_loop ),
};

/* Char_Special 21: sp_dtyphoon (Double Typhoon) */
const Sint32 pl2a_special_21[] = {
	CG_FRAME(    0,    0,   4,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   549, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  15,           0,   550, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   551, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   6,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   6,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    1,   4,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  73 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   553, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2a_special_21 ),
};

/* Char_Special 22 */
const Sint32 pl2a_special_22[] = {
	CG_FRAME(    0,    0,   4,           0,  1079, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1080, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1081, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1082, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1083, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1084, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1085, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1086, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1087, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1088, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1089, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1090, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1091, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1092, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1093, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1094, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_special_22_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1095, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_special_22_loop ),
};

/* Char_Special 4: lstorm_init (Lightning Storm) */
const Sint32 pl2a_special_04[] = {
	CG_FRAME(    0,    0,   2,           0,   508, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   509, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   511, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   513, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   514, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   517, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   518, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   519, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    1,   1,           0,   520, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   521, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   522, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   523, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   525, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   526, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   529, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   530, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   531, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   532, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   533, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   534, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   535, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME( 0x86,    0,   1,           0,   536, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0,    0,   1,           0,   537, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0,    0,   6,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0x80,   0x00,  ATK,  0x00,       0x15,    0,   0,     0, 0x0c00,  51 ),
};

const Sint32 pl2a_special_04_loop[] = {
	CG_FRAME(    0, 0x80,   2,           0,   540, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0,    0,   2,           0,   541, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0, 0x80,   2,           0,   542, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0,    0,   2,           0,   543, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0, 0x80,   2,           0,   544, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0,    0,   2,           0,   545, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0, 0x80,   2,           0,   546, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0, 0x0c00,  51 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   547, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0c00,  51 ),
	CG_NEXT( pl2a_special_04_loop ),
};

/* Char_Special 27: lstorm_move (Lightning Storm) */
const Sint32 pl2a_special_27[] = {
	CG_FRAME(    0,    0,  10,           0,   583, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   584, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   585, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   586, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   587, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   588, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   589, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   590, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   591, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   592, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl2a_special_27 ),
};

/* Char_Special 5 */
const Sint32 pl2a_special_05[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_05 ),
};

/* Char_Special 6 */
const Sint32 pl2a_special_06[] = {
	CG_FRAME(    0,    0,   2,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_06 ),
};

/* Char_Special 7 */
const Sint32 pl2a_special_07[] = {
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl2a_special_08[] = {
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_08 ),
};

/* Char_Special 9 */
const Sint32 pl2a_special_09[] = {
	CG_FRAME(    0,    0,   2,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32788, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl2a_special_10[] = {
	CG_FRAME(    0,    0,   2,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32791, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_10 ),
};

/* Char_Special 11 */
const Sint32 pl2a_special_11[] = {
	CG_FRAME(    0,    0,   2,           0, 32792, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32793, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32794, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_11 ),
};

/* Char_Special 12 */
const Sint32 pl2a_special_12[] = {
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl2a_special_13[] = {
	CG_FRAME(    0,    0,   2,           0, 32798, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32799, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32800, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl2a_special_14[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32801, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl2a_special_15[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32802, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_15 ),
};

/* Char_Special 16 */
const Sint32 pl2a_special_16[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32803, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_16 ),
};

/* Char_Special 17 */
const Sint32 pl2a_special_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32804, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_17 ),
};

/* Char_Special 18 */
const Sint32 pl2a_special_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32805, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_18 ),
};

/* Char_Special 19 */
const Sint32 pl2a_special_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32806, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_19 ),
};

const Sint32 pl2a_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32807, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	0x000057e4,
};

/* Char_Special 28 */
const Sint32 pl2a_special_28[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_28 ),
};

/* Char_Special 29 */
const Sint32 pl2a_special_29[] = {
	CG_FRAME(    0,    0,   2,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32779, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_29 ),
};

/* Char_Special 30 */
const Sint32 pl2a_special_30[] = {
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32782, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_30 ),
};

/* Char_Special 31 */
const Sint32 pl2a_special_31[] = {
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32785, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_31 ),
};

/* Char_Special 32 */
const Sint32 pl2a_special_32[] = {
	CG_FRAME(    0,    0,   2,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32788, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_32 ),
};

/* Char_Special 33 */
const Sint32 pl2a_special_33[] = {
	CG_FRAME(    0,    0,   2,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32791, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_33 ),
};

/* Char_Special 34 */
const Sint32 pl2a_special_34[] = {
	CG_FRAME(    0,    0,   2,           0, 32792, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32793, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32794, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_34 ),
};

/* Char_Special 35 */
const Sint32 pl2a_special_35[] = {
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  52 ),
	CG_NEXT( pl2a_special_35 ),
};

/* Char_Super 0 */
const Sint32 pl2a_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   1,           0,   491, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   491, 0x80,   0x00,  ATK,  0x00,       0x27,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl2a_super_00 ),
};

/* Char_Super 1 */
const Sint32 pl2a_super_01[] = {
	CG_FRAME(    0,    0,   3,           0,   492, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   493, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl2a_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl2a_super_02[] = {
	CG_FRAME(    1,    0,   3,           0,   506, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    0,   3, END_OF_DATA,   507, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl2a_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl2a_super_03[] = {
	CG_FRAME(    2,    0,   3,           0,   504, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    2,    0,   3, END_OF_DATA,   505, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl2a_super_03 ),
};

/* Char_Super 4 */
const Sint32 pl2a_super_04[] = {
	CG_FRAME(    3,    0,   3,           0,   502, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    3,    0,   3, END_OF_DATA,   503, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl2a_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl2a_super_05[] = {
	CG_FRAME(    4,    0,   3,           0,   500, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    4,    0,   3, END_OF_DATA,   501, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl2a_super_05 ),
};

/* Char_Super 6 */
const Sint32 pl2a_super_06[] = {
	CG_FRAME(    5,    0,   3,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    5,    0,   3, END_OF_DATA,   499, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl2a_super_06 ),
};

/* Char_Super 7 */
const Sint32 pl2a_super_07[] = {
	CG_FRAME(    6,    0,   3,           0,   496, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    6,    0,   3, END_OF_DATA,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl2a_super_07 ),
};

/* Char_Super 8 */
const Sint32 pl2a_super_08[] = {
	CG_FRAME(    7,    0,   3,           0,   494, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    7,    0,   3, END_OF_DATA,   495, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl2a_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl2a_super_09[] = {
	CG_FRAME(    0,    0,   3,           0,   582, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   583, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   587, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   588, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   589, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   590, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   591, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   592, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   593, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   594, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   595, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   596, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   597, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   598, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   599, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   600, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   601, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   602, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   603, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   604, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   605, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   606, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   607, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   608, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   609, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   612, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl2a_super_09 ),
};

/* Char_Super 10 */
const Sint32 pl2a_super_10[] = {
	CG_FRAME(    0,    0,   3,           0,   599, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   3,           0,   600, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   3,           0,   601, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   602, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl2a_super_10 ),
};

/* Char_Special 20: lattack_move */
const Sint32 pl2a_special_20[] = {
	CG_FRAME(    0,    0,   1,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   282, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   284, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   286, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   287, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2a_special_20 ),
};

/* Char_Super 11: lattack_move */
const Sint32 pl2a_super_11[] = {
	CG_FRAME(    0,    0,   6,           0,   491, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   194, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl2a_super_11 ),
};

/* Char_Catch 0 */
const Sint32 pl2a_catch_00[] = {
	CG_FRAME(    0,    0,  16,           0,   378, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0, 0x0701,  10 ),
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    1,    1,   5,           0,   469, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   470, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   471, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_catch_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   472, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_catch_00_loop ),
};

/* Char_Catch 1 */
const Sint32 pl2a_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   473, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
};

const Sint32 pl2a_catch_01_loop[] = {
	CG_FRAME(    1,    0,   4,           0,   476, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0, 0x0b04,   0 ),
	CG_FRAME(    0,    2,   4,           0,   477, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0b05,   0 ),
	CG_FRAME(    0,    0,   4,           0,   476, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0, 0x0b04,   0 ),
	CG_FRAME(    0,    2,   4,           0,   477, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0b05,   0 ),
	CG_FRAME(    0,    0,   4,           0,   476, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0, 0x0b04,   0 ),
	CG_FRAME(    0,    2,   4, END_OF_DATA,   477, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_NEXT( pl2a_catch_01_loop ),
};

/* Char_Catch 2: pl2a_catch */
const Sint32 pl2a_catch_02[] = {
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   481, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   482, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   484, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_catch_02 ),
};

/* Char_Caught 0 */
const Sint32 pl2a_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   709, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl2a_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   710, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl2a_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   711, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl2a_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl2a_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   712, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl2a_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   713, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl2a_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   714, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl2a_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   715, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl2a_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl2a_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   716, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl2a_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl2a_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   717, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl2a_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl2a_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   718, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl2a_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   719, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl2a_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   720, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl2a_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   721, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2a_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl2a_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   722, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl2a_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl2a_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   723, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl2a_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl2a_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   724, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl2a_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   725, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl2a_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl2a_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   726, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl2a_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl2a_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   727, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl2a_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   728, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl2a_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl2a_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   729, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl2a_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl2a_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   730, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2a_caught_21 ),
};

/* Char_Fly 0: sp_fly (Flight) */
const Sint32 pl2a_fly_00[] = {
	CG_FRAME(    0,    0,   2,           0,   582, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   8,           0,   583, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   585, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   586, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   587, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   588, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   589, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   590, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   591, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   592, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl2a_fly_00 ),
};

/* Char_Fly 2 */
const Sint32 pl2a_fly_02[] = {
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl2a_fly_02 ),
};

const Sint32 pl2a_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   141, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	0x0000658c,
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl2a_change_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   247, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   250, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   251, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   252, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   253, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   254, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_change_01_loop[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   255, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_change_01_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl2a_change_00[] = {
	CG_FRAME(    0,    0,   2,           0,   247, 0x30,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   251, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   253, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   254, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_change_00_loop[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   255, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl2a_change_02[] = {
	CG_FRAME(    0,    0,   2,           0,   195, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   196, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2a_change_02 ),
};

/* Char_Extra 0
   Char_Extra 1
   Char_Extra 3: sp_chouhatsu (Taunt), fly_start5 */
const Sint32 pl2a_extra_00[] = {
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   257, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,    0,  0x00,       0x27,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_extra_00 ),
};

/* Char_Extra 2: fly_start5
   Char_Effect 0 */
const Sint32 pl2a_extra_02[] = {
	CG_FRAME(    0,    0,   2,           0, 32808, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32809, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32810, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32811, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32812, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl2a_extra_02 ),
};

/* Char_Effect 1 */
const Sint32 pl2a_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0, 32813, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32814, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32815, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32816, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32817, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32818, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32819, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32820, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl2a_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl2a_effect_02[] = {
	CG_FRAME(    0,    0,   2,           0, 32821, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32822, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32823, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32824, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32825, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl2a_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl2a_effect_03[] = {
	CG_FRAME(    0,    0,   2,           0, 32826, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32827, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32828, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32829, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32830, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32831, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0, 32832, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32833, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl2a_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl2a_effect_04[] = {
	CG_FRAME(    0,    0,   2,           0,   951, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   952, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   953, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl2a_effect_05[] = {
	CG_FRAME(    0,    0,   2,           0,   954, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   955, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   956, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   957, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   958, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   959, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   960, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   961, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   962, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl2a_effect_06[] = {
	CG_FRAME(    0,    0,   2,           0,   963, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   964, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   965, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2a_effect_06_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   966, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   967, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   968, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   969, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   970, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   971, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   972, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   973, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   974, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   975, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_06_loop ),
};

/* Char_Effect 7 */
const Sint32 pl2a_effect_07[] = {
	CG_FRAME(    0,    0,   2,           0,  1052, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1053, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1054, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl2a_effect_08[] = {
	CG_FRAME(    0,    0,   2,           0,   999, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1000, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1001, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1002, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl2a_effect_09[] = {
	CG_FRAME(    0,    0,   2,           0,  1061, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1062, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 19 */
const Sint32 pl2a_effect_19[] = {
	CG_FRAME(    0,    0,   6,           0,   966, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   967, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   968, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   969, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   970, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32768, 0x80,   0x00,  ATK,  0x00,       0x25,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 20 */
const Sint32 pl2a_effect_20[] = {
	CG_FRAME(    0,    0,   2,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32771, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl2a_effect_20 ),
};

/* Char_Effect 21: shl2a_01_move */
const Sint32 pl2a_effect_21[] = {
	CG_FRAME(    0,    0,   6,           0, 32772, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA, 32773, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_21 ),
};

/* Char_Effect 10 */
const Sint32 pl2a_effect_10[] = {
	CG_FRAME(    0,    0,   2,           0,  1069, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1070, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1071, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1072, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1073, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1074, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1075, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1076, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1077, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1078, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl2a_effect_11[] = {
	CG_FRAME(    0,    0,   2,           0,  1079, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1080, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1081, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1082, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1083, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1084, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1085, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1086, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1087, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1088, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1089, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1090, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1091, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1092, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1093, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1094, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1095, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl2a_effect_12[] = {
	CG_FRAME(    0,    0,   2,           0,   999, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1000, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1001, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1002, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl2a_effect_13[] = {
	CG_FRAME(    0,    0,   2,           0,  1003, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1004, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1005, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1006, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1007, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1008, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1009, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1010, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1011, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1012, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1013, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1014, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1015, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl2a_effect_14[] = {
	CG_FRAME(    0,    0,   2,           0,  1016, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1017, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1018, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1019, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1020, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1021, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1022, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1023, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1024, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl2a_effect_15[] = {
	CG_FRAME(    0,    0,   2,           0,  1096, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1097, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1098, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1099, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl2a_effect_16[] = {
	CG_FRAME(    0,    0,   2,           0,  1104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl2a_effect_17[] = {
	CG_FRAME(    0,    0,   2,           0,  1112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl2a_effect_18[] = {
	CG_FRAME(    0,    0,   4,           0,   963, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   964, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   965, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2a_effect_06 ),
};

const Sint32 pl2a_unused_02[] = {	/* nothing reaches it */
	0x00040000,
};

const Sint32 pl2a_unused_03[] = {
	0x000000b9,
};

const Sint32 pl2a_unused_04[] = {
	0x00000080,
};

const Sint32 pl2a_unused_05[] = {
	0x00000000,
};

const Sint32 pl2a_unused_06[] = {
	0x00000000,
};

const Sint32 pl2a_unused_07[] = {
	0x00030000,
};

const Sint32 pl2a_unused_08[] = {
	0x00000107,
};

const Sint32 pl2a_unused_09[] = {
	0x00000080,
};

const Sint32 pl2a_unused_10[] = {
	0x00000000,
};

const Sint32 pl2a_unused_11[] = {
	0x00000000,
};

const Sint32 pl2a_unused_12[] = {
	0x00030000,
};

const Sint32 pl2a_unused_13[] = {
	0x0000010d,
};

const Sint32 pl2a_unused_14[] = {
	0x00000080,
};

const Sint32 pl2a_unused_15[] = {
	0x00000000,
};

const Sint32 pl2a_unused_16[] = {
	0x00000000,
};

const Sint32 pl2a_unused_17[] = {
	0x00030000,
};

const Sint32 pl2a_unused_18[] = {
	0x0000012b,
};

const Sint32 pl2a_unused_19[] = {
	0x00000080,
};

const Sint32 pl2a_unused_20[] = {
	0x00000000,
};

const Sint32 pl2a_unused_21[] = {
	0x00000000,
};

const Sint32 pl2a_unused_22[] = {
	0x00030000,
};

const Sint32 pl2a_unused_23[] = {
	0x00000188,
};

const Sint32 pl2a_unused_24[] = {
	0x00000080,
};

const Sint32 pl2a_unused_25[] = {
	0x00000000,
};

const Sint32 pl2a_unused_26[] = {
	0x00000000,
};

const Sint32 pl2a_unused_27[] = {
	0x00030000,
};

const Sint32 pl2a_unused_28[] = {
	0x000002e0,
};

const Sint32 pl2a_unused_29[] = {
	0x00000080,
};

const Sint32 pl2a_unused_30[] = {
	0x00000000,
};

const Sint32 pl2a_unused_31[] = {
	0x00000000,
};

const Sint32 pl2a_unused_32[] = {
	0x00030000,
};

const Sint32 pl2a_unused_33[] = {
	0x0000039d,
};

const Sint32 pl2a_unused_34[] = {
	0x00000080,
};

const Sint32 pl2a_unused_35[] = {
	0x00000000,
};

const Sint32 pl2a_unused_36[] = {
	0x00000000,
};

const Sint32 pl2a_unused_37[] = {
	0x00030000,
};

const Sint32 pl2a_unused_38[] = {
	0x00000385,
};

const Sint32 pl2a_unused_39[] = {
	0x00000080,
};

const Sint32 pl2a_unused_40[] = {
	0x00000000,
};

const Sint32 pl2a_unused_41[] = {
	0x00000000,
};

const Sint32 pl2a_unused_42[] = {
	0x00030000,
};

const Sint32 pl2a_unused_43[] = {
	0x00000386,
};

const Sint32 pl2a_unused_44[] = {
	0x00000080,
};

const Sint32 pl2a_unused_45[] = {
	0x00000000,
};

const Sint32 pl2a_unused_46[] = {
	0x00000000,
};

const Sint32 pl2a_unused_47[] = {
	0x00030000,
};

const Sint32 pl2a_unused_48[] = {
	0x00000387,
};

const Sint32 pl2a_unused_49[] = {
	0x00000080,
};

const Sint32 pl2a_unused_50[] = {
	0x00000000,
};

const Sint32 pl2a_unused_51[] = {
	0x00000000,
};

const Sint32 pl2a_unused_52[] = {
	0x00030000,
};

const Sint32 pl2a_unused_53[] = {
	0x00000388,
};

const Sint32 pl2a_unused_54[] = {
	0x00000080,
};

const Sint32 pl2a_unused_55[] = {
	0x00000000,
};

const Sint32 pl2a_unused_56[] = {
	0x00000000,
};

const Sint32 pl2a_unused_57[] = {
	0x00030000,
};

const Sint32 pl2a_unused_58[] = {
	0x00000389,
};

const Sint32 pl2a_unused_59[] = {
	0x00000080,
};

const Sint32 pl2a_unused_60[] = {
	0x00000000,
};

const Sint32 pl2a_unused_61[] = {
	0x00000000,
};

const Sint32 pl2a_unused_62[] = {
	0x00030000,
};

const Sint32 pl2a_unused_63[] = {
	0x0000038a,
};

const Sint32 pl2a_unused_64[] = {
	0x00000080,
};

const Sint32 pl2a_unused_65[] = {
	0x00000000,
};

const Sint32 pl2a_unused_66[] = {
	0x00000000,
};

const Sint32 pl2a_unused_67[] = {
	0x00030000,
};

const Sint32 pl2a_unused_68[] = {
	0x0000038b,
};

const Sint32 pl2a_unused_69[] = {
	0x00000080,
};

const Sint32 pl2a_unused_70[] = {
	0x00000000,
};

const Sint32 pl2a_unused_71[] = {
	0x00000000,
};

const Sint32 pl2a_unused_72[] = {
	0x80030000,
};

const Sint32 pl2a_unused_73[] = {
	0x0000038c,
};

const Sint32 pl2a_unused_74[] = {
	0x00000080,
};

const Sint32 pl2a_unused_75[] = {
	0x00000000,
};

const Sint32 pl2a_unused_76[] = {
	0x00000000,
};

const Sint32 pl2a_unused_77[] = {
	0x000074dc,
};

const Sint32 pl2a_unused_78[] = {
	0x76280000,
};

const Sint32 pl2a_unused_79[] = {
	0x76a87664,
};

const Sint32 pl2a_unused_80[] = {
	0x773076ec,
};

const Sint32 pl2a_unused_81[] = {
	0x77b07774,
};

const Sint32 pl2a_unused_82[] = {
	0x783877f4,
};

const Sint32 pl2a_unused_83[] = {
	0x78c0787c,
};

const Sint32 pl2a_unused_84[] = {
	0x79407904,
};

/* group 27 26 */
const Sint32 pl2a_grp27_26[] = {
	EF_FRAME(    0,    0,   4,           0,   757 ),
	EF_FRAME(    0,    0,   4,           0,   758 ),
	EF_FRAME(    0,    0,   4,           0,   759 ),
	EF_FRAME(    0,    0,   4,           0,   760 ),
	EF_FRAME(    0,    0,   4,           0,   761 ),
	EF_FRAME(    0,    0,   4,           0,   762 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   763 ),
	CG_NEXT( pl2a_grp27_26 ),
};

/* group 27 27 */
const Sint32 pl2a_grp27_27[] = {
	EF_FRAME(    0,    0,   4,           0,   764 ),
	EF_FRAME(    0,    0,   4,           0,   765 ),
	EF_FRAME(    0,    0,   4,           0,   766 ),
	EF_FRAME(    0,    0,   4,           0,   767 ),
	EF_FRAME(    0,    0,   4,           0,   768 ),
	EF_FRAME(    0,    0,   4,           0,   769 ),
	EF_FRAME(    0,    0,   4,           0,   770 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   771 ),
	CG_NEXT( pl2a_grp27_27 ),
};

/* group 27 28 */
const Sint32 pl2a_grp27_28[] = {
	EF_FRAME(    0,    0,   4,           0,   772 ),
	EF_FRAME(    0,    0,   4,           0,   773 ),
	EF_FRAME(    0,    0,   4,           0,   774 ),
	EF_FRAME(    0,    0,   4,           0,   775 ),
	EF_FRAME(    0,    0,   4,           0,   776 ),
	EF_FRAME(    0,    0,   4,           0,   777 ),
	EF_FRAME(    0,    0,   4,           0,   778 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   779 ),
	CG_NEXT( pl2a_grp27_28 ),
};

/* group 27 29 */
const Sint32 pl2a_grp27_29[] = {
	EF_FRAME(    0,    0,   4,           0,   780 ),
	EF_FRAME(    0,    0,   4,           0,   781 ),
	EF_FRAME(    0,    0,   4,           0,   782 ),
	EF_FRAME(    0,    0,   4,           0,   783 ),
	EF_FRAME(    0,    0,   4,           0,   784 ),
	EF_FRAME(    0,    0,   4,           0,   785 ),
	EF_FRAME(    0,    0,   4,           0,   786 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   787 ),
	CG_NEXT( pl2a_grp27_29 ),
};

/* group 27 30 */
const Sint32 pl2a_grp27_30[] = {
	EF_FRAME(    0,    0,   4,           0,   788 ),
	EF_FRAME(    0,    0,   4,           0,   789 ),
	EF_FRAME(    0,    0,   4,           0,   790 ),
	EF_FRAME(    0,    0,   4,           0,   791 ),
	EF_FRAME(    0,    0,   4,           0,   792 ),
	EF_FRAME(    0,    0,   4,           0,   793 ),
	EF_FRAME(    0,    0,   4,           0,   794 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   795 ),
	CG_NEXT( pl2a_grp27_30 ),
};

/* group 27 31 */
const Sint32 pl2a_grp27_31[] = {
	EF_FRAME(    0,    0,   4,           0,   796 ),
	EF_FRAME(    0,    0,   4,           0,   797 ),
	EF_FRAME(    0,    0,   4,           0,   798 ),
	EF_FRAME(    0,    0,   4,           0,   799 ),
	EF_FRAME(    0,    0,   4,           0,   800 ),
	EF_FRAME(    0,    0,   4,           0,   801 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   802 ),
	CG_NEXT( pl2a_grp27_31 ),
};

/* group 27 32 */
const Sint32 pl2a_grp27_32[] = {
	EF_FRAME(    0,    0,   4,           0,   803 ),
	EF_FRAME(    0,    0,   4,           0,   804 ),
	EF_FRAME(    0,    0,   4,           0,   805 ),
	EF_FRAME(    0,    0,   4,           0,   806 ),
	EF_FRAME(    0,    0,   4,           0,   807 ),
	EF_FRAME(    0,    0,   4,           0,   808 ),
	EF_FRAME(    0,    0,   4,           0,   809 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   810 ),
	CG_NEXT( pl2a_grp27_32 ),
};

/* group 27 33 */
const Sint32 pl2a_grp27_33[] = {
	EF_FRAME(    0,    0,   4,           0,   811 ),
	EF_FRAME(    0,    0,   4,           0,   812 ),
	EF_FRAME(    0,    0,   4,           0,   813 ),
	EF_FRAME(    0,    0,   4,           0,   814 ),
	EF_FRAME(    0,    0,   4,           0,   815 ),
	EF_FRAME(    0,    0,   4,           0,   816 ),
	EF_FRAME(    0,    0,   4,           0,   817 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   818 ),
	CG_NEXT( pl2a_grp27_33 ),
};

/* group 27 34 */
const Sint32 pl2a_grp27_34[] = {
	EF_FRAME(    0,    0,   4,           0,   819 ),
	EF_FRAME(    0,    0,   4,           0,   820 ),
	EF_FRAME(    0,    0,   4,           0,   821 ),
	EF_FRAME(    0,    0,   4,           0,   822 ),
	EF_FRAME(    0,    0,   4,           0,   823 ),
	EF_FRAME(    0,    0,   4,           0,   824 ),
	EF_FRAME(    0,    0,   4,           0,   825 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   826 ),
	CG_NEXT( pl2a_grp27_34 ),
};

/* group 27 35 */
const Sint32 pl2a_grp27_35[] = {
	EF_FRAME(    0,    0,   4,           0,   827 ),
	EF_FRAME(    0,    0,   4,           0,   828 ),
	EF_FRAME(    0,    0,   4,           0,   829 ),
	EF_FRAME(    0,    0,   4,           0,   830 ),
	EF_FRAME(    0,    0,   4,           0,   831 ),
	EF_FRAME(    0,    0,   4,           0,   832 ),
	EF_FRAME(    0,    0,   4,           0,   833 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   834 ),
	CG_NEXT( pl2a_grp27_35 ),
};

/* group 27 36 */
const Sint32 pl2a_grp27_36[] = {
	EF_FRAME(    0,    0,   4,           0,   835 ),
	EF_FRAME(    0,    0,   4,           0,   836 ),
	EF_FRAME(    0,    0,   4,           0,   837 ),
	EF_FRAME(    0,    0,   4,           0,   838 ),
	EF_FRAME(    0,    0,   4,           0,   839 ),
	EF_FRAME(    0,    0,   4,           0,   840 ),
	EF_FRAME(    0,    0,   4,           0,   841 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   842 ),
	CG_NEXT( pl2a_grp27_36 ),
};

/* group 27 37 */
const Sint32 pl2a_grp27_37[] = {
	EF_FRAME(    0,    0,   4,           0,   843 ),
	EF_FRAME(    0,    0,   4,           0,   844 ),
	EF_FRAME(    0,    0,   4,           0,   845 ),
	EF_FRAME(    0,    0,   4,           0,   846 ),
	EF_FRAME(    0,    0,   4,           0,   847 ),
	EF_FRAME(    0,    0,   4,           0,   848 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   849 ),
	CG_NEXT( pl2a_grp27_37 ),
};

/* group 27 38 */
const Sint32 pl2a_grp27_38[] = {
	EF_FRAME(    0,    0,   4,           0,   819 ),
	EF_FRAME(    0,    0,   4,           0,   820 ),
	EF_FRAME(    0,    0,   4,           0,   821 ),
	EF_FRAME(    0,    0,   4,           0,   822 ),
	EF_FRAME(    0,    0,   4,           0,   823 ),
	EF_FRAME(    0,    0,   4,           0,   824 ),
	EF_FRAME(    0,    0,   4,           0,   825 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   826 ),
	CG_NEXT( pl2a_grp27_38 ),
};

const Sint32 pl2a_unused_85[] = {	/* nothing reaches it */
	0x00200000,
};

const Sint32 pl2a_unused_86[] = {
	0x00780054,
};

const Sint32 pl2a_unused_87[] = {
	0x00c0009c,
};

const Sint32 pl2a_unused_88[] = {
	0x00000000,
};

const Sint32 pl2a_unused_89[] = {
	0x00000000,
};

const Sint32 pl2a_unused_90[] = {
	0x00000000,
};

const Sint32 pl2a_unused_91[] = {
	0x00000000,
};

const Sint32 pl2a_unused_92[] = {
	0x00000000,
};

/* group 27 39 */
const Sint32 pl2a_grp27_39[] = {
	EF_FRAME(    0,    0,   2,           0,   731 ),
	EF_FRAME(    0,    0,   2,           0,   732 ),
	EF_FRAME(    0,    0,   2,           0,   733 ),
	EF_FRAME(    0,    0,   2,           0,   734 ),
	EF_FRAME(    0,    0,   2,           0,   735 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   733 ),
	CG_NEXT( pl2a_grp27_39 ),
};

/* group 27 40 */
const Sint32 pl2a_grp27_40[] = {
	EF_FRAME(    0,    0,   2,           0,   737 ),
	EF_FRAME(    0,    0,   2,           0,   738 ),
	EF_FRAME(    0,    0,   2,           0,   739 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   740 ),
	CG_NEXT( pl2a_grp27_40 ),
};

/* group 27 41 */
const Sint32 pl2a_grp27_41[] = {
	EF_FRAME(    0,    0,   2,           0,   745 ),
	EF_FRAME(    0,    0,   2,           0,   746 ),
	EF_FRAME(    0,    0,   2,           0,   747 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   748 ),
	CG_NEXT( pl2a_grp27_41 ),
};

/* group 27 42 */
const Sint32 pl2a_grp27_42[] = {
	EF_FRAME(    0,    0,   2,           0,   749 ),
	EF_FRAME(    0,    0,   2,           0,   750 ),
	EF_FRAME(    0,    0,   2,           0,   751 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   752 ),
	CG_NEXT( pl2a_grp27_42 ),
};

/* group 27 43 */
const Sint32 pl2a_grp27_43[] = {
	EF_FRAME(    0,    0,   2,           0,   753 ),
	EF_FRAME(    0,    0,   2,           0,   754 ),
	EF_FRAME(    0,    0,   2,           0,   755 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   756 ),
	CG_NEXT( pl2a_grp27_43 ),
};

/* group 27 44 */
const Sint32 pl2a_grp27_44[] = {
	EF_FRAME(    0,    0,   2,           0,   741 ),
	EF_FRAME(    0,    0,   2,           0,   742 ),
	EF_FRAME(    0,    0,   2,           0,   743 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   744 ),
	CG_NEXT( pl2a_grp27_44 ),
};

const Sint32 pl2a_unused_93[] = {	/* nothing reaches it */
	CG_NEXT( pl2a_grp27_45 ),
};

const Sint32 pl2a_unused_94[] = {
	CG_NEXT( pl2a_grp27_46 ),
};

const Sint32 pl2a_unused_95[] = {
	CG_NEXT( pl2a_grp27_47 ),
};

/* group 27 45: pls2a_00_init (45 + type) */
const Sint32 pl2a_grp27_45[] = {
	EF_FRAME(    0,    0,   3,           0,  1061 ),
	EF_FRAME(    0,    0,   3,           0,  1062 ),
	EF_FRAME(    0,    0,   3,           0, 32768 ),
	EF_FRAME(    0,    0,   3,           0, 32769 ),
	EF_FRAME(    0,    0,   3,           0, 32770 ),
	EF_FRAME(    0,    0,   3,           0, 32771 ),
	EF_FRAME(    0,    0,   3,           0, 32772 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA, 32773 ),
	CG_NEXT( pl2a_grp27_45 ),
};

/* group 27 46 */
const Sint32 pl2a_grp27_46[] = {
	EF_FRAME(    0,    0,   3,           0,  1069 ),
	EF_FRAME(    0,    0,   3,           0,  1070 ),
	EF_FRAME(    0,    0,   3,           0,  1071 ),
	EF_FRAME(    0,    0,   3,           0,  1072 ),
	EF_FRAME(    0,    0,   3,           0,  1073 ),
	EF_FRAME(    0,    0,   3,           0,  1074 ),
	EF_FRAME(    0,    0,   3,           0,  1075 ),
	EF_FRAME(    0,    0,   3,           0,  1076 ),
	EF_FRAME(    0,    0,   3,           0,  1077 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,  1078 ),
	CG_NEXT( pl2a_grp27_46 ),
};

/* group 27 47 */
const Sint32 pl2a_grp27_47[] = {
	EF_FRAME(    0,    0,   2,           0,   946 ),
	EF_FRAME(    0,    0,   2,           0,   947 ),
	EF_FRAME(    0,    0,   2,           0,   948 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   949 ),
	CG_NEXT( pl2a_grp27_47 ),
};

/* group 27 48 */
const Sint32 pl2a_grp27_48[] = {
	EF_FRAME(    0,    0,   2,           0,   850 ),
	EF_FRAME(    0,    0,   2,           0,   851 ),
	EF_FRAME(    0,    0,   2,           0,   852 ),
	EF_FRAME(    0,    0,   2,           0,   853 ),
	EF_FRAME(    0,    0,   2,           0,   854 ),
	EF_FRAME(    0,    0,   2,           0,   855 ),
	EF_FRAME(    0,    0,   2,           0,   856 ),
	EF_FRAME(    0,    0,   2,           0,   857 ),
	EF_FRAME(    0,    0,   2,           0,   858 ),
	EF_FRAME(    0,    0,   2,           0,   859 ),
	EF_FRAME(    0,    0,   2,           0,   860 ),
	EF_FRAME(    0,    0,   2,           0,   861 ),
	EF_FRAME(    0,    0,   2,           0,   862 ),
	EF_FRAME(    0,    0,   2,           0,   863 ),
	EF_FRAME(    0,    0,   2,           0,   864 ),
	EF_FRAME(    0,    0,   2,           0,   865 ),
	EF_FRAME(    0,    0,   2,           0,   866 ),
	EF_FRAME(    0,    0,   2,           0,   867 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   868 ),
	CG_NEXT( pl2a_grp27_48 ),
};

/* group 27 49 */
const Sint32 pl2a_grp27_49[] = {
	EF_FRAME(    0,    0,   2,           0,   869 ),
	EF_FRAME(    0,    0,   2,           0,   870 ),
	EF_FRAME(    0,    0,   2,           0,   871 ),
	EF_FRAME(    0,    0,   2,           0,   872 ),
	EF_FRAME(    0,    0,   2,           0,   873 ),
	EF_FRAME(    0,    0,   2,           0,   874 ),
	EF_FRAME(    0,    0,   2,           0,   875 ),
	EF_FRAME(    0,    0,   2,           0,   876 ),
	EF_FRAME(    0,    0,   2,           0,   877 ),
	EF_FRAME(    0,    0,   2,           0,   878 ),
	EF_FRAME(    0,    0,   2,           0,   879 ),
	EF_FRAME(    0,    0,   2,           0,   880 ),
	EF_FRAME(    0,    0,   2,           0,   881 ),
	EF_FRAME(    0,    0,   2,           0,   882 ),
	EF_FRAME(    0,    0,   2,           0,   883 ),
	EF_FRAME(    0,    0,   2,           0,   884 ),
	EF_FRAME(    0,    0,   2,           0,   885 ),
	EF_FRAME(    0,    0,   2,           0,   886 ),
	EF_FRAME(    0,    0,   2,           0,   887 ),
	EF_FRAME(    0,    0,   2,           0,   888 ),
	EF_FRAME(    0,    0,   2,           0,   889 ),
	EF_FRAME(    0,    0,   2,           0,   890 ),
	EF_FRAME(    0,    0,   2,           0,   891 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   892 ),
	CG_NEXT( pl2a_grp27_49 ),
};

/* group 27 50 */
const Sint32 pl2a_grp27_50[] = {
	EF_FRAME(    0,    0,   2,           0,   893 ),
	EF_FRAME(    0,    0,   2,           0,   894 ),
	EF_FRAME(    0,    0,   2,           0,   895 ),
	EF_FRAME(    0,    0,   2,           0,   896 ),
	EF_FRAME(    0,    0,   2,           0,   897 ),
	EF_FRAME(    0,    0,   2,           0,   898 ),
	EF_FRAME(    0,    0,   2,           0,   899 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   900 ),
	CG_NEXT( pl2a_grp27_50 ),
};

/* group 27 25
   group 27 51 */
const Sint32 pl2a_grp27_25[] = {
	EF_FRAME(    0,    0,   1,           0,   909 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   910 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   911 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   912 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   913 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   914 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   915 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   916 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   917 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   918 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   919 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   920 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   921 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   922 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   923 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   924 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   925 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   926 ),
};

const Sint32 pl2a_grp27_25_loop[] = {
	EF_FRAME(    0,    0,  -1, END_OF_DATA,    -1 ),
	CG_NEXT( pl2a_grp27_25_loop ),
};

/* Char_Fly 1: act_fly
   Char_Fly 10
   Char_Fly 14 */
const Sint32 pl2a_fly_01[] = {
	CG_FRAME(    0,    0,   6,           0,    66, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,   6,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,   6,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,   6,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,   6,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,   6,           0,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    73, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_NEXT( pl2a_fly_01 ),
};

/* Char_Fly 7
   Char_Fly 8
   Char_Fly 9 */
const Sint32 pl2a_fly_07[] = {
	CG_FRAME(    0,    0,   1,           0,    75, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    76, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    77, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl2a_fly_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0200,  71 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0200,  71 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0200,  71 ),
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0200,  71 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0200,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0200,  71 ),
	CG_NEXT( pl2a_fly_07_loop ),
};

/* Char_Fly 15
   Char_Fly 16
   Char_Fly 17 */
const Sint32 pl2a_fly_15[] = {
	CG_FRAME(    0,    0,   1,           0,    77, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    76, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    75, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl2a_fly_01 ),
};

/* Char_Fly 3
   Char_Fly 4
   Char_Fly 5 */
const Sint32 pl2a_fly_03[] = {
	CG_FRAME(    0,    0,   1,           0,    82, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    83, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    84, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    85, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    86, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl2a_fly_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    87, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  71 ),
	CG_FRAME(    0,    0,   2,           0,    88, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  71 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  71 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  71 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    88, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0300,  71 ),
	CG_NEXT( pl2a_fly_03_loop ),
};

/* Char_Fly 11
   Char_Fly 12
   Char_Fly 13 */
const Sint32 pl2a_fly_11[] = {
	CG_FRAME(    0,    0,   1,           0,    91, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    92, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    93, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    94, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    95, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    96, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1,           0,    97, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    98, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl2a_fly_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl2a_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,   582, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   586, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   587, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   588, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   589, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   590, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   591, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   592, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   593, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   593, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   585, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   585, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2a_change_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   585, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl2a_change_04[] = {
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,    0,  0x00,       0x27,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_change_04 ),
};

/* Char_Extra 6: pl2a_upper */
const Sint32 pl2a_extra_06[] = {
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x00,  ATK,  0x00,       0x25,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1500,   1 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   393, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   394, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,  10,           0,   395, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
};

const Sint32 pl2a_extra_06_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2a_extra_06_loop ),
};
