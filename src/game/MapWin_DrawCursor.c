/* MapWin_DrawCursor: candidate for 1038:aae0 (166 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_DrawCursor
 * name-source: sibling-build
 *
 * Draws the tile cursor sprite (SEG20:0x64a6) at map tile (x, y): tile
 * size from MapWin_TileSize, the view origin at DGROUP:0x4c44/0x4c46, the
 * sprite scale set to (zoom + 8) by 8 through SetGlobalSpriteScale and back
 * to 1 by 1.  The two MapWin helpers
 * are -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from map_draw_cursor: the Mac build's MapWin::DrawCursor(short, short): this plus a map tile, drawing the tile cursor there. The width matches.
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

typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "icons.h"

extern int g_stub_a;
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern "C" int MapWin_TileSize(MapWin *w);
extern "C" int MapWin_GetZoom(MapWin *mw);

void MapWin_DrawCursor(MapWin *w, int x, int y)
{
    register int ts;
    int dx;
    int dy;
    int bank;
    RECT r;

    ts = MapWin_TileSize(w);
    dx = (x - g_view_x0) * ts;
    dy = (y - g_view_y0) * ts;
    bank = MapWin_GetZoom(w) + 8;
    SetGlobalSpriteScale(bank, 8);
    Sprite_Draw(&r, &g_icon_sprites[ICON_CURSOR], (GRPort *)w, w->rect.left + dx, w->rect.top + dy);
    SetGlobalSpriteScale(1, 1);
}
