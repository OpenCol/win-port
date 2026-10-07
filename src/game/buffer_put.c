/* buffer_put: candidate for 1040:3f96 (130 bytes, 35 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Stores a byte at (x, y) of a map plane given by its memory handle: lock, index as a
 * pointer by width * y then x, store, unlock. Rows outside 1..71 are ignored.
 *
 * RENAMED 2026-09-26 from map_handle_set: the Mac build's buffer_put(char**, short, short, unsigned char), bound by call-sequence alignment (15.0 aligned gaps in build_map leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.78x this one.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

void buffer_put(short handle, int x, int y, char value)
{
    short h = handle;
    int yy = y;
    unsigned char *p;

    if (yy < 0x48 && yy > 0) {
        p = (unsigned char *)MSHLock(h);
        p = p + g_map_size.w * yy + x;
        *p = value;
        p = (unsigned char *)MSHUnlock(h);
    }
}
