/* run_modal_until_done -- shipped at 1050:6c4c.
 *
 * Ported by tools/port.py from matched/game/run_modal_until_done.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_modal_done;                             DGROUP:0x2520
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSDoAllTasks_1050_8be7(void);
void MSDoTask_1050_8d23(void);
void close_dialog_at_26_1050_8bc8(Win *w);
void copy_all_to_screen_1050_843b(Win *w);
void win_center_1050_851f(void *port);

/* same object: 4-byte `push cs; call ` (rule 47) */





void run_modal_until_done(Win *w)
{
    copy_all_to_screen_1050_843b(w);
    win_center_1050_851f(((char *)((unsigned char *)(w) + 0x26)));
    close_dialog_at_26_1050_8bc8(w);
    MSDoAllTasks_1050_8be7();
    while (g_modal_done == 0)
        MSDoTask_1050_8d23();
    MSDoAllTasks_1050_8be7();
    g_modal_done = 0;
}
