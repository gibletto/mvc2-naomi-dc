/* The loading of the players' data and pictures, and the textures of each scene's models
   (TexLoad), with the start-up's load of the system files. The board has every scene's files in
   memory; the Dreamcast reads them from the disc as each scene starts. */
#include "player.h"
#include "naomi.h"
#include "load.h"

#define SEL_OBJ		( ( OBJ_FILE * )DM00_OBJ_ADRS )	/* the model file of scene 4 */
#define VS_OBJ		( ( OBJ_FILE * )DM06_OBJ_ADRS )	/* the model file of scene 10 */

/* a part of a loaded file, from the table of offsets at its start */
#define FILE_PART( p, n )	( ( void * )&( ( Uint8 * )( p ) )[( ( Sint32 * )( p ) )[n]] )

extern int file_load( int no, void *adrs, int flag );
extern void load_speed_set( int time );
extern void load_init( void );
extern void obj_mat_save( int *obj, Float *col );
extern void obj_mat_load( int *obj, Float *col );
extern void col_set( void *col, int no, int cnt );
#ifdef TARGET_DC
extern void load_side_set( int side );
extern void load_init2( void );
extern void load_stop( void );
extern void pl_data_no_init( int side );
extern void fight_load_start( int no );
extern void adx_pause( int flag );
extern void adx_wait2( void );
#endif

/* by stage_no, the stage's two files (am_load.c loads the board's): the second, and its models */
#ifdef TARGET_DC
Sint32 stg_file_tbl[17] = {
	0x271, 0x273, 0x275, 0x277, 0x279, 0x27b, 0x27d, 0x27f, 0x281,
	0x283, 0x285, 0x287, 0x289, 0x28b, 0x28d, 0x28f, 0x291
} ;
Sint32 stg_obj_tbl[17] = {
	0x270, 0x272, 0x274, 0x276, 0x278, 0x27a, 0x27c, 0x27e, 0x280,
	0x282, 0x284, 0x286, 0x288, 0x28a, 0x28c, 0x28e, 0x290
} ;

/* where each player's picture file was read ahead to, for each of tex_buff's two places; 0 for a
   player whose file is read to its own place */
static Uint8 *pic_keep_tbl[12] = {
	( Uint8 * )0x0cc55000, 0, ( Uint8 * )0x0c800000, 0, 0, 0,
	0, ( Uint8 * )0x0cc55000, 0, ( Uint8 * )0x0cbf0000, 0, 0
} ;
#else
Sint32 stg_file_tbl[9] = { 0x316, 0x318, 0x31a, 0x31c, 0x31e, 0x320, 0x322, 0x324, 0x326 } ;
Sint32 stg_obj_tbl[9] = { 0x315, 0x317, 0x319, 0x31b, 0x31d, 0x31f, 0x321, 0x323, 0x325 } ;
#endif

static Uint8 pl_pic_tbl[3] = { 1, 0, 3 } ;
static Uint8 pl_pic_pos[6] = { 0, 3, 1, 4, 2, 5 } ;

