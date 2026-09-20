/* message_buf_1030_e99a -- shipped at 1030:e99a.
 *
 * Ported by tools/port.py from matched/game/message_buf_1030_e99a.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_message_buf[];                     DGROUP:0x4e6c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * message_buf_1030_e99a(void)
{
    return g_message_buf;
}
