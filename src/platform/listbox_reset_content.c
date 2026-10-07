/* listbox_reset_content: candidate for 1070:5496 (30 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, LB_RESETCONTENT, 0, 0)`: empty the list. Both message arguments are
 * constants, which is why this member is a byte shorter than the two beside it.
 */

#include "colonization.h"

void listbox_reset_content(unsigned hwnd)
{
    SendMessage(hwnd, 0x405, 0, 0L);
}
