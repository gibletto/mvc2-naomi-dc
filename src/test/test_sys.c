/* The test program's system: the start-up of the libraries and the screen, the controls read from
   the JAMMA I/O board each vertical blank, the backup data and settings and the credit display. */
#include "player.h"
#include "book.h"
#include "naomi.h"
#include "shinobi.h"
#include "test.h"

/* SHC's built-ins, as its smachine.h names them */
#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()

/* Shinobi / Kamui */
extern void syHwInit_NAOMI( void ) ;
extern void syHwInit2( void ) ;
extern int sprintf( char *s, const char *format, ... ) ;

#define PDD_DEVTYPE_VMS	14	/* a memory card: storage, a screen and a clock */

/* the peripheral library's receive and send buffers */
static Uint8 peri_recv_buff[0xc020] ;
static Uint8 peri_send_buff[0xc020] ;

static void pad_read( int side ) ;
static void jamma_read( void ) ;
static void peri_ck( void ) ;
static void backup_init( void ) ;
static void sys_set_init( void ) ;
void sys_set_load( void ) ;

/*==============================================================*/
/*			A Dreamcast pad's buttons are not read here			*/
/*==============================================================*/
static void pad_read( int side )
{
}

/*==============================================================*/
/* read the switches of the I/O boards: with one board both sides are on it, with two each has its own
   (two of its statements have no effect) */
static void jamma_read( void )
{
	char *in ;
	char *sw ;
	char *analog ;
	char *sw2 ;
	char *analog2 ;
	char c, hi, lo ;
	Uint16 sys, sys2 ;
	SW_WORK *kw ;

	in = ( char * )&nlaInput6149[5] ;
	kw = &board_sw ;
	c = *in ;
	kw->old = kw->now ;
	kw->now = ~( Uint8 )c & 0xff ;
	kw->on = ~kw->old & kw->now ;
	kw->off = kw->old & ~kw->now ;
	if( !jamma_cnt ) return ;
	sw = ( in = nlaJAMMAInput[0] ) + 512 ;
	sw += nlaJAMMAAcknowledgeIndex[1][NLA_JAMMA_COMMAND_SWITCH_READ] ;
	analog = sw ;
	analog += nlaJAMMAAcknowledgeIndex[1][NLA_JAMMA_COMMAND_ANALOG_READ] ;
	if( jamma_cnt == 1 ){
		if( *analog == NLA_JAMMA_REPORT_OK ){
			sw_work[0].stick_x = 0 ;
			sw_work[0].stick_y = 0 ;
			sw_work[0].stick_y ;
			sw_work[1].stick_x = 0 ;
			sw_work[1].stick_y = 0 ;
		}
		if( *sw == NLA_JAMMA_REPORT_OK ){
			sw ++ ;
			c = *sw ++ ;
			sys_sw.old = sys_sw.now ;
			sys_sw.now = ( Uint8 )c ;
			sys_sw.on = ~sys_sw.old & sys_sw.now ;
			sys_sw.off = sys_sw.old & ~sys_sw.now ;
			hi = *sw ++ ;
			lo = *sw ++ ;
			if( 0 ) hi = 0 ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 0 ) ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
			hi = *sw ++ ;
			lo = *sw ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 1 ) ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		} else {
			sys_sw.old = sys_sw.now ;
			sys_sw.on = ~sys_sw.old & sys_sw.now ;
			sys_sw.off = sys_sw.old & ~sys_sw.now ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		}
	} else {
		sw2 = in + 1024 ;
		sw2 += nlaJAMMAAcknowledgeIndex[2][NLA_JAMMA_COMMAND_SWITCH_READ] ;
		analog2 = sw2 ;
		analog2 += nlaJAMMAAcknowledgeIndex[2][NLA_JAMMA_COMMAND_ANALOG_READ] ;
		if( *analog2 == NLA_JAMMA_REPORT_OK ){
			sw_work[0].stick_x = 0 ;
			sw_work[0].stick_y = 0 ;
		}
		if( *sw2 == NLA_JAMMA_REPORT_OK ){
			sw2 ++ ;
			c = *sw2 ++ ;
			sys = ( Uint8 )c ;
			hi = *sw2 ++ ;
			lo = *sw2 ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 0 ) ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
		} else {
			sys = 0 ;
			sw_work[0].old = sw_work[0].now ;
			sw_work[0].on = ~sw_work[0].old & sw_work[0].now ;
			sw_work[0].off = sw_work[0].old & ~sw_work[0].now ;
		}
		if( *analog == NLA_JAMMA_REPORT_OK ){
			sw_work[1].stick_x = 0 ;
			sw_work[1].stick_y = 0 ;
		}
		if( *sw == NLA_JAMMA_REPORT_OK ){
			sw ++ ;
			c = *sw ++ ;
			sys2 = ( Uint8 )c ;
			hi = *sw ++ ;
			lo = *sw ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].now = ( ( Uint16 )hi << 8 & 0xff00 ) + ( Uint8 )lo ;
			pad_read( 1 ) ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		} else {
			sys2 = 0 ;
			sw_work[1].old = sw_work[1].now ;
			sw_work[1].on = ~sw_work[1].old & sw_work[1].now ;
			sw_work[1].off = sw_work[1].old & ~sw_work[1].now ;
		}
		sys |= sys2 ;
		sys_sw.old = sys_sw.now ;
		sys_sw.now = sys ;
		sys_sw.on = ~sys_sw.old & sys_sw.now ;
		sys_sw.off = sys_sw.old & ~sys_sw.now ;
	}
}

