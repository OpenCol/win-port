/* Popup_set_draw_sprite: candidate for 1050:1060 (27 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores the popup's draw_sprite override (+0x38e), which Popup_draw calls in place of the
 * plain sprite draw. It sits beside Popup_set_on_click (+0x38a) among module 1050's Popup setters.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * RENAMED 2026-09-27 from set_p_38e: the record is the Popup: the setter sits among module 1050's Popup_set_* functions, and its offset is the member every Popup declaration names (result +0xa8, on_click +0x38a, draw_sprite +0x38e). The Mac build has no such method, so the name is descriptive.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

void Popup_set_draw_sprite(Popup *popup, void *h)
{
    popup->draw_sprite = (long)h;
}
