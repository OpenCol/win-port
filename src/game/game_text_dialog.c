/* game_text_dialog: candidate for 1050:6419 (42 bytes).  Verdict: RELOCATABLE.
 * symbol: game_text_dialog
 *
 * text_dialog_1050_8e85 on the GAME text file (g_game_txt, DGROUP:0x9e) with three arguments and
 * a trailing 0.  The callee is -called and defined in-file as a stub.
 *
 * RENAMED 2026-09-26 from text_ctx_call_8e85: text_dialog on the shared TEXT context with three arguments and a
 * trailing 0.
 * RENAMED 2026-09-28 from text_ctx_dialog: DGROUP:0x9e is not a context, it is the string "GAME" --
 * the resource file every text_open(file, section) in the game names first. Now g_game_txt.
 */

#include "colonization.h"

extern int g_stub_a;

extern int text_dialog_1050_8e85(void *file, void *name, int sel, int kind);

int game_text_dialog(char *name, int sel)
{
    return text_dialog_1050_8e85(g_game_txt, name, sel, 0);
}
