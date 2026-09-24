/* unit_record_free -- shipped at 1038:2532.
 *
 * Ported by tools/port.py from matched/game/unit_record_free.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern Settlement far g_settlements[];               SEG20:0xcb9a
 *   extern int far g_unit_count;                         SEG20:0x779e
 *   extern int far g_selected_unit;                      SEG20:0x7794
 *   extern unsigned char far g_nation_unit_count[4];     DGROUP:0x4b44
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void unit_unlink(col_i16 u);

#include "units.h"
#include "nations.h"
/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */

typedef struct { char pad[3]; unsigned char flags; char rest[0x12 - 4]; } Settlement_local;




/* same object: 4-byte `push cs; call ` (rule 47) */

void unit_record_free(col_i16 u)
{
    col_i16 owner;
    col_i16 i;

    if (u < 0)
        goto done;
    owner = (unsigned char)(g_units[u].flags & UNIT_NATION_MASK);
    if (owner >= 0 && owner < NATION_EUROPEAN_COUNT)
        g_nation_unit_count[owner]--;
    if (owner >= NATION_EUROPEAN_COUNT) {
        if ((col_i16)g_units[u].home >= 0)
            g_settlements[g_units[u].home].flags |= 1;
    }
    unit_unlink(u);
    for (i = u; g_unit_count - 1 > i; i++)
        g_units[i] = g_units[i + 1];
    g_unit_count--;
    for (i = 0; g_unit_count > i; i++) {
        if (g_units[i].link > u)
            g_units[i].link--;
        if (g_units[i].link2 > u)
            g_units[i].link2--;
    }
    if (g_selected_unit != 0 && g_selected_unit >= u)
        g_selected_unit--;
done:
    ;
}
