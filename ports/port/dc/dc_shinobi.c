/*
 * dc_shinobi.c  Shinobi, the Dreamcast's system library, as far as the game uses it
 *
 * The hardware and the heap need nothing. The console's settings say stereo, its clock is the host's,
 * and the video cable is VGA, the port's screen. The controllers in ports A and B are the host's two
 * players; C and D have nothing, so there is no keyboard. There is no memory card yet: every port
 * answers that it has none, which the game's boot accepts. The modem is not there either.
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "naomi_host.h"
#include "naomi_input.h"
#include "shinobi.h"

/* ---- start-up, the heap, the boot ROM ---- */

void syHwInit(void) {
}

void syHwInit2(void) {
}

void syHwFinish(void) {
}

void syMallocInit(void* heap, Uint32 size) {
    (void)heap;
    (void)size;
}

void syMallocFinish(void) {
}

void syStartGlobalConstructor(void) {
}

void syStartGlobalDestructor(void) {
}

/* back to the console's menu: the port ends */
void syBtExit(void) {
    host_log("the game went back to the console's menu");
    exit(0);
}

/* a Shift-JIS character's JIS code */
Sint16 syBtFntSjis2Jis(Sint16 sjis) {
    int hi = sjis >> 8 & 0xff, lo = sjis & 0xff;
    hi = (hi - (hi <= 0x9f ? 0x71 : 0xb1)) * 2 + 1;
    if (lo > 0x7f) {
        lo--;
    }
    if (lo >= 0x9e) {
        lo -= 0x7d;
        hi++;
    } else {
        lo -= 0x1f;
    }
    return (Sint16)(hi << 8 | lo);
}

Sint32 syCblCheck(void) {
    return SYE_CBL_VGA;
}

/* ---- the console's settings and clock ---- */

Sint32 syCfgInit(void* work) {
    (void)work;
    return 1;
}

Sint32 syCfgExit(void) {
    return 1;
}

static Sint32 sound_mode = SYD_CFG_STEREO;

Sint32 syCfgGetSoundMode(Sint32* mode) {
    *mode = sound_mode;
    return 1;
}

Sint32 syCfgSetSoundMode(Sint32 mode) {
    sound_mode = mode;
    return 1;
}

/* the console's own number: a host has none, so it is 0 */
Sint32 syCfgGetIndividualID(Sint8* id) {
    memset(id, 0, SYD_CFG_IID_SIZE);
    return 1;
}

Sint32 syRtcInit(void) {
    return 1;
}

Sint32 syRtcGetDate(SYS_RTC_DATE* date) {
    struct tm now, *tm = &now;
    host_clock(&now);
    memset(date, 0, sizeof *date);
    date->year = (Uint16)(tm->tm_year + 1900);
    date->month = (Uint8)(tm->tm_mon + 1);
    date->day = (Uint8)tm->tm_mday;
    date->hour = (Uint8)tm->tm_hour;
    date->minute = (Uint8)tm->tm_min;
    date->second = (Uint8)tm->tm_sec;
    date->dayofweek = (Uint8)tm->tm_wday;
    return 1;
}

/* the general timer, which counts the time played: set to 4, it counts 48828 a second; here it
   follows the frames, so that a run is the same every time */
static unsigned tmr_base;

void syTmrGenSetClock(Sint32 clock) {
    (void)clock;
}

void syTmrGenStart(void) {
    tmr_base = host_frame();
}

void syTmrGenSetCount(Uint32 count) {
    (void)count;
    tmr_base = host_frame();
}

Uint32 syTmrGenGetCount(void) {
    return (Uint32)((host_frame() - tmr_base) * 48828u / 60u);
}

/* ---- the controllers ---- */

static PDS_PERIPHERALINFO pad_info = { PDD_DEVTYPE_CONTROLLER };
static PDS_PERIPHERALINFO none_info;
static PDS_PERIPHERAL pads[4];

