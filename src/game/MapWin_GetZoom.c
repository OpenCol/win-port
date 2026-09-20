/* MapWin_GetZoom -- shipped at 1038:9df2.
 *
 * Ported by tools/port.py from matched/game/MapWin_GetZoom.c.
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 MapWin_GetZoom(MapWin *o)
{
    return o->zoom;
}
