/*
 * start.c  Which program to run, from the data found (the MAME set mvsc2, the USA disc's files), and mvc2.cfg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __3DS__
#include "ctr_front.h"
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
      "the arcade's ROM set (MAME's mvsc2, unzipped: epr-23085a.ic11 and mpr-23048.ic17s to mpr-23084.ic32s)" },
    { "dc", "MVC2_DC_DISC", "disc", "the files of the USA Dreamcast disc (1ST_READ.BIN and the rest)" },
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
#ifdef __3DS__
    (void)base, (void)buf, (void)size;
    return path;        /* the 3DS starts in its data folder, sdmc:/3ds/mvc2 */
#else
    if (base[0] == 0) {
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
    char line[1100], copy[1100];
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
    f = fopen(config_path, "w");
    if (f != NULL) {
        if (len == 0) {
            fputs("# where each mode's data is (arcade=, dc=), the mode to start in when both are found, and the settings\n", f);
        } else {
            fputs(text, f);
        }
        if (!done) {
            fprintf(f, "%s = %s\n", key, value);
        }
        fclose(f);
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

static void tell(const char* title, const char* text) {
    fprintf(stderr, "%s\n%s\n", title, text);
    snprintf(told, sizeof told, "%s", text);
#ifdef __3DS__
    if (getenv("MVC2_FRAMES") == NULL) {   /* a test run shows no boxes */
        ctr_tell(title, text);
    }
#endif
}

const char* pick_message(void) {
    return told;
}

/* -1 to quit */
static int ask(const char* const path[MODES]) {
#ifndef __3DS__
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
#ifdef __3DS__
    return ctr_ask(text);
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
        tell("Marvel vs. Capcom 2", "This build has no such mode.");
        return 0;
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
        tell("Marvel vs. Capcom 2", text);
        return 0;
    }
    host_log("%s mode, from %s", modes[mode].name, path[mode]);
    return 1;
}

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
#ifdef MVC2_ARCADE
    if (mode == ARCADE) {
        return naomi_init(&arc_program, path[mode]);
    }
#endif
#ifdef MVC2_DC
    if (mode == DC) {
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
