/* king_tax_demand -- shipped at 1048:694e.
 *
 * Ported by tools/port.py from matched/game/king_tax_demand.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern Nation far *g_cur_nation_rec;                 DGROUP:0x4cc4
 *   extern int g_cur_nation;                             DGROUP:0x4cc8
 *   extern unsigned char far g_difficulty;               SEG20:0x77a8
 *   extern long far g_text_args[];                       SEG31:0x02d4
 *
 * The file's own NationRec52, Nation dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ask_text_resource(char *name);
col_i16 max_int_1048_dd61(col_i16 a, col_i16 b);
void apply_tax_change(char *name, col_i16 delta);
void sub_1048_4e81(void);

#include "nations.h"
#include "control.h"
/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

/* same object: 4-byte `push cs; call ` (rule 47) */

void king_tax_demand(void)
{
    col_i16 delta;
    col_i16 human;
    col_i16 amount;

    human = g_cur_nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_nation].control == CONTROL_HUMAN;
    if ((col_i16)g_cur_nation_rec->tax >= 0x3c) {
        if (human)
            ask_text_resource("KINGNO");
        goto done;
    }
    amount = 6 - (((g_cur_nation < NATION_EUROPEAN_COUNT && g_nation52[g_cur_nation].control == CONTROL_HUMAN) ? g_difficulty : 0) + ((g_cur_nation_rec->demands + 1) >> 1));
    amount = max_int_1048_dd61(1, amount);
    amount *= 100;
    g_text_args[0] = amount;
    if (human && ask_text_resource("KINGFUND") != 1)
        goto done;
    g_cur_nation_rec->gold += amount;
    delta = (g_difficulty >> 1) + g_cur_nation_rec->demands + 5;
    apply_tax_change("KINGTAX", delta);
    g_cur_nation_rec->demands++;
    if (human)
        sub_1048_4e81();
done:
    return;
}
