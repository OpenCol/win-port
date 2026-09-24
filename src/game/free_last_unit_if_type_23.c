/* free_last_unit_if_type_23 -- shipped at 1020:8ddd.
 *
 * Ported by tools/port.py from matched/game/free_last_unit_if_type_23.c.
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

extern void unit_record_free(col_i16 u);

void free_last_unit_if_type_23(void)
{
    if ((g_units - 1)[g_unit_count].type == 0x17)
        unit_record_free(g_unit_count - 1);
}
