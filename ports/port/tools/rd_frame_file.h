/*
 * rd_frame_file.h  Frames saved with MVC2_RDSAVE (rd_soft.c's save_frame), read back for the tools
 */

#ifndef RD_FRAME_FILE_H
#define RD_FRAME_FILE_H

#include <stdint.h>

/* into rd_frame, rd_fog, rd_palette and the textures */
int rd_frame_load(const char* name);
void rd_frame_write_ppm(const char* name, const uint32_t* frame);

#endif
