/* set_dialog_result: candidate for 1068:cbb8 (20 bytes).  Verdict: RELOCATABLE.
 *
 * Stores its second argument at DGROUP:0x56b4 (called from ask_choice).
 *
 * RENAMED 2026-09-26 from set_56b4: stores its value argument in g_dialog_result; ask_choice installs it
 * as the radio set's change callback, so the selected item is the result.
 */

#include "colonization.h"

void set_dialog_result(int unused1, int value)
{
    g_dialog_result = value;
}
