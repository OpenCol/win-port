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
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DlgControl; class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class MenuClass { public: short hmenu; MenuClass(); ~MenuClass(); };
class MapWin { public: char body[0x816 - 0x8e]; MapWin(); ~MapWin(); };
class MiniWin { public: char body[0xf9c - 0x816]; MiniWin(); ~MiniWin(); };
class StatWin { public: char body[0x173c - 0xf9c]; StatWin(); ~StatWin(); };
class ReportWin { public: char body[0x1ff4 - 0x173c]; ReportWin(); ~ReportWin(); };
class ColonyWin { public: char body[0x2a08 - 0x1ff4]; ColonyWin(); ~ColonyWin(); };
class EuropeWin { public: char body[0x32b2 - 0x2a08]; EuropeWin(); ~EuropeWin(); };
class PediaWin { public: char body[0x3b0c - 0x32b2]; PediaWin(); ~PediaWin(); };
class MainWin : public EventWin { public: MenuClass menu; MapWin map; MiniWin mini; StatWin stat; ReportWin report; ColonyWin colony; EuropeWin europe; PediaWin pedia; MainWin(); ~MainWin(); };
#include "gameflag.h"
#include "records.h"
extern GameState g_game;   /* SEG20:0x7782 */

typedef struct { char port[0x26]; char dialog[0x9a]; void *relayout; } GameWin;   /* GameWin : GRPort, DialogWin -- the two bases as their bytes, 0xc4 */
extern void GameWin_Redraw(GameWin *w);
extern void MiniWin_ChangeScale(void *w, int scale);
extern void PediaWin_Done(PediaWin *w);

/* same object: 4-byte `push cs; call ` */
extern "C" void win_show_and_focus_1048_d8f7(DialogWin *w);
extern "C" void StartCheat(void);

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
