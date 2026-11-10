/* Abyss (third form) (character 1a): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_1a      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_1a     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl1a.c) says which group
                   and number each move uses.
   pl1a_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl1a_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl1a_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl1a_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_1a
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_1a, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_1a */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl1a_stand_00[], pl1a_crouch_01[], pl1a_crouch_01_loop[], pl1a_stand_04[],
	pl1a_jump_00[], pl1a_stand_p_00[], pl1a_damage_01[], pl1a_air_guard_00[],
	pl1a_air_guard_00_loop[], pl1a_special_00[], pl1a_special_01[], pl1a_special_01_loop[],
	pl1a_catch_01[], pl1a_stand_02[], pl1a_special_04[], pl1a_special_05[], pl1a_special_06[],
	pl1a_special_06_loop[], pl1a_special_07[], pl1a_effect_00[], pl1a_effect_01[], pl1a_effect_02[],
	pl1a_effect_03[], pl1a_effect_04[], pl1a_effect_05[], pl1a_effect_06[], pl1a_effect_07[],
	pl1a_effect_08[], pl1a_effect_11[], pl1a_effect_12[], pl1a_effect_13[], pl1a_effect_14[],
	pl1a_effect_09[], pl1a_effect_10[], pl1a_effect_15[], pl1a_effect_16[], pl1a_effect_16_loop[],
	pl1a_effect_17[], pl1a_win_00[], pl1a_win_00_loop[], pl1a_win_01[], pl1a_win_02[], pl1a_win_03[],
	pl1a_win_04[], pl1a_catch_00[], pl1a_catch_00_loop[], pl1a_catch_02[], pl1a_catch_02_loop[],
	pl1a_win_07[], pl1a_win_07_loop[], pl1a_grp27_00[], pl1a_grp27_01[], pl1a_grp27_03[],
	pl1a_grp27_02[], pl1a_grp27_04[], pl1a_grp27_05[];
