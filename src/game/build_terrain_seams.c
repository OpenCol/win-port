/* build_terrain_seams -- shipped at 1008:7ec9.
 *
 * Ported by tools/port.py from matched/game/build_terrain_seams.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_map_pixels;                             DGROUP:0x4b2e
 *   extern int far g_map_width;                          DGROUP:0x4c40
 *   extern int far g_map_h;                              DGROUP:0x4c42
 *   extern signed char far g_ortho_dx[];                 DGROUP:0x1c58
 *   extern signed char far g_ortho_dy[];                 DGROUP:0x1c5d
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
signed char cell_draw_terrain(col_i16 x, col_i16 y);

#include "terrain.h"





extern col_u16 MSAllocateHandle(col_u32 size);
extern void * MSHLock(col_i16 h);
extern void * MSHUnlock(col_i16 h);
extern unsigned char map_plane0_at(col_i16 x, col_i16 y);

/* same object: 4-byte `push cs; call ` */


void build_terrain_seams(void)
{
    void *p;
    unsigned char *q;
    unsigned char v;
    signed char t;
    unsigned char base;
    unsigned char nb;
    col_i16 y;
    col_i16 x;
    col_i16 d;
    register col_i16 sx;
    register col_i16 sy;

    if (g_map_pixels == 0)
        g_map_pixels = MSAllocateHandle((col_i32)(*g_map_width) * g_map_h * 4);
    p = MSHLock(g_map_pixels);
    q = p;
    for (y = 0; g_map_h > y; y++) {
        for (x = 0; (*g_map_width) > x; x++) {
            base = map_plane0_at(x, y) & MAP0_TERRAIN_MASK;
            for (d = 0; d < 4; d++) {
                v = 0xff;
                sx = g_ortho_dx[d] + x;
                sy = g_ortho_dy[d] + y;
                if (sx >= 0 && (*g_map_width) > sx && sy >= 0 && g_map_h > sy) {
                    nb = map_plane0_at(sx, sy) & MAP0_TERRAIN_MASK;
                    if (base != nb) {
                        if (nb <= 0x18) {
                            if (nb < 0x18) {
                                if (nb % 8 != base % 8)
                                    v = (nb % 8) * 4 + d;
                            } else if (nb == 0x18) {
                                v = d + 0x20;
                            }
                        } else {
                            if (nb == 0x19) {
                                if (base < 0x19) {
                                    t = cell_draw_terrain(sx, sy);
                                    if ((col_i16)t != -1) {
                                        if ((col_i16)t < 0x18) {
                                            if (t % 8 != base % 8)
                                                v = (t % 8) * 4 + d;
                                        } else if ((col_i16)t == 0x18) {
                                            v = d + 0x20;
                                        }
                                    }
                                } else if (base == 0x1a) {
                                    v = d + 0x28;
                                }
                            } else if (nb == 0x1a) {
                                if (base == 0x19)
                                    v = d + 0x24;
                            }
                        }
                    }
                }
                *q = v;
                q++;
            }
        }
    }
    p = MSHUnlock(g_map_pixels);
}
