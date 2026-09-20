/* shift_cells_up -- shipped at 1028:00fc.
 *
 * Ported by tools/port.py from matched/game/shift_cells_up.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern long far g_cells_l[][64];                     SEG24:0x0000
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void shift_cells_up(int row, int n)
{
 register int i;

 for (i = 0x3e; i >= n; i--)
 g_cells_l[row][i + 1] = g_cells_l[row][i];
}
