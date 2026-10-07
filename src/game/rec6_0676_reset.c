/* rec6_0676_reset -- shipped at 1028:05a3.
 *
 * Ported by tools/port.py from matched/game/rec6_0676_reset.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Rec6 far g_table6[];                          SEG24:0x0676
 *
 * The file's own Rec6 dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: colony -> a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void rec6_0676_reset(void) { col_i16 i; for (i = 0; i < 16; i++) g_table6[i].a = -1; }
