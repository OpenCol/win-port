/* store_and_call_9fad -- shipped at 1068:cbcc.
 *
 * Ported by tools/port.py from matched/gfx/store_and_call_9fad.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_dialog_result;                          DGROUP:0x56b4
 *   extern char far *g_dialog_owner;                     DGROUP:0x56b6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void end_modal_1060_9fad(void *d);

void store_and_call_9fad(col_i16 unused, col_i16 v)
{
    g_dialog_result = v;
    end_modal_1060_9fad(g_dialog_owner + 0x26);
}
