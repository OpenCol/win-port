/* MapWin_Reposition: candidate for 1038:a3c6 (69 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_Reposition
 *
 * Recentres the map on the square under a window position. MapWin_WinPos2MapPos (and its
 * MapWin_TileSize) are defined here because the shipped call is the compiler's own
 * `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern "C" int MapWin_TileSize(MapWin *w);

extern "C" int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);

void MapWin_Reposition(MapWin *o, int x, int y)
{
    int mx, my;

    MapWin_WinPos2MapPos(o, x, y, &mx, &my);
    MapWin_Recenter(o, mx, my);
}
