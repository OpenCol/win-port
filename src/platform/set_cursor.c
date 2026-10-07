/* set_cursor: candidate for 1078:28ce (22 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SetCursor(h)` and nothing else -- `set_focus`'s twin over a different USER
 * import.
 */

#include "colonization.h"

void set_cursor(unsigned h)
{
    SetCursor(h);
}
