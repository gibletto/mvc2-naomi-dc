/* The VMU / backup memory code: the LCD picture shown on a card, the backup library's callback that
   keeps what is on each card, and the reading and saving of the game's files. */
#include "player.h"
#include "naomi.h"
#include "book.h"
#include "vms.h"

/* Shinobi / Kamui */
typedef struct BU_FILE_INFO {
	Uint8	unk_00[10];
	Uint16	year;
	Uint8	month, day, hour, minute, second;
	Uint8	week, unk_12;
} BU_FILE_INFO ;
extern int pdVmsLcdIsReady( Uint32 id ) ;
extern int pdLcdGetDirection( Uint32 id ) ;
extern int pdVmsLcdWrite( Uint32 id, Uint8 *bitmap, int dir ) ;
extern void syTmrGenSetClock( int clock ) ;
extern void syTmrGenStart( void ) ;
extern int buInit( int blocks, int unk_1, int unk_2, void ( *func )( void ) ) ;
extern int buExit( void ) ;
extern int buSetCompleteCallback( int ( *func )( int, int, int, int ) ) ;
extern int buIsExistFile( int port, const char *name ) ;
extern int buGetFileInfo( int port, const char *name, BU_FILE_INFO *info ) ;
extern int buGetDiskFree( int port, int unk ) ;
extern int buFindExecFile( int port, char *name ) ;
extern int buUnmount( int port ) ;
extern int buMountDisk( int port, void *work, int size ) ;

extern void *memcpy( void *d, const void *s, unsigned long n ) ;
extern char *strcpy( char *d, const char *s ) ;
extern void play_time_ck( void ) ;

#define BU_NOT_FOUND	( -251 )	/* 0xff05 */
#define BU_BUSY		( -255 )	/* 0xff01 */
#define BU_ERR_03	( -253 )	/* 0xff03 */

static void vms_lcd_conv( void *src ) ;
static void vms_lcd_put( int port ) ;
static int vms_callback( int port, int func, int result, int unk ) ;
static void vms_callback_set( void ) ;
static int vms_sum_b( Uint8 no ) ;
int vms_sum_c( void ) ;
extern int vms_load_ex() ;

/* the picture's LCD place of each port */
static Sint32 vms_lcd_id[8] = { 1, 2, 7, 8, 13, 14, 19, 20 } ;
static Uint8 vms_lcd_buf[48 * 32] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 8, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 8, 8, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 8, 8, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 8, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 0, 0, 0, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0,
} ;
static Uint8 vms_lcd_img[192] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFE, 0x80, 0x0F, 0xFF, 0x56, 0xC5,
	0x4A, 0x3F, 0xE3, 0xFF, 0x45, 0x55, 0x5A, 0xE0, 0x39, 0xFF, 0x45, 0x4D, 0x4B, 0x80, 0x0D, 0xFF,
	0x54, 0x55, 0x5B, 0x3F, 0x05, 0xFF, 0x55, 0x56, 0xC9, 0xFF, 0x85, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF,
	0x84, 0x00, 0x00, 0x7B, 0xCF, 0xFF, 0xFF, 0xFE, 0xFF, 0x5B, 0xBF, 0x9B, 0x12, 0x2A, 0xFF, 0x5B,
	0xBF, 0xB5, 0x56, 0xA2, 0xFF, 0x5D, 0x5F, 0xB1, 0x36, 0xA2, 0xFF, 0x5D, 0x6F, 0xB5, 0x76, 0xAA,
	0xFF, 0x5E, 0xEF, 0x95, 0x72, 0x2A, 0xFF, 0x6E, 0x9B, 0xFF, 0xFF, 0xFE, 0xFF, 0x2F, 0xFF, 0xC3,
	0x00, 0x00, 0xFF, 0xB7, 0xFF, 0x0E, 0x7F, 0xFF, 0xFF, 0x99, 0xFC, 0x38, 0x0F, 0xFF, 0xFF, 0xCE,
	0x70, 0xE3, 0xEF, 0xFF, 0xFF, 0xE3, 0x83, 0x8E, 0x6F, 0xFF, 0xFF, 0x8E, 0x0E, 0x38, 0xCF, 0xFF,
	0xFF, 0x38, 0x3F, 0xE1, 0x9F, 0xFF, 0xFE, 0x60, 0x00, 0x03, 0x3F, 0xFF, 0xFE, 0xC0, 0x00, 0x06,
	0x7F, 0xFF, 0xFE, 0xFF, 0xFF, 0xFC, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
} ;

static Uint8 vms_lcd_img2[192] = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xFF, 0xFF, 0xFF, 0xFF, 0x68, 0x16, 0xF9, 0xCC, 0x8C,
	0xCF, 0x68, 0x16, 0xF6, 0xBB, 0xBB, 0xBF, 0x68, 0x16, 0xF0, 0xBB, 0x8D, 0xDF, 0x68, 0x16, 0xF6,
	0xBB, 0xBE, 0xEF, 0x68, 0x16, 0xF6, 0xCC, 0x89, 0x9F, 0x68, 0x16, 0xFF, 0xFF, 0xFF, 0xFF, 0x68,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x56, 0xC5, 0x4A, 0x3F,
	0xE0, 0x00, 0x45, 0x55, 0x5A, 0x60, 0x38, 0x00, 0x54, 0x4D, 0x4B, 0x80, 0x0C, 0x00, 0x55, 0x56,
	0xC9, 0x3F, 0x04, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0x84, 0x00, 0x00, 0x7B, 0x8F, 0xFF, 0xFF, 0xFE,
	0x00, 0x5B, 0xBF, 0x9B, 0x12, 0x2A, 0x00, 0x59, 0x1F, 0xB5, 0x56, 0xA2, 0x00, 0x5D, 0x4F, 0xB1,
	0x36, 0xAA, 0x00, 0x5C, 0x6F, 0x95, 0x72, 0x2A, 0x00, 0x6E, 0x8B, 0xFF, 0xFF, 0xFE, 0x00, 0x2F,
	0xFF, 0xC3, 0x00, 0x00, 0x00, 0x37, 0xFF, 0x0E, 0x00, 0x00, 0x00, 0x19, 0xFC, 0x38, 0x00, 0x00,
	0x00, 0x0E, 0x70, 0xE3, 0xE0, 0x00, 0x00, 0x03, 0x83, 0x8E, 0x60, 0x00, 0x00, 0x0E, 0x0E, 0x38,
	0xC0, 0x00, 0x00, 0x38, 0x3F, 0xE1, 0x80, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00, 0xC0,
	0x00, 0x06, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
} ;

/* the name, comment and icon colours of each file the card holds (Shift-JIS) */
static char vms_name_0[18] = "MVS.C2_UPLOAD   " ;
static char vms_comment_0[34] = "\xCF\xB0\xB3\xDE\xD9 VS.\xB6\xCC\xDF\xBA\xDD 2_UPLOAD\xCC\xA7\xB2\xD9     " ;
static Uint8 vms_pal_0[16] = { 0x3F, 0x02, 0x06, 0x4C, 0x31, 0x02, 0x16, 0x1A, 0x0C, 0x38, 0x14, 0x53, 0x56, 0x2F, 0x55, 0x00 } ;
static char vms_name_1[18] = "MVS.C2_\xBC\xBD\xC3\xD1\xCC\xA7\xB2\xD9 " ;
static char vms_comment_1[34] = "\xCF\xB0\xB3\xDE\xD9 VS.\xB6\xCC\xDF\xBA\xDD 2_\xBC\xBD\xC3\xD1\xCC\xA7\xB2\xD9       " ;
static Uint8 vms_pal_1[16] = { 0x3F, 0x02, 0x06, 0x4C, 0x31, 0x02, 0x16, 0x1A, 0x0C, 0x38, 0x14, 0x53, 0x56, 0x2F, 0x56, 0x00 } ;
static char vms_name_2[18] = "MVS.C2_VS.COM   " ;
static char vms_comment_2[34] = "\xCF\xB0\xB3\xDE\xD9 VS.\xB6\xCC\xDF\xBA\xDD 2_VS.COM\xCC\xA7\xB2\xD9     " ;
static Uint8 vms_pal_2[16] = { 0x3F, 0x02, 0x06, 0x4C, 0x31, 0x02, 0x16, 0x1A, 0x0C, 0x38, 0x14, 0x53, 0x56, 0x2F, 0x57, 0x00 } ;
static char vms_name_3[18] = "MVS.C2_DOWNLOAD " ;
static char vms_comment_3[34] = "\xCF\xB0\xB3\xDE\xD9 VS.\xB6\xCC\xDF\xBA\xDD 2_DOWNLOAD\xCC\xA7\xB2\xD9   " ;
static Uint8 vms_pal_3[16] = { 0x3F, 0x02, 0x06, 0x4C, 0x31, 0x02, 0x16, 0x1A, 0x0C, 0x38, 0x14, 0x53, 0x56, 0x2F, 0x58, 0x00 } ;
static char vms_name_4[18] = "CAP-NET_NET\xCC\xA7\xB2\xD9 " ;
static char vms_comment_4[34] = "\x83J\x83v\x83R\x83\x93\x81@\x83l\x83\x62\x83g\x83\x8F\x81[\x83N\x83t\x83@\x83\x43\x83\x8B  " ;
static Uint8 vms_pal_4[16] = { 0x0C, 0x38, 0x14, 0x53, 0x2E, 0x26, 0x29, 0x4F, 0x02, 0x10, 0x36, 0x02, 0x03, 0x4C, 0x55, 0x00 } ;

