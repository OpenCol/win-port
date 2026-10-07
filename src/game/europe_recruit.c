/* europe_recruit: candidate for 1048:6e52 (1063 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_recruit
 * name-source: descriptive
 *
 * RECRUITING ONE OF THE THREE COLONISTS WAITING ON THE EUROPE DOCKS, in three flavours that
 * share one menu:
 *   - RECRUIT (both flags 0): the player pays passage;
 *   - RECRUITCHOOSE (`choose`): religious unrest brings an emigrant free and the player picks which.
 *     `europe_new_turn` calls it this way when William Brewster lets the player choose;
 *   - LOSTCITY0 (`youth`): the Fountain of Youth. The rumour handler (1038:7c74) calls it eight
 *     times, one free pick each, and it leaves the crosses alone.
 * Each sets the help context DGROUP:0x251a (2, 4, 3) while the menu is up and clears it to -1.
 *
 * THE PRICE. It starts at `(difficulty + recruits bought) * 20 + 140`, with the count kept in the
 * nation record's +6. It is discounted by the crosses already gathered: `price -= (price - floor) *
 * crosses / (need + 1)`, where the floor is the larger of 100 and a fifth of the price. The result
 * is at least 10, and a free pick costs 0. A line the treasury cannot pay is greyed.
 *
 * THE MENU. It lists the three recruits by profession, JOB_COLONIST_ALT shown as JOB_COLONIST.
 * A paid recruit gets an extra first line, the parenthesised word at SEG20:0xdcce, as item 0x19d.
 * A computer nation takes slot 1 without asking.
 * Picking with the Colonopedia modifier (`Popup_help_me`) opens article 3 on
 * that profession, or on -1 for the extra line, and rebuilds the menu.
 *
 * A PICK creates the unit (`unit_from_docks`). If that fails, nothing more happens -- but for every pick except the youth one the
 * crosses were already zeroed just before the call.
 * Otherwise it takes the price from the gold and refills the slot from `fill_docks(0)`.
 * A paid recruit also steps the purchase count, capped at 180, and refreshes the docks. Every pick
 * except the youth one zeroes the crosses.
 *
 * THE DISCOUNT NEEDS ITS `(int)`: `price -= (int)(long expression)`. Without the cast Borland
 * computes the subtraction in DX and stores it back. With it, it subtracts AX from the variable
 * in place, as the shipped does. The two `max`/`min` template calls take their operands through
 * comma expressions inside the argument list (rule 61, as in `europe_new_taxes`).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "jobs.h"
#include "misc.h"
#include "w16types.h"
typedef struct { short style; void *ctable; void *win; short fa; char disabled; void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; } EventWin;   /* EventWin : Screen, EventTable, flattened for C: 0x8c bytes */
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern char g_pedia_win;                /* SEG20:0x387c */

extern void popup_say(int slot, int word);
extern void open_string(char *s);
extern void close_string(char *s);

extern void Popup_item(Popup *popup, char *s, int id);
extern void Popup_grey(Popup *p, int id, int on);
extern int Popup_help_me(Popup *popup);
extern void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int unit_from_docks(int type);
extern "C" unsigned char fill_docks(int from_pool);
extern "C" void reset_europe_units(void);
extern "C" void europe_change_mode(void);

void europe_recruit(int youth, int choose)
{
    int price;
    int pick;
    int slot;
    int unit;
    int n;
    int fifth;
    int hundred;
    int next;
    int cap;
    char buf[0x50];
    register int i;
    register int type;
    Popup p(0x800);

    price = (g_game.difficulty + g_cur_nation_rec->recruit_count) * 20 + 0x8c;
    n = *max_ptr_1048_efef((hundred = 100, &hundred), (fifth = price / 5, &fifth));
    price -= (int)((long)(price - n) * g_cur_nation_rec->crosses / (g_cur_nation_rec->need + 1L));
    price = max_int_1048_dd61(10, price);
    if (youth != 0 || choose != 0)
        price = 0;
    g_text_args[0] = price;
extern "C" top: if (youth != 0); else if (choose != 0) {
        g_popup_dude = 4;
        popup_say(0, g_nation_name_word[g_cur_nation]);
        show_plain_dialog_1048_dde3(&p, "RECRUITCHOOSE", 0);
    } else {
        g_popup_dude = 2;
        show_plain_dialog_1048_dde3(&p, "RECRUIT", 0);
    }
    if (youth == 0 && choose == 0)
        p.flags |= 1;
    n = 0;
    if (youth == 0 && choose == 0) {
        buf[0] = 0;
        open_string(buf);
        say_string(buf, g_misc_words[MISC_NONE]);
        close_string(buf);
        Popup_item(&p, buf, 0x19d);
        n = 1;
    }
    for (i = 0; i < 3; i++) {
        type = g_cur_nation_rec->recruit[i];
        if (type == JOB_COLONIST_ALT)
            type = JOB_COLONIST;
        buf[0] = 0;
        say_string(buf, g_spec_recs[type].expert);
        Popup_item(&p, buf, i + n);
        if ((long)price > g_cur_nation_rec->gold)
            Popup_grey(&p, i + n, 1);
    }
    g_dialog_help_used = 0;
    if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) {
        p.flags |= 0x40;
        p.flags &= ~1;
        pick = popup_run_thunk_1048_de13(&p) - 2;
        if (youth != 0 || choose != 0)
            pick++;
    } else {
        pick = 1;
    }
    if (pick < 0)
        goto done;
    slot = pick;
    type = g_cur_nation_rec->recruit[slot];
    if (pick == 0x19c && !Popup_help_me(&p))
        goto done;
    if (Popup_help_me(&p)) {
        if (type == JOB_COLONIST_ALT)
            type = JOB_COLONIST;
        if (pick == 0x19c)
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, -1, (EventWin *)&g_europe_win_dlg);
        else
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, type, (EventWin *)&g_europe_win_dlg);
        goto top;
    }
    if (youth == 0)
        g_cur_nation_rec->crosses = 0;
    unit = unit_from_docks(type);
    if (unit < 0)
        goto done;
    g_cur_nation_rec->gold -= price;
    g_cur_nation_rec->recruit[slot] = fill_docks(0);
    if (youth == 0 && choose == 0) {
        g_cur_nation_rec->recruit_count = *min_ptr_1048_f021((cap = 0xb4, &cap), (next = g_cur_nation_rec->recruit_count + 1, &next));
        reset_europe_units();
        europe_change_mode();
    }
done:
    g_popup_dude = -1;
}

