/* listbox_add_string: candidate for 1070:5414 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, LB_ADDSTRING, 0, s)`. One of four list-box message wrappers at
 * 1070:5414, 5433, 5496 and 54b4 (set_font_from_handle, a WM_SETFONT wrapper, sits at 5452), each a single `SendMessage` with a fixed message
 * number and the caller's arguments in the two parameter slots.
 *
 * The message numbers are the Win16 `LB_*` block at `WM_USER`, and this family is what fixes
 * their numbering here without a header: 0x409 takes nothing and its result is used as the
 * INDEX for 0x40a, which also takes a buffer -- so 0x409 is `LB_GETCURSEL` and 0x40a is
 * `LB_GETTEXT`, not the other way round, and the rest follow from the offsets.
 */

#include "colonization.h"

void listbox_add_string(unsigned hwnd, const char *s)
{
    SendMessage(hwnd, 0x401, 0, (long)s);
}
