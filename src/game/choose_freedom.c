/* choose_freedom: candidate for 1020:a27a (838 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: choose_freedom
 *
 * Chooses the nation's next Founding Father, one candidate per category. The 6-byte records
 * at SEG20:0xd6fa hold a name word, a category byte at +2 and THREE weights at +3 -- one per
 * era, which `Century` selects from the year (0 before 1600, 1 from 1600, 2 from 1700). For each of the five categories
 * the eligible fathers' weights are summed, `rand_range(1, total)` draws, and the draw is
 * walked down until it runs out: a weighted pick with the weights stored per rank, so the
 * same father is likelier in one era than another.
 *
 * The HUMAN is then offered the five candidates by `WHICHFREEDOM`, each line naming the
 * father, its category and a trailing word; the AI takes the one
 * `best_freedom` already picked. Picking with the pedia modifier held opens the
 * Colonopedia on that father and comes back. `add_liberty` (§116) is what spends the bells
 * that make this happen.
 *
 * Four spellings the bytes fixed.
 *
 * THE HUMAN/AI SPLIT IS AN EARLY `goto` WITH `||`, not a wrapping `if` with `&&`. The
 * shipped `JGE` is SHORT and lands nineteen bytes ahead on a `JMP ` -- the two-jump form
 * -- where `if (nation < 4 && control == 0) { ... }` gives a single `JGE ` over the whole
 * block. The same idiom §120 settled, spelled the other way round.
 *
 * The category test stays 8-bit: `g_fathers[j].cat != (unsigned char)i`, because the shipped
 * compares `AL` against the byte at `[bp-0x3fe]` and the uncast form promotes both sides.
 *
 * Two tests are positive `if`s and not `continue`s -- the availability check at the top of
 * the outer loop and `if (roll <= 0) best = j;` at the bottom of the inner one -- and both
 * are the direct-jump half of rule 12.
 *
 * The exit is an explicit `return;` (rule 41): the unwind restore after the Popup destructor
 * is `MOV DX`, which is what a `return` gives and falling off the end does not.
 *
 * RENAMED 2026-09-26 from pick_founding_father: the Mac build's choose_freedom(short), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller add_liberty; callee help_me; callee item; callee say_string; callee set_europe), the runner-up gets 4, and the arity agrees (1).
 */

#include "colonization.h"

#include "fathers.h"
#include "control.h"
#include "nations.h"
#include "misc.h"
#include "w16types.h"
typedef struct { short style; void *ctable; void *win; short fa; char disabled; void *on_mousemove, *on_lbutton_down, *on_lbutton_up, *on_lclick, *on_rbutton_down, *on_rbutton_up, *on_rclick, *on_ldblclk, *on_keydown, *on_keyup, *on_keydown_after, *on_char, *on_close, *on_minimize, *on_restore, *on_setfocus, *on_size, *on_move, *on_commnotify, *on_hscroll, *on_hthumb, *on_vscroll, *on_vthumb; char f69; void *menu; short min_track_w, min_track_h, max_track_w, max_track_h, running, hpage, vpage; long menu_handler; long command_handler; short ldown, rdown; short hpos, vpos; } EventWin;   /* EventWin : Screen, EventTable, flattened for C: 0x8c bytes */
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */
class GRPort { public: int w; int h; int stride; RECT rect; RECT bounds; unsigned char *bits; long *rows; int current; void *win; GRPort(); ~GRPort(); virtual int ResizePort(RECT *r); };
#pragma option -a
class Sprite { public: RECT bounds; RECT area; int hx, hy; int f14, f16; unsigned char key; short handle; unsigned char *bits; Sprite(); ~Sprite(); };
#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion; class Popup { public: GameWin *win; GRPort *dest_port; GRPort port; Sprite portrait; Sprite tile; Font *font; int req_x, req_y, min_height, n_lines, n_edits, n_items, n_sprites, n_buttons, n_extra, cols, flags, box_w, box_h, list_w, list_h, list_rows, text_color, f94, hilite_color, f98, item_margin, f9c, row_h, pad_y, pad_x, gap_y, gap_x, result, extra_button; char *line[64]; int len[64]; int line_x[64]; int line_y[64]; int pos_x, pos_y, org_x, org_y, win_x, win_y, win_w, win_h, box_x, box_y, framed, item_x, item_y, text_x, text_y, sprite_x, sprite_y, edit_x, edit_y, label_w, button_y, item_w, width, sprite_w, f35c, edit_w, button_w; char *title; struct PopupItem *sel_item; struct PopupSprite *first_sprite; struct PopupItem *items; struct PopupItem *last_item; struct PopupLine *lines; struct PopupSprite *sprites; struct PopupSprite *cur_sprite; int sprite_wraps, sprites_h; struct PopupEdit *edit_items; long on_click, draw_sprite; MemBlock arena; SRadioSet *radios; SCheckBox *checks; SEditBox *edit_boxes; SButton *buttons; SListBox *listbox; SRegion *regions; char *extra_labels[3]; ButtonSlot btn[6]; Popup(int n); ~Popup(); };
/* 6 bytes at SEG20:0xd6fa */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;     /* SEG20:0x5ca, declared */
extern char g_pedia_win;    /* SEG20:0x387c, declared */

