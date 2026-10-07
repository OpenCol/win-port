/* main_screen_refresh: candidate for 1038:b27d (14 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * An empty function: prologue, epilogue, nothing between. One of **126** in the image,
 * all byte-identical, and `text_close` carries the reading of what they are and why the
 * Macintosh's zero-argument `memory_check(void)` is NOT adopted for them.
 *
 * Recorded because that is the useful fact: a call site that looks like it does something
 * does not, and a reader who finds one need not follow it.
 *
 * RENAMED 2026-09-27 from empty_1038_b27d: Mac main_screen_refresh(void). On the Mac it is EMPTY too: LINK, UNLK, RTS (the 34 bytes are the symbol). Caller: give_me_liberty on both. Layout: Mac viceroy_message, viceroy_do_message, main_screen_refresh, tell_unit_to_europe; Windows viceroy_message, viceroy_do_message, empty_1038_b26f (0 callers, Windows-only), b27d, tell_unit_to_europe. Arity: void. The text_close precedent applies: an empty function takes the Mac name when the call site and the position both bind it. Here both bodies are empty, which is a line of agreement in itself.
 */

#include "colonization.h"

void main_screen_refresh(void)
{
}
