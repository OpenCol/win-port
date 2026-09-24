/* port_set_clip -- shipped at 1068:4e10.
 *
 * Ported by tools/port.py from matched/gfx/port_set_clip.c.
 *
 * The file's own MSWin, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal IntersectRect(RECT *dst, RECT *a, RECT *b);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void empty_1070_af4e(MSWin *w, RECT *r);
void port_set_clip(GRPort *p, RECT *r)
{
    SetRect(&(*((RECT *)((unsigned char *)(p) + 0x8))), r->left, r->top, r->right, r->bottom);
    IntersectRect(&(*((RECT *)((unsigned char *)(p) + 0x8))), &(*((RECT *)((unsigned char *)(p) + 0x8))), &(*((RECT *)((unsigned char *)(p) + 0x10))));
    empty_1070_af4e((*((MSWin * *)((unsigned char *)(p) + 0x22))), &(*((RECT *)((unsigned char *)(p) + 0x8))));
}
