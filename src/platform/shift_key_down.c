/* shift_key_down: candidate for 1070:9b7d (37 bytes).  Verdict: RELOCATABLE.
 *
 * Whether the Shift key (virtual key 0x10) is down.
 */

#include "colonization.h"

int shift_key_down(void)
{
    return (GetKeyState(0x10) & 0x8000) != 0;
}

