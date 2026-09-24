/* native_settlement_delete -- shipped at 1030:82f0.
 *
 * Ported by tools/port.py from matched/game/native_settlement_delete.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Tribe far *g_cur_tribe_rec;                   DGROUP:0x4c28
 *   extern int g_cur_tribe;                              DGROUP:0x4c30
 *   extern int g_cur_tribe_nation;                       DGROUP:0x4c32
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_settlement_count;                   SEG20:0x779c
 *   extern unsigned char far g_tribe_settlement_count[]; SEG20:0xdc10
 *
 * The file's own Unit, Tribe dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 lookup_with_two_overrides(col_i16 n);

#include "terrain.h"
#include "units.h"
extern void set_current_native_settlement(col_i16 s);
extern void map_plane1_set_bits(col_i16 x, col_i16 y, col_i16 mask, col_i16 on);
extern void unit_record_free(col_i16 u);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void set_name_slot_from_text(col_i16 slot, col_i16 word);
extern void text_message_show(char *msg, col_i16 n);

void native_settlement_delete(col_i16 s)
{
    col_i16 i;

    set_current_native_settlement(s);
    map_plane1_set_bits(g_settlements[s].x, g_settlements[s].y, MAP1_SETTLEMENT, 0);
    for (i = g_unit_count - 1; i >= 0; i--) {
        if ((unsigned char)(g_units[i].flags & UNIT_NATION_MASK) >= 4) {
            if (g_units[i].home == (char)s)
                unit_record_free(i);
            else if (g_units[i].home > (char)s)
                g_units[i].home--;
        }
    }
    for (i = s; g_settlement_count - 1 > i; i++)
        g_settlements[i] = g_settlements[i + 1];
    g_settlement_count--;
    g_tribe_settlement_count[g_cur_tribe]--;
    if (g_tribe_settlement_count[g_cur_tribe] > 0) {
        g_cur_tribe_rec->horse_taken -= g_cur_tribe_rec->horse_taken / (g_tribe_settlement_count[g_cur_tribe] + 1);
        g_cur_tribe_rec->horses -= g_cur_tribe_rec->horses / (g_tribe_settlement_count[g_cur_tribe] + 1);
    } else {
        g_cur_tribe_rec->flags |= 0x80;
        set_name_slot_from_text(0, lookup_with_two_overrides(g_cur_tribe_nation));
        text_message_show("EXTINCT", 3);
    }
}
