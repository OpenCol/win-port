/* popup_ask: candidate for 1050:60f8 (280 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: popup_ask
 *
 * `popup_ask_nocancel` without the two `popup.kind &= ~1` stores, 0x134 bytes above it
 * and otherwise the same program: parse the directives, run the dialog, and go round again --
 * re-parsing from the top and restoring the three globals at DGROUP:0x2518, 0x251a and 0x251c
 * -- for as long as the answer in SEG31:0xa4 comes back EMPTY.
 *
 * The empty string it compares against is at DGROUP:0x25ef and its twin's is at 0x25f0: the
 * compiler laid down TWO separate `""` literals rather than pooling them, which is visible in
 * the data and not in either function's bytes -- the offset is a fixup the comparator masks.
 *
 * 280 bytes, not the gap's 308: the 28 bytes after it belong to the NEXT function's exception
 * descriptor, which Borland places in front of the function that uses it.
 *
 * RENAMED 2026-09-26 from prompt_text_loop: the Mac build's popup_ask(char*, char*, int, char*), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller perform_orders_build; caller route_create; callee exec), the runner-up gets 2, and the arity agrees (4). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 * Popup::Popup, Popup_start, Popup_exec and Popup::~Popup are defined in-file first, for the 4-byte calls.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern int g_popup_tribe;                    /* DGROUP:0x2518, NEAR in this module */

extern int g_popup_nation;                    /* DGROUP:0x251c */
/* Two empty strings sit next to each other: DGROUP:0x25ef and DGROUP:0x25f0 are both
 * a lone NUL, and the rest of the corpus uses the second. This file uses the first --
 * the shipped immediate says 0x25ef -- so it needs its own name, because one name for
 * two addresses is what tools/check_global_names.py refuses. */

extern "C" int Popup_start(Popup *p, void *ctx, char *name, int sel, char *dflt, GameWin *win, GRPort *port, RECT *rect, int kind);
extern "C" int Popup_exec(Popup *popup, char *buf);

extern "C" int popup_ask(void *ctx, void *name, int max_len, void *dflt)
{
    register int sel = 0;
    Popup popup(0x800);
    int s2518;
    int s251c;
    int s251a;

    s2518 = g_popup_tribe;
    s251c = g_popup_nation;
    s251a = g_popup_dude;
    if (Popup_start(&popup, ctx, (char *)name, max_len, (char *)dflt, 0L, 0L, 0L, 1) != 0)
        goto done;
again:
    sel = Popup_exec(&popup, g_name_slots[0]);
    if (sel != -1) {
        if (STRCMP(g_name_slots[0], "") == 0) {
            if (Popup_start(&popup, ctx, (char *)name, max_len, (char *)dflt, 0L, 0L, 0L, 1) != 0)
                goto done;
            g_popup_tribe = s2518;
            g_popup_nation = s251c;
            g_popup_dude = s251a;
            goto again;
        }
    }
done:
    return sel;
}
