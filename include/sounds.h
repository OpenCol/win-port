/* sounds.h -- the ids `play_sound_effect` (1040:89f2) takes: CD music and sound effects.
 *
 * THREE KINDS, and the dispatcher treats each differently (play_sound_effect.c):
 *   - SOUND_MUSIC_*, 0x20..0x3d: a CD audio track, gated by one on-switch (DGROUP `g_music_on`);
 *   - the "event" sounds 0x3c..0x3f and 0x8020..0x8024: a WAV that stops the CD first, gated by
 *     a second switch;
 *   - SOUND_* effects 0x40..0x5c: a WAV, gated by a third. SOUNDOPTIONS offers three checkboxes
 *     (Background Music, Event Music, Sound Effects); which global is which is not yet traced.
 * Ids 0 and 1 close the CD (SOUND_STOP_MUSIC); 2..5 and 0x3a are cases that do nothing, and
 * any id without a case (0x5b is pushed once, in SEG5) plays nothing at all.
 *
 * WHERE THE NAMES COME FROM, and tools/check_headers.py re-derives both:
 *   - an effect or event sound is named after the WAV file its case plays: the case for 0x58
 *     plays cockgun.wav, so it is SOUND_COCKGUN;
 *   - a music id is named after its jukebox menu entry: `jukebox_by_song` maps PICKMUSIC item k
 *     to an id, and the three submenus PICKINDEPENDENCE, PICKMILITARY and PICKINDIAN to
 *     `pick + base`. Ids the menus never reach are not named: 0x28, 0x34, 0x37 and 0x3d.
 * Where matched code already uses a name as a case label, the byte match pins its value.
 *     0x3d is what `closing_play` starts, CD track 27, the disc's last.
 *
 * THE FOUR ANTHEMS are 0x8020 + the nation (england, france, spain, dutch .wav), but the one
 * switch that plays them (SEG7:0x5373, by nation 0..3) gives the Netherlands 0x8022, Spain's.
 * SOUND_ANTHEM_NETHERLANDS is never pushed anywhere in the image.
 */
#ifndef SOUNDS_H
#define SOUNDS_H

#define SOUND_STOP_MUSIC                    1

/* ---- CD music, by jukebox entry ------------------------------------------------------- */
#define SOUND_MUSIC_BIRD_SONG               0x20
#define SOUND_MUSIC_SMOKY_TUNE              0x21
#define SOUND_MUSIC_CORNWALL                0x22
#define SOUND_MUSIC_SHADY_GROVE             0x23
#define SOUND_MUSIC_FIDDLERS_DANCE          0x24
#define SOUND_MUSIC_JINE_THE_CAVALRY        0x25
#define SOUND_MUSIC_JOE_CLARK               0x26
#define SOUND_MUSIC_LITTLE_FIDDLE           0x27
#define SOUND_MUSIC_LOVE_FOREVER            0x29    /* PICKINDEPENDENCE, pick + 0x28 */
#define SOUND_MUSIC_YORK_FUSILIERS          0x2a
#define SOUND_MUSIC_WASHINGTON_ARTILLERY_MARCH 0x2b
#define SOUND_MUSIC_ROAD_TO_BOSTON          0x2c
#define SOUND_MUSIC_INDEPENDENCE_WAY        0x2d
#define SOUND_MUSIC_THE_REVEILLE            0x2e    /* PICKMILITARY, pick + 0x2d */
#define SOUND_MUSIC_SUCCESSFUL_CAMPAIGN     0x2f
#define SOUND_MUSIC_MORELLIS_LESSON         0x30
#define SOUND_MUSIC_TO_ARMS                 0x31
#define SOUND_MUSIC_INDIAN_VICTORY          0x32    /* PICKINDIAN, pick + 0x31, skipping 0x34 */
#define SOUND_MUSIC_NATIVES                 0x33
#define SOUND_MUSIC_TENOCHTITLAN            0x35
#define SOUND_MUSIC_PIZARRO_AT_CUZCO        0x36
#define SOUND_MUSIC_BONNY_MORN              0x38
#define SOUND_MUSIC_HORNPIPE                0x39
#define SOUND_MUSIC_NIGHTINGALE             0x3b

/* ---- event sounds: a WAV that stops the CD first --------------------------------------- */
#define SOUND_GOLDCITY                      0x3c
#define SOUND_MEETKING                      0x3e
#define SOUND_1812                          0x3f
#define SOUND_ANTHEM_ENGLAND                0x8020  /* england.wav */
#define SOUND_ANTHEM_FRANCE                 0x8021  /* france.wav */
#define SOUND_ANTHEM_SPAIN                  0x8022  /* spain.wav */
#define SOUND_ANTHEM_NETHERLANDS            0x8023  /* dutch.wav */
#define SOUND_AMEN                          0x8024

/* ---- sound effects -------------------------------------------------------------------- */
#define SOUND_LILGUN                        0x40
#define SOUND_BIGGUN                        0x41
#define SOUND_CANNON0                       0x42
#define SOUND_CANNON1                       0x43
#define SOUND_BLTH20                        0x44
#define SOUND_BLTDIRT                       0x45
#define SOUND_LILEXPLO                      0x48
#define SOUND_BOOM2                         0x49
#define SOUND_BOOM3                         0x4a
#define SOUND_BIGEXPLO                      0x4b
#define SOUND_DRAGOONS                      0x4c
#define SOUND_BOARD                         0x4d
#define SOUND_WHOOP                         0x4e
#define SOUND_GUNWHOOP                      0x4f
#define SOUND_WHPHORSE                      0x50
#define SOUND_HORGNWHP                      0x51
#define SOUND_WGNSTOP                       0x52
#define SOUND_FIRE1                         0x53
#define SOUND_HAMMERS                       0x54
#define SOUND_ARROW                         0x55
#define SOUND_MUSPARTY                      0x56
#define SOUND_SHIPSINK                      0x57
#define SOUND_COCKGUN                       0x58
#define SOUND_HORSES                        0x5c

#endif
