/* PORT: this file's own flattened PediaWin replaced by PediaWin_ctor.c's class (list asserted at 0x83e): its
 * int fields are 4 bytes here and it was not packed, so every member after tile_w was misplaced (W7). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 86 of the imported file. */
/* europe_purchase: candidate for 1048:7296 (957 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_purchase
 * name-source: sibling-build
 *
 * The Europe purchase screen. Six buyable types come from the 6-byte records at
 * SEG20:0xd83d -- a type byte at +0 and a price word at +3 -- and each becomes a menu line
 * built by hand: the type's name, a space, the `":   "` separator, the MISC_COST word, a space, the price
 * through `LTOA1`, and the MISC_CLOSE_PAREN word. A line the player cannot afford is greyed
 * with `Popup_grey` rather than hidden.
 *
 * ARTILLERY GETS DEARER EVERY TIME. Type 0xb has `nation->artillery * 100` added to its
 * listed price, and buying one increments that counter -- so the escalation is stored per
 * nation at +0x1e and the price in the table never changes. The addition appears TWICE, once
 * to display the price and once to charge it, and the second is what makes it real.
 *
 * The whole screen is a loop: picking a line with the pedia modifier held (the high bit
 * `Popup_help_me` reports) opens the Colonopedia on that type and comes back
 * to the top rather than buying. Item 0x19d is the MISC_NONE line; picked with the modifier it opens the pedia at -1.
 *
 * A new unit starts with orders 0 if its type is in 0xd..0x12 -- the ship range, the same
 * one purge_square uses -- and 1 otherwise, then takes two bytes from the nation record at
 * +0x32 and +0x33.
 *
 * NAME: the Macintosh build's `europe_purchase(void)`, and it takes no arguments, which
 * `tools/signatures.py` confirms from the call site. Recorded as ADOPTED even though the
 * descriptive route reaches the same word from the `PURCHASE` resource id and the body,
 * because the Macintosh table was consulted in the same breath and an independence that
 * cannot be demonstrated should not be counted (§117). The agreement is worth noting and
 * not worth spending.
 *
 * The separator literal is `":   "` -- a colon and THREE spaces -- read out of the image at
 * DGROUP:0x21a4. It was first written as `"-"` on the strength of nothing, and a byte-match
 * cannot tell: the push is a fixup and every literal compiles to the same bytes. The length
 * is fixed by its neighbours (`PURCHASE` ends at 0x21a4 and `REALLYBUY` starts at 0x21a9,
 * so four characters plus a NUL) and the content by reading those five bytes.
 */

#include "colonization.h"

#include "units.h"
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
typedef char pedia_list_at_0x83e[(offsetof(PediaWin, list) == 0x83e) ? 1 : -1];

#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
                                  /* 6 bytes at SEG20:0xd83c -- the base `setup_purchase`
                                     writes, one byte BEFORE the 0xd83d the readers used */

extern char g_pedia_win;          /* SEG20:0x387c, declared */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern char *strings(int n);

extern void space_string(char *buf);

extern void Popup_grey(Popup *p, int id, int on);
extern int Popup_help_me(Popup *popup);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int article, void *parent);   /* PORT: C linkage, as defined (W2) */
extern void popup_say(int slot, int v);
extern "C" int popm(char *msg, int n);   /* PORT: C linkage, as defined (W2) */

extern "C" void set_text_arg_i_1048_db6a(int slot, int v);
extern "C" void draw_europe_screen(void);

void europe_purchase(void)
{
    Popup p(0x800);
    int ev;
    int price;
    int idx;
    int type;
    int unit;
    char buf[0x50];
    char num[0x14];
    register int i;

restart:
    g_popup_dude = 2;
    show_plain_dialog_1048_dde3(&p, (void*)("PURCHASE"), 0);
    p.flags |= 1;
    Popup_item(&p, strings(g_misc_words[MISC_NONE]), 0x19d);
    for (i = 0; i < 6; i++) {
        type = g_recruit[i].type;
        price = g_recruit[i].price;
        if (type == 0xb)
            price += g_cur_nation_rec->artillery * 100;
        STRCPY(buf, strings(g_unit_types[type].name));
        space_string(buf);
        STRCAT(buf, ":   ");
        say_string(buf, g_misc_words[MISC_COST]);
        space_string(buf);
        LTOA1(price, num, 10);
        STRCAT(buf, num);
        say_string(buf, g_misc_words[MISC_CLOSE_PAREN]);
        Popup_item(&p, buf, i + 1);
        if ((long)price > g_cur_nation_rec->gold)
            Popup_grey(&p, i + 1, 1);
    }
    p.flags |= 0x40;
    ev = popup_run_thunk_1048_de13(&p);
    if (ev < 2)
        goto out;
    if (ev == 0x19e && Popup_help_me(&p) == 0)
        goto out;
    if (ev != 0x19e) {
        idx = ev - 2;
        price = g_recruit[idx].price;
        type = g_recruit[idx].type;
    }
    if (Popup_help_me(&p) != 0) {
        if (ev == 0x19e)
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, -1, (EventWin *)&g_europe_win_dlg);
        else
            PediaWin_ShowModal((PediaWin *)&g_pedia_win, 1, type, (EventWin *)&g_europe_win_dlg);
        goto restart;
    }
    if (type == 0xb) {
        price += g_cur_nation_rec->artillery * 100;
        g_cur_nation_rec->artillery++;
    }
    popup_say(0, g_unit_types[type].name);
    set_text_arg_i_1048_db6a(0, price);
    if (popm("REALLYBUY", 2) != 1)
        goto out;
    g_cur_nation_rec->gold -= price;
    unit = create_unit(type, g_cur_nation, g_cur_nation - 20, g_cur_nation - 20);
    if (unit < 0)
        goto out;
    g_units[unit].orders =
        (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST) ? 0 : 1;
    g_units[unit].dest_x = g_cur_nation_rec->start_x;
    g_units[unit].dest_y = g_cur_nation_rec->start_y;
    draw_europe_screen();
out:
    g_popup_dude = -1;
}

