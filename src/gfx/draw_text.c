/* draw_text -- shipped at 1068:5b90.
 *
 * Ported by tools/port.py from matched/gfx/draw_text.c.
 *
 * The file's own MSWin, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void MSDrawText(MSWin *w, col_u16 hfont, const char *s, RECT *r,
                           col_i16 flags);

void draw_text(GRPort *p, col_u16 *hf, const char *s,
                             RECT *r, col_i16 flags)
{
    MSDrawText((*((MSWin * *)((unsigned char *)(p) + 0x22))), *hf, s, r, flags);
}
