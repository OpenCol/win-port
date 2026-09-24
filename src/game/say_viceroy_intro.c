/* say_viceroy_intro -- shipped at 1008:bcfb.
 *
 * Ported by tools/port.py from matched/game/say_viceroy_intro.c.
 *
 * STRCPY -> strcpy: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_player_nation;                      SEG20:0x779a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void show_endgame_speech(col_i16 a, col_i16 b, char *buf);

#include "nations.h"


extern void say_number(char *buf, col_i16 n);
/* same object: 4-byte `push cs; call ` (rule 47) */

void say_viceroy_intro(void)
{
    char buf[20];

    strcpy(buf, "VICEROY");
    if (g_player_nation == NATION_NETHERLANDS)
        say_number(buf, 2);
    show_endgame_speech(1, 1, buf);
}
