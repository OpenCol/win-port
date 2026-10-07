/* PORT: this file's own declarations of FortDraw, GRPort_String removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_ColonyLibertyReport: candidate for 1058:51e2 (2067 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_ColonyLibertyReport
 * name-source: sibling-build
 *
 * THE SONS OF LIBERTY VIEW of the colony report -- view 1 of the two `ReportWin_OnColonyExtraClick`
 * switches between, and the twin of `ReportWin_ReportsColony`. The dispatcher never reaches it
 * directly; the only way in is the button on the other view. It sets the window's view flag to 1
 * where its twin sets it to 0, and that flag is what the toggle reads.
 *
 * THE SAME BUTTON BAR, laid out in thirds, with the same ids -- 0xc8 "Previous Page", 0xc9
 * "Next Page", 0xca for the toggle -- and the same delete-and-rebuild of the third button. Its
 * caption here is "Military Units", because a toggle is named for the view it takes you TO.
 * Everything down to the same eleven colonies a page and the same step-back-and-recurse when a
 * page comes up empty is shared with the other view; the two functions differ only in the row.
 *
 * A ROW is the colony picture, its name, the rebel icon (sprite 0x7c, SEG20:0x71c6), the Sons of Liberty membership as
 * a percentage, a one-word status, the bell sprite (SEG20:0x6a26, icon 0x3f), this turn's bell production, and then a
 * portrait for every statesman in the colony. The status is two flags read in order: flag 0x14
 * first, and only if that is clear flag 0x13, so one of them wins and the line can also be blank.
 * The statesmen are colonists whose job is 0x11, and each is drawn TWICE -- `Sprite_DrawMono` two
 * pixels left for the shadow, then `Sprite_Draw` at the true position -- 12 pixels apart.
 *
 * THE EXPENSIVE PART: every row calls `colony_indian_table`, `compute_colony_parameters` and
 * `compute_colony_yield` before drawing, a full recomputation of the colony's economy, and
 * the only thing the row takes from it is the bell count at SEG21:0x146. Eleven colonies a page
 * means eleven whole production passes per repaint.
 *
 * `rows` is counted and never read, exactly as in the other view.
 *
 * WHAT THE BYTES SETTLED, the same three as its twin: a byte-wide owner test on both sides, `for`
 * loops with `continue`s rather than nested ifs (rule 12's short jump over a jump), and an
 * explicit `return` after the recursive call, which is why the epilogue appears twice. One thing
 * differs: `ReportWin_ColonyReportTop` is reached here through the five-byte NOP form and is
 * EXTERNAL, where the other view -calls it in four (rule 47).
 *
 * RENAMED 2026-09-27 from report_sons_of_liberty_page: CONST PROPOSAL. Mac ReportWin::ColonyLibertyReport(short), CODE 18 0x4cf8, 2002 bytes. Callers (bsr): OnColonyPrevClick, OnColonyNextClick, OnColonyExtraClick, itself. Windows: ReportWin_OnColonyPrevClick, ReportWin_OnColonyNextClick, toggle_colony_report_view (= OnColonyExtraClick), itself. Callees: DrawRaisedRect, 3 buttons (SButton ctor/init/action), set_europe, set_colony x2, colony_indian_table, compute_colony_parameters, compute_colony_yield, FortDraw, TextColor/String x4, Sprite::Draw, colony_rebel, say_number x2, percent_string, has_building x2, say_string x2, get_job, get_icon, DrawMono, and bsr ColonyReportTop. The same on Windows. Arity: this+short against (ReportWin *o, int nation). Layout: after OnColonyExtraClick, before ColonyReportTop, on both. Size 2002 against 2067. (Mac edges marked bsr are BSR.L/BSR.W same-segment calls, which tools/mac_callgraph.py does not read; they were decoded from the CODE resource.)
 */

#include "colonization.h"

#include "goods.h"
#include "w16types.h"
#include "building.h"
#include "jobs.h"
#include "misc.h"
/* PORT: this file's own Font, DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, ReportWin, Sprite removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"










class SButton;

#include "records.h"
#pragma option -a

#pragma option -a-

#include "icons.h"
extern GameState g_game;   /* SEG20:0x7782 */

