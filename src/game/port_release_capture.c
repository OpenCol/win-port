/* PORT: this file's own declarations of get_win_1048_e6d0 removed: the header declares it from the definition (W1 step 3). */
/* port_release_capture: candidate for 1048:db1f (37 bytes).  Verdict: RELOCATABLE.
 * symbol: port_release_capture
 *
 * win_release_capture on the port get_win_1048_e6d0 derives from the window (the
 * latter -called and defined in-file as a stub).
 *
 * RENAMED 2026-09-26 from port_call_2d3e: win_release_capture on the window the port belongs to.
 */

#include "colonization.h"

#include "classes.h"


void port_release_capture(Screen *w)
{
    win_release_capture(get_win_1048_e6d0(w));
}