/* the icons: sixteen colours and the picture of each file */
static Uint8 vms_icon_pal[5][32] = {
	{ 0x98, 0xFA, 0xFF, 0xFE, 0xCA, 0xFC, 0xCB, 0xFF, 0x97, 0xFD, 0x64, 0xFA, 0x32, 0xF6, 0xDB, 0xF0, 0xA8, 0xF0, 0x77, 0xF0, 0x9F, 0xF7, 0x6C, 0xF5, 0x59, 0xF4, 0x35, 0xF0, 0x40, 0xFF, 0xF0, 0xFF },
	{ 0x87, 0xFA, 0x8D, 0xF7, 0x59, 0xF4, 0x37, 0xF2, 0x23, 0xF0, 0xC9, 0xFF, 0x96, 0xFD, 0xD0, 0xFE, 0xA2, 0xFE, 0x73, 0xFB, 0x42, 0xF7, 0xEE, 0xFE, 0xBB, 0xFB, 0x88, 0xF8, 0x55, 0xF5, 0x22, 0xFF },
	{ 0x44, 0xF3, 0x8A, 0xF4, 0xAB, 0xF7, 0xDD, 0xFA, 0x67, 0xF3, 0xD0, 0xFF, 0xEA, 0xFF, 0x90, 0xFF, 0xB8, 0xFE, 0x41, 0xFA, 0x31, 0xF7, 0x64, 0xFC, 0xA7, 0xFF, 0x52, 0xFE, 0xFF, 0xFF, 0x22, 0xF2 },
	{ 0x98, 0xFA, 0xDA, 0xFF, 0xB8, 0xFF, 0x84, 0xFC, 0x50, 0xFB, 0xF0, 0xFF, 0xFF, 0xFF, 0xCB, 0xFB, 0x87, 0xF7, 0x55, 0xFF, 0x31, 0xFF, 0x21, 0xFC, 0x31, 0xFA, 0x31, 0xF8, 0x21, 0xF6, 0x90, 0xFF },
	{ 0x0F, 0xF0, 0x0B, 0xF3, 0x1A, 0xF4, 0xEF, 0xF5, 0x29, 0xF6, 0x85, 0xFB, 0x66, 0xFC, 0xA4, 0xFC, 0x54, 0xFF, 0x64, 0xFF, 0x74, 0xFF, 0x83, 0xFF, 0x93, 0xFF, 0xB3, 0xFF, 0xD2, 0xFF, 0xE2, 0xFF },
} ;
static Uint8 *vms_icon_pal_tbl[5] = { vms_icon_pal[0], vms_icon_pal[1], vms_icon_pal[2], vms_icon_pal[3], vms_icon_pal[4] } ;
static Uint8 vms_icon_img[5][512] = {
	{
		0xCC, 0xCC, 0xCC, 0xC5, 0x22, 0x25, 0x22, 0x22, 0x26, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x5C, 0xCC, 0xCC, 0xC5, 0x22, 0x52, 0x22, 0x22, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x2C, 0xCC, 0xCC, 0xC5, 0x24, 0x52, 0x22, 0x25, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x26, 0xCC, 0xCC, 0xC5, 0x45, 0x22, 0x22, 0x56, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x26, 0xCC, 0xCC, 0x55, 0x52, 0x22, 0x22, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x6C, 0xCC, 0xC5, 0x22, 0x22, 0x22, 0xE6, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0xCC, 0xCC, 0x52, 0x22, 0xE2, 0x56, 0xCE, 0xCC, 0x5C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0xCC, 0xC5, 0x22, 0xE2, 0x2E, 0xEE, 0x5C, 0xE5, 0xCE, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0xCC, 0xC2, 0xE2, 0x2E, 0xEE, 0x5E, 0xEE, 0x5E, 0xEC, 0xEC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0xDC, 0x52, 0x2E, 0xEE, 0xEE, 0xE5, 0xEE, 0xEE, 0xEE, 0x5E, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x9D, 0x22, 0x2E, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x79, 0xDD, 0x5E, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xE5, 0x55, 0xCC, 0xCC, 0xCC, 0xCC,
		0x78, 0xD9, 0xC5, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xEE, 0xE5, 0xCC, 0xCC, 0xCC,
		0x87, 0x98, 0x8C, 0xE5, 0xEE, 0x5E, 0xEE, 0xEE, 0xEE, 0xEE, 0xEC, 0xCC, 0xCE, 0x5C, 0xCC, 0xCC,
		0x88, 0x98, 0x89, 0x5E, 0x5E, 0xE5, 0xE5, 0xEE, 0xEE, 0xE5, 0x5E, 0xEC, 0xCC, 0xC5, 0xCC, 0xCC,
		0x99, 0x99, 0xCC, 0x5E, 0xE5, 0xE4, 0x5E, 0x45, 0xEE, 0x5E, 0xEE, 0xEE, 0xEC, 0xCC, 0x5C, 0xCC,
		0x99, 0xD8, 0x83, 0x56, 0xE5, 0xE3, 0x3E, 0x34, 0xEE, 0x5E, 0x55, 0x5E, 0xEE, 0xCC, 0xCC, 0xCC,
		0x9D, 0x88, 0x83, 0x63, 0xE5, 0x54, 0x33, 0x33, 0x35, 0x44, 0x5C, 0xCC, 0xCE, 0xEC, 0xCC, 0xCC,
		0xCD, 0x88, 0x83, 0x63, 0xE3, 0x46, 0x43, 0x33, 0x33, 0x33, 0x5C, 0xCC, 0xCC, 0xCE, 0xCC, 0xCC,
		0x9D, 0x88, 0x83, 0x36, 0x53, 0x35, 0x16, 0x33, 0x33, 0x35, 0x6C, 0xCC, 0xCC, 0xCC, 0xEC, 0xCC,
		0x9D, 0x98, 0x83, 0x44, 0x63, 0x33, 0x5A, 0xB5, 0x45, 0xB6, 0xCC, 0xCC, 0xCC, 0xCC, 0xC5, 0xCC,
		0x8D, 0xC9, 0x99, 0x44, 0x64, 0x33, 0x33, 0x33, 0x36, 0x46, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x89, 0xDD, 0xDD, 0x66, 0x66, 0x33, 0x33, 0x34, 0x64, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x88, 0x99, 0xDF, 0x65, 0x44, 0x63, 0x35, 0x56, 0x46, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC,
		0x78, 0x9D, 0x56, 0x54, 0x44, 0x46, 0x43, 0x44, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0xDD, 0xCC, 0xCC,
		0x88, 0x9D, 0x65, 0x55, 0x44, 0x44, 0x66, 0x66, 0xCC, 0xCC, 0xCC, 0xCC, 0xCD, 0xBB, 0xDC, 0xCC,
		0x88, 0x9D, 0x66, 0x65, 0x55, 0x54, 0x46, 0xCC, 0xCC, 0xCD, 0xDC, 0xCC, 0xCD, 0xAA, 0xDC, 0xCC,
		0x88, 0x9D, 0x65, 0x55, 0x44, 0x44, 0x56, 0xCC, 0xCC, 0xDA, 0xAC, 0xCC, 0xDC, 0xAD, 0xCD, 0xDC,
		0x88, 0xCD, 0x66, 0x55, 0x44, 0x44, 0x45, 0x6C, 0xCC, 0xDA, 0xAD, 0xCD, 0xCB, 0xDC, 0xCD, 0xBD,
		0x89, 0xDC, 0xC6, 0x55, 0x54, 0x44, 0x44, 0x6C, 0xCC, 0xDB, 0xAC, 0xDB, 0xBC, 0xDD, 0xDB, 0xAD,
		0xDD, 0xCC, 0xCC, 0x65, 0x54, 0x44, 0x44, 0x56, 0xCC, 0xDD, 0xDB, 0xBD, 0xBB, 0xCD, 0xBA, 0xAD,
		0xCC, 0xCC, 0xCC, 0x65, 0x55, 0x44, 0x44, 0x56, 0xCC, 0xDC, 0xBB, 0xBD, 0xCB, 0xBD, 0xCC, 0xDC,
	},
	{
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0xDC, 0xCC, 0xCD, 0xCC, 0xD2, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x22, 0xDC, 0xBB, 0xBB, 0xDB, 0xBB, 0xCD, 0xCD, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0x2D, 0xCC, 0xBB, 0xBC, 0xCB, 0xBB, 0xCD, 0xCC, 0xD2, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x22, 0xDC, 0xBB, 0xBB, 0xBD, 0xBB, 0xCD, 0xDD, 0xDC, 0xD2, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x2E, 0xDD, 0xCC, 0xCC, 0xDE, 0xBC, 0xDE, 0xED, 0xDD, 0xD2, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0x2D, 0xCC, 0xCC, 0xD9, 0x6D, 0xCD, 0x69, 0xDD, 0xE2, 0xD2, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0xED, 0xDD, 0xDD, 0xDD, 0x56, 0xD5, 0x55, 0x55, 0xA2, 0xD2, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x22, 0xED, 0x9D, 0xCC, 0xCD, 0x56, 0xD5, 0x55, 0x55, 0xA2, 0xD2, 0x82, 0x22, 0x22,
		0x22, 0x22, 0x22, 0xD9, 0x5D, 0xCC, 0xD5, 0x5C, 0xBB, 0x56, 0x55, 0x92, 0x87, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x2E, 0xD9, 0x56, 0xDD, 0x55, 0x56, 0xED, 0xCB, 0x65, 0x67, 0xB7, 0x82, 0x22, 0x22,
		0x22, 0x22, 0x2E, 0xEA, 0x69, 0xD6, 0x55, 0x59, 0xCB, 0xED, 0xA9, 0xAB, 0xBB, 0xB7, 0x78, 0x92,
		0x22, 0x22, 0x2A, 0x69, 0x96, 0xD6, 0x55, 0x56, 0x99, 0xA6, 0x56, 0xAB, 0xB7, 0x82, 0x22, 0x22,
		0x22, 0x22, 0x29, 0x69, 0xA6, 0xD6, 0x65, 0x55, 0x56, 0x95, 0x56, 0xA8, 0x78, 0x22, 0x22, 0x22,
		0x22, 0x22, 0x36, 0x69, 0x6A, 0x96, 0x66, 0x55, 0x56, 0x95, 0x56, 0xA2, 0x22, 0x82, 0x22, 0x22,
		0x22, 0x23, 0x12, 0x69, 0x6A, 0x95, 0x56, 0x55, 0x66, 0x69, 0x9A, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x22, 0x31, 0x11, 0x29, 0x6A, 0x95, 0x55, 0x56, 0x9B, 0xBC, 0x9A, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x23, 0x21, 0x11, 0x12, 0x6A, 0x95, 0x55, 0x56, 0xAB, 0xBC, 0x9A, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x82, 0x11, 0x11, 0x11, 0x2A, 0x95, 0x55, 0x56, 0xA9, 0x99, 0x9A, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x78, 0x81, 0x11, 0x21, 0x12, 0xAA, 0x65, 0x55, 0x55, 0x66, 0x92, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x77, 0x78, 0x81, 0x21, 0x12, 0x23, 0xAA, 0x96, 0x55, 0x55, 0xA2, 0x22, 0x22, 0x22, 0x22, 0x22,
		0x87, 0x77, 0x78, 0x21, 0x11, 0x21, 0x23, 0x3A, 0xAA, 0xAA, 0x98, 0x8D, 0xDD, 0xDD, 0xD2, 0x22,
		0x98, 0x77, 0x77, 0x81, 0x11, 0x21, 0x12, 0x23, 0x33, 0x43, 0x38, 0x77, 0x8C, 0xCC, 0xCC, 0xDD,
		0x29, 0x87, 0x77, 0x78, 0x11, 0x12, 0x12, 0x22, 0x33, 0x43, 0x39, 0x77, 0x78, 0xDC, 0xCB, 0xBC,
		0x22, 0x98, 0x77, 0x77, 0x82, 0x22, 0x23, 0x22, 0x24, 0x32, 0x22, 0x97, 0x77, 0x8D, 0xCC, 0xBB,
		0x12, 0x29, 0x89, 0x44, 0x44, 0x81, 0x12, 0x32, 0x32, 0x11, 0x11, 0x29, 0x84, 0x44, 0x4D, 0xCC,
		0x12, 0xA8, 0x44, 0x4C, 0xF4, 0x48, 0x11, 0x21, 0x21, 0x11, 0x11, 0x28, 0x44, 0xCF, 0x44, 0xDC,
		0x11, 0x97, 0x4F, 0xF4, 0x4F, 0xF4, 0x81, 0x12, 0x12, 0x11, 0x11, 0x97, 0x4F, 0xF4, 0x4F, 0x4D,
		0x11, 0x97, 0x4F, 0xF4, 0x4F, 0xF4, 0x81, 0x12, 0x12, 0x11, 0x11, 0x97, 0x4F, 0xF4, 0x4F, 0x4D,
		0x11, 0xA7, 0x44, 0x4F, 0xF4, 0x44, 0x87, 0x77, 0x77, 0x77, 0x77, 0xA8, 0x74, 0x4F, 0xF4, 0x4A,
	},
	{
		0x77, 0x77, 0x77, 0x7A, 0xAA, 0x00, 0x00, 0x00, 0xDD, 0xD4, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0x77, 0x77, 0xAA, 0x00, 0x00, 0x00, 0x0D, 0xD4, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0x77, 0x7A, 0x00, 0x00, 0x00, 0x00, 0xDD, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0x77, 0xA0, 0x00, 0x00, 0x00, 0x4D, 0xD4, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0x77, 0xA0, 0x00, 0x0F, 0x44, 0xDD, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0x7A, 0x00, 0x00, 0xF1, 0x11, 0xD1, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44,
		0x77, 0x7A, 0x00, 0x00, 0xF4, 0x4D, 0xD1, 0x11, 0x11, 0x11, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0x7A, 0x00, 0x00, 0xF4, 0x4D, 0x11, 0x14, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0xA0, 0x00, 0x00, 0xF4, 0xDD, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44,
		0x77, 0xA0, 0x00, 0x00, 0x0D, 0xD4, 0x44, 0x44, 0x44, 0x44, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x7A, 0x00, 0x00, 0x00, 0xDD, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
		0xA0, 0x00, 0x00, 0x0D, 0xD0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0x00, 0x00, 0x0D, 0xDD, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xAA, 0x9B, 0xBB, 0xAA, 0xFF, 0xFF,
		0x00, 0x0D, 0xDD, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x0A, 0xB9, 0x5A, 0x8A, 0x55, 0x55, 0x55, 0xBD,
		0xDD, 0xDD, 0xFF, 0xFF, 0xFF, 0xF4, 0x4C, 0xA0, 0x43, 0xCB, 0x59, 0x8A, 0x55, 0xA5, 0x55, 0x55,
		0xAA, 0xFF, 0xB5, 0x55, 0xBF, 0x44, 0xBC, 0xC0, 0x4B, 0xCB, 0x55, 0x99, 0x99, 0xA9, 0x55, 0x55,
		0x55, 0x55, 0x55, 0x55, 0xB9, 0xF9, 0xC8, 0x88, 0xC6, 0x6C, 0x55, 0x55, 0x5B, 0x00, 0x00, 0x55,
		0x55, 0x55, 0x55, 0x55, 0x97, 0x79, 0x88, 0x88, 0x88, 0x88, 0x55, 0x55, 0x55, 0x04, 0x44, 0x05,
		0x75, 0xB7, 0x75, 0x5B, 0x77, 0x77, 0xBC, 0x88, 0x88, 0x88, 0xB5, 0x55, 0x55, 0x01, 0x11, 0x40,
		0x77, 0x55, 0x77, 0x77, 0x77, 0x77, 0xAB, 0x9C, 0x88, 0x8B, 0x95, 0x55, 0x55, 0x55, 0x55, 0x14,
		0x75, 0x57, 0x77, 0x77, 0xEE, 0xE3, 0x2A, 0x88, 0x8C, 0x99, 0xB2, 0x2E, 0x55, 0x5B, 0x04, 0x24,
		0x77, 0x77, 0x77, 0x73, 0x00, 0xEE, 0x32, 0xBB, 0x99, 0xBE, 0xEE, 0xEE, 0xEE, 0xEE, 0x20, 0x20,
		0x77, 0x77, 0x77, 0x73, 0x00, 0x2E, 0x33, 0x2A, 0x9C, 0x3E, 0xEE, 0xEE, 0xEE, 0xEE, 0x20, 0x40,
		0x77, 0x00, 0x00, 0x30, 0x00, 0x0E, 0xE3, 0x29, 0xBC, 0xE2, 0x22, 0x3E, 0xEE, 0xEE, 0x10, 0x04,
		0x70, 0x00, 0x23, 0x00, 0x00, 0x01, 0xE3, 0xB8, 0xB8, 0xE2, 0x22, 0x21, 0xEE, 0xEE, 0x40, 0x41,
		0x70, 0x43, 0x34, 0x44, 0x02, 0xF1, 0xEB, 0xC8, 0xC8, 0x2E, 0xEE, 0x10, 0x1E, 0xEE, 0x04, 0x11,
		0x70, 0x00, 0x44, 0x40, 0x2E, 0x24, 0xEB, 0x88, 0xCC, 0xEE, 0xEE, 0x01, 0x14, 0xEE, 0x21, 0x11,
		0x70, 0x00, 0x44, 0x42, 0xEE, 0x2E, 0x1C, 0x8C, 0xE3, 0x2E, 0xE4, 0x14, 0x00, 0x3E, 0x31, 0x11,
		0x7F, 0x00, 0x44, 0x43, 0xEE, 0x2E, 0x1C, 0xCE, 0xEE, 0x2E, 0x34, 0x40, 0x04, 0x1E, 0xE1, 0x14,
		0x74, 0x00, 0x44, 0x4E, 0x11, 0xE3, 0x4B, 0xEE, 0xE3, 0xEE, 0x44, 0x04, 0x44, 0x44, 0xE4, 0x40,
		0x44, 0xF0, 0x44, 0x0E, 0x1E, 0xE2, 0x9E, 0xE2, 0x22, 0xE2, 0x00, 0x44, 0x44, 0x44, 0x43, 0xFF,
		0x44, 0x4F, 0x44, 0x02, 0xE1, 0xE4, 0xEE, 0xE2, 0xEE, 0x24, 0x44, 0x44, 0x44, 0x00, 0xFF, 0x11,
	},
	{
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x44, 0x44, 0x44, 0x44, 0x44, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0x22, 0x32, 0x55, 0x23, 0x44, 0x44, 0x44, 0x4F, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xF2, 0x52, 0x55, 0x22, 0x52, 0x33, 0x44, 0x44, 0x34, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x35, 0x53, 0x55, 0x22, 0x23, 0x35, 0x53, 0x33, 0x33, 0x4F, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x32, 0x34, 0x32, 0x55, 0x55, 0x22, 0x55, 0x33, 0x33, 0x4F, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xF3, 0x23, 0x44, 0x44, 0x33, 0x25, 0x55, 0x22, 0x53, 0x34, 0x44, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xF3, 0x34, 0x44, 0x42, 0x13, 0x43, 0x25, 0x53, 0x22, 0x44, 0x44, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xF3, 0x33, 0x44, 0x21, 0x11, 0x34, 0x43, 0x55, 0x32, 0x34, 0x44, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xF3, 0x32, 0x42, 0x11, 0x11, 0x24, 0x44, 0x35, 0x23, 0x34, 0x44, 0x4F, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x31, 0x21, 0x11, 0x11, 0x12, 0x44, 0x43, 0x24, 0x13, 0x44, 0x4F, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x31, 0x11, 0x11, 0x12, 0x11, 0x23, 0x44, 0x31, 0x31, 0x44, 0x44, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x31, 0x11, 0x11, 0x31, 0x11, 0x12, 0x34, 0x42, 0x41, 0x4D, 0xDD, 0xDF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x42, 0x11, 0x1D, 0x21, 0x11, 0x11, 0x24, 0x44, 0x13, 0x4D, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xF4, 0x21, 0x3D, 0xDD, 0x42, 0x11, 0x24, 0x42, 0x34, 0x4D, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFD, 0x31, 0x23, 0xDD, 0x61, 0x11, 0x43, 0x4D, 0xD4, 0x4D, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xF4, 0x32, 0x11, 0x43, 0x21, 0x14, 0x33, 0x4D, 0xCD, 0xDA, 0xAB, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0x44, 0x21, 0x11, 0x11, 0x21, 0x44, 0x44, 0x1D, 0xBA, 0xDA, 0xBF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFD, 0x43, 0x21, 0x12, 0x31, 0x12, 0x41, 0x13, 0xBA, 0xAB, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0x43, 0xDD, 0x43, 0x11, 0x23, 0x11, 0x35, 0xBA, 0xBC, 0xBB, 0xCF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0xA4, 0x32, 0x11, 0x23, 0x32, 0x23, 0x25, 0xBA, 0xCA, 0xAA, 0xAC, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFB, 0xAA, 0x44, 0x33, 0x44, 0x33, 0x25, 0x23, 0x7A, 0xCB, 0xAA, 0xAB, 0xCF,
		0xFF, 0xFF, 0xFF, 0xBA, 0xAA, 0xAA, 0xAB, 0x32, 0x52, 0x55, 0x37, 0x7B, 0xCB, 0xBB, 0xAC, 0xAB,
		0xFF, 0xFF, 0xFF, 0xFF, 0xCC, 0xCB, 0xAA, 0xB3, 0x32, 0x33, 0x55, 0x3B, 0xCA, 0xBB, 0xBC, 0xAA,
		0xFF, 0xFF, 0xFF, 0xFB, 0xBB, 0xAA, 0xBB, 0xA7, 0x25, 0x52, 0x25, 0x53, 0xCA, 0xAB, 0xCB, 0xAA,
		0xFF, 0xFF, 0xFF, 0xBA, 0xAA, 0xBA, 0xAA, 0xB2, 0x55, 0x55, 0x35, 0x55, 0x3A, 0xAA, 0xDB, 0xAA,
	},
	{
		0xED, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDC,
		0xC8, 0x88, 0x88, 0x89, 0x99, 0x9A, 0xAA, 0xAA, 0xAA, 0xAA, 0xA9, 0x99, 0x99, 0x89, 0x88, 0x8B,
		0xC8, 0x88, 0x89, 0x99, 0xA9, 0xAA, 0x96, 0x44, 0x21, 0x22, 0x46, 0x99, 0x99, 0x98, 0x88, 0x8B,
		0xC8, 0x88, 0x99, 0x99, 0xAA, 0x94, 0x11, 0x11, 0x11, 0x11, 0x11, 0x16, 0x99, 0x99, 0x98, 0x8B,
		0xC8, 0x89, 0x99, 0xAA, 0xA6, 0x11, 0x12, 0x65, 0x77, 0x75, 0x62, 0x11, 0x28, 0x99, 0x99, 0x8B,
		0xC8, 0x99, 0x9A, 0xAA, 0x41, 0x14, 0x7F, 0xFF, 0xFF, 0xFF, 0xFE, 0x52, 0x11, 0x8A, 0x99, 0x8B,
		0xC9, 0x99, 0xAA, 0xA4, 0x11, 0x6F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0x41, 0x16, 0x99, 0x9B,
		0xC9, 0x99, 0xAA, 0x61, 0x17, 0xFF, 0xFF, 0xFF, 0x75, 0x7E, 0xFF, 0xFF, 0xE4, 0x11, 0x89, 0x9B,
		0xC9, 0x9A, 0xA8, 0x11, 0x5F, 0xFF, 0xFF, 0x74, 0x11, 0x12, 0x7F, 0xFF, 0xFD, 0x11, 0x4A, 0x9B,
		0xC9, 0x9A, 0xA4, 0x14, 0xFF, 0xFF, 0xF7, 0x11, 0x24, 0x11, 0x2E, 0xFF, 0xF6, 0x12, 0x9A, 0x9B,
		0xC9, 0xAA, 0x91, 0x17, 0xFF, 0xFF, 0xD2, 0x16, 0xDD, 0x51, 0x17, 0xFF, 0x41, 0x28, 0xBA, 0x9B,
		0xC9, 0xAA, 0x61, 0x4F, 0xFF, 0xFF, 0x61, 0x4D, 0xDD, 0x51, 0x1E, 0xF6, 0x11, 0x5B, 0xBB, 0x9B,
		0xCA, 0xAB, 0x41, 0x5F, 0xFF, 0xFF, 0x21, 0x5D, 0xDD, 0x61, 0x15, 0x61, 0x15, 0xCB, 0xBB, 0xAB,
		0xC9, 0xAB, 0x21, 0x7F, 0xFF, 0xFE, 0x11, 0x7E, 0xEE, 0xD4, 0x11, 0x11, 0x5C, 0xCC, 0xBB, 0xAB,
		0xC9, 0xAB, 0x11, 0xDF, 0xFF, 0xFD, 0x11, 0xDE, 0xEE, 0xED, 0x61, 0x16, 0xCC, 0xCC, 0xBB, 0xAB,
		0xCA, 0xAB, 0x11, 0x7F, 0xFF, 0xFE, 0x11, 0x7E, 0xFE, 0xEE, 0xD5, 0x6D, 0xDC, 0xCC, 0xBB, 0xAB,
		0xC9, 0xBB, 0x21, 0x7F, 0xFF, 0xFF, 0x21, 0x5E, 0xEE, 0xEE, 0xDD, 0xDD, 0xCC, 0xCC, 0xBB, 0xAB,
		0xC9, 0xAB, 0x41, 0x5F, 0xFF, 0xFF, 0x61, 0x4E, 0xEE, 0xEE, 0xDD, 0xDD, 0xCC, 0xCC, 0xBB, 0xAB,
		0xC9, 0xBB, 0x61, 0x4F, 0xFF, 0xFF, 0xD1, 0x15, 0xEE, 0xED, 0xDD, 0x64, 0x44, 0x46, 0xBB, 0xAB,
		0xC9, 0xAA, 0x91, 0x17, 0xFF, 0xFF, 0xF5, 0x11, 0x6D, 0xED, 0xD5, 0x11, 0x11, 0x14, 0xBB, 0xAB,
		0xC9, 0xAB, 0xB4, 0x14, 0xEF, 0xFF, 0xFF, 0x51, 0x12, 0x66, 0x41, 0x12, 0x42, 0x14, 0xBB, 0xAB,
		0xC9, 0xAA, 0xB9, 0x11, 0x6F, 0xFF, 0xFF, 0xF7, 0x21, 0x11, 0x11, 0x4D, 0xF6, 0x14, 0xBA, 0xAB,
		0xC9, 0xAA, 0xBB, 0x61, 0x15, 0xFF, 0xFF, 0xFF, 0xE5, 0x64, 0x65, 0xFF, 0xF6, 0x14, 0xBA, 0xAB,
		0xC3, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3B,
		0xC3, 0x00, 0x00, 0x33, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x3B,
		0xC3, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x03, 0x33, 0x33, 0x33, 0x33, 0x00, 0x03, 0x33, 0x3B,
		0xC3, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x03, 0x44, 0x43, 0x00, 0x03, 0x99, 0x8B,
		0xC3, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x03, 0x33, 0x33, 0x22, 0x23, 0x00, 0x03, 0x99, 0x9B,
		0xC3, 0x00, 0x03, 0x30, 0x00, 0x03, 0x00, 0x03, 0x33, 0x33, 0x3B, 0xB3, 0x00, 0x03, 0x98, 0x8B,
		0xC3, 0x00, 0x03, 0x33, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x3A, 0xA3, 0x00, 0x03, 0x98, 0x8B,
		0xC3, 0x33, 0x33, 0x93, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3A, 0xA3, 0x33, 0x33, 0x88, 0x8B,
		0x86, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x65,
	},
} ;
static Uint8 *vms_icon_img_tbl[5] = { vms_icon_img[0], vms_icon_img[1], vms_icon_img[2], vms_icon_img[3], vms_icon_img[4] } ;

