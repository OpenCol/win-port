/* set_pending_pair -- shipped at 1010:c146.
 *
 * Ported by tools/port.py from matched/game/set_pending_pair.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_pending_flag;                           DGROUP:0x0a10
 *   extern int g_pending_a;                              DGROUP:0x4b9a
 *   extern int g_pending_b;                              DGROUP:0x4b9c
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_pending_pair(col_i16 a, col_i16 b)
{
    g_pending_flag = 1;
    g_pending_a = a;
    g_pending_b = b;
}
