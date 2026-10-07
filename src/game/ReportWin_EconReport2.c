/* ReportWin_EconReport2: candidate for 1058:49ea (764 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * symbol: ReportWin_EconReport2
 *
 * The economy report page: the nation's colonies are counted (17 per page) to show or hide
 * the Next button (+0x8b0) and show Previous (+0x8ac), a raised 40-pixel strip is drawn along
 * the bottom, the grid comes from ReportWin_EconReport3, and the page's 17 colonies are
 * listed 20 apart from y 79 -- the name in colour 252 at x 2, then the 16 stock counts
 * (words at +0x9a of the colony record) 30 apart from x 145, right-shifted 4 per missing
 * digit, in colour 0 for empty, 12 for some, 252 for 100 or more and 255 for thousands
 * (written as "n K" with the word at SEG20:0xdd3a). A page past the last colony steps back
 * and redraws itself. ReportWin_EconReport3 and the module's wrappers are defined here
 * because the shipped calls to them are the compiler's own `push cs / call `.
 *
 * RENAMED 2026-09-26 from draw_econ_report_paged: the Mac build's ReportWin::EconReport2(short). Mac EconReport3 has exactly one caller, EconReport2, and this function is draw_econ_report_block's (now ReportWin_EconReport3's) only caller; the two also share their other callees -- DrawRaisedRect, GRPort::String, itoa, say_string, set_colony, space_string. That settles the EconReport2/3 split RE_STATUS had left open.
 */

#include "colonization.h"

#include "w16types.h"
#include "goods.h"
#include "misc.h"
#include "classes.h"
#include "records.h"

extern GameState g_game;   /* SEG20:0x7782 */
#include "icons.h"

extern void set_colony(int colony);
extern void set_europe(int n);

extern char *strings(int n);
extern void GRPort_String(GRPort *port, unsigned *font, char *s, int x, int y, int size);
extern void GRPort_DrawLine(GRPort *w, short x1, short y1, short x2, short y2);
extern void GRPort_DrawRaisedRect(GRPort *w, RECT *r);
extern void SetGlobalSpriteScale(int num, int den);
extern RECT *Sprite_Draw(RECT *out, Sprite *s, GRPort *port, int px, int py);

extern void space_string(char *buf);

extern int port_width_1058_bcfb(GRPort *port);

extern void ctl_flag_set(SButton *ctl);

extern void ctl_flag_clear(SButton *control);

extern void ReportWin_EconReport3(ReportWin *o, int nation);

void ReportWin_EconReport2(ReportWin *o, int nation)
{
    int x;
    int y;
    int xx;
    int colony;
    int rows;
    int color;
    int goods;
    int count;
    RECT rc;
    char buf[0x50];
    int xs;
    int amount;

    count = 0;
    for (colony = 0; g_game.colony_count > colony; colony++) {
        set_colony(colony);
        if ((char)g_cur_colony_rec->nation == (char)nation)
            count++;
    }
    if (count / 17 > o->page)
        ctl_flag_set(o->next);
    else
        ctl_flag_clear(o->next);
    ctl_flag_set(o->prev);
    SetRect(&rc, o->rect.left, o->rect.bottom - 40, o->rect.right, o->rect.bottom);
    GRPort_DrawRaisedRect((GRPort *)o, &rc);
    ReportWin_EconReport3(o, nation);
    x = 2;
    y = 79;
    rows = 0;
    count = -1;
    for (colony = 0; g_game.colony_count > colony; colony++) {
        set_colony(colony);
        if ((char)g_cur_colony_rec->nation == (char)nation)
            count++;
        else
            continue;
        if (o->page * 17 - 17 > count || o->page * 17 < count)
            continue;
        TextColor(0xfc);
        GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, (char *)g_cur_colony_rec + 2, o->rect.left + x, o->rect.top + y + 2, 1);
        xx = 145;
        for (goods = 0; goods < 16; goods++) {
            amount = g_cur_colony_rec->stock[goods];
            LTOA1(amount, buf, 10);
            color = 0;
            if (amount != 0)
                color = 12;
            if (amount >= 100)
                color = 0xfc;
            xs = xx;
            if (amount < 1000)
                xs += 4;
            if (amount < 100)
                xs += 4;
            if (amount < 10)
                xs += 4;
            if (amount >= 1000) {
                LTOA1(amount / 1000, buf, 10);
                space_string(buf);
                say_string(buf, g_misc_words[MISC_K]);
                color = 0xff;
                xs += 4;
            }
            TextColor(color);
            GRPort_String((GRPort *)o, (unsigned *)&g_number_font, buf, o->rect.left + xs, o->rect.top + y + 2, 1);
            xx += 30;
        }
        rows++;
        y += 20;
    }
    if (o->page * 17 - 17 >= count) {
        o->page--;
        ReportWin_EconReport2(o, nation);
        return;
    }
}