/*==============================================================*/
static void vms_lcd_conv( void *src )
{
	Uint8 *p = src ;
	Uint8 *dst = vms_lcd_buf ;
	Uint8 b ;
	int i, j ;

	for( i = 192 ; i > 0 ; i -- ){
		b = *p ++ ;
		for( j = 7 ; j > -1 ; j -- ){
			if( ( b >> j ) & 1 ) *dst = 8 ;
			else *dst = 0 ;
			dst ++ ;
		}
	}
}

/*==============================================================*/
static void vms_lcd_put( int port )
{
	if( pdVmsLcdIsReady( vms_lcd_id[port] ) ){
		if( pdLcdGetDirection( vms_lcd_id[port] ) == 1 ) pdVmsLcdWrite( vms_lcd_id[port], vms_lcd_buf, 3 ) ;
		else pdVmsLcdWrite( vms_lcd_id[port], vms_lcd_buf, 0 ) ;
	}
}

/* the places for the data the library fills in */
/* what each file holds in the game (they lie one after the other, the first one's 0x6000
   bytes for the upload file running on over the rest) */
static Uint8 vms_sys_buf[0x728] ;
static Uint8 vms_dow_buf[0x4c] ;
static Uint8 vms_net_buf[0x980] ;
/* the image of a file as the library keeps it: the size of its data is at 0x48 */
typedef struct BU_IMAGE {
	Uint8	unk_00[0x48];
	Uint32	size;
	Uint8	unk_4c[0x6400 - 0x4c];
} BU_IMAGE ;
static BU_IMAGE vms_buf_a ;
/* what a card holds of the game's exec file's data: the unlocks, the level and what has been played */
typedef struct VMS_CARD_DATA {
	Uint8	unlock[56];	/* 000: bit 7 for a character or stage opened, bit 6 for another kind */
	Uint8	unk_38;		/* 038 */
	Uint8	level;		/* 039 */
	Uint8	level_pt;	/* 03a: the points to the next level, up to 99 */
	Uint8	unk_3b[0x46 - 0x3b];
	Uint8	unk_46[2];	/* 046: a count, the hundreds in the second */
	Uint8	unk_48[2];
	Uint8	unk_4a[2];	/* 04a: another */
	Uint8	unk_4c[4];
	Uint8	sum;		/* 050: the sum of the bytes before it */
	Uint8	unk_51[0x200 - 0x51];
} VMS_CARD_DATA ;
typedef union VMS_CARD {
	VMS_CARD_DATA	d ;
	Uint8		raw[0x200] ;
} VMS_CARD ;
static VMS_CARD vms_buf_b[3] ;
/* the work a card is mounted with: 122 blocks and 32 bytes */
static Uint8 vms_disk_work[( 122 << 9 ) + 32] ;
static VMS_SLOT vms_slot[VMS_PORT_MAX] ;
static char vms_exec_name[0x20] ;

