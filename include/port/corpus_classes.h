/* The classes the byte-matching corpus never put in a shared header: every file that used one
 * defined it for itself, identically (42 of 43 C++ copies, 61 of 63 C views). The port defines
 * each once, the C view with the class's own member types (the corpus's spelled the port, the
 * sprites and the arena as bytes, which are not their sizes here); tools/gen_headers.py widens the members and writes them into include/gen/classes.h.
 * Written by the W1 import of the per-file copies; edit here, not in the files.
 */
#ifndef PORT_CORPUS_CLASSES_H
#define PORT_CORPUS_CLASSES_H

#ifdef __cplusplus
struct PopupItem;
struct PopupSprite;
struct PopupEdit;
struct PopupLine;
class SRadioSet;
class SCheckBox;
class SEditBox;
class SButton;
class SListBox;
class SRegion;
class Popup {
public:
    GameWin *win;
    GRPort *dest_port;
    GRPort port;
    Sprite portrait;
    Sprite tile;
    Font *font;
    int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button;
    char *line[64];
    int len[64];
    int line_x[64];
    int line_y[64];
    int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w;
    char *title;
    struct PopupItem *sel_item;
    struct PopupSprite *first_sprite;
    struct PopupItem *items;
    struct PopupItem *last_item;
    struct PopupLine *lines;
    struct PopupSprite *sprites;
    struct PopupSprite *cur_sprite;
    int sprite_wraps, sprites_h;
    struct PopupEdit *edit_items;
    long on_click, draw_sprite;
    MemBlock arena;
    SRadioSet *radios;
    SCheckBox *checks;
    SEditBox *edit_boxes;
    SButton *buttons;
    SListBox *listbox;
    SRegion *regions;
    char *extra_labels[3];
    ButtonSlot btn[6];
    Popup(int n);
    ~Popup();
};

#else

/* the class, for C: the same members and types, so both languages lay it out alike */
typedef struct {
    void *win;
    void *dest_port;
    GRPort port;
    Sprite portrait;
    Sprite tile;
    void *font;
    int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button;
    char *line[64];
    int len[64];
    int line_x[64];
    int line_y[64];
    int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w;
    char *title;
    struct PopupItem *sel_item;
    struct PopupSprite *first_sprite;
    struct PopupItem *items;
    struct PopupItem *last_item;
    struct PopupLine *lines;
    struct PopupSprite *sprites;
    struct PopupSprite *cur_sprite;
    int sprite_wraps, sprites_h;
    struct PopupEdit *edit_items;
    long on_click, draw_sprite;
    MemBlock arena;
    void *radios;
    void *checks;
    void *edit_boxes;
    void *buttons;
    void *listbox;
    void *regions;
    char *extra_labels[3];
    ButtonSlot btn[6];
} Popup;

#endif
#endif
