/*
 * start.c  Which program to run, from the data found (the MAME set mvsc2, the USA disc's files), and mvc2.cfg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__3DS__)
#include "ctr_front.h"
#elif defined(__PSP__)
#include "psp_front.h"
#elif defined(MVC2_MISTER)
#include "mister_front.h"
#else
#include "sdl_front.h"
#endif

#include "naomi_host.h"

#define CONFIG "mvc2.cfg"

enum { ARCADE, DC, MODES };

static const struct {
    const char* name;
    const char* env;
    const char* folder;
    const char* needs;
} modes[MODES] = {
    { "arcade", "MVC2_ROM", "roms/mvsc2",
      "the arcade's ROM set (MAME's mvsc2.zip, or its files: epr-23085a.ic11 and mpr-23048.ic17s to mpr-23084.ic32s)" },
    { "dc", "MVC2_DC_DISC", "disc", "the USA Dreamcast disc: an image of it (.chd, .gdi, .cue or .iso), or its files (1ST_READ.BIN and the rest)" },
};

static const int built[MODES] = {
#ifdef MVC2_ARCADE
    1,
#else
    0,
#endif
#ifdef MVC2_DC
    1,
#else
    0,
#endif
};

#define CONFIG_KEYS 64
static struct {
    char key[32];
    char value[1024];
} config[CONFIG_KEYS];
static int config_n;
static char config_path[1100];
static char folder[MODES][1100];

/* path, or when it is relative, path in the executable's folder */
static const char* beside_exe(const char* path, char* buf, size_t size) {
    static char base[1024];
    if (path[0] == '/' || path[0] == '\\' || (path[0] != 0 && path[1] == ':')) {
        return path;
    }
#if defined(__3DS__) || defined(__PSP__) || defined(MVC2_MISTER)
    (void)base;
    /* each starts in its data folder, sdmc:/3ds/mvc2, ms0:/PSP/GAME/MVC2 or /media/fat/games/mvc2 */
    snprintf(buf, size, "%s", path);
    return buf;
#else
    if (base[0] == 0) {
        const char* appimage = getenv("APPIMAGE");
        const char* slash = appimage != NULL ? strrchr(appimage, '/') : NULL;
        if (slash != NULL) {
            /* an AppImage runs from a mount of its own: the folder that holds it */
            snprintf(base, sizeof base, "%.*s", (int)(slash + 1 - appimage), appimage);
        } else {
#ifdef MVC2_SDL3
            const char* b = SDL_GetBasePath();
#else
            char* b = SDL_GetBasePath();
#endif
            snprintf(base, sizeof base, "%s", b != NULL ? b : "");
#ifndef MVC2_SDL3
            SDL_free(b);
#endif
        }
    }
    snprintf(buf, size, "%s%s", base, path);
    return buf;
#endif
}

const char* host_save_path(const char* name) {
    static char buf[1100];
    return beside_exe(name, buf, sizeof buf);
}

static char* parse_line(char* line, char** value) {
    char* eq = strchr(line, '=');
    char *k, *v, *end;
    if (line[0] == '#' || eq == NULL) {
        return NULL;
    }
    *eq = 0;
    for (k = line; *k == ' ' || *k == '\t'; k++) {
    }
    for (end = eq; end > k && (end[-1] == ' ' || end[-1] == '\t'); end--) {
    }
    *end = 0;
    for (v = eq + 1; *v == ' ' || *v == '\t'; v++) {
    }
    for (end = v + strcspn(v, "\r\n"); end > v && (end[-1] == ' ' || end[-1] == '\t'); end--) {
    }
    *end = 0;
    *value = v;
    return k;
}

static void keep(const char* key, const char* value) {
    int i;
    for (i = 0; i < config_n && strcmp(config[i].key, key) != 0; i++) {
    }
    if (i == CONFIG_KEYS) {
        return;
    }
    if (i == config_n) {
        config_n++;
        snprintf(config[i].key, sizeof config[i].key, "%s", key);
    }
    snprintf(config[i].value, sizeof config[i].value, "%s", value);
}

static void read_config(void) {
    char line[1100];
    FILE* f;
    beside_exe(CONFIG, config_path, sizeof config_path);
    f = fopen(config_path, "r");
    while (f != NULL && fgets(line, sizeof line, f) != NULL) {
        char* v;
        char* k = parse_line(line, &v);
        if (k != NULL) {
            keep(k, v);
        }
    }
    if (f != NULL) {
        fclose(f);
    }
}

#ifdef _WIN32
/* for main_sdl.c's console = yes, before the data is looked for */
void config_read(void) {
    read_config();
}
#endif