/****************************************************************/
/*			A held key repeats: after 36 frames it is given		*/
/*			again every 6										*/
/****************************************************************/
void key_repeat( SW_WORK *kw )
{
	kw->rep = 0 ;
	if( kw->old == kw->now ){
		kw->rep_cnt ++ ;
		if( kw->rep_cnt > 30 ){
			if( kw->rep_cnt > 35 ){
				kw->rep_cnt = 30 ;
				kw->rep = kw->now ;
			}
		}
	} else kw->rep_cnt = 0 ;
}

/*==============================================================*/
/*			What is connected to each side's Dreamcast port,	*/
/*			as far as the setting has it looked: a memory		*/
/*			card or a vibration pack in the pad's first and		*/
/*			second sockets, and the pad itself					*/
/*==============================================================*/
static void peri_ck( void )
{
	PDS_PERIPHERALINFO *info ;

	peri_bit = 0 ;
	if( sys_set.vms_mode == 0 ) return ;
	info = pdGetPeripheralInfo( 7 ) ;
	if( info->type == PDD_DEVTYPE_VMS || ( info->type & PDD_DEVTYPE_VIBRATION ) ) peri_bit |= 4 ;
	info = pdGetPeripheralInfo( 13 ) ;
	if( info->type == PDD_DEVTYPE_VMS || ( info->type & PDD_DEVTYPE_VIBRATION ) ) peri_bit |= 8 ;
	if( sys_set.vms_mode == 1 ) return ;
	info = pdGetPeripheralInfo( 8 ) ;
	if( info->type == PDD_DEVTYPE_VMS || ( info->type & PDD_DEVTYPE_VIBRATION ) ) peri_bit |= 0x10 ;
	info = pdGetPeripheralInfo( 14 ) ;
	if( info->type == PDD_DEVTYPE_VMS || ( info->type & PDD_DEVTYPE_VIBRATION ) ) peri_bit |= 0x20 ;
	pad_peri[0] = pdGetPeripheral( 6 ) ;
	pad_peri[1] = pdGetPeripheral( 12 ) ;
	if( pad_peri[0]->info->type & PDD_DEVTYPE_CONTROLLER ) peri_bit |= 1 ;
	if( pad_peri[1]->info->type & PDD_DEVTYPE_CONTROLLER ) peri_bit |= 2 ;
}

