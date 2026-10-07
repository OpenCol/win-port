/* sound_init: candidate for 1040:8584 (84 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Registers LoadTray as the CD callback and reads the track count:
 * -1 (no drive) or 1 clears the CD flag at DGROUP:0x1d1c; any count other
 * than 27 clears it too and sets DGROUP:0x1d1e.
 *
 * RENAMED 2026-09-26 from cd_init: the Mac build's sound_init(void), bound by layout (between PlayRandomTrack and music_by_number on both sides); the arity agrees and the Mac body is 1.17x this one.
 */

#include "colonization.h"

extern void LoadTray(void);
extern void SetCDCallback(void *fn);

void sound_init(void)
{
    register int n;

    SetCDCallback((void *)LoadTray);
    n = MSGetCDTracks();
    if (n == -1) {
        g_cd_is_game_disc = 0;
        goto done;
    } else if (n == 1) {
        g_cd_is_game_disc = 0;
        goto done;
    }
    if (n != 0x1b) {
        g_cd_is_game_disc = 0;
        g_cd_wrong_disc = 1;
    } else
        ;
done:
    ;
}
