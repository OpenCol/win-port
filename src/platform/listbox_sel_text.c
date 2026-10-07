/* listbox_sel_text: candidate for 1070:54d4 (59 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * The text of the SELECTED item: ask the list box which item is current (`LB_GETCURSEL`,
 * 0x409) and hand that index to `listbox_get_text_1070_54b4`.
 *
 * This pairing is what numbers the family. A previous reading of this file guessed 0x409 was
 * `LB_GETCOUNT`, which cannot be right: a count used as an index is off by one, where the
 * current selection used as an index is the whole point. So 0x409 is `LB_GETCURSEL`, 0x40a
 * is `LB_GETTEXT`, and the `LB_*` block is numbered from `WM_USER+1` with no gap before
 * `LB_RESETCONTENT` at +5.
 *
 * `listbox_get_text_1070_54b4` is -called and defined in-file as a stub (rule 47).
 */

#include "colonization.h"

extern int g_stub_a;

void listbox_sel_text(int h, char *buf)
{
    int i;

    i = SendMessage(h, 0x409, 0, 0L);
    listbox_get_text_1070_54b4(h, i, buf);
}
