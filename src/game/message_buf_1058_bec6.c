/* message_buf_1058_bec6 -- shipped at 1058:bec6.
 *
 * Ported by tools/port.py from matched/game/message_buf_1058_bec6.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_message_buf[];                     DGROUP:0x4e6c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * message_buf_1058_bec6(void)
{
    return g_message_buf;
}
