/* ReportWin_ReportsShip: candidate for 1058:6776 (2177 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_ReportsShip
 * name-source: sibling-build
 *
 * THE NAVAL ADVISOR REPORT, case 7 of the report dispatcher at 1058:0738, matching
 * `ReportWin::ReportsShip(short)` in the Macintosh build. Nine ships a page, each on a 0x23-pixel
 * row from y 0x37, with "Previous Page" (0xc8) and "Next Page" (0xc9) split down the MIDDLE of
 * the bottom strip -- two buttons in halves where the colony report has three in thirds.
 *
 * WHAT COUNTS AS A SHIP is either a unit whose type is in the band 0x0d..0x12 or any unit of the
 * player's standing on ocean. The list is scanned twice with that same filter: once to count the
 * ships so Next can be greyed at `ships / 9`, and once to draw. A ship in the band is drawn 0x10
 * from the left and one merely afloat 0xab, so the two kinds sit in different columns -- the band
 * is the ocean-going hulls and everything else is a land unit caught at sea.
 *
 * A ROW is the unit icon, its cargo, its type name, where it is, and where it is bound. The cargo
 * sprites start 0x9c across and step 0x18; each picks its art by ADDING the cargo type to a base
 * that is 0x27 for a load under a hundred and 0x17 for a full one, so a full hold and a part load
 * are two different pictures of the same commodity.
 *
 * WHERE IT IS BOUND is the interesting column. A unit in state 3, 0xb or 2 has a real destination
 * and `say_where` is asked for it. Anything else falls back on the unit's own position, and when
 * that is OFF THE MAP the position doubles as a code: `nation - 0x18` or `nation - 0x1c` means
 * sailing to the New World, so the nation's country name is copied in, and `nation - 0xc` or
 * `nation - 0x10` means sailing to Europe, so the nation word is said instead. Four sentinel
 * x-coordinates per nation, two for each direction. A unit off the map matching none of them
 * leaves the column blank.
 *
 * `rows` is counted and never read, the same dead counter the two colony views carry.
 *
 * WHAT THE BYTES SETTLED: `(unsigned char)(unit->flags & 0xf)` keeps the AND in AL, where the
 * uncast form would widen first and mask a word (rule 59) -- it is needed BOTH in the owner test
 * and as the `say_where` argument, which then widens the byte after the AND. The four sentinel
 * comparisons are char on both sides, `(char)(nation - 0x18) == (char)px`. The halves are `/ 2`
 * and not `>> 1`: `CWD / SUB AX,DX / SAR AX,1` is the signed divide.
 */

#include "colonization.h"

#include "w16types.h"
#include "units.h"
/* PORT: this file's own Font, DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, ReportWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"










class SButton;

#include "records.h"

#pragma option -a

#pragma option -a-

extern GameState g_game;   /* SEG20:0x7782 */

extern void GRPort_DrawRaisedRect(GRPort *w, RECT *r);

extern void GRPort_String(GRPort *port, unsigned *font, char *s,
                                     int x, int y, int size);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y,
                                         int cx, int cy);
extern int terrain_is_ocean(int x, int y);

extern int get_cargo(int u, int slot);
extern int get_cargo_amount(int u, int slot);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void sbutton_init_1058_c11b(SButton *button, void *win, int id, RECT *pos, char *title);
extern "C" void set_button_action_1058_c17f(SButton *button, void *h);
extern "C" void ctl_flag_clear(SButton *control);
extern "C" void ctl_flag_set(SButton *ctl);
extern "C" void ReportWin_ShipReportTop(ReportWin *o, int nation);

