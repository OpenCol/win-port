/* map_draw_square: candidate for 1040:14d4 (1286 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: map_draw_square
 * name-source: descriptive
 *
 * DRAWS ONE MAP SQUARE, for `GenerateTerrainMapRegion`, from the cursors it leaves in DGROUP
 * (the plane bytes at 0x4c7e/0x4c82/0x4c86, the square at 0x4c8e, the pixel position at 0x4c94).
 * The three plane bytes and the terrain class are copied to 0x4c9a..0x4c9d for the helpers.
 *
 * FOG. A square the viewer has not seen (its bit in 0x4c92 is clear in plane 3), or a square on the
 * map border (`edge`), is covered with the fog sprite and nothing else is drawn.
 *
 * WATER that `neighbor_terrain_scan_1040_02cc` finds no coast around is its ocean or sea-lane tile
 * plus any special resource. Everything else, in drawing order:
 *   - the base tile: the terrain class's low 3 bits for the land kinds (so a forest draws the open
 *     tile beneath it), class 24 up as itself, and scrub forest from tile 0x11;
 *   - for a forest, the forest overlay chosen by which neighbours are the same forest (0x41 + mask);
 *   - plowing (0x96);
 *   - hills (plane-0 0x20) and rivers (0x40), each joined to its neighbours, major (0x80) or minor;
 *   - unless the map mode at 0x1ca4 hides them: the special resource (0x5a + kind), and a lost-city
 *     rumour (0x68, when `lost_city_at` answers);
 *   - roads (plane-1 0x0a) joined in up to eight directions (0x52 + direction), or a lone road
 *     piece (0x51);
 *   - ON A COAST SQUARE: one of four straight shore pieces (0x97 + side) when the land around
 *     matches one of four patterns, otherwise four corner pieces, one per quarter of the square.
 *     Then river mouths flowing into it from the four orthogonal land neighbours with rivers
 *     (0x8d major, 0x91 minor), and its special resource.
 * Units, colonies and settlements on the square come last (`draw_neighbor_sprite`).
 *
 * A local `Sprite` is constructed and destroyed and never used.
 *
 * The only DIFFERS was the arms of the base-tile `if`: the shipped tests `cls != 1 || !forest` and
 * draws `cls` first.
 */

#include "colonization.h"

#include "w16types.h"
#include "terrain.h"
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-

extern unsigned char *g_draw_feature_loc;       /* DGROUP:0x4c7e, plane 1 at the square */
extern unsigned char *g_draw_map_loc;       /* DGROUP:0x4c82, plane 0 */
extern unsigned char *g_draw_site_loc;       /* DGROUP:0x4c86, plane 3 */

extern unsigned char g_sq_feature;               /* DGROUP:0x4c9a */
extern unsigned char g_sq_site;               /* DGROUP:0x4c9b */
extern unsigned char g_sq_terrain;               /* DGROUP:0x4c9c */

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

typedef struct { short vptr; int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; } GRPort;

#include "icons.h"

extern char terrain_fix_2(int terrain);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern int resource_at(int x, int y);
extern int lost_city_at(int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void draw_terrain_sprite(unsigned char kind);

#define FOREST(t) (((t) >= TERRAIN_BOREAL_FOREST && (t) < TERRAIN_BOREAL_FOREST_2) \
                   || ((t) >= TERRAIN_BOREAL_FOREST_2 && (t) < TERRAIN_ARCTIC))

