/* MapWin_SetZoom -- shipped at 1038:9dbf.
 *
 * Ported by tools/port.py from matched/game/MapWin_SetZoom.c.
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void window_relayout(void *w);

void MapWin_SetZoom(MapWin *w, col_i16 z)
{
    if (z >= -4 && z <= 4) {
        w->zoom = z;
        window_relayout(w);
    }
}
