/* ask_text_resource: candidate for 1048:dbb1 (37 bytes).  Verdict: RELOCATABLE.
 * symbol: ask_text_resource
 *
 * Asks the named text-resource question in the dialog context at DGROUP:0x24dc and returns
 * the answer. The module's wrapper text_dialog_1048_e6ed is defined here because the shipped call is
 * the compiler's own `push cs / call `; its own callee is not in this file.
 */

#include "colonization.h"

int ask_text_resource(char *name)
{
    return text_dialog_1048_e6ed(g_popup_game_txt, name);
}
