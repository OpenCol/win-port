/* music_request_track -- shipped at 1040:88e1.
 *
 * Ported by tools/port.py from matched/game/music_request_track.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int g_pending_track;                          DGROUP:0x1d10
 *   extern int g_cur_track;                              DGROUP:0x1d12
 *   extern int g_music_change;                           DGROUP:0x1d1a
 *   extern int g_cd_is_game_disc;                        DGROUP:0x1d1c
 *   extern int far g_sound_event_music;                  DGROUP:0x00b8
 *   extern int far g_sound_music;                        DGROUP:0x00ba
 *   extern int g_stub_a;                                 
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void music_pick_track(void);

#include "sounds.h"







extern void play_sound_effect(col_i16 n);



void music_request_track(col_i16 t)
{
    if (t != g_cur_track) {
        g_pending_track = t;
        if (g_sound_event_music != 0 && g_sound_music == 0)
            g_music_change = 1;
        if (t >= 0)
            play_sound_effect(SOUND_STOP_MUSIC);
        g_cd_is_game_disc = 1;
    }
    music_pick_track();
}
