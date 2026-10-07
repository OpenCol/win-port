/* MapWin_UpdateTerrain_all: candidate for 1038:a573 (173 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_UpdateTerrain_all
 * name-source: sibling-build
 *
 * Copies the whole map backdrop (the port at SEG28:0) into the map window:
 * the source is the window's full size (rect_width_1038_dfbd/dfdc on the size
 * words at +0x76a), the destination shifted by the origin, skipped when
 * either rectangle is empty.  The three module helpers are -called and
 * defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from map_copy_backdrop_1038_a573: the Mac build's MapWin::UpdateTerrain(void), the whole-window overload: the free UpdateTerrain() -- bound by layout -- is exactly a call of this with &g_map_window, as the Mac free function wraps the method. MapWin_UpdateTerrain is the four-argument region overload, so this one takes the _all suffix, the convention GRPort_FillColor_all set.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern int g_stub_a;

extern void GRPort_CopyToPort(GRPort *src, GRPort *dst, RECT *sr, RECT *dr);

void MapWin_UpdateTerrain_all(MapWin *w)
{
    RECT dst;
    RECT src;

    SetRect(&src, 0, 0, rect_width_1038_dfbd(&w->rect), rect_height_1038_dfdc(&w->rect));
    CopyRect(&dst, &src);
    OffsetRect(&dst, w->rect.left, w->rect.top);
    if (rect_is_empty(&dst) == 0 && rect_is_empty(&src) == 0)
        GRPort_CopyToPort(&g_map_port, (GRPort *)w, &src, &dst);
}
