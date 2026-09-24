/* tally_natives -- shipped at 1030:2822.
 *
 * Ported by tools/port.py from matched/game/tally_natives.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Settlement far * far g_cur_settlement_rec;    DGROUP:0x4c2c
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern unsigned char far g_tribe_strength[];         SEG20:0xdc00
 *   extern unsigned char far g_tribe_pop[];              SEG20:0xdc08
 *   extern unsigned char far g_tribe_settlement_count[]; SEG20:0xdc10
 *   extern unsigned char far g_tribe_region[][0x10];     SEG20:0xdc18
 *   extern unsigned char far g_region_settlements[];     SEG20:0xda60
 *
 * The file's own Settlement, Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void add_clamped_255(unsigned char *p, col_i16 delta);

#define NATION(u) ((unsigned char)((u).flags & 0xf))









extern void set_current_native_settlement(col_i16 s);
extern col_i16 square_region_if_land(col_i16 x, col_i16 y);
extern col_i16 unit_combat_strength(col_i16 u, col_i16 a);
extern col_i16 unit_tile_nibble(col_i16 u);

/* same object: 4-byte `push cs; call ` */


void tally_natives(register col_i16 tribe)
{
    col_i16 owner;
    col_i16 cell;
    col_i16 str;
    register col_i16 i;

    owner = tribe + 4;
    g_tribe_strength[tribe] = 0;
    g_tribe_pop[tribe] = 0;
    g_tribe_settlement_count[tribe] = 0;
    for (i = 0; i < 0x10; i++) {
        g_tribe_region[tribe][i] = 0;
        g_region_settlements[i] = 0;
    }
    for (i = 0; g_settlement_count > i; i++) {
        set_current_native_settlement(i);
        if (g_cur_settlement_rec->owner == (unsigned char)owner) {
            g_tribe_settlement_count[tribe]++;
            g_tribe_pop[tribe] += g_cur_settlement_rec->size;
            cell = square_region_if_land(g_cur_settlement_rec->x,
                                             g_cur_settlement_rec->y);
            g_region_settlements[cell]++;
        }
    }
    for (i = 0; g_unit_count > i; i++) {
        if (NATION(g_units[i]) == (unsigned char)owner) {
            str = unit_combat_strength(i, 1);
            add_clamped_255(&g_tribe_strength[tribe], str);
            cell = unit_tile_nibble(i);
            if (cell >= 0)
                add_clamped_255(&g_tribe_region[tribe][cell], str);
        }
    }
}
