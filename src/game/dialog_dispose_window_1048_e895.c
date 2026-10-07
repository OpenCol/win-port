/* dialog_dispose_window_1048_e895: candidate for 1048:e895 (44 bytes).  Verdict: RELOCATABLE.
 *
 * Replaces the dialog's window pointer (+6) with what MSDisposeWindow
 * returns for it (segment 1048's copy).
 *
 * RENAMED 2026-09-26 from menu_attach_1048_e895: disposes the DialogWin's window record at +6 through MSDisposeWindow and clears the pointer
 * (it was read as attaching a menu, after MSDisposeWindow's old name).
 */

#include "colonization.h"

#include "records.h"
#include "classes.h"
extern void *MSDisposeWindow(void *win);

void dialog_dispose_window_1048_e895(DialogWin *dialog)
{
    dialog->win = MSDisposeWindow((MSWin *)dialog->win);
}
