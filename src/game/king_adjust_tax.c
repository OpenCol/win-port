/* king_adjust_tax -- shipped at 1048:6a74.
 *
 * Ported by tools/port.py from matched/game/king_adjust_tax.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern int far g_turn_counter;                       SEG20:0x7790
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *
 * The file's own Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ask_text_resource(char *name);
void apply_tax_change(char *name, col_i16 delta);

extern col_i16 rand_range(col_i16 lo, col_i16 hi);

/* same object: 4-byte `push cs; call ` (rule 47) */

void king_adjust_tax(void)
{
    col_i16 q;
    col_i16 step;
    col_i16 lim;
    col_i16 r;

    if ((col_i16)g_cur_nation_rec->tax <= 1)
        goto raise;
    q = g_turn_counter / 400;
    step = ((g_difficulty >> 1) + 1) << 2;
    lim = (q + 1) * step;
    if (g_cur_nation_rec->tax <= (char)(lim + 5))
        goto raise;
    if (g_cur_nation_rec->tax <= (char)lim || rand_range(1, g_difficulty + 1) != 1) {
        ask_text_resource("KINGNOTHING");
        goto done;
    }
    r = 0 - rand_range(1, 5 - g_difficulty);
    apply_tax_change("KINGLOWER", r);
    goto done;
raise:
    r = rand_range(1, g_difficulty) * 2;
    apply_tax_change("KINGRAISE", r);
done:
    return;
}
