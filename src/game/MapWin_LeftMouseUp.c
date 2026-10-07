/* MapWin_LeftMouseUp: candidate for 1038:97e7 (850 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_LeftMouseUp
 *
 * Mouse-up on the map window (re-entry flag at DGROUP:0x1c57). Outside the map rectangle the
 * window is merely selected. Inside, the click is converted to a map square and the drag
 * state (+0x784) decides: -1 or 0 (a plain click) opens the colony there when it is the
 * player's (or in god mode), else activates the player's unit there, else recentres the map,
 * and in view mode also refreshes the viewed square; -2 (the hand cursor) resets the cursor
 * and moves the selected unit to the square (not in view mode); any other value is a drag
 * direction, handed to the unit as an order. Two same-module wrappers are defined here because
 * the shipped calls to them are the compiler's own `push cs / call `; top_unit_in_square is
 * a call TLINK converted.
 */

#include "colonization.h"

#include "units.h"
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

extern GameBrains g_game_brains;    /* SEG20:0x40d6 */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;   /* SEG20:0x1566 */
extern void *GetEventWindow(void);
extern void MSSetFocus_of_1038_e53e(DialogWin *w);
extern void DialogWin_SetDefaultControlFocus(DialogWin *w);
extern void MSSetCursor_id(MSWin *win, int cursor);
extern int MapWin_WinPos2MapPos(MapWin *o, int x, int y, int *mx, int *my);

extern int top_unit_in_square(int x, int y);
extern void ColonyWin_Show(void *w, int colony);
extern void GameBrains_UnitActivate(void *rec, int x, int y);
extern void MapWin_Reposition(MapWin *o, int x, int y);

extern void GameWin_Redraw(GameWin *rec);
extern void GameBrains_UnitGotoXY(void *rec, int x, int y);
extern void GameBrains_MoveUnit(void *rec, int dir);

extern "C" void win_show_and_focus_1038_e279(DialogWin *w);

extern "C" void win_load_cursor(DialogWin *w, int cursor);

void MapWin_LeftMouseUp(int x, int y)
{
    MapWin *o;
    int mx, my;
    int colony;
    int u;
    EventWin *w;

    if (g_mouse_up_busy != 0)
        return;
    g_mouse_up_busy = 1;
    w = (EventWin *)GetEventWindow();
    o = (MapWin *)w;
    if (o->rect.left > x || o->rect.right < x || o->rect.top > y || o->rect.bottom < y)
        win_show_and_focus_1038_e279(&(DialogWin &)*o);
    else {
        x -= o->rect.left;
        y -= o->rect.top;
        MapWin_WinPos2MapPos(o, x, y, &mx, &my);
        o->lbutton_held = 0;
        if (o->drag == -1) {
            MapWin_WinPos2MapPos(o, x, y, &mx, &my);
            colony = find_colony(mx, my);
            u = top_unit_in_square(mx, my);
            colony = find_colony(mx, my);
            if (colony >= 0 && ((char)g_colonies[colony].nation == ((char)g_game.view_nation) || g_game.reveal_all != 0))
                ColonyWin_Show(&g_colony_window, colony);
            else if (u >= 0 && (char)(g_units[u].flags & UNIT_NATION_MASK) == ((char)g_game.view_nation))
                GameBrains_UnitActivate(&g_game_brains, mx, my);
            else
                MapWin_Reposition(o, x, y);
            if (g_game.view_mode == 1) {
                set_cursor_pos(mx, my);
                GameWin_Redraw((GameWin *)&g_stat_win);
            }
        } else if (o->drag == -2) {
            g_cursor_state = 0;
            win_load_cursor(&(DialogWin &)*o, -1);
            if (g_game.view_mode != 1)
                GameBrains_UnitGotoXY(&g_game_brains, mx, my);
        } else if (o->drag == 0) {
            MapWin_WinPos2MapPos(o, x, y, &mx, &my);
            u = top_unit_in_square(mx, my);
            colony = find_colony(mx, my);
            if (colony >= 0 && ((char)g_colonies[colony].nation == ((char)g_game.view_nation) || g_game.reveal_all != 0))
                ColonyWin_Show(&g_colony_window, colony);
            else if (u >= 0 && (char)(g_units[u].flags & UNIT_NATION_MASK) == ((char)g_game.view_nation))
                GameBrains_UnitActivate(&g_game_brains, mx, my);
            else
                MapWin_Reposition(o, x, y);
            if (g_game.view_mode == 1) {
                set_cursor_pos(mx, my);
                GameWin_Redraw((GameWin *)&g_stat_win);
            }
        } else {
            if (g_game.view_mode != 1)
                GameBrains_MoveUnit(&g_game_brains, o->drag - 1);
        }
    }
    g_mouse_up_busy = 0;
}
