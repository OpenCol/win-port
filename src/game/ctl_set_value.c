/* ctl_set_value -- shipped at 1050:0b91.
 *
 * Ported by tools/port.py from matched/game/ctl_set_value.c.
 *
 * The file's own Ctl dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 ctl_clamped_width(Ctl *c);

/* same object: 4-byte `push cs; call ` (rule 47) */

void ctl_set_value(Ctl *c, col_i32 v)
{
    c->value = v;
    c->hctl = ctl_clamped_width(c);
}
