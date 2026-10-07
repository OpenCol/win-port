/* ConformToViewSize: candidate for 1040:0635 (218 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The x/y/width/height form of `ConformToView`, which sits 130 bytes above it and
 * does the same job on left/top/right/bottom. This one converts to edges, clamps into the
 * view rectangle at DGROUP:0x4c44..0x4c4a, converts back, and floors both extents at zero
 * so a rectangle clipped away entirely comes back as 0 x 0 rather than negative.
 *
 * The edges are computed BEFORE the origin is clamped -- both into registers, SI and DI --
 * which is what makes the final `right - *x + 1` use the clamped origin and a right edge
 * computed from the unclamped one. That ordering is the whole function; written with the clamps first it would give
 * the same answer only when the rectangle already fitted.
 *
 * RENAMED 2026-09-26 from clip_xywh_to_map_rect: the Mac build's ConformToViewSize(short*, short*, short*, short*), bound by layout (between ConformToView and UpdateTerrainRegion on both sides); the arity agrees and the Mac body is 1.08x this one.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

void ConformToViewSize(int *x, int *y, int *w, int *h)
{
    register int right;
    register int bottom;

    right  = *x + *w - 1;
    bottom = *y + *h - 1;
    *x = max_int_1040_445d(*x, g_view_x0);
    *y = max_int_1040_445d(*y, g_view_y0);
    right  = min_int_1040_4483(right,  g_view_x1);
    bottom = min_int_1040_4483(bottom, g_view_y1);
    *w = right - *x + 1;
    *h = bottom - *y + 1;
    *w = max_int_1040_445d(0, *w);
    *h = max_int_1040_445d(0, *h);
}