/****************************************************************/
/*			The vertical blank: count the frame and read the	*/
/*			controls											*/
/****************************************************************/
Sint32 VBlankInIrq( void )
{
	Sys->vblank_cnt ++ ;
	vsync_count ++ ;
	stream_start_flag = 0 ;
	if( task_wait ) task_wait -- ;
	nlaPutLED( Sys->vblank_cnt ) ;
	if( !rom_dma_flag ) pdExecPeripheralServer() ;
	jamma_read() ;
	peri_ck() ;
	return 0 ;
}

/****************************************************************/
/*			Set the system up: the hardware, the libraries,		*/
/*			the screen, the controls, the backup data, the		*/
/*			settings and the interrupts; test is set when		*/
/*			the test menu is wanted								*/
/****************************************************************/
void system_init( int test )
{
	Float work[129] ;
	int i ;
	NLA_JAMMA_INFO *info ;

	set_imask( 14 ) ;
	syHwInit_NAOMI() ;
	nlaGetBootFlag() ;
	sys_flag = nlaGetSystemFlag() ;
	if( sys_flag->DisplayMode == NLA_DISPLAY_31K ) nlInitLibrary( 0, 0x80, 1 ) ;
	else nlInitLibrary( 9, 0x80, 1 ) ;
	nlSysSetTextureSize( 0x300000 ) ;
	set_imask( 14 ) ;
	syHwInit2() ;
	set_imask( 0 ) ;
	nlSysMetricsInit() ;
	nlSysUseLatency2V() ;
	nlSysSetDirectListTypeLatency2V( 0 ) ;
	nlSysSetVertexBufferBaseAddress( ( void * )0x0cf00000 ) ;
	nlSysCreatePunchBuffer( 0x10000 ) ;
	nlCreateVertexBuffer( 0, 0x4000, 0x24000, 0x4000 ) ;
	nlSysSetTimeOutCount( 10, 0 ) ;
	nlInitPrint() ;
	nlPrnIndentOn() ;
	nlSetBackColor( 0, 0, 0 ) ;
	nlSetBorderColor( 0 ) ;
	nlInitCommonTexture( common_tex_work, 0xc00, 0x1000 ) ;
	nlInitMatrix( matrix_stack, 64 ) ;
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlPerspective( 0x1d28, 1.3333334f, 0.3f, 12000.0f ) ;
	nlMatrixMode( 1 ) ;
	for( i = 0 ; i < 129 ; i ++ ) work[i] = 0.0f ;
	nlFogTableColor( 0x101010 ) ;
	nlFogDensity( 0xff09 ) ;
	nlSetFontSize( 16, 16 ) ;
	if( !test ){
		nlLocate( 12, 14 ) ;
		nlPrintf( "LOADING GAME NOW...\n" ) ;
	} else {
		nlLocate( 8, 10 ) ;
		nlPrintf( "LOADING GAME TEST NOW...\n" ) ;
	}
	nlWaitVSync() ;
	i = 2000000 ;
	while( i -- ) ;
	nlSetFontSize( 8, 8 ) ;
	set_imask( 14 ) ;
	pdInitPeripheral( 0, peri_recv_buff, peri_send_buff ) ;
	nlaInitSystem() ;
	nlaJAMMAInit() ;
	nlaJAMMACreditInit() ;
	jamma_cnt = nlaJAMMANodes ;
	if( !jamma_cnt ) jamma_err = 1 ;
	else {
		jamma_err = 0 ;
		nlaJAMMASetPrimaryNode( 1 ) ;
		info = nlaJAMMAInfo ;
		if( info[1].Switch[0] < 2 || info[1].Switch[1] < 10 || info[1].Coin < 2 ) jamma_err = 2 ;
		if( jamma_cnt == 2 ){
			if( info[2].Switch[0] < 2 || info[2].Switch[1] < 10 || info[2].Coin < 2 ) jamma_err = 2 ;
		}
	}
	set_imask( 0 ) ;
	if( test ) nlaInTestMode() ;
	nlaROMTypeSet( 1, 0 ) ;
	credit_conf = nlaGetCreditConf() ;
	nlaGetCreditData( &credit_info ) ;
	backup_init() ;
	sys_set_load() ;
	rom_dma_flag = 0 ;
	nlInitInterrupt() ;
	nlSetIntrFunction( 1, VBlankInIrq ) ;
	nlSetIntrFunction( 0, vblank_func ) ;
}

