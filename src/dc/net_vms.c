/* The network menu's memory card dialogs, and the page that sends the score attack's best to Capcom's
   site: an HTML form with the player's records as hidden inputs, made in upload_buf. */
#include "player.h"
#include "vms.h"
#include "shinobi.h"

extern int sprintf( char *s, const char *fmt, ... ) ;
extern void dm0d_vm_win_set( void ) ;
extern void dm0d_vm_msg_set( void ) ;
extern Uint16 menu_sw_get( Uint8 side_bit ) ;
extern int lr_select_loop() ;
extern int decide_ck( void ) ;
extern int cancel_ck( void ) ;
extern int decide_cancel_ck( void ) ;
extern void d_point_get( void ) ;
extern void n_point_get( void ) ;
extern int vms_card_ck( int port ) ;
extern int vms_card_sys_ck( int port ) ;
extern int vms_check_unit( void ) ;
extern int vms_ports_net_ck( void ) ;
extern int vms_ports_save_ck( void ) ;
extern int vms_date_ck( int port ) ;
extern int vms_net_cnt_up( void ) ;
extern int vms_net_key_ck( void ) ;
extern int vms_data_load( Sint8 type, int port ) ;
extern PLAY_CNT play_cnt ;

/* the upload file's data: its page starts 0x40 bytes in */
Uint8 upload_buf[0x6000] ;
static union {
	Uint32	l[2] ;
	Sint8	b[8] ;
} html_id ;	/* the machine's ID, in its last six bytes */

static char html_hex[16] = "0123456789ABCDEF" ;
static char *html_name_tbl[46] = {	/* the name entry's letters */
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
	"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	".", ",", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"!", "?", "^A", "&", " ", "_", "^C", "^B"
} ;
static char html_input[] = "<INPUT TYPE=\"hidden\" NAME=\"" ;
static char html_input_end[] = "\">\n" ;

