/*
 * nl_xmtr.c  The NAOMI library's matrices (nlxmtr, nlvect). XMTRX is a plain array: 4x4 by rows,
 * p' = p x XMTRX, each transform in front; 0x10000 a turn.
 */

#include <math.h>
#include <string.h>

typedef float NLMTX[16];

typedef struct {
    float x, y, z;
} NLV3;

typedef struct {
    float x, y, z, w;
} NLV4;

typedef struct {
    short count;
    short max;
    NLMTX* base;
    NLMTX* matrix;  /* XMTRX is written back here */
} NLMATRIXSTACK;

#define VIEW_DEPTH 8
#define PROJ_DEPTH 8

NLMATRIXSTACK nlMatrixStack[4];
static unsigned char matrix_mode = 1;
static NLMTX xm;
static NLMTX view_stack[VIEW_DEPTH + 1];
static NLMTX proj_stack[PROJ_DEPTH + 1];

static const NLMTX unit = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };

static void mul(float* d, const float* a, const float* b) {
    NLMTX t;
    int r, c;
    for (r = 0; r < 4; r++) {
        for (c = 0; c < 4; c++) {
            t[r * 4 + c] = a[r * 4] * b[c] + a[r * 4 + 1] * b[4 + c] + a[r * 4 + 2] * b[8 + c] + a[r * 4 + 3] * b[12 + c];
        }
    }
    memcpy(d, t, sizeof t);
}

static float sin_a(int ang) {
    return (float)sin((ang & 0xffff) * (6.283185307179586 / 65536.0));
}

static float cos_a(int ang) {
    return (float)cos((ang & 0xffff) * (6.283185307179586 / 65536.0));
}

/* ---- the stacks ---- */

static void stack_init(NLMATRIXSTACK* s, NLMTX* base, int n) {
    s->count = 0;
    s->max = (short)(n - 1);
    s->base = base;
    s->matrix = base;
    memcpy(base, unit, sizeof unit);
}

/* the model stack is the game's array, the others the library's; nlMatrixStack[0] is the one in
   use, 1 to 3 keep each while another is */
NLMTX* nlInitMatrix(NLMTX* m, int n) {
    NLMTX* base = (NLMTX*)(((unsigned long)m + 31) & ~31ul);
    stack_init(&nlMatrixStack[1], base, n);
    stack_init(&nlMatrixStack[2], view_stack, VIEW_DEPTH);
    stack_init(&nlMatrixStack[3], proj_stack, PROJ_DEPTH);
    matrix_mode = 1;
    nlMatrixStack[0] = nlMatrixStack[1];
    memcpy(xm, unit, sizeof xm);
    return base;
}

void nlLoadMatrix(const NLMTX* m) {
    memcpy(xm, m, sizeof xm);
}

/* NULL: XMTRX written back to the current matrix */
NLMTX* nlGetMatrix(NLMTX* m) {
    if (m == NULL) {
        m = nlMatrixStack[0].matrix;
    }
    memcpy(m, xm, sizeof xm);
    return m;
}

void nlMatrixMode(int mode) {
    nlGetMatrix(NULL);
    nlMatrixStack[matrix_mode] = nlMatrixStack[0];
    matrix_mode = (unsigned char)mode;
    nlMatrixStack[0] = nlMatrixStack[matrix_mode];
    nlLoadMatrix(nlMatrixStack[0].matrix);
}

/* XMTRX pushed; then m (NULL: the same) loaded */
int nlPushMatrix(const NLMTX* m) {
    NLMATRIXSTACK* s = &nlMatrixStack[0];
    int ok = s->count < s->max;
    memcpy(s->matrix, xm, sizeof xm);
    s->count++;
    if (ok) {
        s->matrix++;
    }
    if (m != NULL) {
        memcpy(xm, m, sizeof xm);
    }
    return ok;
}

static int pop(int n, int load) {
    NLMATRIXSTACK* s = &nlMatrixStack[0];
    int ok;
    if (n <= 0) {
        n = 1;
    }
    ok = s->count >= n;
    s->count = (short)(ok ? s->count - n : 0);
    if (s->count < s->max) {
        s->matrix = s->base + s->count;
    }
    if (load) {
        memcpy(xm, s->matrix, sizeof xm);
    }
    return ok;
}

int nlPopMatrix(int n) {
    return pop(n, 1);
}

int nlDecMatrixPtr(int n) {
    return pop(n, 0);
}

/* XMTRX = the matrix pushed last x XMTRX */
void nlMultiPushedMatrix(void) {
    mul(xm, *(nlMatrixStack[0].matrix - 1), xm);
}

void nlMultiMatrix(const NLMTX* m) {
    mul(xm, *m, xm);
}

NLMTX* nlGetMatrix8(NLMTX* m) {
    return nlGetMatrix(m);
}

void nlLoadMatrix8(const NLMTX* m) {
    nlLoadMatrix(m);
}

void nlMultiMatrix8(const NLMTX* m) {
    nlMultiMatrix(m);
}

void nlUnitMatrix(void) {
    memcpy(xm, unit, sizeof xm);
}

/* ---- transforms, each in front of XMTRX ---- */