static NLthd tex05_thd[] = {
	{ 0x100, 0x100, 0x100, ( void * )PIC_ADRS, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;

#ifdef TARGET_DC
/* the versus mode's stage pictures: the one shown, and the small ones */
static NLthd selstg_thd[] = {
	{ 0x80, 0x80, 0x101, ( void * )0x0cc60000, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;

static NLthd selstg_s_thd[] = {
	{ 0x40, 0x40, 0x102, ( void * )0x0ccf0000, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;
#endif

static NLthd tex09_thd[] = {
	{ 0x100, 0x100, 0x101, 0, 0 },
	{ 0x100, 0x100, 0x101, 0, 0 },
	{ 0x80, 0x80, 0x102, ( void * )0x200000, 0 },
	{ 0x80, 0x80, 0x102, ( void * )0x208000, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;

static Sint8 pl_pic_no[6] = { 0, 3, 1, 4, 2, 5 } ;

static NLthd tex0e_thd[] = {
	{ 0x80, 0x80, 0x102, ( void * )0x0cc80000, 0 },
	{ -1, -1, -1, ( void * )-1, -1 },
} ;

/****************************************************************/
/*			Load a player's data file to buffer no, and set		*/
/*			up the player from it								*/
/****************************************************************/
void pl_data_load( PLAYER_WORK *pl_w, int no )
{
#ifdef TARGET_DC
	void *prg ;
#else
	Uint8 *chr ;
	Sint16 *src ;
#endif
	Uint8 *adrs ;

	adrs = pl_data_adrs_tbl[no] ;
#ifdef TARGET_DC
	load_side_set( 3 ) ;	/* the file was read ahead */
#else
	file_load2( pl_file_tbl[pl_w->load_type], adrs, 1 ) ;
#endif
	pl_w->pl_data_adrs = ( Sint32 * )adrs ;
	pl_w->cell_adrs = FILE_PART( adrs, 0 ) ;
	pl_w->spr_adrs = FILE_PART( adrs, 1 ) ;
	pl_w->col_adrs = FILE_PART( adrs, 2 ) ;
	pl_w->catch_pos_tbl = FILE_PART( adrs, 3 ) ;
	pl_w->ef_pos_tbl = FILE_PART( adrs, 4 ) ;
	pl_w->spr_set_buff = spr_set_buff_tbl[no] ;
	pl_w->col_flag = 1 ;
	pl_w->col_no = col_no_tbl[pl_w->pl_index] ;
	pl_w->col_cnt = col_cnt_tbl[pl_w->load_type] ;
	pl_w->char_ofs = 0x100 ;
	if( pl_w->load_type == 0x34 ) pl_w->char_ofs = 0x200 ;
	col_set( ( Uint16 * )pl_w->col_adrs + pl_w->load_color * 0x80, pl_w->col_no, 8 ) ;
#ifdef TARGET_DC
	/* the disc's file has the fight's data too, unpacked; the character's program is a file */
	pl_w->frame_box_tbl = FILE_PART( adrs, 6 ) ;
	pl_w->hit_box_tbl = FILE_PART( adrs, 7 ) ;
	pl_w->hit_data_tbl = FILE_PART( adrs, 8 ) ;
	pl_w->char_adrs = FILE_PART( adrs, 5 ) ;
	pl_w->cpu_adrs[0] = FILE_PART( adrs, 9 ) ;
	pl_w->cpu_adrs[1] = FILE_PART( adrs, 10 ) ;
	pl_w->cpu_answer_adrs = FILE_PART( adrs, 12 ) ;
	pl_w->cpu_adrs[2] = FILE_PART( adrs, 11 ) ;
	pl_w->cpu_ex_adrs = FILE_PART( adrs, 13 ) ;
	prg = ( void * )( S_PL_ADRS + pl_w->pl_index * 0x8000 ) ;
	pl_w->prg_adrs = prg ;
	file_load2( FILE_S_PL + pl_w->pl_index * 59 + pl_w->load_type, prg, 1 ) ;
#else
	src = FILE_PART( 0x0dd00000, pl_w->load_type ) ;
	lz_unpack16( src, ( Uint16 * )( chr = adrs + 0x130000 ) ) ;
	pl_w->char_adrs = FILE_PART( chr, 0 ) ;
	pl_w->frame_box_tbl = FILE_PART( chr, 1 ) ;
	pl_w->hit_box_tbl = FILE_PART( chr, 2 ) ;
	pl_w->hit_data_tbl = FILE_PART( chr, 3 ) ;
	pl_w->cpu_adrs[0] = FILE_PART( chr, 4 ) ;
	pl_w->cpu_adrs[1] = FILE_PART( chr, 5 ) ;
	pl_w->cpu_answer_adrs = FILE_PART( chr, 7 ) ;
	pl_w->cpu_adrs[2] = FILE_PART( chr, 6 ) ;
	pl_w->cpu_ex_adrs = FILE_PART( chr, 8 ) ;
#endif
}

/****************************************************************/
/*			Empty											*/
/****************************************************************/
void tex_dummy_0()
{
}

/****************************************************************/
void tex_dummy_1( void )
{
}

/****************************************************************/
/*			The home game's "now loading" picture, one of		*/
/*			four: read, unpacked and made a texture (the		*/
/*			board's five routines from here are empty)			*/
/****************************************************************/
void rand_pic_load( void )
{
#ifdef TARGET_DC
	NLthd *thd ;
	int i ;

	file_load( FILE_NOWLOAD + ( ( Uint8 )nlRand() & 3 ), ( void * )TEX_ADRS, 1 ) ;
	lz_unpack16( ( Sint16 * )TEX_ADRS, ( Uint16 * )PIC_ADRS ) ;
	thd = ( NLthd * )PIC_ADRS ;
	for( i = 0 ; ( Sint32 )thd[i].address > 0 ; i ++ ){
		thd[i].address = ( Uint8 * )thd[i].address + PIC_ADRS ;
	}
	cache_purge( ( void * )PIC_ADRS, 0x20000 ) ;
	nlSetCommonTextureOffset( 12 ) ;
	nlSpriteLoadTexture( ( NLthd * )PIC_ADRS, 0 ) ;
	nlSetCommonTextureOffset( 0 ) ;
#endif
}

/****************************************************************/
/*			The same, from the pack kept in memory				*/
/****************************************************************/
void rand_pic_set( void )
{
#ifdef TARGET_DC
	NLthd *thd ;
	int i ;

	lz_unpack16( ( Sint16 * )NOWLOAD_KEEP_ADRS, ( Uint16 * )PIC_ADRS ) ;
	thd = ( NLthd * )PIC_ADRS ;
	for( i = 0 ; ( Sint32 )thd[i].address > 0 ; i ++ ){
		thd[i].address = ( Uint8 * )thd[i].address + PIC_ADRS ;
	}
	cache_purge( ( void * )PIC_ADRS, 0x20000 ) ;
	nlSetCommonTextureOffset( 13 ) ;
	nlSpriteLoadTexture( ( NLthd * )PIC_ADRS, 0 ) ;
	nlSetCommonTextureOffset( 0 ) ;
#endif
}

/****************************************************************/
/*			Read the kana-kanji engine							*/
/****************************************************************/
void ask_file_load( void )
{
#ifdef TARGET_DC
	file_load( FILE_ASK, ( void * )ASK_ADRS, 1 ) ;
#endif
}

/****************************************************************/
/*			Read and unpack the keyboard's pictures				*/
/****************************************************************/
void keyboard_tex_load( void )
{
#ifdef TARGET_DC
	NLthd *thd ;
	int i ;

	file_load( FILE_KEYBOARD, ( void * )PIC_ADRS, 1 ) ;
	lz_unpack16( ( Sint16 * )PIC_ADRS, ( Uint16 * )TEX_ADRS ) ;
	thd = ( NLthd * )TEX_ADRS ;
	for( i = 0 ; ( Sint32 )thd[i].address > 0 ; i ++ ){
		thd[i].address = ( Uint8 * )thd[i].address + TEX_ADRS ;
	}
#endif
}

/****************************************************************/
/*			And make them textures								*/
/****************************************************************/
void keyboard_tex_set( void )
{
#ifdef TARGET_DC
	cache_purge( ( void * )TEX_ADRS, 0x200000 ) ;
	nlSetCommonTextureOffset( 0x850 ) ;
	nlSpriteLoadTexture( ( NLthd * )TEX_ADRS, 0 ) ;
	nlSetCommonTextureOffset( 0 ) ;
#endif
}

/****************************************************************/
void tex_load_stage( void )
{
	int i ;

	nlSetCommonTextureOffset( 0xc10 ) ;
	obj_stg = ( OBJ_FILE * )STG_OBJ_ADRS ;
	for( i = 0 ; i < obj_stg->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_stg->obj[i], obj_stg->thd ) ;
	}
}

/****************************************************************/
/*			Unpack a picture into texture no of a model file	*/
/****************************************************************/
void obj_tex_unpack( int no, Sint16 *src, OBJ_FILE *of )
{
	NLthd *thd ;
	Uint16 *dst ;

	thd = of->thd ;
	dst = thd[no].address ;
	lz_unpack16( src, dst ) ;
}

#if ROM_BOARD != 2	/* the M2 board reads its model files to where they were built to sit */
/****************************************************************/
/*			Move a model file's pointers from where it was		*/
/*			built to where it was loaded, with its textures'	*/
/*			data at adrs										*/
/****************************************************************/
void obj_file_reloc( void *data, Uint8 *adrs )
{
	Sint32 *p = data ;
	OBJ_FILE *of ;
	Sint32 ofs ;
	NLthd *thd ;
	Sint32 *tbl ;
	int i ;

	ofs = p[0] - ( Sint32 )data - 16 ;	/* the file was built to sit at p[0] - 16 */
	p[0] -= ofs ;
	p[2] -= ofs ;
	of = data ;
	tbl = ( Sint32 * )p[0] ;
	for( i = 0 ; i < of->cnt ; i ++ ) tbl[i] -= ofs ;
	thd = of->thd ;
	ofs = ( Sint32 )thd->address - ( Sint32 )adrs ;
	while( thd->width ){
		thd->address = ( Uint8 * )thd->address - ofs ;
		thd ++ ;
	}
}
#endif

/****************************************************************/
/*			Load the textures of a model file					*/
/****************************************************************/
void obj_tex_load( Sint32 ofs, OBJ_FILE **ofp, OBJ_FILE *of )
{
	int i ;

	nlSetCommonTextureOffset( ofs ) ;
	*ofp = of ;
	for( i = 0 ; i < ( *ofp )->cnt ; i ++ ){
		nlObjPutLoadTexture( ( *ofp )->obj[i], ( *ofp )->thd ) ;
	}
}

/****************************************************************/
void tex_load_select( void )
{
	Sys->tex_buff = ( Uint8 * )TEX_BUFF0 ;
#ifdef TARGET_DC
	/* the pictures and the models were read ahead */
	switch( Sys->play_side ){
	case 0:
		if( Sys->new_side == 1 ) break ;
	case 1:
		Sys->tex_buff = ( Uint8 * )TEX_BUFF1 ;
		break ;
	}
	lz_unpack16( ( Sint16 * )TEX_ADRS, ( Uint16 * )Sys->tex_buff ) ;
	memcpy( ( void * )DM01_OBJ_ADRS, ( void * )DM01_KEEP_ADRS, DM01_KEEP_SIZE ) ;
	file_load( FILE_TEX( FILE_DM01POL ), ( void * )TEX_ADRS, 0 ) ;
#else
	if( Sys->play_side == 1 ) Sys->tex_buff = ( Uint8 * )TEX_BUFF1 ;
	file_load( FILE_SELTEX, ( void * )TEX_ADRS, 0 ) ;
	lz_unpack16( ( Sint16 * )TEX_ADRS, ( Uint16 * )Sys->tex_buff ) ;
	file_load( FILE_DM01POL, ( void * )DM01_OBJ_ADRS, 0 ) ;
#endif
	if( Sys->cpu_level_type ) file_load( FILE_SELVMU, ( void * )PIC_ADRS, 0 ) ;
	else file_load( FILE_SELVMJ, ( void * )PIC_ADRS, 0 ) ;
}

/****************************************************************/
/*			Load each player's picture file						*/
/****************************************************************/
void pl_pic_load( void )
{
	PLAYER_WORK *pl_w ;
	int i ;
#ifdef TARGET_DC
	int n ;
#endif

	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
#ifdef TARGET_DC
		/* the files were read ahead: the ones that could not go to their own place are copied */
		pl_w->pic_adrs = pic_adrs_tbl[pl_w->pic_buff_no] ;
		n = i ;
		if( Sys->tex_buff == ( Uint8 * )TEX_BUFF1 ) n += 6 ;
		if( pic_keep_tbl[n] ) memcpy( pl_w->pic_adrs, pic_keep_tbl[n], 0x8000 ) ;
#else
		pl_w->pic_adrs = pic_adrs_tbl[i] ;
		file_load2( pic_file_tbl[pl_w->load_type], pl_w->pic_adrs, 1 ) ;
#endif
	}
}

/****************************************************************/
void tex_load_st_demo( void )
{
#ifdef TARGET_DC
	memcpy( ( void * )DM02_OBJ_ADRS, ( void * )DM02_KEEP_ADRS, DM02_KEEP_SIZE ) ;
#endif
	pl_pic_load() ;
}

/****************************************************************/
void tex_load_staff( void )
{
	Sys->tex_buff = ( Uint8 * )TEX_BUFF1 ;
	if( Sys->play_side == 1 ) Sys->tex_buff = ( Uint8 * )TEX_BUFF0 ;
	file_load( FILE_ENDTEX, ( void * )TEX_ADRS, 0 ) ;
	lz_unpack16( ( Sint16 * )TEX_ADRS, ( Uint16 * )Sys->tex_buff ) ;
#ifdef TARGET_DC
	file_load( FILE_DM05POL, ( void * )DM05_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM05POL ), ( void * )TEX_ADRS, 0 ) ;
#endif
}

/****************************************************************/
/* load the pictures of the side's team; em_w is unused */
void tex_load_ranking( void )
{
#ifdef TARGET_DC
	PLAYER_WORK *pl_w ;
	Sint32 no, tex ;
	Sint32 pic ;
	Sint16 *src ;
	Uint16 *dst ;
	int side ;
	int i ;

	/* the home game has a ranking for each of its modes */
	switch( Sys->rank_disp ){
	case 0:
		no = FILE_DM06POL ;
		tex = FILE_TEX( no ) ;
		break ;
	case 1:
		no = FILE_DM13POL ;
		tex = FILE_TEX( no ) ;
		break ;
	case 2:
		no = FILE_DM14POL ;
		tex = FILE_TEX( no ) ;
		break ;
	}
	file_load( no, ( void * )DM06_OBJ_ADRS, 0 ) ;
	file_load( tex, ( void * )TEX_ADRS, 0 ) ;
	side = Sys->rank_side ;
	if( side ^ 1 ) Sys->tex_buff = ( Uint8 * )TEX_BUFF1 ;
	else Sys->tex_buff = ( Uint8 * )TEX_BUFF0 ;
	pl_w = vs_sys_work.team_w[side][0] ;
	Sys->rank_char = pl_w->load_type ;
	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = vs_sys_work.team_w[side][i] ;
		file_load2( vs_file_tbl[pl_w->load_type], ( void * )( VS_PIC_ADRS + i * 0x10000 ), 1 ) ;
	}
	pic = 0x24000 ;
	if( Sys->rank_disp == 1 ) pic = 0x2c000 ;
	if( Sys->rank_disp == 2 ) pic = 0x24000 ;
	src = ( Sint16 * )( TEX_ADRS + pic ) ;
	dst = ( Uint16 * )Sys->tex_buff ;
	lz_unpack16( src, dst ) ;
#else
	PLAYER_WORK *pl_w, *em_w ;
	int side ;
	int i ;

	file_load( FILE_TEX( FILE_DM06POL ), ( void * )DM06_TEX_ADRS, 0 ) ;
	side = Sys->rank_side ;
	if( side ^ 1 ) Sys->tex_buff = ( Uint8 * )TEX_BUFF1 ;
	else Sys->tex_buff = ( Uint8 * )TEX_BUFF0 ;
	pl_w = vs_sys_work.team_w[side][0] ;
	Sys->rank_char = pl_w->load_type ;
	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = vs_sys_work.team_w[side][i] ;
		em_w = vs_sys_work.team_w[side ^ 1][i] ;
		file_load2( vs_file_tbl[pl_w->load_type], pl_data_adrs_tbl[i] + 0x130000, 1 ) ;
	}
	lz_unpack16( ( Sint16 * )( DM06_TEX_ADRS + 0x36000 ), ( Uint16 * )Sys->tex_buff ) ;
#endif
}

/****************************************************************/
/* the board's alone: the opening's pictures */
void tex_load_opening( void )
{
#ifndef TARGET_DC
	const FILE_DATA *ft ;
	char *dst ;

	file_load( FILE_DM08CAB, ( void * )0x0c420000, 0 ) ;
	ft = &stream_tbl[141] ;
	dst = ( char * )0x0c520000 ;
	rom_dma_flag = 1 ;
	cache_inv( dst, 0x200000 ) ;
	nlaReadROMDMA( ft->adrs, dst, 0x200000, 1 ) ;
	rom_dma_flag = 0 ;
#endif
}


/****************************************************************/
/*			Unpack the texture of scene 14's model, and			*/
/*			point the model at it								*/
/****************************************************************/
void tex_load_0e( void )
{
#ifdef TARGET_DC
	/* the model is English or Japanese */
	if( !Sys->cpu_level_type ){
		file_load( FILE_DM0APOL, ( void * )DM0A_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0APOL ), ( void * )DM0A_TEX_ADRS, 0 ) ;
	} else {
		file_load( FILE_DM0EPOL, ( void * )DM0A_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0EPOL ), ( void * )DM0A_TEX_ADRS, 0 ) ;
	}
	lz_unpack16( ( Sint16 * )DM0A_TEX_ADRS, ( Uint16 * )TEX_ADRS ) ;
#else
	Uint16 *dst ;

	if( 0 ) *( Uint16 ** )( DM0A_OBJ_ADRS + 0x20 ) = dst ;
	dst = ( Uint16 * )TEX_ADRS ;
	if( Sys->cpu_level_type == 0 ){ }	/* tested, and nothing done either way */
	*( Uint16 ** )( DM0A_OBJ_ADRS + 0x20 ) = dst ;
	lz_unpack16( ( Sint16 * )DM0A_TEX_ADRS, dst ) ;
#endif
}

/****************************************************************/
/*			Unpack each player's picture into the textures,		*/
/*			and load the model file's							*/
/****************************************************************/
void tex_load_gauge( void )
{
	PLAYER_WORK *pl_w ;
	NLthd *thd ;
	Uint32 *src, *dst ;
	void *pic ;
	int i, j, n ;

	obj_dm00 = SEL_OBJ ;
	thd = SEL_OBJ->thd ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		if( !Sys->cpu_level_type ){
			pic = FILE_PART( pl_w->pic_adrs, 0 ) ;
		} else {
			n = 0 ;
			if( pl_w->load_type < 32 ){
				if( ( 1 << pl_w->load_type ) & 0xd0010000 ) n = 3 ;
			} else {
				if( ( 1 << ( pl_w->load_type - 32 ) ) & 0x04000042 ) n = 3 ;
			}
			pic = FILE_PART( pl_w->pic_adrs, n ) ;
		}
		lz_unpack16( pic, ( Uint16 * )PIC_ADRS ) ;
		dst = thd[pl_pic_pos[i] + 10].address ;
		src = ( Uint32 * )( PIC_ADRS + pl_pic_tbl[pl_w->pl_personal] * 0x800 ) ;
		for( j = 0x200 ; j ; j -- ) *dst ++ = *src ++ ;
		dst = thd[pl_pic_pos[i] + 16].address ;
		src = ( Uint32 * )( PIC_ADRS + 0x1000 ) ;
		for( j = 0x200 ; j ; j -- ) *dst ++ = *src ++ ;
	}
	nlSetCommonTextureOffset( 0xc90 ) ;
	for( i = 0 ; i < obj_dm00->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm00->obj[i], obj_dm00->thd ) ;
	}
}