const char* config_get(const char* key) {
    int i;
    for (i = 0; i < config_n; i++) {
        if (strcmp(config[i].key, key) == 0) {
            return config[i].value[0] != 0 ? config[i].value : NULL;
        }
    }
    return NULL;
}

/* the key's line is changed in place, or added */
void config_set(const char* key, const char* value) {
    char* text = NULL;
    size_t len = 0;
    char line[1100], copy[1100], tmp[1110];
    int done = 0;
    FILE* f;
    keep(key, value);
    f = fopen(config_path, "r");
    while (f != NULL && fgets(line, sizeof line, f) != NULL) {
        char* v;
        char* k;
        char* grown;
        size_t n;
        snprintf(copy, sizeof copy, "%s", line);
        k = parse_line(copy, &v);
        if (k != NULL && strcmp(k, key) == 0) {
            if (done) {
                continue;
            }
            snprintf(line, sizeof line, "%s = %s\n", key, value);
            done = 1;
        }
        n = strlen(line);
        grown = realloc(text, len + n + 2);
        if (grown == NULL) {
            break;
        }
        text = grown;
        memcpy(text + len, line, n + 1);
        len += n;
        if (text[len - 1] != '\n') {
            text[len++] = '\n';
            text[len] = 0;
        }
    }
    if (f != NULL) {
        fclose(f);
    }
    snprintf(tmp, sizeof tmp, "%s.new", config_path);
    f = fopen(tmp, "w");
    if (f != NULL) {
        int whole;
        if (len == 0) {
            whole = fputs("# where each mode's data is (arcade=, dc=), the mode to start in when both are found, and the settings\n", f) >= 0;
        } else {
            whole = fputs(text, f) >= 0;
        }
        if (!done) {
            whole = fprintf(f, "%s = %s\n", key, value) >= 0 && whole;
        }
        if (fclose(f) == 0 && whole) {
            remove(config_path);
            rename(tmp, config_path);
        } else {
            remove(tmp);    /* a full disk: the old settings kept */
        }
    }
    free(text);
}

/* the first file missing or of the wrong size, or NULL */
static const char* check(int mode, const char* dir) {
    if (!built[mode]) {
        return "(not built in)";
    }
#ifdef MVC2_ARCADE
    if (mode == ARCADE) {
        return naomi_rom_check(dir);
    }
#endif
#ifdef MVC2_DC
    if (mode == DC) {
        return dc_disc_check(dir);
    }
#endif
    return "(not built in)";
}

static char told[4000];
static int why;

int pick_why(void) {
    return why;
}

static void front_tell(const char* title, const char* text) {
    fprintf(stderr, "%s\n%s\n", title, text);
    snprintf(told, sizeof told, "%s", text);
#ifdef __3DS__
    if (getenv("MVC2_FRAMES") == NULL) {   /* a test run shows no boxes */
        ctr_tell(title, text);
    }
#elif defined(__PSP__)
    psp_tell(title, text);
#endif
}

const char* pick_message(void) {
    return told;
}

/* -1 to quit */
static int ask(const char* const path[MODES]) {
#if !defined(__3DS__) && !defined(__PSP__) && !defined(MVC2_MISTER)
    static const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, ARCADE, "Arcade" },
        { 0, DC, "Dreamcast" },
        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, -1, "Quit" },
    };
    char text[3000];
    SDL_MessageBoxData box = { SDL_MESSAGEBOX_INFORMATION, NULL, "Marvel vs. Capcom 2", text, 3, buttons, NULL };
    int chosen = -1;
#else
    char text[3000];
#endif
    snprintf(text, sizeof text,
             "Both are here: the arcade's ROM set in %s and the Dreamcast's disc files in %s.\n\n"
             "Which do you want to play? " CONFIG " keeps the choice (mode=); --arcade or --dc picks one for a run.",
             path[ARCADE], path[DC]);
    if (getenv("MVC2_FRAMES") != NULL) {
        fprintf(stderr, "%s\n", text);
        return -1;
    }
#if defined(__3DS__)
    return ctr_ask(text);
#elif defined(__PSP__)
    return psp_ask(text);
#elif defined(MVC2_MISTER)
    return DC;      /* no menu yet: mode = arcade in mvc2.cfg for the other */
#else
    if (!SDL_SUCCEEDED(SDL_ShowMessageBox(&box, &chosen))) {
        return -1;
    }
    return chosen;
#endif
}

static int mode = -1;
static const char* path[MODES];
static int found[MODES];

