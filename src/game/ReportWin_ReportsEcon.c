/* PORT: a window's DialogWin base is PORT_DIALOG_BASE (sizeof(GRPort)), not the game's 0x26: 12 bytes further on 64-bit (W9). */
/* PORT: this file's own declarations of GRPort_String, MSGetTextLength_1058_be29 removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_ReportsEcon: candidate for 1058:3c8a (2216 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_ReportsEcon
 * name-source: sibling-build
 *
 * THE ECONOMIC ADVISOR REPORT, case 5 of the report dispatcher at 1058:0738, matching
 * `ReportWin::ReportsEcon(short)` in the Macintosh build. Sixteen commodities, one 0x14-pixel row
 * each, ruled into a table: the goods name, the tons traded (Nation +0xbc, under "TONS"), the gold
 * they brought (+0x7c, under "GOLD", with the currency sign), and the two European prices. The
 * members were `tax_paid` and `traded` until 2026-09-27; their writers settle it -- plan_move adds
 * the cargo amount to +0xbc and value x amount to +0x7c.
 *
 * IT IS NOT A PAGED REPORT, although it carries the paging buttons. Both buttons are DELETED AND
 * REBUILT on every call, the page index is forced back to 0, and "Previous Page" is greyed
 * immediately -- so the pair exists only to keep the bottom strip looking like the other reports.
 * Sixteen goods fit on one page and always will.
 *
 * NUMBERS OVER TEN THOUSAND are divided by a thousand and a suffix word is appended, so a large
 * figure reads as thousands. The suffix is built in a separate 40-byte buffer and concatenated
 * onto the shared message buffer afterwards, because the buffer helpers only append. Every number
 * in the table is RIGHT-ALIGNED by measuring the finished string and subtracting its width from a
 * fixed column edge, which is why `MSGetTextLength_1058_be29` is called once per cell.
 *
 * THE DEAD COLOUR: each value computes a colour into a local -- 2 for positive, 4 for negative,
 * plus 8 when the figure was scaled to thousands, and 7 for the price columns -- and NOTHING EVER
 * READS IT. Every cell is drawn with the literal 12. So the report was meant to colour gains and
 * losses differently and ships without doing it; the arithmetic that would have chosen the colour
 * is all still there.
 *
 * WHAT THE BYTES SETTLED: `v = v / 0x3e8` and not `v /= 0x3e8` -- the compound form loads the
 * divisor before the dividend (rule 78). And the two sign-flips are written in opposite orders:
 * the tons column negates and then sets the colour, the gold column sets the colour and then
 * negates. That is two statements swapped in the source, visible because the stores land in a
 * different order.
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
/* PORT: this file's own Font, DlgControl, SButton, Screen, EventTable, EventWin, DialogWin, GRPort, GameWin, SImageButton, ColChild, ReportWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"










class SButton;

#include "records.h"

extern Nation g_nations[];           /* SEG20:0xc2f4, 0x13c bytes each */

extern void set_europe(int n);

extern void GRPort_DrawRaisedRect(GRPort *w, RECT *r);
extern void GRPort_DrawLine(GRPort *w, short x1, short y1, short x2, short y2);

extern char *strings(int n);

extern int europe_bid_price(int g);
extern int europe_ask_price(int g);

/* same object: 4-byte `push cs; call ` (rule 47) */


extern "C" void sbutton_init_1058_c11b(SButton *button, void *win, int id, RECT *pos, char *title);
extern "C" void set_button_action_1058_c17f(SButton *button, void *h);
extern "C" void ctl_flag_clear(SButton *control);

