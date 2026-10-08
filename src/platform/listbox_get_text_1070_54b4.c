/* PORT: a window procedure's lParam is intptr_t, not long: it carries pointers, 8 bytes on 64-bit Windows (W9). */
/* listbox_get_text_1070_54b4: candidate for 1070:54b4 (32 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, LB_GETTEXT, i, buf)`: copy item `i`'s text into the caller's buffer.
 * The widest member of the family, because both of `SendMessage`'s argument slots carry a
 * real value.
 *
 * That it is `LB_GETTEXT` and not something else is settled by its own caller:
 * `listbox_sel_text` asks 0x409 for a number and passes that number here as `i`,
 * which only makes sense if 0x409 is `LB_GETCURSEL` and this is the text of an item.
 */

#include "colonization.h"

void listbox_get_text_1070_54b4(unsigned hwnd, int i, char *buf)
{
    SendMessage(hwnd, 0x40a, i, (intptr_t)buf);
}
