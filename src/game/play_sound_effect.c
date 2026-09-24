/* play_sound_effect -- shipped at 1040:89f2.
 *
 * Ported by tools/port.py from matched/game/play_sound_effect.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_sound_event_music;                  DGROUP:0x00b8
 *   extern int far g_sound_music;                        DGROUP:0x00ba
 *   extern int far g_sound_effects;                      DGROUP:0x00bc
 *   extern int g_cd_is_game_disc;                        DGROUP:0x1d1c
 *   extern int g_cd_wrong_disc;                          DGROUP:0x1d1e
 *   extern char g_snd_meetking[];                        DGROUP:0x1e10
 *   extern char g_snd_1812[];                            DGROUP:0x1e1d
 *   extern char g_snd_musparty[];                        DGROUP:0x1e26
 *   extern char g_snd_wgnstop[];                         DGROUP:0x1e33
 *   extern char g_snd_hammers[];                         DGROUP:0x1e3f
 *   extern char g_snd_arrow[];                           DGROUP:0x1e4b
 *   extern char g_snd_board[];                           DGROUP:0x1e55
 *   extern char g_snd_shipsink[];                        DGROUP:0x1e5f
 *   extern char g_snd_fire1[];                           DGROUP:0x1e6c
 *   extern char g_snd_cockgun[];                         DGROUP:0x1e76
 *   extern char g_snd_amen[];                            DGROUP:0x1e82
 *   extern char g_snd_goldcity[];                        DGROUP:0x1e8b
 *   extern char g_snd_england[];                         DGROUP:0x1e98
 *   extern char g_snd_france[];                          DGROUP:0x1ea4
 *   extern char g_snd_dutch[];                           DGROUP:0x1eaf
 *   extern char g_snd_spain[];                           DGROUP:0x1eb9
 *   extern char g_snd_bltdirt[];                         DGROUP:0x1ec3
 *   extern char g_snd_blth20[];                          DGROUP:0x1ecf
 *   extern char g_snd_whoop[];                           DGROUP:0x1eda
 *   extern char g_snd_gunwhoop[];                        DGROUP:0x1ee4
 *   extern char g_snd_whphorse[];                        DGROUP:0x1ef1
 *   extern char g_snd_horgnwhp[];                        DGROUP:0x1efe
 *   extern char g_snd_lilgun[];                          DGROUP:0x1f0b
 *   extern char g_snd_biggun[];                          DGROUP:0x1f16
 *   extern char g_snd_dragoons[];                        DGROUP:0x1f21
 *   extern char g_snd_cannon0[];                         DGROUP:0x1f2e
 *   extern char g_snd_cannon1[];                         DGROUP:0x1f3a
 *   extern char g_snd_horses[];                          DGROUP:0x1f46
 *   extern char g_snd_lilexplo[];                        DGROUP:0x1f51
 *   extern char g_snd_boom2[];                           DGROUP:0x1f5e
 *   extern char g_snd_boom3[];                           DGROUP:0x1f68
 *   extern char g_snd_bigexplo[];                        DGROUP:0x1f72
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void cd_play_random_track(void);

#include "sounds.h"





extern void MSCloseCD(void);
extern col_i32 MSPlayCDTrack(col_i16 track);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void MSPlaySound_1068_a582(char *name);

































void play_sound_effect(col_u16 id)
{
    switch (id) {
    case 0x0: case SOUND_STOP_MUSIC:
        MSCloseCD();
        break;
    case 0x2: case 0x3:
        break;
    case 0x4: case 0x5:
        break;
    case SOUND_MUSIC_BIRD_SONG:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(9))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_SMOKY_TUNE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            else if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            if (MSPlayCDTrack(6))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_CORNWALL:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(13))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_SHADY_GROVE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(10))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_FIDDLERS_DANCE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(22))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_JINE_THE_CAVALRY:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(3))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_JOE_CLARK:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(25))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_LITTLE_FIDDLE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(19))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x28:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(7))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_LOVE_FOREVER:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(4))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_YORK_FUSILIERS:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(20))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_WASHINGTON_ARTILLERY_MARCH:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(23))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_ROAD_TO_BOSTON:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(14))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_INDEPENDENCE_WAY:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(4))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_THE_REVEILLE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(26))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_SUCCESSFUL_CAMPAIGN:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(11))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_MORELLIS_LESSON:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(16))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_TO_ARMS:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(21))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_INDIAN_VICTORY:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(8))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_NATIVES:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(5))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x34:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(2))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_TENOCHTITLAN:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(12))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_PIZARRO_AT_CUZCO:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(17))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x37:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(15))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_BONNY_MORN:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(4))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_HORNPIPE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(24))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x3a:
        break;
    case SOUND_MUSIC_NIGHTINGALE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(18))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x3d:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                cd_play_random_track();
            else if (MSPlayCDTrack(27))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MEETKING:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_meetking);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_1812:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_1812);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_MUSPARTY:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_musparty);
        break;
    case SOUND_WGNSTOP:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_wgnstop);
        break;
    case SOUND_HAMMERS:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_hammers);
        break;
    case SOUND_ARROW:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_arrow);
        break;
    case SOUND_BOARD:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_board);
        break;
    case SOUND_SHIPSINK:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_shipsink);
        break;
    case SOUND_FIRE1:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_fire1);
        break;
    case SOUND_COCKGUN:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_cockgun);
        break;
    case SOUND_AMEN:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_amen);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_GOLDCITY:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_goldcity);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_ENGLAND:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_england);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_FRANCE:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_france);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_NETHERLANDS:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_dutch);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_SPAIN:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            MSPlaySound_1068_a582(g_snd_spain);
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_BLTDIRT:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_bltdirt);
        break;
    case SOUND_BLTH20:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_blth20);
        break;
    case SOUND_WHOOP:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_whoop);
        break;
    case SOUND_GUNWHOOP:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_gunwhoop);
        break;
    case SOUND_WHPHORSE:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_whphorse);
        break;
    case SOUND_HORGNWHP:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_horgnwhp);
        break;
    case SOUND_LILGUN:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_lilgun);
        break;
    case SOUND_BIGGUN:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_biggun);
        break;
    case SOUND_DRAGOONS:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_dragoons);
        break;
    case SOUND_CANNON0:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_cannon0);
        break;
    case SOUND_CANNON1:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_cannon1);
        break;
    case SOUND_HORSES:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_horses);
        break;
    case SOUND_LILEXPLO:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_lilexplo);
        break;
    case SOUND_BOOM2:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_boom2);
        break;
    case SOUND_BOOM3:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_boom3);
        break;
    case SOUND_BIGEXPLO:
        if (g_sound_effects)
            MSPlaySound_1068_a582(g_snd_bigexplo);
        break;
    }
}
