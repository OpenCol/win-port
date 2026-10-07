/* ctrl_key_down: candidate for 1070:9b58 (37 bytes).  Verdict: RELOCATABLE.
 *
 * Whether the Control key (virtual key 0x11) is down.
 */

#include "colonization.h"

int ctrl_key_down(void)
{
    return (GetKeyState(0x11) & 0x8000) != 0;
}

