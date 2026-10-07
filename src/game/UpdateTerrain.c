/* UpdateTerrain: candidate for 1040:07b9 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Segment 1040's one-line forwarder to `MapWin_UpdateTerrain_all` with the map window at
 * SEG20:0x658, named for the routine it reaches. Its twin immediately after it (ShowMap, 1040:07d5) reaches
 * `MapWin_ShowMap_all` with the same argument, so the two are the module's redraw pair.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * RENAMED 2026-09-26 from map_copy_backdrop_1040_07b9: the Mac build's UpdateTerrain(void), bound by layout (between UpdateTerrainRegion and ShowMapRegion on both sides); the arity agrees and the Mac body is 1.43x this one.
 */

#include "colonization.h"

#include "classes.h"

extern void MapWin_UpdateTerrain_all(void *mw);

void UpdateTerrain(void)
{
    MapWin_UpdateTerrain_all(&g_map_window);
}
