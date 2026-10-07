/* list_hide_items: candidate for 1060:91ad (56 bytes).  Verdict: RELOCATABLE.
 *
 * Hides the window of every item (0x92-byte records at the pointer at
 * +0x2e, count at +0x26).
 */

#include "colonization.h"

#include "records.h"

void list_hide_items(ListWin *o)
{
    register int i;

    for (i = 0; o->count > i; i++)
        hide_hwnd(o->items[i].hwnd);
}
