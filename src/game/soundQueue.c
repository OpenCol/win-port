/* soundQueue: candidate for 1040:85d8 (17 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Returns the game-disc flag at DGROUP:0x1d1c, the word `LoadTray` at 1040:8513
 * raises. The two are the usual pair and three bytes apart in length for the usual reason:
 * this one fetches no argument.
 *
 * RENAMED 2026-09-27 from cd_get_game_disc: Mac soundQueue(void). The Mac body is move.w $31cc,d0, returning the flag LoadTray sets, which is this function (return g_cd_is_game_disc). Caller: game_menu_sound on both. Arity: void. Layout: Mac CODE 15 sort_8x8, sort_16x16, LoadTray, PlayRandomTrack, sound_init, soundQueue, music_by_number, check_music; Windows [three empties], 8513, PlayRandomTrack, sound_init, 85d8, music_by_number, check_music.
 */

#include "colonization.h"

int soundQueue(void)
{
    return g_cd_is_game_disc;
}
