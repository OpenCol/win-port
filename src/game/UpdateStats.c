/* PORT: this file's own declarations of StatWin_Stats removed: the header declares it from the definition (W1 step 3). */
/* UpdateStats: candidate for 1058:b928 (106 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: UpdateStats
 * name-source: sibling-build
 *
 * Redraws the info window and re-selects the map scale. The scale pair comes out of
 * GetGlobalSpriteScale by reference and goes straight back into
 * SetGlobalSpriteScale; nothing between them touches it.
 *
 * The -0x26 on the current window is the C++ base-class conversion documented in
 * docs/findings/cpp-base-offset.md: -38 is the one delta in the binary that is null-guarded
 * at all 62 of its sites, and the guard is what makes it a base conversion rather than a
 * field offset. The ternary is how that reaches C -- the same spelling
 * ReportWin_OnTradeRouteClick.c uses for the +0x26 direction.
 *
 * RENAMED 2026-09-27 from repaint_info_window: Mac UpdateStats(void), 126 bytes, against 106. Caller: StatWin::LeftMouseClick on both. Callees: GetGlobalSpriteScale, GetEventWindow, StatWin::Stats and SetGlobalSpriteScale, 4 of 4. Layout: after StatWin::Stats on both. Arity: void.
 */

#include "colonization.h"

extern void GetGlobalSpriteScale(int *num, int *den);
extern void SetGlobalSpriteScale(int num, int den);
#include "w16types.h"
#include "classes.h"
extern void *GetEventWindow(void);   /* 1060:9fcd, a call: it lives in SEG13 */


void UpdateStats(void)
{
    StatWin *p;
    int x, y;
    EventWin *w;

    GetGlobalSpriteScale(&x, &y);
    w = (EventWin *)GetEventWindow();
    p = (StatWin *)w;
    StatWin_Stats(p);
    SetGlobalSpriteScale(x, y);
}
