/* dialog_ok_cancel -- shipped at 1068:cb6d.
 *
 * Ported by tools/port.py from matched/gfx/dialog_ok_cancel.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far *g_dialog_owner;                     DGROUP:0x56b6
 *   extern int g_dialog_result;                          DGROUP:0x56b4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void end_modal_1060_9fad(void *win);

void dialog_ok_cancel(col_i16 code)
{
    if (code == 0x65)
        end_modal_1060_9fad(g_dialog_owner + 0x26);
    else if (code == 0x66) {
        g_dialog_result = -1;
        end_modal_1060_9fad(g_dialog_owner + 0x26);
    }
}