/* a player's JVS switches as a pad's buttons: B1 to B6 are X, Y, L, A, B and R, the game's
   punches, kicks and assists in the arcade's order */
static const struct {
    uint32_t jvs, pad;
} button_map[] = {
    { JVS_UP, PDD_DGT_KU }, { JVS_DOWN, PDD_DGT_KD }, { JVS_LEFT, PDD_DGT_KL }, { JVS_RIGHT, PDD_DGT_KR },
    { JVS_START, PDD_DGT_ST }, { JVS_B1, PDD_DGT_TX }, { JVS_B2, PDD_DGT_TY }, { JVS_B3, PDD_DGT_TL },
    { JVS_B4, PDD_DGT_TA }, { JVS_B5, PDD_DGT_TB }, { JVS_B6, PDD_DGT_TR },
};

void pdInitPeripheral(Sint32 logic, void* recvbuf, void* sendbuf) {
    int i;
    (void)logic;
    (void)recvbuf;
    (void)sendbuf;
    memset(pads, 0, sizeof pads);
    for (i = 0; i < 4; i++) {
        pads[i].info = i < 2 ? &pad_info : &none_info;
    }
}

void pdExitPeripheral(void) {
}

/* once a frame: the host's controls into the two pads */
void pdExecPeripheralServer(void) {
    const HOST_INPUT* in = host_input();
    int i;
    size_t b;
    for (i = 0; i < 2; i++) {
        PDS_PERIPHERAL* p = &pads[i];
        Uint32 on = 0;
        for (b = 0; b < sizeof button_map / sizeof button_map[0]; b++) {
            if (in->player[i] & button_map[b].jvs) {
                on |= button_map[b].pad;
            }
        }
        p->press = on & ~p->old;
        p->release = ~on & p->old;
        p->on = on;
        p->off = ~on;
        p->old = on;
        p->l = on & PDD_DGT_TL ? 255 : 0;
        p->r = on & PDD_DGT_TR ? 255 : 0;
        p->support = 0;
        p->id = 0;
    }
}

/* a port's first socket (A0, B0, C0, D0) is its controller; the sockets after it hold nothing */
PDS_PERIPHERAL* pdGetPeripheral(Uint32 port) {
    static PDS_PERIPHERAL none = { 0, 0, 0, 0xFFFFFFFF, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL, 0, &none_info };
    return port % 6 == 0 && port < 24 ? &pads[port / 6] : &none;
}

PDS_PERIPHERALINFO* pdGetPeripheralInfo(Uint32 port) {
    return pdGetPeripheral(port)->info;
}

Sint32 pdVibMxIsReady(Uint32 port) {
    (void)port;
    return 0;
}

Sint32 pdVibMxStart(Uint32 port, PDS_VIBPARAM* param) {
    (void)port;
    (void)param;
    return 0;
}

Sint32 pdVibMxStop(Uint32 port) {
    (void)port;
    return 0;
}

/* the memory card's screen */
Sint32 pdVmsLcdIsReady(Uint32 port) {
    (void)port;
    return 0;
}

Sint32 pdVmsLcdWrite(Uint32 port, void* image, Sint32 flag) {
    (void)port;
    (void)image;
    (void)flag;
    return 0;
}

Sint32 pdLcdGetDirection(Uint32 port) {
    (void)port;
    return 0;
}

/* the keyboard: none is plugged in */
void pdKbdInit(void) {
}

void pdKbdExit(void) {
}

const PDS_KEYBOARD* pdKbdGetData(Uint32 port) {
    (void)port;
    return NULL;
}

/* ---- the memory card: dc_vmu.c ---- */

/* ---- the modem and the console's network settings: none ---- */

void mdmClose(MDM handle) {
    (void)handle;
}

void mdmFinish(void) {
}

Sint32 ntInfIsNetworkInfo(void) {
    return NTD_ERR_NOINFO;
}
