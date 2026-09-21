/* europe_release_focus -- shipped at 1048:8d5d.
 *
 * Ported by tools/port.py from matched/game/europe_release_focus.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern WinRec far g_game_record;                     SEG20:0x05ca
 *   extern int far g_tutorial_on;                        DGROUP:0x00b4
 *   extern int g_stub_a;                                 
 *
 * The file's own EuropeWin, WinRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void MSSetFocus_of_1048_e0cd(WinRec *w);

/* same object: 4-byte `push cs; call ` (rule 47) */


void europe_release_focus(EuropeWin *o)
{
    o->nation = -1;
    g_tutorial_on = 0;
    MSSetFocus_of_1048_e0cd(&g_game_record);
}
