/* Dhalsim (character 25): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_25      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_25     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl25.c) says which group
                   and number each move uses.
   pl25_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl25_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl25_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl25_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_25
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_25, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_25 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl25_stand_00[], pl25_stand_02[], pl25_stand_03[], pl25_jump_00[],
	pl25_jump_00_loop[], pl25_jump_01[], pl25_jump_01_loop[], pl25_jump_02[], pl25_jump_02_loop[],
	pl25_jump_06[], pl25_jump_09[], pl25_jump_07[], pl25_jump_07_loop[], pl25_jump_08[],
	pl25_jump_08_loop[], pl25_jump_03[], pl25_dash_00[], pl25_dash_00_loop[], pl25_dash_02[],
	pl25_crouch_00[], pl25_crouch_01[], pl25_crouch_02[], pl25_turn_00[], pl25_turn_01[],
	pl25_guard_00[], pl25_guard_01[], pl25_guard_04[], pl25_guard_05[], pl25_stand_p_00[],
	pl25_stand_p_03[], pl25_stand_p_01[], pl25_stand_p_04[], pl25_stand_p_02[], pl25_stand_p_05[],
	pl25_stand_k_00[], pl25_stand_k_03[], pl25_stand_k_01[], pl25_stand_k_04[], pl25_stand_k_02[],
	pl25_stand_k_05[], pl25_air_guard_00[], pl25_air_guard_01[], pl25_crouch_p_00[],
	pl25_crouch_p_03[], pl25_crouch_p_01[], pl25_crouch_p_04[], pl25_crouch_p_02[],
	pl25_crouch_p_05[], pl25_crouch_k_00[], pl25_crouch_k_03[], pl25_crouch_k_03_loop[],
	pl25_crouch_k_06[], pl25_crouch_k_07[], pl25_crouch_k_08[], pl25_crouch_k_01[],
	pl25_crouch_k_04[], pl25_crouch_k_04_loop[], pl25_crouch_k_02[], pl25_crouch_k_05[],
	pl25_crouch_k_05_loop[], pl25_jump_p_00[], pl25_jump_p_06[], pl25_jump_p_01[], pl25_jump_p_07[],
	pl25_jump_p_02[], pl25_jump_p_08[], pl25_jump_p_09[], pl25_jump_p_09_loop[], pl25_jump_p_10[],
	pl25_jump_k_00[], pl25_jump_k_06[], pl25_jump_k_09[], pl25_jump_k_09_loop[], pl25_jump_k_12[],
	pl25_jump_k_01[], pl25_jump_k_07[], pl25_jump_k_10[], pl25_jump_k_10_loop[], pl25_jump_k_13[],
	pl25_jump_k_02[], pl25_jump_k_08[], pl25_jump_k_11[], pl25_jump_k_11_loop[], pl25_jump_k_14[],
	pl25_extra_16[], pl25_get_up_00[], pl25_get_up_01[], pl25_get_up_02[], pl25_get_up_03[],
	pl25_appear_00[], pl25_win_00[], pl25_win_00_loop[], pl25_win_03[], pl25_win_03_loop[],
	pl25_win_01[], pl25_win_01_loop[], pl25_win_02[], pl25_win_02_loop[], pl25_win_04[],
	pl25_effect_00[], pl25_effect_01[], pl25_effect_01_loop[], pl25_change_01[], pl25_change_02[],
	pl25_change_02_loop[], pl25_change_03[], pl25_change_03_loop[], pl25_damage_00[],
	pl25_damage_01[], pl25_damage_02[], pl25_damage_03[], pl25_damage_04[], pl25_damage_05[],
	pl25_damage_06[], pl25_damage_07[], pl25_damage_08[], pl25_damage_10[], pl25_damage_11[],
	pl25_damage_12[], pl25_damage_13[], pl25_damage_09[], pl25_damage_09_loop[], pl25_damage_16[],
	pl25_damage_17[], pl25_damage_18[], pl25_damage_19[], pl25_damage_20[], pl25_damage_21[],
	pl25_damage_22[], pl25_damage_23[], pl25_damage_24[], pl25_damage_25[], pl25_damage_26[],
	pl25_damage_27[], pl25_damage_30[], pl25_damage_32[], pl25_damage_33[], pl25_damage_28[],
	pl25_damage_29[], pl25_damage_31[], pl25_dizzy_00[], pl25_catch_00[], pl25_catch_00_loop[],
	pl25_unused_00[], pl25_unused_01[], pl25_unused_02[], pl25_unused_03[], pl25_unused_04[],
	pl25_unused_05[], pl25_unused_06[], pl25_unused_07[], pl25_unused_08[], pl25_unused_09[],
	pl25_unused_10[], pl25_unused_11[], pl25_unused_12[], pl25_unused_13[], pl25_unused_14[],
	pl25_unused_15[], pl25_unused_16[], pl25_unused_17[], pl25_unused_18[], pl25_unused_19[],
	pl25_unused_20[], pl25_unused_21[], pl25_unused_22[], pl25_unused_23[], pl25_unused_24[],
	pl25_unused_25[], pl25_unused_26[], pl25_unused_27[], pl25_unused_28[], pl25_unused_29[],
	pl25_unused_30[], pl25_unused_31[], pl25_unused_32[], pl25_unused_33[], pl25_unused_34[],
	pl25_unused_35[], pl25_unused_36[], pl25_unused_37[], pl25_unused_38[], pl25_unused_39[],
	pl25_unused_40[], pl25_unused_41[], pl25_unused_42[], pl25_unused_43[], pl25_unused_44[],
	pl25_unused_45[], pl25_unused_46[], pl25_unused_47[], pl25_unused_48[], pl25_unused_49[],
	pl25_unused_50[], pl25_unused_51[], pl25_unused_52[], pl25_unused_53[], pl25_unused_54[],
	pl25_unused_55[], pl25_unused_56[], pl25_unused_57[], pl25_unused_58[], pl25_unused_59[],
	pl25_unused_60[], pl25_unused_61[], pl25_unused_62[], pl25_unused_63[], pl25_unused_64[],
	pl25_unused_65[], pl25_unused_66[], pl25_unused_67[], pl25_unused_68[], pl25_unused_69[],
	pl25_unused_70[], pl25_unused_71[], pl25_unused_72[], pl25_unused_73[], pl25_unused_74[],
	pl25_catch_07[], pl25_catch_01[], pl25_catch_02[], pl25_catch_03[], pl25_catch_04[],
	pl25_catch_04_loop[], pl25_catch_05[], pl25_catch_06[], pl25_unused_75[], pl25_catch_08[],
	pl25_caught_00[], pl25_caught_01[], pl25_caught_02[], pl25_caught_03[], pl25_caught_04[],
	pl25_caught_05[], pl25_caught_06[], pl25_caught_07[], pl25_caught_08[], pl25_caught_09[],
	pl25_caught_10[], pl25_caught_11[], pl25_caught_12[], pl25_caught_13[], pl25_caught_14[],
	pl25_caught_15[], pl25_caught_16[], pl25_caught_17[], pl25_caught_18[], pl25_caught_19[],
	pl25_caught_20[], pl25_caught_21[], pl25_special_00[], pl25_special_11[], pl25_special_07[],
	pl25_extra_00[], pl25_extra_00_loop[], pl25_extra_01[], pl25_special_08[], pl25_special_09[],
	pl25_special_10[], pl25_special_01[], pl25_special_02[], pl25_extra_02[], pl25_extra_02_loop[],
	pl25_extra_03[], pl25_special_03[], pl25_special_04[], pl25_extra_04[], pl25_extra_04_loop[],
	pl25_extra_05[], pl25_extra_06[], pl25_extra_07[], pl25_unused_76[], pl25_extra_08[],
	pl25_extra_09[], pl25_extra_10[], pl25_extra_11[], pl25_extra_12[], pl25_special_12[],
	pl25_extra_13[], pl25_extra_13_loop[], pl25_extra_14[], pl25_special_13[], pl25_extra_15[],
	pl25_change_04[], pl25_change_04_loop[], pl25_special_05[], pl25_special_06[], pl25_super_00[],
	pl25_super_06_loop[], pl25_super_06[], pl25_super_02[], pl25_super_01[], pl25_super_07_loop[],
	pl25_super_07[], pl25_super_03[], pl25_super_04[], pl25_super_08_loop[], pl25_super_08[],
	pl25_super_05[], pl25_grp25_00[], pl25_grp25_01[];
extern const Sint32 *const pl25_stand_tbl[], *const pl25_jump_tbl[], *const pl25_dash_tbl[],
	*const pl25_crouch_tbl[], *const pl25_turn_tbl[], *const pl25_guard_tbl[],
	*const pl25_air_guard_tbl[], *const pl25_stand_p_tbl[], *const pl25_stand_k_tbl[],
	*const pl25_crouch_p_tbl[], *const pl25_crouch_k_tbl[], *const pl25_jump_p_tbl[],
	*const pl25_jump_k_tbl[], *const pl25_damage_tbl[], *const pl25_dizzy_tbl[],
	*const pl25_catch_tbl[], *const pl25_caught_tbl[], *const pl25_get_up_tbl[],
	*const pl25_appear_tbl[], *const pl25_win_tbl[], *const pl25_extra_tbl[],
	*const pl25_special_tbl[], *const pl25_super_tbl[], *const pl25_effect_tbl[],
	*const pl25_change_tbl[], *const pl25_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_25[16] = {
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
const Sint32 *const *const char_tbl_25[] = {
	pl25_stand_tbl, pl25_jump_tbl, pl25_dash_tbl, pl25_crouch_tbl, pl25_turn_tbl, pl25_guard_tbl,
	pl25_air_guard_tbl, pl25_stand_p_tbl, pl25_stand_k_tbl, pl25_crouch_p_tbl, pl25_crouch_k_tbl, pl25_jump_p_tbl,
	pl25_jump_k_tbl, pl25_damage_tbl, pl25_dizzy_tbl, pl25_catch_tbl, pl25_caught_tbl, pl25_get_up_tbl,
	pl25_appear_tbl, pl25_win_tbl, pl25_extra_tbl, pl25_special_tbl, pl25_super_tbl, pl25_effect_tbl,
	pl25_change_tbl, pl25_grp25_tbl, pl25_grp25_tbl, 0,
};

const Sint32 *const pl25_stand_tbl[] = {
	pl25_stand_00, pl25_stand_00, pl25_stand_02, pl25_stand_03, pl25_stand_00, pl25_stand_00,
	pl25_stand_02, pl25_stand_03, 0,
};

const Sint32 *const pl25_jump_tbl[] = {
	pl25_jump_00, pl25_jump_01, pl25_jump_02, pl25_jump_03, pl25_jump_03, pl25_jump_03,
	pl25_jump_06, pl25_jump_07, pl25_jump_08, pl25_jump_09, pl25_jump_09, pl25_jump_09,
	0,
};

const Sint32 *const pl25_dash_tbl[] = {
	pl25_dash_00, pl25_dash_00, pl25_dash_02, pl25_dash_02, 0,
};

const Sint32 *const pl25_crouch_tbl[] = {
	pl25_crouch_00, pl25_crouch_01, pl25_crouch_02, 0,
};

const Sint32 *const pl25_turn_tbl[] = {
	pl25_turn_00, pl25_turn_01, 0,
};

const Sint32 *const pl25_guard_tbl[] = {
	pl25_guard_00, pl25_guard_01, pl25_guard_01, pl25_guard_01, pl25_guard_04, pl25_guard_05,
	pl25_guard_05, pl25_guard_05, 0,
};

const Sint32 *const pl25_air_guard_tbl[] = {
	pl25_air_guard_00, pl25_air_guard_01, pl25_air_guard_01, 0,
};

const Sint32 *const pl25_stand_p_tbl[] = {
	pl25_stand_p_00, pl25_stand_p_01, pl25_stand_p_02, pl25_stand_p_03, pl25_stand_p_04, pl25_stand_p_05,
	0,
};

const Sint32 *const pl25_stand_k_tbl[] = {
	pl25_stand_k_00, pl25_stand_k_01, pl25_stand_k_02, pl25_stand_k_03, pl25_stand_k_04, pl25_stand_k_05,
	0,
};

const Sint32 *const pl25_crouch_p_tbl[] = {
	pl25_crouch_p_00, pl25_crouch_p_01, pl25_crouch_p_02, pl25_crouch_p_03, pl25_crouch_p_04, pl25_crouch_p_05,
	0,
};

const Sint32 *const pl25_crouch_k_tbl[] = {
	pl25_crouch_k_00, pl25_crouch_k_01, pl25_crouch_k_02, pl25_crouch_k_03, pl25_crouch_k_04, pl25_crouch_k_05,
	pl25_crouch_k_06, pl25_crouch_k_07, pl25_crouch_k_08, 0,
};

const Sint32 *const pl25_jump_p_tbl[] = {
	pl25_jump_p_00, pl25_jump_p_01, pl25_jump_p_02, pl25_jump_p_00, pl25_jump_p_01, pl25_jump_p_02,
	pl25_jump_p_06, pl25_jump_p_07, pl25_jump_p_08, pl25_jump_p_09, pl25_jump_p_10, 0,
};

const Sint32 *const pl25_jump_k_tbl[] = {
	pl25_jump_k_00, pl25_jump_k_01, pl25_jump_k_02, pl25_jump_k_00, pl25_jump_k_01, pl25_jump_k_02,
	pl25_jump_k_06, pl25_jump_k_07, pl25_jump_k_08, pl25_jump_k_09, pl25_jump_k_10, pl25_jump_k_11,
	pl25_jump_k_12, pl25_jump_k_13, pl25_jump_k_14, pl25_jump_k_12, pl25_jump_k_13, pl25_jump_k_14,
	0,
};

const Sint32 *const pl25_damage_tbl[] = {
	pl25_damage_00, pl25_damage_01, pl25_damage_02, pl25_damage_03, pl25_damage_04, pl25_damage_05,
	pl25_damage_06, pl25_damage_07, pl25_damage_08, pl25_damage_09, pl25_damage_10, pl25_damage_11,
	pl25_damage_12, pl25_damage_13, pl25_damage_13, pl25_damage_13, pl25_damage_16, pl25_damage_17,
	pl25_damage_18, pl25_damage_19, pl25_damage_20, pl25_damage_21, pl25_damage_22, pl25_damage_23,
	pl25_damage_24, pl25_damage_25, pl25_damage_26, pl25_damage_27, pl25_damage_28, pl25_damage_29,
	pl25_damage_30, pl25_damage_31, pl25_damage_32, pl25_damage_33, pl25_damage_21, pl25_damage_13,
	0,
};

const Sint32 *const pl25_dizzy_tbl[] = {
	pl25_dizzy_00, 0,
};

const Sint32 *const pl25_catch_tbl[] = {
	pl25_catch_00, pl25_catch_01, pl25_catch_02, pl25_catch_03, pl25_catch_04, pl25_catch_05,
	pl25_catch_06, pl25_catch_07, pl25_catch_08, 0,
};

const Sint32 *const pl25_caught_tbl[] = {
	pl25_caught_00, pl25_caught_01, pl25_caught_02, pl25_caught_03, pl25_caught_04, pl25_caught_05,
	pl25_caught_06, pl25_caught_07, pl25_caught_08, pl25_caught_09, pl25_caught_10, pl25_caught_11,
	pl25_caught_12, pl25_caught_13, pl25_caught_14, pl25_caught_15, pl25_caught_16, pl25_caught_17,
	pl25_caught_18, pl25_caught_19, pl25_caught_20, pl25_caught_21, 0,
};

const Sint32 *const pl25_get_up_tbl[] = {
	pl25_get_up_00, pl25_get_up_01, pl25_get_up_02, pl25_get_up_03, 0,
};

const Sint32 *const pl25_appear_tbl[] = {
	pl25_appear_00, 0,
};

const Sint32 *const pl25_win_tbl[] = {
	pl25_win_00, pl25_win_01, pl25_win_02, pl25_win_03, pl25_win_04, pl25_win_04,
	0,
};

const Sint32 *const pl25_extra_tbl[] = {
	pl25_extra_00, pl25_extra_01, pl25_extra_02, pl25_extra_03, pl25_extra_04, pl25_extra_05,
	pl25_extra_06, pl25_extra_07, pl25_extra_08, pl25_extra_09, pl25_extra_10, pl25_extra_11,
	pl25_extra_12, pl25_extra_13, pl25_extra_14, pl25_extra_15, pl25_extra_16, 0,
};

const Sint32 *const pl25_special_tbl[] = {
	pl25_special_00, pl25_special_01, pl25_special_02, pl25_special_03, pl25_special_04, pl25_special_05,
	pl25_special_06, pl25_special_07, pl25_special_08, pl25_special_09, pl25_special_10, pl25_special_11,
	pl25_special_12, pl25_special_13, 0,
};

const Sint32 *const pl25_super_tbl[] = {
	pl25_super_00, pl25_super_01, pl25_super_02, pl25_super_03, pl25_super_04, pl25_super_05,
	pl25_super_06, pl25_super_07, pl25_super_08, 0,
};

const Sint32 *const pl25_effect_tbl[] = {
	pl25_effect_00, pl25_effect_01, pl25_effect_01, pl25_effect_01, pl25_effect_01, pl25_effect_01,
	pl25_effect_01, 0,
};

const Sint32 *const pl25_change_tbl[] = {
	pl25_effect_01, pl25_change_01, pl25_change_02, pl25_change_03, pl25_change_04, 0,
};

const Sint32 *const pl25_grp25_tbl[] = {
	pl25_grp25_00, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01,
	pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01,
	pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01, pl25_grp25_01,
	0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl25_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    28, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    29, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    30, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl25_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,    31, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    32, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    33, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    34, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    35, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl25_stand_03[] = {
	CG_FRAME(    0,    0,    4,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl25_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
};

const Sint32 pl25_jump_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl25_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl25_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    58, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
};

const Sint32 pl25_jump_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl25_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl25_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    58, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
};

const Sint32 pl25_jump_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl25_jump_02_loop ),
};

/* Char_Jump 6 */
const Sint32 pl25_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl25_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl25_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    58, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl25_jump_07_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl25_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    58, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl25_jump_08_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_jump_08_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl25_jump_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_jump_03 ),
};

