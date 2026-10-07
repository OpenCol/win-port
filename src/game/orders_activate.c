/* orders_activate: candidate for 1008:114a (885 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: orders_activate
 *
 * Clicking a tile with several of your units on it. The stack is walked from
 * `top_unit` and each unit gets a line -- nation adjective, type name, and for a
 * colonist its profession in parentheses -- up to TEN, after which the list stops. The
 * chosen unit is activated; returns 0 when one was, 1 otherwise.
 *
 * A STACK OF ONE SKIPS THE MENU. If the tail's `link2` is negative and the unit is either
 * selectable or already has orders, its orders are cleared and it is activated directly.
 * That is why the function can be called on every click without a popup appearing.
 *
 * The profession line has three cases and they are not symmetric: a colonist whose CLASS
 * differs from its `spec` byte prints the spec's name (or MISC word 3, MISC_NONE, at SEG20:0xdcce when the spec is
 * 0x1c), and one whose class matches prints MISC word 4, MISC_EXPERT, at SEG20:0xdcd0 instead. So the parenthesis says
 * what the unit is doing when that differs from what it is, and a fixed word when it does
 * not.
 *
 * NAME: descriptive. The Macintosh `stack_units(short)` takes ONE argument and
 * `tools/signatures.py` reads four argument bytes -- two ints, a tile x and y -- off both
 * call sites, so the arity excludes it.
 *
 * Five spellings the bytes fixed, and four are about width.
 *
 * ONE REGISTER DOES TWO JOBS: `u = top_unit(u)` overwrites the unit the click found,
 * so DI holds "the current unit" throughout and SI is the second variable assigned from it.
 * Writing the tail into a new variable puts it in SI and moves every later `MOV BX,DI` to
 * `MOV BX,SI`.
 *
 * `g_game.turn_nation` is a BYTE in this module: the shipped compares `AL` against it directly,
 * where the `int` five other files declare promotes the nibble first.
 *
 * `Unit+0x17` is a SIGNED char here -- the shipped loads it with `CBW` before comparing
 * against 0x1c -- and the class comparison beside it is 8-bit, `(char)class != spec`, where
 * the uncast form promotes both sides.
 *
 * And `GameBrains_EndUnitTurn` is the five-byte `NOP / push cs / call ` form, which is
 * TLINK converting a call, so `extern` and not an in-file stub.
 *
 * RENAMED 2026-09-26 from pick_unit_in_stack: the Mac build's orders_activate(short, short), bound by call-graph agreement (tools/mac_callgraph.py): 3 of its Mac call edges already bound land on this function (caller KeyboardEvent; caller UnitActivate; callee sprite), the runner-up gets 1, and the arity agrees (2).
 */

#include "colonization.h"

#include "units.h"
#include "jobs.h"
#include "misc.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */
/* 0x1c bytes at SEG20:0xa208 */

#define NATION(u) ((unsigned char)((u).flags & 0xf))

extern GameBrains g_game_brains;       /* SEG20:0x40d6, declared */
extern GameState g_game;   /* SEG20:0x7782 */

extern int top_unit_in_square(int x, int y);
extern "C" void sort_stack_by_class(int u);   /* PORT: C linkage, as defined (W2) */
extern int top_unit(int u);
extern int unit_is_active(int u);
extern char *strings(int n);

extern void space_string(char *buf);
extern int job_of_unit(int u);
extern "C" void Popup_sprite(Popup *popup, Sprite *art, int id, char *s);   /* PORT: C linkage, as defined (W2) */
extern "C" int UnitIcon(int u);   /* PORT: C linkage, as defined (W2) */
extern int next_unit(int u);
extern void DrawMapRegion(int x, int y, int a, int b, int flag);
extern void unit_activate(int u);
extern "C" int GameBrains_EndUnitTurn(void *brains, int force);   /* PORT: C linkage, as defined (W2) */

extern "C" void popup_setup_plain_1008_ce18(Popup *p);

int orders_activate(int x, int y)
{
    int result = 1;
    int i;
    int pick;
    int count = 0;
    int nameid;
    char buf[0x50];
    Popup p(0x800);
    register int u;
    register int unit;

    u = top_unit_in_square(x, y);
    if (u < 0)
        goto out;
    if (NATION(g_units[u]) != ((unsigned char)g_game.turn_nation))
        goto out;
    sort_stack_by_class(u);
    u = top_unit(u);
    if (g_units[u].link2 < 0) {
        if (unit_is_active(u) != 0 || g_units[u].orders != ORDERS_NONE) {
            g_units[u].orders = ORDERS_NONE;
            unit = u;
            goto activate;
        }
    }
    popup_setup_plain_1008_ce18(&p);
    unit = u;
    i = 1;
    do {
        STRCPY(buf, strings(g_nation_adjective[NATION(g_units[unit])]));
        space_string(buf);
        STRCAT(buf, strings(g_unit_types[g_units[unit].type].name));
        if (job_of_unit(unit) >= 0) {
            STRCAT(buf, " (");
            if ((char)job_of_unit(unit) != g_units[unit].spec) {
                if (g_units[unit].spec == JOB_COLONIST_ALT)
                    nameid = g_misc_words[MISC_NONE];
                else
                    nameid = g_spec_recs[g_units[unit].spec].name;
            } else {
                nameid = g_misc_words[MISC_EXPERT];
            }
            STRCAT(buf, strings(nameid));
            STRCAT(buf, ")");
        }
        Popup_sprite(&p, &g_icon_sprites[UnitIcon(unit)], i++, buf);
        unit = next_unit(unit);
        count++;
    } while (unit >= 0 && count < 0xa);
    pick = popup_run_thunk_1008_cd83(&p);
    if (pick < 1)
        goto out;
    unit = u;
    while (pick > 1) {
        unit = next_unit(unit);
        pick--;
    }
activate:
    if (g_game.selected_unit >= 0) {
        g_unit_blinking = 0;
        DrawMapRegion(g_units[g_game.selected_unit].x, g_units[g_game.selected_unit].y, 1, 1, 1);
        g_unit_blinking = 1;
    }
    unit_activate(unit);
    if (g_game.selected_unit >= 0)
        g_units[g_game.selected_unit].orders = ORDERS_NONE;
    GameBrains_EndUnitTurn(&g_game_brains, 0);
    result = 0;
out:
    return result;
}

