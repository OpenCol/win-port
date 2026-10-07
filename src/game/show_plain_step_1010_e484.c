/* show_plain_step_1010_e484: candidate for 1010:e484 (45 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The PLAIN dialog chain's own step in segment 1010, called directly by `colony_click_ship`,
 * `ColonyWin_ColonyClickUnits` and `colony_custom_house`: three pointers through, a zero appended. One of TWO copies.
 *
 * 45 bytes, not the census 41: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int show_plain_step_1010_e484(void *p, void *ctx, char *name)
{
    return text_plain_step_1010_ed15(p, ctx, name, 0);
}
