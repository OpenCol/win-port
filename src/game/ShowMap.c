/* ShowMap: candidate for 1040:07d5 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * `UpdateTerrain`'s twin: the same map window at SEG20:0x658 handed to
 * `MapWin_ShowMap_all` instead. Named for the routine it reaches.
 *
 * RENAMED 2026-09-26 from copy_all_to_screen_1040_07d5: the Mac build's ShowMap(void), bound by call-sequence alignment (1.0 aligned gaps in combat_fight leave it as the lone unbound callee, none against) and by layout (between UpdateTerrainRegion and ShowMapRegion on both sides); the arity agrees and the Mac body is 1.21x this one.
 */

#include "colonization.h"

#include "classes.h"

extern void MapWin_ShowMap_all(void *mw);

void ShowMap(void)
{
    MapWin_ShowMap_all(&g_map_window);
}
