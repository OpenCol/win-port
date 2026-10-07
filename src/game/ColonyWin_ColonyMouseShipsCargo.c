/* ColonyWin_ColonyMouseShipsCargo: candidate for 1010:52ff (335 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseShipsCargo
 * name-source: sibling-build
 *
 * A click on the cargo strip of the selected ship or wagon (DGROUP:0xc16):
 * slot (px - 252) / 24; its goods, slot and amount are noted at
 * 0x4baf..0x4bb1, the goods message shown, and the goods icon dragged
 * (ColChild_DragIcon); dropped on the warehouse panel (5) it unloads through
 * ColonyWin_AttemptCargoToColony, on panel 8 it goes to ColonyWin_ColonyMouseShipsProper
 * with the panel word (+0xa0a) held at 7.  ColonyMouseArea is -called
 * and defined in-file as a stub.
 *
 * RENAMED 2026-09-27 from colony_cargo_click: BEYOND THE PROPOSALS. Mac ColonyWin::ColonyMouseShipsCargo(short, short), CODE 13 0x20ec, 310 bytes. Callees: set_colony, nth_cargo_vessel, get_cargo, get_cargo_amount, ColonyMarketMessage, DragIcon, ColonyMouseArea x2, shift_key, AttemptCargoToColony and bsr ColonyMouseShipsProper. 52ff calls set_colony, nth_cargo_vessel, get_cargo, get_cargo_amount, ColonyWin_ColonyMarketMessage, ColChild_DragIcon, ColonyMouseArea x2, shift_key, ColonyWin_AttemptCargoToColony (the AttemptCargoToColony slot) and colony_dock_click. Caller: Mac (bsr) ColonyMouseParse, Windows colony_panel_rclick. Arity: this+2 against (w, px, py). Layout: between Market and Map on both. Size 310 against 335. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
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
class SRegion : public DlgControl { public: long on_enter; long on_leave; long on_click; long on_press; long on_dblclk; SRegion(); ~SRegion(); };
class ColonyWin : public ColChild { public: SRegion region; GRPort back; GRPort terrain; SImageButton ib[3]; SButton b[2]; SImageButton ib2; char buy_shown; char change_shown; short panel; short cx; short cy; int timer; int colony; ColonyWin(); ~ColonyWin(); };
typedef struct { RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; char pad_19; short handle; unsigned char *bits; } Sprite;   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */

#include "icons.h"

extern int g_stub_a;
extern void set_colony(int colony);
extern int nth_cargo_vessel(int carrier);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);
extern void ColonyWin_ColonyMarketMessage(ColonyWin *w, int goods, int unused, int flag);
extern void ColChild_DragIcon(ColChild *w, void *sprite, int x, int y, int *rx, int *ry);
extern int shift_key(void);
extern int ColonyWin_AttemptCargoToColony(void *w, int unit, int goods, int ask);
extern void ColonyWin_ColonyMouseShipsProper(ColonyWin *w, int x, int y);

extern "C" int ColonyMouseArea(int x, int y);

void ColonyWin_ColonyMouseShipsCargo(ColonyWin *w, int px, int py)
{
    register int u;
    int rx;
    int ry;
    int rel;
    int slot;
    int goods;

    set_colony(w->colony);
    rel = px - 0xfc;
    slot = rel / 24;
    u = nth_cargo_vessel(g_selected_carrier);
    goods = get_cargo(u, slot);
    if (goods >= 0) {
        g_drag_goods = goods;
        g_drag_slot = slot;
        g_drag_amount = get_cargo_amount(u, slot);
        ColonyWin_ColonyMarketMessage(w, goods, 0, 0);
        ColChild_DragIcon((ColChild *)w, &g_icon_sprites[ICON_GOODS + g_drag_goods], px, py, &rx, &ry);
        if (ColonyMouseArea(rx, ry) == 5) {
            ColonyWin_AttemptCargoToColony(w, u, g_drag_goods, shift_key());
            goto done;
        } else if (ColonyMouseArea(rx, ry) == 8) {
            w->panel = 7;
            ColonyWin_ColonyMouseShipsProper(w, rx, ry);
            w->panel = 0;
        }
    }
done:
    ;
}
