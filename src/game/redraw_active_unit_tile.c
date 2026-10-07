/* redraw_active_unit_tile: candidate for 1040:1203 (160 bytes).  Verdict: RELOCATABLE.
 * symbol: redraw_active_unit_tile
 *
 * Redraws the one-tile area under the unit at SEG20:0x7794: clipped
 * (ConformToViewSize), backdrop copied (UpdateTerrainRegion), the unit refreshed
 * (GenerateActiveUnit with `flag`), then blitted (ShowMapRegion).  All
 * four are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from redraw_arrived_area: Takes the 1x1 tile under g_game.selected_unit (SEG20:0x7794), then ConformToViewSize, UpdateTerrainRegion, GenerateActiveUnit(flag) and ShowMapRegion. That redraws the active/selected unit's tile. 'arrived' came from the old stub name refresh_arrived_unit, which has since become GenerateActiveUnit. Nothing in the body is about a unit arriving, and it has no direct callers that could supply that context.
 */

#include "colonization.h"

#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
extern int g_stub_a;

extern void ConformToViewSize(int *x, int *y, int *w, int *h);
extern void UpdateTerrainRegion(int x, int y, int w, int h);
extern void GenerateActiveUnit(int flag);
extern void ShowMapRegion(int x, int y, int w, int h);

void redraw_active_unit_tile(int flag)
{
    int x;
    int y;
    int w;
    int h;

    x = g_units[g_game.selected_unit].x;
    y = g_units[g_game.selected_unit].y;
    w = h = 1;
    ConformToViewSize(&x, &y, &w, &h);
    UpdateTerrainRegion(x, y, w, h);
    GenerateActiveUnit(flag);
    ShowMapRegion(x, y, w, h);
}
