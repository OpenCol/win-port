/* GenerateTerrain: candidate for 1038:d8f8 (1388 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: GenerateTerrain
 *
 * PAINTS ONE MAP SQUARE'S TERRAIN, for `1010:767d`. It leaves the square's plane bytes and pointers in
 * the DGROUP:0x4c6b..0x4c78 block that the neighbour helpers all read, so the order of the calls matters.
 *
 * WATER (terrain class 0x19 or 0x1a) with no land around it is one flat tile plus its decoration, and
 * nothing else. Everything else is drawn in layers:
 *  - the base tile: the terrain's low three bits under 0x18, or the class itself above it, except that
 *    class 1 in the two forest bands draws tile 0x11;
 *  - the terrain's edge blend against its neighbours (0x41 + mask), skipped in the map's other mode;
 *  - plowed (plane 1 bit 0x40) as icon 0x96;
 *  - hills and mountains (plane 0 bit 0x20) as 0x31 or 0x21 plus the neighbour match;
 *  - rivers (bit 0x40) as 0x11 or 1 plus the orthogonal mask, the major river being the 0x80 variant;
 *  - the square's decoration and, where `lost_city_at` is true (a Lost City Rumour), icon 0x68;
 *  - roads (plane 1 bits 0xa): one icon 0x51 when isolated, otherwise one 0x52 + direction per neighbour.
 *
 * WATER WITH LAND AROUND IT is the coastline: four corner pieces from the table at DGROUP:0x4c6e, or one
 * of the four whole-edge tiles 0x97..0x9a when the neighbour mask matches one of four patterns exactly.
 * Then, for a square that is itself a river mouth (the 0xc0 bits), the four orthogonal neighbours are
 * checked and an inlet piece drawn for each that is a river.
 *
 * A Sprite is constructed and destroyed and never used.
 *
 * Spellings: `scan = 0` initialises before the Sprite's constructor (rule 72); the band test is written
 * NEGATED with the plain-tile arm first; `dx = dy = 0` stores dy first; the icon arguments are
 * `unsigned char` (rule 71's word push, with 0x96 as 0x0096); `terrain_fix_2`'s char result is
 * read as an unsigned char (a cast at both calls; the definition returns plain char); the orthogonal offset tables are NEAR here; and their bytes need `(int)` to compare
 * (rule 51).
 *
 * RENAMED 2026-09-27 from draw_terrain_tile: Mac GenerateTerrain(GRPort&, short, short, short, short, short), 1494 bytes, against 1388. Caller: ColonyWin::GenerateColonyBaseMap on both. Callees: map_get, feature_get, terrain_fix_2 x2, map_loc, feature_loc, lost_city_at and resource_at x3 on both. GenerateTerrainTile x3 = draw_tile_with_seams x3; GenerateTerrainSprite = draw_icon_at; the five Mac scan_* are Windows neighbor_bits, neighbor_mask, neighbor_terrain_scan, orthogonal_neighbor_bits and tile_neighbor_match. Arity: 6 = (port, x, y, px, py, tile). Layout: Mac CODE 20 map_startup, GenerateTerrainTile(GRPort&...), GenerateTerrainSprite(GRPort&...), GenerateTerrain(GRPort&...), DrawColonies(GRPort&), DrawVillages(GRPort&); Windows 1038 neighbor_bits, d7cb, d8a0, d8f8, de64, df27. OVERLOADS: each of these has a no-port Mac twin in CODE 22 (the main map, Windows 1040). Following PieceDraw (GRPort& = the base name) / PieceDraw_noport, the GRPort& versions take the bare name. The 1040 twins keep their names (see rejected).
 */

#include "colonization.h"

#include "terrain.h"
#include "w16types.h"
typedef struct { short vptr; int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; } GRPort;
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-

extern unsigned char *g_tile_map_loc;    /* DGROUP:0x4c72 */
extern unsigned char *g_tile_feature_loc;   /* DGROUP:0x4c76 */
#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" unsigned char map_get(int x, int y);
extern "C" unsigned char feature_get(int x, int y);
extern "C" unsigned char *map_loc(int x, int y);
extern "C" unsigned char *feature_loc(int x, int y);
extern "C" char terrain_fix_2(int terrain);
extern "C" int resource_at(int x, int y);
extern "C" int lost_city_at(int x, int y);

extern "C" void GenerateTerrainTile(void *dst, unsigned char kind, int x, int y, int col, int row);
extern "C" void GenerateTerrainSprite(void *dst, unsigned char icon, int px, int py);