/* the page around the inputs; its Japanese is in Shift JIS */
static char html_top_00[] = "<HTML><HEAD>\n" ;
static char html_top_01[] = "<TITLE>\x83t\x83H\x81[\x83\x80\x93\xfc\x97\xcd</TITLE>\n" ;
static char html_top_02[] = "</HEAD>\n" ;
static char html_top_03[] = "<BODY BGCOLOR=\"#016399\" TEXT=\"#FFFFFF\" BACKGROUND=\"file:/dpwww/haikei3.gif\">\n" ;
static char html_top_04[] = "<CENTER>\n" ;
static char html_top_05[] = "<TABLE BORDER=0 WIDTH=1 CELLSPACING=\"0\" CELLPADDING=\"0\"><TR><TD HEIGHT=\"4\"></TD></TR></TABLE>\n" ;
static char html_top_06[] = "<IMG WIDTH=\"372\" HEIGHT=\"40\" SRC=\"file:/dpwww/s_koumoku8.gif\" ALT=\"\x83X\x83R\x83" "A\x83\x89\x83\x93\x83N\x83" "f\x81[\x83^\x82\xcc\x93o\x98^\"><BR><BR>\n" ;
static char html_top_07[] = "<FORM ACTION=\"http://www2.capcom.co.jp/mvsc2/upload/regist.php3\" METHOD=\"POST\">\n" ;
static char html_top_08[] = "\x83" "f\x81[\x83^\x82\xf0\x93\xfc\x97\xcd\x8c\xe3\x91\x97\x90M\x83{\x83^\x83\x93\x82\xf0\x89\x9f\x82\xb5\x82\xc4\x89\xba\x82\xb3\x82\xa2\x81" "B\n" ;
static char html_top_09[] = "<TABLE BORDER=0 CELLSPACING=\"2\" CELLPADDING=\"1\"><TR VALIGN=top><TD ALIGN=right>\x93o\x98^\x96\xbc\x81" "F</TD>\n" ;
static char html_top_10[] = "<TD COLSPAN=3><INPUT NAME=\"FORM00\" TYPE=Text SIZE=\"16\" MAXLENGTH=\"16\"><BR><FONT SIZE=1 COLOR=\"#FFFF00\">\x81\xa6\x94\xbc\x8ap16\x95\xb6\x8e\x9a(\x91S\x8ap8\x95\xb6\x8e\x9a)\x82\xdc\x82\xc5\x93\xfc\x97\xcd\x82\xc5\x82\xab\x82\xdc\x82\xb7\x81" "B</FONT></TD>\n" ;
static char html_top_11[] = "</TR>\n" ;
static char html_top_12[] = "<TR VALIGN=Top><TD ALIGN=right nowrap>\x94N\x81@\x97\xdf\x81" "F</TD>\n" ;
static char html_top_13[] = "<TD><SELECT NAME=\"FORM01\" SIZE=1>\n" ;
static char html_top_14[] = "<OPTION VALUE=\"0\">--\x91I\x91\xf0--\n" ;
static char html_top_15[] = "<OPTION VALUE=\"1\">10\x8d\xce\x96\xa2\x96\x9e\n" ;
static char html_top_16[] = "<OPTION VALUE=\"2\">10\x81`14\x8d\xce\n" ;
static char html_top_17[] = "<OPTION VALUE=\"3\">15\x81`19\x8d\xce\n" ;
static char html_top_18[] = "<OPTION VALUE=\"4\">20\x81`24\x8d\xce\n" ;
static char html_top_19[] = "<OPTION VALUE=\"5\">25\x81`29\x8d\xce\n" ;
static char html_top_20[] = "<OPTION VALUE=\"6\">30\x81`39\x8d\xce\n" ;
static char html_top_21[] = "<OPTION VALUE=\"7\">40\x81`49\x8d\xce\n" ;
static char html_top_22[] = "<OPTION VALUE=\"8\">50\x81`59\x8d\xce\n" ;
static char html_top_23[] = "<OPTION VALUE=\"9\">60\x8d\xce\x88\xc8\x8f\xe3\n" ;
static char html_top_24[] = "</SELECT></TD>\n" ;
static char html_top_25[] = "<TD ALIGN=right nowrap>\x90\xab\x81@\x95\xca\x81" "F</TD><TD><SELECT NAME=\"FORM02\" SIZE=1>\n" ;
static char html_top_26[] = "<OPTION VALUE=\"0\">--\x91I\x91\xf0--\n" ;
static char html_top_27[] = "<OPTION VALUE=\"1\">\x92j\n" ;
static char html_top_28[] = "<OPTION VALUE=\"2\">\x8f\x97\n" ;
static char html_top_29[] = "</SELECT>\n" ;
static char html_top_30[] = "</TD></TR>\n" ;
static char html_top_31[] = "<TR VALIGN=Top><TD ALIGN=right nowrap>\x8f\x8a\x8d\xdd\x92n\x81" "F</TD>\n" ;
static char html_top_32[] = "<TD COLSPAN=3><SELECT NAME=\"FORM03\" SIZE=\"1\">\n" ;
static char html_top_33[] = "<OPTION VALUE=\"0\">--\x91I\x91\xf0--\n" ;
static char html_top_34[] = "<OPTION VALUE=\"1\">\x96k\x8a" "C\x93\xb9\n" ;
static char html_top_35[] = "<OPTION VALUE=\"2\">\x90\xc2\x90X\x8c\xa7\n" ;
static char html_top_36[] = "<OPTION VALUE=\"3\">\x8a\xe2\x8e\xe8\x8c\xa7\n" ;
static char html_top_37[] = "<OPTION VALUE=\"4\">\x8b{\x8f\xe9\x8c\xa7\n" ;
static char html_top_38[] = "<OPTION VALUE=\"5\">\x8fH\x93" "c\x8c\xa7\n" ;
static char html_top_39[] = "<OPTION VALUE=\"6\">\x8eR\x8c`\x8c\xa7\n" ;
static char html_top_40[] = "<OPTION VALUE=\"7\">\x95\x9f\x93\x87\x8c\xa7\n" ;
static char html_top_41[] = "<OPTION VALUE=\"8\">\x88\xef\x8f\xe9\x8c\xa7\n" ;
static char html_top_42[] = "<OPTION VALUE=\"9\">\x93\xc8\x96\xd8\x8c\xa7\n" ;
static char html_top_43[] = "<OPTION VALUE=\"10\">\x8cQ\x94n\x8c\xa7\n" ;
static char html_top_44[] = "<OPTION VALUE=\"11\">\x8d\xe9\x8b\xca\x8c\xa7\n" ;
static char html_top_45[] = "<OPTION VALUE=\"12\">\x90\xe7\x97t\x8c\xa7\n" ;
static char html_top_46[] = "<OPTION VALUE=\"13\">\x93\x8c\x8b\x9e\x93s\n" ;
static char html_top_47[] = "<OPTION VALUE=\"14\">\x90_\x93\xde\x90\xec\x8c\xa7\n" ;
static char html_top_48[] = "<OPTION VALUE=\"15\">\x8eR\x97\x9c\x8c\xa7\n" ;
static char html_top_49[] = "<OPTION VALUE=\"16\">\x92\xb7\x96\xec\x8c\xa7\n" ;
static char html_top_50[] = "<OPTION VALUE=\"17\">\x90V\x8a\x83\x8c\xa7\n" ;
static char html_top_51[] = "<OPTION VALUE=\"18\">\x95x\x8eR\x8c\xa7\n" ;
static char html_top_52[] = "<OPTION VALUE=\"19\">\x90\xce\x90\xec\x8c\xa7\n" ;
static char html_top_53[] = "<OPTION VALUE=\"20\">\x95\x9f\x88\xe4\x8c\xa7\n" ;
static char html_top_54[] = "<OPTION VALUE=\"21\">\x8a\xf2\x95\x8c\x8c\xa7\n" ;
static char html_top_55[] = "<OPTION VALUE=\"22\">\x90\xc3\x89\xaa\x8c\xa7\n" ;
static char html_top_56[] = "<OPTION VALUE=\"23\">\x88\xa4\x92m\x8c\xa7\n" ;
static char html_top_57[] = "<OPTION VALUE=\"24\">\x8eO\x8f" "d\x8c\xa7\n" ;
static char html_top_58[] = "<OPTION VALUE=\"25\">\x8e\xa0\x89\xea\x8c\xa7\n" ;
static char html_top_59[] = "<OPTION VALUE=\"26\">\x8b\x9e\x93s\x95{\n" ;
static char html_top_60[] = "<OPTION VALUE=\"27\">\x91\xe5\x8d\xe3\x95{\n" ;
static char html_top_61[] = "<OPTION VALUE=\"28\">\x95\xba\x8c\xc9\x8c\xa7\n" ;
static char html_top_62[] = "<OPTION VALUE=\"29\">\x93\xde\x97\xc7\x8c\xa7\n" ;
static char html_top_63[] = "<OPTION VALUE=\"30\">\x98" "a\x89\xcc\x8eR\x8c\xa7\n" ;
static char html_top_64[] = "<OPTION VALUE=\"31\">\x92\xb9\x8e\xe6\x8c\xa7\n" ;
static char html_top_65[] = "<OPTION VALUE=\"32\">\x93\x87\x8d\xaa\x8c\xa7\n" ;
static char html_top_66[] = "<OPTION VALUE=\"33\">\x89\xaa\x8eR\x8c\xa7\n" ;
static char html_top_67[] = "<OPTION VALUE=\"34\">\x8dL\x93\x87\x8c\xa7\n" ;
static char html_top_68[] = "<OPTION VALUE=\"35\">\x8eR\x8c\xfb\x8c\xa7\n" ;
static char html_top_69[] = "<OPTION VALUE=\"36\">\x93\xbf\x93\x87\x8c\xa7\n" ;
static char html_top_70[] = "<OPTION VALUE=\"37\">\x8d\x81\x90\xec\x8c\xa7\n" ;
static char html_top_71[] = "<OPTION VALUE=\"38\">\x88\xa4\x95Q\x8c\xa7\n" ;
static char html_top_72[] = "<OPTION VALUE=\"39\">\x8d\x82\x92m\x8c\xa7\n" ;
static char html_top_73[] = "<OPTION VALUE=\"40\">\x95\x9f\x89\xaa\x8c\xa7\n" ;
static char html_top_74[] = "<OPTION VALUE=\"41\">\x8d\xb2\x89\xea\x8c\xa7\n" ;
static char html_top_75[] = "<OPTION VALUE=\"42\">\x92\xb7\x8d\xe8\x8c\xa7\n" ;
static char html_top_76[] = "<OPTION VALUE=\"43\">\x8c" "F\x96{\x8c\xa7\n" ;
static char html_top_77[] = "<OPTION VALUE=\"44\">\x91\xe5\x95\xaa\x8c\xa7\n" ;
static char html_top_78[] = "<OPTION VALUE=\"45\">\x8b{\x8d\xe8\x8c\xa7\n" ;
static char html_top_79[] = "<OPTION VALUE=\"46\">\x8e\xad\x8e\x99\x93\x87\x8c\xa7\n" ;
static char html_top_80[] = "<OPTION VALUE=\"47\">\x89\xab\x93\xea\x8c\xa7\n" ;
static char html_top_81[] = "</SELECT>\n" ;
static char html_top_82[] = "</TD></TR>\n" ;
static char html_top_83[] = "<TR VALIGN=Top><TD ALIGN=right NOWRAP>\x83R\x83\x81\x83\x93\x83g\x81" "F</TD>\n" ;
static char html_top_84[] = "<TD COLSPAN=3><INPUT NAME=\"FORM04\" SIZE=40 MAXLENGTH=\"60\"><BR>\n" ;
static char html_top_85[] = "<FONT SIZE=1 COLOR=\"#FFFF00\">\x81\xa6\x94\xbc\x8ap60\x95\xb6\x8e\x9a(\x91S\x8ap30\x95\xb6\x8e\x9a)\x82\xdc\x82\xc5\x93\xfc\x97\xcd\x82\xc5\x82\xab\x82\xdc\x82\xb7\x81" "B\n" ;
static char html_top_86[] = "</FONT></TD></TR></TABLE>\n" ;
static char *html_top[87] = {
	html_top_00, html_top_01, html_top_02, html_top_03, html_top_04, html_top_05,
	html_top_06, html_top_07, html_top_08, html_top_09, html_top_10, html_top_11,
	html_top_12, html_top_13, html_top_14, html_top_15, html_top_16, html_top_17,
	html_top_18, html_top_19, html_top_20, html_top_21, html_top_22, html_top_23,
	html_top_24, html_top_25, html_top_26, html_top_27, html_top_28, html_top_29,
	html_top_30, html_top_31, html_top_32, html_top_33, html_top_34, html_top_35,
	html_top_36, html_top_37, html_top_38, html_top_39, html_top_40, html_top_41,
	html_top_42, html_top_43, html_top_44, html_top_45, html_top_46, html_top_47,
	html_top_48, html_top_49, html_top_50, html_top_51, html_top_52, html_top_53,
	html_top_54, html_top_55, html_top_56, html_top_57, html_top_58, html_top_59,
	html_top_60, html_top_61, html_top_62, html_top_63, html_top_64, html_top_65,
	html_top_66, html_top_67, html_top_68, html_top_69, html_top_70, html_top_71,
	html_top_72, html_top_73, html_top_74, html_top_75, html_top_76, html_top_77,
	html_top_78, html_top_79, html_top_80, html_top_81, html_top_82, html_top_83,
	html_top_84, html_top_85, html_top_86
} ;
static char html_end_00[] = "<BR>\n" ;
static char html_end_01[] = "<IMG WIDTH=\"389\" HEIGHT=\"28\" SRC=\"file:/dpwww/s_koumoku11.gif\" ALT=\"\x83X\x83R\x83" "A\x83" "A\x83^\x83" "b\x83N\x83\x89\x83\x93\x83N\x82\xc9\x93o\x98^\x82\xb5\x82\xdc\x82\xb7\"><BR>\n" ;
static char html_end_02[] = "<BR>\n" ;
static char html_end_03[] = "<INPUT NAME=\"FORM10\" TYPE=Submit VALUE=\" \x91\x97\x90M \">\x81@\n" ;
static char html_end_04[] = "<INPUT NAME=\"FORM99\" TYPE=Reset VALUE=\" \x8c\xb3\x82\xc9\x96\xdf\x82\xb7 \">\n" ;
static char html_end_05[] = "</FORM>\n" ;
static char html_end_06[] = "</CENTER></BODY>\n" ;
static char html_end_07[] = "</HTML>\n" ;
static char *html_end[8] = {
	html_end_00, html_end_01, html_end_02, html_end_03, html_end_04, html_end_05,
	html_end_06, html_end_07
} ;


