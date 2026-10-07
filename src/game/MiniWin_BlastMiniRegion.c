/* MiniWin_BlastMiniRegion: candidate for 1060:0a30 (1451 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * name-source: sibling-build
 *
 * DRAWS THE WHOLE-MAP OVERVIEW into a window, one filled cell per square, for `MiniWin_Draw`
 * and `MiniWin_UpdateTerrain`. It draws the squares from (x, y), `cols` wide by `rows` deep. Both
 * callers pass the whole map, and a viewer of -1 when everything is revealed.
 *
 * SCALE: the window's view rectangle at +0x76a is cleared to DGROUP:0x337e. The cell size
 * (DGROUP:0x4e0c/0x4e0e) is the rectangle divided by the map size, at least 1 pixel. The part of the
 * rectangle that whole cells leave over is split evenly to give the origin (0x4e10/0x4e12). The cells
 * start at that origin plus the window's rectangle at +0x77e.
 *
 * A CELL'S COLOUR, first rule that applies:
 *  - `white` set: 15 everywhere;
 *  - a viewer, and the square's plane-3 bit for that viewer (`0x10 << viewer`, MAP3_NATION_BIT) clear:
 *    0, unexplored;
 *  - a colony or village: the owner's colour, from the high nibble of plane 2. It comes from DGROUP:0x9a
 *    for the four Europeans and SEG20:0xd9ca beyond;
 *  - a unit the viewer can see (the `0x10 << viewer` bit in the unit's flags byte, or reveal-all): its
 *    owner's colour the same way. A foreign privateer that is not aboard anything (+0x1a < 0) shows
 *    grey (8) instead;
 *  - otherwise the terrain colour from SEG20:0xde82, with hills and mountains mapped to their own ids.
 *
 * THE VIEWPORT: last, the rectangle of the main map's view (origin DGROUP:0x4c44/0x4c46, size
 * 0x4c50/0x4c52 squares) is outlined in DGROUP:0x337f.
 *
 * The four plane pointers are `` and step with `*p++`; each row ends with `p += mapw - cols`,
 * which the -3 build sign-extends with CWDE. It compiled RELOCATABLE on the first build.
 *
 * RENAMED 2026-09-27 from draw_map_overview: 16 bytes = (short x6)+this; callees DrawLine, FillColor, TextColor, continent/feature/map/site_loc, top_unit_in_square = Mac body's
 */

#include "colonization.h"

#include "w16types.h"
#include "terrain.h"
#include "units.h"

/* PORT: this file's own MiniWin, GRPort removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#include "records.h"

extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

extern GameState g_game;   /* SEG20:0x7782 */
extern unsigned char * map_loc(int x, int y);
extern unsigned char * feature_loc(int x, int y);
extern unsigned char * continent_loc(int x, int y);
extern unsigned char * site_loc(int x, int y);
extern int top_unit_in_square(int x, int y);

extern void GRPort_FillColor(GRPort *w, RECT *r, int color);

extern void GRPort_DrawLine(GRPort *p, short x1, short y1, short x2, short y2);

/* same object: 4-byte `push cs; call ` */

