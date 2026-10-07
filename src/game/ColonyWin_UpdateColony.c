/* ColonyWin_UpdateColony: candidate for 1010:bd3f (129 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_UpdateColony
 * name-source: sibling-build
 *
 * Copies the w x h cell at (x, y) of the colony window's offscreen port
 * (+0x7a8) to the window, shifted by the window origin at +0x76a/+0x76c.
 *
 * RENAMED 2026-09-26 from colony_blit_cell_1010_bd3f: its 1048 twin was the Europe window's (now EuropeWin_UpdateEurope), so the suffix no longer tells two copies apart.
 *
 * RENAMED 2026-09-27 from colony_blit_cell: Mac ColonyWin::UpdateColony(short, short, short, short), 180 bytes, against 129. Callers: DrawBuildings, DrawColonists, DrawColony, DrawColonyShips, DrawMarket and DrawMiscellaneous, 6 of 6, on both. Callee: GRPort::CopyToPort (CopyToPort) only. Layout: after LoadColony on both (Mac DoTheClosing follows). Arity: this+4 = (o, x, y, w, h). mac_bind align 6.0/0.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

void ColonyWin_UpdateColony(ColonyWin *o, int x, int y, int w, int h)
{
    RECT r1;
    RECT r2;

    SetRect(&r1, x, y, x + w, y + h);
    CopyRect(&r2, &r1);
    OffsetRect(&r2, o->rect.left, o->rect.top);
    GRPort_CopyToPort(&o->back, (GRPort *)o, &r1, &r2);
}
