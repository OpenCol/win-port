/* listbox_delete_string: candidate for 1070:5433 (31 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 *
 * `SendMessage(hwnd, LB_DELETESTRING, i, 0)` -- the index in the word slot and a zero long,
 * which is the mirror of `listbox_add_string`'s zero word and real long.
 */

#include "colonization.h"

void listbox_delete_string(unsigned hwnd, int i)
{
    SendMessage(hwnd, 0x403, i, 0L);
}
