/*
 * display_win32.c  What Windows says of the display a window is on: whether NVIDIA's driver has its variable
 * refresh on (NVAPI, read only), and the refreshes' clock (the compositor's timing). Both libraries are loaded as
 * they are needed, and either may be missing.
 */
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dwmapi.h>
#include <string.h>

#include "display_win32.h"

/* NVAPI's entry points by their ids (nvapi_interface.h); each NVAPI_INTERFACE is __cdecl */
typedef void*(__cdecl* NV_QUERY)(unsigned id);
typedef int(__cdecl* NV_INIT)(void);
typedef int(__cdecl* NV_ID_BY_NAME)(const char* name, unsigned* id);
typedef int(__cdecl* NV_VRR_INFO)(unsigned id, unsigned* info);

int display_vrr(void* hwnd) {
    static HMODULE nv;
    static int tried, ready;
    MONITORINFOEXA mi;
    NV_QUERY query;
    NV_ID_BY_NAME by_name;
    NV_VRR_INFO vrr_info;
    unsigned id = 0, info[8];
    if (!tried) {
        tried = 1;
        nv = LoadLibraryA(sizeof(void*) == 8 ? "nvapi64.dll" : "nvapi.dll");
        query = nv != NULL ? (NV_QUERY)(void*)GetProcAddress(nv, "nvapi_QueryInterface") : NULL;
        ready = query != NULL && query(0x0150E828u) != NULL && ((NV_INIT)query(0x0150E828u))() == 0;
    }
    if (!ready) {
        return -1;
    }
    query = (NV_QUERY)(void*)GetProcAddress(nv, "nvapi_QueryInterface");
    by_name = (NV_ID_BY_NAME)query(0xAE457190u);     /* NvAPI_DISP_GetDisplayIdByDisplayName */
    vrr_info = (NV_VRR_INFO)query(0xDF8FDA57u);      /* NvAPI_Disp_GetVRRInfo */
    memset(&mi, 0, sizeof mi);
    mi.cbSize = sizeof mi;
    if (by_name == NULL || vrr_info == NULL ||
        !GetMonitorInfoA(MonitorFromWindow((HWND)hwnd, MONITOR_DEFAULTTONEAREST), (MONITORINFO*)&mi) ||
        by_name(mi.szDevice, &id) != 0) {
        return -1;
    }
    /* NV_GET_VRR_INFO_V1: its version (its size, 24 bytes, and 1), then bIsVRREnabled in bit 0: set while this
       program presents with VRR (bIsDisplayInVRRMode, bit 4, follows whatever presents) */
    memset(info, 0, sizeof info);
    info[0] = 24u | 1u << 16;
    if (vrr_info(id, info) != 0) {
        return -1;
    }
    return info[1] & 1;
}

int display_vblank(uint64_t* last, uint64_t* period) {
    typedef HRESULT(WINAPI * TIMING)(HWND, DWM_TIMING_INFO*);
    static TIMING timing;
    static int tried;
    DWM_TIMING_INFO t;
    if (!tried) {
        HMODULE dwm = LoadLibraryA("dwmapi.dll");
        tried = 1;
        timing = dwm != NULL ? (TIMING)(void*)GetProcAddress(dwm, "DwmGetCompositionTimingInfo") : NULL;
    }
    memset(&t, 0, sizeof t);
    t.cbSize = sizeof t;
    if (timing == NULL || timing(NULL, &t) != S_OK || t.qpcRefreshPeriod == 0 || t.qpcVBlank == 0) {
        return 0;
    }
    *last = t.qpcVBlank;
    *period = t.qpcRefreshPeriod;
    return 1;
}
