/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 108, 110, 112 of the imported file. */
/* ask_port: candidate for 1028:bfd0 (1133 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ask_port
 * name-source: sibling-build
 *
 * The colony picker behind "Sail to which port?". It lists every colony
 * `valid_port` says the unit can reach, ten to a page, with the player's own
 * European port as a fixed extra entry, and returns the chosen colony index (or 0x3e7 for
 * Europe, or 0x3e8 for the caller's own trailing item), -1 if the popup was dismissed.
 *
 * FOUR CALL SITES, TWO JOBS. `route_edit` and `route_create` call it with `unit = -1` to
 * name the stops of a trade route -- that is the `TRADESTART` title, "Select destination
 * number %NUMBER0 for route" -- and SEG2 calls it with a real unit to give that unit a
 * destination. A ship (type 0xd..0x12) gets `SAILPORT`, a land unit `TRAVELPLACE`. All three
 * literals were read out of DGROUP rather than guessed (§119): 0x11f8, 0x1201, 0x120d.
 *
 * `mark_here` is what separates the two jobs. With it set, the colony the unit is standing
 * in is refused outright for a land unit and merely flagged (greyed) for a ship; the route
 * editor passes 0 and so may pick the current stop again. `offer_extra` adds item 0x3e8
 * after the last page, which both route sites then test for by hand.
 *
 * THE HELPER SETS THE GLOBAL IT IS TESTED ON. `valid_port(colony, unit)` calls
 * `set_colony(colony)` first, so `g_cur_colony_rec` is the colony just asked about
 * and the loop reads its name and position straight out of that pointer without indexing
 * anything. That is why no colony record is ever subscripted here.
 *
 * THE POPUP IS DECLARED LAST AND ALLOCATED FIRST (§130). Its slot is bp-0x3de, above the
 * scalars at bp-0x3fe..bp-0x410 and the 0x50-byte label buffer below them, but `ev` and
 * `stored` store their zeroes before the constructor runs -- so every scalar and the array
 * are declared first and the object last.
 *
 * THE PAGE LOOP IS A `do { ... } while (ev == 0x3e6 || ev == 0x3e7);`, not a backward
 * `goto`. Both spellings put the same label at the top, but a `goto` across the object's
 * lifetime makes the compiler re-stamp the unwind state -- `MOV WORD [bp-0x3f0],6` in front
 * of the back edge -- and the shipped code has only the bare `JZ `. The early exit out
 * of it is `break`, which is the invert-and-jump-over-a-jump of rule 12.
 *
 * THE COLONY FILTER IS ONE COMPOUND CONDITION, not a `continue`: `is_here == 0 || (type in
 * range)` gives the direct `JC`/`JA` to the end of the block that the shipped code has.
 *
 * `if (stored == 0) { }` at L6_c3d1 really is empty -- a `JNZ` to the following instruction.
 * It is the twin of the two live `if (stored == 0)` blocks above it and was presumably left
 * behind when its body moved into them.
 *
 * SEG20:0x7796 IS READ BOTH WAYS FROM THIS OBJECT. Here it is a word (`MOV BX,[es:0x7796]`
 * to index the nation tables); in `valid_port`, four hundred bytes earlier in the
 * same object, it is a byte (`CMP AL,[es:0x7796]` against a colony's owner). The two
 * declarations alias, and only the word one appears below.
 *
 * RENAMED 2026-09-26 from pick_trade_route_stop: the Mac build's ask_port(short, short, short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller orders_port; caller route_create; callee say_string), the runner-up gets 1, and the arity agrees (4).
 */

#include "colonization.h"

#include "units.h"
#include "labels.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern GameState g_game;   /* SEG20:0x7782 */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern char *strings(int n);

extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);
extern void Popup_set_active(Popup *popup, int id);
extern void Popup_grey(Popup *p, int id, int on);

/* same object: 4-byte `push cs; call ` */
extern "C" int valid_port(int colony, int unit);

int ask_port(register int unit, int cur, int mark_here, int offer_extra)
{
    int ev = 0;
    int page;
    int pages;
    int per_page;
    int count;
    int shown;
    int lo;
    int hi;
    int is_here;
    int stored = 0;
    char label[0x50];
    Popup p(0x800);
    register int colony_index;

    count = 0;
    for (colony_index = 0; colony_index < g_game.colony_count; colony_index++)
        if (valid_port(colony_index, unit) != 0)
            count++;
    pages = 1;
    page = 0;
    if (count > 0xa) {
        pages = (count + 9) / 10;
        per_page = 0xa;
    } else {
        per_page = count + 1;
    }

    do {
    if (unit >= 0) {
        if (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST)
            show_plain_dialog_1028_e43b(&p, (void*)("SAILPORT"), 0);
        else
            show_plain_dialog_1028_e43b(&p, (void*)("TRAVELPLACE"), 0);
    } else {
        show_plain_dialog_1028_e43b(&p, (void*)("TRADESTART"), 0);
    }
    lo = page * per_page;
    hi = lo + per_page - 1;
    shown = 0;
    p.flags |= 1;
    if (page > 0)
        Popup_item(&p, strings(g_ctitle_word[CTITLE_MORE]), 0x3e5);
    if (unit >= 0 && g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST
        && valid_port(0x3e7, unit) != 0) {
        if (page == 0) {
            label[0] = 0;
            say_string(label, g_nation_name_word[g_game.turn_nation]);
            space_string(label);
            open_string(label);
            say_string(label, g_nation_name[g_game.turn_nation]);
            close_string(label);
            Popup_item(&p, label, 0x3e7);
            if (cur == 0x3e7) {
                Popup_set_active(&p, 0x3e7);
                stored = 1;
            }
            if (stored == 0) {
                Popup_set_active(&p, 0x3e7);
                stored = 1;
            }
        }
        shown++;
    }
    for (colony_index = 0; colony_index < g_game.colony_count; colony_index++) {
        if (valid_port(colony_index, unit) != 0) {
            if (shown >= lo && shown <= hi) {
                is_here = 0;
                if (g_units[unit].x == g_cur_colony_rec->x
                    && g_units[unit].y == g_cur_colony_rec->y)
                    is_here = 1;
                if (mark_here == 0)
                    is_here = 0;
                if (is_here == 0
                    || (g_units[unit].type >= UNIT_SHIP_FIRST && g_units[unit].type <= UNIT_SHIP_LAST)) {
                    Popup_item(&p, g_cur_colony_rec->name, colony_index);
                    if (is_here != 0) {
                        Popup_grey(&p, colony_index, 1);
                    } else if (cur == colony_index) {
                        Popup_set_active(&p, colony_index);
                        stored = 1;
                    }
                    if (stored == 0) {
                        Popup_set_active(&p, colony_index);
                        stored = 1;
                    }
                }
            }
            shown++;
        }
    }
    if (pages - 1 > page) {
        Popup_item(&p, strings(g_ctitle_word[CTITLE_MORE]), 0x3e6);
    } else if (offer_extra != 0) {
        Popup_item(&p, strings(g_route_label[ROUTE_LABEL_DELETE_DESTINATION]), 0x3e8);
    }
    if (p.n_items < 1)
        break;
    if (stored == 0) {
    }
    ev = popup_run_thunk_1028_e46b(&p);
    if (ev == 0x3e6)
        page--;
    if (ev == 0x3e7)
        page++;
    } while (ev == 0x3e6 || ev == 0x3e7);
    return ev - 1;
}