char *html_sa_put( char *p ) ;
char *html_cnt_put( char *p ) ;
Uint32 bin2bcd( Uint32 n ) ;
char *html_hex8_put( char *p, Uint32 n, int keta ) ;
char *html_hex_put( char *p, Uint32 n ) ;
char *html_hidden_hex( char *p, char *name, Uint32 n ) ;
char *html_hidden_dec( char *p, char *name, Uint32 n ) ;
char *html_hidden_name( char *p, char *name, Uint8 *s ) ;
char *html_hidden_str( char *p, char *name, Uint8 *bcd ) ;

/****************************************************************/
/*			The upload page										*/
/****************************************************************/
int upload_html_make( void )
{
	char *p ;
	int i, ret ;

	p = ( char * )&upload_buf[0x40] ;
	ret = syCfgGetIndividualID( &html_id.b[2] ) ;
	if( !ret ) return 0 ;
	for( i = 0 ; i < 0x6000 ; i ++ ) upload_buf[i] = 0 ;
	for( i = 0 ; i < 87 ; i ++ ) p += sprintf( p, "%s", html_top[i] ) ;
	p = html_sa_put( p ) ;
	p = html_cnt_put( p ) ;
	for( i = 0 ; i < 8 ; i ++ ) p += sprintf( p, "%s", html_end[i] ) ;
	return 1 ;
}

