/* set_radio_dblclk_1068_e056: candidate for 1068:e056 (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x1e of a record. Its twin at slot 0x1e.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * RENAMED 2026-09-26 from set_p_1e_1068_e056: stores a radio set's +0x1e double-click callback, the one
 * call_radio_dblclk fires; ask_choice installs end_dialog_with_value.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_radio_dblclk_1068_e056(SRadioSet *radios, void *h)
{
    radios->on_dblclk = (long)h;
}
