/* PORT: a callback is stored as intptr_t, not long: a pointer is 8 bytes on 64-bit Windows (W9). */
/* set_button_action_1068_db5c: candidate for 1068:db5c (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x1c of a record. Slot 0x1c is the one `call_button_action` invokes, so this is how that handler is installed in the graphics module (1068): PostLowMemReport, ask_choice and text_prompt_unused set their OK buttons' actions through it.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * TRANSFERRED: byte-identical to `set_button_action_1010_eaf5` (1010:eaf5), one of 6 copies of this
 * function in the image. Same source, this segment's copy.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_button_action_1068_db5c(SButton *button, void *h)
{
    button->action = (intptr_t)h;
}
