/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 59, 61 of the imported file. */
/* dest_get: candidate for 1028:ca10 (305 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: dest_get
 *
 * Pops up the current route's stops and returns which one was picked, zero-based, or -1.
 * `cur` pre-selects an entry. The title depends on the route's kind byte at +0x20:
 * `SAILPORT` for a sea route, `TRAVELPLACE` otherwise.
 *
 * The entries are names, not numbers: `dest_name(i)` walks the route's ten-byte stop
 * table at +0x22 and returns Europe's name for the 0x3e7 sentinel and the colony's name
 * otherwise -- the same 0x3e7 that route_edit tests.
 *
 * Two `report_error_1028_e4db` calls with `&g_error_log` and the literals "item set" and "event
 * returned" are a debug trace left in the shipped build, through the same error context
 * GRPort_CopyToPort and stub_reporter use.
 *
 * THE POPUP PATTERN, which this is the first function to use. A `Popup` built on the stack
 * gives the whole shape at once: `sub sp,0x3fe` for a 0x3dc-byte object plus two locals,
 * `__InitExceptBlock_rtl` with a table address, `MOV WORD [bp-0x3f0],6`, and at the end a
 * `Popup_dtor` with flag 2 and `MOV [ss:0x14],ax` to unwind. None of that is written here:
 * it is what `-P` emits for a local object with a destructor, exactly as
 * palette_nearest_index's RGBTree does, and the class tower is copied from Popup_ctor.c.
 *
 * That matters beyond this function. Around a dozen unreconstructed functions in the census
 * open with the same four instructions -- `CheatMenuBlow`, `game_menu_debug`, `game_menu_sound`,
 * `DoControlPanel`, `game_menu_options`, `txt_EUROPESHIPCLICK_1048_548a` among them --
 * and they were parked as "C++ with exception blocks". They are a local `Popup` and nothing
 * more.
 *
 * `result` is declared BEFORE the Popup and `i` is `register` (rule 40); the return value
 * lives at bp-0x3fe, below the object, and is pushed across the destructor call.
 *
 * RENAMED 2026-09-26 from route_pick_stop: the Mac build's dest_get(short), bound by call-graph agreement (tools/mac_callgraph.py): 2 of its Mac call edges already bound land on this function (caller orders_route; callee dest_name), the runner-up gets 1, the arity agrees (1), and the Mac body is 0.92x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern void Popup_item(Popup *popup, char *s, int id);
extern void Popup_set_active(Popup *popup, int id);

extern "C" void report_error_1028_e4db(char *ctx, char *s, long v);

extern "C" char *dest_name(int i);

int dest_get(int cur)
{
    int result = -1;
    Popup p(0x800);
    register int i;
    int ev;

    if (g_cur_route_rec->sea != 0)
        show_plain_dialog_1028_e43b(&p, (void*)("SAILPORT"), 0);
    else
        show_plain_dialog_1028_e43b(&p, (void*)("TRAVELPLACE"), 0);
    p.flags |= 1;
    for (i = 0; i < g_cur_route_rec->count; i++) {
        Popup_item(&p, dest_name(i), i + 1);
        report_error_1028_e4db(&g_error_log, "item set", (long)(i + 1));
        if (i == cur)
            Popup_set_active(&p, i + 1);
    }
    ev = popup_run_thunk_1028_e46b(&p);
    report_error_1028_e4db(&g_error_log, "event returned", (long)ev);
    if (ev > 0)
        result = ev - 1;
    return result;
}

