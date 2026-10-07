/* draw_terrain_sprite: candidate for 1040:13be (74 bytes, 16 callers).  Verdict: RELOCATABLE.
 *
 * Draws record `kind` of the second sprite table at the current tile position on the
 * screen, like draw_tile_with_overlays without the overlays.
 *
 * `g_terrain_sprites` WAS `g_sprites_4506`, named 2026-09-25 from the index arithmetic at its
 * two callers and nothing else. `map_draw_square` and `GenerateTerrain` both step it the same
 * way -- river masks at +1 and +0x11, hill masks at +0x21 and +0x31, forest at +0x41, a tile
 * decoration or resource at +0x5a, corner blends at +0x6d, and 0x96 for a plowed square -- so
 * the 0xbe entries are the map's terrain sheet. The two drawers are still called
 * `draw_terrain_sprite` and `GenerateTerrainSprite`, which says icon where it means terrain tile; renaming a
 * function needs a curated record and a re-emit of all 34 segments, so it is left for a batch.
 *
 * RENAMED 2026-09-26 from draw_icon_tile: Draws entry `kind` of g_terrain_sprites (the map terrain sheet: river masks, hills, forest, resources, plowed square) at the current tile position plus the tile offset. Its only caller is map_draw_square, stepping through terrain layers. In this corpus 'icon' means unit icons (PieceDraw_noport), so the name points at the wrong sprite sheet. The file's own header calls this out as a pending rename.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void draw_terrain_sprite(unsigned char kind)
{
    char buf[8];

    Sprite_Draw((RECT *)buf, (Sprite *)&g_terrain_sprites[kind], &g_map_port, g_icon_dx + g_draw_px, g_icon_dy + g_draw_py);
}
