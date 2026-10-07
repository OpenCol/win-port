/* MapWin_GetZoom: candidate for 1038:9df2 (26 bytes).  Verdict: EXACT.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_GetZoom
 *
 * The map window's zoom level (+0x782).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

int MapWin_GetZoom(MapWin *mw)
{
    return mw->zoom;
}
