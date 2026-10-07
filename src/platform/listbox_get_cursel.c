/* listbox_get_cursel: candidate for 1070:550f (30 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, LB_GETCURSEL, 0, 0)`: the index of a list box's current selection.
 * `listbox_sel_text` (1070:54d4, the function immediately before) sends the same message inline and
 * is where 0x409 was pinned as LB_GETCURSEL; `listbox_set_cursel` immediately after this is
 * the setter.
 */

#include "colonization.h"

#include "w16const.h"

int listbox_get_cursel(unsigned hwnd)
{
    return (int)SendMessage(hwnd, LB_GETCURSEL, 0, 0L);
}
