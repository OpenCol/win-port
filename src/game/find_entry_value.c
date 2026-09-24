/* find_entry_value -- shipped at 1040:7470.
 *
 * Ported by tools/port.py from matched/game/find_entry_value.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern char far g_stub_s[];                          
 *
 * The file's own Entry dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: value -> value_a
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
Entry * find_entry(void *root, col_i16 which);

col_i16 find_entry_value(void *root, col_i16 which)
{
    Entry *e;

    e = find_entry(root, which);
    if (e == 0)
        return -1;
    return e->value_a;
}