/* Char_Dash 0: act_dash
   Char_Dash 1: act_back_dash */
const Sint32 pl25_dash_00[] = {
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    0,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    0,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    1,    1,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    1,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    1,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

const Sint32 pl25_dash_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl25_dash_00_loop ),
};

/* Char_Dash 2: dash_move
   Char_Dash 3: noki_move */
const Sint32 pl25_dash_02[] = {
	CG_FRAME(    0,    0,    2,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    0,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    0,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    0,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_dash_02 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl25_crouch_00[] = {
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl25_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    61, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl25_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl25_turn_00[] = {
	CG_FRAME(    0,    0,    3,           0,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl25_turn_01[] = {
	CG_FRAME(    0,    0,    3,           0,    65, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    66, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    67, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl25_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 2
   Char_Guard 3 */
const Sint32 pl25_guard_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_guard_01 ),
};

/* Char_Guard 4 */
const Sint32 pl25_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,    70, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 6
   Char_Guard 7 */
const Sint32 pl25_guard_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    71, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_guard_05 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl25_stand_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   125, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   126, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   127, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   126, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl25_stand_p_03[] = {
	CG_FRAME(    0,    0,    1,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   264, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   262, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   261, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_p_03 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl25_stand_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   266, 0x80,   0x00,              ATK,  0x00,       0x14,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_p_01 ),
};

/* Char_Stand_P 4 */
const Sint32 pl25_stand_p_04[] = {
	CG_FRAME(    0,    0,    1,           0,   129, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   132, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    7,           0,   133, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   134, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    6,           0,   135, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   129, 0x80,   0x0a,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_p_04 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl25_stand_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,    6,           0,   270, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   14,           0,   271, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   217, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_p_02 ),
};

/* Char_Stand_P 5 */
const Sint32 pl25_stand_p_05[] = {
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6,           0,   130, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x0a,              ATK,  0x00,       0x04,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   132, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,    4,           0,   136, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    7,           0,   136, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    1,           0,   137, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   138, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   129, 0x80,   0x0a,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_p_05 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl25_stand_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   142, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   139, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_k_00 ),
};

/* Char_Stand_K 3 */
const Sint32 pl25_stand_k_03[] = {
	CG_FRAME(    0,    0,    1,           0,   139, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   140, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    5,           0,   141, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    1,           0,   142, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   139, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_k_03 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl25_stand_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   294, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   295, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   295, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   294, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_k_01 ),
};

/* Char_Stand_K 4 */
const Sint32 pl25_stand_k_04[] = {
	CG_FRAME(    0,    0,    1,           0,   139, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   144, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   145, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    7,           0,   147, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    1,           0,   148, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    6,           0,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   139, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_k_04 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl25_stand_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   275, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   277, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    5,           0,   278, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   273, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   272, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_k_02 ),
};

/* Char_Stand_K 5 */
const Sint32 pl25_stand_k_05[] = {
	CG_FRAME(    0,    0,    5,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   152, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   153, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    9,           0,   154, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    1,           0,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    8,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   157, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_stand_k_05 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl25_air_guard_00[] = {
	CG_FRAME(    0,    0,    6,           0,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl25_air_guard_01[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl25_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   279, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_crouch_p_00 ),
};

/* Char_Crouch_P 3 */
const Sint32 pl25_crouch_p_03[] = {
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   160, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   161, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   162, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   163, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0,   164, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0,   161, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   160, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl25_crouch_p_03 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl25_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   280, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   279, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_crouch_p_01 ),
};

/* Char_Crouch_P 4 */
const Sint32 pl25_crouch_p_04[] = {
	CG_FRAME(    0,    0,    5,           0,   159, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    1,           0,   160, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    1,           0,   161, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    7,           0,   162, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    1,           0,   163, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    6,           0,   164, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   161, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   160, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl25_crouch_p_04 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl25_crouch_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   279, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    6,           0,   280, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   12,           0,   281, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   279, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_crouch_p_02 ),
};

/* Char_Crouch_P 5 */
const Sint32 pl25_crouch_p_05[] = {
	CG_FRAME(    0,    0,    6,           0,   159, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    1,           0,   160, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    1,           0,   161, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    4,           0,   162, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    7,           0,   162, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    1,           0,   163, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    8,           0,   164, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    2,           0,   161, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   160, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl25_crouch_p_05 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl25_crouch_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   282, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   282, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl25_crouch_k_00 ),
};

/* Char_Crouch_K 3 */
const Sint32 pl25_crouch_k_03[] = {
	CG_FRAME(    0,    0,    2,           0,   165, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x8b,              ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,  96 ),
};

const Sint32 pl25_crouch_k_03_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   166, 0x80,   0x8b,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x8b,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_NEXT( pl25_crouch_k_03_loop ),
};

/* Char_Crouch_K 6: crouch_kick_slide_move (6 + atck_level) */
const Sint32 pl25_crouch_k_06[] = {
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x8b,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl25_crouch_k_06 ),
};

/* Char_Crouch_K 7 */
const Sint32 pl25_crouch_k_07[] = {
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x8b,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   165, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl25_crouch_k_06 ),
};

/* Char_Crouch_K 8 */
const Sint32 pl25_crouch_k_08[] = {
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x8b,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   165, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl25_crouch_k_06 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl25_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   283, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   284, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   285, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   283, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   282, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl25_crouch_k_01 ),
};

/* Char_Crouch_K 4 */
const Sint32 pl25_crouch_k_04[] = {
	CG_FRAME(    0,    0,    2,           0,   165, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x8b,              ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,  96 ),
};

const Sint32 pl25_crouch_k_04_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   166, 0x80,   0x8b,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x8b,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl25_crouch_k_04_loop ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl25_crouch_k_02[] = {
	CG_FRAME(    0,    0,    6,           0,   282, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6,           0,   283, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6,           0,   284, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    6,           0,   285, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    6,           0,   283, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   282, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl25_crouch_k_02 ),
};

/* Char_Crouch_K 5 */
const Sint32 pl25_crouch_k_05[] = {
	CG_FRAME(    0,    0,    1,           0,   165, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    1,           0,   165, 0x80,   0x4b,              ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x8b,              ATK,  0x00,       0x04,    0,   0,     0,      0,  96 ),
};

const Sint32 pl25_crouch_k_05_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   166, 0x80,   0x8b,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x8b,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl25_crouch_k_05_loop ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl25_jump_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   168, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_p_00 ),
};

