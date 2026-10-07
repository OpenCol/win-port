/* EuropeWin_DrawLandUnit: candidate for 1048:9bfc (372 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EuropeWin_DrawLandUnit
 * name-source: sibling-build
 *
 * Draws one unit into the Europe dock's grid. `EuropeWin_DrawUnits` calls it once per
 * unit and passes the running index BY ADDRESS; this function reads it, lays the cell out
 * through `get_unit_loc`, and increments it on the way out -- which is why the caller never
 * touches it.
 *
 * `get_unit_loc` returns a BAND, and band 2 means off the end of the panel: a unit there is
 * not drawn and not outlined, but still counted.
 *
 * A ship in band 0 with something in its hold gets a second sprite on top of it, the cargo's,
 * from the 0x20-byte array at SEG20:0x6526. Types 0xd..0x12 are the ships -- the same range
 * `try_to_purchase` uses to decide the byte at Unit+8.
 *
 * THAT SECOND SPRITE IS DRAWN AT THE WRONG PLACE, and the bytes say so plainly: both
 * coordinates are taken from the window's X origin at +0x76a, `MOV AX,[ES:BX+0x76A]` twice,
 * where every other call in the function pairs +0x76a with +0x76c. A `+0x76a` where a `+0x76c`
 * belongs is a one-character slip in the source, and it ships.
 *
 * The outline is drawn only when `h` is not negative, and its rectangle is 0x20 wide by 0x20
 * tall centred on the cell.
 *
 * `set_europe` and `get_unit_loc` come through the 4-byte form and are the stubs (rule 47).
 *
 * RENAMED 2026-09-26 from dock_draw_unit: the Mac build's EuropeWin::DrawLandUnit(short, short, short*, short): this plus a unit, x, the running index BY ADDRESS and the height, in that order; EuropeWin_DrawUnits calls it once per unit on the dock.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
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
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class EuropeWin : public ColChild { public: short nation; SRegion region; SButton recruit; SButton train; SButton purchase; SImageButton image; GRPort port; EuropeWin(); ~EuropeWin(); };

typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
#include "records.h"

#include "icons.h"

/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void get_unit_loc(int index, int x, int *band, int *pos, int *width, int *p6, int *p7);
extern int get_cargo(int u, int slot);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void GRPort_DrawRect(GRPort *w, RECT *r);

extern void PieceDraw(GRPort *port, int u, int flags, int x, int y, int span, int mode);

/* same object: 4-byte `push cs; call ` */
extern "C" void set_europe(int n);

void EuropeWin_DrawLandUnit(EuropeWin *w, register int u, int x, int *n, int h)
{
    int band;
    int px;
    int py;
    int d;
    int e;
    RECT r;
    RECT s;

    set_europe(w->nation);
    get_unit_loc(*n, x, &band, &px, &py, &d, &e);
    if (band < 2) {
        PieceDraw((GRPort *)w, u, 0, w->rect.left + px, w->rect.top + py, 0x20, 0x64);
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST && band == 0
            && g_units[u].cargo_count != 0)
            Sprite_Draw(&s, &g_icon_sprites[ICON_GOODS + get_cargo(u, 0)], (GRPort *)w,
                        w->rect.left + px, w->rect.left + py);
    }
    if (h >= 0 && band < 2) {
        SetRect(&r, w->rect.left + px - 0x10, w->rect.top + py, w->rect.left + px + 0x10, w->rect.top + py + 0x20);
        TextColor(0);
        GRPort_DrawRect((GRPort *)w, &r);
    }
    (*n)++;
}
