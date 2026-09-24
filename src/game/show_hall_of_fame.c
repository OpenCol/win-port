/* show_hall_of_fame -- shipped at 1050:6f2f.
 *
 * Ported by tools/port.py from matched/game/show_hall_of_fame.c.
 *
 * The file's own Fame dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void HallOfFame(Fame *entry, char show);

void show_hall_of_fame(void)
{
    HallOfFame(0, 0);
}