/****************************************************************/
/*			The test button, of the I/O board or of the			*/
/*			NAOMI board: go to the test menu					*/
/****************************************************************/
int test_sw_ck( void )
{
	int ret = 0 ;

	if( ( sys_sw.now & 0x80 ) || ( board_sw.now & 0x10 ) ){
		task_wait = 3 ;
		Sys->play_mode = 0x80 ;
		Sys->task_no = 3 ;
		Sys->task_step = 0 ;
		Sys->scene_no = 0 ;
		Sys->routine_0 = 0 ;
		Sys->routine_1 = 0 ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		ret = 1 ;
	}
	return ret ;
}

/****************************************************************/
/*			Go to the boot ROM's test menu						*/
/****************************************************************/
void test_mode_jump( void )
{
	volatile int i ;

	nlSetFontSize( 16, 16 ) ;
	nlLocate( 8, 10 ) ;
	nlPrintColor( -1 ) ;
	nlSetBackColor( 0, 0, 0 ) ;
	nlPrintf( "LOADING TEST MODE NOW...\n" ) ;
	nlWaitVSync() ;
	i = 2000000 ;
	while( i -- ) ;
	nlInitInterrupt() ;
	nlSetIntrFunction( 1, 0 ) ;
	i = 16500000 ;
	while( i -- ) ;
	nlSetFontSize( 8, 8 ) ;
	nlaJumpTestMode() ;
}

/****************************************************************/
/* both start buttons and the first side's second button held with the service switch clears the
   book-keeping */
void book_clear_ck( void )
{
	if( ( sw_work[0].now & KEY_START ) && ( sw_work[1].now & KEY_START )
	 && ( sw_work[0].now & KEY_LP ) && ( board_sw.now & 0x20 ) ){
		book.coin_all = 0 ;
		book.service_all = 0 ;
		book.play_all = 0 ;
		book.coin = 0 ;
		book.service = 0 ;
		book.play = 0 ;
		book.point = 0 ;
		book.char_open[0] = 0 ;
		book.char_open[1] = 0 ;
		book.char_open_all[0] = 0 ;
		book.char_open_all[1] = 0 ;
		book.play_time = 0 ;
		book_save = 1 ;
	}
}

/****************************************************************/
void book_count_clear( void )
{
	book.coin = 0 ;
	book.service = 0 ;
	book.play = 0 ;
	book_save = 1 ;
}

/****************************************************************/
/*			Count a game played									*/
/****************************************************************/
void book_play_add( void )
{
	book.play ++ ;
	book.play_all ++ ;
	book_save = 1 ;
}

/****************************************************************/
/*			Count the coins and service credits put in since	*/
/*			the last frame, and save the book-keeping			*/
/****************************************************************/
void book_coin_ck( void )
{
	NLA_BACKUP_BANK bank ;

	nlaGetBackupData( &bank ) ;
	if( coin_old != bank.credit.TotalCoin ){
		book.coin += bank.credit.TotalCoin - coin_old ;
		book.coin_all += bank.credit.TotalCoin - coin_old ;
		coin_old = bank.credit.TotalCoin ;
		book_save = 1 ;
	}
	if( service_old != bank.credit.ServiceCredit ){
		book.service += bank.credit.ServiceCredit - service_old ;
		book.service_all += bank.credit.ServiceCredit - service_old ;
		service_old = bank.credit.ServiceCredit ;
		book_save = 1 ;
	}
	nlaUpdateSystemBackup() ;
	if( book_save ) nlaUpdateUserBackup() ;
	book_save = 0 ;
	Sys->st_demo_flag = 0 ;
}

