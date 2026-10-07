/* PORT: this file's own declarations of MapWin_ShowMap removed: the header declares it from the definition (W1 step 3). */
/* ShowMapRegion: candidate for 1040:07f1 (134 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Copies the w x h tile block at map (x, y) to the screen: pixel origin from
 * the view (DGROUP:0x4c44/0x4c46) and tile size (0x4c4c/0x4c4e), through
 * MapWin_ShowMap on the map window when the block has extent.
 *
 * RENAMED 2026-09-26 from map_blit_tiles: the Mac build's ShowMapRegion(short, short, short, short), bound by call-sequence alignment (1.5 aligned gaps in GameBrains_DoHuman, combat_fight leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.93x this one.
 */

#include "colonization.h"

#include "classes.h"


void ShowMapRegion(int x, int y, int w, int h)
{
    int px;
    int py;
    register int pw;
    register int ph;

    px = (x - g_view_x0) * g_tile_w;
    py = (y - g_view_y0) * g_tile_h;
    pw = g_tile_w * w;
    ph = g_tile_h * h;
    if (pw > 0 && ph > 0)
        MapWin_ShowMap(&g_map_window, px, py, pw, ph);
}
