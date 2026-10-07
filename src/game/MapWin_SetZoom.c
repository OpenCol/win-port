/* MapWin_SetZoom: candidate for 1038:9dbf (51 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_SetZoom
 *
 * Sets the map window's zoom level (+0x782) when it is within -4..4 and
 * has the window relaid (GameWin_Redraw).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
extern void GameWin_Redraw(GameWin *w);

void MapWin_SetZoom(MapWin *w, int zoom)
{
    if (zoom >= -4 && zoom <= 4) {
        w->zoom = zoom;
        GameWin_Redraw((GameWin *)w);
    }
}
