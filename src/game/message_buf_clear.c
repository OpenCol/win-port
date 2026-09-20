/* message_buf_clear -- shipped at 1058:bead.
 *
 * Ported by tools/port.py from matched/game/message_buf_clear.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_message_buf[];                     DGROUP:0x4e6c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void message_buf_clear(void)
{
    g_message_buf[0] = 0;
}