void nlTranslate(float x, float y, float z) {
    int c;
    for (c = 0; c < 4; c++) {
        xm[12 + c] += x * xm[c] + y * xm[4 + c] + z * xm[8 + c];
    }
}

void nlTranslateV(const NLV3* v) {
    nlTranslate(v->x, v->y, v->z);
}

void nlScale(float x, float y, float z) {
    int c;
    for (c = 0; c < 4; c++) {
        xm[c] *= x;
        xm[4 + c] *= y;
        xm[8 + c] *= z;
    }
}

void nlScaleV(const NLV3* v) {
    nlScale(v->x, v->y, v->z);
}

/* rows i and j of XMTRX turned by ang */
static void rotate(int i, int j, int ang) {
    float s = sin_a(ang), c = cos_a(ang);
    int k;
    for (k = 0; k < 4; k++) {
        float a = xm[i * 4 + k], b = xm[j * 4 + k];
        xm[i * 4 + k] = c * a + s * b;
        xm[j * 4 + k] = -s * a + c * b;
    }
}

void nlRotateAX(int ang) {
    rotate(1, 2, ang);
}

void nlRotateAY(int ang) {
    rotate(2, 0, ang);
}

void nlRotateAZ(int ang) {
    rotate(0, 1, ang);
}

/* ---- points and vectors ---- */

void nlCalcPoint(const NLV3* s, NLV3* d) {
    float x = s->x, y = s->y, z = s->z;
    d->x = x * xm[0] + y * xm[4] + z * xm[8] + xm[12];
    d->y = x * xm[1] + y * xm[5] + z * xm[9] + xm[13];
    d->z = x * xm[2] + y * xm[6] + z * xm[10] + xm[14];
}

void nlCalcPoint4(const NLV3* s, NLV4* d) {
    float x = s->x, y = s->y, z = s->z;
    d->x = x * xm[0] + y * xm[4] + z * xm[8] + xm[12];
    d->y = x * xm[1] + y * xm[5] + z * xm[9] + xm[13];
    d->z = x * xm[2] + y * xm[6] + z * xm[10] + xm[14];
    d->w = x * xm[3] + y * xm[7] + z * xm[11] + xm[15];
}

void nlCalcVector(const NLV3* s, NLV3* d) {
    float x = s->x, y = s->y, z = s->z;
    d->x = x * xm[0] + y * xm[4] + z * xm[8];
    d->y = x * xm[1] + y * xm[5] + z * xm[9];
    d->z = x * xm[2] + y * xm[6] + z * xm[10];
}

/* returns the length before */
float nlUnitVector(NLV3* v) {
    float n = sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
    if (n != 0.0f) {
        float r = 1.0f / n;
        v->x *= r;
        v->y *= r;
        v->z *= r;
    }
    return n;
}

/* +X right, +Y up, +Z towards the eye */
void nlLookAt(const NLV3* eye, const NLV3* obj, int z) {
    NLV3 f, r, u;
    NLMTX m;
    f.x = eye->x - obj->x;
    f.y = eye->y - obj->y;
    f.z = eye->z - obj->z;
    nlUnitVector(&f);
    r.x = f.z;              /* (0, 1, 0) x f */
    r.y = 0.0f;
    r.z = -f.x;
    if (nlUnitVector(&r) == 0.0f) {
        r.x = 1.0f;
    }
    u.x = f.y * r.z - f.z * r.y;
    u.y = f.z * r.x - f.x * r.z;
    u.z = f.x * r.y - f.y * r.x;
    m[0] = r.x, m[1] = u.x, m[2] = f.x, m[3] = 0.0f;
    m[4] = r.y, m[5] = u.y, m[6] = f.y, m[7] = 0.0f;
    m[8] = r.z, m[9] = u.z, m[10] = f.z, m[11] = 0.0f;
    m[12] = -(eye->x * r.x + eye->y * r.y + eye->z * r.z);
    m[13] = -(eye->x * u.x + eye->y * u.y + eye->z * u.z);
    m[14] = -(eye->x * f.x + eye->y * f.y + eye->z * f.z);
    m[15] = 1.0f;
    if (z != 0) {
        NLMTX t;
        memcpy(t, unit, sizeof t);
        t[0] = t[5] = cos_a(z);
        t[1] = sin_a(z);
        t[4] = -t[1];
        mul(m, m, t);
    }
    mul(xm, m, xm);
}

/* ---- for nlproj.c's assembler routines ---- */

/* XMTRX = XMTRX x a x b */
void nl_host_mul_matrix_ftrv(const NLMTX* a, const NLMTX* b) {
    mul(xm, xm, *a);
    mul(xm, xm, *b);
}

/* as load_matrix_ftrv_asm: each row (m3, m0, m1, -m3) */
void nl_host_load_matrix_ftrv(const NLMTX* m) {
    int r;
    for (r = 0; r < 4; r++) {
        const float* s = *m + r * 4;
        xm[r * 4 + 0] = s[3];
        xm[r * 4 + 1] = s[0];
        xm[r * 4 + 2] = s[1];
        xm[r * 4 + 3] = -s[3];
    }
}

/* ---- for the model routines (nl_obj.c) ---- */

const float* nl_xmtrx(void) {
    return xm;
}

const float* nl_view_matrix(void) {
    return *nlMatrixStack[2].matrix;
}
