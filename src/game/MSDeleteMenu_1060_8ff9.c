/* MSDeleteMenu_1060_8ff9: candidate for 1060:8ff9 (28 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * This module's one-line forwarder to `MSDeleteMenu`, named for the routine it reaches.
 *
 * IT IGNORES ITS FIRST TWO WORDS. The menu handle it passes on comes from `[bp+0xa]`, not
 * `[bp+6]`, so two words sit in front of it that nothing in the function reads -- §119's
 * unused-parameter blind spot, visible here only because the frame steps over them.
 *
 * The callee is a call into another segment and is `extern`.
 */

#include "colonization.h"

void MSDeleteMenu_1060_8ff9(int unused1, int unused2, unsigned hmenu, unsigned id)
{
    MSDeleteMenu(hmenu, id);
}
