/* orders_clear: candidate for 1008:03d4 (715 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: orders_clear
 *
 * The Plow order for the selected unit. Impossible on plane-1 bit 0x40 (NOPLOW, priority 3).
 * On land no European owns, a native settlement whose distance at DGROUP:0x4bc6 is
 * within the tribe's `tribe_radius`, at peace with the player (pair flag 0x40), without the
 * claim already settled (plane-1 bit 0x10) and on terrain 8..23 asks a price (village_bribe with the
 * current settlement at DGROUP:0x4c34): the INDIANFOREST Popup (button 1 disabled when the treasury is
 * short) -- answer 1 cancels the order, 2 pays (INDIANBRIBE, claim-settled bit set, tribe counter +5
 * bumped). Otherwise the unit is put to work (state 8) and the clearing started.
 *
 * RENAMED 2026-09-26 from plow_or_clear_tile: the Mac build's orders_clear(void), bound by call-graph agreement (tools/mac_callgraph.py): 12 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee end_unit_turn; callee feature_get; callee find_close_village; callee get_treaty), the runner-up gets 10, and the arity agrees (0). Its rival build_road (10) is orders_road in the same batch: the Mac orders_road ties the two at 10, and orders_clear breaks the tie here.
 */

#include "colonization.h"

#include "units.h"
#include "terrain.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern GameState g_game;   /* SEG20:0x7782 */

extern int g_popup_tribe;             /* DGROUP:0x2518, declared */

extern int land_region_at(int x, int y);
extern unsigned char feature_get(int x, int y);

extern int popm(char *msg, int n);
extern int is_colony(int x, int y);
extern int find_close_village(int x, int y, int nation, int owner);
extern int tribe_radius(int tribe);
extern int get_treaty(int nation_a, int nation_b);

extern void popup_say(int slot, int word);
extern int village_bribe(int kind, int nation, int x, int y);

extern int Popup_exec(Popup *popup, char *text);
extern long check_gold(int nation);
extern void Popup_grey(Popup *p, int key, int on);
extern void end_unit_turn(int u);
extern void subtract_gold(int nation, long amount);
extern void feature_set(int x, int y, int mask, int on);
extern int popi(char *msg, int tribe);
extern void perform_orders_clear(int u);

/* The module's one-line wrappers are defined here because the shipped calls to them are
 * the compiler's own `push cs / call `; show_plain_dialog_1008_cd53's own callee is not here. */

extern "C" void orders_clear(void)
{
    Popup popup(0x800);
    int u;
    int s;
    int lvl;
    int price;
    int r;
    int terrain;
    int region;
    int x, y;

    u = g_game.selected_unit;
    x = g_units[u].x;
    y = g_units[u].y;
    region = land_region_at(x, y);
    terrain = terrain_at(x, y);
    if (feature_get(x, y) & MAP1_PLOWED) {
        popm("NOPLOW", 3);
        goto done;
    }
    if (is_colony(x, y) >= 0)
        goto build;
    s = find_close_village(x, y, -1, region);
    if (s < 0)
        goto build;
    lvl = tribe_radius(g_cur_tribe);
    if (g_nearest_dist <= lvl) {
        if (get_treaty(g_cur_tribe_nation, g_game.turn_nation) & 0x40) {
            if (!(feature_get(x, y) & MAP1_CLAIM_SETTLED)) {
                if ((terrain >= TERRAIN_FOREST_FIRST && terrain < TERRAIN_FOREST_2_FIRST) || (terrain >= TERRAIN_FOREST_2_FIRST && terrain < TERRAIN_ARCTIC)) {
                    popup_say(0, get_adjective(g_cur_tribe_nation));
                    price = village_bribe(g_cur_settlement, g_game.turn_nation, x, y);
                    if (price <= 0)
                        goto build;
                    set_text_arg_i_1008_cda6(1, price);
                    show_plain_dialog_1008_cd53(&popup, "INDIANFOREST", 0);
                    if (check_gold(g_game.turn_nation) < price)
                        Popup_grey(&popup, 1, 1);
                    g_popup_tribe = g_cur_tribe;
                    r = popup_run_thunk_1008_cd83(&popup);
                    if (r == 1) {
                        g_units[u].orders = ORDERS_NONE;
                        end_unit_turn(u);
                        goto done;
                    }
                    if (r == 2) {
                        feature_set(x, y, MAP1_CLAIM_SETTLED, 1);
                        g_cur_tribe_rec->count++;
                        subtract_gold(g_game.turn_nation, price);
                        popi("INDIANBRIBE", g_cur_tribe);
                    }
                }
            }
        }
    }
build:
    g_units[u].orders = ORDERS_PLOW;
    perform_orders_clear(u);
done: ;
}
