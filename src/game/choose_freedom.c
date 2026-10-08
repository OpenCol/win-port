/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 128 of the imported file. */
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
/* PORT: this file's own EventWin, GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* EventWin : Screen, EventTable, flattened for C: 0x8c bytes */
#include "records.h"
#include "classes.h"
#include <stddef.h>
class DlgControl;
template <class T> class LinkedList : public VList { public: LinkedList(); };
class PediaWin : public ColChild { public: short category; SImageButton button; SScrollBar bar; GRPort port; LinkedList<PediaEntry> list; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; PediaWin(); ~PediaWin(); };
typedef char pedia_list_at_0x83e[(sizeof(void *) != 4 || offsetof(PediaWin, list) == 0x83e) ? 1 : -1];   /* PORT: the game's offset where a pointer is 4 bytes (W9) */

#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
/* 6 bytes at SEG20:0xd6fa */

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;     /* SEG20:0x5ca, declared */
extern char g_pedia_win;    /* SEG20:0x387c, declared */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern void set_europe(int n);
extern int has_father(int nation, int bit);

extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);
extern void force_mood(int mood);
extern int Popup_help_me(Popup *popup);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int article, void *parent);   /* PORT: C linkage, as defined (W2) */

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
    show_plain_dialog_1020_a8d3(&p, (void*)("WHICHFREEDOM"), 0);
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

