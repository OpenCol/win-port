/* unit_spend_tools -- shipped at 1028:a250.
 *
 * Ported by tools/port.py from matched/game/unit_spend_tools.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern NationRec52 far g_nation52[];                 SEG20:0x7840
 *   extern char g_msg[];                                 DGROUP:0x11c4
 *
 * The file's own Unit, NationRec52 dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: tools -> f15
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "control.h"
#include "jobs.h"
#include "units.h"
/* +1 is CONTROL: 0 human, 1 computer, 2 defeated */



extern void text_message_show(char *msg, col_i16 n);
#define NATION(u) ((unsigned char)((u).flags & 0xf))

void unit_spend_tools(col_i16 i)
{
    col_i16 n = i;

    g_units[n].f15 -= 20;
    if (g_units[n].f15 < 20) {
        g_units[n].f15 = 0;
        g_units[n].type = UNIT_COLONISTS;
        if ((col_i16)g_units[n].spec == JOB_MISSIONARY)
            g_units[n].type = UNIT_MISSIONARIES;
        if (NATION(g_units[n]) < 4 && g_nation52[NATION(g_units[n])].control == CONTROL_HUMAN)
            text_message_show(g_msg, 3);
    }
}
