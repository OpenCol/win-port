/* text_dialog_1030_f158: candidate for 1030:f158 (42 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * One step of the TEXT dialog chain, which every module that shows a message carries a
 * copy of: supply the `GAME` resource file, add a zero, move an argument along, reach
 * `run_text_dialog`. Segment 1008's copy was read instruction by instruction
 * (`show_text_dialog_1008_cdf3` and the three below it); these are the same four steps
 * in the other modules, and the step each one reaches is read from its own
 * disassembly.
 *
 * The third step: its own third argument moves to the FOURTH place and a zero takes the
 * third. That reordering is what distinguishes it from a plain default-argument
 * wrapper -- the parameter it is given is not the parameter it passes on in the same
 * position.
 *
 * 42 bytes, not the census 38: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int text_dialog_1030_f158(void *file, void *name, int kind)
{
    return text_dialog_1030_f2a8(file, name, 0, kind);
}
