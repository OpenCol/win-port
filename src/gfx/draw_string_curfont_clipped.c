/* draw_string_curfont_clipped -- shipped at 1068:58af.
 *
 * Ported by tools/port.py from matched/gfx/draw_string_curfont_clipped.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern FontRec far * far g_current_font;             DGROUP:0x2ac6
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own MSWin, GRPort, FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1068_963d(void *ctx, char *msg);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal OffsetRect(RECT *r, col_i16 dx, col_i16 dy);
#endif

extern col_i16 font_metrics(col_u16 hfont);
extern unsigned char TextColor(unsigned char v);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSDrawString(MSWin *w, col_u16 hfont, const char *s, RECT *r,
                             col_i16 flags);


void draw_string_curfont_clipped(GRPort *p, const char *s, RECT *clip,
                                      col_i16 align)
{
    register col_i16 flags;
    register col_i16 off;
    char saved;
    RECT r;

    flags = align;
    if (g_current_font == 0) {
        empty_1068_963d(&(*g_error_ctx), "Error: No current font selected");
        return;
    }
    if (flags & 0x10) {
        off = font_metrics(g_current_font->hfont) / 8;
        r = *clip;
        OffsetRect(&r, off, off);
        saved = TextColor(g_draw_color);
        MSDrawString((*((MSWin * *)((unsigned char *)(p) + 0x22))), g_current_font->hfont, s, &r, flags);
        TextColor(saved);
    }
    MSDrawString((*((MSWin * *)((unsigned char *)(p) + 0x22))), g_current_font->hfont, s, clip, flags);
}
