/* set_focus: candidate for 1070:0593 (22 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SetFocus(hwnd)` and nothing else -- the widget layer's one-line wrapper over the USER
 * import. Its twin at 1078:28ce, the same 22 bytes, is `SetCursor`.
 */

#include "colonization.h"

void set_focus(unsigned hwnd)
{
    SetFocus(hwnd);
}
