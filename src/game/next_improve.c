/* next_improve: candidate for 1020:3ed7 (71 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Maps a job onto the picture it should use, as a default followed by four
 * unconditional overrides tested in source order: 0x15 becomes -1 when bit 0
 * of SEG20:0x7784 is set, 0x1a becomes 0x19, 0x19 becomes 0x1c, and 0x1b
 * stays 0x1b.  The compiler keeps all four tests because each is a separate
 * statement, not an else-if chain.
 *
 * RENAMED 2026-09-26 from map_job_overrides: the Mac build's next_improve(short), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between resolve_raid and try_to_improve, which are already bound; the arity agrees and the Mac body is 1.13x this one.
 */

#include "colonization.h"

#include "gameflag.h"
#include "jobs.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

int next_improve(int job)
{
    int r = 0x15;

    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) && job == JOB_SOLDIER)
        r = -1;
    if (job == JOB_CRIMINAL)
        r = 0x19;
    if (job == JOB_INDENTURED_SERVANT)
        r = 0x1c;
    if (job == JOB_CONVERT)
        r = 0x1b;
    return r;
}
