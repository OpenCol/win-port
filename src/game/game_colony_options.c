/* game_colony_options: candidate for 1048:b374 (798 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_colony_options
 *
 * The `COLONYOPTIONS` dialog over ten bits of SEG20:0x7786. Three masks clear exactly those
 * ten -- `~0x103`, `~0xe0`, `~0x21c`, in that order -- and each checkbox puts its bit back.
 *
 * EVERY ONE of the ten is stored INVERTED, in both directions: set from `!(flags & mask)`
 * and restored when the box comes back CLEAR. So each bit is a SUPPRESSION flag and the
 * dialog shows the player the feature, not the suppression. `game_menu_options` at 1048:b6ae
 * has the same structure with only slot 6 inverted, which is what makes that one easy to
 * get wrong and this one easy to get right: here the `NEG/SBB/INC` is on every line.
 *
 * The bit order is not the slot order -- 0x2, 0x1, 0x80, 0x40, 0x20, 0x10, 0x8, 0x4, 0x100,
 * 0x200 -- so the first two are swapped and 0x100/0x200 come last. That is the dialog's
 * layout, not the word's, and there is no arithmetic that produces it.
 */

#include "colonization.h"

#include "colopt.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };
extern GameState g_game;   /* SEG20:0x7782 */

extern void Popup_set_check(Popup *p, int key, int on);
extern int Popup_get_check(Popup *p, int key);

void game_colony_options(void)
{
    Popup p(0x800);

    show_text_dialog_1048_e35c(&p, "COLONYOPTIONS");
    Popup_set_check(&p, 0, !(g_game.colony_options & COLOPT_NO_BUILDING_LABELS));
    Popup_set_check(&p, 1, !(g_game.colony_options & COLOPT_NO_TERRAIN_LABELS));
    Popup_set_check(&p, 2, !(g_game.colony_options & COLOPT_NO_TRAINED));
    Popup_set_check(&p, 3, !(g_game.colony_options & COLOPT_NO_FOOD_SHORTAGE));
    Popup_set_check(&p, 4, !(g_game.colony_options & COLOPT_NO_MATERIALS_SHORTAGE));
    Popup_set_check(&p, 5, !(g_game.colony_options & COLOPT_NO_TOOLS_NEEDED));
    Popup_set_check(&p, 6, !(g_game.colony_options & COLOPT_NO_BAD_GOVERNMENT));
    Popup_set_check(&p, 7, !(g_game.colony_options & COLOPT_NO_NEW_CARGO));
    Popup_set_check(&p, 8, !(g_game.colony_options & COLOPT_NO_SONS_OF_LIBERTY));
    Popup_set_check(&p, 9, !(g_game.colony_options & COLOPT_NO_REBEL_MAJORITY));
    popup_run_thunk_1048_de13(&p);
    g_game.colony_options &= ~0x103;
    g_game.colony_options &= ~0xe0;
    g_game.colony_options &= ~0x21c;
    if (!Popup_get_check(&p, 0))
        g_game.colony_options |= COLOPT_NO_BUILDING_LABELS;
    if (!Popup_get_check(&p, 1))
        g_game.colony_options |= COLOPT_NO_TERRAIN_LABELS;
    if (!Popup_get_check(&p, 2))
        g_game.colony_options |= COLOPT_NO_TRAINED;
    if (!Popup_get_check(&p, 3))
        g_game.colony_options |= COLOPT_NO_FOOD_SHORTAGE;
    if (!Popup_get_check(&p, 4))
        g_game.colony_options |= COLOPT_NO_MATERIALS_SHORTAGE;
    if (!Popup_get_check(&p, 5))
        g_game.colony_options |= COLOPT_NO_TOOLS_NEEDED;
    if (!Popup_get_check(&p, 6))
        g_game.colony_options |= COLOPT_NO_BAD_GOVERNMENT;
    if (!Popup_get_check(&p, 7))
        g_game.colony_options |= COLOPT_NO_NEW_CARGO;
    if (!Popup_get_check(&p, 8))
        g_game.colony_options |= COLOPT_NO_SONS_OF_LIBERTY;
    if (!Popup_get_check(&p, 9))
        g_game.colony_options |= COLOPT_NO_REBEL_MAJORITY;
}

