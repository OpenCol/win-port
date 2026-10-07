/* Opening_DoPan: candidate for 1010:0cee (168 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Opening_DoPan
 * name-source: sibling-build
 *
 * One step of the opening's title scroll: while the Opening's state
 * (+0x550) is under 3, the 640x323 band at y 56 of its port (+0xc4) is
 * copied to the window (+0x26), shifted right by 0x500 minus the scroll
 * counter (+0x1e78) until that reaches 0x500; the counter advances by 2.
 *
 * RENAMED 2026-09-26 from opening_scroll_step: the Mac build's Opening::DoPan(void): this only, one step of the title band's horizontal scroll. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
#include "records.h"

extern void CopyToScreen(GRPort *port, Screen *screen, RECT *src, RECT *dst);

void Opening_DoPan(Opening *w)
{
    RECT r1;
    RECT r2;
    register int left;

    if (w->state < 3) {
        if (w->scroll <= 0x500) {
            left = 0x500 - w->scroll;
            SetRect(&r1, left, 0x38, left + 0x280, 0x17b);
        } else {
            SetRect(&r1, 0, 0x38, 0x280, 0x17b);
        }
        SetRect(&r2, 0, 0x38, 0x280, 0x17b);
        CopyToScreen((GRPort *)(&w->port1), (Screen *)(&(DialogWin &)*w), &r1, &r2);
    }
    w->scroll += 2;
}
