/* DrawMapRegion: candidate for 1040:1117 (160 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: DrawMapRegion
 *
 * Redraws a tile area of the map: the area clipped in place (ConformToViewSize),
 * the terrain for the viewing nation (SEG20:0x7798, -1 with reveal-all),
 * settlements, colonies, the backdrop copy (UpdateTerrainRegion), units, and with
 * `flag` the blit to the screen.  Six helpers are -called and defined
 * in-file as stubs.
 *
 * RENAMED 2026-09-26 from redraw_map_area: the Mac build's DrawMapRegion(short, short, short, short, short), bound by call-sequence alignment (14.0 aligned gaps in GameBrains_SetViewMode, combat_fight, execute_move_unit leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.09x this one.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;
extern void GenerateTerrainMapRegion(int x, int y, int w, int h, int nation);

extern void ConformToViewSize(int *x, int *y, int *w, int *h);

extern void UpdateTerrainRegion(int x, int y, int w, int h);
extern void GenerateUnitsRegion(int x, int y, int w, int h);
extern void ShowMapRegion(int x, int y, int w, int h);

void DrawMapRegion(int x, int y, int w, int h, int flag)
{
    ConformToViewSize(&x, &y, &w, &h);
    GenerateTerrainMapRegion(x, y, w, h, g_game.reveal_all != 0 ? -1 : g_game.view_nation);
    draw_settlements_view();
    draw_colonies_view();
    UpdateTerrainRegion(x, y, w, h);
    GenerateUnitsRegion(x, y, w, h);
    if (flag != 0)
        ShowMapRegion(x, y, w, h);
}
