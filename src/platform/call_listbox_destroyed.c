/* call_listbox_destroyed: candidate for 1070:9a76 (42 bytes).  Verdict: EXACT.
 *
 * Calls the handler stored at +0x1c of the table with the table's own word at +2
 * when one is set.
 *
 * RENAMED 2026-09-26 from call_handler_slot_28_self: calls the list box's +0x1c callback with its own id;
 * listbox_subclass_proc fires it on WM_DESTROY, its only caller.
 */

#include "colonization.h"

#include "records.h"

void call_listbox_destroyed(ListboxHandlers *listbox)
{
    if (listbox->handler != 0)
        (*listbox->handler)(listbox->id);
}
