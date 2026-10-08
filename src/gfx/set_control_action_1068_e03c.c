/* PORT: a callback is stored as intptr_t, not long: a pointer is 8 bytes on 64-bit Windows (W9). */
/* set_control_action_1068_e03c: candidate for 1068:e03c (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x1a of a record. Segment 1068's copy of the same slot 0x1a setter.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_control_action_1068_e03c(SRadioSet *radios, void *h)
{
    radios->on_change = (intptr_t)h;
}
