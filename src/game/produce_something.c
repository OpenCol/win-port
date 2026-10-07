/* produce_something: candidate for 1018:8e3d (513 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 *
 * The current colony finishes what it was building. `get_production_class` splits the
 * build slot (+0x94) into a kind and an index: kind 1 is a building, kind 2 is a unit, and
 * kind 0 is nothing to finish. Either way the progress word at +0x92 is cleared, the item's
 * name goes into text slot 1 and `BUILT` is announced.
 *
 * A building the colony already has is dropped silently. Warehouse levels live in the byte
 * at +0x95 and items 0xf and 0x10 raise it; a second 0x10 on top of two levels is refused
 * with `NOMOREWAREHOUSE` instead. Capitol levels (BUILDING_CAPITOL, BUILDING_CAPITOL_EXPANSION) live at +0x96 and items 0x1e and 0x1f
 * raise that. The top level of each pair -- 0x10 and 0x1f -- skips `set_building`
 * entirely: the flag bit records HAVING the building and a level counter already does that.
 *
 * A unit is built with `create_unit(type, owner, x, y)`, and type 0xb -- Artillery,
 * per docs/findings/pedia-tables.md, where kind 2 is exactly unit types 11..17 -- also banks
 * a per-nation credit at Nation+0x48. That byte is written HERE and nowhere else in the
 * binary, and read in one place: `foreign_country` decrements it to make a Europe artillery
 * purchase cost nothing instead of testing the nation's gold at +0x2a. So an AI that builds
 * artillery in a colony earns one free artillery in Europe, which is why the credit is gated
 * on `owner >= 4 || g_nation_text[owner].control != 0` -- the negation of the
 * `nation < 4 && dead == 0` idiom every other file uses for "is the human player".
 *
 * NAME: descriptive. The Macintosh build has twenty `build*`/`building*` symbols and none
 * of them fits: this takes no arguments and completes a build, where the Mac's four `(void)`
 * build symbols are `count_eligible_builds`, `make_building_table`, `orders_build` and
 * `reset_building_sequence`. Absence of a match is not a reason to force one.
 *
 * Three divergences, all rules already written down. The four early exits are `goto` and not
 * `return` (rule 12) -- the shipped code has ONE epilogue and jumps to it. `build` at +0x94
 * is a signed `char`: the shipped promotion is `CBW`, where `unsigned char` gives `MOV AH,0`,
 * which is what +0x1a, +0x0 and +0x1 all use in the same function. And +0x92 is a word, so
 * +0x94 follows it with no padding -- a byte of pad there moved every later field by one and
 * read as a wrong layout rather than one wrong offset.
 *
 * RENAMED 2026-09-26 from colony_build_complete: the Mac build's produce_something(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller new_turn_colony; callee has_building; callee possible_popup; callee set_building), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "nations.h"
#include "control.h"
#include "colflag.h"
#include "records.h"

extern Nation g_nations[];              /* SEG20:0xc2f4 */

extern void new_mood(int mood);

extern int get_production_class(int item, int *idx);
extern char *get_production_name(int item);

extern void set_building(int bit, int on);
extern int has_building(int bit);
extern char possible_popup(char *msg, int a, int b, int c, int d, int e, int f);
extern void make_building_table(void);

void produce_something(void)
{
    int item;
    int type;
    register int kind;
    int unit;

    kind = get_production_class(g_cur_colony_rec->item, &item);
    if (kind == 0)
        goto out;
    if (kind == 1) {
        if (has_building(item))
            goto out;
        if (item == 0x10 && g_cur_colony_rec->warehouses >= 2) {
            g_msg_seen |= possible_popup("NOMOREWAREHOUSE", !g_msg_seen, 2, 0, 0, 5, 0);
            g_cur_colony_rec->status |= COLONY_BUILD_DONE;
            goto out;
        }
        if (item == 0xf || item == 0x10) {
            g_cur_colony_rec->warehouses++;
            if (item == 0x10) {
                g_cur_colony_rec->status |= COLONY_BUILD_DONE;
                goto done;
            }
        } else if (item == 0x1e || item == 0x1f) {
            g_cur_colony_rec->capitols++;
            if (item == 0x1f)
                goto done;
        }
        set_building(item, 1);
        g_cur_colony_rec->status |= COLONY_BUILD_DONE;
    }
    if (kind == 2) {
        type = item;
        unit = create_unit(type, g_cur_colony_rec->nation,
                                     g_cur_colony_rec->x, g_cur_colony_rec->y);
        if (type == 0xb
            && (g_cur_colony_rec->nation >= NATION_EUROPEAN_COUNT || g_nation_text[g_cur_colony_rec->nation].control != CONTROL_HUMAN))
            g_nations[g_cur_colony_rec->nation].artillery_credit++;
    }
done:
    g_cur_colony_rec->hammers = 0;
    set_name_slot_1018_b3f6(1, get_production_name(g_cur_colony_rec->item));
    if (g_cur_colony_is_players) {
        if (kind == 1 && (item == 0x13 || item == 0x14 || item == 3 || item == 4))
            new_mood(3);
        else
            new_mood(2);
    }
    g_msg_seen |= possible_popup("BUILT", !g_msg_seen, 2, 0, 0, 5, 0x54);
    if (kind == 1 && item != 0x1f)
        g_new_building = item;
    make_building_table();
out:
    ;
}
