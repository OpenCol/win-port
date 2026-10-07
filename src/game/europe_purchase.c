/* europe_purchase: candidate for 1048:7296 (957 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_purchase
 * name-source: sibling-build
 *
 * The Europe purchase screen. Six buyable types come from the 6-byte records at
 * SEG20:0xd83d -- a type byte at +0 and a price word at +3 -- and each becomes a menu line
 * built by hand: the type's name, a space, the `":   "` separator, the MISC_COST word, a space, the price
 * through `LTOA1`, and the MISC_CLOSE_PAREN word. A line the player cannot afford is greyed
 * with `Popup_grey` rather than hidden.
 *
 * ARTILLERY GETS DEARER EVERY TIME. Type 0xb has `nation->artillery * 100` added to its
 * listed price, and buying one increments that counter -- so the escalation is stored per
 * nation at +0x1e and the price in the table never changes. The addition appears TWICE, once
 * to display the price and once to charge it, and the second is what makes it real.
 *
 * The whole screen is a loop: picking a line with the pedia modifier held (the high bit
 * `Popup_help_me` reports) opens the Colonopedia on that type and comes back
 * to the top rather than buying. Item 0x19d is the MISC_NONE line; picked with the modifier it opens the pedia at -1.
 *
 * A new unit starts with orders 0 if its type is in 0xd..0x12 -- the ship range, the same
 * one purge_square uses -- and 1 otherwise, then takes two bytes from the nation record at
 * +0x32 and +0x33.
 *
 * NAME: the Macintosh build's `europe_purchase(void)`, and it takes no arguments, which
 * `tools/signatures.py` confirms from the call site. Recorded as ADOPTED even though the
 * descriptive route reaches the same word from the `PURCHASE` resource id and the body,
 * because the Macintosh table was consulted in the same breath and an independence that
 * cannot be demonstrated should not be counted (§117). The agreement is worth noting and
 * not worth spending.
 *
 * The separator literal is `":   "` -- a colon and THREE spaces -- read out of the image at
 * DGROUP:0x21a4. It was first written as `"-"` on the strength of nothing, and a byte-match
 * cannot tell: the push is a fixup and every literal compiles to the same bytes. The length
 * is fixed by its neighbours (`PURCHASE` ends at 0x21a4 and `REALLYBUY` starts at 0x21a9,
 * so four characters plus a NUL) and the content by reading those five bytes.
 */

#include "colonization.h"

#include "units.h"
#include "misc.h"
#include "w16types.h"
/* PORT: this file's own EventWin, GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* EventWin : Screen, EventTable, flattened for C: 0x8c bytes */
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */

#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };
                                  /* 6 bytes at SEG20:0xd83c -- the base `setup_purchase`
                                     writes, one byte BEFORE the 0xd83d the readers used */

extern char g_pedia_win;          /* SEG20:0x387c, declared */

extern void Popup_item(Popup *popup, char *s, int id);
extern char *strings(int n);

extern void space_string(char *buf);

extern void Popup_grey(Popup *p, int id, int on);
extern int Popup_help_me(Popup *popup);
extern void PediaWin_ShowModal(void *pedia, int category, int article, void *parent);
extern void popup_say(int slot, int v);
extern int popm(char *msg, int n);

extern "C" void set_text_arg_i_1048_db6a(int slot, int v);
extern "C" void draw_europe_screen(void);

void europe_purchase(void)
{
    Popup p(0x800);
    int ev;
    int price;
    int idx;
    int type;
    int unit;
    char buf[0x50];
    char num[0x14];
    register int i;

restart:
    g_popup_dude = 2;
    show_plain_dialog_1048_dde3(&p, "PURCHASE", 0);
    p.flags |= 1;
    Popup_item(&p, strings(g_misc_words[MISC_NONE]), 0x19d);
    for (i = 0; i < 6; i++) {
        type = g_recruit[i].type;
        price = g_recruit[i].price;
        if (type == 0xb)
            price += g_cur_nation_rec->artillery * 100;
        STRCPY(buf, strings(g_unit_types[type].name));
        space_string(buf);
        STRCAT(buf, ":   ");
        say_string(buf, g_misc_words[MISC_COST]);
        space_string(buf);
        LTOA1(price, num, 10);
        STRCAT(buf, num);
        say_string(buf, g_misc_words[MISC_CLOSE_PAREN]);
        Popup_item(&p, buf, i + 1);
        if ((long)price > g_cur_nation_rec->gold)
            Popup_grey(&p, i + 1, 1);
    }
    p.flags |= 0x40;
    ev = popup_run_thunk_1048_de13(&p);
    if (ev < 2)
        goto out;
    if (ev == 0x19e && Popup_help_me(&p) == 0)
        goto out;
    if (ev != 0x19e) {
        idx = ev - 2;
        price = g_recruit[idx].price;
        type = g_recruit[idx].type;
    }
    if (Popup_help_me(&p) != 0) {
        if (ev == 0x19e)
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, -1, (EventWin *)&g_europe_win_dlg);
        else
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, type, (EventWin *)&g_europe_win_dlg);
        goto restart;
    }
    if (type == 0xb) {
        price += g_cur_nation_rec->artillery * 100;
        g_cur_nation_rec->artillery++;
    }
    popup_say(0, g_unit_types[type].name);
    set_text_arg_i_1048_db6a(0, price);
    if (popm("REALLYBUY", 2) != 1)
        goto out;
    g_cur_nation_rec->gold -= price;
    unit = create_unit(type, g_cur_nation, g_cur_nation - 20, g_cur_nation - 20);
    if (unit < 0)
        goto out;
    g_units[unit].orders =
        (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) ? 0 : 1;
    g_units[unit].dest_x = g_cur_nation_rec->start_x;
    g_units[unit].dest_y = g_cur_nation_rec->start_y;
    draw_europe_screen();
out:
    g_popup_dude = -1;
}

