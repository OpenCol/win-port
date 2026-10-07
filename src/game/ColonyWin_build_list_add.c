/* ColonyWin_build_list_add: candidate for 1010:6d39 (447 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_build_list_add
 * name-source: sibling-build
 *
 * Makes one line of the colony's "what to build" menu. Item -1 is the nothing entry and takes
 * a plain string from the table at SEG20:0xd98c; everything else is built up: the item's name
 * in CAPITALS, five spaces (`space_string` and then four more), and the cost in brackets.
 *
 * THE COST SHOWN IS WHAT IS LEFT TO PAY, not what the item costs: `max(0, cost - hammers)`, so
 * an item already paid for reads zero rather than going negative. A building the colony already
 * has -- kind 1 with the colony's flag set -- shows the word at SEG20:0xddc6 instead of a
 * number, and its tools requirement is cleared so the second bracket is suppressed.
 *
 * The tools bracket is only added when the item wants tools, so most lines carry one bracket
 * and a few carry two.
 *
 * `left = cost - hammers;` is its own statement before `max_int` is called on it: written
 * inline as the argument, the difference is pushed from DX and the register the shipped keeps
 * it in is never written.
 *
 * The nothing entry is the `if` and the rest the `else` -- written as an early `return` the
 * function grows a second epilogue where the shipped has a `JMP NEAR` to the only one (rule 44).
 *
 * RENAMED 2026-09-27 from build_menu_item: BEYOND THE PROPOSALS. Mac ColonyWin::build_list_add(Popup&, short), CODE 30 0x000c, 428 bytes, called only by colony_hammer_change. 6d39 (ColonyWin *w, struct Popup *p, int item): this, a Popup reference and a short. It is called only by colony_choose_build (= colony_hammer_change, below). Callees item for item: strings, Popup::item x2, get_production_class, get_production_cost, get_production_name, strupr, space_string x3, open_string x2, has_building, say_string x3, say_number x2, close_string x2. Layout: first in the colony_hammer block on both, directly before colony_hammer_change / colony_choose_build. Size 428 against 447. The Mac spelling is lower-case, like colony_hammer_buy, and the Class_Method form follows ColonyWin_colony_hammer_buy.
 */

#include "colonization.h"

#include "goods.h"
#include "misc.h"
#include "labels.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"

extern char *strings(int n);
/* PORT: `typedef struct Popup Popup;` removed: include/gen/classes.h defines Popup (W1). */
extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern int get_production_class(int item, int *idx);
extern int get_production_cost(int item, int *tools);
extern char *get_production_name(int item);

extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);

extern int has_building(int bit);

/* same object: 4-byte `push cs; call ` (rule 47) */

void ColonyWin_build_list_add(ColonyWin *w, struct Popup *p, int item)
{
    int id;
    int cost;
    int tools;
    int kind;
    char buf[0x50];
    register int it;
    register int left;

    it = item;
    id = it + 1;
    if (it == -1) {
        Popup_item(p, strings(g_ctitle_word[CTITLE_NO_PRODUCTION]), id);
    } else {
        kind = get_production_class(it, 0);
        cost = get_production_cost(it, &tools);
        STRCPY(buf, get_production_name(it));
        STRUPR(buf);
        space_string(buf);
        STRCAT(buf, "    ");
        open_string(buf);
        left = cost - g_colonies[w->colony].hammers;
        left = max_int_1010_e632(0, left);
        if (kind == 1 && has_building(it) != 0) {
            say_string(buf, g_misc_words[MISC_COMPLETE]);
            tools = 0;
    } else {
            say_number(buf, left);
            space_string(buf);
            say_string(buf, g_goods_name_word[PROD_HAMMERS]);
        }
        close_string(buf);
        if (tools != 0) {
            open_string(buf);
            say_number(buf, tools);
            space_string(buf);
            say_string(buf, g_goods_name_word[GOODS_TOOLS]);
            close_string(buf);
        }
        Popup_item(p, buf, id);
    }
}
