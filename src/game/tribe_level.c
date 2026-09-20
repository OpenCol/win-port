/* tribe_level -- shipped at 1038:00b2.
 *
 * Ported by tools/port.py from matched/game/tribe_level.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Tribe far g_tribes[];                         SEG20:0xd182
 *
 * The file's own Tribe dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 tribe_level(col_i16 t)
{
    col_i16 level;

    switch (g_tribes[t].level) {
    case 0: level = 1; break;
    case 1: level = 1; break;
    case 2: level = 2; break;
    case 3:
    default: level = 3; break;
    }
    return level;
}
