/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 27, 28 of the imported file. */
/* Popup_set_heap: candidate for 1050:00fa (55 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Empties a Popup's string arena by freeing it and allocating it again: `heap_destroy` on
 * the block at +0x392, then `heap_create(block, 9, size)`.
 *
 * +0x392 is the arena the popup's own text comes out of -- `Popup_button` puts each string
 * there and `Popup_string` says its lines come "out of the object's OWN arena at +0x392".
 * The one caller, `Popup_init`, calls this right after `Popup_close` with the
 * popup's word at +0x39c as the size, when a popup that is already up is being pointed at
 * new contents.
 *
 * Both calls are , into segment 1040, so both callees are extern.
 *
 * RENAMED 2026-09-26 from popup_arena_reset: the Mac build's Popup::set_heap(int): this plus a size, freeing the popup's string arena and allocating it again at that size. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */
extern void heap_destroy(MemBlock *block);
extern int heap_create(MemBlock *heap, int kind, unsigned size);

void Popup_set_heap(Popup *popup, unsigned size)
{
    heap_destroy((MemBlock *)(&popup->arena));
    heap_create((MemBlock *)(&popup->arena), 9, size);
}
