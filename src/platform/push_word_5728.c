/* push_word_5728 -- shipped at 1078:1e4c.
 *
 * Ported by tools/port.py from matched/platform/push_word_5728.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_tab_5728[];                             DGROUP:0x5728
 *   extern int g_count_3c80;                             DGROUP:0x3c80
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void push_word_5728(col_i16 v)
{
    g_tab_5728[g_count_3c80] = v;
    g_count_3c80++;
}
