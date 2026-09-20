/* colony_known_to -- shipped at 1018:af4a.
 *
 * Ported by tools/port.py from matched/game/colony_known_to.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far g_colonies[];                      SEG20:0x7c28
 *   extern int far g_reveal_all;                         SEG20:0x77a4
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* 0xca bytes */



col_i16 colony_known_to(col_i16 c, col_i16 nation)
{
    return g_colonies[c].nation == (unsigned char)nation || g_reveal_all != 0 || ((unsigned char *)((unsigned char *)&(g_colonies[c]) + 0xba))[nation] != 0;
}
