/* draw_string_shadowed -- shipped at 1068:59af.
 *
 * Ported by tools/port.py from matched/gfx/draw_string_shadowed.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own MSWin, GRPort dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i16 font_metrics(col_u16 hfont);
extern unsigned char TextColor(unsigned char v);
extern void MSDrawString_aligned(MSWin *w, col_u16 hfont, const char *s,
                                     col_i16 x, col_i16 y, RECT *clip, col_i16 align);

void draw_string_shadowed(GRPort *p, col_u16 *hf, const char *s,
                              col_i16 x, col_i16 y, col_i16 align)
{
    char saved;
    col_i16 off;

    if (align & 0x10) {
        off = font_metrics(*hf) / 8;
        saved = TextColor(g_draw_color);
        MSDrawString_aligned((*((MSWin * *)((unsigned char *)(p) + 0x22))), *hf, s, x + off, y + off, &(*((RECT *)((unsigned char *)(p) + 0x8))), align);
        TextColor(saved);
    }
    MSDrawString_aligned((*((MSWin * *)((unsigned char *)(p) + 0x22))), *hf, s, x, y, &(*((RECT *)((unsigned char *)(p) + 0x8))), align);
}
