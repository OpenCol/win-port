/* DrawMap: candidate for 1040:11b7 (76 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: DrawMap
 *
 * Redraws the whole map: the terrain for the viewing nation (SEG20:0x7798,
 * or -1 with reveal-all), then settlements, colonies, UpdateTerrain, units,
 * and with `flag` ShowMap.  The five module helpers are -called
 * and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from redraw_map_1040_11b7: its 1060 namesake is now MiniWin_Draw, so the suffix no longer tells two copies apart.
 *
 * RENAMED 2026-09-26 from redraw_map: the Mac build's DrawMap(short), bound by call-sequence alignment (12.0 aligned gaps in CheatMenuBlow, CheatMenuHuman, MainWin_MainMenuProc leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.00x this one.
 */

#include "colonization.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;
extern void GenerateTerrainMap(int nation);

extern void UpdateTerrain(void);
extern void GenerateUnits(void);
extern void ShowMap(void);

void DrawMap(int flag)
{
    GenerateTerrainMap(g_game.reveal_all != 0 ? -1 : g_game.view_nation);
    draw_settlements_view();
    draw_colonies_view();
    UpdateTerrain();
    GenerateUnits();
    if (flag != 0)
        ShowMap();
}
