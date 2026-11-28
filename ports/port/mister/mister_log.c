/*
 * mister_log.c  The log's lines queued in memory and written to stderr (the run's log) by a thread of their own. The
 * MiSTer mounts its SD card sync, so each write waits for the card: a few lines took the game's thread 90 ms.
 */

#define _GNU_SOURCE     /* pthread_setname_np */

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "mister_front.h"
#include "naomi_host.h"

#define RING (256 << 10)

static char ring[RING];
static size_t put_at, written;      /* bytes put and bytes written, counted from the start */
static unsigned lost;               /* lines that found the ring full */
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t more = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t writing = PTHREAD_MUTEX_INITIALIZER;     /* one writer at a time, in order */
static pthread_once_t once = PTHREAD_ONCE_INIT;
static int running;

static void out(const char* p, size_t n) {
    while (n > 0) {
        ssize_t k = write(2, p, n);
        if (k <= 0) {
            return;
        }
        p += k;
        n -= (size_t)k;
    }
}

/* everything put so far written */
void mister_log_flush(void) {
    pthread_mutex_lock(&writing);
    pthread_mutex_lock(&lock);
    while (written != put_at || lost != 0) {
        size_t at = written % RING, n = put_at - written;
        unsigned l = lost;
        n = n < RING - at ? n : RING - at;
        lost = 0;
        pthread_mutex_unlock(&lock);
        out(ring + at, n);
        if (l != 0) {
            char text[64];
            out(text, (size_t)snprintf(text, sizeof text, "log: %u lines lost, the queue full\n", l));
        }
        pthread_mutex_lock(&lock);
        written += n;
    }
    pthread_mutex_unlock(&lock);
    pthread_mutex_unlock(&writing);
}

static void* writer(void* arg) {
    (void)arg;
    mister_on_cpu1();
    for (;;) {
        pthread_mutex_lock(&lock);
        while (written == put_at && lost == 0) {
            pthread_cond_wait(&more, &lock);
        }
        pthread_mutex_unlock(&lock);
        mister_log_flush();
    }
    return NULL;
}

static void start(void) {
    pthread_t t;
    if (pthread_create(&t, NULL, writer, NULL) == 0) {
        pthread_setname_np(t, "log");
        pthread_detach(t);
        running = 1;
    }
}

/* a line queued for the writer; 0 when there is no writer (the caller writes it) */
int mister_log_line(const char* fmt, va_list ap) {
    char line[2048];
    int n;
    size_t at, first;
    pthread_once(&once, start);
    if (!running) {
        return 0;
    }
    n = vsnprintf(line, sizeof line - 1, fmt, ap);
    n = n < 0 ? 0 : n < (int)sizeof line - 1 ? n : (int)sizeof line - 2;
    line[n++] = '\n';
    pthread_mutex_lock(&lock);
    if (RING - (put_at - written) < (size_t)n) {
        lost++;
    } else {
        at = put_at % RING;
        first = (size_t)n < RING - at ? (size_t)n : RING - at;
        memcpy(ring + at, line, first);
        memcpy(ring, line + first, (size_t)n - first);
        put_at += (size_t)n;
    }
    pthread_cond_signal(&more);
    pthread_mutex_unlock(&lock);
    return 1;
}

/* the lines not written yet, straight out without the locks: for a fatal signal's handler */
void mister_log_rescue(void) {
    size_t w = written, p = put_at;
    while (w != p && p - w <= RING) {
        size_t at = w % RING, n = p - w < RING - at ? p - w : RING - at;
        out(ring + at, n);
        w += n;
    }
}
