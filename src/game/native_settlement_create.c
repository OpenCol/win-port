/* native_settlement_create -- shipped at 1030:81f1.
 *
 * Ported by tools/port.py from matched/game/native_settlement_create.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern Settlement far *g_cur_settlement_rec;         DGROUP:0x4c2c
 *
 * The file's own Settlement dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: growth -> f6
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
char settle_initial_size(col_i16 s);

extern void set_current_native_settlement(col_i16 s);
extern void set_current_tribe(col_i16 t);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern unsigned char * map_plane1_ptr(col_i16 x, col_i16 y);
extern void map_plane2_set_high_nibble(col_i16 x, col_i16 y, col_i16 n);

col_i16 native_settlement_create(col_i16 nation, col_i16 x, col_i16 y)
{
    col_i16 s = -1;

    if (g_settlement_count >= 0x54)
        goto done;
    {
        s = g_settlement_count;
        g_settlement_count++;
        set_current_native_settlement(s);
        set_current_tribe(nation - 4);
        g_cur_settlement_rec->owner = nation;
        g_cur_settlement_rec->x = x;
        g_cur_settlement_rec->y = y;
        g_cur_settlement_rec->size = settle_initial_size(s);
        g_cur_settlement_rec->mission = 0xff;
        g_cur_settlement_rec->f6 = 0;
        *map_plane1_ptr(x, y) |= 2;
        map_plane2_set_high_nibble(x, y, nation);
        g_settlements[s].flags = 0;
        g_settlements[s].refused = 0xff;
        g_settlements[s].sold = 0xff;
        g_settlements[s].bought = 0xff;
    }
done:
    return s;
}
