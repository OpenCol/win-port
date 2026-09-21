/* neighbor_terrain_scan_1040_02cc -- shipped at 1040:02cc.
 *
 * Ported by tools/port.py from matched/game/neighbor_terrain_scan_1040_02cc.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char huge *g_map_tiles2;             DGROUP:0x4c82
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern signed char far g_dir_dx[];                   SEG27:0x0000
 *   extern signed char far g_dir_dy[];                   SEG27:0x0009
 *   extern unsigned char g_4c9e;                         DGROUP:0x4c9e
 *   extern unsigned char g_4ca1;                         DGROUP:0x4ca1
 *   extern unsigned char g_sq_class;                     DGROUP:0x4c9d
 *   extern unsigned char g_sq_p0;                        DGROUP:0x4c9c
 *   extern unsigned char g_4ca2[];                       DGROUP:0x4ca2
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "terrain.h"










extern char terrain_class(col_i16 t);

col_i16 neighbor_terrain_scan_1040_02cc(void)
{
    col_i16 i;
    col_i16 j;
    unsigned char t;
    char cls;
    register col_i16 a;
    register col_i16 b;

    g_4c9e = 0;
    g_4ca1 = 0;
    for (i = 0; i < 4; i++)
        g_4ca2[i] = 0;
    for (i = 0; i < 8; i++) {
        t = *(g_map_tiles2 + g_dir_dx[i]
              + ((col_i16)g_dir_dy[i] < 0 ? -(*g_map_width) : 0)
              + ((col_i16)g_dir_dy[i] > 0 ? (*g_map_width) : 0));
        t = t & 0x1f;
        t = t < 0x18 ? t & 7 : t;
        cls = terrain_class(t);
        if (cls != TERRAIN_OCEAN && cls != TERRAIN_SEA_LANE) {
            g_4ca1 |= 1 << i;
            g_4c9e++;
            if (i & 1) {
                a = ((i + 1) & 7) >> 1;
                g_4ca2[a] |= 2;
            } else {
                g_sq_p0 = t;
                j = i >> 1;
                a = j;
                b = (j + 1) & 3;
                g_4ca2[a] |= 4;
                g_4ca2[b] |= 1;
            }
        }
    }
    g_sq_class = terrain_class(g_sq_p0);
    return g_4c9e;
}
