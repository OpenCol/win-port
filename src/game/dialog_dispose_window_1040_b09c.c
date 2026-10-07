/* PORT: this file's own declarations of MSDisposeWindow removed: the header declares it from the definition (W1 step 3). */
/* dialog_dispose_window_1040_b09c: candidate for 1040:b09c (44 bytes).  Verdict: RELOCATABLE.
 *
 * Replaces the dialog's window record pointer (+6) with what MSDisposeWindow
 * returns for it.
 *
 * RENAMED 2026-09-26 from menu_attach_1040_b09c: disposes the DialogWin's window record at +6 through MSDisposeWindow and clears the pointer
 * (it was read as attaching a menu, after MSDisposeWindow's old name).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void dialog_dispose_window_1040_b09c(DialogWin *dialog)
{
    dialog->win = MSDisposeWindow((MSWin *)dialog->win);
}
