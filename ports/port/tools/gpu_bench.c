/*
 * gpu_bench.c  A frame saved with MVC2_RDSAVE, drawn again and again by the SDL_GPU back end
 *
 * gpu_bench FILE [TIMES] [OUT.ppm] [-w]: prints the best time a frame and writes the last one; -w shows
 * it in a window. SDL_GPU_DRIVER picks the driver, MVC2_GPUDEBUG=1 its debug layer. Built by hand:
 *   gcc -O2 -DRD_SDL3 -Iport/render port/tools/gpu_bench.c port/tools/rd_frame_file.c port/render/rd.c
 *       port/render/rd_gpu.c $(pkg-config --cflags --libs sdl3) -o gpu_bench
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "rd.h"
#include "rd_gpu.h"
#include "rd_list.h"
#include "rd_frame_file.h"

static uint32_t frame[RD_WIDTH * RD_HEIGHT];

static void show(SDL_GPUDevice* dev, SDL_Window* win, SDL_GPUTexture* tex) {
    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(dev);
    SDL_GPUTexture* sc;
    Uint32 w, h;
    if (SDL_WaitAndAcquireGPUSwapchainTexture(cmd, win, &sc, &w, &h) && sc != NULL) {
        SDL_GPUBlitInfo b;
        memset(&b, 0, sizeof b);
        b.source.texture = tex;
        b.source.w = RD_WIDTH, b.source.h = RD_HEIGHT;
        b.destination.texture = sc;
        b.destination.w = w, b.destination.h = h;
        b.load_op = SDL_GPU_LOADOP_DONT_CARE;
        b.filter = SDL_GPU_FILTER_NEAREST;
        SDL_BlitGPUTexture(cmd, &b);
    }
    SDL_SubmitGPUCommandBuffer(cmd);
}

int main(int argc, char** argv) {
    int times = argc > 2 ? atoi(argv[2]) : 50, i, window = 0;
    const char* out = NULL;
    SDL_GPUDevice* dev;
    SDL_Window* win = NULL;
    SDL_GPUTexture* tex = NULL;
    double best = 1e9;
    for (i = 3; i < argc; i++) {
        if (strcmp(argv[i], "-w") == 0) {
            window = 1;
        } else {
            out = argv[i];
        }
    }
    if (argc < 2 || !rd_frame_load(argv[1])) {
        fprintf(stderr, "gpu_bench FILE [TIMES] [OUT.ppm] [-w]: no frame\n");
        return 1;
    }
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    dev = SDL_CreateGPUDevice(RD_GPU_SHADER_FORMATS, getenv("MVC2_GPUDEBUG") != NULL, NULL);
    if (dev == NULL) {
        fprintf(stderr, "SDL_CreateGPUDevice: %s\n", SDL_GetError());
        return 1;
    }
    printf("driver %s\n", SDL_GetGPUDeviceDriver(dev));
    if (window) {
        win = SDL_CreateWindow("gpu_bench", RD_WIDTH, RD_HEIGHT, 0);
        if (win == NULL || !SDL_ClaimWindowForGPUDevice(dev, win)) {
            fprintf(stderr, "window: %s\n", SDL_GetError());
            return 1;
        }
    }
    if (!rd_gpu_init(dev)) {
        return 1;
    }
    for (i = 0; i < times; i++) {
        Uint64 start = SDL_GetPerformanceCounter();
        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(dev);
        SDL_GPUFence* fence;
        double ms;
        tex = rd_gpu_end_frame(cmd);
        fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
        SDL_WaitForGPUFences(dev, true, &fence, 1);
        SDL_ReleaseGPUFence(dev, fence);
        ms = (double)(SDL_GetPerformanceCounter() - start) * 1000.0 / (double)SDL_GetPerformanceFrequency();
        best = ms < best ? ms : best;
    }
    printf("%.2f ms a frame at best\n", best);
    if (tex != NULL) {
        if (rd_gpu_read(frame) && out != NULL) {
            rd_frame_write_ppm(out, frame);
        }
    }
    while (win != NULL) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                SDL_ReleaseWindowFromGPUDevice(dev, win);
                SDL_DestroyWindow(win);
                win = NULL;
                break;
            }
        }
        if (win != NULL) {
            show(dev, win, tex);
        }
    }
    rd_gpu_quit();
    SDL_DestroyGPUDevice(dev);
    SDL_Quit();
    return 0;
}
