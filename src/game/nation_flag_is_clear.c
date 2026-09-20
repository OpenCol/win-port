/* nation_flag_is_clear -- shipped at 1020:9857.
 *
 * Ported by tools/port.py from matched/game/nation_flag_is_clear.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern int nation_flag_test(int nation, int bit);

int nation_flag_is_clear(int nation, int bit)
{
 if (nation_flag_test(nation, bit) != 0)
 return 0;
 return 1;
}
