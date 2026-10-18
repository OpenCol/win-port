/* PORT: the view is at most the map's size, map size - 1 tiles each way, the most the clamp
   below keeps inside the map. A window wider or taller than the map (a 2560- or 3440-pixel
   screen, or zoomed out on a smaller one) made the view larger, and the clamp then pulled its
   origin below column 1: the terrain was drawn shifted from the units (GenerateTerrainMapRegion
   starts its pointers at column 1 but its pixels at the origin), discovered land showed as
   ocean, and a click there named a square off the map (OpenCol/win-port#7). The 1995 screens
   never had such a window. What the window shows beyond the map is black (MapWin_RedrawMap). */
/* MapWin_ComputeViewParameters: candidate for 1038:9e82 (477 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_ComputeViewParameters
 * name-source: sibling-build
 *
 * Lays out the map view for the window's current size and zoom. The tile edge comes from
 * `MapWin_TileSize` (16..48 pixels by zoom); the view is the window's frame at +0x76a divided
 * into tiles, ROUNDED UP -- a partial tile at the edge still counts -- and both view globals
 * and both tile globals are set from that one figure.
 *
 * Then the view is centred on the map centre (DGROUP:0x1c92/0x1c94) and clamped: the left and
 * top edges never go below 1, and the view never runs past `map size - 1`, which pulls it back
 * to `map size - view size` when it would. The corner is the origin plus the view size.
 *
 * `(width % tile != 0)` as a value is the `OR DX,DX / JZ / MOV AX,1 / JMP / XOR AX,AX` the
 * shipped has between the pushed quotient and the add. The frame is passed as `&w->frame`
 * through a typed pointer, so the offset is computed before the segment is pushed
 * (rule 56). All three callees come through the 4-byte `PUSH CS` form and are stubs here
 * (rule 47).
 *
 * RENAMED 2026-09-27 from map_view_layout: 4 bytes, calls TileSize; its only caller 1040:0597 is the free ComputeViewParameters(void) (Mac graph: same shape); mac_bind layout
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

/* same object: 4-byte `push cs; call ` */
extern "C" int MapWin_TileSize(MapWin *w);

void MapWin_ComputeViewParameters(MapWin *w)
{
    register int tile;

    tile = MapWin_TileSize(w);
    g_view_w = rect_width_1038_dfbd(&w->rect) / tile
             + (rect_width_1038_dfbd(&w->rect) % tile != 0);
    g_view_h = rect_height_1038_dfdc(&w->rect) / tile
             + (rect_height_1038_dfdc(&w->rect) % tile != 0);
    g_tile_w = tile;
    g_tile_h = tile;
    /* PORT: never more tiles than the map has (OpenCol/win-port#7) */
    if (g_view_w > g_map_size.w - 1)
        g_view_w = g_map_size.w - 1;
    if (g_view_h > g_map_size.h - 1)
        g_view_h = g_map_size.h - 1;
    g_view_x0 = g_map_center_x - (g_view_w >> 1);
    g_view_y0 = g_map_center_y - (g_view_h >> 1);
    if (g_view_x0 < 1)
        g_view_x0 = 1;
    if (g_view_x0 + g_view_w > g_map_size.w - 1)
        g_view_x0 = g_map_size.w - g_view_w;
    if (g_view_y0 < 1)
        g_view_y0 = 1;
    if (g_view_y0 + g_view_h > g_map_size.h - 1)
        g_view_y0 = g_map_size.h - g_view_h;
    g_view_x1 = g_view_x0 + g_view_w;
    g_view_y1 = g_view_y0 + g_view_h;
}
