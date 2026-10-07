/* CheatMenuBlow: candidate for 1048:b9da (249 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CheatMenuBlow
 *
 * The Cheat entry that wipes out a tribe. It counts how many of the eight already carry
 * flag 0x80 -- gone -- and does nothing at all if all eight do; otherwise every surviving
 * tribe is offered by adjective (`get_name(t + 4)` into the string table) and the
 * chosen one has its settlements destroyed.
 *
 * `DrawMap(1)` is OUTSIDE the `if`: the map is repainted even when the popup
 * never opened, which is what puts the call after the closing brace rather than inside it,
 * and it costs ten bytes to get wrong -- enough to push the guard's `JGE` out of short
 * range, which is how the first compile showed it.
 *
 * The two loops test the same flag in opposite spellings and Borland distinguishes them
 * (§114): the counting loop is `if (flags & 0x80)` and tests memory directly, the offering
 * loop is `if (!(flags & 0x80))` and loads the byte, promotes it and tests AL.
 *
 * `gone` is declared first so it takes DI; `t` is `register` and takes SI (rule 40).
 *
 * RENAMED 2026-09-26 from kill_indians: the Mac build's CheatMenuBlow(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between memory_check and game_menu_debug, which are already bound; the arity agrees and the Mac body is 1.08x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern void Popup_item(Popup *popup, char *s, int id);
extern int get_name(int nation);
extern char *strings(int n);
extern void kill_tribe(int tribe);
extern void DrawMap(int mode);

extern "C" void popup_setup_plain_1048_e387(Popup *p);

void CheatMenuBlow(void)
{
    int gone;
    register int tribe;
    int pick;
    Popup p(0x800);

    gone = 0;
    for (tribe = 0; tribe < 8; tribe++)
        if (g_tribes[tribe].flags & 0x80)
            gone++;
    if (gone < 8) {
        popup_setup_plain_1048_e387(&p);
        for (tribe = 0; tribe < 8; tribe++) {
            if (!(g_tribes[tribe].flags & 0x80))
                Popup_item(&p, strings(get_name(tribe + 4)), tribe);
        }
        pick = popup_run_thunk_1048_de13(&p);
        if (pick > 0)
            kill_tribe(pick - 1);
    }
    DrawMap(1);
}

