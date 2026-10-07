/* ColonyWin_colony_hammer_change: candidate for 1010:6f3a (1041 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_colony_hammer_change
 * name-source: sibling-build
 *
 * The colony window's "what to build" menu. It lists what the colony can build, one
 * `ColonyWin_build_list_add` line per item, under the heading word at SEG20:0xd98a, with the item
 * under construction pre-selected. Up to 22 items fit one menu; more are split into two pages of 16
 * with page-turn lines (ids 0x62 back, 0x63 forward, both labelled with the word at SEG20:0xd98e),
 * and turning the page rebuilds the menu.
 *
 * The colony's item (+0x94) becomes the answer less one, with bit 7 of +0x1c cleared -- so answer 0
 * stores -1, building nothing, and it alone hides the cost display (+0xa08 cleared, control +0x950
 * hidden) where every other answer shows it. When the menu reports its
 * alternate-click flag (`Popup_help_me`), the item is not chosen: its Colonopedia article
 * opens instead -- buildings as category 4, units as category 1 -- and the menu is rebuilt.
 *
 * THE COLONOPEDIA PATH LEAKS THE MENU. It jumps back to the top of the loop, which allocates a new
 * `Popup`, without deleting the one it came from; every other path deletes it. Each such look-up
 * costs 0x3dc bytes of the heap until the game exits.
 *
 * THE CLASS DECLARATION DECIDES THE `delete`. The menu is `new Popup` / `delete`, and the shipped
 * code copies the pointer into a temporary, null-tests it, moves the unwinder headcount by 4 and
 * brackets the destructor call with exception-state stores. That is what Borland writes for a class
 * with three destructible sub-objects -- Popup's GRPort at +8 and Sprites at +0x2e and +0x4e, as
 * `Popup_ctor` builds them (rule 49's `1 + 3`). Declared as a flat block of bytes, the same source
 * calls the destructor directly and the frame is four bytes smaller.
 *
 * The count test is `if (count <= 14) {} else if (count > 22)`: an empty then-arm, which is where
 * the shipped JMP over the second test comes from. The window's timer at +0xa10 is reset and control
 * +0x972 shown first, and DGROUP:0xa04 guards re-entry.
 *
 * RENAMED 2026-09-27 from colony_choose_build: CONST PROPOSAL. Mac ColonyWin::colony_hammer_change(short), CODE 30 0x01b8, 1072 bytes. Callers: Mac KeyClick and ColonyHammerChange; Windows ColonyWin_KeyClick and colony_choose_build_clicked (= ColonyHammerChange, below). Callees: ResetTimer, show/enable controls, set_colony, count_eligible_builds, Popup ctor, set_width, strings x3, string, item x2, nth_eligible_build, build_list_add (= build_menu_item), hilite x3, set_active x3, exec, help_me, get_production_class, ShowModal x2, Popup dtor, colony_change_mode. 6f3a's callees are the same (PediaWin_ShowModal x2 for ShowModal, popup_run_thunk for exec). Arity: this+short against (ColonyWin *w, int unused1). Layout: after build_list_add on both, and before the two button thunks and colony_hammer_buy. Size 1072 against 1041. The Mac spelling is lower-case, as in the already-adopted ColonyWin_colony_hammer_buy. The file is C++ with an extern "C" definition. Apply colony_choose_build_clicked -> ColonyHammerChange with it: the old name is a prefix of that file's name.
 */

#include "colonization.h"

#include "labels.h"
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
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;                   /* SEG20:0x5ca */

extern char g_pedia_win;                /* SEG20:0x387c, declared */

