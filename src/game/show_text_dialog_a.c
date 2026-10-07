/* show_text_dialog_a: candidate for 1028:e4b3 (36 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * A front door onto segment 1028's TEXT dialog chain that takes the chain's third argument
 * as well as the message name. Every module carries the chain as four steps -- in 1008,
 * `show_text_dialog(name)` supplies the "GAME" file, the next step adds a zero `a`, the
 * next a zero `b`, and the last reaches run_text_dialog. This one supplies the file and
 * passes `a` from its caller, so it enters at the THIRD step, `text_dialog_1028_e56e(file,
 * name, a)`, and skips the one that would have zeroed it. `a` is the slot run_text_dialog
 * returns its result through, so this is the entry for a caller that seeds it.
 *
 * `g_popup_game_txt` is the string "GAME" at DGROUP:0x24dc. The call is the four-byte
 * `push cs; call `, so the callee is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

/* same object: 4-byte `push cs; call ` (rule 47) */

int show_text_dialog_a(void *name, int kind)
{
    return text_dialog_1028_e56e(g_popup_game_txt, name, kind);
}
