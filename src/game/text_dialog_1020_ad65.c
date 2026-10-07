/* text_dialog_1020_ad65: candidate for 1020:ad65 (39 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * One step of the TEXT dialog chain, which every module that shows a message carries a
 * copy of: supply the `GAME` resource file, add a zero, move an argument along, reach
 * `run_text_dialog`. Segment 1008's copy was read instruction by instruction
 * (`show_text_dialog_1008_cdf3` and the three below it); these are the same four steps
 * in the other modules, and the step each one reaches is read from its own
 * disassembly.
 *
 * The second step: adds the zero third argument.
 *
 * 39 bytes, not the census 35: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int text_dialog_1020_ad65(void *file, void *name)
{
    return text_dialog_1020_b05e(file, name, 0);
}