/****************************************************************/
void tex_set_select( void )
{
	int i ;

	obj_dm01 = ( OBJ_FILE * )DM01_OBJ_ADRS ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm01->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm01->obj[i], obj_dm01->thd ) ;
	}
	nlSetCommonTextureOffset( 0x910 ) ;
	nlSpriteLoadTexture( tex05_thd, 0 ) ;
#ifdef TARGET_DC
	/* the versus mode chooses its stage: the stages' pictures */
	if( Sys->home_mode != HOME_VERSUS ) return ;
	file_load( FILE_SELSTG, ( void * )0x0ccf8000, 0 ) ;
	lz_unpack16( ( Sint16 * )0x0ccf8000, ( Uint16 * )0x0cc60000 ) ;
	nlSetCommonTextureOffset( 0 ) ;
	nlDispStripLoadTexture( 11, selstg_s_thd ) ;
	nlSetCommonTextureOffset( 0x914 ) ;
	nlSpriteLoadTextureOnce( selstg_thd, 0, 0 ) ;
#endif
}

/****************************************************************/
void tex_set_staff( void )
{
	int i ;

	obj_dm05 = ( OBJ_FILE * )DM05_OBJ_ADRS ;
	obj_dm07 = ( OBJ_FILE * )DM05_OBJ_ADRS ;
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm05->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm05->obj[i], obj_dm05->thd ) ;
	}
	tex09_thd[0].address = Sys->tex_buff ;
	tex09_thd[1].address = Sys->tex_buff ;
	tex09_thd[2].address = Sys->tex_buff + 0x200000 ;
	tex09_thd[3].address = Sys->tex_buff + 0x208000 ;
	nlSetCommonTextureOffset( 0x910 ) ;
	nlSpriteLoadTexture( tex09_thd, 0 ) ;
}

