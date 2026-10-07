/* MapWin_Recenter: candidate for 1038:a40b (54 bytes).  Verdict: RELOCATABLE.
 *
 * Stores the new map centre in DGROUP:0x1c92/0x1c94 and relays the window
 * (GameWin_Redraw).
 */

#include "colonization.h"

#include "classes.h"

extern void GameWin_Redraw(GameWin *w);

void MapWin_Recenter(MapWin *w, int x, int y)
{
    g_map_center_x = x;
    g_map_center_y = y;
    GameWin_Redraw((GameWin *)w);
}
