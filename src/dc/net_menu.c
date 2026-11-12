/* The network menu. The USA build switches its body off; this stub and its data are what is left. */
#include "player.h"

/* a row for each of a profile's fields */
typedef struct NET_MENU_ITEM {
	Sint16	unk_00;		/* under the field's size */
	Sint16	unk_02;
	Sint32	unk_04;
	Sint32	unk_08;
	Sint32	unk_0c;
} NET_MENU_ITEM;

/* unused: the profile's fields, then where each side's are in net_rec */
static NET_MENU_ITEM net_menu_tbl[10] = {
	{ 16, 0x8e, 0x1ff, 0, 1 }, { 62, 0x82, 0xf2, 0, 1 }, { 126, 0x82, 0xf2, 0, 1 },
	{ 8, 0x82, 0xf2, 0, 1 }, { 8, 0x82, 0xf2, 0, 1 }, { 62, 0x8e, 0xff, 0, 1 },
	{ 2, 0x82, 0xf2, 0, 1 }, { 62, 0x8e, 0xff, 0, 1 }, { 7, 0x82, 0xf2, 0, 1 },
	{ 110, 0x8e, 0xff, 0, 1 }
} ;
static char *net_menu_str[2][10] = {
	{
		net_rec[0].handle, net_rec[0].name, net_rec[0].unk_080, net_rec[0].entry_str,
		net_rec[0].entry_str, net_rec[0].unk_100, net_rec[0].age, net_rec[0].unk_140,
		net_rec[0].post_code, net_rec[0].unk_18a
	}, {
		net_rec[1].handle, net_rec[1].name, net_rec[1].unk_080, net_rec[1].entry_str,
		net_rec[1].entry_str, net_rec[1].unk_100, net_rec[1].age, net_rec[1].unk_140,
		net_rec[1].post_code, net_rec[1].unk_18a
	}
} ;
static Uint8 unk_8C1F8B44[16] ;	/* unused */

void net_nop( void )
{
}
