/* check_music: candidate for 1040:86ac (564 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * size-source: 564, not the census 548. The function runs to 0x88d1 and its seven-entry jump
 *   table follows at 0x88d3, inside the gap to `force_music` at 0x88e1. The shipped
 *   aligns the table with one padding byte the object does not need, because three -call
 *   conversions shorten the code ahead of it; 565 is therefore one byte more than the object
 *   produces and 564 is the largest size that compares.
 *
 * PICKS THE NEXT PIECE OF MUSIC. Nothing happens unless music is on (DGROUP:0xba) or a change
 * has been asked for (0x1d1a). The CD is tried first: no drive or one track turns the CD off,
 * any count other than 27 plays a random track once and gives up on it for good, and exactly
 * 27 tracks is the real disc and falls through to the chooser.
 *
 * A REQUESTED TRACK WINS. `force_music` leaves one at DGROUP:0x1d10 and it is taken
 * and cleared to -1. Otherwise a track is drawn at random from a (base, count) range, and the
 * range comes from three layers: a coin-flip between the two halves of the catalogue, weighted
 * by bit 0 of SEG20:0x7784; an override when DGROUP:0x8a is set; and then the current GROUP at
 * 0x1d16, a seven-way switch, which wins outright. Groups 5, 6 and 7 only override when the
 * current track is not already theirs.
 *
 * The draw repeats until it is not the track already playing, and the code it draws is mapped
 * through `music_by_number` -- so the loop tests the mapped value and the group is derived from the
 * unmapped one, by six descending `if`s rather than a chain.
 *
 * THE RANDOM SEQUENCE IS BRACKETED: `random_activate` is called with DGROUP:0x4b8a before
 * the draw and with 0x4b86 after, so the music does not consume the game's own random stream.
 *
 * THE GLOBAL NAMES HERE ARE MUSIC NAMES. `force_music` calls DGROUP:0x1d10 the
 * selected unit, 0x1d12 the locked unit and 0x1d1a a refresh request; 0x1d12 is the current
 * TRACK -- `game_menu_music` reads it to tick the music menu -- and this function shows the
 * other two are the requested track and a change request (§114).
 *
 * `rand_range`, `random_activate` and `music_by_number` come through the 4-byte `PUSH CS`
 * form and so are stubs here; `sound_play` is a real call even though it is in this
 * same segment (rule 47). The dead `JMP` after the first `goto` is the `else if`'s skip.
 *
 * RENAMED 2026-09-26 from music_pick_track: the Mac build's check_music(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (caller DoHuman; caller MouseMove; caller MouseMove; callee MSGetCDTracks; callee MSPlayCDTrack), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "gameflag.h"

#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern int g_next_mood;                   /* DGROUP:0x1d14 */
extern int g_mood;                        /* DGROUP:0x1d16 */

extern void sound_play(int n);

/* same object: 4-byte `push cs; call ` */

extern void random_activate(int seed);
extern int music_by_number(int x);

void check_music(void)
{
    int track;
    int code;
    int n;
    int count;
    int base;

    if (g_sound_music == 0 && g_music_change == 0)
        goto out;
    if (g_cd_is_game_disc == 0 && g_music_change == 0)
        goto out;
    g_music_change = 0;
    n = MSGetCDTracks();
    if (n == -1) {
        g_cd_is_game_disc = 0;
        goto out;
    } else if (n == 1) {
        g_cd_is_game_disc = 0;
        goto out;
    }
    if (n != 0x1b) {
        MSPlayCDTrack(rand_range(1, n));
        g_cd_is_game_disc = 0;
        g_cd_wrong_disc = 1;
        goto out;
    }
    if (g_pending_track >= 0) {
        track = g_pending_track;
        g_pending_track = -1;
    } else {
        random_activate(g_music_seed);
        if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
            base = 1;
            count = 0xc;
            if (rand_range(0, 8) == 0) {
                base = 0xd;
                count = 0xb;
            }
        } else {
            base = 0xd;
            count = 6;
            if (rand_range(0, 4) == 0) {
                base = 1;
                count = 0xc;
            }
        }
        if (g_debug_mode != 0) {
            base = 1;
            count = 0x18;
        }
        switch (g_mood) {
        case 1:
            base = 1;
            count = 7;
            break;
        case 2:
            base = 8;
            count = 5;
            break;
        case 3:
            base = 0xd;
            count = 6;
            break;
        case 4:
            base = 0x13;
            count = 4;
            break;
        case 5:
            if (g_cur_track != 0x33) {
                base = 0x17;
                count = 1;
            }
            break;
        case 6:
            if (g_cur_track != 0x35) {
                base = 0x19;
                count = 1;
            }
            break;
        case 7:
            if (g_cur_track != 0x36) {
                base = 0x1a;
                count = 1;
            }
            break;
        }
        do {
            code = rand_range(0, count - 1) + base;
            track = music_by_number(code);
        } while (track == g_cur_track);
        random_activate(g_random_seed);
        if (g_mood == 0) {
            g_mood = 7;
            if (code <= 0x19)
                g_mood = 6;
            if (code <= 0x18)
                g_mood = 5;
            if (code <= 0x16)
                g_mood = 4;
            if (code <= 0x12)
                g_mood = 3;
            if (code <= 0xc)
                g_mood = 2;
            if (code <= 6)
                g_mood = 1;
        }
        g_prev_group = g_mood;
        g_mood = g_next_mood;
        g_next_mood = 0;
    }
    g_cur_track = track;
    sound_play(track);
out:
    ;
}
