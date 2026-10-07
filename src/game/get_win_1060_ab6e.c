/* get_win_1060_ab6e: candidate for 1060:ab6e (29 bytes).  Verdict: EXACT.
 *
 * The pointer at +6 of the object. The accessor exists so callers do not have to know
 * the offset, which is why it is 25 bytes of prologue around one LES.
 *
 * RENAMED 2026-09-26 from window_of: the window a game-window object holds at +6 -- byte-identical to the eight get_win_*
 * copies (analysis/re-exact-dupes.json), so it joins their name.
 */

#include "colonization.h"

#include "classes.h"

void *get_win_1060_ab6e(Screen *o)
{
    return o->win;
}