int pick_game(int argc, char** argv) {
    const char* missing[MODES];
    const char* arg = NULL;
    int flag = -1, i;
    mode = -1;
    told[0] = 0;
    why = PICK_QUIT;
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--arcade") == 0) {
            flag = ARCADE;
        } else if (strcmp(argv[i], "--dc") == 0) {
            flag = DC;
        } else {
            arg = argv[i];
        }
    }
    read_config();
    for (i = 0; i < MODES; i++) {
        path[i] = getenv(modes[i].env) != NULL ? getenv(modes[i].env)
                  : beside_exe(config_get(modes[i].name) != NULL ? config_get(modes[i].name) : modes[i].folder, folder[i],
                                               sizeof folder[i]);
        if (arg != NULL && (flag == i || flag < 0)) {
            path[i] = arg;
        }
        missing[i] = check(i, path[i]);
        found[i] = missing[i] == NULL;
    }
    if (flag >= 0 && !built[flag]) {
        front_tell("Marvel vs. Capcom 2", "This build has no such mode.");
        return 0;
    }
    /* main RAM too small for the arcade's program (an Old 3DS's 3dsx): the Dreamcast's */
    if (host_ram_size < SH4_RAM_SIZE && built[ARCADE] && flag != DC) {
        if (flag == ARCADE || missing[DC] != NULL) {
            front_tell("Marvel vs. Capcom 2", "Arcade mode needs more memory than a 3dsx has on an Old 3DS. Install the "
                                              "CIA (with FBI) and start it from the HOME Menu, or play the Dreamcast version.");
            return 0;
        }
        flag = DC;
    }
    if (flag >= 0) {
        mode = flag;
    } else if (missing[ARCADE] == NULL && missing[DC] == NULL && getenv("MVC2_FRAMES") != NULL) {
        mode = ARCADE;
    } else if (missing[ARCADE] == NULL && missing[DC] == NULL) {
        for (i = 0; i < MODES; i++) {
            if (config_get("mode") != NULL && strcmp(config_get("mode"), modes[i].name) == 0) {
                mode = i;
            }
        }
        if (mode < 0) {
            mode = ask(path);
            if (mode < 0) {
                return 0;
            }
            config_set("mode", modes[mode].name);
        }
    } else {
        mode = missing[ARCADE] == NULL ? ARCADE : missing[DC] == NULL ? DC : -1;
    }
    if (mode < 0 || missing[mode] != NULL) {
        char text[4000];
        why = mode < 0 ? PICK_NO_DATA : mode == ARCADE ? PICK_NO_ARCADE : PICK_NO_DC;
        int n = snprintf(text, sizeof text, "%s", mode < 0 ? "No game data was found.\n" : "");
        for (i = 0; i < MODES; i++) {
            if (built[i] && (mode < 0 || mode == i) && n < (int)sizeof text) {
                n += snprintf(text + n, sizeof text - n,
                              "\n%s mode needs %s, in %s (or set %s= in " CONFIG ", or %s). %s is missing there, "
                              "or the wrong size.\n",
                              i == ARCADE ? "Arcade" : "Dreamcast", modes[i].needs, path[i], modes[i].name,
                              modes[i].env, missing[i]);
            }
        }
        front_tell("Marvel vs. Capcom 2", text);
        return 0;
    }
    host_log("%s mode, from %s", modes[mode].name, path[mode]);
    return 1;
}

#ifdef MVC2_DC
#ifdef MVC2_MISTER
static const char* unpacking;

static void unpack_progress(uint32_t done, uint32_t total) {
    static uint32_t shown = ~0u;
    uint32_t pct = total > 0 ? (uint32_t)((uint64_t)done * 100 / total) : 0;
    if (pct / 10 != shown) {
        shown = pct / 10;
        host_log("unpacking the disc's files: %u%%", (unsigned)pct);
    }
    mister_unpacking(unpacking, done, total);
}
#elif !defined(__3DS__) && !defined(__PSP__)
/* the window's title says how far it is, and the window keeps answering */
static void unpack_progress(uint32_t done, uint32_t total) {
    static uint32_t shown = ~0u;
    uint32_t pct = total > 0 ? (uint32_t)((uint64_t)done * 100 / total) : 0;
    SDL_Window* w = SDL_GetKeyboardFocus();
    char title[100];
    SDL_PumpEvents();
    if (pct != shown && w != NULL) {
        shown = pct;
        snprintf(title, sizeof title, "Marvel vs. Capcom 2: unpacking the disc's files, %u%%", (unsigned)pct);
        SDL_SetWindowTitle(w, title);
    }
}
#endif

