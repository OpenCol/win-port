/* is_ordinary_job -- shipped at 1018:3915.
 *
 * Ported by tools/port.py from matched/game/is_ordinary_job.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "jobs.h"
int is_ordinary_job(int job)
{
 return job != JOB_COLONIST_ALT && job != JOB_COLONIST && job != JOB_INDENTURED_SERVANT
 && job != JOB_CRIMINAL && job != JOB_CONVERT;
}
