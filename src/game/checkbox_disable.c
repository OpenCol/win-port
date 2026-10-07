/* checkbox_disable: candidate for 1050:87ba (23 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Zeroes the word at +0x20 of a check box. It was named by its offset while nothing
 * established what the word was; get_check_enabled's readers do (below).
 *
 * RENAMED 2026-09-26 from ctl_clear_20: clears a check box's +0x20 enabled word (the word get_check_enabled
 * reads, 0 painting it grey and skipping it for focus); Popup_make_controls calls it for an item
 * whose flags have bit 1, the popup's disabled bit.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void checkbox_disable(SCheckBox *checkbox)
{
    checkbox->enabled = 0;
}
