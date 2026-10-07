/* PORT: this file's own declarations of MapWin_ConformToViewWindow removed: the header declares it from the definition (W1 step 3). */
/* MapWin_UpdateTerrain: candidate for 1038:a620 (175 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_UpdateTerrain
 * name-source: sibling-build
 *
 * Copies the w x h rectangle at (x, y) of the map backdrop (SEG28:0) into
 * the map window: clamped to the window, the destination shifted by the
 * origin, skipped when either rectangle is empty.  The two module helpers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from map_copy_rect: the Mac build's MapWin::UpdateTerrain(short, short, short, short), bound by call-sequence alignment (1.0 aligned gaps in UpdateTerrainRegion leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.01x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);


void MapWin_UpdateTerrain(MapWin *w, int x, int y, int wd, int ht)
{
    RECT dst;
    RECT src;

    SetRect(&src, x, y, x + wd, y + ht);
    MapWin_ConformToViewWindow(w, &src);
    CopyRect(&dst, &src);
    OffsetRect(&dst, w->rect.left, w->rect.top);
    if (rect_is_empty(&dst) == 0 && rect_is_empty(&src) == 0)
        GRPort_CopyToPort(&g_map_port, (GRPort *)w, &src, &dst);
}
