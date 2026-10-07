/* PORT: this file's own declarations of FortDraw removed: the header declares it from the definition (W1 step 3). */
/* DrawColonies: candidate for 1038:de64 (195 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `DrawVillages`'s twin, written the same way over the other array: every colony
 * (0xca-byte records at SEG20:0x7c28, count at 0x77a0) whose position falls inside the view
 * window DGROUP:0x4c44..0x4c4a is drawn at tile-size steps through `FortDraw` with size
 * 0x10 and two trailing zeroes.
 *
 * Where they differ is the STORAGE, and it is the whole difference between the two files.
 * The village loop keeps its index and its x in registers; this one spills all three to the
 * frame as `unsigned char` -- `MOV BYTE [BP-3],0`, `INC BYTE [BP-3]`, and a `MOV AH,0`
 * before every use. Three bytes of locals where the twin has none, in two functions written
 * back to back. The zero-extend is what makes them unsigned; a signed char would be `CBW`.
 *
 * The declaration order is the reverse of the slot order: `i` takes the highest slot at
 * BP-3, so it is declared first.
 *
 * The Macintosh calls this pair `DrawColonies(GRPort&)` and `DrawVillages(GRPort&)` -- the
 * same two names, the same arity, next to each other in `macui`. They are not adopted here,
 * because the pair is more useful spelled alike than half-renamed, and `DrawColonies` has a
 * `(void)` overload beside it that this is not.
 *
 * RENAMED 2026-09-27 from draw_colonies: Mac DrawColonies(GRPort&), 214 bytes, against 195. Caller: ColonyWin::GenerateColonyBaseMap on both. Callee: FortDraw only. Arity: 1 (port). Layout: Mac CODE 20 map_startup, GenerateTerrainTile(GRPort&...), GenerateTerrainSprite(GRPort&...), GenerateTerrain(GRPort&...), DrawColonies(GRPort&), DrawVillages(GRPort&); Windows 1038 neighbor_bits, d7cb, d8a0, d8f8, de64, df27. OVERLOADS: each of these has a no-port Mac twin in CODE 22 (the main map, Windows 1040). Following PieceDraw (GRPort& = the base name) / PieceDraw_noport, the GRPort& versions take the bare name. The 1040 twins keep their names (see rejected).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */


void DrawColonies(GRPort *port)
{
    unsigned char i;
    unsigned char x;
    unsigned char y;

    for (i = 0; i < g_game.colony_count; i++) {
        x = g_colonies[i].x;
        y = g_colonies[i].y;
        if (x >= g_view_x0 && x <= g_view_x1 && y >= g_view_y0 && y <= g_view_y1)
            FortDraw(port, i, (x - g_view_x0) * g_tile_w, (y - g_view_y0) * g_tile_h,
                        0x10, 0, 0);
    }
}