/*==============================================================*/
/*			The backup library's completion callback: notes		*/
/*			what each call for a card found						*/
/*==============================================================*/
static int vms_callback( int port, int func, int result, int unk )
{
	BU_FILE_INFO info ;
	int ret ;

	switch( func ){
	case 18:
		vms_slot[port].port = port ;
		vms_slot[port].func = func ;
		vms_slot[port].result = 0xffff0000 ;
		vms_slot[port].disk_blocks = result ;
		vms_slot[port].unk_10 = unk ;
		vms_slot[port].sys_file = 0 ;
		vms_slot[port].up_file = 0 ;
		vms_slot[port].net_file = 0 ;
		vms_slot[port].dow_file = 0 ;
		vms_slot[port].vm_file = 0 ;
		vms_slot[port].other_exec = 0 ;
		vms_slot[port].free_blocks = 0 ;
		vms_slot[port].exec_free = 0 ;
		return 0 ;
	case 1:
		vms_slot[port].func = func ;
		vms_slot[port].result = result ;
		if( result == 0 ){
			switch( buIsExistFile( port, "MVLVS.C2_SYS" ) ){
			case 0:
				vms_slot[port].sys_file = 2 ;
				ret = buGetFileInfo( port, "MVLVS.C2_SYS", &info ) ;
				if( ret == 0 ){
					vms_slot[port].year = info.year ;
					vms_slot[port].month = info.month ;
					vms_slot[port].day = info.day ;
					vms_slot[port].hour = info.hour ;
					vms_slot[port].minute = info.minute ;
					vms_slot[port].second = info.second ;
					vms_slot[port].week = info.week ;
					break ;
				}
				vms_slot[port].year = 0 ;
				vms_slot[port].month = 0 ;
				vms_slot[port].day = 0 ;
				vms_slot[port].hour = 0 ;
				vms_slot[port].minute = 0 ;
				vms_slot[port].second = 0 ;
				vms_slot[port].week = 0 ;
				break ;
			case BU_NOT_FOUND:
				vms_slot[port].sys_file = 1 ;
				vms_slot[port].year = 0 ;
				vms_slot[port].month = 0 ;
				vms_slot[port].day = 0 ;
				vms_slot[port].hour = 0 ;
				vms_slot[port].minute = 0 ;
				vms_slot[port].second = 0 ;
				vms_slot[port].week = 0 ;
			}
			switch( buIsExistFile( port, "MVLVS.C2__UP" ) ){
			case 0: vms_slot[port].up_file = 2 ; break ;
			case BU_NOT_FOUND: vms_slot[port].up_file = 1 ; break ;
			default: break ;
			}
			switch( buIsExistFile( port, "CAP_NET_.NET" ) ){
			case 0: vms_slot[port].net_file = 2 ; break ;
			case BU_NOT_FOUND: vms_slot[port].net_file = 1 ; break ;
			default: break ;
			}
			switch( buIsExistFile( port, "MVLVS.C2_DOW" ) ){
			case 0: vms_slot[port].dow_file = 2 ; break ;
			case BU_NOT_FOUND: vms_slot[port].dow_file = 1 ; break ;
			default: break ;
			}
			switch( buFindExecFile( port, vms_exec_name ) ){
			case 0:
				switch( buIsExistFile( port, "MVLVS.C2__VM" ) ){
				case 0:
					vms_slot[port].vm_file = 2 ;
					vms_slot[port].other_exec = 1 ;
					break ;
				case BU_NOT_FOUND:
					vms_slot[port].vm_file = 1 ;
					vms_slot[port].other_exec = 2 ;
					break ;
				}
				break ;
			case BU_NOT_FOUND:
				vms_slot[port].vm_file = 1 ;
				vms_slot[port].other_exec = 1 ;
				break ;
			}
			vms_slot[port].free_blocks = buGetDiskFree( port, 0 ) ;
			vms_slot[port].exec_free = buGetDiskFree( port, 1 ) ;
			vms_work.mount_port = port ;
			vms_lcd_conv( vms_lcd_img ) ;
			vms_lcd_put( vms_work.mount_port ) ;
		} else if( result == BU_BUSY ) vms_slot[port].func = 2 ;
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	case 2:
		vms_slot[port].func = func ;
		vms_slot[port].result = BU_BUSY ;
		vms_slot[port].sys_file = 0 ;
		vms_slot[port].up_file = 0 ;
		vms_slot[port].net_file = 0 ;
		vms_slot[port].dow_file = 0 ;
		vms_slot[port].vm_file = 0 ;
		vms_slot[port].other_exec = 0 ;
		vms_slot[port].free_blocks = 0 ;
		vms_slot[port].exec_free = 0 ;
		vms_slot[port].year = 0 ;
		vms_slot[port].month = 0 ;
		vms_slot[port].day = 0 ;
		vms_slot[port].hour = 0 ;
		vms_slot[port].minute = 0 ;
		vms_slot[port].second = 0 ;
		vms_slot[port].week = 0 ;
		if( vms_work.mount_port == port ){
			vms_work.mount_port = -1 ;
			vms_work.task_flag = 0 ;
		}
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	case 11:
		vms_slot[port].result = result ;
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	case 12:
		vms_slot[port].result = result ;
		if( result == 0 ){
			switch( buIsExistFile( port, "MVLVS.C2_SYS" ) ){
			case 0: vms_slot[port].sys_file = 2 ; break ;
			case BU_NOT_FOUND: break ;
			default: break ;
			}
			switch( buIsExistFile( port, "MVLVS.C2__UP" ) ){
			case 0: vms_slot[port].up_file = 2 ; break ;
			case BU_NOT_FOUND: break ;
			default: break ;
			}
			switch( buIsExistFile( port, "CAP_NET_.NET" ) ){
			case 0: vms_slot[port].net_file = 2 ; break ;
			case BU_NOT_FOUND: break ;
			default: break ;
			}
			switch( buIsExistFile( port, "MVLVS.C2_DOW" ) ){
			case 0: vms_slot[port].dow_file = 2 ; break ;
			case BU_NOT_FOUND: break ;
			default: break ;
			}
			vms_slot[port].free_blocks = buGetDiskFree( port, 0 ) ;
			vms_slot[port].exec_free = buGetDiskFree( port, 1 ) ;
		}
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	case 9:
		vms_slot[port].result = result ;
		if( result == 0 ){
			vms_slot[port].free_blocks = buGetDiskFree( port, 0 ) ;
			vms_slot[port].exec_free = buGetDiskFree( port, 1 ) ;
		}
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	case 13:
		vms_slot[port].result = result ;
		if( result == 0 ){
			switch( buFindExecFile( port, vms_exec_name ) ){
			case 0:
				switch( buIsExistFile( port, "MVLVS.C2__VM" ) ){
				case 0:
					vms_slot[port].vm_file = 2 ;
					vms_slot[port].other_exec = 1 ;
					break ;
				case BU_NOT_FOUND:
					vms_slot[port].vm_file = 1 ;
					vms_slot[port].other_exec = 2 ;
					break ;
				}
				break ;
			case BU_NOT_FOUND:
				vms_slot[port].vm_file = 1 ;
				vms_slot[port].other_exec = 1 ;
				break ;
			}
			vms_slot[port].free_blocks = buGetDiskFree( port, 0 ) ;
			vms_slot[port].exec_free = buGetDiskFree( port, 1 ) ;
		}
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	case 14:
		vms_slot[port].result = result ;
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	case 20:
		vms_slot[port].result = result ;
		vms_work.bu_busy_bit &= ( 1 << port ) ^ 0xff ;
		return 0 ;
	default:
		return 0 ;
	}
}

