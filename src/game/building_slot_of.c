/* building_slot_of -- shipped at 1018:47af, 46 bytes.
 *
 * The signed byte at SEG21:0x00f7 for a building under 0x13, and -1 for anything else.
 *
 * Ported from matched/game/building_slot_of.c.
 */
#include "col/globals.h"

int building_slot_of(int b)
{
    int r = -1;

    if (b < 0x13)
        r = g_tbl21_f7[b];
    return r;
}
