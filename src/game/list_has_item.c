/* list_has_item: candidate for 1060:911b (64 bytes).  Verdict: EXACT.
 *
 * 1 when one of the list's items has window handle `key`.
 */

#include "colonization.h"

#include "records.h"

int list_has_item(ListWin *o, int key)
{
    int i;

    for (i = 0; o->count > i; i++) {
        if (o->items[i].hwnd == key)
            return 1;
    }
    return 0;
}
