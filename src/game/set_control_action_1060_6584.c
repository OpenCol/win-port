/* PORT: a callback is stored as intptr_t, not long: a pointer is 8 bytes on 64-bit Windows (W9). */
/* set_control_action_1060_6584: candidate for 1060:6584 (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x1a of a record. Slot 0x1a is the one `call_checkbox_changed` invokes -- the two-word handler.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * TRANSFERRED: byte-identical to `set_control_action_1010_eb0f` (1010:eb0f), one of 7 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_control_action_1060_6584(SImageButton *button, void *h)
{
    button->on_click = (intptr_t)h;
}
