/* GameBrains_DoViceroy: candidate for 1008:2be6 (246 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * Hands the turn to nation `n`, and the byte at SEG20:0x7841 + n * 0x34 decides what that
 * means: 1 is a nation the game plays out here and then, 0 is one that is handed back to the
 * caller through the three fields of the record it was passed, and 2 gets neither.
 *
 * DGROUP:0x8b gates the whole thing twice over. While it is set the turn only ever moves
 * FORWARD -- a request for a nation behind the current one returns at once -- and it also
 * suppresses both `new_turn_bookkeeping` and the autosave. Clear, everything runs.
 *
 * The relayout of the view record at SEG20:0x1566 happens when the view is already on this
 * nation OR when everything is revealed, so a spectator sees each nation's turn and a player
 * sees only their own.
 *
 * The autosave needs three things at once: the gate clear, the nation equal to the one at
 * SEG20:0x779a, and bit 2 of the flags at SEG20:0x7785.
 *
 * The `f0 == 0` arm writes 1, 0x64 and `n` into +6, +4 and +8 of the caller's record and
 * returns -- a request queued rather than carried out.
 *
 * RENAMED 2026-09-26 from begin_nation_turn: the Mac build's GameBrains::DoViceroy(short): this plus a nation, handing the turn to that nation. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "gameflag.h"
#include "control.h"
#include "records.h"

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;         /* SEG20:0x1566 */
extern void new_turn_bookkeeping(void);
#include "classes.h"
extern void GameWin_Redraw(GameWin *w);
extern void foreign_turn(int nation);

/* same object: 4-byte `push cs; call ` */
extern void autosave(void);

void GameBrains_DoViceroy(GameBrains *brains, register int n)
{
    if (g_loaded_flag != 0 && g_game.turn_nation > n)
        goto out;
    g_game.turn_nation = n;
    if (g_loaded_flag == 0 && g_nation_text[n].control != CONTROL_DEFEATED)
        new_turn_bookkeeping();
    if (g_nation_text[n].control == CONTROL_AI) {
        if (g_game.view_nation == n || g_game.reveal_all != 0)
            GameWin_Redraw((GameWin *)&g_stat_win);
        if (g_loaded_flag == 0 && g_game.player_nation == n && (HI_BYTE(g_game.game_flags) & GAMEFLAG_HI_AUTOSAVE))
            autosave();
        foreign_turn(n);
    } else if (g_nation_text[n].control == CONTROL_HUMAN) {
        brains->awaiting_human = 1;
        brains->phase = 0x64;
        brains->nation = n;
    }
out:
    ;
}
