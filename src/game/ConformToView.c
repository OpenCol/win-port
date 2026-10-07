/* ConformToView: candidate for 1040:05b3 (130 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Clamps the four rectangle words into the view rectangle at DGROUP:0x4c44..0x4c4a
 * (declared in this module).
 *
 * RENAMED 2026-09-26 from clip_rect_to_map_rect: the Mac build's ConformToView(short*, short*, short*, short*), bound by call-sequence alignment (1.0 aligned gaps in GenerateUnitsRegion leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.17x this one.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void ConformToView(int *l, int *top, int *r, int *bottom)
{
    *l = max_int_1040_445d(*l, g_view_x0);
    *top = max_int_1040_445d(*top, g_view_y0);
    *r = min_int_1040_4483(*r, g_view_x1);
    *bottom = min_int_1040_4483(*bottom, g_view_y1);
}
