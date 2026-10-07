/* ColonyWin_ColonyMouseMisc: candidate for 1010:5ee7 (67 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * A click on panel 4 by the mode byte at DGROUP:0xc0f: only mode 1 does
 * anything (ColonyWin_ColonyMouseUnits); modes 0 and 2 are explicit no-ops.
 *
 * RENAMED 2026-09-26 from colony_panel4_click: the Mac build's ColonyWin::ColonyMouseMisc(short, short), bound by LAYOUT inside an uneven gap: both builds keep source-file order, and this sits between ColonyWin_ColonyMouseMap and ColonyWin_ColonyClickUnits, where the Mac gap holds 2 functions and the Windows one 3, so the pairing inside it is by size and arity (the Mac body is 1.22x this one, the arity agrees). The names corroborate it, which is what the pairing needed: an uneven gap alone does not settle order.
 */

#include "colonization.h"

typedef struct { char pad[1]; } ColonyWin;

extern void ColonyWin_ColonyMouseUnits(void *w, int x, int y);

void ColonyWin_ColonyMouseMisc(ColonyWin *w, int x, int y)
{
    switch (g_colony_mode) {
    case 0:
        break;
    case 1:
        ColonyWin_ColonyMouseUnits(w, x, y);
        break;
    case 2:
        break;
    }
}
