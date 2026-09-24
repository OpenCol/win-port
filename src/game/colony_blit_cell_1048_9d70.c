/* colony_blit_cell_1048_9d70 -- shipped at 1048:9d70.
 *
 * Ported by tools/port.py from matched/game/colony_blit_cell_1048_9d70.c.
 *
 * The file's own ColonyWin dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: ox -> left, oy -> top
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal CopyRect(RECT *dst, RECT *src);
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern void CopyToPort(void *src, void *dst, RECT *sr, RECT *dr);

void colony_blit_cell_1048_9d70(ColonyWin *o, col_i16 x, col_i16 y, col_i16 w, col_i16 h)
{
    RECT r1;
    RECT r2;

    SetRect(&r1, x, y, x + w, y + h);
    CopyRect(&r2, &r1);
    OffsetRect(&r2, o->left, o->top);
    CopyToPort(((char *)((unsigned char *)(o) + 0x884)), o, &r1, &r2);
}
