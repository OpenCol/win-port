/* ColonyWin_ColonyMouseUnits: candidate for 1010:62bd (412 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyMouseUnits
 * name-source: sibling-build
 *
 * A click on the units strip of the colony window: the bottom row (y >=
 * 337) has 34-pixel cells from x 421, the two rows above 10-pixel cells
 * (index row*17 + col - 12); min_ptr_1010_f2b7 clamps the index to the tile's
 * unit count (a C++ const reference bound to the count expression), panel 3 becomes current and the unit is selected.  Without
 * the right-button flag (DGROUP:0x9dc) the orders menu opens; with it the
 * unit's type goes to the Colonizopedia.  The three module helpers are
 * -called and defined in-file as stubs.
 *
 * RENAMED 2026-09-26 from colony_units_click: the Mac build's ColonyWin::ColonyMouseUnits(short, short): this plus x and y, a click on the units strip. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
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

#include "records.h"

extern char g_pedia_win;             /* SEG20:0x387c */
extern int g_stub_a;
extern void set_colony(int colony);

extern void ColonyWin_DrawMiscellaneous(void *w, int redraw);
extern int nth_noncarrier_in_chain(int first, int n);
extern void PediaWin_ShowModal(void *ctx, int category, int type, void *dlg);

extern "C" void ColonyWin_ColonyClickUnits(void *w);
extern "C" void colony_change_selected(void);

extern "C" void ColonyWin_ColonyMouseUnits(ColonyWin *w, int px, int py)
{
    register int rel;
    register int col;
    int row;
    int idx;
    int last;

    set_colony(w->colony);
    if (py >= 0x151) {
        row = 0;
        rel = px - 0x1a5;
        rel = minimax(rel, 0, 0xa9);
        col = rel / 34;
        idx = col;
    } else {
        if (py >= 0x135)
            row = 1;
        else
            row = 2;
        rel = px - 0x1a5;
        rel = minimax(rel, 0, 0xa8);
        col = (rel + 1) / 10;
        idx = row * 17 + col - 12;
    }
    idx = *min_ptr_1010_f2b7(&idx, (last = g_tile_noncarriers - 1, &last));
    g_panel_mode = 3;
    g_tile_sel = idx;
    if (g_pedia_mode == 0) {
        ColonyWin_DrawMiscellaneous(w, 1);
        MSDoAllTasks_1010_e5bd();
        ColonyWin_ColonyClickUnits(w);
        ColonyWin_DrawMiscellaneous(w, 1);
        colony_change_selected();
    } else if (g_tile_colonists != 0) {
        ColonyWin_DrawMiscellaneous(w, 1);
        MSDoAllTasks_1010_e5bd();
        idx = nth_noncarrier_in_chain(g_tile_first_unit, g_tile_sel);
        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, g_units[idx].type, &(DialogWin &)*w);
        ColonyWin_DrawMiscellaneous(w, 1);
    }
}
