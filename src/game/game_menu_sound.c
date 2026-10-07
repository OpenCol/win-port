/* game_menu_sound: candidate for 1048:b1b2 (421 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_menu_sound
 *
 * The `SOUNDOPTIONS` dialog: three checkboxes over the words at DGROUP:0xba, 0xb8 and 0xbc,
 * read in, edited, and written back.
 *
 * THE RESOURCE NAMES THEM and two were wrong here. SOUNDOPTIONS lists exactly three options in
 * slot order -- "~Background Music", "~Event Music", "~Sound Effects" -- and the code fills slot
 * 0 from 0xba, slot 1 from 0xb8 and slot 2 from 0xbc. So 0xb8 is the EVENT MUSIC setting and 0xbc
 * is SOUND EFFECTS; they were called `g_sound_fx` and `g_sound_speech`, which had the second one
 * describing the third and invented a speech setting the game does not have. Colonization has no
 * speech option and this dialog is the whole of its sound configuration.
 *
 * The three settings are then MIRRORED into bits 1, 2 and 3 of SEG20:0x7788, which is
 * cleared with `&= ~0xe` first. So the same three booleans live in two places: the DGROUP
 * words the dialog edits and a flags word the rest of the game reads.
 *
 * Turning music on starts it -- `soundQueue()` reports whether something is already
 * playing and `LoadTray()` begins if not -- and `sound_play(SOUND_STOP_MUSIC)` closes the CD
 * whenever ANY of the three is off, which is the `||` at the end and not an `&&`.
 *
 * RENAMED 2026-09-26 from sound_options: the Mac build's game_menu_sound(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller MainMenuProc; callee get_check; callee set_check; callee sound_play), the runner-up gets 3, and the arity agrees (0). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "sounds.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern GameState g_game;   /* SEG20:0x7782 */

extern void Popup_set_check(Popup *p, int key, int on);
extern int Popup_get_check(Popup *p, int key);
extern int soundQueue(void);
extern void LoadTray(void);
extern void sound_play(int n);

void game_menu_sound(void)
{
    Popup p(0x800);

    show_text_dialog_1048_e35c(&p, "SOUNDOPTIONS");
    Popup_set_check(&p, 0, g_sound_music);
    Popup_set_check(&p, 1, g_sound_event_music);
    Popup_set_check(&p, 2, g_sound_effects);
    popup_run_thunk_1048_de13(&p);
    g_sound_music = Popup_get_check(&p, 0);
    g_sound_event_music = Popup_get_check(&p, 1);
    g_sound_effects = Popup_get_check(&p, 2);
    g_game.hint_flags &= ~0xe;
    if (g_sound_music != 0)
        g_game.hint_flags |= 2;
    if (g_sound_event_music != 0)
        g_game.hint_flags |= 4;
    if (g_sound_effects != 0)
        g_game.hint_flags |= 8;
    if (g_sound_music != 0 && soundQueue() == 0)
        LoadTray();
    if (g_sound_event_music == 0 || g_sound_music == 0 || g_sound_effects == 0)
        sound_play(SOUND_STOP_MUSIC);
}

