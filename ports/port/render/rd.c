/*
 * rd.c  The frame's triangles, gathered by list for a back end
 */

#include <math.h>
#include <string.h>

#include "host_cache.h"
#include "rd.h"
#include "rd_list.h"

uint32_t rd_palette[1024];
uint32_t rd_palette_serial;
int rd_palette_mode = 2;

static RD_FRAME first_frame;
RD_FRAME* rd_frame = &first_frame;
RD_QUALITY rd_q;

void rd_quality(const char* words) {
    rd_q.half = words != NULL && strstr(words, "half") != NULL;
    rd_q.fast = words != NULL && strstr(words, "fast") != NULL;
    rd_q.nofog = words != NULL && strstr(words, "nofog") != NULL;
    rd_q.nosort = words != NULL && strstr(words, "nosort") != NULL;
    rd_q.lite = words != NULL && strstr(words, "lite") != NULL;
}

#ifdef RD_SHARED_POOL
/* each list's part of the pool: its share of their recent most, each with a quarter more and 512 over. A list that
   ran out with the pool full is taken to want twice its part. Its most falls by a 64th a frame */
static void share_pool(RD_FRAME* f) {
    static int most[RD_LISTS];
    int want[RD_LISTS], i, sum = 0, at = 0;
    for (i = 0; i < RD_LISTS; i++) {
        int n = f->room[i] > 0 && f->ntris[i] >= f->room[i] ? f->room[i] * 2 : f->ntris[i];
        most[i] = n > most[i] - most[i] / 64 ? n : most[i] - most[i] / 64;
        want[i] = most[i] + most[i] / 4 + 512;
        sum += want[i];
    }
    for (i = 0; i < RD_LISTS; i++) {
        int room = (int)((int64_t)RD_POOL_TRIS * want[i] / sum);
        f->tris[i] = &f->pool[at];
#ifdef __3DS__
        f->info[i] = &f->info_pool[at];
#endif
        f->room[i] = room;
        at += room;
    }
    f->dropped = 0;
    f->moved = 0;
}

/* a list's part full: the parts laid out again, each keeping its triangles, the list given half of the pool's unused
   rest and the others the other half. 0 when none is left */
static int grow_part(RD_FRAME* f, int list) {
    int start[RD_LISTS], room[RD_LISTS], i, used = 0, spare, mine, at = 0;
    for (i = 0; i < RD_LISTS; i++) {
        used += f->ntris[i];
    }
    spare = RD_POOL_TRIS - used;
    if (spare <= 0) {
        return 0;
    }
    mine = (spare + 1) / 2;
    for (i = 0; i < RD_LISTS; i++) {
        room[i] = f->ntris[i] + (i == list ? mine : (spare - mine) / (RD_LISTS - 1));
        start[i] = at;
        at += room[i];
    }
    room[RD_LISTS - 1] += RD_POOL_TRIS - at;
    /* those moving up from the last, then those moving down from the first: none lands on one not yet moved */
    for (i = RD_LISTS - 1; i >= 0; i--) {
        if (&f->pool[start[i]] > f->tris[i]) {
            memmove(&f->pool[start[i]], f->tris[i], (size_t)f->ntris[i] * sizeof(RD_TRI));
#ifdef __3DS__
            memmove(&f->info_pool[start[i]], f->info[i], (size_t)f->ntris[i] * sizeof(RD_TRI_INFO));
#endif
        }
    }
    for (i = 0; i < RD_LISTS; i++) {
        if (&f->pool[start[i]] < f->tris[i]) {
            memmove(&f->pool[start[i]], f->tris[i], (size_t)f->ntris[i] * sizeof(RD_TRI));
#ifdef __3DS__
            memmove(&f->info_pool[start[i]], f->info[i], (size_t)f->ntris[i] * sizeof(RD_TRI_INFO));
#endif
        }
        f->tris[i] = &f->pool[start[i]];
#ifdef __3DS__
        f->info[i] = &f->info_pool[start[i]];
#endif
        f->room[i] = room[i];
    }
    f->moved++;
    return 1;
}
#endif

void rd_begin_frame(void) {
    int i;
#ifdef RD_SHARED_POOL
    share_pool(rd_frame);
#else
    for (i = 0; i < RD_LISTS; i++) {
        rd_frame->tris[i] = &rd_frame->pool[i * RD_MAX_TRIS];
        rd_frame->room[i] = RD_MAX_TRIS;
    }
#endif
    for (i = 0; i < RD_LISTS; i++) {
        rd_frame->ntris[i] = 0;
    }
    rd_frame->nstates = 0;
    rd_frame->nputs = 0;
    rd_frame->ngroups = 0;
#ifdef __PSP__
    rd_zrange_begin();
#endif
}

