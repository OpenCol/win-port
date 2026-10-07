/* set_map_view_mode: candidate for 1008:ccf6 (93 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Clamps a setting to 0..3, stores it at DGROUP:0x1c9a, then rescrolls the map to the
 * cursor and redraws if no scroll was needed.
 *
 * What the four values mean is not established here. What is: the word at 0x1c9a is part of
 * the SAVED GAME -- SEG2 reads and writes it through `save_read_field` with a length of 2,
 * in the same run as the cursor position at 0x4b30/0x4b32 -- and changing it forces the map
 * to be rebuilt, so it is a persisted view setting rather than a transient one.
 *
 * `if (possibly_center(...) == 0) DrawMap(1);` -- the redraw happens when the scroll
 * did NOT move, because a scroll that moves redraws on its own.
 *
 * The cursor pair is reached DS-relative here and in `orders_disband` and
 * `recenter_map_on_cursor`; the view word at 0x1c9a, some twelve thousand bytes below them in the same data
 * segment is in this module. Both seams in one function again.
 */

#include "colonization.h"

extern int possibly_center(int x1, int y1, int x2, int y2, int flag);
extern void DrawMap(int full);

/* same object: 4-byte `push cs; call ` */

void set_map_view_mode(int mode)
{
    register int zoom;

    zoom = mode;
    zoom = max_int_1008_ce9c(0, zoom);
    zoom = min_int_1008_ce76(3, zoom);
    g_zoom = zoom;
    if (possibly_center(g_cursor_x, g_cursor_y, g_cursor_x, g_cursor_y, 0) == 0)
        DrawMap(1);
}