extern void set_europe(int n);
extern void set_colony(int colony);

extern void GRPort_DrawRaisedRect(GRPort *w, RECT *r);

extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);
extern RECT *Sprite_DrawMono(RECT *out, Sprite *sprite, GRPort *dst,
                                  int x, int y, unsigned char flags);

extern void percent_string(char *buf);
extern void colony_indian_table(void);
extern void compute_colony_parameters(void);
extern void compute_colony_yield(void);
extern int colony_rebel(void);
extern int has_building(int bit);
extern int get_job(int colonist);
extern int get_icon(int colonist);
extern void ReportWin_ColonyReportTop(ReportWin *o);

extern void ReportWin_OnColonyExtraClick(void);

/* same object: 4-byte `push cs; call ` (rule 47) */

extern "C" void sbutton_init_1058_c11b(SButton *button, void *win, int id, RECT *pos, char *title);
extern "C" void set_button_action_1058_c17f(SButton *button, void *h);
extern "C" void ctl_flag_clear(SButton *control);
extern "C" void ctl_flag_set(SButton *ctl);

extern "C" void ReportWin_ColonyLibertyReport(ReportWin *o, int nation)
{
    register int x;
    register int j;
    int margin;
    int y;
    int i;
    int rows;
    int job;
    int portrait;
    int n;
    RECT r;
    RECT r2;
    RECT r3;
    RECT r4;
    RECT r5;
    char buf[0x50];

    o->view = 1;
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
    sbutton_init_1058_c11b(o->extra_btn, (char *)o + 0x26, 0xca, &r, "Military Units");
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
    set_europe(nation);
    margin = 2;
    y = 0x28;
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
        colony_indian_table();
        compute_colony_parameters();
        compute_colony_yield();
        x = margin;
        FortDraw(o, g_cur_colony_idx, o->rect.left + x + 2, o->rect.top + y, 8, 1, 0);
        x += 0x32;
        TextColor(0xc);
        GRPort_String(o, (unsigned *)&g_heading_font, g_cur_colony_rec->name,
                             o->rect.left + x + 3, o->rect.top + y + 0xf, 1);
        x += 0xbe;
        Sprite_Draw(&r2, &g_icon_sprites[ICON_SOL_FLAG], o, o->rect.left + x, o->rect.top + y + 2);
        x += 0x18;
        buf[0] = 0;
        say_number(buf, colony_rebel());
        percent_string(buf);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + x + 3,
                             o->rect.top + y + 0xf, 1);
        x += 0x32;
        buf[0] = 0;
        if (has_building(BUILDING_NEWSPAPER))
            say_string(buf, g_buildings[BUILDING_NEWSPAPER].name);
        else if (has_building(BUILDING_PRINTING_PRESS))
            say_string(buf, g_misc_words[MISC_PRESS]);
        TextColor(0xfa);
        GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + x + 3,
                             o->rect.top + y + 0xf, 1);
        x += 0x96;
        Sprite_Draw(&r3, &g_icon_sprites[ICON_BELL], o, o->rect.left + x, o->rect.top + y + 2);
        x += 0x22;
        buf[0] = 0;
        say_number(buf, g_prod[PROD_BELLS]);
        TextColor(0xff);
        GRPort_String(o, (unsigned *)&g_heading_font, buf, o->rect.left + x + 3,
                             o->rect.top + y + 0xf, 1);
        x += 0x3c;
        for (j = 0; g_cur_colony_rec->pop > j; j++) {
            job = get_job(j);
            if (job != JOB_STATESMAN)
                continue;
            portrait = get_icon(j);
            Sprite_DrawMono(&r4, &g_icon_sprites[portrait], o, o->rect.left + x - 2,
                         o->rect.top + y + 1, 0);
            Sprite_Draw(&r5, &g_icon_sprites[portrait], o, o->rect.left + x,
                        o->rect.top + y + 1);
            x += 0xc;
        }
        rows++;
        y += 0x23;
    }
    if (o->page * 0xb > n && n != -1) {
        o->page--;
        ReportWin_ColonyLibertyReport(o, nation);
        return;
    }
}
