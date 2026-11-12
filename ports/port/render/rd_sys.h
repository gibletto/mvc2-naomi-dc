/*
 * rd_sys.h  The few SDL calls the back ends make (threads, semaphores, the clock), for SDL2 and,
 * with RD_SDL3 defined, SDL3
 */

#ifndef RD_SYS_H
#define RD_SYS_H

#if defined(__3DS__)
/* the 3DS draws with its GPU (port/ctr); rd_soft runs there on the one thread, for checks (CTR_SOFT) */
#include <time.h>
typedef int RD_SEM;
#define rd_sem_wait(s) ((void)(s))
#define rd_sem_post(s) ((void)(s))
#define rd_cpu_count() 1
#define SDL_CreateSemaphore(n) ((RD_SEM*)NULL)
#define SDL_CreateThread(f, name, data) ((void)(f), (void*)NULL)
#define SDL_GetPerformanceCounter() ((uint64_t)clock())
#define SDL_GetPerformanceFrequency() ((uint64_t)CLOCKS_PER_SEC)
#elif defined(RD_SDL3)
#include <SDL3/SDL.h>
typedef SDL_Semaphore RD_SEM;
#define rd_sem_wait SDL_WaitSemaphore
#define rd_sem_post SDL_SignalSemaphore
#define rd_cpu_count SDL_GetNumLogicalCPUCores
#else
#include <SDL.h>
typedef SDL_sem RD_SEM;
#define rd_sem_wait SDL_SemWait
#define rd_sem_post SDL_SemPost
#define rd_cpu_count SDL_GetCPUCount
#endif

#endif
