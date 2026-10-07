/* PORT: this file's own declarations of MapWin_GetZoom removed: the header declares it from the definition (W1 step 3). */
/* GenerateTerrainMapRegion: candidate for 1040:19da (924 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * REDRAWS A RECTANGLE OF MAP SQUARES into the map's off-screen pixels. Its arguments are the
 * top-left square (x, y), the size in squares (w, h), and a viewing `nation` (-1 reveals every square). It sets up
 * the per-square state and calls `map_draw_square` once per square, and that routine does the drawing.
 *
 * The sprites are scaled by the map window's zoom, `(zoom + 8) / 8`, and reset to 1:1 at the end.
 * A nation >= 0 puts bit `nation` (its visibility bit) into the high nibble of the mask at DGROUP:0x4c92. The rectangle is
 * dropped whole when it starts past the view's edge (0x4c48, 0x4c4a), and otherwise clipped to
 * the view.
 *
 * THE CURSORS. Three map-plane pointers (planes 0, 1 and 3) and a pixel pointer 4 bytes a square
 * start at the first square that is not on the map's outer border, and are stored in DGROUP for
 * the per-square routine. They are `` pointers, and every step goes through the
 * `__AHSHIFT` segment carry. A square on the outer border (x or y of 0, or the last column or
 * row) is drawn with `edge` set and does not advance them, so the border squares all read the
 * neighbouring inner square. DGROUP:0x4c98 flips every column and restarts at 0 each row, a
 * checkerboard parity.
 *
 * The pixel buffer is `MSHLock`ed on the way in, and `MSHUnlock` on the way out. The early exits
 * unlock it too, without having locked it, and the unlock's result lands in the pixel cursor.
 *
 * The pixel pointer is a BYTE pointer stepped by 4 explicitly (`+= 4`, `+= width << 2`). As a
 * 4-byte-element pointer the index is scaled a second time, and that was the one DIFFERS.
 *
 * RENAMED 2026-09-27 from map_draw_region: Mac GenerateTerrainMapRegion(short, short, short, short, short), 754 bytes, against 924. Callers: DrawMapRegion and GenerateTerrainMap (12d5). Callees: GetZoom, SetGlobalSpriteScale x2, ComputeViewParameters, map_loc, feature_loc, site_loc, MSHLock, GenerateTerrain(short) (= map_draw_square) and MSHUnlock, the same set on both. Arity: 5 = (x, y, w, h, nation). No overload. Layout: Mac CODE 22 DrawMapRegion, DrawMap, GenerateTerrainMap, GenerateTerrainTile(uchar), GenerateTerrainSprite(uchar), GenerateCloakedTile, GenerateTerrain(short), GenerateTerrainMapRegion, ColonistIcon, UnitIcon; Windows DrawMapRegion, DrawMap, [redraw_active_unit_tile, redraw_map_in_mode], 12d5, draw_tile_with_overlays, draw_terrain_sprite, terrain_tile_index, draw_neighbor_sprite, map_draw_square, 19da, ColonistIcon, UnitIcon.
 */

#include "colonization.h"

#include "classes.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern unsigned char *g_draw_feature_loc;       /* DGROUP:0x4c7e, plane 1 at the square being drawn */
extern unsigned char *g_draw_map_loc;       /* DGROUP:0x4c82, plane 0 */
extern unsigned char *g_draw_site_loc;       /* DGROUP:0x4c86, plane 3 */

extern void SetGlobalSpriteScale(int num, int den);
extern unsigned char * map_loc(int x, int y);
extern unsigned char * feature_loc(int x, int y);
extern unsigned char * site_loc(int x, int y);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void ComputeViewParameters(void);

void GenerateTerrainMapRegion(int x, int y, int w, int h, int nation)
{
    int row;
    int x2;
    int y2;
    int px;
    int py;
    int width;
    int inner_row;
    int edge;
    unsigned char *p0;
    unsigned char *p1;
    unsigned char *p3;
    unsigned char *pix;
    int num;
    int den;
    register int col;
    register int inner_col;

    num = MapWin_GetZoom(&g_map_window) + 8;
    den = 8;
    SetGlobalSpriteScale(num, den);
    if (nation >= 0)
        g_draw_mask = (unsigned char)(1 << nation) << 4;
    else
        g_draw_mask = 0;
    ComputeViewParameters();
    if (g_view_x1 < x)
        goto out;
    if (g_view_y1 < y)
        goto out;
    x2 = x + w - 1;
    y2 = y + h - 1;
    x = max_int_1040_445d(x, g_view_x0);
    y = max_int_1040_445d(y, g_view_y0);
    x2 = min_int_1040_4483(x2, g_view_x1);
    y2 = min_int_1040_4483(y2, g_view_y1);
    w = x2 - x + 1;
    h = y2 - y + 1;
    col = max_int_1040_445d(1, x);
    row = max_int_1040_445d(1, y);
    px = (col - g_view_x0) * g_tile_w;
    py = (row - g_view_y0) * g_tile_h;
    p0 = map_loc(col, row);
    p1 = feature_loc(col, row);
    p3 = site_loc(col, row);
    width = g_map_size.w;
    pix = (unsigned char *)MSHLock(g_seam_handle);
    pix += (col << 2) + ((g_map_size.w * row) << 2);
    g_draw_py = py;
    for (row = y; row <= y2; row++) {
        g_draw_y = row;
        g_draw_map_loc = p0;
        g_draw_feature_loc = p1;
        g_draw_site_loc = p3;
        g_draw_pix = pix;
        inner_row = row > 0 && g_map_size.h - 1 > row;
        g_draw_parity = 0;
        g_draw_px = px;
        for (col = x; col <= x2; col++) {
            g_draw_x = col;
            inner_col = col > 0 && g_map_size.w - 1 > col;
            edge = inner_col == 0 || inner_row == 0;
            map_draw_square(edge);
            g_draw_px += g_tile_w;
            if (inner_col != 0 && inner_row != 0) {
                g_draw_map_loc++;
                g_draw_feature_loc++;
                g_draw_site_loc++;
                g_draw_pix += 4;
            }
            g_draw_parity ^= 1;
        }
        g_draw_py += g_tile_h;
        if (inner_row != 0) {
            p0 += width;
            p1 += width;
            p3 += width;
            pix += width << 2;
        }
    }
out:
    g_draw_pix = (unsigned char *)MSHUnlock(g_seam_handle);
    SetGlobalSpriteScale(1, 1);
}