extern "C" void ReportWin_ReportsShip(ReportWin *o, int nation)
{
    register int u;
    register int cargo_slot;
    int x0;
    int y;
    int cx;
    int cy;
    int px;
    int py;
    int n;
    int rows;
    int base;
    int ctype;
    int amount;
    RECT r;
    RECT r2;
    char buf[0x50];

    SetRect(&r, o->rect.left, o->rect.bottom - 0x28, o->rect.right, o->rect.bottom);
    GRPort_DrawRaisedRect(o, &r);
    if (o->prev == 0) {
        SetRect(&r, o->rect.left + 5, o->rect.bottom - 0x23,
                port_width_1058_be8e((GRPort *)o) / 2 - 1, o->rect.bottom - 5);
        o->prev = new SButton;
        sbutton_init_1058_c11b(o->prev, (char *)o + 0x26, 0xc8, &r, "Previous Page");
        set_button_action_1058_c17f(o->prev, (void *)ReportWin_OnShipPrevClick);
    }
    if (o->page == 0)
        ctl_flag_clear(o->prev);
    else
        ctl_flag_set(o->prev);
    if (o->next == 0) {
        SetRect(&r, port_width_1058_be8e((GRPort *)o) / 2 + 1, o->rect.bottom - 0x23,
                o->rect.right - 5, o->rect.bottom - 5);
        o->next = new SButton;
        sbutton_init_1058_c11b(o->next, (char *)o + 0x26, 0xc9, &r, "Next Page");
        set_button_action_1058_c17f(o->next, (void *)ReportWin_OnShipNextClick);
    }
    n = -1;
    for (u = 0; g_game.unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) != (unsigned char)nation)
            continue;
        if ((g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            && terrain_is_ocean(g_units[u].x, g_units[u].y) == 0)
            continue;
        n++;
    }
    if (n / 9 <= o->page)
        ctl_flag_clear(o->next);
    else
        ctl_flag_set(o->next);
    ReportWin_ShipReportTop(o, nation);
    x0 = 2;
    y = 0x37;
    rows = 0;
    n = -1;
    for (u = 0; g_game.unit_count > u; u++) {
        if ((unsigned char)(g_units[u].flags & UNIT_NATION_MASK) != (unsigned char)nation)
            continue;
        if ((g_units[u].type < UNIT_SHIP_FIRST || g_units[u].type > UNIT_SHIP_LAST)
            && terrain_is_ocean(g_units[u].x, g_units[u].y) == 0)
            continue;
        n++;
        if (o->page * 9 > n || o->page * 9 + 9 <= n)
            continue;
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
            PieceDraw(o, u, 0, o->rect.left + x0 + 0x10,
                                     o->rect.top + y, 0x20, 0x64);
        else
            PieceDraw(o, u, 0, o->rect.left + x0 + 0xab,
                                     o->rect.top + y, 0x20, 0x64);
        cx = x0 + 0x9c;
        cy = y + 0x13;
        px = cx;
        for (cargo_slot = 0; g_units[u].cargo_count > cargo_slot; cargo_slot++) {
            ctype = get_cargo(u, cargo_slot);
            amount = get_cargo_amount(u, cargo_slot);
            base = amount < 0x64 ? 0x27 : 0x17;
            Sprite_Draw(&r2, &g_icon_sprites[base + ctype], o, o->rect.left + px + 8,
                        o->rect.top + cy - 0x14);
            px += 0x18;
        }
        cy = y + 0x16;
        cx = x0 + 0x23;
        buf[0] = 0;
        say_string(buf, g_unit_types[g_units[u].type].name);
        if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST) {
            TextColor(0xff);
            GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + cx + 0x10,
                                 o->rect.top + cy, 1);
        } else {
            TextColor(0xff);
            GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + cx + 0xab,
                                 o->rect.top + cy, 1);
        }
        cx = x0 + 0x12c;
        px = g_units[u].x;
        py = g_units[u].y;
        buf[0] = 0;
        say_where(buf, px, py, (unsigned char)(g_units[u].flags & UNIT_NATION_MASK), 0);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + cx + 0x4b,
                             o->rect.top + cy, 0);
        cx = x0 + 0x1c2;
        if (g_units[u].orders == 3 || g_units[u].orders == ORDERS_MISSION
            || g_units[u].orders == ORDERS_TRADE_ROUTE) {
            px = g_units[u].dest_x;
            py = g_units[u].dest_y;
            buf[0] = 0;
            say_where(buf, px, py, nation, 1);
            TextColor(0xff);
            GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + cx + 0x4b,
                                 o->rect.top + cy, 0);
        } else if (on_map(px, py) == 0) {
            buf[0] = 0;
            if ((char)(nation - 0x18) == (char)px || (char)(nation - 0x1c) == (char)px)
                STRCPY(buf, g_nation_text[nation].country);
            else if ((char)(nation - 0xc) == (char)px || (char)(nation - 0x10) == (char)px)
                say_string(buf, g_nation_name_word[nation]);
            TextColor(0xff);
            GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + cx + 0x4b,
                                 o->rect.top + cy, 0);
        }
        rows++;
        y += 0x23;
    }
    if (o->page * 9 > n && n != -1) {
        o->page--;
        ReportWin_ReportsShip(o, nation);
        return;
    }
}