extern void Popup_item(Popup *popup, char *s, int id);
extern void set_europe(int n);
extern int has_father(int nation, int bit);

extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);
extern void force_mood(int mood);
extern int Popup_help_me(Popup *popup);
extern void PediaWin_ShowModal(void *pedia, int category, int article, void *parent);

extern "C" int Century(void);
extern "C" int avail_of_father(int nation, int cat);
extern "C" int best_freedom(int nation);

void choose_freedom(int nation)
{
    int i;
    int ev;
    int chosen;
    int cand[FATHER_CAT_COUNT];
    int avail = 0;
    int best;
    int rank;
    int total;
    char buf[0x50];
    Popup p(0x800);
    register int j;
    register int roll;

    set_europe(nation);
    rank = Century();
    for (i = 0; i < FATHER_CAT_COUNT; i++) {
        cand[i] = -1;
        total = 0;
        if (avail_of_father(nation, i) != 0) {
            avail++;
            for (j = 0; j < FATHER_COUNT; j++) {
                if (has_father(nation, j))
                    continue;
                if (g_fathers[j].cat != (unsigned char)i)
                    continue;
                total += g_fathers[j].weight[rank];
            }
            roll = rand_range(1, total);
            best = -1;
            for (j = 0; best < 0 && j < FATHER_COUNT; j++) {
                if (has_father(nation, j))
                    continue;
                if (g_fathers[j].cat != (unsigned char)i)
                    continue;
                roll -= g_fathers[j].weight[rank];
                if (roll <= 0)
                    best = j;
            }
            cand[i] = best;
        }
    }
    chosen = best_freedom(nation);
    if (nation >= NATION_EUROPEAN_COUNT || g_nation_text[nation].control != CONTROL_HUMAN)
        goto assign;
    if (avail == 0)
        goto out;
    force_mood(3);
restart:
    show_plain_dialog_1020_a8d3(&p, "WHICHFREEDOM", 0);
    if (avail != 0) {
        for (i = 0; i < FATHER_CAT_COUNT; i++) {
            if (avail_of_father(nation, i) == 0)
                continue;
            buf[0] = 0;
            say_string(buf, g_fathers[cand[i]].name);
            space_string(buf);
            open_string(buf);
            say_string(buf, g_founding_word[i]);
            space_string(buf);
            say_string(buf, g_misc_words[MISC_ADVISOR]);
            close_string(buf);
            Popup_item(&p, buf, i);
        }
    }
    p.flags |= 0x40;
    ev = popup_run_thunk_1020_a903(&p);
    if (ev <= 0)
        goto restart;
    chosen = ev - 1;
    if (Popup_help_me(&p) != 0) {
        PediaWin_ShowModal((PediaWin *)&g_pedia_win, 5, cand[chosen], (EventWin *)&g_main_win);
        goto restart;
    }
assign:
    g_cur_nation_rec->next_father = cand[chosen];
out:
    return;
}

