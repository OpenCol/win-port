/* say_number_1058_bede -- shipped at 1058:bede.
 *
 * Ported by tools/port.py from matched/game/say_number_1058_bede.c.
 *
 * STRCAT -> strcat: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_message_buf[];                     DGROUP:0x4e6c
 *   extern char far g_number_buf[];                      DGROUP:0x566c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

extern char * LTOA1(col_i16 v, char *s, col_i16 radix);


void say_number_1058_bede(col_i16 n)
{
    LTOA1(n, g_number_buf, 10);
    strcat(g_message_buf, g_number_buf);
}
