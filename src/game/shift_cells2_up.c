/* shift_cells2_up -- shipped at 1028:014c.
 *
 * Ported by tools/port.py from matched/game/shift_cells2_up.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_cells2[][16];                      SEG24:0x0400
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void shift_cells2_up(int row, int n)
{
 register int i;

 for (i = 0xe; i >= n; i--)
 g_cells2[row][i + 1] = g_cells2[row][i];
}
