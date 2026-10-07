/* ColonyWin_ColonyClickUnits: candidate for 1010:5f46 (886 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ColonyWin_ColonyClickUnits
 *
 * The orders menu for the colony-window unit under the cursor (DGROUP:0x4bb8/0x4bba): the
 * unit is selected when selectable, named with its specialty in parentheses (the 8-byte
 * specialty table at SEG20:0xd61c; 0x1c = none) into name slot 1, and a COLONYUNIT Popup
 * with the unit's sprite offers the five UNITOPTIONS lines that apply -- Move to front (not
 * already the first unit), Clear orders (orders not 0), Sentry (orders not 1), Fortify (orders
 * not 5 or 6), and No changes always, as the default. Answers: 1 moves the unit to the front,
 * 2 clears its orders, 3 sentries, 4 sets orders 5 (fortify) with sound 0x58.

 * SIZE: the census says 627 and the function is 886. a CS-relative jump table sits past the code, and
 * Ghidra sizes a function by its reachable CODE, so the table is excluded and the
 * reconstruction stops before it. `tools/re_agent/check_sizes.py` reports the table's
 * address; the reconstruction already grades over the whole extent unchanged.
 *
 * RENAMED 2026-09-26 from unit_orders_menu: the Mac build's ColonyWin::ColonyClickUnits(void), bound by call-graph agreement (tools/mac_callgraph.py): 6 of its Mac call edges already bound land on this function (caller ColonyMouseUnits; callee item; callee popup_say; callee set_active; callee sound_play), the runner-up gets 3, and the arity agrees (1).
 */

#include "colonization.h"

#include "jobs.h"
#include "units.h"
#include "sounds.h"
#include "w16types.h"
/* PORT: this file's own ColonyWin, GRPort, Sprite, Screen, DialogWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"

#pragma option -a

#pragma option -a-
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */


typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;    /* SEG20:0x5ca */

extern void MSEnableWindow(void *win, int on);

extern void MSSetFocus_of_1010_eb8f(Screen *w);
   /* DialogWin : EventWin, flattened for C */

extern void DialogWin_SetDefaultControlFocus(DialogWin *w);
extern int Popup_exec(Popup *popup, char *text);

extern GameState g_game;   /* SEG20:0x7782 */

extern int nth_noncarrier_in_chain(int u, int b);
extern int unit_is_active(int u);
extern void popup_say(int slot, int word);
extern int is_colonist(int u);
extern void space_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);

extern "C" int text_open(void *ctx, char *name);   /* PORT: C linkage, as defined (W2) */
extern "C" int UnitIcon(int u);   /* PORT: C linkage, as defined (W2) */
extern "C" void Popup_sprite(Popup *popup, Sprite *sprite, int id, char *s);   /* PORT: C linkage, as defined (W2) */
extern "C" char *text_get(void);   /* PORT: C linkage, as defined (W2) */
extern void expand_text_template(char *src, char *dst);
extern "C" void Popup_item(Popup *popup, char *s, int id);   /* PORT: C linkage, as defined (W2) */
extern void Popup_set_active(Popup *popup, int key);
extern void move_to_top(int u);
extern "C" void sound_play(int n);   /* PORT: C linkage, as defined (W2) */

/* The module's one-line wrappers are defined here because the shipped calls to them are
 * the compiler's own `push cs / call `; their own callees are not in this file. */

extern "C" void win_disable_1010_e459(Screen *w);

extern "C" void win_enable_1010_e4d4(Screen *w);

extern "C" void ColonyWin_ColonyClickUnits(ColonyWin *unused1)
{
    Popup popup(0x800);
    int r;
    int ok;
    int spec;
    char *line;
    char buf[0x50];
    int u;
    int i;

    win_disable_1010_e459((Screen *)&g_colony_win_rec);
    u = nth_noncarrier_in_chain(g_tile_first_unit, g_tile_sel);
    if (u < 0)
        goto done;
    if (unit_is_active(u))
        g_game.selected_unit = u;
    popup_say(0, g_unit_types[g_units[u].type].name);
    buf[0] = 0;
    if (is_colonist(u) != 0) {
        spec = g_units[u].spec;
        if (spec != JOB_COLONIST_ALT) {
            space_string(buf);
            open_string(buf);
            say_string(buf, g_spec_recs[g_units[u].spec].expert);
            close_string(buf);
        }
    }
    set_name_slot_1010_e658(1, buf);
    show_plain_step_1010_e484(&popup, g_game_txt, "COLONYUNIT");
    popup.flags |= 2;
    if (text_open(g_game_txt, "UNITOPTIONS") != 0)
        goto done;
    Popup_sprite(&popup, &g_icon_sprites[UnitIcon(u)], 0, 0);
    for (i = 0; i < 5; i++) {
        line = text_get();
        expand_text_template(line, buf);
        ok = 0;
        switch (i) {
        case 0:
            ok = g_tile_first_unit != u;
            break;
        case 1:
            ok = g_units[u].orders != ORDERS_NONE;
            break;
        case 2:
            ok = g_units[u].orders != ORDERS_SENTRY;
            break;
        case 3:
            ok = g_units[u].orders != ORDERS_FORTIFY && g_units[u].orders != ORDERS_FORTIFIED;
            break;
        case 4:
            ok = 1;
            break;
        }
        if (ok != 0) {
            Popup_item(&popup, buf, i);
            if (i == 4)
                Popup_set_active(&popup, i);
        }
    }
    r = popup_run_thunk_1010_e4b1(&popup);
    switch (r) {
    case 1:
        move_to_top(u);
        g_tile_first_unit = u;
        g_tile_sel = 0;
        break;
    case 2:
        g_units[u].orders = ORDERS_NONE;
        break;
    case 3:
        g_units[u].orders = ORDERS_SENTRY;
        break;
    case 4:
        g_units[u].orders = ORDERS_FORTIFY;
        sound_play(SOUND_COCKGUN);
        break;
    }
done:
    win_disable_1010_e459((Screen *)&g_main_win);
    win_enable_1010_e4d4((Screen *)&g_colony_win_rec);
    win_show_and_focus_1010_e4ff(&g_colony_win_rec);
}