/****************************************************************/
void tex_set_st_demo( void )
{
	PLAYER_WORK *pl_w ;
	void *pic ;
	int i ;

	obj_dm02 = ( OBJ_FILE * )DM02_OBJ_ADRS ;
	for( i = 0 ; i < 6 ; i ++ ){
		pl_w = &pl_work[i] ;
		pic = FILE_PART( pl_w->pic_adrs, 2 ) ;
		obj_tex_unpack( pl_pic_no[i] + 2, pic, obj_dm02 ) ;
	}
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm02->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm02->obj[i], obj_dm02->thd ) ;
	}
}

/****************************************************************/
/* copy the versus model's texture headers to the buffer, and build its pictures there */
void tex_set_ranking( void )
{
	static const Uint8 score_pos_tbl[16] = {	/* the score's cells, by digit */
		0x05, 0x07, 0x0d, 0x0f, 0x04, 0x06, 0x0c, 0x0e, 0x01, 0x03, 0x09, 0x0b, 0x00, 0x02, 0x08, 0x0a
	} ;
	NLthd *src, *dst, *thd ;
	PLAYER_WORK *pl_w ;
	int side ;
	int i ;
#ifdef TARGET_DC
	int no ;
#endif

	obj_dm06 = VS_OBJ ;
	thd = ( NLthd * )( Sys->tex_buff + 0x14000 ) ;
	src = VS_OBJ->thd ;
	dst = thd ;
	while( src->width ) *dst ++ = *src ++ ;
	*dst = *src ;
	side = Sys->rank_side ;
#ifdef TARGET_DC
	/* each mode's model has the team's pictures and the score at its own textures */
	no = 9 ;
	if( Sys->rank_disp == 1 ) no = 13 ;
	if( Sys->rank_disp == 2 ) no = 9 ;
	if( 0 ) no ++ ;	/* a build setting: the pictures one texture on */
	for( i = 0 ; i < 3 ; i ++ ){
		obj_tex_unpack( i + no, ( Sint16 * )( VS_PIC_ADRS + i * 0x10000 ), obj_dm06 ) ;
	}
	pl_w = &pl_work[side] ;
	if( Sys->rank_disp == 1 ){
		win_tex_set( pl_w->win_max, thd ) ;
	} else {
		if( !Sys->rank_disp ) no = 8 ;
		else no = 8 ;
		score_tex_set( pl_w->old_score_hi, pl_w->old_score, thd[no].address, score_pos_tbl ) ;
		memset( ( Uint8 * )thd[no].address + 0x1600, 0, 0x200 ) ;
	}
#else
	for( i = 0 ; i < 3 ; i ++ ){
		pl_w = vs_sys_work.team_w[side][i] ;	/* unused: left from tex_load_ranking's loop */
		obj_tex_unpack( i + 17, ( Sint16 * )( pl_data_adrs_tbl[i] + 0x130000 ), obj_dm06 ) ;
	}
	pl_w = &pl_work[side] ;
	if( Sys->vs_flag ){
		win_tex_set( pl_w->win_max, thd ) ;
	} else {
		score_tex_set( pl_w->old_score_hi, pl_w->old_score, thd[16].address, score_pos_tbl ) ;
		memset( ( Uint8 * )thd[16].address + 0x1600, 0, 0x200 ) ;
	}
	if( !Sys->vs_flag ) thd[5].address = Sys->tex_buff + 0x10000 ;
#endif
	nlSetCommonTextureOffset( 0x810 ) ;
	for( i = 0 ; i < obj_dm06->cnt ; i ++ ){
		nlObjPutLoadTexture( obj_dm06->obj[i], thd ) ;
	}
}

