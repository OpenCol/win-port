/* GenerateTerrainSprite: candidate for 1038:d8a0 (60 bytes, 16 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws record `kind` of the second 32-byte sprite table at (x, y) on dst.
 *
 * RENAMED 2026-09-27 from draw_icon_at: NOT PROPOSED BY mac_bind. Mac GenerateTerrainSprite(GRPort&, unsigned char, short, short). Caller: GenerateTerrain (d8f8) on both, many times. Callee: Sprite::Draw. Arity: 4 = (dst, kind, x, y). Layout: Mac CODE 20 map_startup, GenerateTerrainTile(GRPort&...), GenerateTerrainSprite(GRPort&...), GenerateTerrain(GRPort&...), DrawColonies(GRPort&), DrawVillages(GRPort&); Windows 1038 neighbor_bits, d7cb, d8a0, d8f8, de64, df27. OVERLOADS: each of these has a no-port Mac twin in CODE 22 (the main map, Windows 1040). Following PieceDraw (GRPort& = the base name) / PieceDraw_noport, the GRPort& versions take the bare name. The 1040 twins keep their names (see rejected).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

void GenerateTerrainSprite(GRPort *dst, unsigned char kind, int x, int y)
{
    char buf[8];

    Sprite_Draw((RECT *)buf, (Sprite *)&g_terrain_sprites[kind], dst, x, y);
}
