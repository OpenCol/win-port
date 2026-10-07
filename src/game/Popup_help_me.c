/* Popup_help_me: candidate for 1050:0a88 (36 bytes).  Verdict: EXACT.
 * name-source: sibling-build
 *
 * Whether bit 7 of the byte at +0x86 of the record is set.
 *
 * RENAMED 2026-09-26 from flag_86_high_bit: whether the popup's second action was used: bit 0x80 of the
 * flags at +0x86, which button_click sets for a button of code 1, after callers enable
 * the action with bit 0x40. europe_purchase, europe_recruit, choose_freedom and
 * ColonyWin_colony_hammer_change answer it by opening the Colonopedia on the selected item and
 * re-running the popup; colony_select_profession uses it to flip pages and run_text_dialog
 * reports it as g_dialog_help_used. Medium confidence: the help reading is the majority use.
 *
 * RENAMED 2026-09-26 from popup_help_requested: the Mac build's Popup::help_me(void): this only, whether the popup's help action was used (bit 0x80 of the flags at +0x86, set by button_click for button code 1). It sits in the Popup module (1050) and the width matches.
 */

#include "colonization.h"

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

int Popup_help_me(Popup *popup)
{
    return (popup->flags & 0x80) != 0;
}
