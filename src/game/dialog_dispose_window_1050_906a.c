/* PORT: this file's own declarations of MSDisposeWindow removed: the header declares it from the definition (W1 step 3). */
/* dialog_dispose_window_1050_906a: candidate for 1050:906a (44 bytes).  Verdict: RELOCATABLE.
 *
 * Replaces the dialog's window pointer (+6) with what MSDisposeWindow
 * returns for it (segment 1050's copy).
 *
 * RENAMED 2026-09-26 from menu_attach_1050_906a: disposes the DialogWin's window record at +6 through MSDisposeWindow and clears the pointer
 * (it was read as attaching a menu, after MSDisposeWindow's old name).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"

void dialog_dispose_window_1050_906a(DialogWin *dialog)
{
    dialog->win = MSDisposeWindow((MSWin *)dialog->win);
}
