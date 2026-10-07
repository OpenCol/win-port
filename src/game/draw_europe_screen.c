/* draw_europe_screen: candidate for 1048:4ecd (28 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: draw_europe_screen
 * name-source: sibling-build
 *
 * Redraws the Europe window through `EuropeWin_DrawEurope`, with the object at SEG20:0x2fd2.
 * The shipped has a NOP before the `push cs`, which is the five-byte form TLINK leaves when it
 * shortens a call to an `extern` -- so the callee is declared and not defined here (rule 47).
 *
 * RENAMED 2026-09-26 from europe_redraw_main: the Mac build's draw_europe_screen(void), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between europe_change_mode and cargo_diff_adjust, where the Mac gap holds 1 functions and the Windows one 4, so the pairing inside it is by size and arity (the Mac body is 1.64x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void EuropeWin_DrawEurope(EuropeWin *o);

void draw_europe_screen(void)
{
    EuropeWin_DrawEurope(&g_europe_win);
}
