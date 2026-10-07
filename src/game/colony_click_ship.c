/* colony_click_ship: candidate for 1010:674c (984 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: colony_click_ship
 * name-source: sibling-build
 * size-source: the census says 622, which is where Ghidra stopped following code. The
 *   function runs to 0x6b24: its two CS-relative switch tables sit at 0x6b0e and 0x6b18,
 *   inside the function and past the `RETF`.
 *
 * The menu that opens when a ship on the colony dock is clicked. `COLONYUNIT` titles it and
 * `SHIPOPTIONS` supplies the six item captions, read a line at a time; clicking a unit that
 * `nth_cargo_vessel` will not give back does nothing.
 *
 * Five of the six items are conditional, and each condition is a different question: item 1
 * needs a positive DGROUP:0xc16, items 2 and 3 the orders byte NOT already 0 or 1, item 4
 * the orders not 5 or 6 and the type not 0xc, item 5 some cargo aboard. Item 6 is always
 * offered. The picks then set orders 0, 1 or 5, and item 5 unloads the hold one slot at a
 * time until `attempt_cargo_to_colony` refuses.
 *
 * A TREASURE TRAIN prices itself into the title: type 0xa appends " (" + `say_bucks` of
 * `Unit+0x17 * 100` + ")". The multiply is done in 32 bits (`MOVZX` then `IMUL EAX`), so the
 * value is a long before it is scaled, not after.
 *
 * `g_selected_carrier = g_carrier_highlight = 0` is one chained assignment: the shipped code makes the zero once and
 * stores it twice through DX and AX, where two statements give two `MOV WORD [mem],0`.
 *
 * `attempt_cargo_to_colony` is a plain call (`9A`) even though it is in this segment, so it is
 * `extern` and not an in-file stub -- the four-byte `push cs / call ` form is what a
 * same-object callee gets, and this is not one.
 *
 * NAME: the Macintosh build's `colony_click_ship(void)` -- no arguments, which
 * `tools/signatures.py` confirms from the call site, and the `SHIPOPTIONS` resource agrees
 * with the word. Recorded as ADOPTED: the descriptive name from the bytes alone was
 * `colony_click_unit`, and the Macintosh changed it, so this is not independent
 * corroboration by any reading (§117).
 *
 * FILED FIRST AT 622 BYTES AND WRONG. It graded RELOCATABLE there, because `bytematch`
 * compares only as many bytes as it is told; the size ratchet in `re-baseline.sh` caught it
 * within the hour and the extra 362 bytes turned up two calls the reconstruction did not
 * have at all -- `win_enable_1010_e4d4` and `win_show_and_focus_1010_e4ff`, both on the exit path.
 */

#include "colonization.h"

#include "units.h"
#include "sounds.h"
#include "w16types.h"
typedef struct { short style; void *ctable; void *win; short fa; char disabled; } Screen;
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

typedef struct { short style; void *ctable; void *win; short fa; char disabled; void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; struct DlgControl *first_ctl; struct DlgControl *ok_btn; struct DlgControl *cancel_btn; char keyboard_nav; char f99; } DialogWin;   /* DialogWin : EventWin, flattened for C */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;        /* SEG20:0x5ca, declared */
extern GameState g_game;   /* SEG20:0x7782 */

extern void Popup_item(Popup *popup, char *s, int id);
extern void popup_say(int slot, int v);
extern int nth_cargo_vessel(int carrier);
extern int unit_is_active(int u);
extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);

extern char *text_get(void);
extern void expand_text_template(char *src, char *dst);
extern int text_open(void *unused, char *id);
extern void Popup_sprite(Popup *popup, Sprite *art, int u, char *s);
extern int UnitIcon(int u);
extern void Popup_set_active(Popup *popup, int id);
extern void move_to_top(int u);
extern void sound_play(int n);
extern int get_cargo(int u, int slot);
extern int attempt_cargo_to_colony(int u, int type, int n);

extern "C" void win_disable_1010_e459(void *ctx);
extern "C" void win_enable_1010_e4d4(void *ctx);

extern "C" void colony_change_mode(void);

void colony_click_ship(void)
{
    int ev;
    int i;
    int type;
    long treasure;
    char *line;
    char buf[0x50];
    Popup p(0x800);
    register int show;
    register int u;

    win_disable_1010_e459(&g_colony_win_rec);
    u = nth_cargo_vessel(g_selected_carrier);
    if (u < 0)
        goto out;
    if (unit_is_active(u))
        g_game.selected_unit = u;
    popup_say(0, g_unit_types[g_units[u].type].name);
    buf[0] = 0;
    if (g_units[u].type == UNIT_TREASURE_TRAIN) {
        treasure = (unsigned char)g_units[u].spec;
        treasure = treasure * 100;
        space_string(buf);
        open_string(buf);
        say_bucks(buf, treasure);
        close_string(buf);
    }
    set_name_slot_1010_e658(1, buf);
    show_plain_step_1010_e484(&p, g_game_txt, "COLONYUNIT");
    p.flags |= 1;
    p.flags |= 2;
    if (text_open(g_game_txt, "SHIPOPTIONS") != 0)
        goto out;
    Popup_sprite(&p, &g_icon_sprites[UnitIcon(u)], 0, 0);
    for (i = 1; i <= 6; i++) {
        line = text_get();
        expand_text_template(line, buf);
        switch (i) {
        case 1: show = (g_selected_carrier > 0); break;
        case 2: show = (g_units[u].orders != ORDERS_NONE); break;
        case 3: show = (g_units[u].orders != ORDERS_SENTRY); break;
        case 4: show = !(g_units[u].orders == ORDERS_FORTIFY || g_units[u].orders == ORDERS_FORTIFIED
                         || g_units[u].type == UNIT_WAGON_TRAIN); break;
        case 5: show = g_units[u].cargo_count; break;
        case 6: show = 1; break;
        }
        if (show != 0)
            Popup_item(&p, buf, i);
    }
    Popup_set_active(&p, 6);
    ev = popup_run_thunk_1010_e4b1(&p);
    ev--;
    switch (ev) {
    case 1:
        move_to_top(u);
        g_tile_first_unit = u;
        g_selected_carrier = g_carrier_highlight = 0;
        break;
    case 2:
        g_units[u].orders = ORDERS_NONE;
        break;
    case 3:
        g_units[u].orders = ORDERS_SENTRY;
        break;
    case 4:
        g_units[u].orders = ORDERS_FORTIFY;
        sound_play(SOUND_COCKGUN);
        break;
    case 5:
        while (g_units[u].cargo_count != 0) {
            type = get_cargo(u, 0);
            if (attempt_cargo_to_colony(u, type, 0))
                break;
        }
        break;
    }
    colony_change_mode();
out:
    win_disable_1010_e459((Screen *)&g_main_win);
    win_enable_1010_e4d4(&g_colony_win_rec);
    win_show_and_focus_1010_e4ff(&g_colony_win_rec);
}

