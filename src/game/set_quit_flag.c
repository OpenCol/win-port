/* set_quit_flag: candidate for 1050:658f (33 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Sets the quit flag at DGROUP:0x4b04 and returns 1 -- the close handler debug_palette installs
 * through set_on_close_1050_8ef2. MainWin_CanClose asks "DOS" before setting the same flag; this one does not ask.
 *
 * 33 bytes, not the census 29: the written-out `return` adds a dead second epilogue (rule 44).
 */

#include "colonization.h"

int set_quit_flag(void)
{
    g_quit_flag = 1;
    return 1;
}
