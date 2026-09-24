/* call_slot910_global -- shipped at 1050:3319.
 *
 * Ported by tools/port.py from matched/game/call_slot910_global.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far *g_cur_dialog;                       DGROUP:0x2500
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void popup_layout(void *p);

void call_slot910_global(void)
{
    if (g_cur_dialog != 0)
        popup_layout(g_cur_dialog);
}
