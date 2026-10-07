/* mouse_right_down_1078_2cfb: candidate for 1078:2cfb (41 bytes).  Verdict: RELOCATABLE.
 *
 * Whether the right mouse button (virtual key 2) is down right now.
 */

#include "colonization.h"

int mouse_right_down_1078_2cfb(void)
{
    if ((unsigned char)(GetAsyncKeyState(2) >> 8) != 0)
        return 1;
    return 0;
}

