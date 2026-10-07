/* show_text_dialog_1008_cdf3: candidate for 1008:cdf3 (37 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Segment 1008's front door to the TEXT dialog: one argument, the message name, with the
 * resource file supplied. `g_popup_game_txt` is the string "GAME" at DGROUP:0x24dc -- read out
 * of the data segment -- and it is the same global `show_text_dialog_1048_e35c` passes for
 * the same purpose from the other module.
 *
 * 37 bytes, not the census 33: the trailing `return` is written out, which costs a second,
 * unreachable epilogue (rule 44).
 *
 * The callee is -called and defined in-file as a stub (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int show_text_dialog_1008_cdf3(void *name)
{
    return text_dialog_1008_db8c(g_popup_game_txt, name);
}