RD_FOG rd_fog;

void rd_background_plane(const RD_VTX* v) {
    memcpy(rd_frame->background, v, sizeof rd_frame->background);
}

void rd_background(uint32_t argb) {
    RD_VTX v[3];
    memset(v, 0, sizeof v);
    v[1].x = RD_WIDTH;
    v[2].y = RD_HEIGHT;
    v[0].base = v[1].base = v[2].base = argb;
    rd_background_plane(v);
}

/* the fog registers keep their values from frame to frame */
void rd_fog_table(const float* table) {
    int i;
    for (i = 0; i < 128; i++) {
        float f = table[i];
        rd_fog.table[i] = (float)(int)((f < 0.0f ? 0.0f : f > 1.0f ? 1.0f : f) * 255.0f + 0.5f) / 255.0f;
    }
}

void rd_fog_color(uint32_t argb) {
    rd_fog.color = argb;
}

void rd_fog_density(uint32_t reg) {
    rd_fog.density = (float)(reg >> 8 & 255) / 128.0f * ldexpf(1.0f, (int8_t)(reg & 255));
}

/* the frame's copy of s: the last one again when it is the same */
static __attribute__((section(".text.nlhot"))) int add_state(const RD_STATE* s) {
    if (rd_frame->nstates > 0 && rd_same_state(&rd_frame->states[rd_frame->nstates - 1], s)) {
        return rd_frame->nstates - 1;
    }
    if (rd_frame->nstates == RD_MAX_STATES) {
        return -1;
    }
    rd_frame->states[rd_frame->nstates] = *s;
#ifdef __3DS__
    rd_frame->seen[rd_frame->nstates] = 0;
#endif
    return rd_frame->nstates++;
}

static int add_tri(int list, int state, const RD_VTX* a, const RD_VTX* b, const RD_VTX* c, int share, int stays, int culled) {
    RD_TRI* t;
    int i = rd_frame->ntris[list];
    if (i >= rd_frame->room[list]) {
#ifdef RD_SHARED_POOL
        if (!grow_part(rd_frame, list)) {
            rd_frame->dropped++;
            return 0;
        }
#else
        return 0;
#endif
    }
#ifdef __PSP__
    rd_tri_z(list, a, b, c);
#endif
    t = &rd_frame->tris[list][rd_frame->ntris[list]++];
    host_lines_new(t, sizeof *t, rd_frame->tris[list] + rd_frame->room[list]);
    HOST_WRITTEN(HOST_W_TRIS, sizeof *t);
    t->state = (uint16_t)state;
    t->share = (uint8_t)share;
    t->stays = (uint8_t)stays;
    t->v[0] = *a;
    t->v[1] = *b;
    t->v[2] = *c;
#ifdef __3DS__
    if (rd_seen && !rd_frame->seen[state] && (list != RD_TRANS || !rd_unseen(a, b, c))) {
        rd_frame->seen[state] = 1;
    }
#endif
    (void)culled;   /* the 3DS finds it again with the triangle's info, as its lists are built */
    return 1;
}

int rd_discard;
#ifdef __3DS__
int rd_seen, rd_group_info;
#endif

/* the back ends' area, from the same corners in the same order: in floats where doubles are library calls (the
   PSP's FPU has none) */
#ifdef __PSP__
typedef float RD_AREA;
#else
typedef double RD_AREA;
#endif

static RD_AREA area(const RD_VTX* a, const RD_VTX* b, const RD_VTX* c) {
    return ((RD_AREA)b->x - a->x) * ((RD_AREA)c->y - a->y) - ((RD_AREA)b->y - a->y) * ((RD_AREA)c->x - a->x);
}

static int tiny(RD_AREA d) {
    return (d < 0 ? -d : d) < (RD_AREA)RD_CULL_VAL;
}

#ifdef __PSP__
int rd_items;

/* run item's last triangle added: its count of them kept, or it made a plain triangle */
static void run_end(int list, int item, int ntri) {
    RD_TRI* t = &rd_frame->tris[list][item];
    if (ntri == 1) {
        t->stays = 1;
    } else {
        t[1].state = (uint16_t)ntri;
    }
}

