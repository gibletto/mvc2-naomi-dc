/*
 * rd_gpu.h  The SDL_GPU back end (SDL3): the frame drawn by the GPU, by rd_soft.c's rules.
 * MVC2_GPULAYERS sets the translucent layers (12, 4 with nosort); MVC2_GPUEDGES=0 the GPU's own edges.
 */

#ifndef RD_GPU_H
#define RD_GPU_H

#include <SDL3/SDL_gpu.h>

/* for SDL_CreateGPUDevice: DXBC for Direct3D 12, SPIR-V for Vulkan */
#define RD_GPU_SHADER_FORMATS (SDL_GPU_SHADERFORMAT_DXBC | SDL_GPU_SHADERFORMAT_SPIRV)

bool rd_gpu_init(SDL_GPUDevice* device);
/* the frame drawn by cmd into the back end's texture (R8G8B8A8, the same every frame), in place of
   rd_end_frame */
SDL_GPUTexture* rd_gpu_end_frame(SDL_GPUCommandBuffer* cmd);
/* the last frame as ARGB: waits for the GPU */
bool rd_gpu_read(uint32_t* argb);
void rd_gpu_quit(void);

#endif
