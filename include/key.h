/* The stick and buttons, as a side's switch words (SW_WORK) and a player's (pl_sw_new and the
   rest) hold them. A player's words have left and right turned by the facing (pl_sw_set):
   KEY_FORWARD is toward the opponent, KEY_BACK away. */
#ifndef KEY_H
#define KEY_H

#define KEY_START	0x8000
#define KEY_UP		0x2000
#define KEY_DOWN	0x1000
#define KEY_LEFT	0x0800
#define KEY_RIGHT	0x0400
#define KEY_FORWARD	0x0800
#define KEY_BACK	0x0400
#define KEY_LEVER	0x3c00	/* any direction */

#define KEY_LP		0x0200	/* light punch */
#define KEY_HP		0x0100	/* heavy punch */
#define KEY_A1		0x0080	/* assist 1 */
#define KEY_LK		0x0040	/* light kick */
#define KEY_HK		0x0020	/* heavy kick */
#define KEY_A2		0x0010	/* assist 2 */

#define KEY_PUNCH	0x0300	/* either punch */
#define KEY_KICK	0x0060	/* either kick */
#define KEY_LIGHT	0x0240	/* either light attack */
#define KEY_HEAVY	0x0120	/* either heavy attack */
#define KEY_ASSIST	0x0090	/* either assist */
#define KEY_ATCK	0x0360	/* any of the four attack buttons */
#define KEY_BUTTON	0x03f0	/* any of the six buttons */

#endif
