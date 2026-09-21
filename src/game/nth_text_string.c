/* nth_text_string -- shipped at 1040:774e.
 *
 * Ported by tools/port.py from matched/game/nth_text_string.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_text_count;                             DGROUP:0x1cf4
 *   extern char g_empty_string[];                        DGROUP:0x1cf6
 *   extern char huge * far g_text_strings;               SEG29:0x10e0
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

char * nth_text_string(col_i16 n)
{
    char *p;

    if (n > g_text_count)
        return g_empty_string;
    p = g_text_strings;
    while (n != 0) {
        while (*p != 0)
            p++;
        p++;
        n--;
    }
    return p;
}
