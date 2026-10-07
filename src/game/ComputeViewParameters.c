/* PORT: this file's own declarations of MapWin_ComputeViewParameters removed: the header declares it from the definition (W1 step 3). */
/* ComputeViewParameters: candidate for 1040:0597 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Segment 1040's one-line forwarder to `MapWin_ComputeViewParameters` with the map window at
 * SEG20:0x658, named for the routine it reaches. The module's redraw pair on the same
 * window, UpdateTerrain and ShowMap, sits further along at 1040:07b9/07d5.
 *
 * The callee is a call into another segment and is `extern`.
 *
 * TRANSFERRED from `UpdateTerrain` (1040:07b9), byte-identical except for the one relocated call:
 * this copy reaches `MapWin_ComputeViewParameters` where that one reaches `MapWin_UpdateTerrain_all`. The comparator masks that
 * word, so the name comes from the disassembly and not from the verdict.
 *
 * RENAMED 2026-09-27 from map_view_layout_1040_0597: free ComputeViewParameters(void), the Mac caller of the method; not width-checked here
 */

#include "colonization.h"

#include "classes.h"


void ComputeViewParameters(void)
{
    MapWin_ComputeViewParameters(&g_map_window);
}
