/* PORT: this file's own declarations of MainWin_ResetWindows removed: the header declares it from the definition (W1 step 3). */
/* begin_game: candidate for 1008:ca99 (358 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The opening menu, run as a `do` loop around one dialog. `show_text_dialog_1008_cdf3` on the
 * text at DGROUP:0x8d8 returns a choice and a `switch` over -1..6 acts on it: 1, 2 and 3 start
 * a game, 4 loads one, 5 goes to `ShowHallOfFame`, -1 and 6 ask the quit question at 0x8e2, and
 * anything else leaves at once. The loop runs until `more` is cleared.
 *
 * ALL THREE NEW-GAME CHOICES SHARE ONE ARM. Five words at SEG22:0 are seeded first -- 1 apiece
 * for choice 3, a 0..3 roll for the other two -- so choice 3 is "I will pick" and 1 and 2 are
 * "roll for me". Only choice 3 then opens `customize`, and only choice 2 passes 1 to
 * `start_new_game`.
 *
 * The palette is SAVED at the top and put back at the bottom, but only if `ok` survived: the
 * quit answer clears it, so leaving by the quit question skips the restore and leaves the
 * screen as the menu left it.
 *
 * `more = 1` is stored TWICE, back to back, before anything else. Both are in the shipped code.
 *
 * AN EMPTY `else`. The customize test compiles to `JZ join / JMP end / JMP join` with the last
 * a zero-displacement jump -- the `break` leaves and an else that compiles to nothing jumps to
 * the instruction after itself. `if (...) break;` with no else, and the `&&` form, are both two
 * bytes shorter. See the note under rule 54.
 *
 * `say_version` is reached through the five-byte NOP form and is extern; the three other 
 * callees have the four-byte form and are stubs (rule 47).
 *
 * RENAMED 2026-09-27 from start_new_game: String witness: BEGINMENU and DOS are in Mac begin_game(void), not in Mac start_new_game(short). Call graph: Mac begin_game calls sound_play, CopyRawColors, say_version, random, customize, start_new_game, LoadSaveGame, Fill and ResetWindows. ca99 calls sound_play, CTable_CopyRawColors, set_default_name_slots (=say_version, see 1008:cc49), rand_range, customize, new_game_setup (=start_new_game, see c44a), LoadSaveGame, CTable_Fill and MainWin_ResetWindows, in the same roles. Callers: Mac GameMain against Windows run_game. Arity: Mac begin_game is (void) and the reconstruction is int start_new_game(void). The Mac start_new_game takes a short and ca99 takes nothing. Size is 522 Mac against 358 Windows; the Mac version also does map-file selection. Why it went wrong: tools/mac_rename.py swapped the paraphrase start_game_menu for the Mac identifier that read closest ('BEGINMENU is the title screen; start_new_game is what it starts'). Its arity control passed 'windows reads >=0 bytes, mac block 2-2' as consistent, although ca99 reads none. Apply this only after 1008:2927 has been renamed.
 */

#include "colonization.h"

#include "sounds.h"
#include "classes.h"

extern MainWin g_main_win;             /* SEG20:0x5ca */

extern void sound_play(int n);
struct logpal { char pad[1]; };
extern void CTable_CopyRawColors(CTable *pal, unsigned char *dst, int first, int count);
extern void CTable_Fill(CTable *pal, int start, int count, unsigned char *rgb);

extern int customize(void);
extern void ShowHallOfFame(void);

/* same object: 4-byte `push cs; call ` */
extern void say_version(void);

extern int LoadSaveGame(void);

int begin_game(void)
{
    int ok;
    int choice;
    int r;
    char pal[0x300];
    register int i;
    register int more;

    ok = 1;
    more = 1;
    more = 1;
    g_loaded_flag = 0;
    sound_play(SOUND_MUSIC_NATIVES);
    CTable_CopyRawColors((CTable *)((struct logpal *)&g_game_palette), pal, 10, 0xec);
    do {
        say_version();
        choice = show_text_dialog_1008_cdf3("BEGINMENU");
        switch (choice) {
        case 1:
        case 2:
        case 3:
            for (i = 0; i < 5; i++)
                g_world_opt[i] = choice == 3 ? 1 : rand_range(0, 3);
            if (choice == 3) {
                if (customize() != 0)
                    break;
                else
                    ;
            }
            if (more != 0) {
                r = start_new_game(choice == 2 ? 1 : 0);
                if (r == 1)
                    more = 1;
                if (r > 1)
                    goto out;
            }
            break;
        case 4:
            more = LoadSaveGame();
            if (more == 0)
                g_loaded_flag = 1;
            break;
        case 5:
            ShowHallOfFame();
            break;
        case -1:
        case 6:
            more = show_text_dialog_1008_cdf3("DOS");
            if (more == 2) {
                ok = 0;
                more = 0;
            }
            break;
        default:
            goto out;
        }
    } while (more != 0);
extern out: if (ok != 0);
    return ok;
}
