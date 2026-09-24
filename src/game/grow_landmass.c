/* grow_landmass -- shipped at 1020:031d.
 *
 * Ported by tools/port.py from matched/game/grow_landmass.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern short far g_plane3_handle;                    DGROUP:0x1c70
 *   extern short far g_plane1_handle;                    DGROUP:0x1c6c
 *   extern unsigned char huge *far g_plane3;             DGROUP:0x1c7e
 *   extern unsigned char huge *far g_plane1;             DGROUP:0x1c76
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern int g_land_count;                             DGROUP:0x4bd4
 *   extern int far g_seg22_2;                            SEG22:0x0002
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void random_walk_blob(col_i16 x, col_i16 y);
void walk_diagonal(col_i16 x, col_i16 y);
void walk_orthogonal(col_i16 x, col_i16 y);

extern void handle_fill_bytes(col_i16 handle, char c);
extern unsigned char map_handle_get(col_i16 handle, col_i16 x, col_i16 y);
extern col_i16 rand_range(col_i16 lo, col_i16 hi);
extern void check_timer_tick(col_i16 n);

/* same object: 4-byte `push cs; call ` (rule 47) */




void grow_landmass(col_i16 pick_land)
{
    col_i16 r;
    unsigned char *p;
    unsigned char *q;
    register col_i16 x;
    register col_i16 y;

    handle_fill_bytes(g_plane3_handle, 0);
    do {
        x = rand_range(1, (*g_map_width) - 0x10) + 7;
        y = rand_range(1, g_map_h - 8) + 3;
    } while (pick_land != 0 && map_handle_get(g_plane1_handle, x, y) != 0);
    if (pick_land != 0) {
        r = rand_range(1, 10);
        walk_orthogonal(x, y);
        if (r >= 7)
            walk_orthogonal(x, y);
        if (r >= 8)
            walk_orthogonal(x, y);
    } else {
        if (g_seg22_2 >= 2)
            random_walk_blob(x, y);
        else
            walk_diagonal(x, y);
    }
    p = g_plane3;
    q = g_plane1;
    for (y = 0; g_map_h > y; y++) {
        for (x = 0; (*g_map_width) > x; x++) {
            if (*p++ != 0) {
                (*q)++;
                g_land_count++;
            }
            q++;
        }
    }
    check_timer_tick(0);
}
