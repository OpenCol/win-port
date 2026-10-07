/* list_find_by_field6: candidate for 1050:0da7 (102 bytes).  Verdict: EXACT.
 *
 * First edit item in the popup's list at +0x386 whose field +6 equals key (items link at
 * +0x10); 0 when none.
 */

#include "colonization.h"

#include "records.h"
/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
#include "classes.h"   /* the class, for C: the port, the two sprites and the arena (a MemBlock) as their bytes */

PopupEdit *list_find_by_field6(Popup *dialog, int key)
{
    PopupEdit *found = 0;
    PopupEdit *p = dialog->edit_items;

    while (p != 0 && found == 0) {
        if (p->index == key)
            found = p;
        p = p->next;
    }
    return found;
}