/*==============================================================*/
static void vms_callback_set( void )
{
	buSetCompleteCallback( vms_callback ) ;
}

/****************************************************************/
void vms_init( void )
{
	int i ;

	syTmrGenSetClock( 4 ) ;
	syTmrGenStart() ;
	memset( &vms_buf_a, 0, 0xa00 ) ;
	memset( &vms_buf_b[0], 0, 0x200 ) ;
	memset( ( Uint8 * )&vms_buf_b[0] + 0x200, 0, 0x200 ) ;
	memset( &vms_work, 0, 0x90 ) ;
	memset( vms_slot, 0, sizeof( vms_slot ) ) ;
	vms_lcd_conv( vms_lcd_img ) ;
	vms_work.mount_port = -1 ;
	vms_work.task_flag = 0 ;
	vms_work.bu_busy_bit = 0 ;
	Sys->char_open0 |= book.char_open[0] ;
	Sys->char_open1 |= book.char_open[1] ;
	Sys->char_open_all0 = book.char_open_all[0] ;
	Sys->char_open_all1 = book.char_open_all[1] ;
	vms_work.play_time = book.play_time ;
	vms_work.point = book.point ;
	point_lv_set() ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		vms_slot[i].func = 2 ;
		vms_slot[i].result = 0xffff0000 ;
		vms_slot[i].disk_blocks = 122 ;
		vms_slot[i].sys_file = 0 ;
		vms_slot[i].up_file = 0 ;
		vms_slot[i].net_file = 0 ;
		vms_slot[i].dow_file = 0 ;
		vms_slot[i].vm_file = 0 ;
		vms_slot[i].other_exec = 0 ;
		vms_slot[i].free_blocks = 0 ;
		vms_slot[i].exec_free = 0 ;
		vms_slot[i].year = 0 ;
		vms_slot[i].month = 0 ;
		vms_slot[i].day = 0 ;
		vms_slot[i].hour = 0 ;
		vms_slot[i].minute = 0 ;
		vms_slot[i].second = 0 ;
		vms_slot[i].week = 0 ;
	}
	buInit( 122, 0xff, 0, vms_callback_set ) ;
}

/****************************************************************/
void vms_exit( void )
{
	buExit() ;
}

/****************************************************************/
/*			Each frame: plays the clock, then lets a card be	*/
/*			read or put away									*/
/****************************************************************/
void vms_main( void )
{
	int i, no ;
	VMS_WORK *vms_w ;
	void *work ;
	VMS_SLOT *slot, *s ;
	Sint32 ret ;

	play_time_ck() ;
	if( !Sys->vms_mode ) return ;
	vms_w = &vms_work ;
	if( vms_w->task_flag || vms_w->bu_busy_bit || rom_dma_flag ) return ;
	work = vms_disk_work ;
	if( 0 ) slot->func = 18 ;
	slot = vms_slot ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( slot[i].func == 18 && slot[i].result == 0xffff0000 ){
			if( ( no = vms_w->mount_port ) != -1 ){
				ret = buUnmount( slot[no].port ) ;
				if( ret == 0 ){
					( vms_w->mount_port + slot )->func = 18 ;
					vms_w->mount_port = -1 ;
					break ;
				}
			} else {
				s = &slot[i] ;
				ret = buMountDisk( s->port, work, ( s->disk_blocks << 9 ) + 32 ) ;
				if( ret == 0 ){
					vms_w->bu_busy_bit |= 1 << i ;
					return ;
				}
			}
		}
	}
	if( 0 ){ slot->func = 0 ; slot->result = -1 ; vms_w->mount_port = -1 ; }
}

/****************************************************************/
/*			What a port's card shows: -1 while it has not		*/
/*			answered, else its bit if it has					*/
/****************************************************************/
int vms_card_ck( int port )
{
	int mask ;

	if( vms_slot[port].func == 18 && vms_slot[port].result == 0xffff0000 ) return -1 ;
	mask = 0 ;
	if( vms_slot[port].func == 18 || vms_slot[port].func == 1 ){
		if( vms_slot[port].result != 0xffff0000 ) mask |= 1 << port ;
	}
	return mask ;
}

/****************************************************************/
int vms_card_sys_ck( int port )
{
	int mask ;

	if( vms_slot[port].func == 18 && vms_slot[port].result == 0xffff0000 ) return -1 ;
	mask = 0 ;
	if( vms_slot[port].func == 18 || vms_slot[port].func == 1 ){
		if( vms_slot[port].sys_file == 2 ) mask |= 1 << port ;
	}
	return mask ;
}

/****************************************************************/
/*			The ports whose card has answered; -1 when the		*/
/*			setting is off or one is still being read			*/
/****************************************************************/
int vms_ports_card_ck( void )
{
	VMS_SLOT *s ;
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( s = vms_slot ; s < &vms_slot[VMS_PORT_MAX] ; s ++ ){
		if( s->func == 18 && s->result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].result != 0xffff0000 ) mask |= 1 << i ;
		}
	}
	return mask ;
}

/****************************************************************/
/*			The same, without waiting for the cards still		*/
/*			being read											*/
/****************************************************************/
int vms_ports_card_now_ck( void )
{
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].result != 0xffff0000 ) mask |= 1 << i ;
		}
	}
	return mask ;
}

/****************************************************************/
/*			The ports whose card has the game's system file		*/
/****************************************************************/
int vms_ports_sys_ck( void )
{
	VMS_SLOT *s ;
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( s = vms_slot ; s < &vms_slot[VMS_PORT_MAX] ; s ++ ){
		if( s->func == 18 && s->result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].sys_file == 2 ) mask |= 1 << i ;
		}
	}
	return mask ;
}

/****************************************************************/
int vms_ports_vm_ck( void )
{
	VMS_SLOT *s ;
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( s = vms_slot ; s < &vms_slot[VMS_PORT_MAX] ; s ++ ){
		if( s->func == 18 && s->result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].vm_file == 2 ) mask |= 1 << i ;
		}
	}
	return mask ;
}

/****************************************************************/
int vms_ports_other_ck( void )
{
	VMS_SLOT *s ;
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( s = vms_slot ; s < &vms_slot[VMS_PORT_MAX] ; s ++ ){
		if( s->func == 18 && s->result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].other_exec == 2 ) mask |= 1 << i ;
		}
	}
	return mask ;
}

/****************************************************************/
/*			The ports whose card has room for 5 blocks			*/
/****************************************************************/
int vms_ports_sys_free_ck( void )
{
	VMS_SLOT *s ;
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( s = vms_slot ; s < &vms_slot[VMS_PORT_MAX] ; s ++ ){
		if( s->func == 18 && s->result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].free_blocks >= 5 ) mask |= 1 << i ;
		}
	}
	return mask ;
}

/****************************************************************/
/*			The ports whose card has room for 50 blocks			*/
/****************************************************************/
int vms_ports_ck6( void )
{
	VMS_SLOT *s ;
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( s = vms_slot ; s < &vms_slot[VMS_PORT_MAX] ; s ++ ){
		if( s->func == 18 && s->result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].free_blocks >= 50 ) mask |= 1 << i ;
		}
	}
	return mask ;
}

/****************************************************************/
/*			The ports whose card has room to save: the			*/
/*			blocks the files not yet there need					*/
/****************************************************************/
int vms_ports_save_ck( void )
{
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 && vms_slot[i].result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].sys_file == 2 ){
				if( vms_slot[i].net_file == 2 ) mask |= 1 << i ;
				else if( vms_slot[i].free_blocks >= 6 ) mask |= 1 << i ;
			} else {
				if( vms_slot[i].net_file == 2 ){
					if( vms_slot[i].free_blocks >= 5 ) mask |= 1 << i ;
				} else if( vms_slot[i].free_blocks >= 11 ) mask |= 1 << i ;
			}
		}
	}
	return mask ;
}