extern const Sint32 *const pl1a_stand_tbl[], *const pl1a_jump_tbl[], *const pl1a_dash_tbl[],
	*const pl1a_crouch_tbl[], *const pl1a_turn_tbl[], *const pl1a_guard_tbl[],
	*const pl1a_air_guard_tbl[], *const pl1a_stand_p_tbl[], *const pl1a_stand_k_tbl[],
	*const pl1a_crouch_p_tbl[], *const pl1a_crouch_k_tbl[], *const pl1a_jump_p_tbl[],
	*const pl1a_jump_k_tbl[], *const pl1a_damage_tbl[], *const pl1a_dizzy_tbl[],
	*const pl1a_catch_tbl[], *const pl1a_caught_tbl[], *const pl1a_get_up_tbl[],
	*const pl1a_appear_tbl[], *const pl1a_win_tbl[], *const pl1a_extra_tbl[],
	*const pl1a_special_tbl[], *const pl1a_super_tbl[], *const pl1a_effect_tbl[],
	*const pl1a_change_tbl[], *const pl1a_grp25_tbl[], *const pl1a_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_1a[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -16384, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -14336, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -12288, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -10240, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_1a[] = {
	pl1a_stand_tbl, pl1a_jump_tbl, pl1a_dash_tbl, pl1a_crouch_tbl, pl1a_turn_tbl, pl1a_guard_tbl,
	pl1a_air_guard_tbl, pl1a_stand_p_tbl, pl1a_stand_k_tbl, pl1a_crouch_p_tbl, pl1a_crouch_k_tbl, pl1a_jump_p_tbl,
	pl1a_jump_k_tbl, pl1a_damage_tbl, pl1a_dizzy_tbl, pl1a_catch_tbl, pl1a_caught_tbl, pl1a_get_up_tbl,
	pl1a_appear_tbl, pl1a_win_tbl, pl1a_extra_tbl, pl1a_special_tbl, pl1a_super_tbl, pl1a_effect_tbl,
	pl1a_change_tbl, pl1a_grp25_tbl, pl1a_grp25_tbl, pl1a_grp27_tbl, 0,
};

const Sint32 *const pl1a_stand_tbl[] = {
	pl1a_stand_00, pl1a_stand_00, pl1a_stand_02, pl1a_stand_02, pl1a_stand_04, pl1a_stand_00,
	pl1a_stand_02, pl1a_stand_02, 0,
};

const Sint32 *const pl1a_jump_tbl[] = {
	pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00,
	pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00,
	0,
};

const Sint32 *const pl1a_dash_tbl[] = {
	pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, 0,
};

const Sint32 *const pl1a_crouch_tbl[] = {
	pl1a_stand_02, pl1a_crouch_01, pl1a_stand_04, 0,
};

const Sint32 *const pl1a_turn_tbl[] = {
	pl1a_jump_00, pl1a_jump_00, 0,
};

const Sint32 *const pl1a_guard_tbl[] = {
	pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00, pl1a_jump_00,
	pl1a_jump_00, pl1a_jump_00, 0,
};

const Sint32 *const pl1a_air_guard_tbl[] = {
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_stand_p_tbl[] = {
	pl1a_stand_p_00, pl1a_stand_p_00, pl1a_stand_p_00, 0,
};

const Sint32 *const pl1a_stand_k_tbl[] = {
	pl1a_stand_p_00, pl1a_stand_p_00, pl1a_stand_p_00, 0,
};

const Sint32 *const pl1a_crouch_p_tbl[] = {
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_crouch_k_tbl[] = {
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_jump_p_tbl[] = {
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00,
	0,
};

const Sint32 *const pl1a_jump_k_tbl[] = {
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00,
	0,
};

const Sint32 *const pl1a_damage_tbl[] = {
	pl1a_stand_p_00, pl1a_damage_01, pl1a_damage_01, pl1a_stand_p_00, pl1a_damage_01, pl1a_damage_01,
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00,
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00,
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00,
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00,
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_dizzy_tbl[] = {
	pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_catch_tbl[] = {
	pl1a_catch_00, pl1a_catch_01, pl1a_catch_02, 0,
};

const Sint32 *const pl1a_caught_tbl[] = {
	pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_get_up_tbl[] = {
	pl1a_air_guard_00, pl1a_air_guard_00, pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_appear_tbl[] = {
	pl1a_air_guard_00, 0,
};

const Sint32 *const pl1a_win_tbl[] = {
	pl1a_win_00, pl1a_win_01, pl1a_win_02, pl1a_win_03, pl1a_win_04, pl1a_catch_00,
	pl1a_catch_02, pl1a_win_07, pl1a_win_07, 0,
};

const Sint32 *const pl1a_extra_tbl[] = {
	pl1a_stand_00, 0,
};

const Sint32 *const pl1a_special_tbl[] = {
	pl1a_special_00, pl1a_special_01, pl1a_catch_01, pl1a_stand_02, pl1a_special_04, pl1a_special_05,
	pl1a_special_06, pl1a_special_07, 0,
};

const Sint32 *const pl1a_super_tbl[] = {
	pl1a_stand_00, 0,
};

const Sint32 *const pl1a_effect_tbl[] = {
	pl1a_effect_00, pl1a_effect_01, pl1a_effect_02, pl1a_effect_03, pl1a_effect_04, pl1a_effect_05,
	pl1a_effect_06, pl1a_effect_07, pl1a_effect_08, pl1a_effect_09, pl1a_effect_10, pl1a_effect_11,
	pl1a_effect_12, pl1a_effect_13, pl1a_effect_14, pl1a_effect_15, pl1a_effect_16, pl1a_effect_17,
	0,
};

const Sint32 *const pl1a_change_tbl[] = {
	pl1a_stand_00, 0,
};

const Sint32 *const pl1a_grp25_tbl[] = {
	pl1a_stand_00, 0,
};

const Sint32 *const pl1a_grp27_tbl[] = {
	pl1a_grp27_00, pl1a_grp27_01, pl1a_grp27_02, pl1a_grp27_03, pl1a_grp27_04, pl1a_grp27_05,
	0,
};

	/*        prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 5
   Char_Extra 0
   Char_Super 0
   Char_Change 0: act_change_tag_init */
const Sint32 pl1a_stand_00[] = {
	CG_FRAME(   0,    0,   6,           0,    16, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    17, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    18, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    19, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    20, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    21, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    22, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    23, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    24, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    25, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    26, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    27, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    28, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    29, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    30, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    31, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    32, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    33, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    34, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    19, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    20, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    21, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    22, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    23, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    35, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    36, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    37, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    39, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    40, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    41, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    42, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    43, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    44, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    45, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    46, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    47, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    48, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    49, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    50, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    51, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    52, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    53, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    54, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    55, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    56, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    57, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    58, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    59, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    60, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    61, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    62, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    63, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    64, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    65, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    66, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    24, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    25, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    26, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    27, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    28, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    29, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    30, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    31, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    32, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    33, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    34, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    38, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    39, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    40, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    41, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    42, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    43, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    44, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    45, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    46, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    47, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    48, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    49, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    50, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    51, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    52, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    53, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    54, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    55, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    56, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    57, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    58, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    59, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    60, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    61, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    62, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    63, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    64, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,    65, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,    66, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_stand_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl1a_crouch_01[] = {
	CG_FRAME(   0,    0,   6,           0,    67, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    68, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    69, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    70, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    71, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    72, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    73, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    74, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    75, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    76, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    77, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    78, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    79, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    80, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    81, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    82, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    84, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    85, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    86, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    87, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    88, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    89, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    90, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    91, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    92, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    93, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   1,    0,  22,           0,    94, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1a_crouch_01_loop[] = {
	CG_FRAME(   0,    1,   6, END_OF_DATA,    94, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_crouch_01_loop ),
};

/* Char_Stand 4: sp_win (sinks and comes up before the opponent (win)), sp_win_voice (sinks and comes up before the opponent (win, with a voice)), act_walk
   Char_Crouch 2: stomp_land (a leap that comes down on the opponent), jump_end (jumps at the opponent, firing), act_stand_up, act_crouch */
const Sint32 pl1a_stand_04[] = {
	CG_FRAME(   0,    0,   4,           0,    95, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,    96, 0x40,   0x00,    0,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,    97, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,    98, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,    99, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   100, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   101, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   102, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   103, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   104, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   105, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   106, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   107, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   108, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   109, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   110, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   111, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   112, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   113, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   114, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   115, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_stand_04 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2
   Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl1a_jump_00[] = {
	CG_FRAME(   0,    0,   1, END_OF_DATA,    16, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_stand_00 ),
};

/* Char_Stand_P 0 (light)
   Char_Stand_P 1 (medium)
   Char_Stand_P 2 (heavy)
   Char_Stand_K 0 (light)
   Char_Stand_K 1 (medium)
   Char_Stand_K 2 (heavy) */
const Sint32 pl1a_stand_p_00[] = {
	CG_FRAME(   0,    0,   3,           0,   215, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   216, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   217, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   216, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA,   216, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_stand_p_00 ),
};

/* Char_Damage 1
   Char_Damage 2
   Char_Damage 4
   Char_Damage 5 */
const Sint32 pl1a_damage_01[] = {
	CG_FRAME(   0,    0,   2,           0,   218, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   219, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   220, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   219, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   220, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   219, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   220, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   221, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   222, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   223, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   224, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_damage_01 ),
};

/* Char_Air_Guard 0: act_air_guard
   Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set
   Char_Crouch_P 0 (light)
   Char_Crouch_P 1 (medium)
   Char_Crouch_P 2 (heavy) */
const Sint32 pl1a_air_guard_00[] = {
	CG_FRAME(   0,    0,   1,           0,    95, 0x40,   0x00,    0,  0x00,       0x19,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,    95, 0x40,   0x00,    0,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,    96, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,    97, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,    98, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,    99, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   100, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   101, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   102, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   103, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   104, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   105, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   106, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   107, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   108, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   109, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   110, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   111, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   112, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   113, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   114, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,   115, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1a_air_guard_00_loop[] = {
	CG_FRAME(   0,    0,   5, END_OF_DATA,    16, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_air_guard_00_loop ),
};

/* Char_Special 0: sp_storm (a storm of shots) */
const Sint32 pl1a_special_00[] = {
	CG_FRAME(   0,    0,   4,           0,   161, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   162, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   8,           0,   163, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   8,           0,   163, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   164, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   165, 0x40,   0x00,  ATK,  0x01,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   166, 0x40,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   167, 0x40,   0x00,  ATK,  0x03,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   168, 0x40,   0x00,  ATK,  0x04,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   169, 0x40,   0x00,  ATK,  0x05,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   170, 0x40,   0x00,  ATK,  0x06,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   171, 0x40,   0x00,  ATK,  0x07,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   172, 0x40,   0x00,  ATK,  0x08,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   173, 0x40,   0x00,  ATK,  0x09,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   174, 0x40,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   175, 0x40,   0x00,  ATK,  0x0b,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   174, 0x40,   0x00,  ATK,  0x0c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   175, 0x40,   0x00,  ATK,  0x0d,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   174, 0x40,   0x00,  ATK,  0x0e,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   175, 0x40,   0x00,  ATK,  0x0f,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   174, 0x40,   0x00,  ATK,  0x10,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   175, 0x40,   0x00,  ATK,  0x11,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   174, 0x40,   0x00,  ATK,  0x12,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   175, 0x40,   0x00,  ATK,  0x13,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   174, 0x40,   0x00,  ATK,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   175, 0x40,   0x00,  ATK,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   174, 0x40,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   175, 0x40,   0x00,  ATK,  0xff,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   176, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_special_00 ),
};

/* Char_Special 1: stomp_init (a leap that comes down on the opponent), stomp_move (a leap that comes down on the opponent) */
const Sint32 pl1a_special_01[] = {
	CG_FRAME(   0,    1,   4,           0,   192, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   4,           0,   193, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,  10,           0,   194, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   4,           0,   177, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   4,           0,   178, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   4,           0,   179, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(   0,    0,   1,           0,   180, 0x40,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  12 ),
	CG_FRAME(   0,    0,   7,           0,   180, 0x40,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,  12 ),
	CG_FRAME(   0,    0,  48,           0,   181, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(   0,    0,  10,           0,   181, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(   0,    0,   6,           0,   182, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
};

const Sint32 pl1a_special_01_loop[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   183, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl1a_special_01_loop ),
};

/* Char_Catch 1
   Char_Special 2: stomp_move (a leap that comes down on the opponent), throw_fall (grabs, leaps and drops on the opponent) */
const Sint32 pl1a_catch_01[] = {
	CG_FRAME(   0,    1,   1,           0,   184, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(   0,    1,   3,           0,   184, 0x40,   0x00,  ATE,  0x00,       0x14,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(   0,    1,   4,           0,   185, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(   0,    1,   4,           0,   186, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(   0,    1,   4,           0,   187, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(   0,    1,   4,           0,   188, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(   0,    1,   4, END_OF_DATA,   189, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_NEXT( pl1a_catch_01 ),
};

/* Char_Stand 2: act_walk, act_walk (2 + no)
   Char_Stand 3
   Char_Stand 6
   Char_Stand 7
   Char_Crouch 0: act_crouch, act_stand_up
   Char_Special 3: jump_init (jumps at the opponent, firing) */
const Sint32 pl1a_stand_02[] = {
	CG_FRAME(   0,    0,   2,           0,    67, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    68, 0x40,   0x00,    0,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    69, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    70, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    71, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    72, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    73, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    74, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    75, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    76, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    77, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    78, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    79, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    80, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    81, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    82, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    84, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    85, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    86, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    87, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    88, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    89, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    90, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    91, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    92, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,    93, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    1,  16,           0,    94, 0x40,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    1,   1, END_OF_DATA,    -1, 0x40,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_stand_02 ),
};

/* Char_Special 4: wave_init (a shot along the floor) */
const Sint32 pl1a_special_04[] = {
	CG_FRAME(   0,    0,   4,           0,   195, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  12,           0,   196, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   1,           0,   197, 0x40,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   3,           0,   197, 0x40,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   198, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(   0,    1,   1,           0,   199, 0x40,   0x00,  ATK,  0x00, 0x31 | CSE,    0,   0,     0,      0,  30 ),
	CG_FRAME(   0,    0,   5,           0,   199, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   200, 0x40,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   201, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   202, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,   203, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_special_04 ),
};

/* Char_Special 5: spray_init (shots sprayed out at random) */
const Sint32 pl1a_special_05[] = {
	CG_FRAME(   0,    0,   4,           0,   161, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6,           0,   162, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  16,           0,   163, 0x40,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   164, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   2,           0,   165, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   166, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   167, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   168, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   169, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   170, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   171, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   172, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   173, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   2,           0,   174, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   2,           0,   175, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   176, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_special_05 ),
};

/* Char_Special 6: blast_init (a lasting blast in front of him) */
const Sint32 pl1a_special_06[] = {
	CG_FRAME(   0,    0,   4,           0,   204, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   205, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   206, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   207, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   1,           0,   208, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1a_special_06_loop[] = {
	CG_FRAME(   0,    0,   4,           0,   208, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   209, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_special_06_loop ),
};

/* Char_Special 7: blast_wait (a lasting blast in front of him) */
const Sint32 pl1a_special_07[] = {
	CG_FRAME(   0,    0,   4,           0,   208, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   209, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   208, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   209, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   210, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   211, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   212, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   213, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   214, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,    19, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,    20, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,    21, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,    22, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,    23, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,    16, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_special_07 ),
};

/* Char_Effect 0 */
const Sint32 pl1a_effect_00[] = {
	CG_FRAME(   0,    0,   3,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   230, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   231, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   3, END_OF_DATA,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl1a_effect_01[] = {
	CG_FRAME(   0,    0,   4,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   237, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   238, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   239, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   241, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   242, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   243, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   244, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl1a_effect_02[] = {
	CG_FRAME(   0,    0,   4,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA, 32775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl1a_effect_03[] = {
	CG_FRAME(   0,    0,   4,           0,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_03 ),
};

/* Char_Effect 4: shl1a_00_storm_move */
const Sint32 pl1a_effect_04[] = {
	CG_FRAME(   0,    0,   1,           0,   261, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   262, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   263, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   264, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   265, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(   0,    0,   1,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(   0,    0,   1,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(   0,    0,   1,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(   0,    0,   1,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(   0,    0,   1,           0,   270, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(   0,    0,   1,           0,   271, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(   0,    0,   1,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(   0,    0,   1,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(   0,    0,   1,           0,   274, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(   0,    0,   1,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(   0,    0,   1,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(   0,    0,   1,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1,           0,   281, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1, END_OF_DATA,   282, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_04 ),
};

/* Char_Effect 5: shl1a_00_up_init */
const Sint32 pl1a_effect_05[] = {
	CG_FRAME(   0,    0,   4,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_FRAME(   0,    0,   4,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_FRAME(   0,    0,   4,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_FRAME(   0,    0,   4,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_FRAME(   0,    0,   4,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_FRAME(   0,    0,   4,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_FRAME(   0,    0,   4,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  15 ),
	CG_NEXT( pl1a_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl1a_effect_06[] = {
	CG_FRAME(   0,    0,   4,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl1a_effect_07[] = {
	CG_FRAME(   0,    0,   4,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl1a_effect_08[] = {
	CG_FRAME(   0,    0,   4,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   4, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_08 ),
};

/* Char_Effect 11: shl1a_00_shot_init, shl1a_00_shot_move */
const Sint32 pl1a_effect_11[] = {
	CG_FRAME(   0,    0,   2,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_NEXT( pl1a_effect_11 ),
};

/* Char_Effect 12: shl1a_00_shot_move */
const Sint32 pl1a_effect_12[] = {
	CG_FRAME(   0,    0,   2,           0,   329, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   330, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   331, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   332, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   333, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   334, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   335, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA,   336, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_12 ),
};

/* Char_Effect 13: shl1a_00_shot_move */
const Sint32 pl1a_effect_13[] = {
	CG_FRAME(   0,    0,   2,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2,           0, 32782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA, 32783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_NEXT( pl1a_effect_11 ),
};

/* Char_Effect 14: shl1a_00_spray_init */
const Sint32 pl1a_effect_14[] = {
	CG_FRAME(   0,    0,   2,           0,   345, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   346, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   347, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(   0,    0,   2,           0,   348, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(   0,    0,   2,           0,   349, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(   0,    0,   2,           0,   350, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(   0,    0,   2,           0,   351, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(   0,    0,   2,           0,   352, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(   0,    0,   2,           0,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA,   354, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl1a_effect_14 ),
};

/* Char_Effect 9: shl1a_00_wave_init */
const Sint32 pl1a_effect_09[] = {
	CG_FRAME(   0,    0,   2,           0,   289, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   292, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(   0,    0,   2,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(   0,    1,   2,           0,   295, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(   0,    0,   2,           0,   297, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,   3 ),
	CG_FRAME(   0,    0,   2,           0,   299, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   3 ),
	CG_FRAME(   0,    0,   1,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,   4 ),
	CG_FRAME(   0,    0,   1,           0,   303, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0500,   4 ),
	CG_FRAME(   0,    0,   1,           0,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0600,   5 ),
	CG_FRAME(   0,    0,   1,           0,   307, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,   0 ),
	CG_FRAME(   0,    0,   1, END_OF_DATA,   309, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_NEXT( pl1a_effect_09 ),
};

/* Char_Effect 10: shl1a_00_wave_part_move */
const Sint32 pl1a_effect_10[] = {
	CG_FRAME(   0,    0,   1,           0,   296, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(   0,    0,   1,           0,   298, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(   0,    0,   1,           0,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(   0,    0,   1,           0,   302, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(   0,    0,   1,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(   0,    0,   1,           0,   306, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(   0,    0,   1,           0,   308, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   1, END_OF_DATA,   310, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_10 ),
};

/* Char_Effect 15: shl1a_01_init */
const Sint32 pl1a_effect_15[] = {
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   296, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl1a_effect_15 ),
};

/* Char_Effect 16: shl1a_00_blast_init */
const Sint32 pl1a_effect_16[] = {
	CG_FRAME(   0,    0,   2,           0,   355, 0x40,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   356, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(   0,    0,   2,           0,   357, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(   0,    0,   2,           0,   358, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
};

const Sint32 pl1a_effect_16_loop[] = {
	CG_FRAME(   0,    0,   2,           0,   359, 0x40,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  27 ),
	CG_FRAME(   0,    0,   2,           0,   360, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(   0,    0,   2,           0,   361, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(   0,    0,   2,           0,   362, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(   0,    0,   2,           0,   363, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA,   364, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl1a_effect_16_loop ),
};

/* Char_Effect 17: shl1a_00_blast_move */
const Sint32 pl1a_effect_17[] = {
	CG_FRAME(   0,    0,   2,           0,   365, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   366, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2,           0,   367, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   2, END_OF_DATA,   368, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_effect_17 ),
};

/* Char_Win 0: sp_win (sinks and comes up before the opponent (win)) */
const Sint32 pl1a_win_00[] = {
	CG_FRAME(   0,    0,   6,           0,    16, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    17, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    18, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    19, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    20, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    21, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    22, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    23, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   116, 0x40,   0x00,    0,  0x00,       0x17,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   117, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   118, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   119, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   120, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   121, 0x40,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   122, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   123, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   124, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   125, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   126, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   127, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   128, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   129, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   130, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   131, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   132, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   133, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   134, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   135, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   136, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   137, 0x40,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1a_win_00_loop[] = {
	CG_FRAME(   0,    0,   6,           0,   138, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   139, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   140, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,   141, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_win_00_loop ),
};

/* Char_Win 1: sp_win_voice (sinks and comes up before the opponent (win, with a voice)) */
const Sint32 pl1a_win_01[] = {
	CG_FRAME(   0,    0,   6,           0,    16, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    17, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    18, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    19, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    20, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    21, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    22, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    23, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    24, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    25, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    26, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    27, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,    28, 0x40,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_win_01 ),
};

/* Char_Win 2: sp_win_voice (sinks and comes up before the opponent (win, with a voice)) */
const Sint32 pl1a_win_02[] = {
	CG_FRAME(   0,    0,   6,           0,   142, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   143, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   142, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   143, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   142, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   143, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   142, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,   143, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    30, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    31, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    32, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    33, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    34, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    19, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    20, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    21, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    22, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    23, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    24, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    25, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    26, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    27, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,    28, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_win_02 ),
};

/* Char_Win 3: throw_init (grabs, leaps and drops on the opponent) */
const Sint32 pl1a_win_03[] = {
	CG_FRAME(   0,    0,   4,           0,   195, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  12,           0,   196, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   1,           0,   197, 0x40,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   197, 0x40,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   4,           0,   198, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    1,   6,           0,   199, 0x40,   0x00,  ATK,  0x00, 0x31 | CSE,    0,   0,     0,      0,  28 ),
	CG_FRAME(   0,    0,   6,           0,   199, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(   0,    0,   6,           0,   200, 0x40,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0,  28 ),
	CG_FRAME(   0,    0,   6,           0,   201, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(   0,    0,   6,           0,   202, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,   6, END_OF_DATA,   203, 0x40,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_win_03 ),
};

/* Char_Win 4: throw_jump (grabs, leaps and drops on the opponent) */
const Sint32 pl1a_win_04[] = {
	CG_FRAME(   0,    0,   4,           0,   180, 0xa0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   0,    0,  -1, END_OF_DATA,   181, 0xa0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1a_win_04 ),
};

/* Char_Catch 0
   Char_Win 5: throw_fly (grabs, leaps and drops on the opponent) */
const Sint32 pl1a_catch_00[] = {
	CG_FRAME(   0,    0,   4,           0,   182, 0x00,   0x00,    0,  0x00,       0x18,    0,   0,     0, 0x0001,   1 ),
};

const Sint32 pl1a_catch_00_loop[] = {
	CG_FRAME(   0,    0,   4, END_OF_DATA,   183, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_NEXT( pl1a_catch_00_loop ),
};

/* Char_Catch 2
   Char_Win 6: throw_land (grabs, leaps and drops on the opponent) */
const Sint32 pl1a_catch_02[] = {
	CG_FRAME(   0,    0,   1,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   3,           0,    95, 0x80,   0x00,    0,  0x00,       0x1a,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   4,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   148, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(   0,    0,   6,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
};

const Sint32 pl1a_catch_02_loop[] = {
	CG_FRAME(   0,    0,   6, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_NEXT( pl1a_catch_02_loop ),
};

/* Char_Win 7: sink_init (sinks away for a time)
   Char_Win 8: sp_fade (fades away (lose)) */
const Sint32 pl1a_win_07[] = {
	CG_FRAME(   0,    0,   1,           0,    67, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   5,           0,    67, 0x40,   0x00,    0,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    68, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    69, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    70, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    71, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    72, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    73, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    74, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    75, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    76, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    77, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    78, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    79, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    80, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    81, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    82, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    84, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    85, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    86, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    87, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    88, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    89, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    90, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    91, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    92, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    0,   6,           0,    93, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   0,    1,  16,           0,    94, 0x40,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1a_win_07_loop[] = {
	CG_FRAME(   0,    1,  -1, END_OF_DATA,    -1, 0x40,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1a_win_07_loop ),
};

/* group 27 0: pls1a_00_t0_move */
const Sint32 pl1a_grp27_00[] = {
	EF_FRAME(   0,    0,   4,           0,   190 ),
	EF_FRAME(   0,    1,  -1, END_OF_DATA,   191 ),
	CG_NEXT( pl1a_grp27_00 ),
};

/* group 27 1: shl1a_00_spot_init */
const Sint32 pl1a_grp27_01[] = {
	EF_FRAME(   0,    0,   2,           0,   283 ),
	EF_FRAME(   0,    0,   2,           0,   284 ),
	EF_FRAME(   0,    0,   2,           0,   285 ),
	EF_FRAME(   0,    0,   2,           0,   286 ),
	EF_FRAME(   0,    0,   2,           0,   287 ),
	EF_FRAME(   0,    0,   2,           0,   288 ),
	EF_FRAME(   0,    1,   2,           0,   236 ),
	EF_FRAME(   0,    0,   2,           0,   237 ),
	EF_FRAME(   0,    0,   2,           0,   238 ),
	EF_FRAME(   0,    0,   2,           0,   239 ),
	EF_FRAME(   0,    0,   2,           0,   240 ),
	EF_FRAME(   0,    0,   2,           0,   241 ),
	EF_FRAME(   0,    0,   2,           0,   242 ),
	EF_FRAME(   0,    0,   2,           0,   243 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   244 ),
	CG_NEXT( pl1a_grp27_01 ),
};

/* group 27 3: shl1a_00_spray_init */
const Sint32 pl1a_grp27_03[] = {
	EF_FRAME(   0,    0,   2,           0,   311 ),
	EF_FRAME(   0,    1,   2,           0,   312 ),
	EF_FRAME(   0,    0,   2,           0,   313 ),
	EF_FRAME(   0,    0,   2,           0,   314 ),
	EF_FRAME(   0,    0,   2,           0,   315 ),
	EF_FRAME(   0,    0,   2,           0,   316 ),
	EF_FRAME(   0,    0,   2,           0,   317 ),
	EF_FRAME(   0,    0,   2,           0,   318 ),
	EF_FRAME(   0,    0,   2,           0,   319 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   320 ),
	CG_NEXT( pl1a_grp27_03 ),
};

/* group 27 2: pls1a_00_spark_init */
const Sint32 pl1a_grp27_02[] = {
	EF_FRAME(   0,    0,   2,           0,   225 ),
	EF_FRAME(   0,    0,   2,           0,   226 ),
	EF_FRAME(   0,    0,   2,           0,   227 ),
	EF_FRAME(   0,    0,   2,           0,   228 ),
	EF_FRAME(   0,    0,   2,           0,   229 ),
	EF_FRAME(   0,    0,   2,           0,   230 ),
	EF_FRAME(   0,    0,   2,           0,   231 ),
	EF_FRAME(   0,    0,   2,           0,   232 ),
	EF_FRAME(   0,    0,   2,           0,   233 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   234 ),
	CG_NEXT( pl1a_grp27_02 ),
};

/* group 27 4: pls1a_00_sink_init, pls1a_00_t4_init */
const Sint32 pl1a_grp27_04[] = {
	EF_FRAME(   0,    0,  -1, END_OF_DATA,   369 ),
	CG_NEXT( pl1a_grp27_04 ),
};

/* group 27 5: pls1a_00_land_init */
const Sint32 pl1a_grp27_05[] = {
	EF_FRAME(   0,    0,   2,           0,   370 ),
	EF_FRAME(   0,    0,   2,           0,   371 ),
	EF_FRAME(   0,    0,   2,           0,   372 ),
	EF_FRAME(   0,    0,   2,           0,   373 ),
	EF_FRAME(   0,    0,   2,           0,   374 ),
	EF_FRAME(   0,    0,   2,           0,   375 ),
	EF_FRAME(   0,    0,   2,           0,   376 ),
	EF_FRAME(   0,    0,   2,           0,   377 ),
	EF_FRAME(   0,    0,   2,           0,   378 ),
	EF_FRAME(   0,    0,   2,           0,   379 ),
	EF_FRAME(   0,    0,   2,           0,   380 ),
	EF_FRAME(   0,    0,   2,           0,   381 ),
	EF_FRAME(   0,    0,   2,           0,   382 ),
	EF_FRAME(   0,    0,   2,           0,   383 ),
	EF_FRAME(   0,    0,   2,           0,   384 ),
	EF_FRAME(   0,    0,   2, END_OF_DATA,   385 ),
	CG_NEXT( pl1a_grp27_05 ),
};
