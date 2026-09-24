/* draw_string_curfont -- shipped at 1068:57b2.
 *
 * Ported by tools/port.py from matched/gfx/draw_string_curfont.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern FontRec far * far g_current_font;             DGROUP:0x2ac6
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *   extern unsigned char far g_draw_color;               DGROUP:0x337e
 *
 * The file's own Win, GRPort, FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1068_963d(void *ctx, char *msg);

extern col_i16 font_metrics(col_u16 hfont);
extern unsigned char TextColor(unsigned char v);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSDrawString_aligned(Win *w, col_u16 hfont, const char *s,
                                     col_i16 x, col_i16 y, RECT *clip, col_i16 align);

void draw_string_curfont(GRPort *p, const char *s, col_i16 x, col_i16 y, col_i16 align)
{
    char saved;
    col_i16 off;

    if (g_current_font == 0) {
        empty_1068_963d(&(*g_error_ctx), "Error: No current font selected");
        return;
    }
    if (align & 0x10) {
        off = font_metrics(g_current_font->hfont) / 8;
        saved = TextColor(g_draw_color);
        MSDrawString_aligned((*((Win * *)((unsigned char *)(p) + 0x22))), g_current_font->hfont, s, x + off, y + off,
                             &(*((RECT *)((unsigned char *)(p) + 0x8))), align);
        TextColor(saved);
    }
    MSDrawString_aligned((*((Win * *)((unsigned char *)(p) + 0x22))), g_current_font->hfont, s, x, y, &(*((RECT *)((unsigned char *)(p) + 0x8))), align);
}
