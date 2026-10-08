/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of GRPort_String, font_height_1058_be4f removed: the header declares them from the definition (W1 step 3). */
/* PORT: this file's own declarations of DrawIconRow, sprite_height_1058_c24c removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_ReportsContinental: candidate for 1058:7056 (2900 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_ReportsContinental
 * name-source: sibling-build
 *
 * THE CONTINENTAL CONGRESS ACTIVITIES PAGE, case 3 of the report dispatcher at 1058:0738. Four
 * stacked blocks, each a label line and a row of icons, laid out by one running `y` that every
 * block advances by the height of the sprite it just drew plus 0xc -- so a block that draws
 * nothing still leaves its gap, and a block that is skipped leaves none.
 *
 * ONE BUTTON, "View Congress Floor" (id 0xca), rebuilt from scratch on every call across the
 * whole bottom strip. Its handler takes no arguments, so the nation is stashed at DGROUP:0x4e06
 * between creating the button and wiring it -- the one assignment in this function that exists
 * purely to get a value past a callback signature.
 *
 * THE FIRST LINE depends on two game flags. Clear bit 0 and it names the next founding father in
 * parentheses, taking the name from the six-byte records at SEG20:0xd6fa; bit 0 set and bit 1
 * clear and it names the intervention power (SEG20:0x77d6) followed by the Intervention word; both set and the line starts empty. A third
 * flag, bit 5 of the next byte, appends "(short by N of M)" -- the debug breakdown of the bell
 * target against what the nation has.
 *
 * THE SECOND BLOCK splits the nation's POPULATION by the rebel sentiment held at SEG20:0x77d2:
 * the percentage and its complement are both printed with a per-cent sign, and the rebels and
 * tories are pushed as items 0x7c and 0x7d for the icon row. The population is forced to at least
 * 1. (Until 2026-09-27 the locals read as a tax rate, and `tory` held the rebels.)
 *
 * THE FORCE BLOCKS -- the Royal Expeditionary Force (SEG20:0x77dc) and the foreign intervention
 * force (SEG20:0x77e4) -- are drawn only when their four-entry table sums non-zero, and each pushes
 * four unit icons whose TYPES come from the unit-type table (6, 8, 11 and 18 for the King's; 9, 7,
 * 11 and 18 for the intervention) against amounts taken from the table in the order 0, 1, 3, 2. The last two are
 * transposed in BOTH blocks, which is either the tables being stored in a different order from
 * the display or the same slip made twice. DGROUP:0x1940 is raised around each of these two rows
 * and lowered again, and around neither of the other two.
 *
 * THE LAST BLOCK lists every founding father the nation already has, four across, wrapping by the
 * row font's height. The row counter it keeps while wrapping is incremented and never read.
 *
 * WHAT THE BYTES SETTLED: `royal_total = intervention_total = 0` and `nrow = ncol = 0` are CHAINED assignments,
 * and the chain's direction is visible -- the second name is stored first. The two `!= 0` tests
 * guarding the trade blocks and the founding-father test are long conditionals, so all three are
 * structured ifs and not `continue`s (rule 12).
 *
 * RENAMED 2026-09-27 from report_congress_activities: CONST PROPOSAL. Mac ReportWin::ReportsContinental(short), CODE 18 0x6942, 2576 bytes. Caller (bsr): ReallyDrawReport; Windows: ReportWin_ReallyDrawReport. Callees: SetGlobalSpriteScale, DrawRaisedRect, one button, set_europe, strings, colon_string, open/close_string, get_adjective x2, freedom_level, DrawIconRow, percent_string, ResetMultiIcon/AddMultiIcon/DrawMultiIconRow x3, get_true_adjective, has_father. 7056 calls the same, including nation_text_field2, which leads3 renames to get_true_adjective, x1. This is consistent with leads3's call-count match. Arity: this+short against (ReportWin *o, int nation). Layout: Mac ReportsShip, ShowFloor, ReportsContinental, TradeRoute1; Windows ReportWin_ReportsShip, 6ff7, 7056, ReportWin_TradeRoute1. Size 2576 against 2900. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "fathers.h"
#include "units.h"
#include "royal.h"
#include "gameflag.h"
#include "w16types.h"
#include "misc.h"
#pragma option -a
/* PORT: this file's own Sprite, Font, DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, ReportWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-











class SButton;

#include "records.h"

#include "icons.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern void GRPort_DrawRaisedRect(GRPort *w, RECT *r);
extern void SetGlobalSpriteScale(int num, int den);
extern void set_europe(int n);

extern char *strings(int n);

extern void space_string(char *buf);
extern void colon_string(char *buf);
extern void percent_string(char *buf);
extern void open_string(char *s);
extern void close_string(char *s);

extern int freedom_level(int nation);

extern int has_father(int nation, int bit);
extern void DrawMultiIconRow(GRPort *w, int x, int y, int cx, int gap);
extern void ResetMultiIcon(void);
extern void AddMultiIcon(int kind, int count, int part);
extern void ShowFloor(void);

/* same object: 4-byte `push cs; call ` (rule 47) */