/* rd_strip with rd_items: its runs of triangles that stay, from one whose share isn't (0, 2), as RD_STRIP items */
static void strip_items(const RD_STATE* s, int st, const RD_VTX* v, int n) {
    int list = s->list, i, added = 0, item = -1, ntri = 0;
    for (i = 2; i < n; i++) {
        const RD_VTX *a = i & 1 ? &v[i - 1] : &v[i - 2], *b = i & 1 ? &v[i - 2] : &v[i - 1];
        int stays = 1, share;
        if (s->cull != 0) {
            RD_AREA d = area(a, b, &v[i]);
            if (s->cull >= 2 && (s->cull == 2 ? d < 0 : d > 0)) {
                if (item >= 0) {
                    run_end(list, item, ntri);
                    item = -1;
                }
                added = 0;
                continue;
            }
            stays = !tiny(d);
        }
        if (item >= 0 && stays) {
            RD_TRI* tris = rd_frame->tris[list];      /* read again: a full part's add_tri may move it */
            int k = ntri - 1;
            if (k % 3 != 0 || rd_frame->ntris[list] < rd_frame->room[list]) {
                if (k % 3 == 0) {
                    RD_TRI* x = &tris[rd_frame->ntris[list]++];
                    host_lines_new(x, sizeof *x, tris + rd_frame->room[list]);
                    HOST_WRITTEN(HOST_W_TRIS, sizeof *x);
                }
                rd_tri_z(list, a, b, &v[i]);
                tris[item + 1 + k / 3].v[k % 3] = v[i];
                ntri++;
                continue;
            }
        }
        if (item >= 0) {
            run_end(list, item, ntri);
            item = -1;
        }
        share = !added ? 0 : i & 1 ? RD_SHARE(2, 1) : RD_SHARE(0, 2);
        if (stays && share != RD_SHARE(0, 2)) {
            int at = rd_frame->ntris[list];
            added = add_tri(list, st, a, b, &v[i], share, 1 | RD_STRIP | (i & 1 ? RD_ODD : 0), 0);
            if (added) {
                item = at;
                ntri = 1;
            }
        } else {
            added = add_tri(list, st, a, b, &v[i], share, stays, 0);
        }
    }
    if (item >= 0) {
        run_end(list, item, ntri);
    }
}
#endif

void rd_strip(const RD_STATE* s, const RD_VTX* v, int n) {
    int st, i, added = 0;
    if (rd_discard || (st = add_state(s)) < 0) {
        return;
    }
#ifdef __PSP__
    if (rd_items) {
        strip_items(s, st, v, n);
        return;
    }
#endif
    for (i = 2; i < n; i++) {
        /* one winding for the whole strip */
        const RD_VTX *a = i & 1 ? &v[i - 1] : &v[i - 2], *b = i & 1 ? &v[i - 2] : &v[i - 1];
        int stays = 1;
        if (s->cull != 0) {
            RD_AREA d = area(a, b, &v[i]);
            /* every back end culls it by its winding (cull 2: area below 0, 3: above) */
            if (s->cull >= 2 && (s->cull == 2 ? d < 0 : d > 0)) {
                added = 0;
                continue;
            }
            stays = !tiny(d);
        }
        added = add_tri(s->list, st, a, b, &v[i], !added ? 0 : i & 1 ? RD_SHARE(2, 1) : RD_SHARE(0, 2), stays,
                        s->cull != 0 && !stays);
    }
}

#ifdef __3DS__
/* a quad's triangle culled by its state, by its area as the back ends find it */
static int quad_culled(const RD_STATE* s, const RD_VTX* a, const RD_VTX* b, const RD_VTX* c) {
    RD_AREA d;
    if (s->cull == 0) {
        return 0;
    }
    d = area(a, b, c);
    return tiny(d) || (s->cull == 2 && d < 0) || (s->cull == 3 && d > 0);
}
#else
#define quad_culled(s, a, b, c) 0
#endif

int rd_snap_x;

void rd_quad(const RD_STATE* s, const RD_VTX* v) {
    RD_VTX q[4];
    int st, first;
    if (rd_discard || (st = add_state(s)) < 0) {
        return;
    }
    if (rd_snap_x && v[0].x == v[3].x && v[1].x == v[2].x) {
        /* each side to the first column whose centre is at or past it */
        memcpy(q, v, sizeof q);
        q[0].x = q[3].x = ceilf(v[0].x - 0.5f);
        q[1].x = q[2].x = ceilf(v[1].x - 0.5f);
        v = q;
    }
#ifdef __PSP__
    if (rd_items) {
        RD_VTX d;
        rd_quad_last(&v[0], &v[1], &v[2], &d);
        if (memcmp(&d, &v[3], sizeof d) == 0) {
            if (add_tri(s->list, st, &v[0], &v[1], &v[2], 0, RD_QUAD | (s->cull == 0), 0)) {
                rd_tri_z(s->list, &v[0], &v[2], &v[3]);     /* its second triangle's */
            }
            return;
        }
    }
#endif
    first = add_tri(s->list, st, &v[0], &v[1], &v[2], 0, s->cull == 0, quad_culled(s, &v[0], &v[1], &v[2]));
    add_tri(s->list, st, &v[0], &v[2], &v[3], first ? RD_SHARE(0, 2) : 0, s->cull == 0, quad_culled(s, &v[0], &v[2], &v[3]));
}

