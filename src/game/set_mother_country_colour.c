/* set_mother_country_colour -- shipped at 1038:4f4a.
 *
 * Ported by tools/port.py from matched/game/set_mother_country_colour.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_nation_colour[];          DGROUP:0x009a
 *   extern int far g_mother_country_idx;                 SEG20:0x77d4
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void set_mother_country_colour(void)
{
    g_nation_colour[g_mother_country_idx] = 0xe;
}
