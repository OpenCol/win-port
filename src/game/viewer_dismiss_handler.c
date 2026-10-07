/* viewer_dismiss_handler: candidate for 1040:4668 (27 bytes).  Verdict: RELOCATABLE.
 *
 * A click handler of the debug picture viewer: sets the flag at DGROUP:0x4ca6 and reports 1.
 */

#include "colonization.h"

int viewer_dismiss_handler(void)
{
    g_viewer_dismissed = 1;
    return 1;
}
