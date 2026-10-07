/* GRPort_String: candidate for 1068:59af (192 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GRPort_String
 * name-source: sibling-build
 *
 * Draws a string twice when bit 0x10 of `align` is set: first a shadow copy, offset
 * down-and-right by an eighth of the font height and in the shadow colour, then the
 * string itself in the current colour. The shadow pass swaps the global colour byte and
 * puts the old value back afterwards. Called from 288 sites, more than any other
 * function of its size in the binary.
 *
 * RENAMED 2026-09-26 from draw_string_shadowed: the Mac build's GRPort::String(Font&, char*, short, short, short): this plus a font, a string, x, y and the alignment flags, in that order. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int MSGetTextHeight(unsigned hfont);

extern void MSDrawString(GrafRec *graf, unsigned hfont, const char *s,
                                     int x, int y, RECT *clip, int align);

void GRPort_String(GRPort *port, unsigned *hf, const char *s,
                              int x, int y, int align)
{
    char saved;
    int off;

    if (align & 0x10) {
        off = MSGetTextHeight(*hf) / 8;
        saved = TextColor(g_draw_color);
        MSDrawString((GrafRec *)port->win, *hf, s, x + off, y + off, &port->rect, align);
        TextColor(saved);
    }
    MSDrawString((GrafRec *)port->win, *hf, s, x, y, &port->rect, align);
}
