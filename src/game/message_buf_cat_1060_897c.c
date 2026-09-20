/* message_buf_cat_1060_897c -- shipped at 1060:897c.
 *
 * Ported by tools/port.py from matched/game/message_buf_cat_1060_897c.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_message_buf[];                     DGROUP:0x4e6c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void message_buf_cat_1060_897c(char *s)
{
    strcat(g_message_buf, s);
}
