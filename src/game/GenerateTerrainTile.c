/* GenerateTerrainTile: candidate for 1038:d7cb (213 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws sprite record `kind` at (x, y) on dst, then up to four overlays whose indices are
 * the four bytes of the map's overlay plane at (col, row); negative entries are skipped.
 * The overlay plane is locked into a global pointer for the call and unlocked after.
 *
 * RENAMED 2026-09-26 from draw_unit_at: Draws sprite g_art_base[kind] at (x,y), then up to four overlays from g_overlay_sprites (0x2c sprites) indexed by the four bytes at (col,row) of the handle at DGROUP:0x4b2e. That handle is the w*h*4 terrain-seam table SetupDitherMap fills (values 0..0x2b, 0xff = none), and the only caller, GenerateTerrain, passes the base terrain tile index (water class, kind & 7, or 0x11 for forest). It draws a terrain square's base tile plus its seams to the neighbours; no unit is involved.
 *
 * RENAMED 2026-09-27 from draw_tile_with_seams: NOT PROPOSED BY mac_bind. Mac GenerateTerrainTile(GRPort&, unsigned char, short, short, short, short). Caller: GenerateTerrain (d8f8) on both, x3 in the seq. Callees: MSHLock, MSHUnlock and Sprite::Draw on both. Arity: 6 = (dst, kind, x, y, col, row). Layout: Mac CODE 20 map_startup, GenerateTerrainTile(GRPort&...), GenerateTerrainSprite(GRPort&...), GenerateTerrain(GRPort&...), DrawColonies(GRPort&), DrawVillages(GRPort&); Windows 1038 neighbor_bits, d7cb, d8a0, d8f8, de64, df27. OVERLOADS: each of these has a no-port Mac twin in CODE 22 (the main map, Windows 1040). Following PieceDraw (GRPort& = the base name) / PieceDraw_noport, the GRPort& versions take the bare name. The 1040 twins keep their names (see rejected).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40 */

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void GenerateTerrainTile(GRPort *dst, unsigned char kind, int x, int y, int col, int row)
{
    signed char *p;
    char buf1[8];
    char buf2[8];
    int i;

    g_overlay_plane = (signed char *)MSHLock(g_seam_handle);
    Sprite_Draw((RECT *)buf1, &g_art_base[kind], dst, x, y);
    p = g_overlay_plane + col * 4;
    p += g_map_size.w * 4 * row;
    for (i = 0; i < 4; i++, p++) {
        if ((int)*p >= 0)
            Sprite_Draw((RECT *)buf2, &g_overlay_sprites[*p], dst, x, y);
    }
    g_overlay_plane = (signed char *)MSHUnlock(g_seam_handle);
}
