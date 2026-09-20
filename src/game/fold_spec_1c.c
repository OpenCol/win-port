/* fold_spec_1c -- shipped at 1018:3bd6.
 *
 * Ported by tools/port.py from matched/game/fold_spec_1c.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "jobs.h"
col_i16 fold_spec_1c(col_i16 spec)
{
    if (spec == JOB_COLONIST_ALT)
        spec = JOB_COLONIST;
    return spec;
}
