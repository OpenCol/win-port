/* draw_string_shadowed_clipped -- shipped at 1068:5a6f.
 *
 * Ported by tools/port.py from matched/gfx/draw_string_shadowed_clipped.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own Win, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
#endif

extern col_i16 font_metrics(col_u16 hfont);
extern unsigned char TextColor(unsigned char v);
extern void MSDrawString(Win *w, col_u16 hfont, const char *s, RECT *r,
                             col_i16 flags);


void draw_string_shadowed_clipped(GRPort *p, col_u16 *hf, const char *s,
                                      RECT *clip, col_i16 align)
{
    register col_i16 flags;
    register col_i16 off;
    char saved;
    RECT r;

    flags = align;
    if (flags & 0x10) {
        off = font_metrics(*hf) / 8;
        r = *clip;
        OffsetRect(&r, off, off);
        saved = TextColor(g_draw_color);
        MSDrawString((*((Win * *)((unsigned char *)(p) + 0x22))), *hf, s, &r, flags);
        TextColor(saved);
    }
    MSDrawString((*((Win * *)((unsigned char *)(p) + 0x22))), *hf, s, clip, flags);
}
