/* The sound requests: the sound driver's calls for sequences, packs and volumes, the s3l streams,
   the players' voices and sound effects panned to their place on the screen, and the sound test. */
#include "player.h"
#include "naomi.h"

/* Shinobi / Kamui */
extern Sint32 nlsndPackLoad( void *PackAddress, Sint32 Port ) ;
extern Sint32 nlsndPackFree( Sint32 Port ) ;
extern Sint32 nlsndSystemControl( Uint16 Pack, Uint32 Command, Sint32 Parameter ) ;
extern Sint32 nlsndControlChange( Uint32 Pack, Uint32 MidiCh, Uint32 Command, Sint32 Parameter ) ;
extern Sint32 nlsndSeqRequest( Uint32 Request ) ;
extern Sint32 nlsndRemainMem( void ) ;
extern char *nlsndVersion( void ) ;
extern int s3lInit( int mono, int stereo, int quad ) ;
extern int s3lSetStereoPort( int p, Uint32 start, int loop ) ;
extern int s3lSetVolume( int p, int vol ) ;
extern int s3lStartPort( int p ) ;
extern int s3lStopPort( int p ) ;
extern void s3lIntr( void ) ;

#define SND_ALL_SOUND_OFF	0x0100a0
#define SND_ALL_SE_OFF		0x0300a0
#define SND_STEREO_SW		0x0800a0
#define SND_MONO_SW		0x0900a0
#define SND_PORT_SOUND_OFF	0x1100a0
#define SND_TOTAL_VOLUME	0x01a0
#define SND_SONG_VOLUME		0x04a0
#define SND_FADE_IN		0x09a0
#define SND_FADE_OUT		0x0aa0
#define SND_SE_VOLUME		0x10a0
#define SND_MIDICH_VOLUME	0x0027b0
#define SND_MIDICH_PAN		0x002ab0

/* a request for a sequence: a song (0xa8) or a sound (0xa9, plus the side) from a bank */
#define SEQ_SONG		0xa8
#define SEQ_SE			0xa9

/* the banks of sounds beside the characters' own, which are numbered as the characters */
#define BANK_COMMON		0x3c
#define BANK_NA		0x3d

extern void file_load( Sint32 no, void *adrs, Sint32 a ) ;
extern int dbg_flag_ck( int a, int b ) ;
extern void get_2d_pos( VEC3 *pos, Float *scr ) ;
extern const Uint8 *voice_tbl[] ;	/* each character's voices for the other voice type */

typedef struct SOUND_WORK {
	Uint8	unk_00[8];
	Sint32	s3l_port;	/* 008: the stream's port: 1 stereo, 0 mono */
	Sint32	s3l_init;	/* 00c */
	Sint32	s3l_set;	/* 010 */
	Sint8	dbg_pl_type;	/* 014: the sound test's character */
	Sint8	dbg_pl_se;	/* 015: and its sound */
	Sint8	dbg_stream;	/* 016 */
	Sint8	dbg_song;	/* 017 */
	Uint32	req_code;	/* 018: the last request */
	Sint32	pan;		/* 01c: the last pan */
	Uint8	dbg_s3l_vol;	/* 020 */
	Sint8	dbg_page;	/* 021 */
	Sint8	dbg_se;		/* 022 */
	Sint8	dbg_na;		/* 023 */
	Uint8	dbg_side;	/* 024 */
	Uint8	unk_25;
	Uint8	pack[6];	/* 026: for each player work, the first one with its sound pack */
	Float	s3l_vol;	/* 02c */
	Float	s3l_spd;	/* 030: the stream's fade */
	Float	vol[8];		/* 034: the total volume, then each pack's */
	Sint32	vol_max;	/* 054 */
} SOUND_WORK;

/* the last requests and loads, for the sound test's display */
typedef struct SOUND_LOG {
	Uint32	req[21];
	Sint32	load[11];
} SOUND_LOG;

void s3l_vol_set( Uint8 vol ) ;
void s3l_fade_move( void ) ;
void voice_req( PLAYER_WORK *pl_w, Uint8 no ) ;
int heavy_se( PLAYER_WORK *pl_w, Uint8 no ) ;
void req_log_set( Uint32 code ) ;
void load_log_set( Sint32 n ) ;
void ver_disp( void ) ;
void dbg_pl_load( Uint8 no ) ;
void dbg_song_load( Sint8 no ) ;
void dbg_pl_se_req( Uint8 pl, Uint8 no ) ;
void dbg_se_req( Uint8 no ) ;
void dbg_na_req( Uint8 no ) ;
void dbg_song_req( Uint8 no ) ;

/* the port of each player's pack */
const Sint32 port_tbl[6] = { 2, 3, 4, 5, 6, 7 } ;

/* the hit sounds a heavy character makes */
const Uint8 heavy_se_tbl[17] = {
	0x11, 0x12, 0x13, 0x13, 0x11, 0x12, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x13, 0x11, 0x12,
	0x13
} ;
const Uint8 dm_se_tbl[3] = { 0x3c, 0x3d, 0x3e } ;

