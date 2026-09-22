/* ctl_text_width_plus_8 -- shipped at 1050:0b5b.
 *
 * Ported by tools/port.py from matched/game/ctl_text_width_plus_8.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 * Members renamed to the merged record's spelling, by OFFSET: text -> value
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ctl_line_height(char *s);

/* same object: 4-byte `push cs; call ` (rule 47) */

col_i16 ctl_text_width_plus_8(Ctl *c)
{
    col_i16 r;

    r = ctl_line_height(c->value) + 8;
    return r;
}
