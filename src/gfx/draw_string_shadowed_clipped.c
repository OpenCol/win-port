/* PORT: this file's own declarations of MSDrawString_rect removed: the header declares it from the definition (W1 step 3). */
/* draw_string_shadowed_clipped: candidate for 1068:5a6f (193 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: draw_string_shadowed_clipped
 * name-source: descriptive
 *
 * `GRPort_String`'s twin, written immediately after it and differing in exactly two
 * things: the clip rectangle comes from the CALLER instead of the port, and the shadow is
 * made by offsetting that rectangle rather than the coordinates.
 *
 * So the shadow pass copies the caller's rectangle into a local -- an eight-byte struct
 * assignment, which Borland does through `F_SCOPY` with CX = 8 -- moves the copy down and
 * right by an eighth of the font height, swaps the global colour byte for the shadow colour
 * at DGROUP:0x337e, draws, and puts the colour back. The real pass then draws through the
 * caller's own rectangle. Both passes go to `MSDrawString_rect`, which positions inside a
 * rectangle, where the sibling uses `MSDrawString`, which positions at a point.
 *
 * The register allocation is rule 53 and took three spellings. `align` reaches SI by being
 * COPIED into a register local declared first, not by being marked `register` itself: a
 * register parameter is allocated after every register local, so marking the parameter puts
 * `off` in SI and the parameter in DI -- the wrong way round, at +0x10. Two register locals
 * in declaration order give SI then DI, which is what the shipped code has.
 *
 * `saved` takes the highest frame slot at BP-3 and so is declared first, with the rectangle
 * below it at BP-0xc.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern int MSGetTextHeight(unsigned hfont);


void draw_string_shadowed_clipped(GRPort *port, unsigned *hf, const char *s,
                                      RECT *clip, int align)
{
    register int flags;
    register int off;
    char saved;
    RECT r;

    flags = align;
    if (flags & 0x10) {
        off = MSGetTextHeight(*hf) / 8;
        r = *clip;
        OffsetRect(&r, off, off);
        saved = TextColor(g_draw_color);
        MSDrawString_rect((GrafRec *)port->win, *hf, s, &r, flags);
        TextColor(saved);
    }
    MSDrawString_rect((GrafRec *)port->win, *hf, s, clip, flags);
}
