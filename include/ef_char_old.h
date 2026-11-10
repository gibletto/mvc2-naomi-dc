/* ef_char_set as some older modules declared it: the animation number passed as an int, so the caller
   extends it (extu.b) before the call. The routine itself takes bytes. */
#ifndef EF_CHAR_OLD_H
#define EF_CHAR_OLD_H

extern int ef_char_set( PLAYER_WORK *ef_w, Uint8 group, int no );

#endif