/****************************************************************/
void tex_set_0e( void )
{
	obj_tex_load( 0xd10, &obj_dm0a, ( OBJ_FILE * )DM0A_OBJ_ADRS ) ;
	nlSetCommonTextureOffset( 0xd50 ) ;
	nlSpriteLoadTexture( tex0e_thd, 0 ) ;
}

void obj_col_get( OBJ_FILE *of ) ;

/****************************************************************/
/*			Load the textures of scene no: on the Dreamcast,	*/
/*			after reading the scene's files, or copying the		*/
/*			ones read ahead										*/
/****************************************************************/
void TexLoad( int no )
{
	switch( no ){
	case 1:
#ifdef TARGET_DC
		file_load( stg_file_tbl[stage_no], ( void * )TEX_ADRS, 0 ) ;
#endif
		tex_load_stage() ;
		break ;
	case 2:
#ifdef TARGET_DC
		obj_file_reloc( ( OBJ_FILE * )EFKY_OBJ_ADRS, ( Uint8 * )EFKY_TEX_ADRS ) ;
#endif
		obj_tex_load( 0xc50, &obj_efky, ( OBJ_FILE * )EFKY_OBJ_ADRS ) ;
		break ;
	case 4:
#ifdef TARGET_DC
		memcpy( ( void * )DM00_OBJ_ADRS, ( void * )DM00_KEEP_ADRS, DM00_KEEP_SIZE ) ;
		obj_file_reloc( ( OBJ_FILE * )DM00_OBJ_ADRS, ( Uint8 * )DM00_TEX_ADRS ) ;
#endif
		tex_load_gauge() ;
		obj_col_get( obj_dm00 ) ;
		break ;
	case 5:
		tex_load_select() ;
		tex_set_select() ;
		break ;
	case 6:
		tex_load_st_demo() ;
		tex_set_st_demo() ;
		break ;
	case 8:
		cache_purge( ( void * )DM04_OBJ_ADRS, 0xa000 ) ;
		obj_tex_load( 0xd10, &obj_dm04, ( OBJ_FILE * )DM04_OBJ_ADRS ) ;
		break ;
	case 9:
		tex_load_staff() ;
		tex_set_staff() ;
		break ;
	case 10:
		tex_load_ranking() ;
		tex_set_ranking() ;
		break ;
	case 11:
#ifdef TARGET_DC
		file_load( FILE_DM07POL, ( void * )DM07_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM07POL ), ( void * )TEX_ADRS, 0 ) ;
#endif
		obj_tex_load( 0x810, &obj_dm07, ( OBJ_FILE * )DM07_OBJ_ADRS ) ;
		break ;
	case 12:
#ifdef TARGET_DC
		memcpy( ( void * )DM08_OBJ_ADRS, ( void * )DM08_KEEP_ADRS, DM08_KEEP_SIZE ) ;
#endif
		obj_tex_load( 0x810, &obj_dm08, ( OBJ_FILE * )DM08_OBJ_ADRS ) ;
		tex_load_opening() ;
		break ;
	case 13:
#ifdef TARGET_DC
		memcpy( ( void * )DM09_OBJ_ADRS, ( void * )DM09_KEEP_ADRS, DM09_KEEP_SIZE ) ;
#endif
		obj_tex_load( 0x810, &obj_dm09, ( OBJ_FILE * )DM09_OBJ_ADRS ) ;
		break ;
	case 14:
		tex_load_0e() ;
		tex_set_0e() ;
		break ;
	case 15:
#ifdef TARGET_DC
		file_load( FILE_DM0BPOL, ( void * )DM0B_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0BPOL ), ( void * )TEX_ADRS, 0 ) ;
