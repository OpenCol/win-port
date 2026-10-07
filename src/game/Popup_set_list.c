/* Popup_set_list: candidate for 1050:0bd9 (79 bytes, 1 caller).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Flags the control 0x1000, records a width and a column count, and sets its cell word (+0x8e)
 * to the text font's line height (MSGetLineHeight_1050_834b) times the count plus 2.
 *
 * RENAMED 2026-09-26 from ctl_set_columns: the Mac build's Popup::set_list(int, int): this plus a count and a width -- the list layout Popup_start sets up. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

#include "classes.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
/* same object: 4-byte `push cs; call ` (rule 47) */
extern int MSGetLineHeight_1050_834b(Font *font);
void Popup_set_list(Popup *popup, int n, int w)
{
    popup->flags |= 0x1000;
    popup->list_w = w;
    popup->list_rows = n;
    popup->list_h = MSGetLineHeight_1050_834b(popup->font) * n + 2;
}
