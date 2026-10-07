/* LoadTray: candidate for 1040:8513 (20 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Raises the flag at DGROUP:0x1d1c that says the disc in the drive is the game's own.
 * `force_mood` sets the same flag inline when the mood changes, which is what identifies
 * this one-line function: it is that store on its own, for the two callers that need it
 * without the rest of the mood change -- `load_game_file` after a restore and
 * `game_menu_sound` when the sound settings are applied.
 *
 * RENAMED 2026-09-27 from cd_set_game_disc: Mac LoadTray(void). The Mac body is move.w #1,$31cc, one flag store, which is this function (g_cd_is_game_disc = 1). Callers: game_menu_sound and load_game(FSSpec*) (load_game_file) on both. Arity: void. The Mac name reads oddly for what the body does, but body, callers and position all bind it. Layout: Mac CODE 15 sort_8x8, sort_16x16, LoadTray, PlayRandomTrack, sound_init, soundQueue, music_by_number, check_music; Windows [three empties], 8513, PlayRandomTrack, sound_init, 85d8, music_by_number, check_music.
 */

#include "colonization.h"

void LoadTray(void)
{
    g_cd_is_game_disc = 1;
}
