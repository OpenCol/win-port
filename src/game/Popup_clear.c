/* Popup_clear: candidate for 1050:0131 (657 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup_clear
 * name-source: sibling-build
 *
 * Puts a Popup's bookkeeping back to its defaults: the two longs at the front and some forty
 * fields cleared, the -1 sentinels at +0x72/+0x74 and +0x32c/+0x32e set, the owner at
 * DGROUP:0x24f0 stored through `Popup_set_font`, the four default COLOUR bytes (text, highlight, and two more -- not margins) taken from
 * DGROUP:0x24f8..0x24fb, the fixed spacing 2, 2, 5, 4 written at +0xa0..+0xa6, and the popup
 * enabled at +0x84. `Popup::Popup(int)` calls it twice, before and after allocating the entry
 * block.
 *
 * THE BODY WAS ALREADY WRITTEN, in `Popup_ctor.c`, as the in-file stub the shipped `PUSH CS /
 * CALL NEAR` forced that file to carry -- but a stub's bytes are never compared, because
 * `--symbol` slices the constructor out. Graded here at its own address, the transcription is
 * right to the byte, so what was an unverified reading is now a verified one.
 *
 * Most of the zeroing is written as chained assignment -- `a = b = c = 0` -- which is what the
 * shipped's single `XOR AX,AX` followed by several stores says; one store per statement would
 * re-zero AX each time.
 *
 * RENAMED 2026-09-26 from popup_reset: the Mac build's Popup::clear(void): this only, putting the popup's bookkeeping back to its defaults (the font set again through Popup_set_font). It sits in the Popup module (1050) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern void Popup_set_font(Popup *popup, Font *owner);
extern int heap_create(MemBlock *heap, int kind, unsigned size);

extern "C" void Popup_clear(Popup *popup)
{
    popup->win = 0;
    popup->dest_port = 0;
    popup->framed = 0;
    Popup_set_font(popup, g_popup_font);
    popup->n_lines = popup->n_edits = popup->n_items = popup->n_sprites = 0;
    popup->n_buttons = popup->n_extra = 0;
    popup->flags = 0;
    popup->pos_x = popup->pos_y = -1;
    popup->org_x = popup->org_y = 0;
    popup->box_w = popup->box_h = 0;
    popup->win_x = popup->win_y = popup->win_w = popup->win_h = 0;
    popup->box_x = popup->box_y = 0;
    popup->req_x = popup->req_y = -1;
    popup->min_height = 0;
    popup->label_w = 0;
    popup->f98 = g_popup_m3;
    popup->text_color = g_popup_text_color;
    popup->hilite_color = g_popup_hilite_color;
    popup->f94 = g_popup_m2;
    popup->item_w = popup->width = popup->sprite_w = popup->f35c = popup->edit_w = 0;
    popup->sel_item = 0;
    popup->first_sprite = 0;
    popup->items = popup->last_item = 0;
    popup->lines = 0;
    popup->sprites = 0;
    popup->edit_items = 0;
    popup->f9c = 0;
    popup->pad_y = 2;
    popup->pad_x = 2;
    popup->gap_y = 5;
    popup->gap_x = 4;
    popup->item_margin = 0;
    popup->cols = 1;
    popup->title = 0;
    popup->radios = 0;
    popup->checks = 0;
    popup->edit_boxes = 0;
    popup->buttons = 0;
    popup->listbox = 0;
    popup->regions = 0;
    popup->draw_sprite = 0;
    popup->on_click = 0;
    popup->result = 0;
    popup->extra_button = 0;
}
