/* PORT: this file's own declarations of MSDrawString_rect removed: the header declares it from the definition (W1 step 3). */
/* draw_string_curfont_clipped: candidate for 1068:58af (256 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: draw_string_curfont_clipped
 * name-source: descriptive
 *
 * The fourth corner of the text-drawing square at 1068:57b2..5b27: CURRENT font, CALLER's
 * clip rectangle. The font is the pointer at DGROUP:0x2ac6 and a null one is refused
 * with "Error: No current font selected". The test is a 32-bit compare --
 * `66 26 83 3E C6 2A 00` -- so it is the whole pointer being compared against 0 and not
 * just its offset. See `GRPort_String_curfont` (1068:57b2) for the family table.
 *
 * Everything after that is `draw_string_shadowed_clipped` (1068:5a6f): with bit 0x10 of
 * `align` set, copy the caller's clip rectangle into a local, move the copy down and right
 * by an eighth of the font height, swap the colour byte at DGROUP:0x337e for the shadow, draw
 * and put the colour back; then draw for real through the caller's own rectangle.
 *
 * The refusal is a real `return` -- it has its own epilogue rather than jumping to the one at
 * the end -- which is what makes the function 256 bytes and not 252.
 *
 * `align` is copied into the first of two `register` locals rather than marked `register`
 * itself (rule 53); `saved` takes the highest frame slot and is declared first.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int MSGetTextHeight(unsigned hfont);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void report_error_1068_963d(void *ctx, char *msg);

void draw_string_curfont_clipped(GRPort *port, const char *s, RECT *clip,
                                      int align)
{
    register int flags;
    register int off;
    char saved;
    RECT r;

    flags = align;
    if (g_current_font == 0) {
        report_error_1068_963d(&g_error_log, "Error: No current font selected");
        return;
    }
    if (flags & 0x10) {
        off = MSGetTextHeight(g_current_font->hfont) / 8;
        r = *clip;
        OffsetRect(&r, off, off);
        saved = TextColor(g_draw_color);
        MSDrawString_rect((GrafRec *)port->win, g_current_font->hfont, s, &r, flags);
        TextColor(saved);
    }
    MSDrawString_rect((GrafRec *)port->win, g_current_font->hfont, s, clip, flags);
}
