/* GenerateUnits: candidate for 1040:10da (61 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: GenerateUnits
 *
 * GenerateUnitsRegion over the current view (DGROUP:0x4c44/0x4c46 origin,
 * 0x4c50/0x4c52 size).  The painter is -called and defined in-file as
 * a stub.
 *
 * RENAMED 2026-09-26 from draw_units_view: the Mac build's GenerateUnits(void), bound by call-sequence alignment (1.0 aligned gaps in DrawMap leave it as the lone unbound callee, none against) and by layout (between GenerateUnitsRegion and DrawMapRegion on both sides); the arity agrees and the Mac body is 0.92x this one.
 */

#include "colonization.h"

extern int g_stub_a;

extern void GenerateUnitsRegion(int x, int y, int w, int h);

void GenerateUnits(void)
{
    GenerateUnitsRegion(g_view_x0, g_view_y0, g_view_w, g_view_h);
}

