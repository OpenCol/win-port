/* set_initial_focus: candidate for 1050:8b46 (22 bytes).  Verdict: EXACT.
 * name-source: descriptive
 *
 * Sets the byte at +0x16 of a dialog control to 1 and nothing else.
 *
 * It was named by its offset while nothing established what the flag meant; its readers do
 * (below).
 *
 * RENAMED 2026-09-26 from set_flag_16: marks the control that takes the initial focus. find_control
 * and DialogWin_SetDefaultControlFocus pick the first control with +0x16 set; Popup_make_controls marks the edit
 * list, else the radio set, else the last button.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

void set_initial_focus(DlgControl *control)
{
    control->initial_focus = 1;
}
