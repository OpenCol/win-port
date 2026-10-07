/* redraw_map_in_mode: candidate for 1040:12a3 (50 bytes).  Verdict: RELOCATABLE.
 *
 * Runs the map redraw (GenerateTerrainMap) with the mode word at DGROUP:0x1ca0
 * set to `mode`, then clears it.
 *
 * RENAMED 2026-09-26 from with_mode: GenerateTerrainMap with the mode word at DGROUP:0x1ca0 set to `mode`, then cleared --
 * the map redraw run in a given mode.
 */

#include "colonization.h"

extern void GenerateTerrainMap(int nation);

void redraw_map_in_mode(int nation, int mode)
{
    g_redraw_mode = mode;
    GenerateTerrainMap(nation);
    g_redraw_mode = 0;
}
