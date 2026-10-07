/* PORT: this file's own declarations of popup_setup_plain_1048_e387 removed: the header declares it from the definition (W1 step 3). */
/* CheatMenuBlow: candidate for 1048:b9da (249 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CheatMenuBlow
 *
 * The Cheat entry that wipes out a tribe. It counts how many of the eight already carry
 * flag 0x80 -- gone -- and does nothing at all if all eight do; otherwise every surviving
 * tribe is offered by adjective (`get_name(t + 4)` into the string table) and the
 * chosen one has its settlements destroyed.
 *
 * `DrawMap(1)` is OUTSIDE the `if`: the map is repainted even when the popup
 * never opened, which is what puts the call after the closing brace rather than inside it,
 * and it costs ten bytes to get wrong -- enough to push the guard's `JGE` out of short
 * range, which is how the first compile showed it.
 *
 * The two loops test the same flag in opposite spellings and Borland distinguishes them
 * (§114): the counting loop is `if (flags & 0x80)` and tests memory directly, the offering
 * loop is `if (!(flags & 0x80))` and loads the byte, promotes it and tests AL.
 *
 * `gone` is declared first so it takes DI; `t` is `register` and takes SI (rule 40).
 *
 * RENAMED 2026-09-26 from kill_indians: the Mac build's CheatMenuBlow(void), bound by LAYOUT: both builds keep their source files' function order, and on both sides this is the one function between memory_check and game_menu_debug, which are already bound; the arity agrees and the Mac body is 1.08x this one.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern int get_name(int nation);
extern char *strings(int n);
extern void kill_tribe(int tribe);
extern void DrawMap(int mode);


void CheatMenuBlow(void)
{
    int gone;
    register int tribe;
    int pick;
    Popup p(0x800);

    gone = 0;
    for (tribe = 0; tribe < 8; tribe++)
        if (g_tribes[tribe].flags & 0x80)
            gone++;
    if (gone < 8) {
        popup_setup_plain_1048_e387(&p);
        for (tribe = 0; tribe < 8; tribe++) {
            if (!(g_tribes[tribe].flags & 0x80))
                Popup_item(&p, strings(get_name(tribe + 4)), tribe);
        }
        pick = popup_run_thunk_1048_de13(&p);
        if (pick > 0)
            kill_tribe(pick - 1);
    }
    DrawMap(1);
}

