/* get_win_1068_d72a: candidate for 1068:d72a (29 bytes).  Verdict: EXACT.
 * size-source: 29, not the census 25. The last 4 bytes are an unreachable second
 * epilogue; Ghidra sizes by reachable code.
 *
 * The pointer at +6 of the object -- module 1068's copy of `get_win_1060_ab6e` (1060:ab6e).
 * `audio_open_element` and `audio_play_element` both reach for it, and both pass the
 * result on to segment 1078 as the window an MCI command should notify.
 *
 * RENAMED 2026-09-26 from window_of_1068_d72a: the window a game-window object holds at +6 -- byte-identical to the eight get_win_*
 * copies (analysis/re-exact-dupes.json), so it joins their name.
 */

#include "colonization.h"

#include "classes.h"

void *get_win_1068_d72a(Screen *o)
{
    return o->win;
}
