/* MapWin_RedrawMap: candidate for 1038:a76e (219 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_RedrawMap
 * name-source: sibling-build
 *
 * Rebuilds the map after a zoom change: the current window's owner (the map window) has its
 * off-screen port (data segment 28) resized to the window rectangle and its palette
 * re-selected, the global sprite scale set to (zoom + 8, 8) (SetGlobalSpriteScale), the map redrawn (DrawMap), the
 * mini window at SEG20:0xde0 redrawn (GameWin_Redraw) and the sprite scale reset to (1, 1). The module's rectangle
 * helpers are defined here because the shipped calls to them are the compiler's own
 * `push cs / call `.
 *
 * RENAMED 2026-09-27 from map_zoom_redraw: the Mac build's MapWin_RedrawMap, bound by tools/mac_bind.py's proposal and checked against the body and the Mac call graph: current_window's MapWin, then GRPort_ColorSync, SetGlobalSpriteScale, DrawMap(1), GameWin_Redraw, SetGlobalSpriteScale(1,1): the Mac RedrawMap sequence (Windows adds a ResizePort); a handler with no this; ratio 2.29.
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

typedef struct { char pal[0x404]; unsigned hpal; short seed; short steps, first, count; short steps2, first2, count2; unsigned char r, g, b; char f417; short hsrc_a, hsrc_b, hsrc; } CTable;   /* 0x41e bytes: the palette, its handle and the cycling state */

extern char g_mini_win;  /* SEG20:0xde0 */
extern void *GetEventWindow(void);

extern int GRPort_ResizePort(GRPort *port, RECT *r);
extern void GRPort_ColorSync(GRPort *port, CTable *pal);
extern void SetGlobalSpriteScale(int num, int den);
extern void DrawMap(int flag);
extern void GameWin_Redraw(GameWin *rec);

void MapWin_RedrawMap(void)
{
    MapWin *o;
    RECT rc;
    int zoom;
    int eight;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (MapWin *)w;
    zoom = o->zoom + 8;
    eight = 8;
    SetRect(&rc, 0, 0, rect_width_1038_dfbd(&o->rect), rect_height_1038_dfdc(&o->rect));
    GRPort_ResizePort(&g_map_port, &rc);
    GRPort_ColorSync(&g_map_port, &g_game_palette);
    SetGlobalSpriteScale(zoom, eight);
    DrawMap(1);
    GameWin_Redraw((GameWin *)&g_mini_win);
    SetGlobalSpriteScale(1, 1);
}
