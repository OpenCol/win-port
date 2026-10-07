/* PORT: this file's own declarations of SEditBox_Init, obj_forward_and_count, radio_double_click, radioset_init, set_initial_focus, sprite_click removed: the header declares them from the definition (W1 step 3). */
/* Popup_make_controls: candidate for 1050:35aa (3075 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: Popup_make_controls
 * name-source: sibling-build
 *
 * EVERY CONTROL A POPUP CAN HAVE IS BUILT HERE, in one pass, from the description the caller left
 * in the popup: four linked lists of items, regions and edit fields, and a table of button slots.
 * `Popup_close` is the other end of it.
 *
 * THE ITEM LIST BECOMES ONE OF THREE THINGS, chosen by two bits of the flags word. Bit 12 makes a
 * single LIST BOX and every item is forwarded into it; bit 2 makes an array of CHECK BOXES, one
 * per item, stacked by the row height and each given the item's own flags; and with neither bit
 * the items become a RADIO GROUP -- an image button laid out in as many rows as the widest item
 * needs, which is `(cols + count - 1) / max(cols, 1)`.
 *
 * ONLY THE RADIO PATH COLLECTS THE LABELS, into a 41-slot array on the stack, and it is the only
 * path that can overrun it -- which is why the count is clamped to 40 first, in the same `if` that
 * fills the array. But the fill walks the whole item list, not the clamped count, so the clamp does not bound it: a list of more than 41 items still overruns the array.
 *
 * THE RADIO PATH ALSO EDITS THE ITEMS' OWN TEXT. An item with flag bit 0 set (the bit the check-box path treats as DISABLED) gets "!" put in front of it by
 * concatenating the marker and the text into a scratch buffer, TRUNCATING the item's string in
 * place and copying the whole thing back -- so the marker is permanent, the item's buffer has to
 * have room for it, and building the popup twice would prepend it twice. The 0x200 flag set on the
 * last line is what stops that: it is tested on the first line and the function does nothing at all
 * on a second call.
 *
 * BUTTONS COME FROM TWO SOURCES. The first `n_buttons` take their handler from the global table at
 * SEG31:0x98 indexed by the slot's own command number; the rest, up to three, take theirs from the
 * popup's own array. Every one gets `button_click` as its click handler regardless, so the
 * per-button handler is what the dialog does and the click handler is how it hears about it.
 *
 * EVERY ALLOCATION IS FOLLOWED BY A BARE `return` on failure, which leaves the popup half-built
 * with the 0x200 flag still clear -- so a later call tries again from the beginning, on a popup
 * that already has some of its controls.
 *
 * WHAT THE BYTES SETTLED: `&one` is passed with the comma form `(one = 1, &one)`, which puts the
 * store before the `LEA` and the `PUSH SS` after it (rule 61); `max_ptr_1050_97d1` is reached
 * through the five-byte NOP form and so is external where every other callee is a stub (rule
 * 47); the region loop advances its index in the FOR clause and the check-box and edit loops
 * advance theirs in the body, which is one byte of difference each; and the class at +0x3b0 is 0x20
 * bytes, not 0x22 -- the `new` says so. (The constructor it calls, 1050:860c, carried the name
 * SCheckBox_ctor until 2026-09-28, when tools/check_data_refs.py showed the two constructor files
 * were swapped: 860c is `SListBox::SListBox()` and 86d6, the vector-new constructor of the
 * check-box array here, is `SCheckBox::SCheckBox()`.)
 *
 * RENAMED 2026-09-26 from popup_build_controls: the Mac build's Popup::make_controls(void): this only, creating the popup's controls after Popup_compute_size has placed them. It sits in the Popup module (1050:0000..50c4) and the width matches.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, DlgControl, SRadioSet, SListBox, SCheckBox, SEditBox, SButton, SRegion, Sprite, Font removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"


class DlgControl;










#include "records.h"
   /* 0x20 bytes: the class, word-aligned (-a), with its pad byte */
typedef struct PopupSprite { int id; int f2; char *text; RECT rect; Sprite *sprite; struct PopupSprite *next; struct PopupSprite *prev; } PopupSprite;

/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern Popup *g_cur_dialog;                   /* DGROUP:0x2500 */


/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void listbox_init(SListBox *listbox, DialogWin *win, int id, RECT *r);
extern "C" void checkbox_init(SCheckBox *checkbox, DialogWin *win, int id, int x, int y, char *text);
extern "C" void set_checked_1050_87a2(SCheckBox *check, int checked);
extern "C" void checkbox_disable(SCheckBox *checkbox);

extern "C" void set_radio_dblclk_1050_88d6(SRadioSet *g, void *h);
extern "C" void set_cur_item_1050_88f0(SRadioSet *g, int cur_item);

extern "C" void sregion_init_1050_894a(SRegion *s, DialogWin *win, int id, RECT *r);
extern "C" void set_region_click_1050_89cd(SRegion *s, void *h);
extern "C" int MSGetTextLength_1050_8391(Font *font, const char *s);
extern "C" int Popup_edit_y_size(Popup *popup);
extern "C" void MSSetEBLimit(SEditBox *edit, int max_chars);
extern "C" int Popup_button_y_size(Popup *popup);
extern "C" void ctl_init_button_1050_8ac8(SButton *button, DialogWin *win, int id, RECT *r, char *title);
extern "C" void set_button_action_1050_8b2c(SButton *button, void *h);

