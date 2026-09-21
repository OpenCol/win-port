/* win_close_and_reset -- shipped at 1010:b004.
 *
 * Ported by tools/port.py from matched/game/win_close_and_reset.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_game_record;                       SEG20:0x05ca
 *
 * The file's own Win dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSHideWindow_of_1010_eb6e(void *port);
void MSSetFocus_of_1010_eb8f(void *p);

/* same object: 4-byte `push cs; call ` (rule 47) */


void win_close_and_reset(Win *w)
{
    MSHideWindow_of_1010_eb6e(((char *)((unsigned char *)(w) + 0x26)));
    w->fa12 = -1;
    MSSetFocus_of_1010_eb8f(&g_game_record);
}