/* the streamed music: its stream, and whether it loops */
const Sint32 stream_no_tbl[14] = {
	0x7c, 0x7d, 0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x88, 0x89, 0x87
} ;
const Uint8 stream_loop_tbl[14] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 } ;

/* each character's sound pack */
const Sint32 pl_snd_file_tbl[59] = {
	0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46,
	0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e,
	0x4f, 0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56,
	0x57, 0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e,
	0x5f, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66,
	0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e,
	0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76,
	0x77, 0x78, 0x79
} ;

/* the common sound of each hit, 0xff none */
const Uint8 hit_se_tbl[29] = {
	0xff, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x02, 0x03, 0x00,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x06, 0x08, 0x08, 0x0d,
	0x00, 0x07, 0x09, 0x0a, 0x0b, 0x0c, 0x0e, 0x0f, 0x10
} ;

SOUND_WORK snd_w ;
SOUND_LOG snd_log ;

/****************************************************************/
void sound_req( Uint32 code )
{
	if( Sys->demo_sound || Sys->play_mode != 64 ){
		nlsndSeqRequest( code ) ;
		snd_w.req_code = code ;
		req_log_set( code ) ;
	}
}

/****************************************************************/
void sound_req_direct( Uint32 code )
{
	nlsndSeqRequest( code ) ;
	snd_w.req_code = code ;
	req_log_set( code ) ;
}

/****************************************************************/
void sound_all_off( void )
{
	nlsndSystemControl( 0, SND_ALL_SOUND_OFF, 0 ) ;
}

/****************************************************************/
void port_sound_off( Sint8 port )
{
	nlsndSystemControl( port, SND_PORT_SOUND_OFF, 0 ) ;
}

/****************************************************************/
void se_all_off( void )
{
	nlsndSystemControl( 0, SND_ALL_SE_OFF, 0 ) ;
}

/****************************************************************/
void song_vol_set( Sint8 pack, Uint8 vol )
{
	vol &= 127 ;
	nlsndSystemControl( pack, SND_SONG_VOLUME, vol ) ;
}

/****************************************************************/
void s3l_vol_set( Uint8 vol )
{
	vol &= 127 ;
	s3lSetVolume( snd_w.s3l_port, vol ) ;
}

/****************************************************************/
void fade_out_set( Uint16 pack, Uint8 spd )
{
	spd &= 127 ;
	nlsndSystemControl( pack, SND_FADE_OUT, spd ) ;
}

/****************************************************************/
void fade_in_set( Uint16 pack, Uint8 spd )
{
	spd &= 127 ;
	nlsndSystemControl( pack, SND_FADE_IN, spd ) ;
}

/****************************************************************/
void total_vol_down( Uint8 spd )
{
	Float step ;

	spd &= 127 ;
	step = snd_w.vol_max / spd ;
	snd_w.vol[0] -= step ;
	if( snd_w.vol[0] < 0.0f ) snd_w.vol[0] = 0.0f ;
	nlsndSystemControl( 0, SND_TOTAL_VOLUME, ( Uint8 )snd_w.vol[0] ) ;
}

/****************************************************************/
void total_vol_up( Uint8 spd )
{
	Float step ;

	spd &= 127 ;
	step = snd_w.vol_max / spd ;
	snd_w.vol[0] += step ;
	if( snd_w.vol[0] > snd_w.vol_max ) snd_w.vol[0] = snd_w.vol_max ;
	nlsndSystemControl( 0, SND_TOTAL_VOLUME, ( Uint8 )snd_w.vol[0] ) ;
}

/****************************************************************/
void se_vol_down( Uint16 pack, Uint8 spd )
{
	Float step ;

	spd &= 127 ;
	step = snd_w.vol_max / spd ;
	snd_w.vol[pack] -= step ;
	if( snd_w.vol[pack] < 0.0f ) snd_w.vol[pack] = 0.0f ;
	nlsndSystemControl( pack, SND_SE_VOLUME, ( Uint8 )snd_w.vol[pack] ) ;
}

/****************************************************************/
void se_vol_up( Uint16 pack, Uint8 spd )
{
	Float step ;

	spd &= 127 ;
	step = snd_w.vol_max / spd ;
	snd_w.vol[pack] += step ;
	if( snd_w.vol[pack] > snd_w.vol_max ) snd_w.vol[pack] = snd_w.vol_max ;
	nlsndSystemControl( pack, SND_SE_VOLUME, ( Uint8 )snd_w.vol[pack] ) ;
}

/****************************************************************/
void s3l_fade_out_set( Uint8 spd )
{
	spd &= 127 ;
	snd_w.s3l_spd = -( Float )( snd_w.vol_max / spd ) ;
}

/****************************************************************/
void s3l_fade_in_set( Uint8 spd )
{
	spd &= 127 ;
	snd_w.s3l_spd = snd_w.vol_max / spd ;
}

/****************************************************************/
void s3l_fade_move( void )
{
	snd_w.s3l_vol += snd_w.s3l_spd ;
	if( snd_w.s3l_vol > snd_w.vol_max ) snd_w.s3l_vol = snd_w.vol_max ;
	if( snd_w.s3l_vol < 0.0f ) snd_w.s3l_vol = 0.0f ;
	s3l_vol_set( snd_w.s3l_vol ) ;
}

