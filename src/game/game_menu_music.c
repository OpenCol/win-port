/* game_menu_music: candidate for 1048:af9a (506 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: game_menu_music
 * size-source: the census says 218, which stops where Ghidra stopped following code. The
 *   function runs to 0xb195: two jump tables live inside it, at 0xb141 and 0xb15d.
 *
 * The music menu. `PICKMUSIC` lists fourteen entries; the current track at DGROUP:0x1d12
 * decides which is pre-ticked, and the pick becomes the new track.
 *
 * TWO SWITCHES THAT ARE EACH OTHER'S INVERSE. The first maps a track number to a menu item,
 * the second a menu item back to a track, and both compile to jump tables -- 28 entries at
 * 0xb15d and 14 at 0xb141. Reading the tables out of the image is what made this writable:
 * they say which track numbers share a menu item, and the two agree everywhere.
 *
 * Three menu items are submenus rather than tracks. Item 12 asks `PICKINDEPENDENCE` and the
 * answer picks a track at +0x28; item 13 asks `PICKMILITARY` at +0x2d; item 14 asks
 * `PICKINDIAN` at +0x31 -- and that one **skips track 0x34**, via `if (n > 2) n++`. The
 * first table proves it independently: tracks 0x32, 0x33, 0x35 and 0x36 all map back to
 * item 14 and 0x34 maps to the default. Two encodings of the same gap, in different halves
 * of the function.
 *
 * THE CASE BODIES MUST BE WRITTEN IN THE ORDER THE OBJECT LAYS THEM OUT, which is the order
 * of the VALUES assigned, not of the case labels: items 1..8 come from tracks 0x20..0x27,
 * then item 9 from 0x39, 10 from 0x38, 11 from 0x3b, then the three submenu ranges. Sorting
 * the first switch by case label instead compiles cleanly and lays the bodies out wrongly.
 *
 * RULE 12 HELD AND I DOUBTED IT, which cost two compiles. The shipped `JG +3 / JMP `
 * over 174 bytes says `goto`, and the rule says so at any distance -- `-3` gives Borland a
 * 386 long conditional jump, so it never needs the two-instruction form for a structured
 * `if`. My first attempt had the form right and the LABEL in the wrong place, one block too
 * early; rewriting it as a positive `if` produced exactly the single `0F 8E` the rule
 * predicts, which should have been read as the rule confirming itself rather than as a
 * distance caveat. The fix was to move `out:` past the `if (track != 0)` block.
 *
 * RENAMED 2026-09-26 from jukebox_by_song: the Mac build's game_menu_music(void), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller MainMenuProc; callee hilite; callee sound_play), the runner-up gets 2, and the arity agrees (0). The margin is one edge, so the binding also rests on the two names describing the same behaviour: the descriptive name here was derived from the body before the Mac table was consulted.
 */

#include "colonization.h"

#include "sounds.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };

extern void Popup_hilite(Popup *p, int key, int on);
extern void sound_play(int n);

extern "C" void win_draw_text_default(Popup *p, char *s, int sel, int n);

void game_menu_music(void)
{
    register int pick = 0;
    int track = 0;
    Popup p(0x800);

    switch (g_cur_track) {
    case SOUND_MUSIC_BIRD_SONG: pick = 1; break;
    case SOUND_MUSIC_SMOKY_TUNE: pick = 2; break;
    case SOUND_MUSIC_CORNWALL: pick = 3; break;
    case SOUND_MUSIC_SHADY_GROVE: pick = 4; break;
    case SOUND_MUSIC_FIDDLERS_DANCE: pick = 5; break;
    case SOUND_MUSIC_JINE_THE_CAVALRY: pick = 6; break;
    case SOUND_MUSIC_JOE_CLARK: pick = 7; break;
    case SOUND_MUSIC_LITTLE_FIDDLE: pick = 8; break;
    case SOUND_MUSIC_HORNPIPE: pick = 9; break;
    case SOUND_MUSIC_BONNY_MORN: pick = 10; break;
    case SOUND_MUSIC_NIGHTINGALE: pick = 11; break;
    case 0x28: case SOUND_MUSIC_LOVE_FOREVER: case SOUND_MUSIC_YORK_FUSILIERS: case SOUND_MUSIC_WASHINGTON_ARTILLERY_MARCH: case SOUND_MUSIC_ROAD_TO_BOSTON: case SOUND_MUSIC_INDEPENDENCE_WAY: pick = 12; break;
    case SOUND_MUSIC_THE_REVEILLE: case SOUND_MUSIC_SUCCESSFUL_CAMPAIGN: case SOUND_MUSIC_MORELLIS_LESSON: case SOUND_MUSIC_TO_ARMS: pick = 13; break;
    case SOUND_MUSIC_INDIAN_VICTORY: case SOUND_MUSIC_NATIVES: case SOUND_MUSIC_TENOCHTITLAN: case SOUND_MUSIC_PIZARRO_AT_CUZCO: pick = 14; break;
    }
    win_draw_text_default(&p, "PICKMUSIC", pick - 1, 0);
    if (pick != 0)
        Popup_hilite(&p, pick, 1);
    pick = popup_run_thunk_1048_de13(&p);
    if (pick <= 0)
        goto out;
    switch (pick) {
    case 1: track = SOUND_MUSIC_BIRD_SONG; break;
    case 2: track = SOUND_MUSIC_SMOKY_TUNE; break;
    case 3: track = SOUND_MUSIC_CORNWALL; break;
    case 4: track = SOUND_MUSIC_SHADY_GROVE; break;
    case 5: track = SOUND_MUSIC_FIDDLERS_DANCE; break;
    case 6: track = SOUND_MUSIC_JINE_THE_CAVALRY; break;
    case 7: track = SOUND_MUSIC_JOE_CLARK; break;
    case 8: track = SOUND_MUSIC_LITTLE_FIDDLE; break;
    case 9: track = SOUND_MUSIC_HORNPIPE; break;
    case 10: track = SOUND_MUSIC_BONNY_MORN; break;
    case 11: track = SOUND_MUSIC_NIGHTINGALE; break;
    case 12:
        pick = ask_text_resource("PICKINDEPENDENCE");
        if (pick != 0)
            track = pick + 0x28;
        break;
    case 13:
        pick = ask_text_resource("PICKMILITARY");
        if (pick != 0)
            track = pick + 0x2d;
        break;
    case 14:
        pick = ask_text_resource("PICKINDIAN");
        if (pick > 2)
            pick++;
        if (pick != 0)
            track = pick + 0x31;
        break;
    }
    if (track != 0) {
        g_cur_track = track;
        sound_play(track);
    }
out:
    ;
}

