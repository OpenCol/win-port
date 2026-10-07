/* show_text_dialog_1048_e35c: candidate for 1048:e35c (43 bytes).  Verdict: RELOCATABLE.
 * symbol: show_text_dialog_1048_e35c
 *
 * show_plain_step_1048_e8e4 with the record at DGROUP:0x24dc as its second argument.
 * The callee is -called and defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

int show_text_dialog_1048_e35c(Popup *w, char *name)
{
    return show_plain_step_1048_e8e4(w, g_popup_game_txt, name);
}
