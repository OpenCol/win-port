/* PORT: this file's own declarations of DrawColonies, DrawVillages, GenerateTerrain removed: the header declares them from the definition (W1 step 3). */
/* ColonyWin_GenerateColonyBaseMap: candidate for 1010:767d (590 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_GenerateColonyBaseMap
 * name-source: sibling-build
 *
 * Draws the three-by-three square of land around the colony into the colony window's second
 * GRPort (at +0x7ce), which is the terrain the colony screen shows behind its work slots.
 *
 * It does it by BORROWING THE MAP VIEW. The five words of view state at DGROUP:0x4c44..0x4c4c
 * are saved into locals, replaced with a window one tile either side of the colony and a tile
 * size of 0x40, used for the whole draw -- nine `GenerateTerrain` calls, then `DrawColonies` and
 * `DrawVillages`, which read the same view words -- and put back at the end.
 *
 * THE SAVE AND THE RESTORE DO NOT AGREE. Five words are saved but SIX are written: 0x4c4e is
 * set from `cell` and never saved, and the restore puts the saved 0x4c4c into BOTH 0x4c4c and
 * 0x4c4e. The two are the tile width and height and the map always has them equal, so the bug
 * cannot show; it is in the shipped bytes and is left there.
 *
 * `g_tile_w = g_tile_h = cell` is one chained assignment and not two statements: the value
 * stays in a register across the second store (`MOV AX,DX`) where two statements would reload.
 * The same shape appears in the restore.
 *
 * `tx` and `px` are the register locals, SI then DI in declaration order (rule 53); the row
 * and column counters and the five saved words stay in the frame.
 *
 * RENAMED 2026-09-27 from colony_draw_tiles: CONST PROPOSAL. Mac ColonyWin::GenerateColonyBaseMap(void), CODE 30 0x0918, 492 bytes. Its seq is set_colony, GRPort::Init, SetGlobalSpriteScale, GenerateTerrain, DrawColonies, DrawVillages, SetGlobalSpriteScale. 767d calls set_colony, GRPort_Init_wh, SetGlobalSpriteScale x2, GenerateTerrain, DrawColonies and DrawVillages. Caller: Mac DrawColony, Windows ColonyWin_DrawColony. Arity: this against (ColonyWin *w). Layout: Mac colony_hammer_buy, GenerateColonyBaseMap, DrawMakeList, DrawColonyUnits, DrawProduction; Windows ColonyWin_colony_hammer_buy, 767d, 78cb, DrawColonyUnits, 7edb. Size 492 against 590. Constants: overlap 1.0.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"

extern void set_colony(int colony);

extern void SetGlobalSpriteScale(int num, int den);

void ColonyWin_GenerateColonyBaseMap(ColonyWin *w)
{
    int ty;
    int py;
    int row;
    int col;
    int sx0;
    int sy0;
    int sx1;
    int sy1;
    int sw;
    int cell;
    register int tx;
    register int px;

    cell = 0x40;
    set_colony(w->colony);
    GRPort_Init_wh(&w->terrain, 0xc0, 0xc0);
    tx = g_colonies[w->colony].x - 1;
    ty = g_colonies[w->colony].y - 1;
    px = 0;
    py = 0;
    SetGlobalSpriteScale(2, 1);
    sx0 = g_view_x0;
    sy0 = g_view_y0;
    sx1 = g_view_x1;
    sy1 = g_view_y1;
    sw = g_tile_w;
    g_view_x0 = g_colonies[w->colony].x - 1;
    g_view_x1 = g_colonies[w->colony].x + 1;
    g_view_y0 = g_colonies[w->colony].y - 1;
    g_view_y1 = g_colonies[w->colony].y + 1;
    g_tile_w = g_tile_h = cell;
    for (row = 0; row < 3; row++) {
        for (col = 0; col < 3; col++) {
            GenerateTerrain(&w->terrain, tx, ty, px, py, cell);
            tx++;
            px += cell;
        }
        px = 0;
        ty++;
        tx = g_colonies[w->colony].x - 1;
        py += cell;
    }
    DrawColonies(&w->terrain);
    DrawVillages(&w->terrain);
    SetGlobalSpriteScale(1, 1);
    g_view_x0 = sx0;
    g_view_y0 = sy0;
    g_view_y1 = sy1;
    g_view_x1 = sx1;
    g_tile_w = g_tile_h = sw;
}
