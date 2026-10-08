/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 102, 106, 109 of the imported file. */
/* europe_recruit: candidate for 1048:6e52 (1063 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_recruit
 * name-source: descriptive
 *
 * RECRUITING ONE OF THE THREE COLONISTS WAITING ON THE EUROPE DOCKS, in three flavours that
 * share one menu:
 *   - RECRUIT (both flags 0): the player pays passage;
 *   - RECRUITCHOOSE (`choose`): religious unrest brings an emigrant free and the player picks which.
 *     `europe_new_turn` calls it this way when William Brewster lets the player choose;
 *   - LOSTCITY0 (`youth`): the Fountain of Youth. The rumour handler (1038:7c74) calls it eight
 *     times, one free pick each, and it leaves the crosses alone.
 * Each sets the help context DGROUP:0x251a (2, 4, 3) while the menu is up and clears it to -1.
 *
 * THE PRICE. It starts at `(difficulty + recruits bought) * 20 + 140`, with the count kept in the
 * nation record's +6. It is discounted by the crosses already gathered: `price -= (price - floor) *
 * crosses / (need + 1)`, where the floor is the larger of 100 and a fifth of the price. The result
 * is at least 10, and a free pick costs 0. A line the treasury cannot pay is greyed.
 *
 * THE MENU. It lists the three recruits by profession, JOB_COLONIST_ALT shown as JOB_COLONIST.
 * A paid recruit gets an extra first line, the parenthesised word at SEG20:0xdcce, as item 0x19d.
 * A computer nation takes slot 1 without asking.
 * Picking with the Colonopedia modifier (`Popup_help_me`) opens article 3 on
 * that profession, or on -1 for the extra line, and rebuilds the menu.
 *
 * A PICK creates the unit (`unit_from_docks`). If that fails, nothing more happens -- but for every pick except the youth one the
 * crosses were already zeroed just before the call.
 * Otherwise it takes the price from the gold and refills the slot from `fill_docks(0)`.
 * A paid recruit also steps the purchase count, capped at 180, and refreshes the docks. Every pick
 * except the youth one zeroes the crosses.
 *
 * THE DISCOUNT NEEDS ITS `(int)`: `price -= (int)(long expression)`. Without the cast Borland
 * computes the subtraction in DX and stores it back. With it, it subtracts AX from the variable
 * in place, as the shipped does. The two `max`/`min` template calls take their operands through
 * comma expressions inside the argument list (rule 61, as in `europe_new_taxes`).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "jobs.h"
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

/* 52 bytes at SEG20:0x7840; +1 is CONTROL: 0 human, 1 computer, 2 defeated */

extern GameState g_game;   /* SEG20:0x7782 */

extern char g_pedia_win;                /* SEG20:0x387c */

extern void popup_say(int slot, int word);
extern void open_string(char *s);
extern void close_string(char *s);

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern void Popup_grey(Popup *p, int id, int on);
extern int Popup_help_me(Popup *popup);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int type, void *parent);   /* PORT: C linkage, as defined (W2) */
/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int unit_from_docks(int type);
extern "C" unsigned char fill_docks(int from_pool);
extern "C" void reset_europe_units(void);
extern "C" void europe_change_mode(void);

void europe_recruit(int youth, int choose)
{
    int price;
    int pick;
    int slot;
    int unit;
    int n;
    int fifth;
    int hundred;
    int next;
    int cap;
    char buf[0x50];
    register int i;
    register int type;
    Popup p(0x800);

    price = (g_game.difficulty + g_cur_nation_rec->recruit_count) * 20 + 0x8c;
    n = *max_ptr_1048_efef((hundred = 100, &hundred), (fifth = price / 5, &fifth));
    price -= (int)((long)(price - n) * g_cur_nation_rec->crosses / (g_cur_nation_rec->need + 1L));
    price = max_int_1048_dd61(10, price);
    if (youth != 0 || choose != 0)
        price = 0;
    g_text_args[0] = price;
top:
    if (youth != 0) {
        g_popup_dude = 3;
        show_plain_dialog_1048_dde3(&p, (void*)("LOSTCITY0"), 0);
    } else if (choose != 0) {
        g_popup_dude = 4;
        popup_say(0, g_nation_name_word[g_cur_nation]);
        show_plain_dialog_1048_dde3(&p, (void*)("RECRUITCHOOSE"), 0);
    } else {
        g_popup_dude = 2;
        show_plain_dialog_1048_dde3(&p, (void*)("RECRUIT"), 0);
    }
    if (youth == 0 && choose == 0)
        p.flags |= 1;
    n = 0;
    if (youth == 0 && choose == 0) {
        buf[0] = 0;
        open_string(buf);
        say_string(buf, g_misc_words[MISC_NONE]);
        close_string(buf);
        Popup_item(&p, buf, 0x19d);
        n = 1;
    }
    for (i = 0; i < 3; i++) {
        type = g_cur_nation_rec->recruit[i];
        if (type == JOB_COLONIST_ALT)
            type = JOB_COLONIST;
        buf[0] = 0;
        say_string(buf, g_spec_recs[type].expert);
        Popup_item(&p, buf, i + n);
        if ((long)price > g_cur_nation_rec->gold)
            Popup_grey(&p, i + n, 1);
    }
    g_dialog_help_used = 0;
    if (g_cur_nation < NATION_EUROPEAN_COUNT && g_nation_text[g_cur_nation].control == CONTROL_HUMAN) {
        p.flags |= 0x40;
        p.flags &= ~1;
        pick = popup_run_thunk_1048_de13(&p) - 2;
        if (youth != 0 || choose != 0)
            pick++;
    } else {
        pick = 1;
    }
    if (pick < 0)
        goto done;
    slot = pick;
    type = g_cur_nation_rec->recruit[slot];
    if (pick == 0x19c && !Popup_help_me(&p))
        goto done;
    if (Popup_help_me(&p)) {
        if (type == JOB_COLONIST_ALT)
            type = JOB_COLONIST;
        if (pick == 0x19c)
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, -1, (EventWin *)&g_europe_win_dlg);
        else
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, type, (EventWin *)&g_europe_win_dlg);
        goto top;
    }
    if (youth == 0)
        g_cur_nation_rec->crosses = 0;
    unit = unit_from_docks(type);
    if (unit < 0)
        goto done;
    g_cur_nation_rec->gold -= price;
    g_cur_nation_rec->recruit[slot] = fill_docks(0);
    if (youth == 0 && choose == 0) {
        g_cur_nation_rec->recruit_count = *min_ptr_1048_f021((cap = 0xb4, &cap), (next = g_cur_nation_rec->recruit_count + 1, &next));
        reset_europe_units();
        europe_change_mode();
    }
done:
    g_popup_dude = -1;
}