/* Char_Jump_P 6 */
const Sint32 pl25_jump_p_06[] = {
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   169, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   170, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   171, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   172, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   169, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   168, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_p_06 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl25_jump_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   190, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_p_01 ),
};

/* Char_Jump_P 7 */
const Sint32 pl25_jump_p_07[] = {
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   190, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_p_07 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl25_jump_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   168, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6,           0,   169, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   168, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_p_02 ),
};

/* Char_Jump_P 8 */
const Sint32 pl25_jump_p_08[] = {
	CG_FRAME(    0,    0,    6,           0,   168, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6,           0,   169, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6,           0,   173, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    6,           0,   174, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    6,           0,   175, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    6,           0,   169, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   168, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_p_08 ),
};

/* Char_Jump_P 9 */
const Sint32 pl25_jump_p_09[] = {
	CG_FRAME(    0,    0,   13,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    1,    0,    1,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl25_jump_p_09_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   207, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    4,           0,   208, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   210, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl25_jump_p_09_loop ),
};

/* Char_Jump_P 10: lever_atck0_move */
const Sint32 pl25_jump_p_10[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    59, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_jump_p_10 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl25_jump_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   176, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   177, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    2,           0,   177, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   176, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_k_00 ),
};

/* Char_Jump_K 6 */
const Sint32 pl25_jump_k_06[] = {
	CG_FRAME(    0,    0,    6,           0,   176, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   177, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    5,           0,   178, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    1,           0,   179, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    2,           0,   177, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   176, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_k_06 ),
};