#endif
		obj_tex_load( 0x850, &obj_dm0b, ( OBJ_FILE * )DM0B_OBJ_ADRS ) ;
		break ;
#ifdef TARGET_DC
	/***** the home game's scenes *****/
	case 16:
		if( Sys->adx_pause_flag ) adx_pause( 0 ) ;
		adx_wait2() ;
		file_load( FILE_DM0CPOL, ( void * )SCENE_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0CPOL ), ( void * )TEX_ADRS, 0 ) ;
		obj_tex_load( 0x850, &obj_dm0c, ( OBJ_FILE * )SCENE_OBJ_ADRS ) ;
		load_stop() ;
		fight_load_start( 8 ) ;
		break ;
	case 17:
		adx_pause( 1 ) ;
		file_load( FILE_DM0DPOL, ( void * )SCENE_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0DPOL ), ( void * )TEX_BUFF0, 0 ) ;
		obj_tex_load( 0x860, &obj_dm0d, ( OBJ_FILE * )SCENE_OBJ_ADRS ) ;
		keyboard_tex_load() ;
		keyboard_tex_set() ;
		rand_pic_set() ;
		ask_file_load() ;
		adx_pause( 0 ) ;
		break ;
	case 18:
		file_load( FILE_DM0FPOL, ( void * )SCENE_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0FPOL ), ( void * )TEX_ADRS, 0 ) ;
		obj_tex_load( 0x850, &obj_dm0f, ( OBJ_FILE * )SCENE_OBJ_ADRS ) ;
		break ;
	case 19:
		file_load( FILE_DM10POL, ( void * )SCENE_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM10POL ), ( void * )TEX_ADRS, 0 ) ;
		obj_tex_load( 0x850, &obj_dm10, ( OBJ_FILE * )SCENE_OBJ_ADRS ) ;
		break ;
	case 20:
		file_load( FILE_DM11POL, ( void * )SCENE_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM11POL ), ( void * )TEX_ADRS, 0 ) ;
		obj_tex_load( 0x850, &obj_dm11, ( OBJ_FILE * )SCENE_OBJ_ADRS ) ;
		break ;
	case 21:
		file_load( FILE_DM12POL, ( void * )SCENE_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM12POL ), ( void * )TEX_ADRS, 0 ) ;
		obj_tex_load( 0x850, &obj_dm12, ( OBJ_FILE * )SCENE_OBJ_ADRS ) ;
		break ;
	/***** the stage, the effects and the ending's models again, for the attract and the ending *****/
	case 23:
		file_load( stg_obj_tbl[stage_no], ( void * )STG_OBJ_ADRS, 0 ) ;
		tex_load_stage() ;
		break ;
	case 24:
		obj_file_reloc( ( OBJ_FILE * )EFKY_OBJ_ADRS, ( Uint8 * )EFKY_TEX2_ADRS ) ;
		obj_tex_load( 0xc50, &obj_efky, ( OBJ_FILE * )EFKY_OBJ_ADRS ) ;
		break ;
	case 25:
		memcpy( ( void * )DM07_OBJ_ADRS, ( void * )DM07_KEEP_ADRS, DM07_KEEP_SIZE ) ;
		obj_tex_load( 0x810, &obj_dm07, ( OBJ_FILE * )DM07_OBJ_ADRS ) ;
		break ;
