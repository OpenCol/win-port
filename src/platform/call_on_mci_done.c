/* call_on_mci_done: candidate for 1070:a06a (34 bytes).  Verdict: EXACT.
 *
 * Invokes handler slot +144 (0x90) of the widget layer's handler table, without
 * arguments, when a handler is installed.
 *
 * RENAMED 2026-09-27 from call_on_create: its one caller runs it from MM_MCINOTIFY with wParam 1 = MCI_NOTIFY_SUCCESSFUL (the case was spelled WM_CREATE, also 1): it calls the handler at +0x90, just past the MCI record, when a notified play completes.
 */

#include "colonization.h"

#include "records.h"

void call_on_mci_done(Audio *table)
{
    if (table->on_mci_done != 0)
        (*table->on_mci_done)();
}