/****************************************************************/
int vms_ports_vm_save_ck( void )
{
	VMS_SLOT *s ;
	int i, mask ;

	if( !Sys->vms_mode ) return -1 ;
	for( s = vms_slot ; s < &vms_slot[VMS_PORT_MAX] ; s ++ ){
		if( s->func == 18 && s->result == 0xffff0000 ) return -1 ;
	}
	mask = 0 ;
	for( i = 0 ; i < VMS_PORT_MAX ; i ++ ){
		if( vms_slot[i].func == 18 || vms_slot[i].func == 1 ){
			if( vms_slot[i].other_exec == 1 ){
				if( vms_slot[i].sys_file == 2 ){
					if( vms_slot[i].vm_file == 2 ) mask |= 1 << i ;
					else if( vms_slot[i].free_blocks >= 64 ) mask |= 1 << i ;
				} else {
					if( vms_slot[i].vm_file == 2 ){
						if( vms_slot[i].free_blocks >= 5 ) mask |= 1 << i ;
					} else if( vms_slot[i].free_blocks >= 69 ) mask |= 1 << i ;
				}
			}
		}
	}
	return mask ;
}

/****************************************************************/
/*			Two routines that do nothing, the third with a		*/
/*			stack frame of its own								*/
/****************************************************************/
void vms_nop0( Sint8 type )
{
}

/****************************************************************/
void vms_nop1( Sint8 type )
{
}

/****************************************************************/
void vms_nop2( Sint8 type )
{
	char unused[56] ;
}

/* the header of a file for the card: the library's description of what it saves */
typedef struct BU_FILE_HEAD {
	char	vms_name[18];		/* 000: the name the card's menu shows */
	char	comment[34];		/* 012 */
	Uint8	pal[16];		/* 034: the icon's colours */
	void	*icon;			/* 044 */
	void	*unk_48;		/* 048 */
	Uint16	icon_cnt;		/* 04c */
	Uint16	icon_spd;		/* 04e */
	Sint32	unk_50;			/* 050 */
	Uint16	unk_54;			/* 054 */
	void	*data;			/* 058: what is saved */
	Uint32	size;			/* 05c: and its size */
} BU_FILE_HEAD ;

extern int buMakeBackupFileImage( void *image, BU_FILE_HEAD *head ) ;
extern int buSaveFile( int port, const char *name, void *image, int size, void *date, Uint32 flag ) ;
extern int buAnalyzeBackupFileImage( BU_FILE_HEAD *head, void *image ) ;
extern int buGetFileSize( int port, const char *name ) ;
extern int buLoadFile( int port, const char *name, void *buf, int size ) ;
extern int buLoadFileEx( int port, const char *name, void *buf, int unk_1, int unk_2 ) ;
extern int buRewriteExecFile( int port, const char *name, void *buf, int unk_1, int unk_2 ) ;
extern int buSaveExecFile( int port, const char *name, void *image, int size, void *date, Uint32 flag ) ;
extern int buDefragDisk( int port, void *work ) ;

/****************************************************************/
/*			The image of one of the game's files, in the		*/
/*			buffer the library saves from; returns its size		*/
/****************************************************************/
int vms_file_make( Sint8 type )
{
	BU_FILE_HEAD head ;

	memset( &head, 0, sizeof( head ) ) ;
	switch( type ){
	case 0:
		strcpy( head.vms_name, vms_name_0 ) ;
		strcpy( head.comment, vms_comment_0 ) ;
		memcpy( head.pal, vms_pal_0, 16 ) ;
		head.data = vms_sys_buf ;
		head.size = 0x6000 ;
		break ;
	case 1:
		strcpy( head.vms_name, ( char * )&vms_name_1 ) ;
		strcpy( head.comment, ( char * )&vms_comment_1 ) ;
		memcpy( head.pal, vms_pal_1, 16 ) ;
		head.data = vms_sys_buf ;
		head.size = 0x728 ;
		break ;
	case 2:
		break ;
	case 3:
		strcpy( head.vms_name, vms_name_3 ) ;
		strcpy( head.comment, vms_comment_3 ) ;
		memcpy( head.pal, vms_pal_3, 16 ) ;
		head.data = vms_dow_buf ;
		head.size = 76 ;
		break ;
	case 4:
		strcpy( head.vms_name, vms_name_4 ) ;
		strcpy( head.comment, vms_comment_4 ) ;
		memcpy( head.pal, vms_pal_4, 16 ) ;
		head.data = vms_net_buf ;
		head.size = 0x980 ;
		break ;
	default:
		strcpy( head.vms_name, vms_name_1 ) ;
		strcpy( head.comment, vms_comment_1 ) ;
		memcpy( head.pal, vms_pal_1, 16 ) ;
		head.data = vms_sys_buf ;
		head.size = 0x728 ;
		break ;
	}
	head.icon = vms_icon_pal_tbl[type] ;
	head.unk_48 = vms_icon_img_tbl[type] ;
	head.icon_cnt = 1 ;
	head.icon_spd = 5 ;
	head.unk_50 = 0 ;
	head.unk_54 = 0 ;
	return buMakeBackupFileImage( &vms_buf_a, &head ) ;
}

/* the clock's date, and the one a file is saved with */
typedef struct VMS_DATE {
	Uint16	year ;
	Uint8	month, day, hour, minute, second, dayofweek ;
} VMS_DATE ;

/****************************************************************/
/*			Saves one of the game's files to a port's card;		*/
/*			-256 when there is nothing to save					*/
/****************************************************************/
int vms_file_save( Sint8 type, int port )
{
	NLA_RTC_DATE rtc ;
	VMS_DATE date ;
	const char *name ;
	Uint32 flag ;
	int size, rc ;

	size = vms_file_make( type ) ;
	nlaGetRTC( &rtc ) ;
	date.year = rtc.year ;
	date.month = rtc.month ;
	date.day = rtc.day ;
	date.hour = rtc.hour ;
	date.minute = rtc.minute ;
	date.second = rtc.second ;
	date.dayofweek = rtc.dayofweek ;
	switch( type ){
	case 0:
		name = "MVLVS.C2__UP" ;
		flag = 0x80000000 ;
		break ;
	case 1:
		name = "MVLVS.C2_SYS" ;
		flag = 0x80000000 ;
		break ;
	case 2:
		name = "MVLVS.C2__VM" ;
		flag = 0x80000000 ;
		break ;
	case 3:
		name = "MVLVS.C2_DOW" ;
		flag = 0x80000000 ;
		break ;
	case 4:
		name = "CAP_NET_.NET" ;
		flag = 0x800000ff ;
		break ;
	default:
		return -256 ;
	}
	if( size != -256 ) rc = buSaveFile( port, name, &vms_buf_a, size, &date, flag ) ;
	else return size ;
	return rc ;
}

/* the card tasks below are steps run each frame by whoever wants a card read or written: step 0
   shows the LCD picture of the port, 1 puts away the card the LCD showed before, 2 mounts this one,
   and what is left is the task's own work. They return -1 until they are done, then the result. */

/* a Backup file image's first bytes: where its data's size is kept */
/* the exec file's image in the cartridge, the one the card's menu starts */
#define VMS_EXEC_IMAGE	( ( void * )0x0D980000 )
#define VMS_BIT( port )	( 1 << ( port ) )

/****************************************************************/
/*			The card task of saving a file, step by step; -1	*/
/*			until it is done									*/
/****************************************************************/
int vms_task_save( Sint8 type, int port )
{
	register VMS_WORK *vms_w ;
	int shown ;
	int cur ;
	BU_FILE_INFO info ;
	int rc ;

	switch( vms_w = &vms_work, cur = vms_w->task_port, shown = vms_w->mount_port, vms_w->task_step ){
	case 0:
		vms_w->task_flag = 1 ;
		vms_w->task_port = port ;
		vms_lcd_conv( vms_lcd_img2 ) ;
		vms_lcd_put( vms_w->task_port ) ;
		vms_w->task_step = 1 ;
	case 1:
		if( vms_w->mount_port == vms_w->task_port ){
			vms_w->task_step = 3 ;
			return -1 ;
		}
		if( vms_w->mount_port != -1 ){
			if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
			rc = buUnmount( vms_slot[vms_w->mount_port].port ) ;
			if( rc ) break ;
			else {
				vms_slot[vms_w->mount_port].func = 18 ;
				vms_w->mount_port = -1 ;
			}
		}
		vms_w->task_step = 2 ;
		break ;
	case 2:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
		rc = buMountDisk( vms_slot[cur].port, vms_disk_work, ( vms_slot[cur].disk_blocks << 9 ) + 32 ) ;
		if( rc ) return -1 ;
		vms_w->bu_busy_bit |= VMS_BIT( port ) ;
		vms_w->task_step = 3 ;
		return -1 ;
	case 3:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) break ;
		else {
			vms_w->task_step = 4 ;
			break ; break ;
		}
	case 4:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			if( shown == cur ){
				vms_nop1( type ) ;
				rc = vms_file_save( type, vms_slot[vms_w->mount_port].port ) ;
				switch( rc ){
				default:
					return -1 ;
				case 0:
					vms_w->bu_busy_bit |= VMS_BIT( port ) ;
					break ;
				}
			}
			vms_w->task_step = 5 ;
		}
		return -1 ;
	case 5:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) break ;
		rc = buGetFileInfo( vms_slot[shown].port, "MVLVS.C2_SYS", &info ) ;
		if( shown = vms_w->mount_port, rc == 0 ){
			vms_slot[vms_slot[shown].port].year = info.year ;
			vms_slot[vms_slot[vms_w->mount_port].port].month = info.month ;
			vms_slot[vms_slot[vms_w->mount_port].port].day = info.day ;
			vms_slot[vms_slot[vms_w->mount_port].port].hour = info.hour ;
			vms_slot[vms_slot[vms_w->mount_port].port].minute = info.minute ;
			vms_slot[vms_slot[vms_w->mount_port].port].second = info.second ;
			vms_slot[vms_slot[vms_w->mount_port].port].week = info.week ;
		} else {
			vms_slot[vms_slot[shown].port].year = 0 ;
			vms_slot[vms_slot[vms_w->mount_port].port].month = 0 ;
			vms_slot[vms_slot[vms_w->mount_port].port].day = 0 ;
			vms_slot[vms_slot[vms_w->mount_port].port].hour = 0 ;
			vms_slot[vms_slot[vms_w->mount_port].port].minute = 0 ;
			vms_slot[vms_slot[vms_w->mount_port].port].second = 0 ;
			vms_slot[vms_slot[vms_w->mount_port].port].week = 0 ;
		}
		vms_lcd_conv( vms_lcd_img ) ;
		vms_lcd_put( vms_w->task_port ) ;
		vms_w->task_step = 0 ;
		vms_w->task_flag = 0 ;
		return vms_slot[vms_w->task_port].result ;
	}
	return -1 ;
}

/****************************************************************/
void vms_nop3( Sint8 type )
{
}