void GenerateTerrain(GRPort *port, int x, int y, register int px, register int py,
                                     int tile)
{
    int mask;
    int terrain;
    int deco;
    int i;
    int bit;
    int scan = 0;
    Sprite spr;
    int j;
    int icon;
    int hi;
    int kind;
    int sel;
    char water;
    unsigned char base;
    unsigned char feature;
    int dx;
    int dy;

    g_tile_terrain = map_get(x, y);
    feature = feature_get(x, y);
    g_tile_terrain_class = (unsigned char)terrain_fix_2(g_tile_terrain);
    g_tile_map_loc = map_loc(x, y);
    g_tile_feature_loc = feature_loc(x, y);
    hi = g_tile_terrain & 0xc0;
    water = 0;
    base = 0x19;
    if (g_tile_terrain_class == TERRAIN_OCEAN || g_tile_terrain_class == TERRAIN_SEA_LANE) {
        base = g_tile_terrain_class;
        scan = neighbor_terrain_scan_1038_d520();
        water = 1;
    }
    if (water != 0 && scan == 0) {
        GenerateTerrainTile(port, base, px, py, x, y);
        deco = resource_at(x, y);
        if (deco != -1)
            GenerateTerrainSprite(port, deco + 0x5a, px, py);
    } else {
        if (g_tile_terrain_class < TERRAIN_ARCTIC)
            kind = g_tile_terrain_class & 7;
        else
            kind = g_tile_terrain_class;
        if (kind != 1 || !((g_tile_terrain_class >= 8 && g_tile_terrain_class < 0x10) || (g_tile_terrain_class >= 0x10 && g_tile_terrain_class < TERRAIN_ARCTIC)))
            GenerateTerrainTile(port, kind, px, py, x, y);
        else
            GenerateTerrainTile(port, 0x11, px, py, x, y);
        if (kind != 1 && g_map_mode == 0
            && ((g_tile_terrain_class >= 8 && g_tile_terrain_class < 0x10) || (g_tile_terrain_class >= 0x10 && g_tile_terrain_class < TERRAIN_ARCTIC))) {
            mask = neighbor_mask_1038_d4b9(kind, 3);
            GenerateTerrainSprite(port, mask + 0x41, px, py);
        }
        if (feature & 0x40)
            GenerateTerrainSprite(port, 0x96, px, py);
        if ((g_tile_terrain & 0x20) && water == 0) {
            mask = tile_neighbor_match(g_tile_terrain & 0xa0, 2);
            if (g_tile_terrain & 0x80)
                GenerateTerrainSprite(port, mask + 0x21, px, py);
            else
                GenerateTerrainSprite(port, mask + 0x31, px, py);
        }
        if ((g_tile_terrain & 0x40) && water == 0) {
            mask = orthogonal_neighbor_bits(0x40, 3);
            if (g_tile_terrain & 0x80)
                GenerateTerrainSprite(port, mask + 1, px, py);
            else
                GenerateTerrainSprite(port, mask + 0x11, px, py);
        }
        if (g_map_mode == 0) {
            deco = resource_at(x, y);
            if (deco != -1)
                GenerateTerrainSprite(port, deco + 0x5a, px, py);
            if (lost_city_at(x, y))
                GenerateTerrainSprite(port, 0x68, px, py);
        }
        if ((feature & 0xa) && water == 0 && g_map_mode == 0) {
            mask = neighbor_bits(0xa, 1);
            if (mask == 0) {
                GenerateTerrainSprite(port, 0x51, px, py);
            } else {
                bit = 1;
                for (i = 0; i < 8; i++) {
                    if (mask & bit)
                        GenerateTerrainSprite(port, i + 0x52, px, py);
                    bit = bit * 2;
                }
            }
        }
        if (water != 0) {
            sel = -1;
            if ((g_tile_land_mask & 0xdd) == 0xc1)
                sel = 0;
            if ((g_tile_land_mask & 0x77) == 0x07)
                sel = 1;
            if ((g_tile_land_mask & 0x77) == 0x70)
                sel = 2;
            if ((g_tile_land_mask & 0xdd) == 0x1c)
                sel = 3;
            if (sel < 0) {
                for (j = 0; j < 4; j++) {
                    icon = (j + 1) & 3;
                    dx = (unsigned char)((icon >> 1) * tile / 2);
                    dy = (unsigned char)((j >> 1) * tile / 2);
                    GenerateTerrainSprite(port, g_tile_corner_bits[j] * 4 + j + 0x6d, px + dx, py + dy);
                }
                dx = dy = 0;
            } else {
                GenerateTerrainSprite(port, sel + 0x97, px, py);
            }
            if (hi != 0) {
                icon = (hi & 0x80) ? 0x8d : 0x91;
                for (j = 0; j < 4; j++) {
                    terrain = *(g_tile_map_loc + g_ortho_dx[j]
                          + ((int)g_ortho_dy[j] < 0 ? -g_map_size.w : 0)
                          + ((int)g_ortho_dy[j] > 0 ? g_map_size.w : 0));
                    if (terrain & 0x40) {
                        terrain = (unsigned char)terrain_fix_2(terrain);
                        if (terrain != 0x19 && terrain != 0x1a) {
                            if (icon == 0x8d)
                                GenerateTerrainSprite(port, j + 0x8d, px, py);
                            else
                                GenerateTerrainSprite(port, j + 0x91, px, py);
                        }
                    }
                }
            }
            deco = resource_at(x, y);
            if (deco != -1)
                GenerateTerrainSprite(port, deco + 0x5a, px, py);
        }
    }
}

