/* get_colony_name -- shipped at 1028:d1ba.
 *
 * Ported by tools/port.py from matched/game/get_colony_name.c.
 *
 * STRCAT -> strcat, STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

void get_colony_name(char *out, col_i16 unused1)
{
    strcpy(out, "COLONY");
    strcat(out, ".SAV");
}
