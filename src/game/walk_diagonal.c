/* walk_diagonal -- shipped at 1020:00a8.
 *
 * Ported by tools/port.py from matched/game/walk_diagonal.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern signed char g_x_lo;                           DGROUP:0x4bd6
 *   extern signed char g_y_lo;                           DGROUP:0x4bd7
 *   extern signed char g_x_hi;                           DGROUP:0x4bd8
 *   extern signed char g_y_hi;                           DGROUP:0x4bd9
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void mark_corner(col_i16 x, col_i16 y);

extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void check_timer_tick(col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */


void walk_diagonal(register col_i16 x, register col_i16 y)
{
    col_i16 n;
    col_i16 d;

    n = rand_range(1, 0x40) + 2;
    while (n-- != 0 && g_x_lo < x && g_x_hi > x && g_y_lo < y && g_y_hi > y) {
        mark_corner(x, y);
        d = rand_range(1, 4) * 2 - 1;
        x += g_dir_dx[d];
        y += g_dir_dy[d];
    }
    check_timer_tick(0);
}