/* the score attack's best: its team, name, score, wins, ending and time */
char *html_sa_put( char *p )
{
	RANK_DATA *rk ;
	char name[8] ;
	Uint32 t ;

	name[6] = 0 ;
	name[7] = 0 ;
	rk = &rank_sa[0] ;
	sprintf( name, "SA%dPL0", 0 ) ;
	p = html_hidden_dec( p, name, rk->load_type[0] ) ;
	sprintf( name, "SA%dPL1", 0 ) ;
	p = html_hidden_dec( p, name, rk->load_type[1] ) ;
	sprintf( name, "SA%dPL2", 0 ) ;
	p = html_hidden_dec( p, name, rk->load_type[2] ) ;
	sprintf( name, "SA%dAS0", 0 ) ;
	p = html_hidden_hex( p, name, rk->pl_personal[0] ) ;
	sprintf( name, "SA%dAS1", 0 ) ;
	p = html_hidden_hex( p, name, rk->pl_personal[1] ) ;
	sprintf( name, "SA%dAS2", 0 ) ;
	p = html_hidden_hex( p, name, rk->pl_personal[2] ) ;
	sprintf( name, "SA%dNAM", 0 ) ;
	p = html_hidden_name( p, name, rk->name ) ;
	sprintf( name, "SA%dSCO", 0 ) ;
	p = html_hidden_str( p, name, ( Uint8 * )&rk->score ) ;
	sprintf( name, "SA%dWIN", 0 ) ;
	p = html_hidden_dec( p, name, rk->win_max ) ;
	sprintf( name, "SA%dEND", 0 ) ;
	p = html_hidden_hex( p, name, rk->ending_flag ) ;
	t = ( rk->play_min * 60 + rk->play_sec ) * 100 + rk->play_csec ;
	sprintf( name, "SA%dTIM", 0 ) ;
	return html_hidden_dec( p, name, t ) ;
}

/* the time played, how often each mode and character was played, and what is open */
char *html_cnt_put( char *p )
{
	SECRET_WORK *sw ;
	char name[8] ;
	char *q ;
	int i, n ;
	Uint32 flag ;
	PLAY_CNT *pc ;

	sw = &CS_Secret_Work ;
	pc = &play_cnt ;
	n = ( sw->play_hour * 60 + sw->play_min ) * 60 + sw->play_sec ;
	p = html_hidden_dec( p, "ONTIME", n ) ;
	p = html_hidden_dec( p, "AC_MODE_CNT", pc->ac_mode_cnt ) ;
	p = html_hidden_dec( p, "VS_MODE_CNT", pc->vs_mode_cnt ) ;
	p = html_hidden_dec( p, "SA_MODE_CNT", pc->sa_mode_cnt ) ;
	p = html_hidden_dec( p, "TR_MODE_CNT", pc->tr_mode_cnt ) ;
	p = html_hidden_dec( p, "NE_MODE_CNT", pc->ne_mode_cnt ) ;
	name[7] = 0 ;
	for( n = 0 ; n < PL_TYPE_MAX ; n ++ ){
		sprintf( name, "AC_PL%02d", n ) ;
		p = html_hidden_dec( p, name, pc->ac_pl[n] ) ;
	}
	for( n = 0 ; n < PL_TYPE_MAX ; n ++ ){
		sprintf( name, "VS_PL%02d", n ) ;
		p = html_hidden_dec( p, name, pc->vs_pl[n] ) ;
	}
	for( n = 0 ; n < PL_TYPE_MAX ; n ++ ){
		sprintf( name, "SA_PL%02d", n ) ;
		p = html_hidden_dec( p, name, pc->sa_pl[n] ) ;
	}
	for( n = 0 ; n < PL_TYPE_MAX ; n ++ ){
		sprintf( name, "TR_PL%02d", n ) ;
		p = html_hidden_dec( p, name, pc->tr_pl[n] ) ;
	}
	for( n = 0 ; n < PL_TYPE_MAX ; n ++ ){
		sprintf( name, "NE_PL%02d", n ) ;
		p = html_hidden_dec( p, name, pc->ne_pl[n] ) ;
	}
	q = html_hidden_dec( p, "N_LINK", sw->unk_08 ) ;
	name[7] = 0 ;
	for( i = 0 ; i < PL_TYPE_MAX ; i ++ ){
		sprintf( name, "KA_PL%02d", i ) ;
		if( i < 32 ){
			if( Sys->char_open0 & 1 << i ) flag = 1 ;
			else flag = 0 ;
			q = html_hidden_dec( q, name, flag ) ;
		} else {
			if( Sys->char_open1 & 1 << ( i - 32 ) ) flag = 1 ;
			else flag = 0 ;
			q = html_hidden_dec( q, name, flag ) ;
		}
	}
	for( i = 0 ; i < PL_TYPE_MAX ; i ++ ){
		sprintf( name, "KA_CO%02d", i ) ;
		if( i < 32 ){
			if( Sys->char_open_all0 & 1 << i ) flag = 1 ;
			else flag = 0 ;
			q = html_hidden_dec( q, name, flag ) ;
		} else {
			if( Sys->char_open_all1 & 1 << ( i - 32 ) ) flag = 1 ;
			else flag = 0 ;
			q = html_hidden_dec( q, name, flag ) ;
		}
	}
	p = html_hidden_dec( q, "KA_STG", vms_work.stage_ex_cnt ) ;
	p += sprintf( p, "%s", html_input ) ;
	p += sprintf( p, "ID\" VALUE=\"" ) ;
	p = html_hex8_put( p, html_id.l[1], 8 ) ;
	p = html_hex8_put( p, html_id.l[0], 8 ) ;
	p -= 4 ;
	p += sprintf( p, "%s", html_input_end ) ;
	return p ;
}

/****************************************************************/
/*			Numbers and hidden inputs							*/
/****************************************************************/
/* eight decimal digits, 99999999 at most */
Uint32 bin2bcd( Uint32 n )
{
	Uint32 bcd, d ;

	d = 10000000 ;
	bcd = 0 ;
	if( n > 99999999 ) n = 99999999 ;
	do {
		bcd += n / d ;
		n = n % d ;
		bcd <<= 4 ;
		d /= 10 ;
	} while( d != 1 ) ;
	bcd += n ;
	return bcd ;
}

char *html_hex8_put( char *p, Uint32 n, int keta )
{
	char buf[12] ;
	Uint32 c ;
	int i ;

	buf[8] = '\0' ;
	for( i = 0 ; i < 8 ; i ++ ){
		c = n >> 28 ;
		buf[i] = html_hex[c] ;
		n <<= 4 ;
	}
	p += sprintf( p, "%s", buf ) ;
	return p ;
}

/* without its leading zeros */
char *html_hex_put( char *p, Uint32 n )
{
	char buf[12] ;
	char *q ;
	int keta ;
	Uint32 c ;

	keta = 8 ;
	q = buf ;
	if( !n ){
		p += sprintf( p, "0" ) ;
	} else {
		for( ;; ){
			if( n & 0xf0000000 ) break ;
			keta -- ;
			n <<= 4 ;
		}
		while( keta -- ){
			c = n >> 28 ;
			*q = html_hex[c] ;
			q ++ ;
			n <<= 4 ;
		}
		*q = '\0' ;
		p += sprintf( p, "%s", buf ) ;
	}
	return p ;
}