#endif
	}
}

/****************************************************************/
/*			Fetch the colours of a model file's first four		*/
/*			objects												*/
/****************************************************************/
void obj_col_get( OBJ_FILE *of )
{
	int i ;

	for( i = 0 ; i <= 3 ; i ++ ) obj_mat_save( of->obj[i], obj_col_buff[i] ) ;
}

/****************************************************************/
/*			Give an object colour no							*/
/****************************************************************/
void obj_col_set( int *obj, int no )
{
	obj_mat_load( obj, obj_col_buff[no] ) ;
}

/****************************************************************/
/* at start-up: time the ROM's reads, then load the system's files (the Dreamcast times a copy
   and loads nothing yet) */
void sys_data_load( void )
{
	Sint32 total ;
	Sint32 time ;
	int i ;

	total = 0 ;
	for( i = 0 ; i < 100 ; i ++ ){
#ifdef TARGET_DC
		time = nlSysMetricsCpuPowerGet() ;
#ifndef HOST_PORT	/* the texture memory, which a host build does not map */
		memcpy( ( void * )TEX_ADRS, ( void * )0xc4200000, 0x1000 ) ;
#endif
		total += nlSysMetricsCpuPowerGet() - time ;
#else
		while( !nlaCheckWaitROMDMA() ) ;
		rom_dma_flag = 1 ;
		time = nlSysMetricsCpuPowerGet() ;
		cache_purge( ( void * )EFKY_OBJ_ADRS, 0x800 ) ;
		nlaReadROMDMA( 0x600000, ( char * )EFKY_OBJ_ADRS, 0x800, 0 ) ;
		cache_inv( ( void * )EFKY_OBJ_ADRS, 0x800 ) ;
		total += nlSysMetricsCpuPowerGet() - time ;
		rom_dma_flag = 0 ;
#endif
	}
	load_speed_set( total / 100 ) ;
#ifdef TARGET_DC
	load_init() ;
	load_init2() ;
	pl_data_no_init( 0 ) ;
	pl_data_no_init( 1 ) ;
#else
	file_load( FILE_EFKYPOL, ( void * )EFKY_OBJ_ADRS, 0 ) ;
#if ROM_BOARD != 2
	obj_file_reloc( ( OBJ_FILE * )EFKY_OBJ_ADRS, ( Uint8 * )EFKY_TEX_ADRS ) ;
#endif
	file_load( FILE_TEX( FILE_EFKYPOL ), ( void * )EFKY_TEX_ADRS, 0 ) ;
	file_load( FILE_DM00POL, ( void * )DM00_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM00POL ), ( void * )DM00_TEX_ADRS, 0 ) ;
	file_load( FILE_DM01POL, ( void * )DM01_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM01POL ), ( void * )DM01_TEX_ADRS, 0 ) ;
	file_load( FILE_DM02POL, ( void * )DM02_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM02POL ), ( void * )DM02_TEX_ADRS, 0 ) ;
	file_load( FILE_DM04POL, ( void * )DM04_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM04POL ), ( void * )DM04_TEX_ADRS, 0 ) ;
	file_load( FILE_DM06POL, ( void * )DM06_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM06POL ), ( void * )DM06_TEX_ADRS, 0 ) ;
	file_load( FILE_DM07POL, ( void * )DM07_OBJ_ADRS, 0 ) ;
