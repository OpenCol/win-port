/* job_unit_type -- shipped at 1018:4552.
 *
 * Ported by tools/port.py from matched/game/job_unit_type.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern signed char g_job_unit_type[];                DGROUP:0x0be5
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

col_i16 job_unit_type(col_i16 job) { return g_job_unit_type[job]; }
