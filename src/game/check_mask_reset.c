/* check_mask_reset: candidate for 1050:63b6 (20 bytes).  Verdict: RELOCATABLE.
 *
 * Clears the popup check-box mask at DGROUP:0x2514. `set_flag_bit` (1050:63ca) and
 * `test_check_mask` (1050:63fb) set and test single bits of that word, and `run_text_dialog` does this store INLINE -- `g_check_mask = 0;` before it rebuilds the
 * mask from the items -- so this function is that one statement standing alone.
 *
 * NO CALLER IN THE CENSUS, and the inline store in `run_text_dialog` is why: whatever called
 * this was folded into its caller, or dropped. Reconstructed because the store identifies it
 * exactly and the word it clears is already named from the other side.
 */

#include "colonization.h"

void check_mask_reset(void)
{
    g_check_mask = 0;
}
