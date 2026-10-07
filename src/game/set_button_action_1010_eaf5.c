/* set_button_action_1010_eaf5: candidate for 1010:eaf5 (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x1c of a record. Slot 0x1c is the one `call_button_action` invokes, so this is how that handler is installed from the colony module.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_button_action_1010_eaf5(SButton *button, void *h)
{
    button->action = (long)h;
}
