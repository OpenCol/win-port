/* show_plain_dialog_1008_cd53: candidate for 1008:cd53 (48 bytes).  Verdict: RELOCATABLE.
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
 * The callee's arguments are `(p, ctx, name, a, g)` and not six scalars: `orders_road` and
 * `orders_clear` already call this module's copy that way, and
 * `popup_ask_nocancel` proves the shape at the end of the chain (§119).
 *
 * 48 bytes, not the census 44: the written-out `return` adds a dead second epilogue
 * (rule 44). The callee is -called and is a stub here (rule 47).
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` */

int show_plain_dialog_1008_cd53(void *p, void *name, int kind)
{
    return text_dialog_plain_1008_db4e(p, g_popup_game_txt, name, 0, kind);
}