char *html_hidden_hex( char *p, char *name, Uint32 n )
{
	char *fmt = "%s\" VALUE=\"0x" ;	/* not used */

	p += sprintf( p, "%s", html_input ) ;
	p += sprintf( p, "%s\" VALUE=\"", name ) ;
	p = html_hex_put( p, n ) ;
	p += sprintf( p, "%s", html_input_end ) ;
	return p ;
}

char *html_hidden_dec( char *p, char *name, Uint32 n )
{
	n = bin2bcd( n ) ;
	p += sprintf( p, "%s", html_input ) ;
	p += sprintf( p, "%s\" VALUE=\"", name ) ;
	p = html_hex_put( p, n ) ;
	p += sprintf( p, "%s", html_input_end ) ;
	return p ;
}

/* a name of three letters */
char *html_hidden_name( char *p, char *name, Uint8 *s )
{
	char buf[4] ;
	int i ;

	buf[3] = '\0' ;
	for( i = 0 ; i < 3 ; i ++ ) buf[i] = *html_name_tbl[*s ++] ;
	p += sprintf( p, "%s", html_input ) ;
	p += sprintf( p, "%s\" VALUE=\"", name ) ;
	p += sprintf( p, "%s", buf ) ;
	p += sprintf( p, "%s", html_input_end ) ;
	return p ;
}

/* a score of sixteen BCD digits, low byte first, without its leading zeros */
char *html_hidden_str( char *p, char *name, Uint8 *bcd )
{
	char str[20], num[20] ;
	int i, n, f ;

	str[16] = '\0' ;
	num[16] = '\0' ;
	for( i = 14 ; i >= 0 ; i -= 2 ){
		num[i + 1] = *bcd & 15 ;
		num[i] = ( *bcd ++ & 0xf0 ) >> 4 ;
	}
	for( i = 15 ; i >= 0 ; i -- ) str[i] = html_hex[num[i]] ;
	n = 0 ;
	f = 0 ;
	for( i = 0 ; i < 16 ; i ++ ){
		if( !f ){
			if( str[i] != '0' ){
				f = 1 ;
				num[n] = str[i] ;
				n ++ ;
			} else {	/* empty: the ROM's code has this arm */
			}
		} else {
			num[n] = str[i] ;
			n ++ ;
		}
	}
	num[n] = '\0' ;
	p += sprintf( p, "%s", html_input ) ;
	p += sprintf( p, "%s\" VALUE=\"", name ) ;
	p += sprintf( p, "%s", &num ) ;
	p += sprintf( p, "%s", html_input_end ) ;
	return p ;
}

/****************************************************************/
/*			The card dialogs									*/
/****************************************************************/
/* Each kind runs in card_task_step[1]: 0 waits for the fade, 1 opens the window, 2 checks the
   card, 3 loads the network file, 4 saves, 5 shows a message and asks again, 6 and 7 fade out.
   It returns 2 while busy, 0 when cancelled, and 1 when done; card_task_timer[0] is then the
   error, 0 for none. */

/* kind 0: a new entry; the network file is loaded, or made from the system file, and the
   network's counts are cleared */
int net_vms_00( Uint16 sw )
{
	int ret ;
	int i ;

	if( vms_work.card_task_step[1][0] == 5 ){
		if( vms_work.card_bit == -1
		 || ( vms_work.card_bit & ( 1 << vms_work.file_port ) ) != ( vms_work.card_bit_keep & ( 1 << vms_work.file_port ) ) ){
			vms_work.card_task_step[1][0] = 2 ;
			vms_work.card_task_step[1][1] = 0 ;
		}
	}
	switch( vms_work.card_task_step[1][0] ){
	case 0 :
		if( fade_cnt() ) break ;
		vms_work.card_bit = vms_work.card_bit_keep = -1 ;
		vms_work.card_task_step[1][0] ++ ;
		vms_work.card_task_timer[0] = 0 ;
		work_init() ;
		vms_work.vm_msg[0] = 7 ;
		vms_work.net_cnt_flag = 0 ;
		CS_Secret_Work.unk_0a = 0 ;
		CS_Secret_Work.unk_0c = 0 ;
		CS_Secret_Work.unk_10 = 0 ;
		for( i = 0 ; i < 14 ; i ++ ) unk_8C21385C[i] = 0 ;
		break ;
	case 1 :
		vms_work.card_task_step[1][0] ++ ;
		dm0d_vm_win_set() ;
		break ;
	case 2 :
		if( vms_work.card_bit == -1 ) break ;
		vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.file_flag != 1 || !vms_card_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_step[1][1] = 1 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 3 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_card_sys_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_date_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else {
			ret = vms_ports_net_ck() ;
			if( !( ret & ( 1 << vms_work.file_port ) ) ){
				ret = vms_ports_save_ck() ;
				if( !( ret & ( 1 << vms_work.file_port ) ) ){
					vms_work.card_task_step[1][0] = 5 ;
					vms_work.card_task_step[1][1] = 1 ;
					vms_work.card_task_timer[0] = 1 ;
					vms_work.vm_msg[0] = 3 ;
					dm0d_vm_msg_set() ;
					fade_set( 0xff000000, 1, FADE_IN ) ;
				} else {
					vms_work.card_task_step[1][0] = 4 ;
				}
			} else {
				vms_work.card_task_step[1][0] = 3 ;
			}
		}
		break ;
	case 3 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( ( ret = vms_data_load( 4, vms_work.file_port ) ) != -1 ){
				if( !ret ){
					vms_work.card_task_step[1][1] ++ ;
				} else {
					vms_work.card_task_step[1][0] = 5 ;
					vms_work.card_task_step[1][1] = 1 ;
					vms_work.card_task_timer[0] = 1 ;
					vms_work.vm_msg[0] = 2 ;
					dm0d_vm_msg_set() ;
					fade_set( 0xff000000, 1, FADE_IN ) ;
				}
			}
			break ;
		case 1 :
			if( !vms_net_key_ck() ){
				vms_work.card_task_step[1][0] = 4 ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 0 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
			break ;
		}
		break ;
	case 4 :
		if( ( ret = vms_task_save_all( vms_work.file_port, 1 ) ) != 2 ){
			if( ret == 1 ){
				vms_work.card_task_step[1][0] = 6 ;
				vms_date_keep( vms_work.file_port ) ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 1 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 2 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
		}
		break ;
	case 5 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 3 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 5 ;
				vms_work.vm_msg[1] = 1 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 2 :
			vms_work.vm_msg[1] = lr_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) vms_work.card_task_step[1][1] = 4 ;
				else vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 3 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][0] = 2 ;
				vms_work.card_task_step[1][1] = 0 ;
			}
			break ;
		case 4 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][1] ++ ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 5 :
			if( !fade_cnt() ) return 0 ;
			break ;
		}
		break ;
	case 6 :
		if( !vms_work.card_task_timer[0] ) return 1 ;
		vms_work.card_task_step[1][0] ++ ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 7 :
		if( !fade_cnt() ) return 1 ;
		break ;
	}
	return 2 ;
}

