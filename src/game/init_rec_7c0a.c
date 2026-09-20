/* init_rec_7c0a -- shipped at 1020:8c3d.
 *
 * Ported by tools/port.py from matched/game/init_rec_7c0a.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Rec7c0a far g_rec_7c0a;                       SEG20:0x7c0a
 *
 * The file's own Rec7c0a dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void init_rec_7c0a(col_i16 a, col_i16 b)
{
    g_rec_7c0a.f0 = 0;
    g_rec_7c0a.f2 = a;
    g_rec_7c0a.f5 = b;
    g_rec_7c0a.f6 = b;
    g_rec_7c0a.f7 = 0;
    g_rec_7c0a.f4 = 0;
    g_rec_7c0a.fb = 0;
    g_rec_7c0a.f9 = 1;
}
