/* opening_dismiss_1040_7d80: candidate for 1040:7d80 (20 bytes).  Verdict: RELOCATABLE.
 *
 * A button handler of show_ship's dialog: sets the flag at DGROUP:0x1d04.
 */

#include "colonization.h"

extern int g_opening_dismissed;   /* DGROUP:0x1d04 */

void opening_dismiss_1040_7d80(void)
{
    g_opening_dismissed = 1;
}
