/* PORT: a callback is stored as intptr_t, not long: a pointer is 8 bytes on 64-bit Windows (W9). */
/* set_button_action_1058_c17f: candidate for 1058:c17f (26 bytes).  Verdict: EXACT.
 *
 * Stores the button's click handler (pointer at +0x1c).
 *
 * RENAMED 2026-09-26 from SButton_set_click_handler: this module's copy of set_button_action: stores the pointer
 * at +0x1c that call_button_action runs when the button is pressed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_button_action_1058_c17f(SButton *button, void *h)
{
    button->action = (intptr_t)h;
}