/* Char_Jump_K 9 */
const Sint32 pl25_jump_k_09[] = {
	CG_FRAME(    0,    0,   13,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    1,    0,    1,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl25_jump_k_09_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   290, 0x80,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   291, 0x80,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0x80,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   293, 0x80,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl25_jump_k_09_loop ),
};

/* Char_Jump_K 12: lever_atck1_move
   Char_Jump_K 15: lever_atck1_move */
const Sint32 pl25_jump_k_12[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    59, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_jump_k_12 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl25_jump_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   176, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   177, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   176, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_k_01 ),
};

/* Char_Jump_K 7 */
const Sint32 pl25_jump_k_07[] = {
	CG_FRAME(    0,    0,    6,           0,   176, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   177, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    7,           0,   178, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    1,           0,   179, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   176, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_k_07 ),
};

/* Char_Jump_K 10 */
const Sint32 pl25_jump_k_10[] = {
	CG_FRAME(    0,    0,   13,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    1,    0,    1,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl25_jump_k_10_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   286, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    4,           0,   287, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    4,           0,   288, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   289, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl25_jump_k_10_loop ),
};

/* Char_Jump_K 13: lever_atck1_move
   Char_Jump_K 16: lever_atck1_move */
const Sint32 pl25_jump_k_13[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    59, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_jump_k_13 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl25_jump_k_02[] = {
	CG_FRAME(    0,    1,    4,           0,   181, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   182, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   183, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   184, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   187, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   188, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   189, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_k_02 ),
};

/* Char_Jump_K 8 */
const Sint32 pl25_jump_k_08[] = {
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    3,           0,   182, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   183, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   184, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    9,           0,   185, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    1,           0,   186, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    8,           0,   187, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   189, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_jump_k_08 ),
};

