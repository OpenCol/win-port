/* map_draw_unit_marks: candidate for 1040:20fb (43 bytes).  Verdict: RELOCATABLE.
 *
 * MapWin_PieceIconDraw on the map window (SEG20:0x658) with its four arguments passed
 * through.
 *
 * RENAMED 2026-09-26 from map_call_a99f: MapWin_PieceIconDraw on g_map_window, the sprite and position
 * passed through; PieceDraw_noport's only map-drawing helper.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void MapWin_PieceIconDraw(void *w, void *sprite, int x, int y, int flags);

void map_draw_unit_marks(void *sprite, int x, int y, int flags)
{
    MapWin_PieceIconDraw(&g_map_window, (Sprite *)sprite, x, y, flags);
}
