/* PORT: this file's own declarations of MSDisposeWindow removed: the header declares it from the definition (W1 step 3). */
/* dialog_dispose_window_1060_6c08: candidate for 1060:6c08 (44 bytes).  Verdict: RELOCATABLE.
 *
 * Replaces the dialog's window pointer (+6) with what MSDisposeWindow
 * returns for it (segment 1060's copy).
 *
 * RENAMED 2026-09-26 from menu_attach_1060_6c08: disposes the DialogWin's window record at +6 through MSDisposeWindow and clears the pointer
 * (it was read as attaching a menu, after MSDisposeWindow's old name).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void dialog_dispose_window_1060_6c08(DialogWin *dialog)
{
    dialog->win = MSDisposeWindow((MSWin *)dialog->win);
}
