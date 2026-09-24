/* unit_at -- shipped at 1038:1918.
 *
 * Ported by tools/port.py from matched/game/unit_at.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern int far g_unit_count;                         SEG20:0x779e
 *
 * The file's own Unit dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 unit_chain_tail(col_i16 u);
void set_text_arg_i_1038_86cc(col_i16 slot, col_i16 v);
void show_text_dialog_1038_865b(char *msg);

/* 0x1c bytes, DGROUP- array at SEG20:0xa208 */


extern col_i16 on_map(col_i16 x, col_i16 y);
extern col_i16 square_nation_bit0(col_i16 x, col_i16 y);
/* same object: 4-byte `push cs; call ` (rule 47) */


extern col_i16 lookup_with_two_overrides(col_i16 n);
extern void set_name_slot_from_text(col_i16 slot, col_i16 word);

col_i16 unit_at(col_i16 x, col_i16 y)
{
    col_i16 i;
    col_i16 u = -1;
    col_i16 onmap;

    onmap = on_map(x, y);
    if (onmap != 0 && square_nation_bit0(x, y) < 0)
        goto done;
    for (i = 0; u < 0 && g_unit_count > i; i++)
        if (g_units[i].x == (unsigned char)x && g_units[i].y == (unsigned char)y)
            u = i;
    u = unit_chain_tail(u);
    if (u < 0 && onmap != 0) {
        set_text_arg_i_1038_86cc(0, x);
        set_text_arg_i_1038_86cc(1, y);
        set_name_slot_from_text(0, lookup_with_two_overrides(square_nation_bit0(x, y)));
        show_text_dialog_1038_865b("UNITFLAG");
    }
done:
    return u;
}
