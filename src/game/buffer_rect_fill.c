/* buffer_rect_fill: candidate for 1040:4018 (150 bytes, 6 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * memsets `n` bytes to `c` at (x, y + i) for `rows` rows of a map plane given by handle.
 *
 * RENAMED 2026-09-26 from map_handle_fill_rows: the Mac build's buffer_rect_fill(char**, short, short, short, short, unsigned char), bound by call-sequence alignment (1.0 aligned gaps in build_map leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.95x this one.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

void buffer_rect_fill(short h, int x, int y, unsigned n, int rows, char value)
{
    unsigned char *p;
    unsigned char *q;
    int i;

    p = (unsigned char *)MSHLock(h);
    for (i = 0; i < rows; i++) {
        q = p + g_map_size.w * (y + i) + x;
        memset_rtl(q, (unsigned char)value, n);
    }
    p = (unsigned char *)MSHUnlock(h);
}
