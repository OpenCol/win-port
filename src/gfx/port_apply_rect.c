/* port_apply_rect -- shipped at 1068:4dba.
 *
 * Ported by tools/port.py from matched/gfx/port_apply_rect.c.
 *
 * The file's own Win, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void empty_1070_af4e(Win *w, RECT *r);
void port_apply_rect(GRPort *p)
{
    SetRect(&(*((RECT *)((unsigned char *)(p) + 0x8))), (*((RECT *)((unsigned char *)(p) + 0x10))).left, (*((RECT *)((unsigned char *)(p) + 0x10))).top, (*((RECT *)((unsigned char *)(p) + 0x10))).right, (*((RECT *)((unsigned char *)(p) + 0x10))).bottom);
    empty_1070_af4e((*((Win * *)((unsigned char *)(p) + 0x22))), &(*((RECT *)((unsigned char *)(p) + 0x8))));
}