#if ROM_BOARD != 2
	obj_file_reloc( ( OBJ_FILE * )DM07_OBJ_ADRS, ( Uint8 * )DM07_TEX_ADRS ) ;
#endif
	file_load( FILE_TEX( FILE_DM07POL ), ( void * )DM07_TEX_ADRS, 0 ) ;
	file_load( FILE_DM08POL, ( void * )DM08_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM08POL ), ( void * )DM08_TEX_ADRS, 0 ) ;
	file_load( FILE_DM09POL, ( void * )DM09_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM09POL ), ( void * )DM09_TEX_ADRS, 0 ) ;
	if( !Sys->cpu_level_type ){
		file_load( FILE_DM0APOL, ( void * )DM0A_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0APOL ), ( void * )DM0A_TEX_ADRS, 0 ) ;
	} else {
		file_load( FILE_DM0EPOL, ( void * )DM0A_OBJ_ADRS, 0 ) ;
		file_load( FILE_TEX( FILE_DM0EPOL ), ( void * )DM0A_TEX_ADRS, 0 ) ;
	}
	file_load( FILE_DM0BPOL, ( void * )DM0B_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM0BPOL ), ( void * )DM0B_TEX_ADRS, 0 ) ;
	file_load( FILE_DM05POL, ( void * )DM05_OBJ_ADRS, 0 ) ;
	file_load( FILE_TEX( FILE_DM05POL ), ( void * )DM05_TEX_ADRS, 0 ) ;
	load_init() ;
#endif
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "tex_load.c" End of File	*/
