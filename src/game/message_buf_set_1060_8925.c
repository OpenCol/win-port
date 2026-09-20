/* message_buf_set_1060_8925 -- shipped at 1060:8925.
 *
 * Ported by tools/port.py from matched/game/message_buf_set_1060_8925.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_message_buf[];                     DGROUP:0x4e6c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void message_buf_set_1060_8925(char *s)
{
    strcpy(g_message_buf, s);
}
