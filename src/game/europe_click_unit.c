/* europe_click_unit: candidate for 1048:587e (1880 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_click_unit
 * size-source: 1880, not the census 663; the census stops at the first jump table's dispatch.
 *   The two jump tables follow the epilogue behind one pad byte the object does not emit, so
 *   the 1881-byte gap is one more than compares (as in `CheatMenuColonist`).
 *
 * The Europe dock's options for one land unit, from TEXT resource "ARMOPTIONS" under the
 * "EUROPEARM" heading. The prices go into the heading first: 50 muskets, 100 tools and 50 horses,
 * each at the selling price when the unit already carries them and the buying price when it does
 * not. Then the twelve lines are offered only where they make sense for the unit:
 *
 *   1, 2  the unit's byte +8 is 1 / is not 1; choosing one sets it to 0 / to 1
 *   3     only when the dock index at DGROUP:0x4cee is past 0; it calls
 *         `move_to_top` and resets the index
 *   4, 5  arm (a colonist or scout) / disarm (a soldier or dragoon)
 *   6, 7  give tools (a colonist) / take them (a pioneer)
 *   8, 9  give horses (a colonist or soldier) / take them (a scout or dragoon)
 *   10    a colonist
 *   11    a missionary whose byte +0x17 is not 0x18
 *   12    always
 *
 * Buying lines are withdrawn from a unit whose byte +0x17 is 0x1b; buying AND selling lines are
 * withdrawn from a nation whose flag for that good (`boycott`) is set; buying lines are greyed
 * when the price is more than the treasury. The choice then changes the unit's type, moves the
 * gold and records the trade in the market, with a sound for muskets, horses and line 10.
 *
 * Every body in both switches is in value order, and each ends in a `break`. The exit is an
 * explicit `return;`, which is what restores the exception frame through DX (rule 41).
 *
 * RENAMED 2026-09-26 from europe_arm_unit: the Mac build's europe_click_unit(void), bound by call-graph agreement (tools/mac_callgraph.py): 4 of its Mac call edges already bound land on this function (caller EuropeMouseUnits; callee grey; callee item; callee sprite), the runner-up gets 2, and the arity agrees (0).
 */

#include "colonization.h"

#include "fathers.h"
#include "jobs.h"
#include "units.h"
#include "sounds.h"
#include "w16types.h"
#include "classes.h"
#include "records.h"
class Font; class GameWin; struct PopupItem; struct PopupSprite; struct PopupEdit; struct PopupLine; class SRadioSet; class SCheckBox; class SEditBox; class SButton; class SListBox; class SRegion;/* PORT: this file's own Popup removed: include/gen/classes.h define it once (W1 step 2). */

extern int text_open(void *unused, char *id);
extern int UnitIcon(int u);
extern void Popup_sprite(Popup *popup, Sprite *sprite, int id, char *s);
extern char *text_get(void);
extern void expand_text_template(char *src, char *dst);
extern void Popup_item(Popup *popup, char *s, int id);
extern void Popup_grey(Popup *p, int key, int on);
extern void move_to_top(int u);
extern void sound_play(int n);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" int europe_ask_price(int g);
extern "C" int europe_bid_price(int g);

extern "C" void set_text_arg_i_1048_db6a(int i, int v);
extern "C" int boycott(int bit);

extern "C" void traffic_from_europe(int good, int n);
extern "C" void traffic_to_europe(int good, int n);