/* kinds 1 and 2: the network file is loaded and its count goes up */
int net_vms_01( Uint16 sw )
{
	int ret ;

	if( vms_work.card_task_step[1][0] == 5 ){
		if( vms_work.card_bit == -1
		 || ( vms_work.card_bit & ( 1 << vms_work.file_port ) ) != ( vms_work.card_bit_keep & ( 1 << vms_work.file_port ) ) ){
			vms_work.card_task_step[1][0] = 2 ;
			vms_work.card_task_step[1][1] = 0 ;
		}
	}
	switch( vms_work.card_task_step[1][0] ){
	case 0 :
		if( fade_cnt() ) break ;
		vms_work.card_bit = vms_work.card_bit_keep = -1 ;
		vms_work.card_task_step[1][0] ++ ;
		vms_work.card_task_timer[0] = 0 ;
		work_init() ;
		vms_work.vm_msg[0] = 7 ;
		break ;
	case 1 :
		vms_work.card_task_step[1][0] ++ ;
		dm0d_vm_win_set() ;
		break ;
	case 2 :
		if( vms_work.card_bit == -1 ) break ;
		vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.file_flag != 1 || !vms_card_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_step[1][1] = 1 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 3 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_card_sys_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_date_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else {
			ret = vms_ports_net_ck() ;
			if( !( ret & ( 1 << vms_work.file_port ) ) ){
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			} else {
				vms_work.card_task_step[1][0] = 3 ;
			}
		}
		break ;
	case 3 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( ( ret = vms_data_load( 4, vms_work.file_port ) ) != -1 ){
				if( !ret ){
					vms_work.card_task_step[1][1] ++ ;
				} else {
					vms_work.card_task_step[1][0] = 5 ;
					vms_work.card_task_step[1][1] = 1 ;
					vms_work.card_task_timer[0] = 1 ;
					vms_work.vm_msg[0] = 1 ;
					dm0d_vm_msg_set() ;
					fade_set( 0xff000000, 1, FADE_IN ) ;
				}
			}
			break ;
		case 1 :
			if( !vms_net_key_ck() ){
				vms_work.card_task_step[1][0] = 4 ;
				vms_net_cnt_up() ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 0 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
			break ;
		}
		break ;
	case 4 :
		if( ( ret = vms_task_save_all( vms_work.file_port, 1 ) ) != 2 ){
			if( ret == 1 ){
				vms_work.card_task_step[1][0] = 6 ;
				vms_date_keep( vms_work.file_port ) ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 1 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 2 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
		}
		break ;
	case 5 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 3 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 5 ;
				vms_work.vm_msg[1] = 1 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 2 :
			vms_work.vm_msg[1] = lr_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) vms_work.card_task_step[1][1] = 4 ;
				else vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 3 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][0] = 2 ;
				vms_work.card_task_step[1][1] = 0 ;
			}
			break ;
		case 4 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][1] ++ ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 5 :
			if( !fade_cnt() ) return 0 ;
		}
		break ;
	case 6 :
		if( !vms_work.card_task_timer[0] ) return 1 ;
		vms_work.card_task_step[1][0] ++ ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 7 :
		if( !fade_cnt() ) return 1 ;
		break ;
	}
	return 2 ;
}

/* kinds 3 and 4: the network file is loaded */
int net_vms_03( Uint16 sw )
{
	int ret ;

	if( vms_work.card_task_step[1][0] == 5 ){
		if( vms_work.card_bit == -1
		 || ( vms_work.card_bit & ( 1 << vms_work.file_port ) ) != ( vms_work.card_bit_keep & ( 1 << vms_work.file_port ) ) ){
			vms_work.card_task_step[1][0] = 2 ;
			vms_work.card_task_step[1][1] = 0 ;
		}
	}
	switch( vms_work.card_task_step[1][0] ){
	case 0 :
		if( fade_cnt() ) break ;
		vms_work.card_bit = vms_work.card_bit_keep = -1 ;
		vms_work.card_task_step[1][0] ++ ;
		vms_work.card_task_timer[0] = 0 ;
		work_init() ;
		vms_work.vm_msg[0] = 7 ;
		break ;
	case 1 :
		vms_work.card_task_step[1][0] ++ ;
		dm0d_vm_win_set() ;
		break ;
	case 2 :
		if( vms_work.card_bit == -1 ) break ;
		vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.file_flag != 1 || !vms_card_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_step[1][1] = 1 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 3 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_card_sys_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_date_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else {
			ret = vms_ports_net_ck() ;
			if( !( ret & ( 1 << vms_work.file_port ) ) ){
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			} else {
				vms_work.card_task_step[1][0] = 3 ;
			}
		}
		break ;
	case 3 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( ( ret = vms_data_load( 4, vms_work.file_port ) ) != -1 ){
				if( !ret ){
					vms_work.card_task_step[1][1] ++ ;
				} else {
					vms_work.card_task_step[1][0] = 5 ;
					vms_work.card_task_step[1][1] = 1 ;
					vms_work.card_task_timer[0] = 1 ;
					vms_work.vm_msg[0] = 1 ;
					dm0d_vm_msg_set() ;
					fade_set( 0xff000000, 1, FADE_IN ) ;
				}
			}
			break ;
		case 1 :
			if( !vms_net_key_ck() ){
				vms_work.card_task_step[1][0] = 4 ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 0 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
			break ;
		}
		break ;
	case 4 :
		if( ( ret = vms_task_save_all( vms_work.file_port, 1 ) ) != 2 ){
			if( ret == 1 ){
				vms_work.card_task_step[1][0] = 6 ;
				vms_date_keep( vms_work.file_port ) ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 1 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 2 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
		}
		break ;
	case 5 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 3 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 5 ;
				vms_work.vm_msg[1] = 1 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 2 :
			vms_work.vm_msg[1] = lr_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) vms_work.card_task_step[1][1] = 4 ;
				else vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 3 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][0] = 2 ;
				vms_work.card_task_step[1][1] = 0 ;
			}
			break ;
		case 4 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][1] ++ ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 5 :
			if( !fade_cnt() ) return 0 ;
		}
		break ;
	case 6 :
		if( !vms_work.card_task_timer[0] ) return 1 ;
		vms_work.card_task_step[1][0] ++ ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 7 :
		if( !fade_cnt() ) return 1 ;
		break ;
	}
	return 2 ;
}