/****************************************************************/
/*			Checks the image the card gave, and copies what		*/
/*			it holds into the game's buffer for the file		*/
/****************************************************************/
int vms_file_unpack( Uint8 type )
{
	BU_FILE_HEAD head ;
	Uint8 *dst, *src ;
	int rc, i, cnt ;
	BU_IMAGE *img = &vms_buf_a ;

	switch( type ){
	case 0:
		if( img->size != 0x6000 ) return -191 ;
		break ;
	case 1:
		if( img->size != 0x728 ) return -191 ;
		break ;
	case 2:
		return -191 ;
		break ;
	case 3:
		break ;
	case 4:
		if( img->size != 0x980 ) return -191 ;
		break ;
	default:
		if( img->size != 0x728 ) return -191 ;
		break ;
	}
	if( type != 3 ) rc = buAnalyzeBackupFileImage( &head, &vms_buf_a ) ;
	else rc = 0 ;
	if( rc == 0 ){
		switch( type ){
		case 0:
			dst = ( Uint8 * )&vms_sys_buf ;
			src = head.data ;
			cnt = head.size ;
			memset( dst, 0, 0x6000 ) ;
			break ;
		case 1:
			dst = ( Uint8 * )&vms_sys_buf ;
			src = head.data ;
			cnt = head.size ;
			memset( dst, 0, 0x728 ) ;
			break ;
		case 3:
			dst = vms_dow_buf ;
			src = &vms_buf_a.unk_4c[0x280 - 0x4c] ;
			cnt = 76 ;
			memset( dst, 0, 76 ) ;
			break ;
		case 4:
			dst = vms_net_buf ;
			src = head.data ;
			cnt = head.size ;
			memset( dst, 0, 0x980 ) ;
			break ;
		default:
			dst = ( Uint8 * )&vms_sys_buf ;
			src = head.data ;
			cnt = head.size ;
			memset( dst, 0, 0x728 ) ;
			break ;
		}
		for( i = 0 ; i < cnt ; i ++ ) *dst ++ = *src ++ ;
	}
	return rc ;
}

/****************************************************************/
/*			Reads one of the game's files from a port's card	*/
/*			into the image buffer								*/
/****************************************************************/
int vms_file_load( Sint8 type, int port )
{
	const char *name ;
	int size ;

	switch( type ){
	case 0:
		name = "MVLVS.C2__UP" ;
		break ;
	case 1:
		name = "MVLVS.C2_SYS" ;
		break ;
	case 2:
		name = "MVLVS.C2__VM" ;
		break ;
	case 3:
		name = "MVLVS.C2_DOW" ;
		break ;
	case 4:
		name = "CAP_NET_.NET" ;
		break ;
	default:
		name = "MVLVS.C2_SYS" ;
		break ;
	}
	size = buGetFileSize( port, name ) ;
	return ( size < 0 ) ? size : buLoadFile( port, name, &vms_buf_a, size ) ;
}

/****************************************************************/
/* the card task of loading a file: its steps are those of vms_task_save up to the work itself */
int vms_task_load( Sint8 type, int port )
{
	VMS_SLOT *slot ;
	VMS_WORK *vms_w ;
	int cur ;
	int no, rc ;

	switch( vms_w = &vms_work, slot = vms_slot, cur = vms_w->task_port, vms_w->task_step ){
	case 0:
		vms_w->task_flag = 1 ;
		vms_w->task_port = port ;
		vms_lcd_conv( vms_lcd_img2 ) ;
		vms_lcd_put( vms_w->task_port ) ;
		vms_w->task_step = 1 ;
	case 1:
		if( vms_w->mount_port == vms_w->task_port ){
			vms_w->task_step = 3 ;
		}
		else {
			if( vms_w->mount_port != -1 ){
				if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
					rc = buUnmount( vms_slot[vms_w->mount_port].port ) ;
					if( !rc ){
						vms_slot[vms_w->mount_port].func = 18 ;
						vms_w->mount_port = -1 ;
						vms_w->task_step = 2 ;
					}
				}
			}
			else {
				vms_w->task_step = 2 ;
			}
		}
		break ;
	case 2:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			rc = buMountDisk( slot[cur].port, vms_disk_work, ( slot[cur].disk_blocks << 9 ) + 32 ) ;
			if( !rc ){
				vms_w->bu_busy_bit |= VMS_BIT( port ) ;
				vms_w->task_step = 3 ;
			}
		}
		break ;
	case 3:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			no = vms_w->mount_port ;
			if( no == cur ){
				rc = vms_file_load( type, slot[no].port ) ;
				switch( rc ){
				case 0:
					vms_w->bu_busy_bit |= VMS_BIT( port ) ;
					vms_w->task_step = 4 ;
					break ;
				case BU_NOT_FOUND:
					vms_w->task_step = 4 ;
					break ;
				case BU_ERR_03:
					vms_w->task_step = 4 ;
					break ;
				case BU_BUSY:
					vms_w->task_step = 4 ;
					break ;
				case -1:
					vms_w->task_step = 4 ;
					break ;
				}
			}
			else {
				vms_w->task_step = 4 ;
			}
		}
		break ;
	case 4:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			if( 0 ) cur = type = 0 ;
			slot += cur ;
			if( slot->result == 0 ){
				rc = vms_file_unpack( type ) ;
				if( rc == 0 ){
					vms_nop3( type ) ;
					vms_lcd_conv( vms_lcd_img ) ;
					vms_lcd_put( vms_w->task_port ) ;
					vms_w->task_step = 0 ;
					vms_w->task_flag = 0 ;
					return rc ;
				}
				else {
					vms_nop0( type ) ;
					vms_lcd_conv( vms_lcd_img ) ;
					vms_lcd_put( vms_w->task_port ) ;
					vms_w->task_step = 0 ;
					vms_w->task_flag = 0 ;
					return rc ;
				}
			}
			else {
				vms_nop0( type ) ;
				vms_lcd_conv( vms_lcd_img ) ;
				vms_lcd_put( vms_w->task_port ) ;
				vms_w->task_step = 0 ;
				vms_w->task_flag = 0 ;
				return vms_slot[vms_w->task_port].result ;
			}
		}
		break ;
	}
	return -1 ;
}

/****************************************************************/
/*			Defragments a card's free blocks					*/
/****************************************************************/
int vms_defrag( int port )
{
	int rc ;

	rc = buDefragDisk( port, &vms_buf_b[2] ) ;
	return rc ;
}

/****************************************************************/
/*			Puts the game's exec file (what the card's menu		*/
/*			starts) on a card									*/
/****************************************************************/
int vms_exec_save( int port )
{
	NLA_RTC_DATE rtc ;
	VMS_DATE date ;
	int rc ;

	nlaGetRTC( &rtc ) ;
	date.year = rtc.year ;
	date.month = rtc.month ;
	date.day = rtc.day ;
	date.hour = rtc.hour ;
	date.minute = rtc.minute ;
	date.second = rtc.second ;
	date.dayofweek = rtc.dayofweek ;
	rc = buSaveExecFile( port, "MVLVS.C2__VM", VMS_EXEC_IMAGE, 64, &date, 0x80000000 ) ;
	return rc ;
}

/****************************************************************/
/*			The card task of making room and saving the exec	*/
/*			file: its steps are those of vms_task_save			*/
/****************************************************************/
int vms_task_defrag( int port )
{
	register VMS_WORK *vms_w ;
	int cur ;
	int shown ;
	int rc ;

	switch( vms_w = &vms_work, cur = vms_w->task_port, shown = vms_w->mount_port, vms_w->task_step ){
	case 0:
		vms_w->task_flag = 1 ;
		vms_w->task_port = port ;
		vms_lcd_conv( vms_lcd_img2 ) ;
		vms_lcd_put( vms_w->task_port ) ;
		vms_w->task_step = 1 ;
	case 1:
		if( vms_w->mount_port == vms_w->task_port ){
			vms_w->task_step = 3 ;
			break ;
		}
		if( vms_w->mount_port != -1 ){
			if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
			rc = buUnmount( vms_slot[vms_w->mount_port].port ) ;
			if( rc ) break ;
			else {
				vms_slot[vms_w->mount_port].func = 18 ;
				vms_w->mount_port = -1 ;
			}
		}
		vms_w->task_step = 2 ;
		break ;
	case 2:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
		rc = buMountDisk( vms_slot[cur].port, vms_disk_work, ( vms_slot[cur].disk_blocks << 9 ) + 32 ) ;
		if( !rc ){
			vms_w->bu_busy_bit |= VMS_BIT( port ) ;
			vms_w->task_step = 3 ;
		}
		break ;
	case 3:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			if( shown == cur ){
				if( vms_slot[shown].free_blocks >= 64 && vms_slot[shown].exec_free < 64 ){
					rc = vms_defrag( vms_slot[shown].port ) ;
					switch( rc ){
					case 0:
						vms_w->bu_busy_bit |= VMS_BIT( port ) ;
						break ;
					default:
						return -1 ;
					}
				}
			}
			vms_w->task_step = 4 ;
		}
		break ;
	case 4:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			if( shown == cur ){
				rc = vms_exec_save( vms_slot[shown].port ) ;
				switch( rc ){
				default:
					return -1 ;
				case 0:
					vms_w->bu_busy_bit |= VMS_BIT( port ) ;
					break ;
				}
			}
			vms_w->task_step = 5 ;
		}
		return -1 ;
	case 5:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			vms_lcd_conv( vms_lcd_img ) ;
			vms_lcd_put( vms_w->task_port ) ;
			vms_w->task_step = 0 ;
			vms_w->task_flag = 0 ;
			return vms_slot[vms_w->task_port].result ;
		}
		break ;
	}
	return -1 ;
}

/* reads the game's exec file's extra data from a card into one of the card data buffers */
int vms_load_ex( port, no )
int port ;
Uint8 no ;
{
	memset( &vms_buf_b[no], 0, 0x200 ) ;
	return buLoadFileEx( port, "MVLVS.C2__VM", &vms_buf_b[no], 5, 1 ) ;
}

/****************************************************************/
/*			The card task of reading that data					*/
/****************************************************************/
int vms_task_load_ex( int port, Uint8 no )
{
	VMS_SLOT *slot ;
	VMS_WORK *vms_w ;
	int cur ;
	int shown ;
	int rc ;

	switch( vms_w = &vms_work, slot = vms_slot, cur = vms_w->task_port, vms_w->task_step ){
	case 0:
		vms_w->task_flag = 1 ;
		vms_w->task_port = port ;
		vms_lcd_conv( vms_lcd_img2 ) ;
		vms_lcd_put( vms_w->task_port ) ;
		vms_w->task_step = 1 ;
	case 1:
		if( vms_w->mount_port == vms_w->task_port ){
			vms_w->task_step = 3 ;
			break ;
		}
		if( vms_w->mount_port != -1 ){
			if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
			rc = buUnmount( vms_slot[vms_w->mount_port].port ) ;
			if( rc ) break ;
			else {
				vms_slot[vms_w->mount_port].func = 18 ;
				vms_w->mount_port = -1 ;
			}
		}
		vms_w->task_step = 2 ;
		break ;
	case 2:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
		rc = buMountDisk( slot[cur].port, vms_disk_work, ( slot[cur].disk_blocks << 9 ) + 32 ) ;
		if( !rc ){
			vms_w->bu_busy_bit |= VMS_BIT( port ) ;
			vms_w->task_step = 3 ;
		}
		break ;
	case 3:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
		shown = vms_w->mount_port ;
		if( shown == cur ){
			rc = vms_load_ex( slot[shown].port ) ;
			switch( rc ){
			default:
				return -1 ;
			case 0:
				vms_w->bu_busy_bit |= VMS_BIT( port ) ;
				break ;
			}
		}
		vms_w->task_step = 4 ;
		return -1 ;
	case 4:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			vms_lcd_conv( vms_lcd_img ) ;
			vms_lcd_put( vms_w->task_port ) ;
			vms_w->task_step = 0 ;
			vms_w->task_flag = 0 ;
			return vms_slot[vms_w->task_port].result ;
		}
		break ;
	}
	return -1 ;
}

