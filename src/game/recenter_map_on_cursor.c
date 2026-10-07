/* recenter_map_on_cursor: candidate for 1038:91c3 (48 bytes).  Verdict: RELOCATABLE.
 *
 * Recentres the main map window (SEG20:0x658, the MainWin's map child) on the cursor square
 * (DGROUP:0x4b30/0x4b32).
 */

#include "colonization.h"

#include "classes.h"

void recenter_map_on_cursor(void)
{
    MapWin_Recenter(&g_map_window, g_cursor_x, g_cursor_y);
}