/* kind 6: the points earned are taken, then the network file is loaded */
int net_vms_06( Uint16 sw )
{
	int ret ;

	if( vms_work.card_task_step[1][0] == 5 ){
		if( vms_work.card_bit == -1
		 || ( vms_work.card_bit & ( 1 << vms_work.file_port ) ) != ( vms_work.card_bit_keep & ( 1 << vms_work.file_port ) ) ){
			vms_work.card_task_step[1][0] = 2 ;
			vms_work.card_task_step[1][1] = 0 ;
		}
	}
	switch( vms_work.card_task_step[1][0] ){
	case 0 :
		if( fade_cnt() ) break ;
		vms_work.card_bit = vms_work.card_bit_keep = -1 ;
		vms_work.card_task_step[1][0] ++ ;
		vms_work.card_task_timer[0] = 0 ;
		work_init() ;
		vms_work.vm_msg[0] = 7 ;
		if( vms_work.d_point ) d_point_get() ;
		if( vms_work.n_point ) n_point_get() ;
		break ;
	case 1 :
		vms_work.card_task_step[1][0] ++ ;
		dm0d_vm_win_set() ;
		break ;
	case 2 :
		if( vms_work.card_bit == -1 ) break ;
		vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.file_flag != 1 || !vms_card_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_step[1][1] = 1 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 3 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_card_sys_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_date_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else {
			ret = vms_ports_net_ck() ;
			if( !( ret & ( 1 << vms_work.file_port ) ) ){
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			} else {
				vms_work.card_task_step[1][0] = 3 ;
			}
		}
		break ;
	case 3 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( ( ret = vms_data_load( 4, vms_work.file_port ) ) != -1 ){
				if( !ret ){
					vms_work.card_task_step[1][1] ++ ;
				} else {
					vms_work.card_task_step[1][0] = 5 ;
					vms_work.card_task_step[1][1] = 1 ;
					vms_work.card_task_timer[0] = 1 ;
					vms_work.vm_msg[0] = 1 ;
					dm0d_vm_msg_set() ;
					fade_set( 0xff000000, 1, FADE_IN ) ;
				}
			}
			break ;
		case 1 :
			if( !vms_net_key_ck() ){
				vms_work.card_task_step[1][0] = 4 ;
				vms_work.net_cnt_flag = 0 ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 0 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
			break ;
		}
		break ;
	case 4 :
		if( ( ret = vms_task_save_all( vms_work.file_port, 1 ) ) != 2 ){
			if( ret == 1 ){
				vms_work.card_task_step[1][0] = 6 ;
				vms_date_keep( vms_work.file_port ) ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 1 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 2 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
		}
		break ;
	case 5 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 3 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 5 ;
				vms_work.vm_msg[1] = 1 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 2 :
			vms_work.vm_msg[1] = lr_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) vms_work.card_task_step[1][1] = 4 ;
				else vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 3 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][0] = 2 ;
				vms_work.card_task_step[1][1] = 0 ;
			}
			break ;
		case 4 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][1] ++ ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 5 :
			if( !fade_cnt() ) return 0 ;
		}
		break ;
	case 6 :
		if( !vms_work.card_task_timer[0] ) return 1 ;
		vms_work.card_task_step[1][0] ++ ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 7 :
		if( !fade_cnt() ) return 1 ;
		break ;
	}
	return 2 ;
}

/* kind 5: the network file is loaded and marked; card_task_timer[0] says which check failed */
int net_vms_05( Uint16 sw )
{
	int ret ;

	if( vms_work.card_task_step[1][0] == 5 ){
		if( vms_work.card_bit == -1
		 || ( vms_work.card_bit & ( 1 << vms_work.file_port ) ) != ( vms_work.card_bit_keep & ( 1 << vms_work.file_port ) ) ){
			vms_work.card_task_step[1][0] = 2 ;
			vms_work.card_task_step[1][1] = 0 ;
		}
	}
	switch( vms_work.card_task_step[1][0] ){
	case 0 :
		if( fade_cnt() ) break ;
		vms_work.card_bit = vms_work.card_bit_keep = -1 ;
		vms_work.card_task_step[1][0] ++ ;
		vms_work.card_task_timer[0] = 0 ;
		work_init() ;
		vms_work.vm_msg[0] = 7 ;
		break ;
	case 1 :
		vms_work.card_task_step[1][0] ++ ;
		dm0d_vm_win_set() ;
		break ;
	case 2 :
		if( vms_work.card_bit == -1 ) break ;
		vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.file_flag != 1 || !vms_card_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_step[1][1] = 1 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 3 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_card_sys_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else if( !vms_date_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 5 ;
			vms_work.card_task_timer[0] = 2 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
		} else {
			ret = vms_ports_net_ck() ;
			if( !( ret & ( 1 << vms_work.file_port ) ) ){
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_timer[0] = 3 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			} else {
				vms_work.card_task_step[1][0] = 3 ;
			}
		}
		break ;
	case 3 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( ( ret = vms_data_load( 4, vms_work.file_port ) ) != -1 ){
				if( !ret ){
					vms_work.card_task_step[1][1] ++ ;
				} else {
					vms_work.card_task_step[1][0] = 5 ;
					vms_work.card_task_step[1][1] = 1 ;
					vms_work.card_task_timer[0] = 1 ;
					vms_work.vm_msg[0] = 1 ;
					dm0d_vm_msg_set() ;
					fade_set( 0xff000000, 1, FADE_IN ) ;
				}
			}
			break ;
		case 1 :
			if( !vms_net_key_ck() ){
				vms_work.card_task_step[1][0] = 4 ;
				vms_work.net_cnt_flag = 1 ;
			} else {
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 0 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 4 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
			break ;
		}
		break ;
	case 4 :
		if( ( ret = vms_task_save_all( vms_work.file_port, 1 ) ) != 2 ){
			if( ret == 1 ){
				vms_work.card_task_step[1][0] = 6 ;
				vms_date_keep( vms_work.file_port ) ;
			} else {
				vms_work.net_cnt_flag = 0 ;
				vms_work.card_task_step[1][0] = 5 ;
				vms_work.card_task_step[1][1] = 1 ;
				vms_work.card_task_timer[0] = 1 ;
				vms_work.vm_msg[0] = 2 ;
				dm0d_vm_msg_set() ;
				fade_set( 0xff000000, 1, FADE_IN ) ;
			}
		}
		break ;
	case 5 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 3 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 5 ;
				vms_work.vm_msg[1] = 1 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 2 :
			vms_work.vm_msg[1] = lr_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) vms_work.card_task_step[1][1] = 4 ;
				else vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 3 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][0] = 2 ;
				vms_work.card_task_step[1][1] = 0 ;
			}
			break ;
		case 4 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][1] ++ ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 5 :
			if( !fade_cnt() ) return 0 ;
		}
		break ;
	case 6 :
		if( !vms_work.card_task_timer[0] ) return 1 ;
		vms_work.card_task_step[1][0] ++ ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 7 :
		if( !fade_cnt() ) return 1 ;
		break ;
	}
	return 2 ;
}

