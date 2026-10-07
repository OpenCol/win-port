/* goods_row_reset -- shipped at 1038:44a9.
 *
 * Ported by tools/port.py from matched/game/goods_row_reset.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_row_count;                              DGROUP:0x4c3a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void goods_row_reset(void)
{
    g_row_count = 0;
}