extern "C" void ReportWin_ReportsEcon(ReportWin *o, int nation)
{
    register int i;
    register int cx;
    int x;
    int y;
    int y0;
    int w;
    int colour;
    long amount;
    RECT r;
    char buf[0x28];

    set_europe(nation);
    if (o->prev != 0)
        delete o->prev;
    if (o->next != 0)
        delete o->next;
    o->page = 0;
    SetRect(&r, o->rect.left, o->rect.bottom - 0x28, o->rect.right, o->rect.bottom);
    GRPort_DrawRaisedRect(o, &r);
    SetRect(&r, o->rect.left + 5, o->rect.bottom - 0x23,
            port_width_1058_be8e((GRPort *)o) / 2 - 1, o->rect.bottom - 5);
    o->prev = new SButton;
    sbutton_init_1058_c11b(o->prev, (char *)o + PORT_DIALOG_BASE, 0xc8, &r, "Previous Page");
    set_button_action_1058_c17f(o->prev, (void *)ReportWin_OnEconPrevClick);
    ctl_flag_clear(o->prev);
    SetRect(&r, port_width_1058_be8e((GRPort *)o) / 2 + 1, o->rect.bottom - 0x23,
            o->rect.right - 5, o->rect.bottom - 5);
    o->next = new SButton;
    sbutton_init_1058_c11b(o->next, (char *)o + PORT_DIALOG_BASE, 0xc9, &r, "Next Page");
    set_button_action_1058_c17f(o->next, (void *)ReportWin_OnEconNextClick);
    TextColor(0xf9);
    GRPort_String(o, (unsigned *)&g_report_font, strings(g_misc_words[MISC_ECONOMIC_ADVISOR_REPORT]),
                         port_width_1058_be8e((GRPort *)o) / 2, o->rect.top + 0x12, 0x10);
    x = 0x78;
    y = 0x2e;
    y0 = y;
    TextColor(0xf9);
    GRPort_DrawLine(o, o->rect.left + x - 3, o->rect.top + 0x32,
                    o->rect.left + x - 3, o->rect.top + 0x17a);
    message_buf_clear();
    say_string(message_buf_1058_bec6(), g_misc_words[MISC_TONS]);
    TextColor(0xc);
    GRPort_String(o, (unsigned *)&g_font_small, message_buf_1058_bec6(),
                         o->rect.left + x + 6, o->rect.top + y0, 1);
    message_buf_clear();
    say_string(message_buf_1058_bec6(), g_misc_words[MISC_GOLD]);
    w = MSGetTextLength_1058_be29(&g_font_small, message_buf_1058_bec6()) - 1;
    cx = x + 0x78 - w;
    GRPort_String(o, (unsigned *)&g_font_small, message_buf_1058_bec6(),
                         o->rect.left + cx + 0x14, o->rect.top + y0, 1);
    x += 0xf5;
    cx = x;
    message_buf_clear();
    say_string(message_buf_1058_bec6(), g_misc_words[MISC_BID_PRICE]);
    GRPort_String(o, (unsigned *)&g_font_small, message_buf_1058_bec6(),
                         o->rect.left + cx, o->rect.top + y0, 1);
    cx += 0x69;
    message_buf_clear();
    say_string(message_buf_1058_bec6(), g_misc_words[MISC_ASK_PRICE]);
    GRPort_String(o, (unsigned *)&g_font_small, message_buf_1058_bec6(),
                         o->rect.left + cx, o->rect.top + y0, 1);
    TextColor(0xf9);
    for (i = 0; i < 0x11; i++)
        GRPort_DrawLine(o, o->rect.left + 2, o->rect.top + i * 0x14 + 0x3a,
                        o->rect.right - 2, o->rect.top + i * 0x14 + 0x3a);
    x = 7;
    y = 0x44;
    for (i = 0; i < 0x10; i++) {
        message_buf_clear();
        say_string(message_buf_1058_bec6(), g_goods_name_word[i]);
        TextColor(0xc);
        GRPort_String(o, (unsigned *)&g_heading_font, message_buf_1058_bec6(),
                             o->rect.left + x, o->rect.top + y, 1);
        cx = 0x8c;
        amount = g_nations[nation].trade_tons[i];
        if (amount >= 0)
            colour = 2;
        else {
            amount = 0 - amount;
            colour = 4;
        }
        buf[0] = 0;
        if (amount >= 0x2710) {
            amount = amount / 0x3e8;
            say_string(buf, g_misc_words[MISC_K]);
            colour += 8;
        }
        message_buf_clear();
        say_long(message_buf_1058_bec6(), amount);
        message_buf_cat_1058_c084(buf);
        w = MSGetTextLength_1058_be29(&g_heading_font, message_buf_1058_bec6()) - 1;
        TextColor(0xc);
        GRPort_String(o, (unsigned *)&g_heading_font, message_buf_1058_bec6(),
                             o->rect.left + cx + 0x20 - w - 0xb, o->rect.top + y + 2, 1);
        amount = g_nations[nation].trade_gold[i];
        if (amount >= 0)
            colour = 2;
        else {
            colour = 4;
            amount = 0 - amount;
        }
        buf[0] = 0;
        if (amount >= 0x2710) {
            amount = amount / 0x3e8;
            say_string(buf, g_misc_words[MISC_K]);
            colour += 8;
        }
        message_buf_clear();
        say_long(message_buf_1058_bec6(), amount);
        message_buf_cat_1058_c084(buf);
        message_buf_cat_1058_c084("\244");
        w = MSGetTextLength_1058_be29(&g_heading_font, message_buf_1058_bec6()) - 1;
        GRPort_String(o, (unsigned *)&g_heading_font, message_buf_1058_bec6(),
                             o->rect.left + cx + 0x78 - w, o->rect.top + y + 2, 1);
        colour = 7;
        cx += 0xf0;
        message_buf_clear();
        say_bucks(message_buf_1058_bec6(), (long)europe_bid_price(i));
        w = MSGetTextLength_1058_be29(&g_heading_font, message_buf_1058_bec6()) - 1;
        GRPort_String(o, (unsigned *)&g_heading_font, message_buf_1058_bec6(),
                             o->rect.left + cx + 0x1d - w, o->rect.top + y + 2, 1);
        cx += 0x64;
        message_buf_clear();
        say_bucks(message_buf_1058_bec6(), (long)europe_ask_price(i));
        w = MSGetTextLength_1058_be29(&g_heading_font, message_buf_1058_bec6()) - 1;
        GRPort_String(o, (unsigned *)&g_heading_font, message_buf_1058_bec6(),
                             o->rect.left + cx + 0x1e - w, o->rect.top + y + 2, 1);
        y += 0x14;
    }
}
