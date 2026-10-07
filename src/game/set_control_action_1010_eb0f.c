/* set_control_action_1010_eb0f: candidate for 1010:eb0f (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x1a of a record. Slot 0x1a is the one `call_checkbox_changed` invokes -- the two-word handler.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * The receiver is the SImageButton: its one caller, ColonyWin_Init, passes the SImageButtons at ColonyWin +0x7f4/+0x868/+0x8dc (ColonyWin_MiscButtonClick) and +0x994 (ColonyWin_OnOKClick).
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_control_action_1010_eb0f(SImageButton *button, void *h)
{
    button->on_click = (long)h;
}
