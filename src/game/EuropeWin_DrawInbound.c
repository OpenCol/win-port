/* EuropeWin_DrawInbound: candidate for 1048:95c1 (459 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawInbound
 * name-source: sibling-build
 *
 * Draws the outbound ("Bound for") panel of the Europe screen: the 0x8b by 0x79 cell at (0x8f, 0x102) repainted,
 * a heading from the word at SEG20:0xdcdc, the nation's own name under it, and then every unit
 * in the two Europe pseudo-squares laid along a row.
 *
 * THE TWO SQUARES ARE THE NATION'S INDEX MINUS 0x1c AND MINUS 0x18, used as BOTH coordinates
 * of `top_unit_in_square` -- the same number for x and y. Europe and the high seas are not on the map, so
 * the game gives each nation a pair of negative diagonal positions and hangs its unit chains
 * there; this function walks both and draws them into one row, so a ship at the dock and a
 * ship at sea share the slot counter.
 *
 * That counter is `slot`, passed BY ADDRESS to `EuropeWin_DrawTransitUnit` and never read here, so the
 * drawing helper is what advances it -- which is why the second chain continues where the
 * first left off rather than starting over.
 *
 * RENAMED 2026-09-26 from europe_draw_dock_1048_95c1: its twin suffix told it apart from two panels that now have Mac names; it is the centre sailing panel (x 0x8f), the partner of EuropeWin_DrawOutBound (x 2).
 *
 * RENAMED 2026-09-26 from europe_draw_centre: the Mac build's EuropeWin::DrawOutBound(short): this plus a refresh flag; its panel is headed MISC_BOUND_FOR and the nation's colonial name -- the ships on their way OUT to the New World. The width matches.
 *
 * RENAMED 2026-09-27 from EuropeWin_DrawOutBound: CONST PROPOSAL, SETTLED AS A SWAP with 978c. Mac DrawTransit calls DrawInbound FIRST, then DrawOutBound; Windows EuropeWin_DrawTransit calls 95c1 first, then 978c. Layout: Mac DrawEuropeShips, DrawInbound (0x5618), DrawOutBound (0x5824), DrawTransit; Windows DrawEuropeShips, 95c1, 978c, DrawTransit. Callees: Mac DrawInbound has say_string, String x2 and a string append, which is 95c1's set (say_string, STRCAT, GRPort_String x2); Mac DrawOutBound has strings and String x1, which is 978c's (strings, STRCPY, String x1). Constants: the values ONLY Mac DrawInbound holds, 0x8f, -0x1c, -0x18, 0x90 and 0x34, are 95c1's panel x, its two nation squares (nation-0x1c, nation-0x18), its row x and the nation-record stride. The values only Mac DrawOutBound holds, 0x47, -0x10, -0xc, 0x3 and 0x2, are 978c's. Arity: this+short on all four. Size: 524/454 Mac against 459/383, consistent in this pairing. The earlier binding rested on the English reading of MISC_BOUND_FOR ('on their way OUT'); the developers' 'Inbound' is inbound to the New World. Swap through a temporary. The header prose of both files describes the panels and should keep its content under the new names.
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
/* PORT: this file's own DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, SRegion, EuropeWin, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-










#include "records.h"

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int align);

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);

/* 5-byte NOP form, so extern (rule 47) */
extern void EuropeWin_UpdateEurope(EuropeWin *w, int x, int y, int cx, int cy);
extern void EuropeWin_DrawTransitUnit(EuropeWin *w, int u, int x, int count3, int extra,
                              int *slot, int colour);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void set_europe(int n);
extern "C" void copy_rect_to_screen(EuropeWin *win, RECT *r);

void EuropeWin_DrawInbound(EuropeWin *w, int refresh)
{
    int slot;
    RECT r;
    char buf[0x50];
    register int u;
    register int x;

    set_europe(w->nation);
    EuropeWin_UpdateEurope(w, 0x8f, 0x102, 0x8b, 0x79);
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_BOUND_FOR]);
    TextColor(0x77);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0xd4, w->rect.top + 0x10c, 0);
    buf[0] = 0;
    STRCAT(buf, g_nation_text[g_cur_nation].country);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font_small, buf, w->rect.left + 0xd4, w->rect.top + 0x11b, 0);
    x = 0x90;
    slot = 0;
    u = top_unit_in_square(g_cur_nation - 0x1c, g_cur_nation - 0x1c);
    while (u >= 0) {
        EuropeWin_DrawTransitUnit(w, u, x, 0xd, 1, &slot, -1);
        u = next_unit(u);
    }
    u = top_unit_in_square(g_cur_nation - 0x18, g_cur_nation - 0x18);
    while (u >= 0) {
        EuropeWin_DrawTransitUnit(w, u, x, 0xd, 1, &slot, -1);
        u = next_unit(u);
    }
    if (refresh != 0) {
        SetRect(&r, w->rect.left + 0x8f, w->rect.top + 0x102, w->rect.left + 0x11a, w->rect.top + 0x17b);
        copy_rect_to_screen(w, &r);
    }
}