/* the disc's files from the image in dir, once; 0 when they could not be had */
static int unpack_disc(const char* dir) {
    const char* image = dc_image_find(dir);
    const char* err;
    const char* missing;
    static char text[1500];
#ifdef __3DS__
    snprintf(text, sizeof text,
             "Marvel vs. Capcom 2\n\nONE-TIME DISC UNPACK\n\nThe disc's files are unpacked from\n%.36s\ninto %.30s\n"
             "(210 MB), so the game can read them\nquickly.\n\nThis happens only once. Please don't\nturn off the 3DS "
             "or take out the\nSD card.\n",
             image != NULL ? image : "", dir);
    ctr_unpack_screen(text);
    err = dc_image_unpack(dir, ctr_unpack_progress);
    ctr_unpack_screen(NULL);
#elif defined(__PSP__)
    snprintf(text, sizeof text,
             "ONE-TIME DISC UNPACK\n\nThe disc's files are unpacked from %.40s\ninto %.50s (210 MB),\n"
             "so the game can read them quickly.\n\nThis happens only once. Please don't turn off the PSP\n"
             "or take out the Memory Stick.\n",
             image != NULL ? image : "", dir);
    psp_unpack_screen(text);
    err = dc_image_unpack(dir, psp_unpack_progress);
    psp_unpack_screen(NULL);
#elif defined(MVC2_MISTER)
    unpacking = image != NULL ? image : "";
    err = dc_image_unpack(dir, unpack_progress);
#else
    (void)image;
    err = dc_image_unpack(dir, unpack_progress);
    if (SDL_GetKeyboardFocus() != NULL) {
        SDL_SetWindowTitle(SDL_GetKeyboardFocus(), "Marvel vs. Capcom 2");
    }
#endif
    missing = err == NULL ? dc_disc_missing(dir) : NULL;
    if (err == NULL && missing == NULL) {
        return 1;
    }
    if (err != NULL) {
        snprintf(text, sizeof text, "The disc image in %s could not be unpacked: %s.", dir, err);
    } else {
        snprintf(text, sizeof text,
                 "The disc image in %s is not the USA Dreamcast disc: %s is missing from it, or the wrong size.", dir,
                 missing);
    }
    host_log("%s", text);
    why = PICK_BAD_DISC;
#if defined(__3DS__)
    ctr_rom_error = text;
#elif defined(__PSP__)
    psp_tell("Marvel vs. Capcom 2", text);
#elif !defined(MVC2_MISTER)
    if (getenv("MVC2_FRAMES") == NULL) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Marvel vs. Capcom 2", text, NULL);
    }
#endif
    return 0;
}
#endif

const char* game_title(void) {
    return mode == DC ? "Marvel vs. Capcom 2 (Dreamcast)" : "Marvel vs. Capcom 2 (Arcade)";
}

int start_game(void) {
    const char* unlock = getenv("MVC2_UNLOCK");
    /* again at each start: a switch clears it */
    if (unlock != NULL) {
        host_unlock = atoi(unlock) != 0;
    } else {
        unlock = getenv("MVC2_FRAMES") == NULL ? config_get("unlock") : NULL;
        host_unlock = unlock != NULL && strcmp(unlock, "all") == 0;
    }
    if (getenv("MVC2_FREEPLAY") != NULL) {
        host_freeplay = atoi(getenv("MVC2_FREEPLAY")) != 0;
    } else {
        host_freeplay = getenv("MVC2_FRAMES") == NULL && config_get("freeplay") != NULL &&
                        strcmp(config_get("freeplay"), "on") == 0;
    }
#ifdef MVC2_ARCADE
    if (mode == ARCADE) {
        int started;
        host_keep_program(&arc_program);
        started = naomi_init(&arc_program, path[mode]);
#ifdef MVC2_MISTER
        if (!started && naomi_rom_unpack_failed()) {
            why = PICK_NO_ROOM;
        }
#endif
        return started;
    }
#endif
#ifdef MVC2_DC
    if (mode == DC) {
        HOST_CRUMB("checking the disc's files");
        /* MVC2_UNPACK_AGAIN=1 (tests): the image unpacked even over its files */
        int again = getenv("MVC2_UNPACK_AGAIN") != NULL && atoi(getenv("MVC2_UNPACK_AGAIN")) != 0 &&
                    dc_image_find(path[mode]) != NULL;
        if (!dc_gd_mount(path[mode]) && (again || dc_disc_missing(path[mode]) != NULL) && !unpack_disc(path[mode])) {
            return 0;
        }
        HOST_CRUMB("host_keep_program");
        host_keep_program(&dc_program);
        HOST_CRUMB("dc_init");
        return dc_init(&dc_program, path[mode]);
    }
#endif
    return 0;
}

int switch_game(void) {
    int other = mode == ARCADE ? DC : ARCADE;
    if (!found[other]) {
        host_log("no %s data in %s to switch to", modes[other].name, path[other]);
        return 1;
    }
    host_restart();
    mode = other;
    host_log("%s mode, from %s", modes[mode].name, path[mode]);
    return start_game();
}
