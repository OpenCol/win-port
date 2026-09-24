/* map_handle_get -- shipped at 1040:3efb.
 *
 * Ported by tools/port.py from matched/game/map_handle_get.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);

unsigned char map_handle_get(col_i16 handle, col_i16 x, col_i16 y)
{
    col_i16 h = handle;
    col_i16 yy = y;
    unsigned char *p;
    unsigned char v;

    if (yy >= 0x48 || yy < 0)
        return 0;
    else {
        p = (unsigned char *)MSHLock(h);
        p = p + (*g_map_width) * yy + x;
        v = *p;
        p = (unsigned char *)MSHUnlock(h);
    }
    return v;
}
