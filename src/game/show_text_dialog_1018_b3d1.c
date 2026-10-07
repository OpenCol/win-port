/* show_text_dialog_1018_b3d1: candidate for 1018:b3d1 (37 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * One step of the TEXT dialog chain, which every module that shows a message carries a
 * copy of: supply the `GAME` resource file, add a zero, move an argument along, reach
 * `run_text_dialog`. Segment 1008's copy was read instruction by instruction
 * (`show_text_dialog_1008_cdf3` and the three below it); these are the same four steps
 * in the other modules, and the step each one reaches is read from its own
 * disassembly.
 *
 * The front door: one argument, the message name, with the resource file supplied.
 * `g_popup_game_txt` is the string "GAME" at DGROUP:0x24dc.
 *
 * 37 bytes, not the census 33: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int show_text_dialog_1018_b3d1(void *name)
{
    return text_dialog_1018_b496(g_popup_game_txt, name);
}