/****************************************************************/
void stereo_set( void )
{
	snd_w.s3l_port = 1 ;
	nlsndSystemControl( 0, SND_STEREO_SW, 0 ) ;
}

/****************************************************************/
void mono_set( void )
{
	snd_w.s3l_port = 0 ;
	nlsndSystemControl( 0, SND_MONO_SW, 0 ) ;
}

/****************************************************************/
void sound_init( void )
{
	Uint32 i ;

	for( i = 0 ; i < 8 ; i ++ ) snd_w.vol[i] = 127.0f ;
	snd_w.s3l_init = s3lInit( 1, 1, 0 ) ;
	snd_w.s3l_spd = 0.0f ;
	snd_w.s3l_vol = 127.0f ;
	snd_w.vol_max = 127 ;
}

/****************************************************************/
void total_vol_set( int vol )
{
	Uint32 i ;
	Float f ;

	f = vol ;
	for( i = 0 ; i < 8 ; i ++ ) snd_w.vol[i] = f ;
	nlsndControlChange( 1, 16, SND_MIDICH_VOLUME, vol & 127 ) ;
	nlsndControlChange( 2, 16, SND_MIDICH_VOLUME, vol & 127 ) ;
	nlsndControlChange( 3, 16, SND_MIDICH_VOLUME, vol & 127 ) ;
	nlsndControlChange( 4, 16, SND_MIDICH_VOLUME, vol & 127 ) ;
	nlsndControlChange( 5, 16, SND_MIDICH_VOLUME, vol & 127 ) ;
	nlsndControlChange( 6, 16, SND_MIDICH_VOLUME, vol & 127 ) ;
	nlsndControlChange( 7, 16, SND_MIDICH_VOLUME, vol & 127 ) ;
	snd_w.s3l_vol = f ;
	snd_w.vol_max = vol ;
}

/****************************************************************/
void total_vol_init( void )
{
	if( Sys->demo_sound == 2 ) total_vol_set( 95 ) ;
	else total_vol_set( 127 ) ;
}

/****************************************************************/
void sound_dummy( void )
{
}

/****************************************************************/
/*			Loads each player's sound pack, once for each		*/
/*			character											*/
/****************************************************************/
void pl_sound_load( void )
{
	int i, j ;

	nlsndPackFree( 7 ) ;
	nlsndPackFree( 5 ) ;
	nlsndPackFree( 3 ) ;
	nlsndPackFree( 6 ) ;
	nlsndPackFree( 4 ) ;
	nlsndPackFree( 2 ) ;
	for( i = 0 ; i < 6 ; i ++ ){
		snd_w.pack[i] = i ;
		for( j = 0 ; j < i ; j ++ ){
			if( pl_work[i].load_type == pl_work[j].load_type ){
				snd_w.pack[i] = j ;
				break ;
			}
		}
		if( snd_w.pack[i] == i ){
			file_load( pl_snd_file_tbl[pl_work[i].load_type], ( void * )0x0cc00000, 1 ) ;
			nlsndPackLoad( ( void * )0x0cc00000, port_tbl[snd_w.pack[i]] ) ;
			load_log_set( nlsndRemainMem() ) ;
		}
	}
}

/****************************************************************/
void com_sound_load( void )
{
	load_log_set( nlsndRemainMem() ) ;
	file_load( 0x340, ( void * )0x0c420000, 1 ) ;
	nlsndPackLoad( ( void * )0x0c420000, 0 ) ;
	sound_req( ( 10 << 16 ) + SEQ_SONG ) ;
	load_log_set( nlsndRemainMem() ) ;
	file_load( 0x33f, ( void * )0x0cc00000, 1 ) ;
	nlsndPackLoad( ( void * )0x0cc00000, 1 ) ;
	load_log_set( nlsndRemainMem() ) ;
}

/****************************************************************/
void ex_sound_load( void )
{
	nlsndPackFree( 7 ) ;
	load_log_set( nlsndRemainMem() ) ;
	file_load( 123, ( void * )0x0c420000, 1 ) ;
	nlsndPackLoad( ( void * )0x0c420000, 7 ) ;
	load_log_set( nlsndRemainMem() ) ;
}

/****************************************************************/
void ex_sound_load2( void )
{
	nlsndPackFree( 7 ) ;
	load_log_set( nlsndRemainMem() ) ;
	file_load( 123, ( void * )0x0c420000, 1 ) ;
	nlsndPackLoad( ( void * )0x0c420000, 7 ) ;
	load_log_set( nlsndRemainMem() ) ;
}

/****************************************************************/
void staff_sound_load( void )
{
	nlsndPackFree( 7 ) ;
	load_log_set( nlsndRemainMem() ) ;
	file_load( 122, ( void * )0x0c420000, 1 ) ;
	nlsndPackLoad( ( void * )0x0c420000, 7 ) ;
	load_log_set( nlsndRemainMem() ) ;
}

