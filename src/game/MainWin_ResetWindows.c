/* PORT: this file's own declarations of MiniWin_ChangeScale removed: the header declares it from the definition (W1 step 3). */
/* MainWin_ResetWindows: candidate for 1048:a896 (195 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MainWin_ResetWindows
 * name-source: sibling-build
 *
 * What `load_game`, `load_game_from_dialog` and `begin_game` call, each passing
 * the game record at SEG20:0x5ca: put every window of the main screen back into its
 * start-of-game state.
 *
 * The map child at +0x8e is recentred on the cursor (DGROUP:0x4b30/0x4b32) and relaid out;
 * so are the children at +0xf9c and +0x816, the second after `MiniWin_ChangeScale(.., 3)`; the
 * one at +0x32b2 goes through `PediaWin_Done`; the report window at +0x173c and the dialog
 * at +0xb4 are closed.
 *
 * Then bit 0x2000 of the flag word at SEG20:0x7784 is TOGGLED -- not set, not cleared -- and
 * `StartCheat` runs, which builds the cheat menu when the same bit is clear and deletes
 * it (menu 7) when it is set, and flips the bit again in both arms. So the two are a pair and the xor is deliberate; what the bit
 * means between games, when two loads in a row would leave it back where it started, is not
 * established here.
 *
 * `win_show_and_focus_1048_d8f7` and `StartCheat` are -called and are stubs (rule 47);
 * everything else is a call and `extern`.
 *
 * RENAMED 2026-09-26 from reset_windows_for_new_game: the Mac build's MainWin::ResetWindows(void): this only -- the object at SEG20:0x5ca whose children are the seven windows MainWin_ctor builds -- putting every window of the main screen back. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, MenuClass, MapWin, MiniWin, StatWin, ReportWin, ColonyWin, EuropeWin, GameWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-


class DlgControl;







class PediaWin { public: char body[0x3b0c - 0x32b2]; PediaWin(); ~PediaWin(); };
class MainWin : public EventWin { public: MenuClass menu; MapWin map; MiniWin mini; StatWin stat; ReportWin report; ColonyWin colony; EuropeWin europe; PediaWin pedia; MainWin(); ~MainWin(); };
#include "gameflag.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
extern void GameWin_Redraw(GameWin *w);
extern "C" void PediaWin_Done(PediaWin *w);   /* PORT: C linkage, as defined (W2) */

/* same object: 4-byte `push cs; call ` */
extern "C" void win_show_and_focus_1048_d8f7(DialogWin *w);
extern "C" void StartCheat(void);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void MainWin_ResetWindows(MainWin *o)
{
    MapWin_Recenter(&o->map, g_cursor_x, g_cursor_y);
    GameWin_Redraw((GameWin *)&o->map);
    GameWin_Redraw((GameWin *)&o->stat);
    MiniWin_ChangeScale(&o->mini, 3);
    GameWin_Redraw((GameWin *)&o->mini);
    PediaWin_Done(&o->pedia);
    ReportWin_Done(&o->report);
    win_show_and_focus_1048_d8f7((DialogWin *)((char *)&o->map + 0x26));
    g_game.game_flags ^= GAMEFLAG_CHEAT_MENU;
    StartCheat();
}