int rd_gpu_tl;
int rd_gpu_tl_kinds = 7, rd_gpu_tl_max = RD_GPU_GROUPS, rd_gpu_tl_trans;
float rd_gpu_tl_trans_deep;
uint32_t rd_gpu_tl_deep_left;
int rd_gpu_tl_two_pass;
uint32_t rd_gpu_tl_two_passed;
int rd_gpu_tl_nearclip;
uint32_t rd_gpu_tl_near_left;

__attribute__((section(".text.nlhot"))) int rd_gpu_put(const float m[16], float near_w) {
    RD_GPU_PUT* p;
    if (!rd_gpu_tl || rd_discard || rd_frame->nputs == RD_GPU_PUTS) {
        return -1;
    }
    p = &rd_frame->puts[rd_frame->nputs];
    memcpy(p->m, m, sizeof p->m);
    p->near_w = near_w;
    return rd_frame->nputs++;
}

/* in words: gcc calls memcpy for a struct over 64 bytes */
static void copy_words(void* to, const void* from, size_t n) {
    uint32_t w;
    size_t i;
    for (i = 0; i < n / 4; i++) {
        memcpy(&w, (const char*)from + 4 * i, 4);
        memcpy((char*)to + 4 * i, &w, 4);
    }
}

_Static_assert(sizeof(RD_GPU_GROUP) % 4 == 0, "copied in words");

/* drawn where its list's triangles sent so far end */
__attribute__((section(".text.nlhot"))) int rd_gpu_group(int put, const RD_STATE* s, const RD_GPU_GROUP* g) {
    RD_GPU_DRAW* d;
    int st, deep = s->list == RD_TRANS && rd_gpu_tl_trans_deep > 0.0f && !(g->wlo > 0.0f && g->whi <= g->wlo * rd_gpu_tl_trans_deep);
    /* a deep translucent group is sorted whole, at one depth: the triangles it passes through would go wrong,
       but for a two-sided shell, whose back faces are behind its front ones */
    if (deep && !(rd_gpu_tl_two_pass && g->two_sided)) {
        rd_gpu_tl_deep_left++;
        return 0;
    }
    if (rd_gpu_tl_nearclip && put >= 0 && put < rd_frame->nputs && !(g->wlo >= rd_frame->puts[put].near_w)) {
        rd_gpu_tl_near_left++;
        return 0;
    }
    if (put < 0 || put >= rd_frame->nputs || rd_frame->ngroups + 1 + deep > rd_gpu_tl_max || !(rd_gpu_tl_kinds >> g->kind & 1) ||
        (rd_gpu_take != NULL && !rd_gpu_take(s, g, rd_frame->puts[put].m)) || (st = add_state(s)) < 0) {
        return 0;
    }
    d = &rd_frame->groups[rd_frame->ngroups++];
    copy_words(&d->g, g, sizeof *g);
    d->g.faces = 0;
    d->put = (uint16_t)put;
    d->state = (uint16_t)st;
#ifdef __3DS__
    rd_frame->seen[st] = 1;
#endif
    d->at = rd_frame->ntris[s->list];
    if (deep) {
        d->g.faces = 1;
        d->g.key = g->key_far;
        d[1] = d[0];
        d[1].g.faces = 2;
        d[1].g.key = g->key_near;
        rd_frame->ngroups++;
        rd_gpu_tl_two_passed++;
    }
#ifdef __3DS__
    if (rd_group_info) {
        int k;
        for (k = 0; k <= deep; k++) {
            RD_GPU_INFO q;
            q.wlo = g->wlo;
            q.whi = g->whi;
            q.key = !deep ? g->key : k == 0 ? g->key_far : g->key_near;
            q.near_w = rd_frame->puts[put].near_w;
            q.at = rd_frame->ntris[s->list];
            q.mesh = (int16_t)(g->mesh >= 0 && g->mesh <= 32767 ? g->mesh : -1);
            q.list = s->list;
            q.free = (uint8_t)rd_order_free(s);
            rd_frame->ginfo[d - rd_frame->groups + k] = q;
        }
    }
#endif
    return 1;
}

int (*rd_gpu_take)(const RD_STATE* s, const RD_GPU_GROUP* g, const float m[16]);

#if !defined(__3DS__) && !defined(__PSP__)
/* only the 3DS's and the PSP's back ends draw them */
int rd_gpu_mesh_make(const NL_MESH* m) {
    (void)m;
    return -1;
}

void rd_gpu_mesh_free(int id) {
    (void)id;
}

uint32_t rd_gpu_mesh_room(void) {
    return 0;
}
#endif
