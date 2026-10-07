/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 24 of the imported file. */
/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
/* Popup_set_title: candidate for 1050:0c96 (88 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Gives the control its own copy of a string: `strlen + 1` bytes out of the arena that
 * lives at +0x392 of the control itself, the returned pointer parked at +0x362, and the
 * string copied in. The allocation goes through `heap_get`, which takes the arena
 * by pointer and the size.
 *
 * The arena address is built as `(char *)c + 0x392` with the control's own segment --
 * the object owns its allocator rather than sharing a global one.
 *
 * RENAMED 2026-09-26 from ctl_copy_text: the Mac build's Popup::set_title(char*): this plus a string, copied into the popup's own arena (+0x392) and parked at +0x362 -- the title Popup_make_window gives the window. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */


void Popup_set_title(Popup *popup, char *s)
{
    popup->title = (char *)heap_get((MemBlock *)(&popup->arena), STRLEN(s) + 1);
    STRCPY(popup->title, s);
}
