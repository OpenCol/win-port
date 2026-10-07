/* send_makelong: candidate for 1070:5afb (46 bytes).  Verdict: RELOCATABLE.
 *
 * Sends message 0x401 with lo and hi packed into the long parameter.
 */

#include "colonization.h"

void send_makelong(int h, int lo, int hi)
{
    SendMessage(h, 0x401, 0, (unsigned long)(unsigned)lo | ((unsigned long)(unsigned)hi << 16));
}
