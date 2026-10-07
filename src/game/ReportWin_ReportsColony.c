/* PORT: this file's own declarations of FortDraw, GRPort_String, sprite_width_1058_c203 removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_ReportsColony: candidate for 1058:5ad6 (1788 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_ReportsColony
 * name-source: curated, matching `ReportWin::ReportsColony(short)` in the Macintosh sibling build
 *
 * THE COLONY ADVISOR REPORT, case 6 of the report dispatcher at 1058:0738, and view 0 of the two
 * the page can show. It lists the player's colonies eleven to a page: the colony's own picture,
 * its name, and then every military unit standing in it, drawn as a row of portraits.
 *
 * THE BUTTON BAR is a raised strip across the bottom 0x28 of the window carrying three buttons
 * laid out in thirds: "Previous Page" (id 0xc8) in the left third, "Sons of Liberty" (0xca) in the
 * middle and "Next Page" (0xc9) in the right. Previous and Next are created once and kept in the
 * window; the middle one, Sons of Liberty, is DELETED AND REBUILT on every call, because the other view of this page
 * puts a different caption in the same slot. Prev is greyed on page 0 and Next once the page
 * index reaches `colonies / 11`, which is why the colonies are counted through once before
 * anything is drawn.
 *
 * WHICH UNITS COUNT: those whose type record at SEG20:0x7ac8 has a non-zero byte at +6, minus the
 * band 0x0d to 0x12 -- the ships (UNIT_SHIP_FIRST..UNIT_SHIP_LAST), which have no place in a colony's garrison. They are
 * walked twice. The first pass adds up the width of each portrait plus one and divides by the
 * count to get a step; the second pass draws them at that step until x passes 0x258. The count
 * starts at ONE and not zero, so the step is always the average over one more unit than there
 * are, and the icons overlap a little on purpose -- the fuller the colony the tighter they pack.
 * The step is also clamped to 0x21, so a colony with one unit does not spread it out.
 *
 * WHEN A PAGE COMES UP EMPTY -- the last colony was deleted, or the player switched to a nation
 * with fewer -- the function steps the page index back and CALLS ITSELF, then returns. That is
 * the shipped recursion: `push cs / call ` on its own label.
 *
 * `rows` is counted and never read.
 *
 * WHAT THE BYTES SETTLED: the owner test is byte-wide on both sides, `colony->nation ==
 * (unsigned char)nation`, and either side promoting kills it. Both unit walks are FOR loops with
 * `continue`s, not `while`s with nested ifs -- the shipped code is short conditional jumps over a
 * jump (rule 12), which a structured if would have written as one long conditional. And the
 * recursive call is followed by an explicit `return`, which is the only reason the epilogue
 * appears twice.
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

extern void set_europe(int n);
extern void set_colony(int colony);

extern void GRPort_DrawRaisedRect(GRPort *w, RECT *r);

extern int top_unit_in_square(int x, int y);
extern int next_unit(int u);
extern int UnitIcon(int u);
extern void PieceDraw(GRPort *port, int u, int flags, int x, int y,
                                         int cx, int cy);

extern void ReportWin_OnColonyExtraClick(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void sbutton_init_1058_c11b(SButton *button, void *win, int id, RECT *pos, char *title);
extern "C" void set_button_action_1058_c17f(SButton *button, void *h);
extern "C" void ctl_flag_clear(SButton *control);
extern "C" void ctl_flag_set(SButton *ctl);
extern "C" void ReportWin_ColonyReportTop(ReportWin *o);

extern "C" void ReportWin_ReportsColony(ReportWin *o, int nation)
{
    register int u;
    register int x;
    int x0;
    int y;
    int total;
    int count;
    int i;
    int step;
    int rows;
    int n;
    RECT r;

    set_europe(nation);
    x0 = 2;
    y = 0x28;
    o->view = 0;
    SetRect(&r, o->rect.left, o->rect.bottom - 0x28, o->rect.right, o->rect.bottom);
    GRPort_DrawRaisedRect(o, &r);
    if (o->prev == 0) {
        SetRect(&r, o->rect.left + 5, o->rect.bottom - 0x23,
                port_width_1058_be8e((GRPort *)o) / 3 - 1, o->rect.bottom - 5);
        o->prev = new SButton;
        sbutton_init_1058_c11b(o->prev, (char *)o + 0x26, 0xc8, &r, "Previous Page");
        set_button_action_1058_c17f(o->prev, (void *)ReportWin_OnColonyPrevClick);
    }
    if (o->page == 0)
        ctl_flag_clear(o->prev);
    else
        ctl_flag_set(o->prev);
    if (o->next == 0) {
        SetRect(&r, port_width_1058_be8e((GRPort *)o) / 3 * 2 + 1, o->rect.bottom - 0x23,
                o->rect.right - 5, o->rect.bottom - 5);
        o->next = new SButton;
        sbutton_init_1058_c11b(o->next, (char *)o + 0x26, 0xc9, &r, "Next Page");
        set_button_action_1058_c17f(o->next, (void *)ReportWin_OnColonyNextClick);
    }
    if (o->extra_btn != 0)
        delete o->extra_btn;
    SetRect(&r, port_width_1058_be8e((GRPort *)o) / 3 + 1, o->rect.bottom - 0x23,
            port_width_1058_be8e((GRPort *)o) / 3 * 2 - 1, o->rect.bottom - 5);
    o->extra_btn = new SButton;
    sbutton_init_1058_c11b(o->extra_btn, (char *)o + 0x26, 0xca, &r, "Sons of Liberty");
    set_button_action_1058_c17f(o->extra_btn, (void *)ReportWin_OnColonyExtraClick);

    n = 0;
    for (i = 0; g_game.colony_count > i; i++) {
        set_colony(i);
        if (g_cur_colony_rec->nation == (unsigned char)nation)
            n++;
    }
    if (n / 0xb <= o->page)
        ctl_flag_clear(o->next);
    else
        ctl_flag_set(o->next);
    ReportWin_ColonyReportTop(o);
    rows = 0;
    n = -1;
    for (i = 0; g_game.colony_count > i; i++) {
        set_colony(i);
        if (g_cur_colony_rec->nation == (unsigned char)nation)
            n++;
        else
            continue;
        if (o->page * 0xb > n || o->page * 0xb + 0xb <= n)
            continue;
        x = x0;
        FortDraw(o, g_cur_colony_idx, o->rect.left + x + 2, o->rect.top + y, 8, 1, 0);
        x += 0x32;
        TextColor(0xc);
        GRPort_String(o, (unsigned *)&g_heading_font, g_cur_colony_rec->name,
                             o->rect.left + x + 3, o->rect.top + y + 0xf, 1);
        x += 0x78;
        total = 0;
        count = 1;
        for (u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0;
             u = next_unit(u)) {
            if (g_unit_types[g_units[u].type].attack < 1)
                continue;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                continue;
            total += sprite_width_1058_c203(&g_icon_sprites[UnitIcon(u)]) + 1;
            count++;
        }
        step = total / count;
        if (step > 0x21)
            step = 0x21;
        for (u = top_unit_in_square(g_cur_colony_rec->x, g_cur_colony_rec->y); u >= 0;
             u = next_unit(u)) {
            if (g_unit_types[g_units[u].type].attack < 1)
                continue;
            if (g_units[u].type >= UNIT_SHIP_FIRST && g_units[u].type <= UNIT_SHIP_LAST)
                continue;
            if (x > 0x258)
                continue;
            PieceDraw(o, u, 0, o->rect.left + x,
                                     o->rect.top + y, 0x20, 0x64);
            x += step;
        }
        rows++;
        y += 0x23;
    }
    if (o->page * 0xb > n && n != -1) {
        o->page--;
        ReportWin_ReportsColony(o, nation);
        return;
    }
}
