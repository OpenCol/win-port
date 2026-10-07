/* GenerateTerrainMap: candidate for 1040:12d5 (69 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: GenerateTerrainMap
 *
 * ComputeViewParameters, then GenerateTerrainMapRegion over the current view with `a`.
 * ComputeViewParameters is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from redraw_map_view: Mac GenerateTerrainMap(short), 72 bytes, against 69. Callers: DrawMap and MapWin::RevealHiddenTerrain on both (Windows adds redraw_map_in_mode). Callees: ComputeViewParameters and GenerateTerrainMapRegion (19da), in that order. Arity: 1 (nation). No overload. Layout: Mac CODE 22 DrawMapRegion, DrawMap, GenerateTerrainMap, GenerateTerrainTile(uchar), GenerateTerrainSprite(uchar), GenerateCloakedTile, GenerateTerrain(short), GenerateTerrainMapRegion, ColonistIcon, UnitIcon; Windows DrawMapRegion, DrawMap, [redraw_active_unit_tile, redraw_map_in_mode], 12d5, draw_tile_with_overlays, draw_terrain_sprite, terrain_tile_index, draw_neighbor_sprite, map_draw_square, 19da, ColonistIcon, UnitIcon.
 */

#include "colonization.h"

extern int g_stub_a;
extern void GenerateTerrainMapRegion(int x, int y, int w, int h, int nation);

extern void ComputeViewParameters(void);

void GenerateTerrainMap(int nation)
{
    ComputeViewParameters();
    GenerateTerrainMapRegion(g_view_x0, g_view_y0, g_view_w, g_view_h, nation);
}