extern "C" void map_draw_square(int edge)
{
    int mask;
    int hidden;
    int deco;
    int dir;
    int coast = 0;
    int i;
    int base;
    int river;
    int cls;
    int shore;
    char water;
    char wter;
    RECT r;
    register int terrain;
    register int bit;
    Sprite spr;

    g_sq_feature = *g_draw_feature_loc;
    g_sq_terrain = *g_draw_map_loc;
    g_sq_site = *g_draw_site_loc;
    g_sq_class = terrain_fix_2(g_sq_terrain);
    hidden = (g_draw_mask != 0 && (g_sq_site & g_draw_mask) == 0) || edge != 0 ? 1 : 0;
    river = g_sq_terrain & 0xc0;
    if (hidden) {
        Sprite_Draw(&r, &g_terrain_sprites[TSPR_FOG], &g_map_port, g_draw_px, g_draw_py);
        goto end;
    }
    water = 0;
    wter = TERRAIN_OCEAN;
    if (g_sq_class == TERRAIN_OCEAN || g_sq_class == TERRAIN_SEA_LANE) {
        wter = g_sq_class;
        coast = neighbor_terrain_scan_1040_02cc();
        water = 1;
    }
    if (water && coast == 0) {
        draw_tile_with_overlays(wter);
        deco = resource_at(g_draw_x, g_draw_y);
        if (deco != -1)
            draw_terrain_sprite(deco + 0x5a);
        goto neighbors;
    }
    if (g_sq_class < TERRAIN_ARCTIC)
        cls = g_sq_class & 7;
    else
        cls = g_sq_class;
    if (cls != 1 || !FOREST(g_sq_class))
        draw_tile_with_overlays(cls);
    else
        draw_tile_with_overlays(0x11);
    if (cls != 1 && FOREST(g_sq_class)) {
        mask = neighbor_mask_1040_013f(cls, 3);
        draw_terrain_sprite(mask + 0x41);
    }
    if (g_sq_feature & MAP1_PLOWED)
        draw_terrain_sprite(0x96);
    if ((g_sq_terrain & MAP0_HILLY) && !water) {
        mask = tile_neighbor_match_alt(g_sq_terrain & 0xa0, 2);
        if (g_sq_terrain & 0x80)
            draw_terrain_sprite(mask + 0x21);
        else
            draw_terrain_sprite(mask + 0x31);
    }
    if ((g_sq_terrain & MAP0_RIVER) && !water) {
        mask = tile_neighbor_mask(0x40, 3);
        if (g_sq_terrain & 0x80)
            draw_terrain_sprite(mask + 1);
        else
            draw_terrain_sprite(mask + 0x11);
    }
    if (g_map_mode == 0) {
        deco = resource_at(g_draw_x, g_draw_y);
        if (deco != -1)
            draw_terrain_sprite(deco + 0x5a);
        if (lost_city_at(g_draw_x, g_draw_y))
            draw_terrain_sprite(0x68);
    }
    if ((g_sq_feature & (MAP1_SETTLEMENT | MAP1_ROAD)) && !water && g_map_mode == 0) {
        mask = neighbor_bits_alt(10, 1);
        if (mask == 0)
            draw_terrain_sprite(0x51);
        else
            for (bit = 1, dir = 0; dir < 8; dir++) {
                if (mask & bit)
                    draw_terrain_sprite(dir + 0x52);
                bit = bit * 2;
            }
    }
    if (water) {
        shore = -1;
        if ((g_sq_land_mask & 0xdd) == 0xc1)
            shore = 0;
        if ((g_sq_land_mask & 0x77) == 0x07)
            shore = 1;
        if ((g_sq_land_mask & 0x77) == 0x70)
            shore = 2;
        if ((g_sq_land_mask & 0xdd) == 0x1c)
            shore = 3;
        if (shore < 0) {
            for (i = 0; i < 4; i++) {
                base = (i + 1) & 3;
                g_icon_dx = (base >> 1) * g_tile_w / 2;
                g_icon_dy = (i >> 1) * g_tile_h / 2;
                draw_terrain_sprite((g_sq_corner_bits[i] << 2) + i + 0x6d);
            }
            g_icon_dx = g_icon_dy = 0;
        } else
            draw_terrain_sprite(shore + 0x97);
        if (river != 0) {
            base = (river & 0x80) ? 0x8d : 0x91;
            for (i = 0; i < 4; i++) {
                terrain = *(g_draw_map_loc + g_ortho_dx[i] + ((int)g_ortho_dy[i] < 0 ? -g_map_size.w : 0)
                      + ((int)g_ortho_dy[i] > 0 ? g_map_size.w : 0));
                if (terrain & 0x40) {
                    terrain = (unsigned char)terrain_fix_2(terrain);
                    if (terrain != TERRAIN_OCEAN && terrain != TERRAIN_SEA_LANE) {
                        if (base == 0x8d)
                            draw_terrain_sprite(i + 0x8d);
                        else
                            draw_terrain_sprite(i + 0x91);
                    }
                }
            }
        }
        deco = resource_at(g_draw_x, g_draw_y);
        if (deco != -1)
            draw_terrain_sprite(deco + 0x5a);
    }
neighbors:
    draw_neighbor_sprite();
end:
    ;
}
