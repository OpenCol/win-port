/* win_invalidate: candidate for 1070:5010 (27 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `InvalidateRect(hwnd, NULL, FALSE)`: marks the whole client area of a window for repaint
 * without erasing the background first.
 */

#include "colonization.h"

#include "w16types.h"

void win_invalidate(unsigned hwnd)
{
    InvalidateRect(hwnd, 0, 0);
}
