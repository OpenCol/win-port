/* set_radio_dblclk_1050_88d6: candidate for 1050:88d6 (26 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Stores a pointer into the slot at +0x1e of a record. Its twin at slot 0x1e.
 *
 * One 32-bit `MOV` through ES, which is what a pointer written through a record
 * pointer costs; the plain-store shape, without the "return the previous value" the
 * 57-byte `set_handler_slot_*` family has.
 *
 * TRANSFERRED: byte-identical to `set_radio_dblclk_1068_e056` (1068:e056), one of 2 copies of this
 * function in the image. Same source, this segment's copy.
 *
 * RENAMED 2026-09-26 from set_p_1e_1050_88d6: stores a radio set's +0x1e double-click callback, the one
 * call_radio_dblclk fires; Popup_make_controls installs radio_double_click.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_radio_dblclk_1050_88d6(SRadioSet *radios, void *h)
{
    radios->on_dblclk = (long)h;
}
