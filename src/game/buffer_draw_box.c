/* buffer_draw_box: candidate for 1040:40ae (455 bytes, 3 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Draws a rectangle outline of value c into a map plane given by handle: memset for the
 * top row at y and the bottom row at y + rows, then two column loops of rows - 2 cells at
 * x and x + n, stepping by the map width. The width is reached as a global here.
 * `n` is an int: declared unsigned, the compiler gives DI to another parameter and the
 * shipped `MOV DI,[BP+0xE]` (rows) is gone.
 *
 * RENAMED 2026-09-26 from map_handle_frame: the Mac build's buffer_draw_box(char**, short, short, short, short, unsigned char), bound by call-graph agreement (3 vs 2) and by call-sequence alignment (1.5 aligned gaps in build_map leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 0.54x this one.
 */

#include "colonization.h"

#include "records.h"
extern MapSize g_map_size;   /* DGROUP:0x4c40, declared */

void buffer_draw_box(short h, int x, int y, int n, int rows, char value)
{
    unsigned char *p;
    unsigned char *q;
    int i;

    p = (unsigned char *)MSHLock(h);
    q = p + g_map_size.w * y + x;
    memset_rtl(q, (unsigned char)value, n);
    q = p + g_map_size.w * (y + rows) + x;
    memset_rtl(q, (unsigned char)value, n);
    q = p + g_map_size.w * (y + 1) + x;
    for (i = 0; rows - 2 > i; i++) {
        *q = value;
        q += g_map_size.w;
    }
    q = p + g_map_size.w * (y + 1) + x + n;
    for (i = 0; rows - 2 > i; i++) {
        *q = value;
        q += g_map_size.w;
    }
    p = (unsigned char *)MSHUnlock(h);
}
