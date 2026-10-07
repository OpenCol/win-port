/* Popup_set_result: candidate for 1050:102c (25 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a word at +0xa8 of a record, in the `set_w_<slot>_<addr>` shape the corpus already
 * has four of. One `MOV [ES:BX+0xa8],AX` through a record pointer, no return of the
 * previous value.
 *
 * The record is the Popup: this sits among the Popup_set_* setters of module 1050, and +0xa8
 * is the popup's `result`, which Popup_exec returns and key_click/button_click set. It has
 * no caller in the census.
 *
 * RENAMED 2026-09-27 from set_w_a8: the record is the Popup: the setter sits among module 1050's Popup_set_* functions, and its offset is the member every Popup declaration names (result +0xa8, on_click +0x38a, draw_sprite +0x38e). The Mac build has no such method, so the name is descriptive.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_result(Popup *popup, int v)
{
    popup->result = v;
}
