/* Opening_UnDoCredits: candidate for 1010:2727 (247 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_UnDoCredits
 * name-source: sibling-build
 *
 * When a title card is up (credit_shown, +0x1e7c, == 1) and the scroll counter has reached
 * the card's end tick (credit_times[credit_index].end), the card strip (x 0x500..0x780) is
 * copied down over the visible band, the band is copied to the window and
 * the screen, the card flag drops and the card index advances.
 *
 * RENAMED 2026-09-26 from opening_card_flip: the Mac build's Opening::UnDoCredits(void): this only, taking the title card down when the scroll reaches its end tick -- the undo of Opening_DoCredits. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst);

void Opening_UnDoCredits(Opening *w)
{
    RECT r1;
    RECT r2;

    if ((int)w->credit_shown == 1 && w->credit_times[w->credit_index].end == w->scroll) {
        SetRect(&r1, 0x500, 0x17c, 0x780, 0x1e0);
        SetRect(&r2, 0, 0x17c, 0x280, 0x1e0);
        GRPort_CopyToPort(&w->port1, &w->port1, &r1, &r2);
        GRPort_CopyToPort(&w->port1, (GRPort *)w, &r2, &r2);
        CopyToScreen(&w->port1, &(DialogWin &)*w, &r2, &r2);
        w->credit_shown = 0;
        w->credit_index++;
    }
}