/****************************************************************/
void sound_vsync( void )
{
	s3lIntr() ;
	while( !nlaCheckWaitROMDMA() ) ;
	s3l_fade_move() ;
}

/****************************************************************/
void stream_req( Sint8 no )
{
	register const FILE_DATA *tbl ;
	register const Sint32 *num ;
	register const Uint8 *loop ;

	if( Sys->demo_sound || Sys->play_mode != 64 ){
		snd_w.s3l_port = 1 ;
		loop = &stream_loop_tbl[no] ;
		num = &stream_no_tbl[no] ;
		tbl = stream_tbl ;
port_set :
		snd_w.s3l_set = s3lSetStereoPort( 1, tbl[*num].adrs, *loop ) ;
		load_log_set( snd_w.s3l_set ) ;
		load_log_set( stream_tbl[*num].adrs ) ;
	}
}

/****************************************************************/
void stream_start( void )
{
	if( Sys->demo_sound || Sys->play_mode != 64 ){
		if( !dbg_flag_ck( 3, 6 ) && !Sys->no_bgm_flag ){
			s3lStartPort( snd_w.s3l_port ) ;
			s3l_vol_set( snd_w.s3l_vol ) ;
		}
	}
}

/****************************************************************/
void stream_stop( void )
{
	s3lStopPort( snd_w.s3l_port ) ;
}

/****************************************************************/
void stream_dummy( void )
{
}

/****************************************************************/
void song_req( Uint8 no )
{
	if( !dbg_flag_ck( 3, 6 ) && !Sys->no_bgm_flag ){
		sound_req( no * 0x10000 + SEQ_SONG ) ;
		song_vol_set( 0, snd_w.vol[0] ) ;
	}
}

/****************************************************************/
void song_stop( void )
{
	sound_req( SEQ_SONG ) ;
}

/****************************************************************/
/*			A voice from the character's own bank, panned to	*/
/*			where the player is on the screen					*/
/****************************************************************/
void voice_req( PLAYER_WORK *pl_w, Uint8 no )
{
	const Uint8 *tbl ;
	Float scr[2] ;
	Sint32 pan ;
	Uint8 v, side ;

	get_2d_pos( &pl_w->pos, scr ) ;
	pan = ( scr[0] + -320.0f ) / 5.0f ;
	if( pan < -64 ) pan = -64 ;
	if( pan > 63 ) pan = 63 ;
	snd_w.pan = pan ;
	nlsndControlChange( port_tbl[snd_w.pack[pl_w->work_no]], 16, SND_MIDICH_PAN, pan ) ;
	if( Sys->voice_type ){
		tbl = voice_tbl[pl_w->pl_type] ;
		v = tbl[no] ;
		if( v == 0xfe ) return ;
		if( v != 0xff ) no = v ;
	}
	side = pl_w->pl_no & 1 ;
	sound_req( no * 0x10000 + ( pl_w->pl_type << 8 ) + side + SEQ_SE ) ;
}

/****************************************************************/
void pl_voice_req( PLAYER_WORK *pl_w, Uint8 no )
{
	if( no <= 63 ) voice_req( pl_w, no ) ;
}

/****************************************************************/
/*			The voice or sound the animation's frame asks		*/
/*			for													*/
/****************************************************************/
void cg_se_req( PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_se ) return ;
	if( pl_w->cg_se < 0x80 ){
		if( pl_w->cg_se > 63 ) return ;
		voice_req( pl_w, pl_w->cg_se ) ;
	} else {
		if( pl_w->cg_se - 0x80 > 80 ) return ;
		pl_se_req( pl_w, pl_w->cg_se + 0x80 ) ;
	}
}

/****************************************************************/
void dm_voice_req( PLAYER_WORK *pl_w )
{
	if( pl_w->vital_new ) voice_req( pl_w, ( Rnd() & 1 ) + 1 ) ;
}

