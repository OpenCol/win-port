/* Opening_DoCredits: candidate for 1010:2624 (259 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_DoCredits
 * name-source: sibling-build
 *
 * When no card is up and the scroll counter reaches credit_times[credit_index].start, draws
 * banner credit_index (the 32-byte records at +0x1bac) into the port at (320, 445)
 * and copies the lower band (y 0x17c..0x1e0) to the screen and the window.
 *
 * RENAMED 2026-09-26 from opening_card_show: the Mac build's Opening::DoCredits(void): this only, putting up the next title card when the scroll reaches its start tick; Opening_UnDoCredits takes it down. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);
extern void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst);

void Opening_DoCredits(Opening *w)
{
    RECT r1;
    RECT r2;

    if (w->credit_times[w->credit_index].start != -1 && w->credit_shown == 0 && w->credit_times[w->credit_index].start == w->scroll) {
        w->credit_shown = 1;
        Sprite_Draw(&r2, &w->credit_banners[w->credit_index], &w->port1, 0x140, 0x1bd);
        SetRect(&r1, 0, 0x17c, 0x280, 0x1e0);
        CopyToScreen(&w->port1, &(DialogWin &)*w, &r1, &r1);
        GRPort_CopyToPort(&w->port1, (GRPort *)w, &r1, &r1);
    }
}
