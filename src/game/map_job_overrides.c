/* map_job_overrides -- shipped at 1020:3ed7.
 *
 * Ported by tools/port.py from matched/game/map_job_overrides.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern unsigned char far g_game_flags;               SEG20:0x7784
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

#include "gameflag.h"
#include "jobs.h"


col_i16 map_job_overrides(col_i16 job)
{
    col_i16 r = 0x15;

    if ((g_game_flags & GAMEFLAG_INDEPENDENCE) && job == JOB_SOLDIER)
        r = -1;
    if (job == JOB_CRIMINAL)
        r = 0x19;
    if (job == JOB_INDENTURED_SERVANT)
        r = 0x1c;
    if (job == JOB_CONVERT)
        r = 0x1b;
    return r;
}
