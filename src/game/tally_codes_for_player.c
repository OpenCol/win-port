/* tally_codes_for_player -- shipped at 1030:2731.
 *
 * Ported by tools/port.py from matched/game/tally_codes_for_player.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern unsigned char far g_specialty_tally[0x1d];    SEG20:0xda2a
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_colony_count;                       SEG20:0x77a0
 *   extern Colony far * far g_cur_colony_rec;            DGROUP:0x4b9e
 *
 * The file's own Unit, Colony dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

extern col_i16 unit_colonist_class(col_i16 u);
extern void set_current_colony(col_i16 c);
extern col_i16 colonist_specialty_get(col_i16 k);

void tally_codes_for_player(col_i16 nation)
{
    col_i16 i;
    col_i16 col;
    col_i16 spec;
    col_i16 u;
    col_i16 k;

    for (i = 0; i < 0x1d; i++)
        g_specialty_tally[i] = 0;
    for (u = 0; g_unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) != (unsigned char)nation)
            continue;
        if (unit_colonist_class(u) >= 0)
            g_specialty_tally[g_units[u].spec]++;
    }
    for (col = 0; g_colony_count > col; col++) {
        set_current_colony(col);
        if (g_cur_colony_rec->nation != (unsigned char)nation)
            continue;
        for (k = 0; g_cur_colony_rec->pop > k; k++) {
            spec = colonist_specialty_get(k);
            g_specialty_tally[spec]++;
        }
    }
}
