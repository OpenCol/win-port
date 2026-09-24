/* MapWin_ZoomOut -- shipped at 1038:9e47.
 *
 * Ported by tools/port.py from matched/game/MapWin_ZoomOut.c.
 *
 * The file's own MapWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void window_relayout(MapWin *w);

col_i16 MapWin_ZoomOut(MapWin *w)
{
    if (w->zoom > -4) {
        w->zoom--;
        window_relayout(w);
    }
    return w->zoom;
}
