/* get_win_1048_e6d0: candidate for 1048:e6d0 (29 bytes).  Verdict: EXACT.
 * size-source: 29, not the census 25. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * The pointer at +6 of the object -- module 1048's copy of `get_win_1060_ab6e` (1060:ab6e),
 * to the byte. The accessor exists so callers need not know the offset, which is why it is
 * 25 bytes of prologue around one `LES`.
 *
 * RENAMED 2026-09-26 from window_of_1048_e6d0: the window a game-window object holds at +6 -- byte-identical to the eight get_win_*
 * copies (analysis/re-exact-dupes.json), so it joins their name.
 */

#include "colonization.h"

#include "classes.h"

void *get_win_1048_e6d0(Screen *o)
{
    return o->win;
}
