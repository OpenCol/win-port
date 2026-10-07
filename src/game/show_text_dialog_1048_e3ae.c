/* show_text_dialog_1048_e3ae: candidate for 1048:e3ae (40 bytes).  Verdict: RELOCATABLE.
 * symbol: show_text_dialog_1048_e3ae
 *
 * text_dialog_1048_e911 on the record at DGROUP:0x24dc.  The callee is
 * -called and defined in-file as a stub.
 */

#include "colonization.h"

extern int g_stub_a;

int show_text_dialog_1048_e3ae(char *name, int kind)
{
    return text_dialog_1048_e911(g_popup_game_txt, name, kind);
}
