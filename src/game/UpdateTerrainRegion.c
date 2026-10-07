/* PORT: this file's own declarations of MapWin_UpdateTerrain, MiniWin_UpdateTerrain removed: the header declares them from the definition (W1 step 3). */
/* UpdateTerrainRegion: candidate for 1040:070f (170 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Copies the w x h tile block at map (x, y) from the backdrop into the map
 * window (MapWin_UpdateTerrain) and lets MiniWin_UpdateTerrain redraw the record at
 * SEG20:0xde0 over it, when the block has extent.
 *
 * RENAMED 2026-09-26 from map_copy_tiles: the Mac build's UpdateTerrainRegion(short, short, short, short), bound by call-sequence alignment (1.0 aligned gaps in GameBrains_DoHuman leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.93x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern char g_mini_win;           /* SEG20:0xde0 */

void UpdateTerrainRegion(int x, int y, int w, int h)
{
    int px;
    int py;
    int pw;
    int ph;

    px = (x - g_view_x0) * g_tile_w;
    py = (y - g_view_y0) * g_tile_h;
    pw = g_tile_w * w;
    ph = g_tile_h * h;
    if (pw > 0 && ph > 0) {
        MapWin_UpdateTerrain(&g_map_window, px, py, pw, ph);
        MiniWin_UpdateTerrain((MiniWin *)&g_mini_win, x, y, w, h);
    }
}
