/* MapWin_ZoomOut: candidate for 1038:9e47 (59 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_ZoomOut
 *
 * Zooms the map window out one step (zoom level at +0x782, kept within -4..4) and
 * rebuilds through GameWin_Redraw; returns the new level.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void GameWin_Redraw(GameWin *w);

int MapWin_ZoomOut(MapWin *w)
{
    if (w->zoom > -4) {
        w->zoom--;
        GameWin_Redraw((GameWin *)w);
    }
    return w->zoom;
}
