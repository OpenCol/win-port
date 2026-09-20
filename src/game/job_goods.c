/* job_goods -- shipped at 1018:5d35, 39 bytes.
 *
 * The goods a job produces, or -1 for none: the signed byte table at SEG21:0x00cd.
 *
 * Ported from matched/game/job_goods.c. The per-file `extern signed char far
 * g_job_goods[];` is gone -- col/globals.h names the table once, and the table's CONTENTS
 * come from the shipped image rather than from a zeroed array, which is the whole point
 * of this case: nothing here would fail if the data were wrong, but the differential
 * would.
 */
#include "col/globals.h"

int job_goods(int job)
{
    int r = -1;

    r = g_job_goods[job];
    return r;
}
