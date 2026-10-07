/* enable_window: candidate for 1070:5f76 (25 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `EnableWindow(hwnd, on)` and nothing else. Its twin at 1078:194f is `KillTimer` over
 * the same two-argument shape.
 */

#include "colonization.h"

void enable_window(unsigned hwnd, int on)
{
    EnableWindow(hwnd, on);
}
