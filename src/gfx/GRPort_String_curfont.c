/* GRPort_String_curfont: candidate for 1068:57b2 (253 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_String_curfont
 * name-source: sibling-build
 *
 * The four text drawers at 1068:57b2, 58af, 59af and 5a6f are one routine written four
 * times, over two axes. The font is either a parameter or the CURRENT one -- the pointer
 * at DGROUP:0x2ac6, whose null is refused with "Error: No current font selected" -- and the
 * clip rectangle is either the port's own at +8 or the caller's. This is current font, port
 * rectangle, and a point:
 *
 *     font parameter, point       GRPort_String          1068:59af
 *     font parameter, rectangle   draw_string_shadowed_clipped  1068:5a6f
 *     current font,   point       this one                      1068:57b2
 *     current font,   rectangle   draw_string_curfont_clipped   1068:58af
 *
 * All four draw twice when bit 0x10 of `align` is set: a shadow copy offset down and right
 * by an eighth of the font height and in the colour at DGROUP:0x337e, then the string itself.
 * The point forms offset the COORDINATES, the rectangle forms offset a copy of the rectangle.
 *
 * The refusal is a real `return`, with its own epilogue rather than a jump to the one at the
 * end.
 *
 * NO `register` keyword appears, and none is wanted: Borland enregisters `x` and `align` on
 * its own at -Od, exactly as it does in `GRPort_String`, and marking either would move
 * it (rule 53). `saved` takes the highest frame slot at BP-3 and so is declared first, with
 * `off` below it at BP-6.
 *
 * RENAMED 2026-09-26 from draw_string_curfont: the Mac build's GRPort::String(char*, short, short, short), the overload without a font: this plus a string, x, y and the alignment flags; it draws with the current font. The suffix tells the overloads apart, as GRPort_Init_wh does. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int MSGetTextHeight(unsigned hfont);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void report_error_1068_963d(void *ctx, char *msg);
extern void MSDrawString(GrafRec *graf, unsigned hfont, const char *s,
                                     int x, int y, RECT *clip, int align);

void GRPort_String_curfont(GRPort *port, const char *s, int x, int y, int align)
{
    char saved;
    int off;

    if (g_current_font == 0) {
        report_error_1068_963d(&g_error_log, "Error: No current font selected");
        return;
    }
    if (align & 0x10) {
        off = MSGetTextHeight(g_current_font->hfont) / 8;
        saved = TextColor(g_draw_color);
        MSDrawString((GrafRec *)port->win, g_current_font->hfont, s, x + off, y + off,
                             &port->rect, align);
        TextColor(saved);
    }
    MSDrawString((GrafRec *)port->win, g_current_font->hfont, s, x, y, &port->rect, align);
}
