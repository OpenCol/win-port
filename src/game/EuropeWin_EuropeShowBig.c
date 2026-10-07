/* PORT: this file's own declarations of copy_rect_to_screen removed: the header declares it from the definition (W1 step 3). */
/* EuropeWin_EuropeShowBig: candidate for 1048:1c15 (100 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_EuropeShowBig
 * name-source: sibling-build
 *
 * Makes the window's nation current and blits a fixed band -- (0, 8) to (640, 258), 250
 * pixels tall -- from the window to the screen, offset to wherever the window sits.
 *
 * The rectangle is built with literals and never read from the window, so the band is the
 * same whatever the window is; only the origin at +0x76a/+0x76c moves it.
 *
 * `OffsetRect` IS ``, so its arguments go left to right and the FIRST of the pair is
 * the x displacement. That settles +0x76a as the window's x origin and +0x76c as its y --
 * the other way round from how `EuropeWin_EuropeMouseParse`'s header reads them, and that header
 * is the one to correct, since parameter names do not move bytes.
 *
 * RENAMED 2026-09-27 from copy_window_band_to_screen: CONST PROPOSAL. Mac EuropeWin::EuropeShowBig(void), CODE 33 0x02b8, 180 bytes, called by AttemptCargoFromEurope and AttemptCargoToEurope. 1c15 (EuropeWin *w) is called by europe_buy_cargo and europe_sell_cargo, the same two (leads3). Body: set_europe, then copy the big band to the screen (Mac CopyToScreen + ValidRect twice; Windows SetRect/OffsetRect, copy_rect_to_screen). Layout: directly after EuropeClearBig on both. Size 180 against 100. The Mac repaints two rectangles, Windows one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

/* 5-byte NOP form, so extern (rule 47) */
extern void set_europe(int n);
/* same object: 4-byte `push cs; call ` */

void EuropeWin_EuropeShowBig(EuropeWin *w)
{
    RECT r;

    set_europe(w->nation);
    SetRect(&r, 0, 8, 0x280, 0x102);
    OffsetRect(&r, w->rect.left, w->rect.top);
    copy_rect_to_screen(w, &r);
}
