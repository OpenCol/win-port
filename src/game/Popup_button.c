/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 32 of the imported file. */
/* PORT: this file's own declarations of heap_get removed: the header declares it from the definition (W1 step 3). */
/* Popup_button: candidate for 1050:167f (135 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Appends a string to the control's list, up to THREE and then silently not at all: the
 * count at +0x82 gates the whole body, `strlen + 1` bytes come out of the control's own
 * arena at +0x392, the pointer goes into the -pointer array at +0x3b8, and the count goes
 * up.
 *
 * The arena is the one `Popup_set_title` uses for the single text at +0x362, so a control
 * carries one allocator and two things allocated from it.
 *
 * The count is read back from memory THREE times -- once for the guard, once to index the
 * store, once to index the copy -- and incremented from memory at the end. Nothing is kept
 * in a register across the two calls, which is what `-Od` gives.
 *
 * The array index is `count * 4` with a `SHL`, and the store is two words rather than one
 * 32-bit `MOV`, because the value arrives from `heap_get` already split in DX:AX.
 *
 * RENAMED 2026-09-26 from ctl_add_string: the Mac build's Popup::button(char*): this plus a caption, appended to the popup's button captions, at most three. It sits in the Popup module and the width matches.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */


void Popup_button(Popup *popup, char *s)
{
    if (popup->n_extra < 3) {
        popup->extra_labels[popup->n_extra] = (char *)heap_get((MemBlock *)(&popup->arena), STRLEN(s) + 1);
        STRCPY(popup->extra_labels[popup->n_extra], s);
        popup->n_extra++;
    }
}
