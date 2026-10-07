/* europe_click_ship: candidate for 1048:548a (634 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_click_ship
 * name-source: sibling-build
 *
 * The menu that opens when a ship in the Europe screen is clicked. `EUROPESHIPCLICK` is the
 * title and `EUROPESHIPOPTIONS` the four item captions, read a line at a time out of the
 * shipped text; a missing resource abandons the whole menu.
 *
 * Two of the four items are conditional. Item 2 needs `in_port == 0` and bit 0x80 of
 * Unit+4 clear; item 3 needs cargo aboard and `in_port == 0`. Items 1 and 4 are always
 * offered. Picking 1 resets the destination pair at DGROUP:0x4ce2/0x4cde, 2 opens the
 * sail-for-the-New-World prompt, and 3 walks the cargo from the top down and announces
 * `SOMEBOYCOTT` if the last good it looked at was boycotted.
 *
 * NAME: the Macintosh build's `europe_click_ship(short, short)` -- two shorts, and
 * `tools/signatures.py` reads four argument bytes off this function's call site. The
 * generated placeholder was `txt_EUROPESHIPCLICK_1048_548a`, and for once the resource it
 * names IS the subject rather than a message the function happens to show (§119). That is
 * the fifth `txt_*` placeholder byte-matched and the first that was not misleading.
 *
 * `flagged` is reset INSIDE the cargo loop, so it reports the last good examined rather
 * than whether any was boycotted. That reads like a bug and it is what the bytes say; the
 * store is between the `get_cargo` result and the first test, not before the loop.
 *
 * Two spellings the bytes fixed. `g_europe_selected_ship = g_europe_ship_highlight = 0` is one chained assignment: the
 * shipped code makes the zero once in AX and stores it twice, where two statements give two
 * `MOV WORD [mem],0`. And the cargo test is written with the ZERO case first -- `if (test
 * == 0) { ... } else { flagged = 1; }` -- because Borland emits the branches in source
 * order and the shipped `JNZ` jumps FORWARD to the assignment.

 * SIZE: the census says 533 and the function is 634. the two-entry CS-relative jump table for the four-way item switch sits past the code, and
 * Ghidra sizes a function by its reachable CODE, so the table is excluded and the
 * reconstruction stops before it. `tools/re_agent/check_sizes.py` reports the table's
 * address; the reconstruction already grades over the whole extent unchanged.
 */

#include "colonization.h"

#include "w16types.h"
/* PORT: this file's own GRPort, Sprite, EuropeWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */



extern void popup_say(int slot, int v);
extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern "C" char *text_get(void);   /* PORT: C linkage, as defined (W2) */
extern void expand_text_template(char *src, char *dst);
extern "C" int text_open(void *unused, char *id);   /* PORT: C linkage, as defined (W2) */
extern "C" void Popup_sprite(Popup *popup, Sprite *art, int u, char *s);   /* PORT: C linkage, as defined (W2) */
extern "C" int UnitIcon(int u);   /* PORT: C linkage, as defined (W2) */
extern void move_to_top(int u);
extern int get_cargo(int u, int slot);
extern "C" int popm(char *msg, int n);   /* PORT: C linkage, as defined (W2) */

extern "C" void set_name_slot_1048_de36(int slot, char *s);

extern "C" void attempt_to_set_sail(int ship);
extern "C" int boycott(int bit);
extern "C" int EuropeWin_AttemptCargoToEurope(EuropeWin *ctx, int u, int type, int ask);
extern "C" void europe_change_mode(void);

void europe_click_ship(int unit, int in_port)
{
    Popup p(0x800);
    int pick;
    int show;
    int type;
    int flagged;
    char *line;
    char buf[0x50];
    register int i;

    popup_say(0, g_unit_types[g_units[unit].type].name);
    buf[0] = 0;
    set_name_slot_1048_de36(1, buf);
    text_plain_step_1048_de5f(&p, g_game_txt, "EUROPESHIPCLICK", 0);
    p.flags |= 1;
    p.flags |= 2;
    if (text_open(g_game_txt, "EUROPESHIPOPTIONS") != 0)
        goto out;
    Popup_sprite(&p, &g_icon_sprites[UnitIcon(unit)], unit, 0);
    for (i = 1; i <= 4; i++) {
        line = text_get();
        expand_text_template(line, buf);
        switch (i) {
        case 1: show = 1; break;
        case 2: show = (in_port == 0 && !(g_units[unit].flags2 & 0x80)); break;
        case 3: show = (g_units[unit].cargo_count != 0 && in_port == 0); break;
        case 4: show = 1; break;
        }
        if (show != 0)
            Popup_item(&p, buf, i);
    }
    pick = popup_run_thunk_1048_de13(&p) - 1;
    switch (pick) {
    case 1:
        move_to_top(unit);
        g_europe_selected_ship = g_europe_ship_highlight = 0;
        break;
    case 2:
        attempt_to_set_sail(g_europe_selected_ship);
        break;
    case 3:
        for (i = g_units[unit].cargo_count - 1; i >= 0; i--) {
            type = get_cargo(unit, i);
            flagged = 0;
            if (boycott(type) == 0) {
                if (EuropeWin_AttemptCargoToEurope(&g_europe_win, unit, type, 0) != 0)
                    break;
            } else {
                flagged = 1;
            }
        }
        if (flagged != 0)
            popm("SOMEBOYCOTT", 2);
        break;
    }
out:
    europe_change_mode();
}

