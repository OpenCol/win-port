/* sound_play: candidate for 1040:89f2 (2914 bytes, 64 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The game's sound dispatcher: one `switch` over 66 sound ids. Ids 0x20..0x3d are CD music
 * tracks (stop the CD if the game disc is not the one playing, then a random track when the
 * player asked for that, else the numbered track); 0x3e, 0x3f, 0x3c and 0x8020..0x8024 are
 * sound effects that also stop the music; the rest are plain sound effects played by name.
 * Borland splits the sparse case set into three jump tables and a chain of compares; the
 * source lists the cases in the order the blocks appear.
 *
 * Sound names, from DS: meetking.wav, 1812.wav, musparty.wav, wgnstop.wav, hammers.wav, arrow.wav, board.wav, shipsink.wav, fire1.wav, cockgun.wav, amen.wav, goldcity.wav, england.wav, france.wav, dutch.wav, spain.wav, bltdirt.wav, blth20.wav, whoop.wav, gunwhoop.wav, whphorse.wav, horgnwhp.wav, lilgun.wav, biggun.wav, dragoons.wav, cannon0.wav, cannon1.wav, horses.wav, lilexplo.wav, boom2.wav, boom3.wav, bigexplo.wav
 *
 * RENAMED 2026-09-26 from play_sound_effect: the Mac build's sound_play(short), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller DoClosing; caller MainMenuProc; caller RefreshUpdatePort; caller colony_click_ship; caller resolve_raid), the runner-up gets 2, and the arity agrees (1). tools/mac_rename.py once refused this pairing because a disassembly scan measured >=251 argument bytes, a figure its own note called suspect; the byte-verified reconstruction takes one int.
 */

#include "colonization.h"

#include "sounds.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void PlayRandomTrack(void);
extern void PlaySnd(char *name);

void sound_play(unsigned id)
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
                PlayRandomTrack();
            else if (MSPlayCDTrack(9))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_SMOKY_TUNE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            else if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            if (MSPlayCDTrack(6))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_CORNWALL:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(13))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_SHADY_GROVE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(10))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_FIDDLERS_DANCE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(22))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_JINE_THE_CAVALRY:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(3))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_JOE_CLARK:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(25))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_LITTLE_FIDDLE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(19))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x28:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(7))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_LOVE_FOREVER:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(4))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_YORK_FUSILIERS:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(20))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_WASHINGTON_ARTILLERY_MARCH:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(23))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_ROAD_TO_BOSTON:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(14))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_INDEPENDENCE_WAY:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(4))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_THE_REVEILLE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(26))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_SUCCESSFUL_CAMPAIGN:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(11))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_MORELLIS_LESSON:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(16))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_TO_ARMS:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(21))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_INDIAN_VICTORY:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(8))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_NATIVES:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(5))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x34:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(2))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_TENOCHTITLAN:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(12))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_PIZARRO_AT_CUZCO:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(17))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x37:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(15))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_BONNY_MORN:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(4))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MUSIC_HORNPIPE:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
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
                PlayRandomTrack();
            else if (MSPlayCDTrack(18))
                g_cd_is_game_disc = 0;
        }
        break;
    case 0x3d:
        if (g_sound_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            if (g_cd_wrong_disc != 0)
                PlayRandomTrack();
            else if (MSPlayCDTrack(27))
                g_cd_is_game_disc = 0;
        }
        break;
    case SOUND_MEETKING:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("meetking.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_1812:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("1812.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_MUSPARTY:
        if (g_sound_effects)
            PlaySnd("musparty.wav");
        break;
    case SOUND_WGNSTOP:
        if (g_sound_effects)
            PlaySnd("wgnstop.wav");
        break;
    case SOUND_HAMMERS:
        if (g_sound_effects)
            PlaySnd("hammers.wav");
        break;
    case SOUND_ARROW:
        if (g_sound_effects)
            PlaySnd("arrow.wav");
        break;
    case SOUND_BOARD:
        if (g_sound_effects)
            PlaySnd("board.wav");
        break;
    case SOUND_SHIPSINK:
        if (g_sound_effects)
            PlaySnd("shipsink.wav");
        break;
    case SOUND_FIRE1:
        if (g_sound_effects)
            PlaySnd("fire1.wav");
        break;
    case SOUND_COCKGUN:
        if (g_sound_effects)
            PlaySnd("cockgun.wav");
        break;
    case SOUND_AMEN:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("amen.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_GOLDCITY:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("goldcity.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_ENGLAND:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("england.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_FRANCE:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("france.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_NETHERLANDS:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("dutch.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_ANTHEM_SPAIN:
        if (g_sound_event_music) {
            if (g_cd_is_game_disc == 0)
                MSCloseCD();
            PlaySnd("spain.wav");
            g_cd_is_game_disc = 1;
        }
        break;
    case SOUND_BLTDIRT:
        if (g_sound_effects)
            PlaySnd("bltdirt.wav");
        break;
    case SOUND_BLTH20:
        if (g_sound_effects)
            PlaySnd("blth20.wav");
        break;
    case SOUND_WHOOP:
        if (g_sound_effects)
            PlaySnd("whoop.wav");
        break;
    case SOUND_GUNWHOOP:
        if (g_sound_effects)
            PlaySnd("gunwhoop.wav");
        break;
    case SOUND_WHPHORSE:
        if (g_sound_effects)
            PlaySnd("whphorse.wav");
        break;
    case SOUND_HORGNWHP:
        if (g_sound_effects)
            PlaySnd("horgnwhp.wav");
        break;
    case SOUND_LILGUN:
        if (g_sound_effects)
            PlaySnd("lilgun.wav");
        break;
    case SOUND_BIGGUN:
        if (g_sound_effects)
            PlaySnd("biggun.wav");
        break;
    case SOUND_DRAGOONS:
        if (g_sound_effects)
            PlaySnd("dragoons.wav");
        break;
    case SOUND_CANNON0:
        if (g_sound_effects)
            PlaySnd("cannon0.wav");
        break;
    case SOUND_CANNON1:
        if (g_sound_effects)
            PlaySnd("cannon1.wav");
        break;
    case SOUND_HORSES:
        if (g_sound_effects)
            PlaySnd("horses.wav");
        break;
    case SOUND_LILEXPLO:
        if (g_sound_effects)
            PlaySnd("lilexplo.wav");
        break;
    case SOUND_BOOM2:
        if (g_sound_effects)
            PlaySnd("boom2.wav");
        break;
    case SOUND_BOOM3:
        if (g_sound_effects)
            PlaySnd("boom3.wav");
        break;
    case SOUND_BIGEXPLO:
        if (g_sound_effects)
            PlaySnd("bigexplo.wav");
        break;
    }
}
