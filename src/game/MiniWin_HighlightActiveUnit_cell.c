/* MiniWin_HighlightActiveUnit_cell: candidate for 1060:106d (1379 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * REPAINTS ONE CELL OF THE MAP OVERVIEW -- the active unit's -- for `1060:0fdb`, which is what makes
 * that unit blink. It is `MiniWin_BlastMiniRegion` with the drawing cut down to a single cell:
 * the same scale set-up, the same plane walk and the same colour rules, but the loop stops at the
 * square holding the unit at SEG20:0x7794, fills that one rectangle, copies it to the screen and
 * returns. Nothing at all happens while SEG20:0x77c8 is set.
 *
 * `blink` picks the flash colour: 0xf9 when SEG20:0x7796 is 2 and 0xff otherwise. With it clear the
 * cell goes back to the owner's colour, which is the LAST nation index the colour rules worked out --
 * for a cell that took its colour from the terrain, that index is left over from an earlier cell of
 * the sweep, or never written at all on the first one.
 *
 * The background is not cleared here (the overview did that), so an unexplored cell gets the fill
 * colour at DGROUP:0x337e rather than black, and the same colour is where the terrain rules start.
 *
 * The viewport measurements at the end are still computed and then dropped: the four `GRPort_DrawLine`
 * calls that used them in the overview are not here.
 *
 * Transferred from `matched/MiniWin_BlastMiniRegion.c` and graded RELOCATABLE on the first build.
 *
 * `g_tribe_colour_by_nation` (SEG20:0xd9ca) and `g_tribe_colour` (0xd9ce) are ONE table under
 * two biases. The difference is four bytes, which is four one-byte entries, so
 * `g_tribe_colour_by_nation[n]` and `g_tribe_colour[n - 4]` are the same byte. The biased
 * view exists so a raw nation index needs no arithmetic, which is what lets this line read
 * `n < 4 ? g_nation_colour[n] : g_tribe_colour_by_nation[n]` with both arrays subscripted
 * the same way. `ReportWin_ReportsIndian` writes the other form.
 *
 * RENAMED 2026-09-27 from draw_active_unit_cell: 18 bytes = HighlightActiveUnit(short x6, char)+this; only caller is the (char) overload; callees match the Mac body
 */

#include "colonization.h"

#include "w16types.h"
#include "nations.h"
#include "terrain.h"
#include "units.h"

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
extern void copy_rect_to_screen_1060_631c(MiniWin *w, RECT *r);

void MiniWin_HighlightActiveUnit_cell(MiniWin *w, int x, int y, int cols, int rows, int viewer,
                                        int white, char blink)
{
    unsigned char *p0;
    unsigned char *p1;
    unsigned char *p3;
    unsigned char *p2;
    unsigned char terrain;
    unsigned char feature;
    unsigned char colour;
    unsigned char seen_bits;
    unsigned char owner_byte;
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

    if (g_game.end_of_turn_pending != 0)
        return;
    width = rect_width_1060_63b5(&w->rect);
    height = rect_height_1060_63d4(&w->rect);
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
            owner_byte = *p2++;
            feature = *p1++;
            seen_bits = *p3++;
            if (white) {
                colour = 15;
            } else if (mask && !(mask & seen_bits)) {
                colour = g_draw_color;
            } else {
                colour = g_draw_color;
                if (feature & MAP1_SETTLEMENT) {
                    n = owner_byte >> 4;
                    colour = n < 4 ? g_nation_colour[n] : g_tribe_colour_by_nation[n];
                } else if (feature & MAP1_UNIT) {
                    u = top_unit_in_square(x + col, y + row);
                    if (u >= 0) {
                        if ((UNIT_SEEN_BY(viewer) & g_units[u].flags) || g_game.reveal_all != 0) {
                            n = owner_byte >> 4;
                            colour = n < 4 ? g_nation_colour[n] : g_tribe_colour_by_nation[n];
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
            u = g_game.selected_unit;
            if (g_units[u].x == col && g_units[u].y == row) {
                GRPort_FillColor((GRPort *)w, &r, blink != 0 ? (g_game.turn_nation == NATION_SPAIN ? 0xf9 : 0xff)
                                            : (n < 4 ? g_nation_colour[n] : g_tribe_colour_by_nation[n]));
                copy_rect_to_screen_1060_631c((GameWin *)w, &r);
                return;
            }
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
}
