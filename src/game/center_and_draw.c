/* center_and_draw -- shipped at 1040:a51e.
 *
 * Ported by tools/port.py from matched/game/center_and_draw.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1040_aab7(col_i16 a, col_i16 b);
col_i16 return_arg2_1040_a4f4(void *w, col_i16 x, col_i16 d, col_i16 e);
col_i16 return_zero_1040_a4b7(void *w);

col_i16 center_and_draw(void *w, col_i16 x, col_i16 width, col_i16 d, col_i16 e)
{
    register col_i16 cx;

    cx = (width >> 1) + x - (return_zero_1040_a4b7(w) >> 1);
    cx = max_int_1040_aab7(0, cx);
    cx = return_arg2_1040_a4f4(w, cx, d, e);
    return cx;
}
