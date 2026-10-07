/* MapWin_LeftMouseClick: candidate for 1038:9371 (411 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_LeftMouseClick
 * name-source: sibling-build
 *
 * A left click on the map window, re-entry flag at DGROUP:0x1c50 -- the third of the four
 * 0x1c5x flags, beside `MapWin_LeftMouseUp`'s 0x1c57 and `MapWin_RightMouseClick`'s 0x1c52.
 * Outside the map rectangle the window is closed; inside, the drag state at +0x784 decides.
 *
 * -1, meaning no drag, is the plain click: the colony under the pointer opens when it is the
 * player's or god mode is on, and otherwise the map recentres on the square. In view mode the
 * cursor follows and the view record is laid out again. A drag of 0 does NOTHING, and any
 * other value is a direction handed to `GameBrains_MoveUnit` as `drag - 1` -- the same
 * three cases `MapWin_LeftMouseUp` has, with the unit-activation arm left out.
 *
 * `else if (o->drag == 0) { }` IS AN EMPTY BRANCH IN THE SHIPPED BYTES: the test jumps INTO
 * the last arm and then over it, which is rule 54's empty-`else` shape. Dropping the empty
 * branch and writing `else if (o->drag != 0)` gives one jump past the body instead of two.
 *
 * The nation comparison is done in BYTES -- `CMP AL,ES:[0x7798]`, no widening either side --
 * so both `Colony+0x1a` and the word at SEG20:0x7798 are read through `char` declarations here,
 * as they are in `MapWin_LeftMouseUp`.
 *
 * `win_show_and_focus_1038_e279` and `MSDoAllTasks_1038_e2a2` come through the four-byte `PUSH CS`
 * form and are stubs here (rule 47). `MapWin_RightMouseClick` declares the same two `extern`
 * over the same four-byte sites and still grades RELOCATABLE, because the comparator will
 * collapse a five-byte call to the four-byte form -- but it then has to forgive every jump
 * displacement after it as "one out, same destination". The stub form reproduces them exactly,
 * which is the reason to prefer it.
 */

#include "colonization.h"

#include "w16types.h"
class DlgControl { public: short kind; short id; void *win; RECT rect; short hwnd; DlgControl *next; char initial_focus; char hotkey; short hotkey_pos; DlgControl(); ~DlgControl(); };
class SButton : public DlgControl { public: short enabled; long action; short colour; SButton(); ~SButton(); };
#pragma option -a
class Screen { public: short style; void *ctable; void *win; short fa; char disabled; Screen(); ~Screen(); };
#pragma option -a-
class EventTable { public: void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; EventTable(); ~EventTable(); };
class EventWin : public Screen, public EventTable { public: char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; EventWin(); ~EventWin(); };
class DialogWin : public EventWin { public: DlgControl *first_ctl; DlgControl *ok_btn; DlgControl *cancel_btn; char keyboard_nav; char f99; DialogWin(); ~DialogWin(); };
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
class GameWin : public GRPort, public DialogWin { public: void *relayout; GameWin(); ~GameWin(); virtual int ResizePort(RECT *r); };
class SImageButton : public DlgControl { public: long on_click; long on_rclick; GRPort up; GRPort down; void *ctable; char reinit_faces; char pad_73; SImageButton(); ~SImageButton(); };
class ColChild : public GameWin { public: char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; SImageButton btn; unsigned char active; unsigned char kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; ColChild(); ~ColChild(); };
class MapWin : public ColChild { public: short cx; short cy; char pad_77e[4]; short zoom; short drag; short lbutton_held; MapWin(); ~MapWin(); };
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */

typedef struct { char pad[1]; } ColonyWin;

extern GameBrains g_game_brains;            /* SEG20:0x40d6 */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;           /* SEG20:0x1566 */
extern void *GetEventWindow(void);
extern int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);

extern void ColonyWin_Show(void *w, int colony);
extern void MapWin_Reposition(MapWin *o, int x, int y);

extern void GameWin_Redraw(GameWin *rec);
extern void GameBrains_MoveUnit(void *rec, int dir);

/* same object: 4-byte `push cs; call ` */
extern "C" void win_show_and_focus_1038_e279(void *w);

void MapWin_LeftMouseClick(int x, int y)
{
    MapWin *o;
    int mx;
    int my;
    int colony;
    EventWin *w;

    if (g_map_lclick_busy != 0)
        return;
    g_map_lclick_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (MapWin *)w;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y) {
        win_show_and_focus_1038_e279(&(DialogWin &)*o);
    } else {
        x -= o->rect.left;
        y -= o->rect.top;
        if (o->drag == -1) {
            MapWin_WinPos2MapPos(o, x, y, &mx, &my);
            colony = find_colony(mx, my);
            if (colony >= 0 && ((char)g_colonies[colony].nation == ((char)g_game.view_nation) || g_game.reveal_all != 0))
                ColonyWin_Show(&g_colony_window, colony);
            else
                MapWin_Reposition(o, x, y);
            if (g_game.view_mode == 1) {
                set_cursor_pos(mx, my);
                GameWin_Redraw((GameWin *)&g_stat_win);
            }
        } else if (o->drag == 0) {
        } else {
            if (g_game.view_mode != 1)
                GameBrains_MoveUnit(&g_game_brains, o->drag - 1);
        }
    }
    MSDoAllTasks_1038_e2a2();
    g_map_lclick_busy = 0;
}
