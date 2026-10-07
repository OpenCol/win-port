/* GameBrains_DoEndofTurn: candidate for 1008:29b4 (441 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * THE CALENDAR, AND THE END OF THE GAME. While a game is in progress the turn counter at
 * SEG20:0x7790 goes up by one -- the proof that it is a turn counter and not the treasury
 * (RE_STATUS 245) -- and so does the year at 0x778c, one a turn up to 1600. From 1600 the
 * season word at 0x778e alternates and the year moves on every SECOND turn; the switch is
 * announced once, by the `TIMECHANGE` text, on the first turn of 1600.
 *
 * Then, unless DGROUP:0x8a or 0x88 is set, `end_of_game_check` decides whether the
 * game is over, and if DGROUP:0xdc4 says it is, the closing sequence runs: the game palette's
 * colours 10..245 are saved into a 708-byte local, the score is computed and saved, the
 * closing screen, sound 3 and the Hall of Fame are shown, the whole map is revealed, and the
 * `SCORED` dialog asks. Answer 2 keeps playing -- the in-progress flag goes back to 1 and the
 * quit flag at DGROUP:0x4b04 stays clear -- and either way bit 4 of SEG20:0x7784 records that
 * the game has been scored once, and the palette is put back.
 *
 * The in-progress flag is cleared TWICE, before and after the closing screens; the shipped
 * stores 0 at both places and the second one is not dead code to the compiler.
 *
 * The two literals are checked by hand, not by the comparator: DGROUP:0x351 is `TIMECHANGE`
 * and 0x35c, eleven bytes on, `SCORED` (§119). `show_text_dialog_1008_cdf3`,
 * `end_of_game_check` and `MSDoAllTasks_1008_ce3f` come through the 4-byte `PUSH CS`
 * form and are stubs; `OpenBackdrop` and `CloseBackdrop` through the 5-byte one and are
 * extern (rule 47).
 *
 * RENAMED 2026-09-26 from advance_turn: the Mac build's GameBrains::DoEndofTurn(void): this only -- the calendar steps and the end-of-game checks run. Every function of the GameBrains module (1008:2927..3cc8) is called on the one object g_game_brains (SEG20:0x40d6), and the width matches.
 */

#include "colonization.h"

#include "classes.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

struct logpal { char pad[1]; };
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);
extern void OpenBackdrop(void);
extern void perform_save(int slot);
extern void DoTheClosing(void);
extern void force_mood(int mood);
extern void FinalScore(void);
extern void CloseBackdrop(void);

/* same object: 4-byte `push cs; call ` */

extern void end_of_game_check(void);

void GameBrains_DoEndofTurn(GameBrains *unused1)
{
    int r;
    char pal[0xec * 3];

    if (g_game.game_in_progress != 0) {
        g_game.turn_counter++;
        if (g_game.year < 1600) {
            g_game.year++;
        } else {
            if (g_game.year == 1600 && g_game.season == 0)
                show_text_dialog_1008_cdf3("TIMECHANGE");
            g_game.season++;
            if (g_game.season > 1) {
                g_game.season = 0;
                g_game.year++;
            }
        }
    }
    if (g_debug_mode == 0 && g_autoplay == 0 && g_game.game_in_progress != 0) {
        end_of_game_check();
        if (g_game_over != 0) {
            CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), pal, 10, 0xec);
            OpenBackdrop();
            perform_save(10);
            g_game.game_in_progress = 0;
            DoTheClosing();
            force_mood(3);
            FinalScore();
            g_game.game_in_progress = 0;
            g_game.reveal_all = 1;
            r = show_text_dialog_1008_cdf3("SCORED");
            if (r == 2)
                g_game.game_in_progress = 1;
            g_game.game_flags |= 0x10;
            g_game_over = 0;
            if (g_game.game_in_progress == 0)
                g_quit_flag = 1;
            else
                g_quit_flag = 0;
            CloseBackdrop();
            MSDoAllTasks_1008_ce3f();
            CTable_Fill(&g_game_palette, 10, 0xec, pal);
        }
    }
}
