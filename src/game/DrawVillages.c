/* DrawVillages: candidate for 1038:df27 (150 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws every native village (18-byte records at SEG20:0xcb9a) inside the
 * view window DGROUP:0x4c44..0x4c4a at tile-size steps (0x4c4c/0x4c4e),
 * through TeepeeDraw with mode 0x10.
 *
 * RENAMED 2026-09-27 from draw_villages: Mac DrawVillages(GRPort&). Caller: ColonyWin::GenerateColonyBaseMap on both. Callee: TeepeeDraw only. Arity: 1. Layout: Mac CODE 20 map_startup, GenerateTerrainTile(GRPort&...), GenerateTerrainSprite(GRPort&...), GenerateTerrain(GRPort&...), DrawColonies(GRPort&), DrawVillages(GRPort&); Windows 1038 neighbor_bits, d7cb, d8a0, d8f8, de64, df27. OVERLOADS: each of these has a no-port Mac twin in CODE 22 (the main map, Windows 1040). Following PieceDraw (GRPort& = the base name) / PieceDraw_noport, the GRPort& versions take the bare name. The 1040 twins keep their names (see rejected).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

extern void TeepeeDraw(void *w, int v, int px, int py, int mode);

void DrawVillages(GRPort *port)
{
    register int i;
    register int x;
    int y;

    for (i = 0; i < g_game.settlement_count; i++) {
        x = g_settlements[i].x;
        y = g_settlements[i].y;
        if (x >= g_view_x0 && x <= g_view_x1 && y >= g_view_y0 && y <= g_view_y1)
            TeepeeDraw(port, i, (x - g_view_x0) * g_tile_w, (y - g_view_y0) * g_tile_h, 0x10);
    }
}
