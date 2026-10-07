/* PORT: this file's own declarations of draw_marker removed: the header declares it from the definition (W1 step 3). */
/* map_draw_marker: candidate for 1040:20d3 (40 bytes).  Verdict: RELOCATABLE.
 *
 * draw_marker on the map window (SEG20:0x658) with four arguments passed
 * through.
 *
 * RENAMED 2026-09-26 from map_call_a849: draw_marker on g_map_window with the sprite, position and flags
 * passed through.
 */

#include "colonization.h"

#include "classes.h"


void map_draw_marker(int spr, int x, int y, int flags)
{
    draw_marker(&g_map_window, spr, x, y, flags);
}