/*==============================================================*/
/*			The book-keeping in the backup memory: start it		*/
/*			again if it is new or not the game's				*/
/*==============================================================*/
static void backup_init( void )
{
	NLA_BACKUP_BANK bank ;
	int ret ;

	book.id = 0 ;
	ret = nlaInitUserBackup( 64, ( char * )&book ) ;
	if( ret == 3 || book.id != BOOK_ID ){
		book.id = BOOK_ID ;
		book.coin = 0 ;
		book.service = 0 ;
		book.play = 0 ;
		book.coin_all = 0 ;
		book.service_all = 0 ;
		book.play_all = 0 ;
		book.point = 0 ;
		book.char_open[0] = 0 ;
		book.char_open[1] = 0 ;
		book.char_open_all[0] = 0 ;
		book.char_open_all[1] = 0 ;
		book.play_time = 0 ;
		nlaUpdateUserBackup() ;
	}
	book_save = 0 ;
	nlaGetBackupData( &bank ) ;
	coin_old = bank.credit.TotalCoin ;
	service_old = bank.credit.ServiceCredit ;
}

/* the settings, copied a member at a time */
#define SYS_SET_COPY( dst, src )	\
	( dst )->id = ( src )->id ; ( dst )->country = ( src )->country ;	\
	( dst )->stereo = ( src )->stereo ; ( dst )->demo_sound_half = ( src )->demo_sound_half ;	\
	( dst )->cont_enable = ( src )->cont_enable ; ( dst )->voice_type = ( src )->voice_type ;	\
	( dst )->difficulty = ( src )->difficulty ; ( dst )->damage_level = ( src )->damage_level ;	\
	( dst )->time_speed = ( src )->time_speed ; ( dst )->vms_mode = ( src )->vms_mode ;	\
	( dst )->vm_write = ( src )->vm_write ; ( dst )->speed_fix = ( src )->speed_fix ;	\
	( dst )->event_flag = ( src )->event_flag ; ( dst )->no_challenge_flag = ( src )->no_challenge_flag ;	\
	( dst )->open_lv = ( src )->open_lv ;

const SYS_SET sys_set_tbl[8] = {
	{ BOOK_ID, 0, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 1, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 2, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 3, 1, 0, 1, 0, 3, 1, 1 },
	{ BOOK_ID, 4, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 5, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 6, 1, 0, 1, 0, 1, 1, 1 },
	{ BOOK_ID, 7, 1, 0, 1, 0, 1, 1, 1 },
} ;

/*==============================================================*/
/*			The settings of the country the board is set to		*/
/*==============================================================*/
static void sys_set_init( void )
{
	const SYS_SET *tbl = &sys_set_tbl[sys_flag->Country] ;

	SYS_SET_COPY( &sys_set, tbl )
	conf_country = sys_set.country ;
	conf_stereo = sys_set.stereo ;
	conf_demo_sound_half = sys_set.demo_sound_half ;
	conf_cont_enable = sys_set.cont_enable ;
	conf_voice_type = sys_set.voice_type ;
	conf_difficulty = sys_set.difficulty ;
	conf_damage_level = sys_set.damage_level ;
	conf_time_speed = sys_set.time_speed ;
	conf_vms_mode = sys_set.vms_mode ;
	conf_vm_write = sys_set.vm_write ;
	conf_speed_fix = sys_set.speed_fix ;
	conf_event_flag = sys_set.event_flag ;
	conf_no_challenge_flag = sys_set.no_challenge_flag ;
	conf_open_lv = sys_set.open_lv ;
}

