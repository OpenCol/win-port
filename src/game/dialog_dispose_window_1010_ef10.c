/* PORT: this file's own declarations of MSDisposeWindow removed: the header declares it from the definition (W1 step 3). */
/* dialog_dispose_window_1010_ef10: candidate for 1010:ef10 (44 bytes).  Verdict: RELOCATABLE.
 *
 * Replaces the dialog's window record pointer (+6) with what MSDisposeWindow
 * returns for it.
 *
 * TRANSFERRED: byte-identical to `dialog_dispose_window_1040_b09c` (1040:b09c), one of 7 copies of this
 * function in the image. Same source, this segment's copy.
 *
 * RENAMED 2026-09-26 from menu_attach_1010_ef10: disposes the DialogWin's window record at +6 through MSDisposeWindow and clears the pointer
 * (it was read as attaching a menu, after MSDisposeWindow's old name).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void dialog_dispose_window_1010_ef10(DialogWin *dialog)
{
    dialog->win = MSDisposeWindow((MSWin *)dialog->win);
}
