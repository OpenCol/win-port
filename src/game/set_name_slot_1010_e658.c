/* set_name_slot_1010_e658 -- shipped at 1010:e658.
 *
 * Ported by tools/port.py from matched/game/set_name_slot_1010_e658.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_name_slots[][0x50];                SEG31:0x00a4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void set_name_slot_1010_e658(col_i16 i, char *s)
{
    strcpy(g_name_slots[i], s);
}
