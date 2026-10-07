/* win_draw_text_default: candidate for 1048:e326 (54 bytes, 1 caller).  Verdict: RELOCATABLE.
 *
 * size-source: 54, not the census 50. The trailing 4 bytes are a SECOND epilogue, and
 * they are there because the body ends in an explicit `return;` -- Borland emits an
 * epilogue for the return statement AND one for the end of the function (rule 44).
 *
 * The window's text drawer with the global dialog context g_popup_game_txt (the resource-file string "GAME") and the window's flag word +0x86.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

/* same object: 4-byte `push cs; call ` (rule 47) */

void win_draw_text_default(Popup *popup, char *name, int sel, int unused)
{
    text_dialog_plain_1048_e7f3(popup, g_popup_game_txt, name, sel, popup->flags);
    return;
}