/* kind 10: only the card and its network file are checked, nothing is loaded */
int net_vms_10( Uint16 sw )
{
	int ret ;

	if( vms_work.card_task_step[1][0] == 3 ){
		if( vms_work.card_bit == -1
		 || ( vms_work.card_bit & ( 1 << vms_work.file_port ) ) != ( vms_work.card_bit_keep & ( 1 << vms_work.file_port ) ) ){
			vms_work.card_task_step[1][0] = 2 ;
			vms_work.card_task_step[1][1] = 0 ;
		}
	}
	switch( vms_work.card_task_step[1][0] ){
	case 0 :
		if( fade_cnt() ) break ;
		vms_work.card_bit = vms_work.card_bit_keep = -1 ;
		vms_work.card_task_step[1][0] ++ ;
		vms_work.card_task_timer[0] = 0 ;
		work_init() ;
		break ;
	case 1 :
		vms_work.card_task_step[1][0] ++ ;
		dm0d_vm_win_set() ;
		break ;
	case 2 :
		if( vms_work.card_bit == -1 ) break ;
		vms_work.card_task_step[1][1] = 0 ;
		if( vms_work.file_flag != 1 || !vms_card_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 3 ;
			vms_work.card_task_step[1][1] = 1 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 3 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
			break ;
		}
		if( !vms_card_sys_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 3 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
			break ;
		}
		if( !vms_date_ck( vms_work.file_port ) ){
			vms_work.card_task_step[1][0] = 3 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
			break ;
		}
		ret = vms_ports_net_ck() ;
		if( !( ret & ( 1 << vms_work.file_port ) ) ){
			vms_work.card_task_step[1][0] = 3 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
			break ;
		}
		if( !vms_net_key_ck() ){
			vms_work.card_task_step[1][0] = 4 ;
		} else {
			vms_work.card_task_step[1][0] = 3 ;
			vms_work.card_task_timer[0] = 1 ;
			vms_work.vm_msg[0] = 4 ;
			dm0d_vm_msg_set() ;
			fade_set( 0xff000000, 1, FADE_IN ) ;
			break ;
		}
		break ;
	case 3 :
		switch( vms_work.card_task_step[1][1] ){
		case 0 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 3 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 1 :
			if( decide_cancel_ck() ){
				vms_work.card_task_step[1][1] ++ ;
				vms_work.vm_msg[0] = 5 ;
				vms_work.vm_msg[1] = 1 ;
				dm0d_vm_msg_set() ;
			}
			break ;
		case 2 :
			vms_work.vm_msg[1] = lr_select_loop( sw, vms_work.vm_msg[1], 1 ) ;
			if( decide_ck() ){
				if( !vms_work.vm_msg[1] ) vms_work.card_task_step[1][1] = 4 ;
				else vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] |= 0x80 ;
			} else if( cancel_ck() ){
				vms_work.card_task_step[1][1] = 3 ;
				vms_work.vm_msg[1] = 0xff ;
			}
			break ;
		case 3 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][0] = 2 ;
				vms_work.card_task_step[1][1] = 0 ;
			}
			break ;
		case 4 :
			if( !vms_work.vm_msg[1] ){
				vms_work.card_task_step[1][1] ++ ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 5 :
			if( !fade_cnt() ) return 0 ;
		}
		break ;
	case 4 :
		if( !vms_work.card_task_timer[0] ) return 1 ;
		vms_work.card_task_step[1][0] ++ ;
		fade_set( 0xff000000, 10, FADE_OUT ) ;
		break ;
	case 5 :
		if( !fade_cnt() ) return 1 ;
		break ;
	}
	return 2 ;
}

void net_vms_init( void )
{
	vms_work.card_task_step[0][0] = vms_work.card_task_step[0][1] = 0 ;
	vms_work.card_task_step[1][0] = vms_work.card_task_step[1][1] = 0 ;
	vms_work.vm_msg[0] = vms_work.vm_msg[1] = 0 ;
	vms_work.card_bit = vms_work.card_bit_keep = -1 ;
}

/* runs a kind's dialog for a frame; the card's state is read first */
int net_vms_main( Uint8 kind )
{
	Uint16 sw ;
	int ret ;

	sw = menu_sw_get( 3 ) ;
	vms_work.card_bit_keep = vms_work.card_bit ;
	vms_work.card_bit = vms_check_unit() ;
	switch( kind ){
	case 0 :
		ret = net_vms_00( sw ) ;
		break ;
	case 1 :
	case 2 :
		ret = net_vms_01( sw ) ;
		break ;
	case 3 :
	case 4 :
		ret = net_vms_03( sw ) ;
		break ;
	case 5 :
		ret = net_vms_05( sw ) ;
		break ;
	case 6 :
		ret = net_vms_06( sw ) ;
		break ;
	case 10 :
		ret = net_vms_10( sw ) ;
		break ;
	default :
		ret = 0 ;
		break ;
	}
	return ret ;
}
