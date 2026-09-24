/* scroll_get_range_ex_1060_ae1d -- shipped at 1060:ae1d.
 *
 * Ported by tools/port.py from matched/game/scroll_get_range_ex_1060_ae1d.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void * window_of(void *o);

extern void scroll_get_range_ex_1070_604d(void *w, col_i16 *lo, col_i16 *hi, col_i16 f);



void scroll_get_range_ex_1060_ae1d(void *o, col_i16 *lo, col_i16 *hi)
{
    scroll_get_range_ex_1070_604d(window_of(o), lo, hi, 1);
}
