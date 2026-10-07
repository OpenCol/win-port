/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 54 of the imported file. */
/* orders_overboard: candidate for 1008:00d2 (384 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: orders_overboard
 *
 * Asks which of the selected unit's cargo slots to throw overboard, and throws it. Nothing
 * happens for a unit carrying nothing. The prompt is `OVERBOARD` at DGROUP:0x154.
 *
 * Each slot's label is built into a local buffer, not fetched: the amount as a number, a
 * space, then the goods name through `g_goods_name_word[type]`. Item ids are the slot
 * numbers and 0x63 is the cancel entry, added FIRST so it sits at the top of the list; a
 * pick of 0x63 or a negative event does nothing.
 *
 * NAME: descriptive. The Macintosh build's `orders_unload(void)` has the right arity and
 * is the wrong operation -- it also carries `unload_cargo(short, short)` for putting cargo
 * into a colony, and this one drops it into the sea. `OVERBOARD` is the only word either
 * build offers for that and it is a resource id, not a symbol, so the name stays ours.
 *
 * RENAMED 2026-09-26 from prompt_dump_cargo: the Mac build's orders_overboard(void), bound by call-graph agreement (tools/mac_callgraph.py): 7 of its Mac call edges already bound land on this function (caller KeyboardEvent; callee get_cargo; callee item; callee say_number; callee say_string), the runner-up gets 5, and the arity agrees (0).
 */

#include "colonization.h"

#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern GameState g_game;   /* SEG20:0x7782 */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern char *strings(int n);
extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);

extern void space_string(char *buf);
extern int unload_cargo(int u, int slot);

extern "C"   /* PORT: C linkage: C files call it too (W2) */
void orders_overboard(void)
{
    Popup p(0x800);
    int type;
    int amt;
    int pick;
    char buf[0x50];
    register int slot;
    int u;

    u = g_game.selected_unit;
    if (g_units[u].cargo_count == 0)
        goto out;
    g_popup_dude = 0;
    show_plain_dialog_1008_cd53(&p, (void*)("OVERBOARD"), 0);
    Popup_item(&p, strings(g_misc_words[MISC_NOTHING]), 0x63);
    for (slot = 0; slot < g_units[u].cargo_count; slot++) {
        type = get_cargo(u, slot);
        amt = get_cargo_amount(u, slot);
        buf[0] = 0;
        say_number(buf, amt);
        space_string(buf);
        say_string(buf, g_goods_name_word[type]);
        Popup_item(&p, buf, slot);
    }
    pick = popup_run_thunk_1008_cd83(&p) - 1;
    if (pick < 0 || pick == 0x63)
        goto out;
    unload_cargo(u, pick);
out:
    g_popup_dude = -1;
}