/****************************************************************/
/*			Rewrites the data of the exec file from one of		*/
/*			the card data buffers								*/
/****************************************************************/
int vms_exec_rewrite( int port, Uint8 no )
{
	vms_nop2( no ) ;
	return buRewriteExecFile( port, "MVLVS.C2__VM", &vms_buf_b[no], 5, 1 ) ;
}

/****************************************************************/
/*			The card task of rewriting it, with the buffer's	*/
/*			sum put in first									*/
/****************************************************************/
int vms_task_rewrite( int port, Uint8 no )
{
	VMS_CARD_DATA *card = &vms_buf_b[no].d ;
	VMS_SLOT *slot ;
	VMS_WORK *vms_w ;
	int cur ;
	int shown ;
	int rc ;

	switch( vms_w = &vms_work, slot = vms_slot, cur = vms_w->task_port, vms_w->task_step ){
	case 0:
		vms_w->task_flag = 1 ;
		vms_w->task_port = port ;
		vms_lcd_conv( vms_lcd_img2 ) ;
		vms_lcd_put( vms_w->task_port ) ;
		vms_w->task_step = 1 ;
		rc = vms_sum_b( no ) ;
		card->sum = rc ;
	case 1:
		if( vms_w->mount_port == vms_w->task_port ){
			vms_w->task_step = 3 ;
			break ;
		}
		if( vms_w->mount_port != -1 ){
			if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
			rc = buUnmount( vms_slot[vms_w->mount_port].port ) ;
			if( rc ) break ;
			else {
				vms_slot[vms_w->mount_port].func = 18 ;
				vms_w->mount_port = -1 ;
			}
		}
		vms_w->task_step = 2 ;
		break ;
	case 2:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
		rc = buMountDisk( slot[cur].port, vms_disk_work, ( slot[cur].disk_blocks << 9 ) + 32 ) ;
		if( !rc ){
			vms_w->bu_busy_bit |= VMS_BIT( port ) ;
			vms_w->task_step = 3 ;
		}
		break ;
	case 3:
		if( vms_w->bu_busy_bit & VMS_BIT( port ) ) return -1 ;
		shown = vms_w->mount_port ;
		if( shown == cur ){
			rc = vms_exec_rewrite( slot[shown].port, no ) ;
			switch( rc ){
			default:
				return -1 ;
			case 0:
				vms_w->bu_busy_bit |= VMS_BIT( port ) ;
				break ;
			}
		}
		vms_w->task_step = 4 ;
		return -1 ;
	case 4:
		if( !( vms_w->bu_busy_bit & VMS_BIT( port ) ) ){
			vms_lcd_conv( vms_lcd_img ) ;
			vms_lcd_put( vms_w->task_port ) ;
			vms_w->task_step = 0 ;
			vms_w->task_flag = 0 ;
			return vms_slot[vms_w->task_port].result ;
		}
		break ;
	}
	return -1 ;
}

/****************************************************************/
/* what a card's data holds, put into the points and unlocks: 0 when it holds none */
int vms_card_read( Uint8 no )
{
	VMS_CARD_DATA *card = &vms_buf_b[no].d ;
	int i, pts, add, rest ;

	if( !card->unlock[0] ) return 0 ;
	for( i = 0 ; i < 56 ; i ++ ){
		if( card->unlock[i] & 0x80 ){
			if( ( card->unlock[i] & 63 ) >= 32 ) vms_work.card_open[1] |= 1 << ( ( card->unlock[i] & 63 ) - 32 ) ;
			else vms_work.card_open[0] |= 1 << ( card->unlock[i] & 63 ) ;
		}
		if( card->unlock[i] & 0x40 ){
			if( ( card->unlock[i] & 63 ) >= 32 ) vms_work.card_ex[1] |= 1 << ( ( card->unlock[i] & 63 ) - 32 ) ;
			else vms_work.card_ex[0] |= 1 << ( card->unlock[i] & 63 ) ;
		}
	}
	pts = card->unk_4a[0] ;
	pts += card->unk_4a[1] * 100 ;
	if( pts < 9999 ) pts ++ ;
	card->unk_4a[0] = pts % 100 ;
	card->unk_4a[1] = pts / 100 ;
	if( card->level - vms_work.point_lv > 0 ) add = ( card->level - vms_work.point_lv ) * 10 ;
	else add = 10 ;
	vms_work.add_point[no] = add ;
	if( vms_work.point_lv - card->level > 0 ) rest = ( vms_work.point_lv - card->level ) * 10 ;
	else rest = 10 ;
	pts = card->unk_46[0] ;
	pts += card->unk_46[1] * 100 ;
	if( pts < 9999 ){
		pts += rest ;
		if( pts > 9999 ) pts = 9999 ;
	}
	card->unk_46[0] = pts % 100 ;
	card->unk_46[1] = pts / 100 ;
	vms_work.get_point[no] = rest ;
	while( rest >= 100 ){
		if( card->level < 99 ){
			card->level ++ ;
			rest -= 100 ;
		} else rest = 99 ;
	}
	card->level_pt += rest ;
	if( card->level_pt >= 100 ){
		if( card->level < 99 ){
			card->level ++ ;
			card->level_pt -= 100 ;
		} else card->level_pt = 99 ;
	}
	return 1 ;
}

/****************************************************************/
/*			The card task of one side's card, run each frame	*/
/*			until it says it is done: 0 while it works, 1 if	*/
/*			the side's card is already being looked at, 2		*/
/*			when it is done and the side can not join, 3 to		*/
/*			5 for the ways it fails, 6 when it waited too		*/
/*			long												*/
/****************************************************************/
int vms_task_card( int port, Uint8 side )
{
	VMS_CARD_DATA *card = &vms_buf_b[side].d ;
	int rc ;

	if( 0 ) side = 4 ;
	switch( vms_work.card_task_step[side][0] ){
	case 0:
		if( vms_slot[port].sys_file != 2 || vms_slot[port].vm_file != 2 ) return 5 ;
		if( vms_work.card_done_bit & VMS_BIT( side ) ) return 1 ;
		if( vms_work.card_busy_bit & VMS_BIT( side ) ) return 1 ;
		vms_work.card_busy_bit |= VMS_BIT( side ) ;
		vms_work.card_task_step[side][0] ++ ;
		return 0 ;
	case 1:
		rc = vms_task_load_ex( port, side ) ;
		if( rc != -1 ){
			if( rc == 0 ){
				vms_work.card_task_step[side][0] ++ ;
				return 0 ;
			}
			if( rc == BU_BUSY ){
				vms_work.card_task_step[side][1] = vms_work.card_task_step[side][0] ;
				vms_work.card_task_step[side][0] = 4 ;
				vms_work.card_task_timer[side] = 60 ;
				break ;
			} else {
				vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
				vms_work.card_task_step[side][0] = 0 ;
				return 3 ;
			}
		}
		rc = vms_card_ck( port ) ;
		if( rc == 0 || rc == -1 ){
			vms_work.card_task_step[side][1] = vms_work.card_task_step[side][0] ;
			vms_work.card_task_step[side][0] = 4 ;
			vms_work.card_task_timer[side] = 60 ;
		}
		return 0 ;
	case 2:
		rc = vms_card_ck( port ) ;
		if( rc == 0 || rc == -1 ){
			vms_work.card_task_step[side][1] = vms_work.card_task_step[side][0] ;
			vms_work.card_task_step[side][0] = 4 ;
			vms_work.card_task_timer[side] = 60 ;
			break ;
		}
		rc = ( Uint8 )vms_sum_b( side ) ;
		if( rc != card->sum ){
			vms_work.card_task_step[side][0] = 0 ;
			vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
			return 5 ;
		}
		rc = vms_card_read( side ) ;
		if( !rc ){
			vms_work.card_task_step[side][0] = 0 ;
			vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
			return 0 ;
		} else if( Sys->vm_write ){
			vms_work.card_task_step[side][0] ++ ;
			return 0 ;
		} else {
			vms_work.card_task_step[side][0] = 0 ;
			vms_work.card_done_bit |= VMS_BIT( side ) ;
			vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
			return 2 ;
		}
	case 3:
		rc = vms_task_rewrite( port, side ) ;
		if( rc != -1 ){
			if( rc == 0 ){
				vms_work.card_task_step[side][0] = 0 ;
				vms_work.card_done_bit |= VMS_BIT( side ) ;
				vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
				return 2 ;
			} else if( rc == BU_BUSY ){
				vms_work.card_task_step[side][1] = vms_work.card_task_step[side][0] ;
				vms_work.card_task_step[side][0] = 4 ;
				vms_work.card_task_timer[side] = 60 ;
				break ;
			} else {
				vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
				vms_work.card_task_step[side][0] = 0 ;
				return 4 ;
			}
		}
		rc = vms_card_ck( port ) ;
		if( rc == 0 || rc == -1 ){
			vms_work.card_task_step[side][1] = vms_work.card_task_step[side][0] ;
			vms_work.card_task_step[side][0] = 4 ;
			vms_work.card_task_timer[side] = 60 ;
			break ;
		}
		return 0 ;
	case 4:
		vms_work.task_step = 0 ;
		if( -- vms_work.card_task_timer[side] ){
			rc = vms_card_ck( port ) ;
			if( rc == 0 || rc == -1 ) return 0 ;
			vms_work.card_task_step[side][0] = vms_work.card_task_step[side][1] = 0 ;
			vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
			return 0 ;
		}
		vms_work.card_task_step[side][0] = vms_work.card_task_step[side][1] = 0 ;
		vms_work.card_busy_bit &= ~VMS_BIT( side ) ;
		return 6 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The sum of the bytes of a card data buffer			*/
/*			before its sum										*/
/*==============================================================*/
static int vms_sum_b( Uint8 no )
{
	Uint8 *p = vms_buf_b[no].raw ;
	int i ;
	Uint8 sum ;

	for( i = 0, sum = 0 ; i < 80U ; i ++ ) sum += *p ++ ;
	return sum ;
}

/****************************************************************/
/*			The sum of the download data's bytes before its		*/
/*			last; nothing calls it								*/
/****************************************************************/
int vms_sum_c( void )
{
	Uint8 *p = vms_dow_buf ;
	int i ;
	Uint8 sum ;

	for( i = 0, sum = 0 ; i < sizeof( vms_dow_buf ) - 1 ; i ++ ) sum += *p ++ ;
	return sum ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "vms.c" End of File	*/
