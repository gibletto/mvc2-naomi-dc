/* mister_save.c  The game's saves (backup.bin, eeprom.bin) written by a thread of their own at a low priority: the
   arcade updates its book-keeping as it is played, and a file written through the SD card held the game 9-35 ms */

#define _GNU_SOURCE
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/syscall.h>

#include "mister_front.h"
#include "naomi_host.h"

#define SLOTS 4
#define MOST 4096

static struct {
    char name[256];
    char data[MOST];
    int size, due;
} slot[SLOTS];
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t wake = PTHREAD_COND_INITIALIZER;
static pthread_t writer;
static int started, quit;

static void* writer_main(void* arg) {
    static char name[256], data[MOST];
    (void)arg;
    setpriority(PRIO_PROCESS, (id_t)syscall(SYS_gettid), 10);
    mister_on_cpu1();
    for (;;) {
        int i, size = 0;
        pthread_mutex_lock(&lock);
        for (;;) {
            for (i = 0; i < SLOTS && !slot[i].due; i++) {
            }
            if (i < SLOTS || quit) {
                break;
            }
            pthread_cond_wait(&wake, &lock);
        }
        if (i < SLOTS) {
            memcpy(name, slot[i].name, sizeof name);
            memcpy(data, slot[i].data, (size_t)slot[i].size);
            size = slot[i].size;
            slot[i].due = 0;
        }
        pthread_mutex_unlock(&lock);
        if (i == SLOTS) {
            return NULL;
        }
        host_save_file(name, data, size);
    }
}

/* a copy of the file's bytes given to the writer, the newest for each file; 0 when it can't take them */
int mister_save_later(const char* name, const void* data, int size) {
    int i, free_at = -1;
    if (quit || size <= 0 || size > MOST || strlen(name) >= sizeof slot[0].name) {
        return 0;
    }
    if (!started) {
        if (pthread_create(&writer, NULL, writer_main, NULL) != 0) {
            quit = 1;
            return 0;
        }
        started = 1;
    }
    pthread_mutex_lock(&lock);
    for (i = 0; i < SLOTS; i++) {
        if (strcmp(slot[i].name, name) == 0) {
            break;
        }
        if (slot[i].name[0] == '\0' && free_at < 0) {
            free_at = i;
        }
    }
    if (i == SLOTS) {
        i = free_at;
    }
    if (i >= 0) {
        strcpy(slot[i].name, name);
        memcpy(slot[i].data, data, (size_t)size);
        slot[i].size = size;
        slot[i].due = 1;
        pthread_cond_signal(&wake);
    }
    pthread_mutex_unlock(&lock);
    return i >= 0;
}

/* every save written and the writer stopped, waited for up to 3 s (a card that hangs doesn't hold the exit); later
   ones are written as they come */
void mister_save_flush(void) {
    struct timespec t;
    pthread_mutex_lock(&lock);
    quit = 1;
    pthread_cond_signal(&wake);
    pthread_mutex_unlock(&lock);
    if (started) {
        clock_gettime(CLOCK_REALTIME, &t);
        t.tv_sec += 3;
        if (pthread_timedjoin_np(writer, NULL, &t) != 0) {
            host_log("saves: still being written at the exit");
        }
        started = 0;
    }
}
