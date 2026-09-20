/* get_scale_pair -- shipped at 1068:65e8.
 *
 * Ported by tools/port.py from matched/gfx/get_scale_pair.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_scale_num;                              DGROUP:0x2b0e
 *   extern int g_scale_den;                              DGROUP:0x2b10
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

void get_scale_pair(col_i16 *a, col_i16 *b)
{
    *a = g_scale_num;
    *b = g_scale_den;
}