/****************************************************************/
/*			A voice from Anakaris's bank, panned to ew			*/
/****************************************************************/
void pl04_voice_req( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Uint8 no )
{
	const Uint8 *tbl ;
	Float scr[2] ;
	Sint32 pan ;
	Uint8 v, side ;

	get_2d_pos( &ew->pos, scr ) ;
	pan = ( scr[0] + -320.0f ) / 5.0f ;
	if( pan < -64 ) pan = -64 ;
	if( pan > 63 ) pan = 63 ;
	snd_w.pan = pan ;
	nlsndControlChange( port_tbl[snd_w.pack[pl_w->work_no]], 16, SND_MIDICH_PAN, pan ) ;
	if( Sys->voice_type ){
		tbl = voice_tbl[4] ;
		v = tbl[no] ;
		if( v == 0xfe ) return ;
		if( v != 0xff ) no = v ;
	}
	side = ew->pl_no & 1 ;
	sound_req( no * 0x10000 + side + ( 4 << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
/*			A voice from the second Abyss's bank				*/
/****************************************************************/
void pl19_voice_req( PLAYER_WORK *pl_w, register Uint8 no )
{
	const Uint8 *tbl ;
	Float scr[2] ;
	Sint32 pan ;
	Uint8 v, side ;

	get_2d_pos( &pl_w->pos, scr ) ;
	pan = ( scr[0] + -320.0f ) / 5.0f ;
	if( pan < -64 ) pan = -64 ;
	if( pan > 63 ) pan = 63 ;
	snd_w.pan = pan ;
	nlsndControlChange( port_tbl[snd_w.pack[pl_w->work_no]], 16, SND_MIDICH_PAN, pan ) ;
	if( Sys->voice_type ){
		tbl = voice_tbl[0x19] ;
		v = tbl[no] ;
		if( v == 0xfe ) return ;
		if( v != 0xff ) no = v ;
	}
	side = pl_w->pl_no & 1 ;
	sound_req( no * 0x10000 + side + ( 0x19 << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
/*			A sound from the first Abyss's bank					*/
/****************************************************************/
void pl18_se_req( Uint8 no )
{
	sound_req( no * 0x10000 + ( 0x18 << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
/*			A common sound effect, panned to where the			*/
/*			player is on the screen								*/
/****************************************************************/
void pl_se_req( PLAYER_WORK *pl_w, Uint8 no )
{
	Float scr[2] ;
	Sint32 pan ;
	Uint8 side ;

	get_2d_pos( &pl_w->pos, scr ) ;
	pan = ( scr[0] + -320.0f ) / 5.0f ;
	if( pan < -64 ) pan = -64 ;
	if( pan > 63 ) pan = 63 ;
	snd_w.pan = pan ;
	nlsndControlChange( 1, 16, SND_MIDICH_PAN, pan ) ;
	side = pl_w->pl_no & 1 ;
	sound_req( no * 0x10000 + side + ( BANK_COMMON << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
/*			A common sound effect from the middle				*/
/****************************************************************/
void com_se_req( Uint8 no )
{
	nlsndControlChange( 1, 16, SND_MIDICH_PAN, 0 ) ;
	sound_req( no * 0x10000 + ( BANK_COMMON << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
/*			The sound of the hit that struck the player			*/
/****************************************************************/
void dm_se_req( PLAYER_WORK *pl_w )
{
	HIT_DATA *hd ;
	Uint8 se, v ;

	hd = pl_w->hit_data_adrs ;
	se = hit_se_tbl[hd->hit_se] ;
	if( se == 0xff ) return ;
	if( !pl_w->work_kind ){
		if( ( v = heavy_se( pl_w, se ) ) ) se = v ;
	}
	pl_se_req( pl_w, se ) ;
	voice_req( pl_w, 42 ) ;
	voice_req( pl_w, 43 ) ;
}

/****************************************************************/
/*			A hit sound changed for a heavy character, or 0		*/
/****************************************************************/
int heavy_se( PLAYER_WORK *pl_w, Uint8 no )
{
	if( pl_w->pl_type == PL_SONSON ) return 0 ;
	switch( no ){
	case 4 :
	case 5 :
	case 6 :
	case 14 :
	case 15 :
	case 16 :
		return 0 ;
	}
	if( FLAG_CK( pl_w->pl_bit, PLB0( PL_DOOM ) | PLB0( PL_TRON ) | PLB0( PL_ABYSS1 ), PLB1( PL_WAR_MACHINE ) | PLB1( PL_IRON_MAN ) | PLB1( PL_SENTINEL ) ) ) no = heavy_se_tbl[no] ;
	if( pl_w->armor_flag & ARMOR_HEAVY ) no = heavy_se_tbl[no] ;
	return no ;
}

/****************************************************************/
/*			The sound of the player falling on the floor		*/
/****************************************************************/
void down_se_req( PLAYER_WORK *pl_w )
{
	Uint8 se ;

	if( pl_w->weight > 3 ){ }
	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->weight >= 5 ) se = 49 ;
	else se = 48 ;
	pl_se_req( pl_w, se ) ;
}

/****************************************************************/
void catch_se_req( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	if( em->weight > 3 ){ }
	if( em->armor_flag & ARMOR_HEAVY || em->weight >= 5 ) pl_se_req( em, 49 ) ;
	else pl_se_req( em, 48 ) ;
}

/****************************************************************/
void fall_se_req( PLAYER_WORK *pl_w )
{
	Uint8 se, no ;

	if( pl_w->pl_type == PL_STORM ) return ;
	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->weight >= 5 ) se = 53 ;
	else se = 52 ;
	if( pl_w->pl_type == PL_TRON ){
		voice_req( pl_w, 32 ) ;
		no = 43 ;
	} else if( pl_w->pl_type == PL_SENTINEL ) no = 31 ;
	else {
		pl_se_req( pl_w, se ) ;
		no = 43 ;
	}
	voice_req( pl_w, no ) ;
}

/****************************************************************/
/*			The sound of the player landing						*/
/****************************************************************/
void land_se_req( PLAYER_WORK *pl_w )
{
	Uint8 se ;

	if( pl_w->armor_flag & ARMOR_HEAVY || pl_w->weight >= 5 ) se = 45 ;
	else se = 44 ;
	pl_se_req( pl_w, se ) ;
}

/****************************************************************/
void hit_se_req( PLAYER_WORK *pl_w, Uint8 no )
{
	Uint8 v ;

	if( ( v = heavy_se( pl_w, no ) ) ) no = v ;
	pl_se_req( pl_w, no ) ;
}

/****************************************************************/
void hyper_se_req( PLAYER_WORK *pl_w, Uint8 no )
{
	pl_se_req( pl_w, dm_se_tbl[no] ) ;
	voice_req( pl_w, 43 ) ;
}

/****************************************************************/
void sjump_se_req( PLAYER_WORK *pl_w )
{
	pl_se_req( pl_w, 50 ) ;
	voice_req( pl_w, 43 ) ;
}

/****************************************************************/
void nage_se_req( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;

	pl_se_req( pl_w, 35 ) ;
	voice_req( em, 42 ) ;
	voice_req( em, 43 ) ;
	pl_se_req( em, 58 ) ;
	pl_se_req( em, 59 ) ;
}

/****************************************************************/
void hyper_flash_se_req( PLAYER_WORK *pl_w )
{
	pl_se_req( pl_w, 30 ) ;
	voice_req( pl_w, 43 ) ;
}

/****************************************************************/
void ex_se_req( Uint8 no )
{
	nlsndControlChange( 7, 16, SND_MIDICH_PAN, 0 ) ;
	sound_req( no * 0x10000 + ( 0x3e << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
void staff_se_req( Uint8 no )
{
	nlsndControlChange( 7, 16, SND_MIDICH_PAN, 0 ) ;
	sound_req( no * 0x10000 + ( 0x3f << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
/*			A system sound										*/
/****************************************************************/
void na_req( Uint8 no )
{
	sound_req( no * 0x10000 + ( BANK_NA << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
void na_first_req( void )
{
	Uint32 code ;

	if( Sys->st_demo_flag ) code = SEQ_SONG ;
	else code = ( BANK_NA << 8 ) + SEQ_SE ;
	sound_req_direct( code ) ;
}

/****************************************************************/
/*			The sound test										*/
/****************************************************************/
void sound_debug_init( void )
{
	int i ;

	snd_w.dbg_pl_type = 0 ;
	snd_w.dbg_pl_se = 0 ;
	snd_w.dbg_stream = 0 ;
	snd_w.dbg_song = 0 ;
	snd_w.dbg_side = 0 ;
	stream_req( snd_w.dbg_stream ) ;
	dbg_song_load( snd_w.dbg_song ) ;
	dbg_pl_load( snd_w.dbg_pl_type ) ;
	for( i = 0 ; i < 8 ; i ++ ) nlsndControlChange( i, 16, SND_MIDICH_PAN, 0 ) ;
}

/****************************************************************/
/*			The sound test's pages: a character's sounds		*/
/****************************************************************/
void dbg_pl_page( void )
{
	if( sw_work[1].on & KEY_UP ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_pl_type -= 16 ;
		else snd_w.dbg_pl_type -= 1 ;
		if( snd_w.dbg_pl_type < 0 ) snd_w.dbg_pl_type = 58 ;
		dbg_pl_load( snd_w.dbg_pl_type ) ;
	}
	if( sw_work[1].on & KEY_DOWN ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_pl_type += 16 ;
		else snd_w.dbg_pl_type += 1 ;
		if( snd_w.dbg_pl_type > 58 ) snd_w.dbg_pl_type = 0 ;
		dbg_pl_load( snd_w.dbg_pl_type ) ;
	}
	if( sw_work[1].on & KEY_LEFT ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_pl_se -= 16 ;
		else snd_w.dbg_pl_se -= 1 ;
		if( snd_w.dbg_pl_se < 0 ) snd_w.dbg_pl_se = 63 ;
	}
	if( sw_work[1].on & KEY_RIGHT ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_pl_se += 16 ;
		else snd_w.dbg_pl_se += 1 ;
		if( snd_w.dbg_pl_se > 63 ) snd_w.dbg_pl_se = 0 ;
	}
	if( sw_work[1].on & KEY_HP ) dbg_pl_se_req( snd_w.dbg_pl_type, snd_w.dbg_pl_se ) ;
	if( sw_work[1].on & KEY_A1 ) se_all_off() ;
	if( sw_work[1].on & KEY_LK ) snd_w.dbg_side ^= 1 ;
}

/****************************************************************/
/*			The common sounds									*/
/****************************************************************/
void dbg_se_page( void )
{
	if( sw_work[1].on & KEY_LEFT ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_se -= 16 ;
		else snd_w.dbg_se -= 1 ;
		if( snd_w.dbg_se < 0 ) snd_w.dbg_se = 80 ;
	}
	if( sw_work[1].on & KEY_RIGHT ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_se += 16 ;
		else snd_w.dbg_se += 1 ;
		if( snd_w.dbg_se > 80 ) snd_w.dbg_se = 0 ;
	}
	if( sw_work[1].on & KEY_HP ) dbg_se_req( snd_w.dbg_se ) ;
	if( sw_work[1].on & KEY_A1 ) se_all_off() ;
	if( sw_work[1].on & KEY_LK ) snd_w.dbg_side ^= 1 ;
}

/****************************************************************/
/*			The narrator										*/
/****************************************************************/
void dbg_na_page( void )
{
	if( sw_work[1].on & KEY_LEFT ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_na -= 16 ;
		else snd_w.dbg_na -= 1 ;
		if( snd_w.dbg_na < 0 ) snd_w.dbg_na = 79 ;
	}
	if( sw_work[1].on & KEY_RIGHT ){
		if( sw_work[1].now & KEY_LP ) snd_w.dbg_na += 16 ;
		else snd_w.dbg_na += 1 ;
		if( snd_w.dbg_na > 79 ) snd_w.dbg_na = 0 ;
	}
	if( sw_work[1].on & KEY_HP ) dbg_na_req( snd_w.dbg_na ) ;
	if( sw_work[1].on & KEY_A1 ) se_all_off() ;
}

/****************************************************************/
/*			The songs											*/
/****************************************************************/
void dbg_song_page( void )
{
	if( sw_work[1].on & KEY_LEFT ){
		snd_w.dbg_song -- ;
		if( snd_w.dbg_song < 0 ) snd_w.dbg_song = 13 ;
	}
	if( sw_work[1].on & KEY_RIGHT ){
		snd_w.dbg_song ++ ;
		if( snd_w.dbg_song > 13 ) snd_w.dbg_song = 0 ;
	}
	if( sw_work[1].on & KEY_HP ) dbg_song_req( snd_w.dbg_song ) ;
	if( sw_work[1].on & KEY_A1 ) sound_all_off() ;
}

/****************************************************************/
/*			The streamed music									*/
/****************************************************************/
void dbg_stream_page( void )
{
	if( sw_work[1].on & KEY_LEFT ){
		snd_w.dbg_stream -- ;
		if( snd_w.dbg_stream < 0 ) snd_w.dbg_stream = 13 ;
		stream_req( snd_w.dbg_stream ) ;
	}
	if( sw_work[1].on & KEY_RIGHT ){
		snd_w.dbg_stream ++ ;
		if( snd_w.dbg_stream > 13 ) snd_w.dbg_stream = 0 ;
		stream_req( snd_w.dbg_stream ) ;
	}
	if( sw_work[1].on & KEY_HP ) stream_start() ;
	if( sw_work[1].on & KEY_A1 ) stream_stop() ;
	if( sw_work[1].now & KEY_HK ){
		snd_w.dbg_s3l_vol -- ;
		snd_w.dbg_s3l_vol &= 127 ;
		s3l_vol_set( snd_w.dbg_s3l_vol ) ;
	}
	if( sw_work[1].now & KEY_A2 ){
		snd_w.dbg_s3l_vol ++ ;
		snd_w.dbg_s3l_vol &= 127 ;
		s3l_vol_set( snd_w.dbg_s3l_vol ) ;
	}
}

/****************************************************************/
void sound_debug( void )
{
	static const Uint8 cur_y_tbl[5] = { 10, 15, 20, 25, 30 } ;
	static char *pl_name_tbl[59] = {
		"00 RYU", "01 ZANGIEF", "02 GUILE", "03 MORRIGAN",
		"04 ANAKARIS", "05 STRIDER HIRYU", "06 CYCLOPS", "07 WOLVERINE",
		"08 PSYLOCKE", "09 ICE-MAN", "0A ROGUE", "0B CAPTAIN AMERICA",
		"0C SPIDER-MAN", "0D HULK", "0E VENOM", "0F Dr.DOOM",
		"10 TORON&KOBUN", "11 JILL", "12 HAYATO", "13 New Character(Pirate)",
		"14 New Character(SON-SON)", "15 New Character(SABOTEN)", "16 MARROW", "17 CABLE",
		"18 LAST BOSS 1", "19 LAST BOSS 2", "1A LAST BOSS 3", "1B CHUN-LI",
		"1C ROCK-MAN", "1D ROLL", "1E GOUKI", "1F BULETTA",
		"20 FELICIA", "21 NASH", "22 SAKURA", "23 DAN",
		"24 CAMMY", "25 DHALSIM", "26 VEGA", "27 KEN",
		"28 GAMBIT", "29 JUGGERNAUT", "2A STORM", "2B SABRETOOTH",
		"2C MAGNETO", "2D SHUMAGORATH", "2E WAR-MACHINE", "2F SILVER-SAMURAI",
		"30 OMEGA-RED", "31 SPIRAL", "32 COLOSSUS", "33 IRON-MAN",
		"34 SENTINEL", "35 BLACK HEART", "36 THANOTH", "37 GIN-SAOTOME",
		"38 CAPTAIN COMMANDO", "39 HONE WOLVERINE", "3A KOBUN"
	} ;
	static char *stream_name_tbl[14] = {
		"STAGE 00", "STAGE 01", "STAGE 02", "STAGE 03",
		"STAGE 04", "STAGE 05", "STAGE 06", "STAGE 07",
		"STAGE 08", "STAGE 09", "STAGE 0A", "OPENING 0B",
		"STAFF ROLL 0C", "CAPCOM LOGO 0D"
	} ;
	static char *song_name_tbl[14] = {
		"DUMMY 00", "SELECT 01", "VS 02", "HERE COME 03",
		"CONTINUE 04", "GAME OVER 05", "RANKING 06", "DUMMY 07",
		"WIN 08", "DUMMY 09", "DUMMY 0A", "BOSS DEMO 0B",
		"DUMMY 0C", "ENDING 0D"
	} ;
	static void ( *page_jmp[5] )( void ) = {
		dbg_pl_page, dbg_se_page, dbg_na_page, dbg_song_page, dbg_stream_page
	} ;

	if( sw_work[0].on & KEY_UP ){
		snd_w.dbg_page -- ;
		if( snd_w.dbg_page < 0 ) snd_w.dbg_page = 4 ;
	}
	if( sw_work[0].on & KEY_DOWN ){
		snd_w.dbg_page ++ ;
		if( snd_w.dbg_page > 4 ) snd_w.dbg_page = 0 ;
	}
	page_jmp[snd_w.dbg_page]() ;
	nlLocate( 15, cur_y_tbl[snd_w.dbg_page] ) ;
	nlPrintf( "--->\n" ) ;
	nlLocate( 20, 10 ) ;
	nlPrintf( "%s\n", pl_name_tbl[snd_w.dbg_pl_type] ) ;
	nlPrintf( "PLAYER SE %x\n", snd_w.dbg_pl_se ) ;
	if( !snd_w.dbg_side ) nlPrintf( "1P" ) ;
	else nlPrintf( "2P" ) ;
	nlLocate( 20, 15 ) ;
	nlPrintf( "COMMON SE %x\n", snd_w.dbg_se ) ;
	nlLocate( 20, 20 ) ;
	nlPrintf( "COMMON NA %x\n", snd_w.dbg_na ) ;
	nlLocate( 20, 25 ) ;
	nlPrintf( "%s\n", song_name_tbl[snd_w.dbg_song] ) ;
	nlLocate( 20, 30 ) ;
	nlPrintf( "%s\n", stream_name_tbl[snd_w.dbg_stream] ) ;
	nlLocate( 20, 31 ) ;
	nlPrintf( "STREAM VOL %x\n", snd_w.dbg_s3l_vol ) ;
}

/****************************************************************/
void dbg_pl_load( Uint8 no )
{
	nlsndPackFree( 7 ) ;
	file_load( pl_snd_file_tbl[no], ( void * )0x0cc00000, 1 ) ;
	nlsndPackLoad( ( void * )0x0cc00000, 7 ) ;
}

/****************************************************************/
void dbg_song_load( Sint8 no )
{
}

/****************************************************************/
void dbg_pl_se_req( Uint8 pl, Uint8 no )
{
	sound_req( no * 0x10000 + ( pl << 8 ) + snd_w.dbg_side + SEQ_SE ) ;
}

/****************************************************************/
void dbg_se_req( Uint8 no )
{
	sound_req( no * 0x10000 + snd_w.dbg_side + ( BANK_COMMON << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
void dbg_na_req( Uint8 no )
{
	sound_req( no * 0x10000 + ( BANK_NA << 8 ) + SEQ_SE ) ;
}

/****************************************************************/
void dbg_song_req( Uint8 no )
{
	song_req( no ) ;
}

/****************************************************************/
void sound_dbg_disp( void )
{
	STrPrint( 5, 20, 7, "sound debug %x", snd_w.req_code ) ;
	STrPrint( 5, 21, 7, "sound debug %d", snd_w.pan ) ;
}

/****************************************************************/
void req_log_disp( void )
{
	int i ;

	for( i = 0 ; i < 20 ; i ++ ) STrPrint( 12, 25 - i, 0, "Sound Req %6x", snd_log.req[i] ) ;
}

/****************************************************************/
void req_log_set( Uint32 code )
{
	int i ;

	for( i = 19 ; i >= 0 ; i -- ) snd_log.req[i + 1] = snd_log.req[i] ;
	snd_log.req[0] = code ;
}

/****************************************************************/
void load_log_disp( void )
{
	int i ;

	for( i = 0 ; i < 10 ; i ++ ) STrPrint( 25, 25 - i, 0, "Sound Load %8x", snd_log.load[i] ) ;
	ver_disp() ;
}

/****************************************************************/
void load_log_set( Sint32 n )
{
	int i ;

	for( i = 9 ; i >= 0 ; i -- ) snd_log.load[i + 1] = snd_log.load[i] ;
	snd_log.load[0] = n ;
}

/****************************************************************/
void ver_disp( void )
{
	char *ver = nlsndVersion() ;

	STrPrint( 25, 27, 0, "%s", ver ) ;
}

/****************************************************************/
/*			Sets which player works share a sound pack			*/
/****************************************************************/
void pack_set( Uint8 *src )
{
	Uint8 *dst = snd_w.pack ;

	*dst++ = *src++ ;
	*dst++ = *src++ ;
	*dst++ = *src++ ;
	*dst++ = *src++ ;
	*dst++ = *src++ ;
	*dst = *src ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sound.c" End of File	*/
