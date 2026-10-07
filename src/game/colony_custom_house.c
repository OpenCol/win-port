/* colony_custom_house: candidate for 1010:4068 (328 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: colony_custom_house
 *
 * The custom house dialog of the colony at SEG20:0x2fd0: a CUSTOM Popup listing the 16
 * goods (names from the word table at SEG20:0xd86e) with each row's check set from the
 * colony's export flag; after the dialog every flag is written back from its row. The
 * colony window (SEG20:0x25e4) is disabled meanwhile and redrawn afterwards, with the
 * main window record (SEG20:0x5ca) disabled (`win_disable_1010_e459`). Colours 12 and 7 are set first.
 *
 * RENAMED 2026-09-26 from prompt_custom_house_exports: the Mac build's colony_custom_house(void), bound by call-graph agreement (tools/mac_callgraph.py): 5 of its Mac call edges already bound land on this function (callee get_check; callee item; callee set_check; callee set_colony; callee strings), the runner-up gets 3, and the arity agrees (0).
 */

#include "colonization.h"

#include "goods.h"
#include "w16types.h"
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };
typedef struct { short style; void *ctable; void *win; short fa; char disabled; } Screen;

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;    /* SEG20:0x5ca */

extern void MSEnableWindow(void *win, int on);

extern void MSSetFocus_of_1010_eb8f(Screen *w);
typedef struct { short style; void *ctable; void *win; short fa; char disabled; void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; struct DlgControl *first_ctl; struct DlgControl *ok_btn; struct DlgControl *cancel_btn; char keyboard_nav; char f99; } DialogWin;   /* DialogWin : EventWin, flattened for C */

extern void DialogWin_SetDefaultControlFocus(DialogWin *w);
extern int Popup_exec(Popup *popup, char *text);

extern void set_colony(int colony);
extern char *strings(int n);
extern void Popup_item(Popup *popup, char *s, int id);
extern int custom_sell(int good);
extern void Popup_set_check(Popup *p, int key, int on);
extern int Popup_get_check(Popup *p, int key);
extern void custom_set(int good, int on);
extern void colony_change_mode(void);

/* The module's one-line wrappers are defined here because the shipped calls to them are
 * the compiler's own `push cs / call `; their own callees are not in this file. */

extern "C" void win_disable_1010_e459(Screen *w);

extern "C" void win_enable_1010_e4d4(Screen *w);

extern "C" void colony_custom_house(void)
{
    Popup popup(0x800);
    int i;

    set_check_color_on_1010_e423(12);
    set_check_color_off_1010_e43e(7);
    set_colony(g_custom_house_colony);
    win_disable_1010_e459((Screen *)&g_colony_win_rec);
    show_plain_step_1010_e484(&popup, g_game_txt, "CUSTOM");
    for (i = 0; i < GOODS_COUNT; i++) {
        Popup_item(&popup, strings(g_goods_name_word[i]), i + 1);
        Popup_set_check(&popup, i + 1, custom_sell(i));
    }
    popup_run_thunk_1010_e4b1(&popup);
    for (i = 0; i < GOODS_COUNT; i++)
        custom_set(i, Popup_get_check(&popup, i + 1));
    win_disable_1010_e459((Screen *)&g_main_win);
    win_enable_1010_e4d4((Screen *)&g_colony_win_rec);
    win_show_and_focus_1010_e4ff(&g_colony_win_rec);
    colony_change_mode();
}