/****************************************************************/
void sys_set_get( SYS_SET *set )
{
	const SYS_SET *tbl = &sys_set_tbl[sys_flag->Country] ;

	SYS_SET_COPY( set, tbl )
}

/****************************************************************/
/*			Set the settings again, and wait with the			*/
/*			interrupts off while they are written				*/
/****************************************************************/
void sys_set_save( void )
{
	volatile int i ;
	int mask ;

	sys_set_init() ;
	mask = get_imask() ;
	set_imask( 14 ) ;
	nlaUpdateUserEEPROM() ;
	i = 20000000 ;
	while( i -- ) ;
	set_imask( mask ) ;
}

/****************************************************************/
/*			Read the settings from the EEPROM; start them		*/
/*			again if they are new, another game's or out of		*/
/*			range												*/
/****************************************************************/
void sys_set_load( void )
{
	int err ;
	int ret ;

	err = 0 ;
	sys_set.id = 0 ;
	ret = nlaInitUserEEPROM( 20, ( char * )&sys_set ) ;
	if( ret == 3 || sys_set.id != BOOK_ID || sys_set.country != sys_flag->Country ) err = 1 ;
	else {
		conf_country = sys_set.country ;
		conf_stereo = sys_set.stereo ;
		conf_demo_sound_half = sys_set.demo_sound_half ;
		conf_cont_enable = sys_set.cont_enable ;
		conf_voice_type = sys_set.voice_type ;
		conf_difficulty = sys_set.difficulty ;
		conf_damage_level = sys_set.damage_level ;
		conf_time_speed = sys_set.time_speed ;
		conf_vms_mode = sys_set.vms_mode ;
		conf_vm_write = sys_set.vm_write ;
		conf_speed_fix = sys_set.speed_fix ;
		conf_event_flag = sys_set.event_flag ;
		conf_no_challenge_flag = sys_set.no_challenge_flag ;
		conf_open_lv = sys_set.open_lv ;
		if( conf_country < 0 || conf_country >= 5 ) err = 1 ;
		if( conf_stereo < 0 || conf_stereo >= 2 ) err = 1 ;
		if( conf_demo_sound_half < 0 || conf_demo_sound_half >= 2 ) err = 1 ;
		if( conf_cont_enable < 0 || conf_cont_enable >= 2 ) err = 1 ;
		if( conf_voice_type < 0 || conf_voice_type >= 2 ) err = 1 ;
		if( conf_difficulty < 0 || conf_difficulty >= 8 ) err = 1 ;
		if( conf_damage_level < 0 || conf_damage_level >= 4 ) err = 1 ;
		if( conf_time_speed < 0 || conf_time_speed >= 4 ) err = 1 ;
		if( conf_vms_mode < 0 || conf_vms_mode >= 3 ) err = 1 ;
		if( conf_vm_write < 0 || conf_vm_write >= 2 ) err = 1 ;
		if( conf_speed_fix < 0 || conf_speed_fix >= 3 ) err = 1 ;
		if( conf_event_flag < 0 || conf_event_flag >= 2 ) err = 1 ;
		if( conf_no_challenge_flag < 0 || conf_no_challenge_flag >= 2 ) err = 1 ;
		if( conf_open_lv < 0 || conf_open_lv >= 5 ) err = 1 ;
	}
	if( err ) sys_set_save() ;
}

/****************************************************************/
/*			The I/O boards' error counts, to watch for a bad	*/
/*			connection											*/
/****************************************************************/
void jamma_err_init( void )
{
	int i ;

	jamma_err_timer = 3600 ;
	for( i = 0 ; i < 8 ; i ++ ){
		jamma_err_cnt1[i] = nlaJAMMAAcknowError[1][i] ;
		if( jamma_cnt == 2 ) jamma_err_cnt2[i] = nlaJAMMAAcknowError[2][i] ;
	}
}

