/* game_menu_debug: candidate for 1048:baf0 (242 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_menu_debug
 *
 * The Cheat entry that edits the seven debug-display bits at DGROUP:0xb6. Each bit goes
 * into a popup checkbox, the dialog runs, and the word is rebuilt from the checkboxes --
 * cleared to zero first, so a bit only survives if its box came back set.
 *
 * The rebuild is `(get(i) ? 1 : 0) << i` and not `get(i) << i`: `Popup_get_check` returns
 * whatever it returns, and the shipped code normalises it to 0 or 1 with the
 * `MOV AX,1 / JMP / XOR AX,AX` pair before shifting.
 *
 * A local Popup, so the function carries the exception block; see dest_get for the
 * pattern.
 *
 * RENAMED 2026-09-26 from debug_info_flags: the Mac build's game_menu_debug(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller MainMenuProc; callee get_check; callee set_check), the runner-up gets 2, the arity agrees (0), and the Mac body is 0.92x this one's size (bound pairs: median 1.01, 80% within 0.76-1.83). The margin is small, so the binding also rests on the names describing the same behaviour.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern void Popup_set_check(Popup *p, int key, int on);
extern int Popup_get_check(Popup *p, int key);

void game_menu_debug(void)
{
    Popup p(0x800);
    int i;

    show_text_dialog_1048_e35c(&p, "DEBUGOPTIONS");
    for (i = 0; i < 7; i++)
        Popup_set_check(&p, i, (1 << i) & g_debug_flags);
    popup_run_thunk_1048_de13(&p);
    g_debug_flags = 0;
    for (i = 0; i < 7; i++)
        g_debug_flags |= (Popup_get_check(&p, i) ? 1 : 0) << i;
}

