/* say_unit_job -- shipped at 1058:9371.
 *
 * Ported by tools/port.py from matched/game/say_unit_job.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern Unit far g_units[];                           SEG20:0xa208
 *   extern SpecRec far g_spec_recs[];                    SEG20:0xd61a
 *   extern int far g_misc_words[];                       SEG20:0xdcc8
 *
 * The file's own Unit, SpecRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "units.h"
/* 0x1c bytes at SEG20:0xa208 */

/* 8 bytes to the row at SEG20:0xd61a */


extern col_i16 is_ordinary_job(col_i16 job);
extern void say_string(char *buf, col_i16 word);

void say_unit_job(void *w, col_i16 u, col_i16 flag)
{
    col_i16 type;
    register col_i16 job;
    register col_i16 id;

    job = g_units[u].spec;
    type = g_units[u].type;
    if (job == 0x1c)
        job = 0x13;
    id = g_spec_recs[job].name;
    if (type != UNIT_COLONISTS && flag == 0 && is_ordinary_job(job) == 0)
        id = -1;
    if ((type == UNIT_SOLDIERS || type == UNIT_DRAGOONS) && job == 0x15)
        id = g_misc_words[65];
    if (type == UNIT_SCOUTS && job == 0x16)
        id = g_misc_words[4];
    if (type == UNIT_MISSIONARIES && job == 0x18)
        id = g_misc_words[4];
    if (id >= 0)
        say_string(w, id);
}
