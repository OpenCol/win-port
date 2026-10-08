/* USER's rectangle functions, on 16-bit RECT and POINT (roadmap W4). Pure arithmetic: Win16
   and Win32 agree on what they do, so they are written out rather than converted. */
#include "w16.h"

void w16_SetRect(RECT16 *r, int l, int t, int ri, int b) W16(SetRect);
void w16_SetRect(RECT16 *r, int l, int t, int ri, int b)
{
    r->left = (int16_t)l; r->top = (int16_t)t; r->right = (int16_t)ri; r->bottom = (int16_t)b;
}

void w16_CopyRect(RECT16 *d, RECT16 *s) W16(CopyRect);
void w16_CopyRect(RECT16 *d, RECT16 *s)
{
    *d = *s;
}

void w16_OffsetRect(RECT16 *r, int dx, int dy) W16(OffsetRect);
void w16_OffsetRect(RECT16 *r, int dx, int dy)
{
    r->left += dx; r->right += dx; r->top += dy; r->bottom += dy;
}

void w16_InflateRect(RECT16 *r, int dx, int dy) W16(InflateRect);
void w16_InflateRect(RECT16 *r, int dx, int dy)
{
    r->left -= dx; r->right += dx; r->top -= dy; r->bottom += dy;
}

static int empty(const RECT16 *r)
{
    return r->left >= r->right || r->top >= r->bottom;
}

int w16_IntersectRect(RECT16 *d, RECT16 *a, RECT16 *b) W16(IntersectRect);
int w16_IntersectRect(RECT16 *d, RECT16 *a, RECT16 *b)
{
    RECT16 r;
    r.left = a->left > b->left ? a->left : b->left;
    r.top = a->top > b->top ? a->top : b->top;
    r.right = a->right < b->right ? a->right : b->right;
    r.bottom = a->bottom < b->bottom ? a->bottom : b->bottom;
    if (empty(&r)) {
        d->left = d->top = d->right = d->bottom = 0;
        return 0;
    }
    *d = r;
    return 1;
}

void w16_UnionRect(RECT16 *d, RECT16 *a, RECT16 *b) W16(UnionRect);
void w16_UnionRect(RECT16 *d, RECT16 *a, RECT16 *b)
{
    if (empty(a)) {
        *d = empty(b) ? (RECT16){ 0, 0, 0, 0 } : *b;
        return;
    }
    if (empty(b)) {
        *d = *a;
        return;
    }
    d->left = a->left < b->left ? a->left : b->left;
    d->top = a->top < b->top ? a->top : b->top;
    d->right = a->right > b->right ? a->right : b->right;
    d->bottom = a->bottom > b->bottom ? a->bottom : b->bottom;
}

int w16_PtInRect(RECT16 *r, POINT16 p) W16(PtInRect);
int w16_PtInRect(RECT16 *r, POINT16 p)
{
    return p.x >= r->left && p.x < r->right && p.y >= r->top && p.y < r->bottom;
}