extern "C" void sbutton_init_1058_c11b(SButton *button, void *win, int id, RECT *pos, char *title);
extern "C" void set_button_action_1058_c17f(SButton *button, void *h);

extern "C" void ReportWin_ReportsContinental(ReportWin *o, int nation)
{
    register int y;
    register int bit;
    int x;
    int cx;
    int cy;
    int target;
    int have;
    int population;
    int rebels;
    int tories;
    int nrow;
    int ncol;
    int royal_total;
    int intervention_total;
    RECT r;
    char buf[0x50];

    SetGlobalSpriteScale(1, 1);
    SetRect(&r, o->rect.left, o->rect.bottom - 0x28, o->rect.right, o->rect.bottom);
    GRPort_DrawRaisedRect(o, &r);
    if (o->extra_btn != 0)
        delete o->extra_btn;
    SetRect(&r, o->rect.left + 2, o->rect.bottom - 0x23, o->rect.right - 2, o->rect.bottom - 5);
    o->extra_btn = new SButton;
    sbutton_init_1058_c11b(o->extra_btn, (char *)o + PORT_DIALOG_BASE, 0xca, &r, "View Congress Floor");
    g_congress_nation = nation;
    set_button_action_1058_c17f(o->extra_btn, (void *)ShowFloor);
    set_europe(nation);
    TextColor(0xf9);
    GRPort_String(o, (unsigned *)&g_report_font, strings(g_misc_words[MISC_CONTINENTAL_CONGRESS_ACTIV]),
                         port_width_1058_be8e((GRPort *)o) / 2, o->rect.top + 0x12, 0x10);
    x = 0xa;
    y = 0x37;
    buf[0] = 0;
    if ((g_game.game_flags & GAMEFLAG_INDEPENDENCE) == 0) {
        say_string(buf, g_misc_words[MISC_NEXT_CONTINENTAL_CONGRESS]);
        colon_string(buf);
        if (g_cur_nation_rec->next_father >= 0) {
            open_string(buf);
            say_string(buf, g_fathers[g_cur_nation_rec->next_father].name);
            close_string(buf);
        }
    } else if ((g_game.game_flags & GAMEFLAG_INTERVENTION) == 0) {
        say_string(buf, get_adjective(g_game.intervention_nation));
        space_string(buf);
        say_string(buf, g_misc_words[MISC_INTERVENTION]);
        colon_string(buf);
    }
    target = freedom_level(nation);
    if (g_game.game_flags & GAMEFLAG_INTERVENTION)
        target = max_int_1058_c226(target, g_cur_nation_rec->bells);
    have = min_int_1058_be68(target, g_cur_nation_rec->bells);
    if (g_game.game_flags & GAMEFLAG_CHEAT_MENU) {
        space_string(buf);
        open_string(buf);
        say_number(buf, target - have);
        space_string(buf);
        say_string(buf, g_misc_words[MISC_IN]);
        space_string(buf);
        say_number(buf, target);
        close_string(buf);
    }
    TextColor(0xff);
    GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y, 1);
    y += font_height_1058_be4f(&g_font_small) + 2;
    DrawIconRow(o, 0x3f, target, have, o->rect.left + x + 0xa, o->rect.top + y - 5,
                            rect_width_1058_bd2d(&o->rect) - 0x14, 0, 0, 1);
    y += sprite_height_1058_c24c(&g_icon_sprites[0x3e]) + 2;
    y += 0xc;
    population = max_int_1058_c226(g_nation_population[nation], 1);
    rebels = (int)((long)population * (long)g_game.rebel_sentiment / 100);
    tories = population - rebels;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_REBEL]);
    space_string(buf);
    say_string(buf, g_misc_words[MISC_SENTIMENT]);
    colon_string(buf);
    say_number(buf, g_game.rebel_sentiment);
    percent_string(buf);
    space_string(buf);
    space_string(buf);
    space_string(buf);
    say_string(buf, g_misc_words[MISC_TORY]);
    space_string(buf);
    say_string(buf, g_misc_words[MISC_SENTIMENT]);
    colon_string(buf);
    say_number(buf, 0x64 - g_game.rebel_sentiment);
    percent_string(buf);
    TextColor(0xff);
    GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y, 1);
    y += font_height_1058_be4f(&g_font_small);
    ResetMultiIcon();
    AddMultiIcon(0x7c, rebels, 0);
    AddMultiIcon(0x7d, tories, 0);
    DrawMultiIconRow(o, o->rect.left + x + 0xa, o->rect.top + y - 5,
                             rect_width_1058_bd2d(&o->rect) - 0x1e, 4);
    y += sprite_height_1058_c24c(&g_icon_sprites[ICON_EMPTY_HOLD]) + 2;
    y += 0xc;
    royal_total = intervention_total = 0;
    for (bit = 0; bit < 4; bit++) {
        royal_total += g_game.royal_forces[bit];
        intervention_total += g_game.force[bit];
    }
    if (royal_total != 0) {
        buf[0] = 0;
        say_string(buf, get_true_adjective(nation));
        space_string(buf);
        say_string(buf, g_misc_words[MISC_EXPEDITIONARY_FORCE]);
        colon_string(buf);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y, 1);
        y += font_height_1058_be4f(&g_font_small) + 2;
        ResetMultiIcon();
        AddMultiIcon(g_unit_types[UNIT_REGULARS].icon, g_game.royal_forces[ROYAL_REGULARS], 0);
        AddMultiIcon(g_unit_types[UNIT_CAVALRY].icon, g_game.royal_forces[ROYAL_CAVALRY], 0);
        AddMultiIcon(g_unit_types[UNIT_ARTILLERY].icon, g_game.royal_forces[ROYAL_ARTILLERY], 0);
        AddMultiIcon(g_unit_types[UNIT_MAN_O_WAR].icon, g_game.royal_forces[ROYAL_MAN_O_WAR], 0);
        g_draw_labels = 1;
        DrawMultiIconRow(o, o->rect.left + x + 0xa, o->rect.top + y - 5,
                                 rect_width_1058_bd2d(&o->rect) - 0x1e, 4);
        g_draw_labels = 0;
        y += sprite_height_1058_c24c(&g_icon_sprites[ICON_EMPTY_HOLD]) + 2;
        y += 0xc;
    }
    if (intervention_total != 0) {
        buf[0] = 0;
        say_string(buf, get_adjective(g_game.intervention_nation));
        space_string(buf);
        say_string(buf, g_misc_words[MISC_INTERVENTION_FORCE]);
        colon_string(buf);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y, 1);
        y += font_height_1058_be4f(&g_font_small) + 2;
        ResetMultiIcon();
        AddMultiIcon(g_unit_types[UNIT_CONTINENTAL_ARMY].icon, g_game.force[ROYAL_REGULARS], 0);
        AddMultiIcon(g_unit_types[UNIT_CONTINENTAL_CAVALRY].icon, g_game.force[ROYAL_CAVALRY], 0);
        AddMultiIcon(g_unit_types[UNIT_ARTILLERY].icon, g_game.force[ROYAL_ARTILLERY], 0);
        AddMultiIcon(g_unit_types[UNIT_MAN_O_WAR].icon, g_game.force[ROYAL_MAN_O_WAR], 0);
        g_draw_labels = 1;
        DrawMultiIconRow(o, o->rect.left + x + 0xa, o->rect.top + y - 5,
                                 rect_width_1058_bd2d(&o->rect) - 0x1e, 2);
        g_draw_labels = 0;
        y += sprite_height_1058_c24c(&g_icon_sprites[ICON_EMPTY_HOLD]) + 2;
        y += 0xc;
    }
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_FOUNDING_FATHERS]);
    colon_string(buf);
    TextColor(0xff);
    GRPort_String(o, (unsigned *)&g_font_small, buf, o->rect.left + x, o->rect.top + y, 1);
    y += font_height_1058_be4f(&g_font_small) + 2;
    nrow = ncol = 0;
    cx = x;
    cy = y;
    for (bit = 0; bit < FATHER_COUNT; bit++) {
        if (has_father(nation, bit)) {
            buf[0] = 0;
            say_string(buf, g_fathers[bit].name);
            TextColor(0xc);
            GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + cx, o->rect.top + cy, 1);
            ncol++;
            cx += 0x96;
            if (ncol >= 4) {
                ncol = 0;
                nrow++;
                cy += font_height_1058_be4f(&g_heading_font) + 2;
                cx = x;
            }
        }
    }
}
