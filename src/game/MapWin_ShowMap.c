/* PORT: this file's own declarations of MapWin_ConformToViewWindow, copy_rect_to_screen_1038_e38f removed: the header declares them from the definition (W1 step 3). */
/* MapWin_ShowMap: candidate for 1038:a6ea (132 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_ShowMap
 * name-source: sibling-build
 *
 * Copies the w x h rectangle at (x, y) of the map window to the screen:
 * clamped to the window, shifted by the window origin (+0x76a/+0x76c), and
 * skipped when empty.  The three module helpers are -called and defined
 * in-file as stubs.
 *
 * RENAMED 2026-09-26 from map_blit_rect: the Mac build's MapWin::ShowMap(short, short, short, short), bound by call-sequence alignment (1.0 aligned gaps in ShowMapRegion leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.58x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern int g_stub_a;



void MapWin_ShowMap(MapWin *w, int x, int y, int wd, int ht)
{
    RECT r;

    SetRect(&r, x, y, x + wd, y + ht);
    MapWin_ConformToViewWindow(w, &r);
    OffsetRect(&r, w->rect.left, w->rect.top);
    if (rect_is_empty(&r) == 0)
        copy_rect_to_screen_1038_e38f(w, &r);
}
