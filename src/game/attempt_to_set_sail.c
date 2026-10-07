/* attempt_to_set_sail: candidate for 1048:5284 (489 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: attempt_to_set_sail
 *
 * The SAILAWAY dialog for the ship found by nth_europe_vessel(a): the ship's state (+8) is
 * suspended, its chain of passengers (from top_unit down the +0x1a links) and its
 * cargo shown as icons in a stack Popup, the destination set up (relocate_stacked_units with the
 * player's square), and on answer 1 the ship sails (set_course_for_new_world) with the Europe view
 * state at DGROUP:0x1fc0 adjusted; the state byte is restored (or cleared on sailing).
 *
 * RENAMED 2026-09-26 from prompt_sail_to_new_world: the Mac build's attempt_to_set_sail(short), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller europe_click_ship; callee get_cargo; callee set_width; callee sprite), the runner-up gets 2, and the arity agrees (1).
 */

#include "colonization.h"

#include "units.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern int g_ships_in_port;   /* DGROUP:0x1fca */
extern int g_damaged_in_port;   /* DGROUP:0x1fcc */

#include "icons.h"
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" int nth_europe_vessel(int a);
extern void stack_ship(int u);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void Popup_set_width(Popup *popup, int width);
extern void sort_stack_by_class(int u);
extern int top_unit(int u);
extern int next_unit(int u);
extern int UnitIcon(int u);
extern void Popup_sprite(Popup *popup, Sprite *sprite, int id, char *s);
extern int get_cargo(int u, int slot);
extern void relocate_stacked_units(int u, int x, int y);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void set_course_for_new_world(int u);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void reset_europe_units(void);
/* same object: 4-byte `push cs; call ` (rule 47) */
extern "C" void draw_europe_screen(void);

extern "C" void attempt_to_set_sail(int ship)
{
    Popup popup(0x800);
    int r;
    unsigned char saved;
    int u;
    int k;

    u = nth_europe_vessel(ship);
    saved = g_units[u].orders;
    g_units[u].orders = ORDERS_NONE;
    stack_ship(u);
    g_popup_dude = 0;
    show_plain_dialog_1048_dde3(&popup, "SAILAWAY", 0);
    Popup_set_width(&popup, 0x1e0);
    sort_stack_by_class(u);
    for (k = top_unit(u); k >= 0; k = next_unit(k))
        Popup_sprite(&popup, &g_icon_sprites[UnitIcon(k)], 0, 0);
    for (k = 0; g_units[u].cargo_count > k; k++)
        Popup_sprite(&popup, &g_icon_sprites[ICON_GOODS + get_cargo(u, k)], 0, 0);
    relocate_stacked_units(u, g_cur_nation - 20, g_cur_nation - 20);
    r = popup_run_thunk_1048_de13(&popup);
    if (r != 1)
        goto done;
    set_course_for_new_world(u);
    if (g_selection_kind == 1) {
        if (g_ships_in_port <= 1)
            g_selection_kind = 0;
    } else if (g_selection_kind == 2) {
        reset_europe_units();
        if (g_land_unit_count == 0)
            g_selection_kind = 0;
    }
    if (g_ships_in_port - g_damaged_in_port <= 1)
        g_modal_active = 0;
    saved = 0;
done:
    g_popup_dude = -1;
    g_units[u].orders = saved;
    draw_europe_screen();
}
