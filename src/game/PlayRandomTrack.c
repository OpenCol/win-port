/* PlayRandomTrack: candidate for 1040:8527 (93 bytes, 28 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Plays a random CD track when a disc with more than one track and other than 27 is in:
 * no disc or one track clears the flag, 27 tracks (the game CD itself) sets it.
 *
 * RENAMED 2026-09-26 from cd_play_random_track: the Mac build's PlayRandomTrack(void), bound by call-sequence alignment (28.0 aligned gaps in sound_play leave it as the lone unbound callee, none against); the arity agrees and the Mac body is 1.33x this one.
 */

#include "colonization.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

void PlayRandomTrack(void)
{
    int n = MSGetCDTracks();

    if (n == -1)
        g_cd_is_game_disc = 0;
    else if (n == 1)
        g_cd_is_game_disc = 0;
    else if (n != 0x1b) {
        MSPlayCDTrack(rand_range(1, n));
        g_cd_is_game_disc = 0;
        g_cd_wrong_disc = 1;
    } else
        g_cd_is_game_disc = 1;
}
