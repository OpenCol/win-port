/* cd_play_random_track -- shipped at 1040:8527.
 *
 * Ported by tools/port.py from matched/game/cd_play_random_track.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_cd_is_game_disc;                        DGROUP:0x1d1c
 *   extern int g_cd_wrong_disc;                          DGROUP:0x1d1e
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 rand_range(col_i16 lo, col_i16 hi);

extern col_i16 MSGetCDTracks(void);
extern void MSPlayCDTrack(col_i16 track);
/* same object: 4-byte `push cs; call ` (rule 47) */




void cd_play_random_track(void)
{
    col_i16 n = MSGetCDTracks();

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
