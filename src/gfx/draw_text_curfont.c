/* draw_text_curfont -- shipped at 1068:5b30.
 *
 * Ported by tools/port.py from matched/gfx/draw_text_curfont.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern FontRec far * far g_current_font;             DGROUP:0x2ac6
 *   extern char far g_error_ctx;                         DGROUP:0x2ac0
 *
 * The file's own MSWin, GRPort, FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void empty_1068_963d(void *ctx, char *msg);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSDrawText(MSWin *w, col_u16 hfont, const char *s, RECT *r,
                           col_i16 flags);

void draw_text_curfont(GRPort *p, const char *s, RECT *r, col_i16 flags)
{
    if (g_current_font == 0) {
        empty_1068_963d(&(*g_error_ctx), "Error: No current font selected");
        return;
    }
    MSDrawText((*((MSWin * *)((unsigned char *)(p) + 0x22))), g_current_font->hfont, s, r, flags);
}