extern void set_colony(int colony);
extern int count_eligible_builds(void);
extern int nth_eligible_build(int nth);
extern void Popup_set_width(Popup *popup, int width);
extern char *strings(int n);
extern void Popup_string(Popup *popup, char *s);
extern void Popup_item(Popup *popup, char *s, int id);
extern void Popup_hilite(Popup *o, int key, int on);
extern void Popup_set_active(Popup *popup, int key);
extern int Popup_help_me(Popup *popup);
extern int get_production_class(int item, int *idx);
extern void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void MSShowControl_1010_e574(void *control);
extern "C" void MSHideControl_1010_e54d(void *control);
extern "C" void win_disable_1010_e459(void *o);
extern "C" void win_enable_1010_e4d4(void *o);

extern "C" void ColonyWin_build_list_add(ColonyWin *w, Popup *p, int item);

extern "C" void colony_change_mode(void);

extern "C" void ColonyWin_colony_hammer_change(ColonyWin *w, int unused1)
{
    int count;
    int pages = 1;
    int per_page;
    int base;
    int item;
    int again = 1;
    int choice;
    int kind;
    int preselected = 0;
    Popup *popup;
    register int k;
    register int page;

    if (w->timer != -1) {
        ResetTimer(w->timer);
        w->timer = -1;
    }
    MSShowControl_1010_e574(&w->b[1]);
    if (g_choose_busy != 0)
        return;
    g_choose_busy = 1;
    win_disable_1010_e459(&g_colony_win_rec);
    set_colony(w->colony);
    count = count_eligible_builds();
    per_page = count;
    if (count <= 0xe) {
    } else if (count > 0x16) {
        pages = 2;
        per_page = 0x10;
    }
    page = 0;
    do {
top:
        popup = new Popup(0x800);
        Popup_set_width(popup, 0x1f4);
        popup->flags |= 1;
        Popup_string(popup, strings(g_ctitle_word[CTITLE_SELECT_AN_ITEM_TO_BUILD]));
        if (page > 0)
            Popup_item(popup, strings(g_ctitle_word[CTITLE_MORE]), 0x62);
        base = per_page * page;
        for (k = 0; k < per_page; k++) {
            item = nth_eligible_build(base + k);
            if (item >= -1) {
                ColonyWin_build_list_add(w, popup, item);
                if (g_colonies[w->colony].item == item) {
                    Popup_hilite(popup, item, 1);
                    Popup_set_active(popup, item + 1);
                    preselected = 1;
                }
            }
        }
        if (pages - 1 > page) {
            Popup_item(popup, strings(g_ctitle_word[CTITLE_MORE]), 0x63);
            if (preselected == 0) {
                Popup_set_active(popup, 0x63);
                Popup_hilite(popup, 0x63, 1);
                preselected = 1;
            }
        }
        if (preselected == 0 && page != 0) {
            Popup_set_active(popup, 0x62);
            Popup_hilite(popup, 0x62, 1);
        }
        popup->flags |= 0x40;
        choice = popup_run_thunk_1010_e4b1(popup) - 1;
        switch (choice) {
        case 0x62:
            page--;
            break;
        case 0x63:
            page++;
            break;
        default:
            again = 0;
            if (choice >= 0) {
                if (choice == 0) {
                    w->buy_shown = 0;
                    MSHideControl_1010_e54d(&w->b[0]);
                } else {
                    w->buy_shown = 1;
                    MSShowControl_1010_e574(&w->b[0]);
                }
                if (Popup_help_me(popup) != 0) {
                    kind = choice - 1;
                    kind = get_production_class(kind, &item);
                    if (kind == 1)
                        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 4, item, &g_colony_win_rec);
                    else if (kind == 2)
                        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, item, &g_colony_win_rec);
                    goto top;
                }
                g_colonies[w->colony].item = choice - 1;
                g_colonies[w->colony].status &= 0x7f;
            }
            break;
        }
        delete popup;
    } while (again != 0);
    g_choose_busy = 0;
    win_disable_1010_e459((Screen *)&g_main_win);
    win_enable_1010_e4d4(&g_colony_win_rec);
    win_show_and_focus_1010_e4ff(&g_colony_win_rec);
    colony_change_mode();
}
