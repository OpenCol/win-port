/* spec_job_name -- shipped at 1018:3bf5.
 *
 * Ported by tools/port.py from matched/game/spec_job_name.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern SpecRec far g_spec_recs[];                    SEG20:0xd61a
 *
 * The file's own SpecRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 fold_spec_1c(col_i16 spec);

/* 8 bytes at SEG20:0xd61a, see jobs.h */




col_i16 spec_job_name(col_i16 spec)
{
    return g_spec_recs[fold_spec_1c(spec)].name;
}
