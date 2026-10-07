/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 25 of the imported file. */
/* PORT: this file's own declarations of show_window_1050_8bc8 removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 22 of the imported file. */
/* SuperWin_Execute: candidate for 1050:6c4c (84 bytes, 4 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Calls copy_all_to_screen and show_window_1050_8bc8 on the window and win_center on its port,
 * all BEFORE the loop; then pumps the modal loop while the done flag is clear, drains the task
 * queue once more, and clears the flag. Nothing after the loop closes anything.
 *
 * RENAMED 2026-09-26 from run_modal_until_done: the Mac build's SuperWin::Execute(void): this only; all four callers run SuperWin_Init(&dlg) and then this on the same object, and it pumps until g_modal_done -- the flag SuperWin's three dismiss handlers set. The width matches.
 */

#include "colonization.h"

#include "classes.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern void win_center_1050_851f(DialogWin *port);

void SuperWin_Execute(SuperWin *w)
{
    copy_all_to_screen_1050_843b((GameWin *)(w));
    win_center_1050_851f((DialogWin *)((char *)w + 0x26));
    show_window_1050_8bc8((GameWin *)(w));
    MSDoAllTasks_1050_8be7();
    while (g_modal_done == 0)
        MSDoTask_1050_8d23();
    MSDoAllTasks_1050_8be7();
    g_modal_done = 0;
}
