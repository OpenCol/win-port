/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 107 of the imported file. */
/* europe_recruit_any: candidate for 1048:6596 (952 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_recruit_any
 *
 * The King's recruit screen. The 8-byte records at SEG20:0xd61a are scanned for a positive
 * price at +6, the survivors collected into parallel `types`/`prices` arrays and sorted by
 * `sort_16x16`, and each printed as name, `":   "`, the MISC word COST (13), the price
 * through LTOA1 and the MISC word `)` (14). A line dearer than the nation's gold is greyed
 * rather than hidden, and picking one with the pedia modifier held opens the Colonopedia and
 * comes back to the top.
 *
 * This was parked at 935 of 952 for one register (§130) and the last byte of it is rule 41:
 * the shared exit ends with an explicit `return;`, not by falling off the end. With a
 * destructible local that changes the unwind restore from `MOV AX,[bp-0x3fc]` to `MOV DX`,
 * and nothing else in the function moves.
 *
 * Four more spellings the bytes fixed, all established while it was parked.
 *
 * THE POPUP IS DECLARED LAST AND ALLOCATED FIRST. Its slot is the top of the frame
 * (bp-0x3de) while the scalars run bp-0x3fe..bp-0x410 and the arrays below them, in
 * declaration order -- but the three initialised scalars store their values BEFORE the
 * constructor runs. The only source order producing both is every scalar and array declared
 * first and the object last.
 *
 * THE PAGE-RANGE TEST IS A POSITIVE `if`, not two `continue`s: the shipped `JG ` is a
 * 386 direct long conditional and a `continue` gives invert-and-jump-over (rule 12).
 *
 * THE TWO BACK EDGES ARE DIFFERENT CONSTRUCTS. The Colonopedia detour is a `goto` to a label
 * at the top -- a plain backward `JMP `. The `ev >= 0x62` retry is the condition of a
 * `do { ... } while (ev >= 0x62);` whose top carries that same label, which is what gives
 * the direct `JGE ` backward with no exception-frame reset. Spelled as a second `goto`
 * it emits `MOV WORD [bp-0x3f0],6` first; as `for(;;)` with `continue` the jump goes FORWARD
 * to the bottom of the loop instead.
 *
 * And `types[n] = (unsigned char)i`, not `= i`: the shipped loads the index as a byte and
 * promotes it with `MOV AH,0` before the word store.
 *
 * `pages` is written once (1) and read once, so the "next page" item it guards is
 * unreachable in the shipped build. That is what the bytes say and it is left alone.
 *
 * RENAMED 2026-09-26 from recruit_at_university: the Mac build's europe_recruit_any(void), bound by call-graph agreement (tools/mac_callgraph.py): 8 of its Mac call edges already bound land on this function (caller EuropeButtonClick; callee europe_change_mode; callee grey; callee help_me; callee item), the runner-up gets 4, and the arity agrees (0). The Mac function also calls sort_16x16, itoa and Popup::grey -- this screen's sort_int_pairs_by_key, LTOA1 and its greying of lines dearer than the treasury.
 */

#include "colonization.h"

#include "misc.h"
#include "labels.h"
#include "w16types.h"
/* PORT: this file's own EventWin, GRPort, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"   /* EventWin : Screen, EventTable, flattened for C: 0x8c bytes */
typedef struct { char gamewin[0xc4]; char title[0x80]; int tile_w, tile_h; char save_flag; char own_msg[0xa0]; char queue[0x10][0x50]; char pad_6e9; char btn[0x74]; unsigned char active; unsigned char colchild_kind; int dropped; void *repaint; void *resize; RECT rect; int which; char noclose; char pad_775; int msg_timer; int queued; short category; char button[0x74]; char bar[0x28]; char port[0x26]; char list[0xc]; short kind, sel, count, pages, page, wanted; char scrollbar_built, modal, initialised, f859; } PediaWin;   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */   /* PediaWin : ColChild, flattened for C (ColChild's kind, which PediaWin's hides, as colchild_kind): 0x85a */

#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern char g_pedia_win;          /* SEG20:0x387c, declared */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern char *strings(int n);

extern void space_string(char *buf);

extern void sort_16x16(int n, int *payload, int *key);
extern void Popup_grey(Popup *p, int id, int on);
extern int Popup_help_me(Popup *popup);
extern "C" void PediaWin_ShowModal(void *pedia, int category, int article, void *parent);   /* PORT: C linkage, as defined (W2) */

extern "C" int unit_from_docks(int type);
extern "C" void reset_europe_units(void);
extern "C" int europe_change_mode(void);

void europe_recruit_any(void)
{
    int i;
    int ev;
    int price;
    int idx;
    int unit;
    int type;
    int pages = 1;
    int per_page = 0x14;
    int page = 0;
    int n;
    int prices[28];
    int types[28];
    char buf[0x50];
    char num[0x14];
    Popup p(0x800);
    register int shown;
    register int spec;

    n = 0;
    for (i = 0; i < 0x1c; i++) {
        if (g_spec_recs[i].price > 0) {
            prices[n] = g_spec_recs[i].price;
            types[n] = (unsigned char)i;
            n++;
        }
    }
    sort_16x16(n, types, prices);
    do {
restart:
        show_plain_dialog_1048_dde3(&p, (void*)("KINGRECRUIT"), 0);
        shown = 0;
        if (page == 0)
            Popup_item(&p, strings(g_misc_words[MISC_NONE]), 0x19d);
        else
            Popup_item(&p, strings(g_ctitle_word[CTITLE_MORE]), 0x62);
        for (i = 0; i < n; i++) {
            spec = types[i];
            if (page * per_page <= shown && (page + 1) * per_page > shown) {
                buf[0] = 0;
                say_string(buf, g_spec_recs[spec].expert);
                space_string(buf);
                STRCAT(buf, ":   ");
                say_string(buf, g_misc_words[MISC_COST]);
                space_string(buf);
                price = g_spec_recs[spec].price;
                LTOA1(price, num, 10);
                STRCAT(buf, num);
                say_string(buf, g_misc_words[MISC_CLOSE_PAREN]);
                Popup_item(&p, buf, spec);
                if ((long)price > g_cur_nation_rec->gold)
                    Popup_grey(&p, spec, 1);
            }
            shown++;
        }
        if (pages - 1 > page)
            Popup_item(&p, strings(g_ctitle_word[CTITLE_MORE]), 0x63);
        p.flags |= 0x40;
        p.flags &= ~1;
        ev = popup_run_thunk_1048_de13(&p);
        if (ev < 0)
            goto out;
        if (ev == 0x62)
            page--;
        if (ev == 0x63)
            page++;
        if (Popup_help_me(&p) != 0 && ev != 0x62 && ev != 0x63) {
            if (ev == 0x19e)
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, -1, (EventWin *)&g_europe_win_dlg);
            else
                PediaWin_ShowModal((PediaWin *)&g_pedia_win, 3, ev - 2, (EventWin *)&g_europe_win_dlg);
            goto restart;
        }
        if (ev == 0x19e)
            goto out;
    } while (ev >= 0x62);
    idx = ev - 1;
    price = g_spec_recs[idx].price;
    type = idx;
    unit = unit_from_docks(type);
    if (unit < 0)
        goto out;
    g_cur_nation_rec->gold -= price;
    reset_europe_units();
    europe_change_mode();
out:
    return;
}

