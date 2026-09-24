/* colonist_at_x -- shipped at 1010:5dac.
 *
 * Ported by tools/port.py from matched/game/colonist_at_x.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *   extern int far g_tile_colonists;                     DGROUP:0x4bb2
 *   extern signed char far g_portrait_gap;               DGROUP:0x4bc0
 *   extern char far g_icon_sprites[][32];                SEG20:0x6246
 *   extern int g_stub_a;                                 
 *
 * The file's own Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 max_int_1010_e632(col_i16 a, col_i16 b);
col_i16 rect_width_at_8_1010_e60f(void *sprite);

extern col_i16 colonist_portrait(col_i16 c);





col_i16 colonist_at_x(col_i16 px, col_i16 unused1)
{
    col_i16 i;
    col_i16 x;
    col_i16 found;
    col_i16 done;
    col_i16 total;
    col_i16 pic;
    register col_i16 w;
    register col_i16 carry;

    total = g_cur_colony_rec->pop + g_tile_colonists;
    found = g_cur_colony_rec->pop + g_tile_colonists - 1;
    done = 0;
    x = 1;
    carry = 0;
    for (i = 0; done == 0 && i < total; i++) {
        pic = colonist_portrait(i);
        if (pic != 0x5a)
            w = rect_width_at_8_1010_e60f((&g_icon_sprites[pic])) + g_portrait_gap;
        else
            w = rect_width_at_8_1010_e60f((&g_icon_sprites[pic])) / 2 + g_portrait_gap;
        if (w < 1)
            carry += 1 - w;
        w = max_int_1010_e632(1, w);
        while (w > 1 && carry > 0) {
            w--;
            carry--;
        }
        x += w;
        if (g_cur_colony_rec->pop - 1 == i)
            x += 8;
        if (px < x) {
            found = i;
            done = 1;
        }
    }
    return found;
}
