/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 54 of the imported file. */
/* orders_overboard: candidate for 1008:00d2 (384 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: orders_overboard
 *
 * Asks which of the selected unit's cargo slots to throw overboard, and throws it. Nothing
 * happens for a unit carrying nothing. The prompt is `OVERBOARD` at DGROUP:0x154.
 *
 * Each slot's label is built into a local buffer, not fetched: the amount as a number, a
 * space, then the goods name through `g_goods_name_word[type]`. Item ids are the slot
 * numbers and 0x63 is the cancel entry, added FIRST so it sits at the top of the list; a
 * pick of 0x63 or a negative event does nothing.
 *
 * NAME: descriptive. The Macintosh build's `orders_unload(void)` has the right arity and
 * is the wrong operation -- it also carries `unload_cargo(short, short)` for putting cargo
 * into a colony, and this one drops it into the sea. `OVERBOARD` is the only word either
 * build offers for that and it is a resource id, not a symbol, so the name stays ours.
 *
 * RENAMED 2026-09-26 from prompt_dump_cargo: the Mac build's orders_overboard(void), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee get_cargo; callee item; callee say_number; callee say_string), the runner-up gets 5, and the arity agrees (0).
 */

#include "colonization.h"

#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern GameState g_game;   /* SEG20:0x7782 */

extern void Popup_item(Popup *popup, char *s, int id);
extern char *strings(int n);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);

extern void space_string(char *buf);
extern int unload_cargo(int u, int slot);

void orders_overboard(void)
{
    Popup p(0x800);
    int type;
    int amt;
    int pick;
    char buf[0x50];
    register int slot;
    int u;

    u = g_game.selected_unit;
    if (g_units[u].cargo_count == 0)
        goto out;
    g_popup_dude = 0;
    show_plain_dialog_1008_cd53(&p, (void*)("OVERBOARD"), 0);
    Popup_item(&p, strings(g_misc_words[MISC_NOTHING]), 0x63);
    for (slot = 0; slot < g_units[u].cargo_count; slot++) {
        type = get_cargo(u, slot);
        amt = get_cargo_amount(u, slot);
        buf[0] = 0;
        say_number(buf, amt);
        space_string(buf);
        say_string(buf, g_goods_name_word[type]);
        Popup_item(&p, buf, slot);
    }
    pick = popup_run_thunk_1008_cd83(&p) - 1;
    if (pick < 0 || pick == 0x63)
        goto out;
    unload_cargo(u, pick);
out:
    g_popup_dude = -1;
}

