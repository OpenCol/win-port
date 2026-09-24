/* map_handle_set -- shipped at 1040:3f96.
 *
 * Ported by tools/port.py from matched/game/map_handle_set.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);

void map_handle_set(col_i16 handle, col_i16 x, col_i16 y, char v)
{
    col_i16 h = handle;
    col_i16 yy = y;
    unsigned char *p;

    if (yy < 0x48 && yy > 0) {
        p = (unsigned char *)MSHLock(h);
        p = p + (*g_map_width) * yy + x;
        *p = v;
        p = (unsigned char *)MSHUnlock(h);
    }
}
