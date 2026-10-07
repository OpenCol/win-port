/* listbox_set_cursel: candidate for 1070:5531 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, LB_SETCURSEL, index, 0)`: selects an item in a list box. The setter
 * paired with `listbox_get_cursel` immediately before it. 0x407 is LB_SETCURSEL by the
 * WM_USER+1 numbering listbox_sel_text establishes, and the shape agrees: the caller's index
 * goes in wparam and lparam is zero.
 */

#include "colonization.h"

#include "w16const.h"

void listbox_set_cursel(unsigned hwnd, int index)
{
    SendMessage(hwnd, LB_SETCURSEL, index, 0L);
}
