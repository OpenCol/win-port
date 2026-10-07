/* PORT: this file's own declarations of MSSetWinTitle_of_1008_dbb3 removed: the header declares it from the definition (W1 step 3). */
/* win_set_caption_1008_cf0f: candidate for 1008:cf0f (75 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * Module 1008's copy of `win_set_caption_1048_d48b`: hand the caption to the window's port
 * at +0x26, keep a copy in the window's own buffer at +0xc4, and redraw the title bar.
 *
 * The body is the same three steps; the CALLEES are not. This module's title setter is
 * `MSSetWinTitle_of_1008_dbb3`, reached by the four-byte `push cs; call ` and so defined
 * here (rule 47), and ColChild_DrawTitleBar lives in segment 1048 -- a call, so extern,
 * where the 1048 copy -calls it.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void ColChild_DrawTitleBar(ColChild *w);

/* same object: 4-byte `push cs; call ` (rule 47) */

void win_set_caption_1008_cf0f(char *w, char *s)
{
    MSSetWinTitle_of_1008_dbb3((DialogWin *)(w + 0x26), s);
    STRCPY(w + 0xc4, s);
    ColChild_DrawTitleBar((ColChild *)w);
}
