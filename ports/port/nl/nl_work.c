/*
 * nl_work.c  nlsystem's work that nlproj.c reads, the host's since both programs share the library
 */

#include "nlsystem.h"

NLSYSWORK NLSystemWork_;
NLSCREEN nl_scr;
Sint32 nl_VGA_flag, nl_PAL_EXT_flag, nl_ext_pal_height;
Uint8 nl_objput_param[0x2c];        /* NLOBJPUTPARAM */