extern "C" void Popup_make_controls(Popup *p)
{
    register int k;
    int n = 0;
    PopupItem *item;
    PopupEdit *edit_item;
    PopupSprite *g;
    RECT r;
    int m1;
    int m2;
    int ox;
    int oy;
    int rows;
    char c[2];
    char *label;
    i16 one;   /* PORT: i16, max_ptr_1050_97d1 takes its address */
    char *labels[0x29];
    char buf[0x80];

    g_cur_dialog = p;
    if (p->flags & 0x200)
        return;
    c[0] = 0;
    set_control_font_1050_8598(p->font);
    set_button_font_1050_85b5(p->font);
    set_default_font_ref(p->font);
    set_editbox_font_1050_85ef(p->font);
    m1 = p->flags & 4;
    m2 = p->flags & 0x1000;
    if (m1 == 0 && m2 == 0) {
        if (p->n_items > 0x28)
            p->n_items = 0x28;
        for (item = p->items; item != 0; item = item->next) {
            labels[n] = item->text;
            n++;
        }
    }
    if (p->n_items != 0) {
    if (m2 != 0) {
        p->listbox = new SListBox;
        if (p->listbox == 0)
            return;
        SetRect(&r, p->box_x + p->item_x, p->box_y + p->item_y, p->box_x + p->item_x + p->list_w, p->box_y + p->item_y + p->list_h);
        listbox_init(p->listbox, p->win, 0xa, &r);
        for (item = p->items; item != 0; item = item->next)
            obj_forward_and_count(p->listbox, item->text);
    } else if (m1 != 0) {
        p->checks = new SCheckBox[p->n_items];
        if (p->checks == 0)
            return;
        ox = p->item_x;
        oy = p->item_y;
        k = 0;
        for (item = p->items; item != 0; item = item->next) {
            checkbox_init(&p->checks[k], p->win, k + 0xc8, p->box_x + ox, p->box_y + oy, item->text);
            set_checked_1050_87a2(&p->checks[k], item->flags & 4);
            if (item->flags & 1)
                checkbox_disable(&p->checks[k]);
            oy += p->row_h;
            k++;
        }
    } else {
    p->radios = new SRadioSet;
    if (p->radios == 0)
        return;
    for (item = p->items; item != 0; item = item->next) {
        if (item->flags & 1) {
            buf[0] = 0;
            STRCAT(buf, "!");
            STRCAT(buf, item->text);
            *item->text = 0;
            STRCAT(item->text, buf);
        }
    }
    rows = (p->cols + p->n_items - 1)
           / *max_ptr_1050_97d1(&p->cols, (one = 1, &one));
    SetRect(&r, p->box_x + p->item_x, p->box_y + p->item_y, p->box_x + p->item_x + p->item_w - 5,
            p->box_y + p->item_y + p->row_h * rows);
    radioset_init(p->radios, p->win, 0xc8, &r, p->cols, p->n_items, labels);
    set_radio_dblclk_1050_88d6(p->radios, (void *)radio_double_click);
    if (p->sel_item != 0)
        set_cur_item_1050_88f0(p->radios, p->sel_item->index);
    }
    }
    if (p->n_sprites > 0 && p->n_items == 0) {
        p->regions = new SRegion[p->n_sprites];
        if (p->regions == 0)
            return;
        ox = p->sprite_x;
        oy = p->sprite_y;
        g = p->sprites;
        for (k = 0; g != 0; g = g->next, k++) {
            SetRect(&r, p->box_x + ox, p->box_y + oy, p->box_x + ox + p->sprite_w,
                    p->box_y + oy + rect_height_1050_83da(g->sprite));
            sregion_init_1050_894a(&p->regions[k], p->win, k + 0x12c, &r);
            set_region_click_1050_89cd(&p->regions[k], (void *)sprite_click);
            oy += rect_height_1050_83da(g->sprite) + p->gap_y;
        }
    }
    if (p->edit_items != 0) {
        edit_item = p->edit_items;
        p->edit_boxes = new SEditBox[p->n_edits];
        if (p->edit_boxes == 0)
            return;
        k = 0;
        for (; edit_item != 0; edit_item = edit_item->next) {
            SEditBox_Init(&p->edit_boxes[k], p->win, 0x14,
                                      p->box_x + p->edit_x + p->label_w + 6,
                                      p->box_y + p->edit_y + Popup_edit_y_size(p) * k,
                                      MSGetTextLength_1050_8391(p->font, "W") * edit_item->maxlen - 0xc,
                                      edit_item->buf);
            MSSetEBLimit(&p->edit_boxes[k], edit_item->maxlen);
            k++;
        }
    }
    if (p->n_buttons != 0) {
        p->buttons = new SButton[p->n_buttons + p->n_extra];
        if (p->buttons == 0)
            return;
    }
    for (k = 0; p->n_buttons + p->n_extra > k; k++) {
        if (p->n_buttons > k)
            label = g_popup_label[p->btn[k].cmd];
        else
            label = p->extra_labels[k - p->n_buttons];
        SetRect(&r, p->box_x + p->btn[k].x - 3, p->box_y + p->button_y - 3,
                p->box_x + p->btn[k].x + p->button_w - 3,
                p->box_y + p->button_y + Popup_button_y_size(p) - 3);
        ctl_init_button_1050_8ac8(&p->buttons[k], p->win, k + 0x64, &r, label);
        set_button_action_1050_8b2c(&p->buttons[k], (void *)button_click);
    }
    if (p->edit_items != 0)
        set_initial_focus(p->edit_boxes);
    else if (p->radios != 0)
        set_initial_focus(p->radios);
    else
        set_initial_focus(&p->buttons[p->n_buttons - 1]);
    link_derived_to_ctl(&p->buttons[p->n_buttons - 1]);
    p->flags |= 0x200;
}