/****************************************************************/
/*			Once a minute after the start, too many new			*/
/*			errors stop the game with a message					*/
/****************************************************************/
void jamma_err_ck( void )
{
	int i ;
	int err ;

	if( !jamma_err_timer ) return ;
	jamma_err_timer -- ;
	if( jamma_err_timer ) return ;
	for( i = 0 ; i < 8 ; i ++ ){
		err = 0 ;
		if( nlaJAMMAAcknowError[1][i] - jamma_err_cnt1[i] > 720 ) err = 1 ;
		if( jamma_cnt == 2 ){
			if( nlaJAMMAAcknowError[2][i] - jamma_err_cnt2[i] > 720 ) err = 1 ;
		}
		if( err ){
			nlSetBackColor( 0, 0, 0 ) ;
			for( ;; ){
				if( test_sw_ck() ) test_mode_jump() ;
				nlSetFontSize( 16, 16 ) ;
				nlPrintColor( 0xffff0000 ) ;
				nlLocate( 18, 10 ) ;
				nlPrintf( "ERROR." ) ;
				nlLocate( 11, 12 ) ;
				nlPrintf( " JAMMA I/O BOARD(S)\n" ) ;
				nlPrintf( "BAD CONNECTION ERROR." ) ;
				nlWaitVSync() ;
			}
		}
	}
}

/****************************************************************/
/*			A side can start: in free play always, which		*/
/*			counts a game, else when it has the credits			*/
/****************************************************************/
Sint8 credit_enough_ck( int side, int cont )
{
	int ret = 0 ;

	if( credit_conf->CoinFlag == NLA_FREEPLAY ){
		ret = 1 ;
		ret <<= side ;
		book_play_add() ;
	} else if( nlaCheckCreditEnough( side, cont ) ) ret = 1 << side ;
	return ret ;
}

/****************************************************************/
/*			The credit display: "CREDIT(S) n" and the coins		*/
/*			towards the next credit, for each side that has		*/
/*			its own credits; nothing in free play				*/
/****************************************************************/
void credit_str_set( void )
{
	char *s ;

	nlaGetCreditData( &credit_info ) ;
	if( credit_conf->CoinFlag != NLA_FREEPLAY ){
		s = credit_str1 ;
		s += sprintf( s, "CREDIT(S) " ) ;
		if( !credit_info.Player[0].Credit ){
			if( credit_info.Player[0].BaseRate != 1 ){
				if( credit_info.Player[0].Remain ) s += sprintf( s, "%1d/%1d", credit_info.Player[0].Remain, credit_info.Player[0].BaseRate ) ;
				else s += sprintf( s, "0/%1d", credit_info.Player[0].BaseRate ) ;
			} else s += sprintf( s, "0" ) ;
		} else {
			s += sprintf( s, "%1d ", credit_info.Player[0].Credit ) ;
			if( credit_info.Player[0].BaseRate != 1 && credit_info.Player[0].Remain ){
				s += sprintf( s, "%1d/%1d", credit_info.Player[0].Remain, credit_info.Player[0].BaseRate ) ;
			}
		}
		*s = 0 ;
		if( !credit_conf->ChuteType ) return ;
		s = credit_str2 ;
		s += sprintf( s, "CREDIT(S) " ) ;
		if( !credit_info.Player[1].Credit ){
			if( credit_info.Player[1].BaseRate != 1 ){
				if( credit_info.Player[1].Remain ) s += sprintf( s, "%1d/%1d", credit_info.Player[1].Remain, credit_info.Player[1].BaseRate ) ;
				else s += sprintf( s, "0/%1d", credit_info.Player[1].BaseRate ) ;
			} else s += sprintf( s, "0" ) ;
		} else {
			s += sprintf( s, "%1d ", credit_info.Player[1].Credit ) ;
			if( credit_info.Player[1].BaseRate != 1 && credit_info.Player[1].Remain ){
				s += sprintf( s, "%1d/%1d", credit_info.Player[1].Remain, credit_info.Player[1].BaseRate ) ;
			}
		}
		*s = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
