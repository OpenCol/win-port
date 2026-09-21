/* colony_fort_level -- shipped at 1020:7e1b.
 *
 * Ported by tools/port.py from matched/game/colony_fort_level.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "building.h"
extern col_i16 current_colony_flag_get(col_i16 b);

col_i16 colony_fort_level(void)
{
    register col_i16 n = 0;

    if (current_colony_flag_get(BUILDING_STOCKADE) != 0)
        n++;
    if (current_colony_flag_get(BUILDING_FORT) != 0)
        n++;
    if (current_colony_flag_get(BUILDING_FORTRESS) != 0)
        n++;
    return n;
}
