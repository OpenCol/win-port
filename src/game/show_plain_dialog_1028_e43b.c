/* show_plain_dialog_1028_e43b: candidate for 1028:e43b (48 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The `GAME` file supplied to the module's PLAIN dialog entry -- the one that goes to
 * `Popup_start` rather than to `run_text_dialog`. Its own three arguments are
 * passed on around an injected zero, which is what makes the fourth argument a default
 * rather than a pass-through.
 *
 * `g_popup_game_txt` is the string "GAME" at DGROUP:0x24dc, the same global every other
 * step of the chain carries.
 *
 * 48 bytes, not the census 44: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

int show_plain_dialog_1028_e43b(void *p, void *name, int kind)
{
    return text_dialog_plain_1028_e509(p, g_popup_game_txt, name, 0, kind);
}