void europe_click_unit(void)
{
    Popup p(0x800);
    int i;
    int ev;
    int price;
    char *line;
    int sell_horses;
    int sell_tools;
    int sell_muskets;
    int buy_horses;
    int buy_tools;
    int buy_muskets;
    char buf[0x50];
    register int u;
    register int ok;

    u = nth_land_unit(g_land_unit_index);
    if (u < 0)
        goto out;
    buy_muskets = europe_ask_price(15) * 50;
    buy_tools = europe_ask_price(14) * 100;
    buy_horses = europe_ask_price(8) * 50;
    sell_muskets = europe_bid_price(15) * 50;
    sell_tools = europe_bid_price(14) * 100;
    sell_horses = europe_bid_price(8) * 50;
    text_plain_step_1048_de5f(&p, g_game_txt, "EUROPEARM", 0);
    if (g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS)
        set_text_arg_i_1048_db6a(0, sell_muskets);
    else
        set_text_arg_i_1048_db6a(0, buy_muskets);
    if (g_units[u].type == UNIT_PIONEERS)
        set_text_arg_i_1048_db6a(1, sell_tools);
    else
        set_text_arg_i_1048_db6a(1, buy_tools);
    if (g_units[u].type == UNIT_SCOUTS || g_units[u].type == UNIT_DRAGOONS)
        set_text_arg_i_1048_db6a(2, sell_horses);
    else
        set_text_arg_i_1048_db6a(2, buy_horses);
    p.flags |= 1;
    p.flags |= 2;
    if (text_open(g_game_txt, "ARMOPTIONS") != 0)
        goto out;
    Popup_sprite(&p, &g_icon_sprites[UnitIcon(u)], 0, 0);
    for (i = 1; i <= 12; i++) {
        line = text_get();
        expand_text_template(line, buf);
        price = 0;
        switch (i) {
        case 1:
            ok = g_units[u].orders == ORDERS_SENTRY;
            break;
        case 2:
            ok = g_units[u].orders != ORDERS_SENTRY;
            break;
        case 3:
            ok = g_land_unit_index > 0;
            break;
        case 4:
            ok = g_units[u].type == UNIT_COLONISTS || g_units[u].type == UNIT_SCOUTS;
            if ((int)g_units[u].spec == JOB_CONVERT)
                ok = 0;
            price = buy_muskets;
            if (boycott(FATHER_THOMAS_JEFFERSON))
                ok = 0;
            break;
        case 5:
            ok = g_units[u].type == UNIT_SOLDIERS || g_units[u].type == UNIT_DRAGOONS;
            if (boycott(FATHER_THOMAS_JEFFERSON))
                ok = 0;
            break;
        case 6:
            ok = g_units[u].type == UNIT_COLONISTS;
            if ((int)g_units[u].spec == JOB_CONVERT)
                ok = 0;
            if (boycott(FATHER_JOHN_PAUL_JONES))
                ok = 0;
            price = buy_tools;
            break;
        case 7:
            ok = g_units[u].type == UNIT_PIONEERS;
            if (boycott(FATHER_JOHN_PAUL_JONES))
                ok = 0;
            break;
        case 8:
            ok = g_units[u].type == UNIT_COLONISTS || g_units[u].type == UNIT_SOLDIERS;
            price = buy_horses;
            if ((int)g_units[u].spec == JOB_CONVERT)
                ok = 0;
            if (boycott(FATHER_HENRY_HUDSON))
                ok = 0;
            break;
        case 9:
            ok = g_units[u].type == UNIT_SCOUTS || g_units[u].type == UNIT_DRAGOONS;
            if (boycott(FATHER_HENRY_HUDSON))
                ok = 0;
            break;
        case 10:
            ok = g_units[u].type == UNIT_COLONISTS;
            if ((int)g_units[u].spec == JOB_CONVERT)
                ok = 0;
            break;
        case 11:
            ok = g_units[u].type == UNIT_MISSIONARIES && (int)g_units[u].spec != JOB_MISSIONARY;
            break;
        case 12:
            ok = 1;
            break;
        }
        if (ok) {
            Popup_item(&p, buf, i);
            if (price != 0 && (long)price > g_cur_nation_rec->gold)
                Popup_grey(&p, i, 1);
        }
    }
    ev = popup_run_thunk_1048_de13(&p) - 1;
    switch (ev) {
    case 1:
        g_units[u].orders = ORDERS_NONE;
        break;
    case 2:
        g_units[u].orders = ORDERS_SENTRY;
        break;
    case 3:
        move_to_top(u);
        g_land_unit_index = 0;
        break;
    case 4:
        sound_play(SOUND_COCKGUN);
        if (g_units[u].type == UNIT_SCOUTS)
            g_units[u].type = UNIT_DRAGOONS;
        else
            g_units[u].type = UNIT_SOLDIERS;
        g_cur_nation_rec->gold -= buy_muskets;
        traffic_from_europe(15, 50);
        break;
    case 5:
        if (g_units[u].type == UNIT_DRAGOONS)
            g_units[u].type = UNIT_SCOUTS;
        else
            g_units[u].type = UNIT_COLONISTS;
        g_cur_nation_rec->gold += sell_muskets;
        traffic_to_europe(15, 50);
        break;
    case 6:
        g_units[u].type = UNIT_PIONEERS;
        g_units[u].hold.w.tools = 100;
        g_cur_nation_rec->gold -= buy_tools;
        traffic_from_europe(14, 100);
        break;
    case 7:
        g_units[u].type = UNIT_COLONISTS;
        g_cur_nation_rec->gold += sell_tools;
        traffic_to_europe(14, 100);
        break;
    case 8:
        if (g_units[u].type == UNIT_SOLDIERS)
            g_units[u].type = UNIT_DRAGOONS;
        else
            g_units[u].type = UNIT_SCOUTS;
        sound_play(SOUND_HORSES);
        traffic_from_europe(8, 50);
        g_cur_nation_rec->gold -= buy_horses;
        break;
    case 9:
        if (g_units[u].type == UNIT_DRAGOONS)
            g_units[u].type = UNIT_SOLDIERS;
        else
            g_units[u].type = UNIT_COLONISTS;
        g_cur_nation_rec->gold += sell_horses;
        traffic_to_europe(8, 50);
        break;
    case 10:
        g_units[u].type = UNIT_MISSIONARIES;
        sound_play(SOUND_AMEN);
        break;
    case 11:
        g_units[u].type = UNIT_COLONISTS;
        break;
    }
out:
    return;
}

