/* MSLButtonDown: candidate for 1078:2cd2 (41 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Whether the left mouse button (virtual key 1) is down right now.
 *
 * RENAMED 2026-09-26 from mouse_left_down_1078_2cd2: the platform routine the Screen method forwards to; with that forwarder now Screen_LButtonDownNow, nothing else shares the stem and the suffix no longer tells copies apart.
 *
 * RENAMED 2026-09-27 from mouse_left_down: the Mac build's MSLButtonDown(void), bound by call-sequence alignment: the body tests GetAsyncKeyState(VK_LBUTTON).
 */

#include "colonization.h"

int MSLButtonDown(void)
{
    if ((unsigned char)(GetAsyncKeyState(1) >> 8) != 0)
        return 1;
    return 0;
}