/* Char_Jump_K 11 */
const Sint32 pl25_jump_k_11[] = {
	CG_FRAME(    0,    0,   13,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    1,    0,    1,           0,    50, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl25_jump_k_11_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   211, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   214, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_NEXT( pl25_jump_k_11_loop ),
};

/* Char_Jump_K 14: lever_atck1_move
   Char_Jump_K 17: lever_atck1_move */
const Sint32 pl25_jump_k_14[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    59, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_jump_k_14 ),
};

/* Char_Extra 16: pl25_upper */
const Sint32 pl25_extra_16[] = {
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_extra_16 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl25_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x97,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x97,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   220, 0x80,   0x75,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   222, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   217, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl25_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,    89, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    90, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    91, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    92, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    61, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    60, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl25_get_up_02[] = {
	CG_FRAME(    0,    0,    7,           0,    89, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,    90, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,    91, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,    92, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl25_get_up_03[] = {
	CG_FRAME(    0,    0,    6,           0,    89, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    92, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    90, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    61, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    60, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl25_appear_00[] = {
	CG_FRAME(    0,    0,   40,           0,    80, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,    81, 0x80,   0x35,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,    82, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,    83, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   28,           0,    84, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    85, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_appear_00 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl25_win_00[] = {
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,                0,  0x00,       0x0f,    0,   0,     0,      0,  50 ),
};

const Sint32 pl25_win_00_loop[] = {
	CG_FRAME(    0,    1,    6,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_win_00_loop ),
};

/* Char_Win 3: win_ko_init */
const Sint32 pl25_win_03[] = {
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,    78, 0x80,   0x20,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl25_win_03_loop[] = {
	CG_FRAME(    0,    1,    6,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_win_03_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl25_win_01[] = {
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x36,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl25_win_01_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    77, 0x80,   0x36,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_win_01_loop ),
};

/* Char_Win 2: win_ko_init */
const Sint32 pl25_win_02[] = {
	CG_FRAME(    0,    0,    3,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl25_win_02_loop[] = {
	CG_FRAME(    0,    0,   12,           0,   110, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   111, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   112, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   113, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   114, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   115, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   116, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   115, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   117, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   119, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   120, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   121, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   122, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   120, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   121, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   119, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   115, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   117, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   115, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   116, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   113, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   114, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   111, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   112, 0x80,   0x7a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_win_02_loop ),
};

/* Char_Win 4: lose_time_ko
   Char_Win 5: draw_time */
const Sint32 pl25_win_04[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   365, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_win_04 ),
};

/* Char_Effect 0: shl25_02_init */
const Sint32 pl25_effect_00[] = {
	CG_FRAME(    0,    0,    8,           0,   501, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   502, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   24, END_OF_DATA,   503, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_effect_00 ),
};

/* Char_Effect 1
   Char_Effect 2
   Char_Effect 3
   Char_Effect 4
   Char_Effect 5
   Char_Effect 6 */
const Sint32 pl25_effect_01[] = {
	CG_FRAME(    0,    0,    4,           0,    64, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    62, 0x30,   0x0f,              ATK,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   123, 0x30,   0x0b,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   124, 0x30,   0x0b,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl25_effect_01_loop[] = {
	CG_FRAME(    0,    1,   10, END_OF_DATA,   124, 0x30,   0x0b,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_effect_01_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl25_change_01[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl25_change_02[] = {
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    58, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
};

const Sint32 pl25_change_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl25_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl25_change_03[] = {
	CG_FRAME(    0,    0,    1,           0,    61, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    61, 0x80,   0x40,                0,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,    60, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    76, 0x80,   0x14,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    76, 0x80,   0x14,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   44,           0,    77, 0x80,   0x14,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl25_change_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    86, 0x80,   0x14,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_change_03_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl25_damage_00[] = {
	CG_FRAME( 0x80,    0,    2,           0,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,    6,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl25_damage_01[] = {
	CG_FRAME( 0x80,    0,    2,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,    8,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl25_damage_02[] = {
	CG_FRAME( 0x80,    0,    4,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,    6,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl25_damage_03[] = {
	CG_FRAME( 0x80,    0,    2,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,    6,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl25_damage_04[] = {
	CG_FRAME( 0x80,    0,    2,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,    8,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl25_damage_05[] = {
	CG_FRAME( 0x80,    0,    4,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,    6,           0,   323, 0x80,   0x14,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl25_damage_06[] = {
	CG_FRAME( 0x81,    0,    8,           0,   324, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   324, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl25_damage_07[] = {
	CG_FRAME( 0x80,    0,    4,           0,   324, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x81,    0,    8,           0,   325, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   324, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl25_damage_08[] = {
	CG_FRAME( 0x80,    0,    2,           0,   324, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    4,           0,   325, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x81,    0,    6,           0,   326, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   325, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   324, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl25_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl25_damage_10[] = {
	CG_FRAME( 0x80,    0,    2,           0,   327, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x81,    0,    4,           0,   328, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   327, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl25_damage_11[] = {
	CG_FRAME( 0x80,    0,    1,           0,   327, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   328, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x81,    0,    4,           0,   329, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   328, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   327, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl25_damage_12[] = {
	CG_FRAME( 0x80,    0,    4,           0,   329, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x81,    0,    2,           0,   330, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   329, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   328, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   327, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 35 */
const Sint32 pl25_damage_13[] = {
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x8d,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x8b,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x8d,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   337, 0x80,   0x8d,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl25_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl25_damage_09[] = {
	CG_FRAME(    0,    0,    6,           0,   340, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   341, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   346, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   348, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   349, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   350, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
};

const Sint32 pl25_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   351, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl25_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl25_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   343, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl25_damage_17[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   344, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl25_damage_18[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl25_damage_19[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   346, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl25_damage_20[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   347, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no)
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl25_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   343, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl25_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
};

/* Char_Damage 23 */
const Sint32 pl25_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
};

/* Char_Damage 24 */
const Sint32 pl25_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   346, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
};

/* Char_Damage 25 */
const Sint32 pl25_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   347, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl25_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   348, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   349, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    6,           0,   350, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   351, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl25_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl25_damage_27[] = {
	CG_FRAME(    0,    0,    1,           0,   381, 0x80,   0x20, ATE | CG_ATCK_NG,  0x08,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   381, 0x80,   0x20, ATE | CG_ATCK_NG,  0x08,       0x21,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   385, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x06,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x04,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x02,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0xff,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    2,           0,   225, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    2,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    2,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl25_damage_30[] = {
	CG_FRAME(    0,    0,    1,           0,   381, 0x80,   0x20, ATE | CG_ATCK_NG,  0x08,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   381, 0x80,   0x20, ATE | CG_ATCK_NG,  0x08,       0x21,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   385, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   227, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x06,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x04,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0x02,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x20, ATE | CG_ATCK_NG,  0xff,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   226, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_damage_30 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl25_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl25_damage_33[] = {
	CG_FRAME(    0,    0,    4,           0,   248, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl25_damage_28[] = {
	CG_FRAME(    0,    0,    3,           0,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   355, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   356, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   357, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   358, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl25_damage_29[] = {
	CG_FRAME(    0,    0,    3,           0,   359, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   360, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   361, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   362, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   363, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   364, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl25_damage_31[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   352, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl25_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl25_dizzy_00[] = {
	CG_FRAME(    0,    0,    6,           0,   365, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   366, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   367, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   368, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   369, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   370, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   365, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   366, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   367, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   368, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   369, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   370, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_dizzy_00 ),
};

/* Char_Catch 0: com_catch_set */
const Sint32 pl25_catch_00[] = {
	CG_FRAME(    0,    0,    8,           0,   201, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    8,           0,   202, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
};

const Sint32 pl25_catch_00_loop[] = {
	CG_FRAME(    0,    0,   10,           0,   203, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    1,    4,           0,   205, 0x80,   0x00,                0,  0x00,       0x14,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   206, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   1 ),
	CG_NEXT( pl25_catch_00_loop ),
};

const Sint32 pl25_unused_00[] = {	/* nothing reaches it */
	0x000a0000,
};

const Sint32 pl25_unused_01[] = {
	0x000000cb,
};

const Sint32 pl25_unused_02[] = {
	0x01000080,
};

const Sint32 pl25_unused_03[] = {
	0x00000000,
};

const Sint32 pl25_unused_04[] = {
	0x00010003,
};

const Sint32 pl25_unused_05[] = {
	0x00040000,
};

const Sint32 pl25_unused_06[] = {
	0x000000cc,
};

const Sint32 pl25_unused_07[] = {
	0x00000080,
};

const Sint32 pl25_unused_08[] = {
	0x00000000,
};

const Sint32 pl25_unused_09[] = {
	0x00010004,
};

const Sint32 pl25_unused_10[] = {
	0x0004ff00,
};

const Sint32 pl25_unused_11[] = {
	0x000000cd,
};

const Sint32 pl25_unused_12[] = {
	0x00000080,
};

const Sint32 pl25_unused_13[] = {
	0x00000014,
};

const Sint32 pl25_unused_14[] = {
	0x00010005,
};

const Sint32 pl25_unused_15[] = {
	0x000c0000,
};

const Sint32 pl25_unused_16[] = {
	0x000000ce,
};

const Sint32 pl25_unused_17[] = {
	0x00000080,
};

const Sint32 pl25_unused_18[] = {
	0x00000000,
};

const Sint32 pl25_unused_19[] = {
	0x00010006,
};

const Sint32 pl25_unused_20[] = {
	0x000a0000,
};

const Sint32 pl25_unused_21[] = {
	0x000000cb,
};

const Sint32 pl25_unused_22[] = {
	0x01000080,
};

const Sint32 pl25_unused_23[] = {
	0x00000000,
};

const Sint32 pl25_unused_24[] = {
	0x00010003,
};

const Sint32 pl25_unused_25[] = {
	0x00040000,
};

const Sint32 pl25_unused_26[] = {
	0x000000cc,
};

const Sint32 pl25_unused_27[] = {
	0x00000080,
};

const Sint32 pl25_unused_28[] = {
	0x00000000,
};

const Sint32 pl25_unused_29[] = {
	0x00010004,
};

const Sint32 pl25_unused_30[] = {
	0x0004ff00,
};

const Sint32 pl25_unused_31[] = {
	0x000000cd,
};

const Sint32 pl25_unused_32[] = {
	0x00000080,
};

const Sint32 pl25_unused_33[] = {
	0x00000014,
};

const Sint32 pl25_unused_34[] = {
	0x00010005,
};

const Sint32 pl25_unused_35[] = {
	0x000c0000,
};

const Sint32 pl25_unused_36[] = {
	0x000000ce,
};

const Sint32 pl25_unused_37[] = {
	0x00000080,
};

const Sint32 pl25_unused_38[] = {
	0x00000000,
};

const Sint32 pl25_unused_39[] = {
	0x00010006,
};

const Sint32 pl25_unused_40[] = {
	0x000a0000,
};

const Sint32 pl25_unused_41[] = {
	0x000000cb,
};

const Sint32 pl25_unused_42[] = {
	0x01000080,
};

const Sint32 pl25_unused_43[] = {
	0x00000000,
};

const Sint32 pl25_unused_44[] = {
	0x00010003,
};

const Sint32 pl25_unused_45[] = {
	0x00040000,
};

const Sint32 pl25_unused_46[] = {
	0x000000cc,
};

const Sint32 pl25_unused_47[] = {
	0x00000080,
};

const Sint32 pl25_unused_48[] = {
	0x00000000,
};

const Sint32 pl25_unused_49[] = {
	0x00010004,
};

const Sint32 pl25_unused_50[] = {
	0x0004ff00,
};

const Sint32 pl25_unused_51[] = {
	0x000000cd,
};

const Sint32 pl25_unused_52[] = {
	0x00000080,
};

const Sint32 pl25_unused_53[] = {
	0x00000014,
};

const Sint32 pl25_unused_54[] = {
	0x00010005,
};

const Sint32 pl25_unused_55[] = {
	0x000c0000,
};

const Sint32 pl25_unused_56[] = {
	0x000000ce,
};

const Sint32 pl25_unused_57[] = {
	0x00000080,
};

const Sint32 pl25_unused_58[] = {
	0x00000000,
};

const Sint32 pl25_unused_59[] = {
	0x00010006,
};

const Sint32 pl25_unused_60[] = {
	0x000a0000,
};

const Sint32 pl25_unused_61[] = {
	0x000000cb,
};

const Sint32 pl25_unused_62[] = {
	0x01000080,
};

const Sint32 pl25_unused_63[] = {
	0x00000000,
};

const Sint32 pl25_unused_64[] = {
	0x00010003,
};

const Sint32 pl25_unused_65[] = {
	0x00040000,
};

const Sint32 pl25_unused_66[] = {
	0x000000cc,
};

const Sint32 pl25_unused_67[] = {
	0x00000080,
};

const Sint32 pl25_unused_68[] = {
	0x00000000,
};

const Sint32 pl25_unused_69[] = {
	0x00010004,
};

const Sint32 pl25_unused_70[] = {
	0x0004ff00,
};

const Sint32 pl25_unused_71[] = {
	0x000000cd,
};

const Sint32 pl25_unused_72[] = {
	0x00000080,
};

const Sint32 pl25_unused_73[] = {
	0x00000014,
};

const Sint32 pl25_unused_74[] = {
	0x00010005,
};

/* Char_Catch 7: com_catch_hit */
const Sint32 pl25_catch_07[] = {
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0021,  48 ),
	CG_FRAME(    0,    0,    2,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0022,  48 ),
	CG_FRAME(    0,    1,   24,           0,   267, 0x80,   0x00,              ATK,  0x00,       0x1a,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    8,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_catch_07 ),
};

/* Char_Catch 1: nml_p_set */
const Sint32 pl25_catch_01[] = {
	CG_FRAME(    0,    0,    2,           0,   201, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0007,   1 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   1 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   1 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000a,   1 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000b,   1 ),
	CG_FRAME(    0,    0,    8,           0,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000c,   1 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x00,                0,  0x00,       0x04,    0,   0,     0, 0x000d,   1 ),
	CG_FRAME(    0,    1,   30,           0,   199, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000d,   1 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   200, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_catch_01 ),
};

/* Char_Catch 2: strike_jump (Yoga Strike (hyper)) */
const Sint32 pl25_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   1 ),
	CG_FRAME(    0,    0,    1,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   1 ),
	CG_FRAME(    0,    0,    4,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   1 ),
	CG_FRAME(    0,    0,    4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   1 ),
	CG_FRAME(    0,    0,   -1,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,   -1,    4,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_catch_02 ),
};

/* Char_Catch 3 */
const Sint32 pl25_catch_03[] = {
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0015,   1 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0016,   1 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0017,   1 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0020,   1 ),
	CG_FRAME(    0,    0,    4,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,   -1,    4,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_catch_03 ),
};

/* Char_Catch 4: strike_fall (Yoga Strike (hyper)) */
const Sint32 pl25_catch_04[] = {
	CG_FRAME(    0,   -1,    2,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0015,   1 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x00,                0,  0x00,       0x04,    0,   0,     0, 0x0015,   1 ),
	CG_FRAME(    0,    1,    4,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0016,   1 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0017,   1 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0017,   1 ),
	CG_FRAME(    0,    1,    4,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl25_catch_04_loop[] = {
	CG_FRAME(    0,    0,    3,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_catch_04_loop ),
};

/* Char_Catch 5: jump_p_set */
const Sint32 pl25_catch_05[] = {
	CG_FRAME(    0,    0,    8,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0018,   3 ),
	CG_FRAME(    0,    0,    6,           0,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0019,  50 ),
	CG_FRAME(    0,   -1,    1,           0,   310, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x001a,  50 ),
	CG_FRAME(    0,    0,    7,           0,   310, 0x80,   0x00,                0,  0x00, 0x02 | CSE,    0,   0,     0, 0x001a,  50 ),
	CG_FRAME(    0,    1,    6,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_catch_05 ),
};

/* Char_Catch 6: nml_k_set */
const Sint32 pl25_catch_06[] = {
	CG_FRAME(    0,    0,    8,           0,   201, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    6,           0,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001b,  48 ),
	CG_FRAME(    0,   -1,    1,           0,   298, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x001c,  48 ),
	CG_FRAME(    0,    0,    5,           0,   298, 0x80,   0x00,                0,  0x00, 0x02 | CSE,    0,   0,     0, 0x001c,  48 ),
	CG_FRAME(    0,    1,    6,           0,   298, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_catch_06 ),
};

const Sint32 pl25_unused_75[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    8,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,    6,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,   -1,    6,           0,   304, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x001f,   0 ),
	CG_FRAME(    0,    1,    6,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000041e4,
};

/* Char_Catch 8: strike_catch (Yoga Strike (hyper)) */
const Sint32 pl25_catch_08[] = {
	CG_FRAME(    0,    0,    1,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    0,    3,           0,   250, 0x80,   0x00,                0,  0x00, 0x16 | CSE,    0,   0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0017,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0020,   1 ),
	CG_NEXT( pl25_catch_08 ),
};

/* Char_Caught 0 */
const Sint32 pl25_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl25_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl25_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl25_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl25_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl25_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   323, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl25_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   324, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl25_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   325, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl25_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   326, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl25_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl25_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   329, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl25_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl25_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl25_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   343, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl25_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl25_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl25_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl25_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl25_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   358, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl25_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   364, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl25_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   352, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl25_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl25_caught_21 ),
};

/* Char_Special 0 */
const Sint32 pl25_special_00[] = {
	CG_FRAME(    0,    0,    1,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   215, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   16,           0,   219, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   16,           0,   219, 0x80,   0x00,              ATE,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    64, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_special_00 ),
};

/* Char_Special 11 */
const Sint32 pl25_special_11[] = {
	CG_FRAME(    0,    0,    1,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   215, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   48,           0,   219, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    64, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl25_special_11 ),
};

/* Char_Special 7 */
const Sint32 pl25_special_07[] = {
	CG_FRAME(    0,    0,    1,           0,    50, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    3,           0,    50, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,   16,           0,   237, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   16,           0,   237, 0x80,   0x00,              ATE,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    50, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_special_07 ),
};

/* Char_Extra 0: shl25_00_init */
const Sint32 pl25_extra_00[] = {
	CG_FRAME(    0,    0,    2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
};

const Sint32 pl25_extra_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   476, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   477, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl25_extra_00_loop ),
};

/* Char_Extra 1: shl25_00_move */
const Sint32 pl25_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0,   479, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   480, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   481, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   482, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   483, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   484, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_extra_01 ),
};

/* Char_Special 8 */
const Sint32 pl25_special_08[] = {
	CG_FRAME(    0, 0x80,   60,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6,           0,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6,           0,   298, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6,           0,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6,           0,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_special_08 ),
};

/* Char_Special 9 */
const Sint32 pl25_special_09[] = {
	CG_FRAME(    0, 0x80,   60,           0,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl25_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl25_special_10[] = {
	CG_FRAME(    0, 0x80,   60,           0,   308, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6,           0,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl25_special_10 ),
};

/* Char_Special 1 */
const Sint32 pl25_special_01[] = {
	CG_FRAME(    0,    0,    4,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    64, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_special_01 ),
};

/* Char_Special 2 */
const Sint32 pl25_special_02[] = {
	CG_FRAME(    0,    0,    4,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   18,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   219, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    59, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    64, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_special_02 ),
};

/* Char_Extra 2: shl25_01_init, fly_start5 */
const Sint32 pl25_extra_02[] = {
	CG_FRAME(    0,    0,    1,           0,   454, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    2,           0,   454, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    3,           0,   455, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    3,           0,   456, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    3,           0,   457, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
};

const Sint32 pl25_extra_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0,   459, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl25_extra_02_loop ),
};

/* Char_Extra 3: shl25_01_move, fly_start5 */
const Sint32 pl25_extra_03[] = {
	CG_FRAME(    0,    0,    3,           0,   459, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   465, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   466, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   467, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   468, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   469, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   470, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_extra_03 ),
};

/* Char_Special 3 */
const Sint32 pl25_special_03[] = {
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   222, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   224, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    59, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    64, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_special_03 ),
};

/* Char_Special 4 */
const Sint32 pl25_special_04[] = {
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   222, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   224, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    59, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    64, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_special_04 ),
};

/* Char_Extra 4: shl25_03_init */
const Sint32 pl25_extra_04[] = {
	CG_FRAME(    0,    0,    3,           0,   485, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    3,           0,   486, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    3,           0,   487, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    3,           0,   488, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0,   489, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
};

const Sint32 pl25_extra_04_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0,   491, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0,   493, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_NEXT( pl25_extra_04_loop ),
};

/* Char_Extra 5: shl25_03_move */
const Sint32 pl25_extra_05[] = {
	CG_FRAME(    0,    0,    3,           0,   495, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   496, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   497, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   498, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   499, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   500, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_extra_05 ),
};

/* Char_Extra 6: shl25_04_init */
const Sint32 pl25_extra_06[] = {
	CG_FRAME(    0,    0,    3,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32785, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
};

/* Char_Extra 7 */
const Sint32 pl25_extra_07[] = {
	CG_FRAME(    0,    0,    3,           0, 32786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0, 32787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_extra_06 ),
};

const Sint32 pl25_unused_76[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    3,           0,   434, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   435, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   436, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   437, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_extra_07 ),
};

/* Char_Extra 8 */
const Sint32 pl25_extra_08[] = {
	CG_FRAME(    0,    0,    4,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    2,    4,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    4,    4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    6,    4,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    8,    4,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,   10,    4, END_OF_DATA,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl25_extra_08 ),
};

/* Char_Extra 9 */
const Sint32 pl25_extra_09[] = {
	CG_FRAME(    0,    8,    4,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,   12,    4,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,   14,    4,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,   16,    4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,   18,    4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    2,    4,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    4,    4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    6,    4,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    8,    4,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,   10,    4, END_OF_DATA,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl25_extra_09 ),
};

/* Char_Extra 10 */
const Sint32 pl25_extra_10[] = {
	CG_FRAME(    0,    0,   60, END_OF_DATA,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl25_extra_10 ),
};

/* Char_Extra 11 */
const Sint32 pl25_extra_11[] = {
	CG_FRAME(    0,    0,   60, END_OF_DATA,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl25_extra_11 ),
};

/* Char_Extra 12 */
const Sint32 pl25_extra_12[] = {
	CG_FRAME(    0,    0,   60, END_OF_DATA,   308, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl25_extra_12 ),
};

/* Char_Special 12 */
const Sint32 pl25_special_12[] = {
	CG_FRAME(    0,    0,    2,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl25_special_12 ),
};

/* Char_Extra 13: t_kick_init */
const Sint32 pl25_extra_13[] = {
	CG_FRAME(    0,    0,    4,           0,    50, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl25_extra_13_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   290, 0xa0,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    4,           0,   291, 0xa0,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0xa0,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   293, 0xa0,   0x6f,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl25_extra_13_loop ),
};

/* Char_Extra 14: t_kick_move */
const Sint32 pl25_extra_14[] = {
	CG_FRAME(    0,    0,    1,           0,    61, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3,           0,    61, 0x80,   0x40,              ATE,  0x00,       0x0a,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,    60, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,    59, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x13,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,    76, 0x80,   0x14,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,    4,           0,    76, 0x80,   0x14,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   18,           0,    77, 0x80,   0x14,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    86, 0x80,   0x14,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_extra_14 ),
};

/* Char_Special 13: snapback_init (Snapback) */
const Sint32 pl25_special_13[] = {
	CG_FRAME(    0,    0,   10,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,    4,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   275, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   276, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   277, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    5,           0,   278, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   273, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   272, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_special_13 ),
};

/* Char_Extra 15: chouhatsu_init (Taunt) */
const Sint32 pl25_extra_15[] = {
	CG_FRAME(    0,    0,    1,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   96, END_OF_DATA,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_extra_15 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl25_change_04[] = {
	CG_FRAME(    0,    0,    1,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    76, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl25_change_04_loop[] = {
	CG_FRAME(    0,    0,   33, END_OF_DATA,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_change_04_loop ),
};

/* Char_Special 5: teleport_init (Yoga Teleport) */
const Sint32 pl25_special_05[] = {
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,       0x14,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   225, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x20,                0,  0x00,       0x21,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x02,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x04,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x06,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x08,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   232, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl25_special_05 ),
};

/* Char_Special 6: teleport_move (Yoga Teleport) */
const Sint32 pl25_special_06[] = {
	CG_FRAME(    0,    0,    1,           0,   230, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   230, 0x80,   0x20,                0,  0x00,       0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x08,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x06,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x04,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0x02,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x20,                0,  0xff,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   225, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl25_special_06 ),
};

/* Char_Super 0: inferno_init (Yoga Inferno (hyper)) */
const Sint32 pl25_super_00[] = {
	CG_FRAME(    0,    0,    8,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   215, 0x80,   0x00,              ATK,  0x00,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   238, 0x80,   0x00,              ATK,  0x00,       0x19,    0,   0,     0,      0,   1 ),
};

const Sint32 pl25_super_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   239, 0x80,   0x00,              ATK,  0x00,       0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_super_06_loop ),
};

/* Char_Super 6 */
const Sint32 pl25_super_06[] = {
	CG_FRAME(    0,    0,    8,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   215, 0x80,   0x00,              ATK,  0x00,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_super_06_loop ),
};

/* Char_Super 2 */
const Sint32 pl25_super_02[] = {
	CG_FRAME(    0,    0,    4,           0,   241, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   48, END_OF_DATA,   241, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_super_02 ),
};

/* Char_Super 1: strike_init (Yoga Strike (hyper)) */
const Sint32 pl25_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    77, 0x80,   0x20,                0,  0x01,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,    52, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    52, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    54, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,    55, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,    56, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,    57, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,    58, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
};

const Sint32 pl25_super_07_loop[] = {
	CG_FRAME(    0,    0,    3,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_super_07_loop ),
};

/* Char_Super 7 */
const Sint32 pl25_super_07[] = {
	CG_FRAME(    0,    0,   12,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    77, 0x80,   0x20,                0,  0x01,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,    52, 0x80,   0x20,                0,  0x01, 0x16 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    52, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    54, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,    55, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,    56, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,    57, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,    58, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl25_super_07_loop ),
};

/* Char_Super 3: strike_jump (Yoga Strike (hyper)) */
const Sint32 pl25_super_03[] = {
	CG_FRAME(    0,    0,   12, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl25_super_03 ),
};

/* Char_Super 4: jinferno_init (Yoga Inferno in the air (hyper)) */
const Sint32 pl25_super_04[] = {
	CG_FRAME(    0,    0,    8,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   234, 0x80,   0x00,              ATK,  0x00,       0x18,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   235, 0x80,   0x00,              ATK,  0x00,       0x19,    0,   0,     0,      0,   3 ),
};

const Sint32 pl25_super_08_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   236, 0x80,   0x00,              ATK,  0x00,       0x20,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    4,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    4,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    4,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_super_08_loop ),
};

/* Char_Super 8 */
const Sint32 pl25_super_08[] = {
	CG_FRAME(    0,    0,    8,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   15,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   234, 0x80,   0x00,              ATK,  0x00,       0x19,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_super_08_loop ),
};

/* Char_Super 5 */
const Sint32 pl25_super_05[] = {
	CG_FRAME(    0,    0,   48, END_OF_DATA,   237, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl25_super_05 ),
};

/* group 25 0
   Char_Fly 0: float_init (Yoga Float) */
const Sint32 pl25_grp25_00[] = {
	CG_FRAME(    0,    0,   10,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl25_grp25_00 ),
};

/* group 25 1
   group 25 2
   group 25 3
   group 25 4
   group 25 5
   group 25 6 */
const Sint32 pl25_grp25_01[] = {
	CG_FRAME(    0,    0,    3,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl25_grp25_01 ),
};