void MiniWin_BlastMiniRegion(MiniWin *w, int x, int y, int cols, int rows, int viewer, int white)
{
    unsigned char *p0;
    unsigned char *p1;
    unsigned char *p3;
    unsigned char *p2;
    unsigned char terrain;
    unsigned char feature;
    unsigned char colour;
    unsigned char site;
    unsigned char t2;
    int mapw;
    int col;
    int row;
    int mask;
    RECT r;
    int width;
    int height;
    int tw;
    int th;
    int sx;
    int sy;
    register int n;
    register int u;

    width = rect_width_1060_63b5(&w->rect);
    height = rect_height_1060_63d4(&w->rect);
    GRPort_FillColor((GRPort *)w, &w->rect, g_draw_color);
    g_view_tile_w = width / g_map_size.w;
    g_view_tile_h = height / g_map_size.h;
    g_view_tile_w = max_int_1060_63f4(g_view_tile_w, 1);
    g_view_tile_h = max_int_1060_63f4(g_view_tile_h, 1);
    tw = width / g_view_tile_w;
    th = height / g_view_tile_h;
    tw = min_int_1060_641a(tw, g_map_size.w);
    th = min_int_1060_641a(th, g_map_size.h);
    sx = width - tw * g_view_tile_w;
    sy = height - th * g_view_tile_h;
    g_view_org_x = sx >> 1;
    g_view_org_y = sy >> 1;
    if (viewer >= 0)
        mask = 0x10 << viewer;
    else
        mask = 0;
    p0 = map_loc(x, y);
    p1 = feature_loc(x, y);
    p3 = site_loc(x, y);
    p2 = continent_loc(x, y);
    mapw = g_map_size.w;
    SetRect(&r, g_view_org_x, g_view_org_y, g_view_org_x + g_view_tile_w, g_view_org_y + g_view_tile_h);
    OffsetRect(&r, w->clip.left, w->clip.top);
    for (row = 0; row < rows; row++) {
        for (col = 0; col < cols; col++) {
            terrain = *p0++;
            t2 = *p2++;
            feature = *p1++;
            site = *p3++;
            if (white) {
                colour = 15;
            } else if (mask && !(mask & site)) {
                colour = 0;
            } else {
                colour = 0;
                if (feature & MAP1_SETTLEMENT) {
                    n = t2 >> 4;
                    colour = n < 4 ? g_nation_colour[n] : g_tribe_colour[n - 4];
                } else if (feature & MAP1_UNIT) {
                    u = top_unit_in_square(x + col, y + row);
                    if (u >= 0) {
                        if ((UNIT_SEEN_BY(viewer) & g_units[u].flags) || g_game.reveal_all != 0) {
                            n = t2 >> 4;
                            colour = n < 4 ? g_nation_colour[n] : g_tribe_colour[n - 4];
                            if (g_units[u].type == UNIT_PRIVATEER && (char)(g_units[u].flags & UNIT_NATION_MASK) != (char)viewer
                                && g_game.reveal_all == 0 && g_units[u].link2 < 0)
                                colour = 8;
                        }
                    }
                }
                if (colour == 0) {
                    if (terrain & MAP0_HILLY) {
                        if (terrain & MAP0_MOUNTAINS)
                            terrain = TERRAIN_MOUNTAINS;
                        else
                            terrain = TERRAIN_HILLS;
                    } else {
                        terrain = terrain & MAP0_TERRAIN_MASK;
                    }
                    colour = g_terrain_colour[terrain];
                }
            }
            GRPort_FillColor((GRPort *)w, &r, colour);
            OffsetRect(&r, g_view_tile_w, 0);
        }
        p0 += mapw - cols;
        p1 += mapw - cols;
        p2 += mapw - cols;
        p3 += mapw - cols;
        OffsetRect(&r, 0, g_view_tile_h);
        r.left = w->clip.left + g_view_org_x;
        r.right = w->clip.left + g_view_tile_w + g_view_org_x;
    }
    col = g_view_x0 * g_view_tile_w;
    row = g_view_y0 * g_view_tile_h;
    width = g_view_w * g_view_tile_w;
    height = g_view_h * g_view_tile_h;
    sx = w->clip.left + g_view_org_x;
    sy = w->clip.top + g_view_org_y;
    TextColor(g_text_colour);
    GRPort_DrawLine((GRPort *)w, sx + col, sy + row, sx + col + width, sy + row);
    GRPort_DrawLine((GRPort *)w, sx + col + width, sy + row, sx + col + width, sy + row + height);
    GRPort_DrawLine((GRPort *)w, sx + col + width, sy + row + height, sx + col, sy + row + height);
    GRPort_DrawLine((GRPort *)w, sx + col, sy + row + height, sx + col, sy + row);
}
