/* MapWin_PieceIconDraw: candidate for 1038:a99f (321 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: MapWin_PieceIconDraw
 * name-source: sibling-build
 *
 * Draws a unit sprite's marks at tile pixel (x, y) with the sprite bank
 * switched to zoom + 8: flag 8 a coloured copy two pixels right, flag 1
 * one two pixels left (colour 0x5f with flag 4, else the byte at
 * DGROUP:0x337e), flag 2 the sprite itself; the bank goes back to 1.
 *
 * RENAMED 2026-09-27 from draw_unit_marks: CONST PROPOSAL, SIBLING RESOLVED. The witness points both draw_marker (a849) and draw_unit_marks (a99f) at MapWin::PieceIconDraw(Sprite&, short, short, short), CODE 23 0x1318, 300 bytes. The constants tie (the same 4 values and overlap 1.0 for both), so they cannot decide. Arity does: the Mac method takes a Sprite&, a99f takes (MapWin *w, void *sprite, int x, int y, int flags), and a849 takes an int sprite INDEX. Call chain: the Mac method's only caller is the free thunk PieceIconDraw(Sprite&, s, s, s) (CODE 22 0x1d2c, 66 bytes), which is called 5 times by PieceDraw(short x6). a99f's only caller is the thunk map_draw_unit_marks (1040:20fb, 43 bytes, void *sprite), which is called 5 times by PieceDraw_noport (int x6). Callees: SetGlobalSpriteScale x2, SetClip, DrawMono x2/Sprite_DrawMono x2, Draw, ResetClip on both. Size 300 against 321. Layout: the Mac has ONE method between RedrawMap and DrawCursor; Windows has two there, a849 then a99f. a849's thunk map_draw_marker (1040:20d3) has 0 callers, and the Mac has no thunk between PieceIconDraw(Sprite&) and PieceInit, so the index form is a Windows-only overload (see rejected).
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

extern void SetGlobalSpriteScale(int num, int den);
extern void GRPort_SetClip(GRPort *port, RECT *r);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *port, int x, int y, unsigned char mode);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_ResetClip(GRPort *port);

void MapWin_PieceIconDraw(MapWin *w, Sprite *sprite, int x, int y, int flags)
{
    int bank;
    int eight;
    RECT r1;
    RECT r2;
    RECT r3;

    bank = w->zoom + 8;
    eight = 8;
    SetGlobalSpriteScale(bank, eight);
    GRPort_SetClip((GRPort *)w, (RECT *)&w->rect.left);
    if (flags & 8)
        Sprite_DrawMono(&r1, sprite, (GRPort *)w, w->rect.left + x + 2, w->rect.top + y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 1)
        Sprite_DrawMono(&r2, sprite, (GRPort *)w, w->rect.left + x - 2, w->rect.top + y, (flags & 4) ? 0x5f : g_draw_color);
    if (flags & 2)
        Sprite_Draw(&r3, sprite, (GRPort *)w, w->rect.left + x, w->rect.top + y);
    GRPort_ResetClip((GRPort *)w);
    SetGlobalSpriteScale(1, 1);
}
