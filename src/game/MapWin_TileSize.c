/* MapWin_TileSize: candidate for 1038:9d81 (62 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_TileSize
 *
 * (zoom + 8) * 32 / 8 for the map window's zoom level at +0x782 (kept in -4..4 by
 * MapWin_ZoomIn/ZoomOut): tiles of 16..48 pixels.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int MapWin_TileSize(MapWin *w)
{
    int a = w->zoom + 8;
    int d = 8;
    int r = (a << 5) / d;
    return r;
}
