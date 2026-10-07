/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* ReportWin_ShipReportTop: candidate for 1058:6481 (691 bytes).  Verdict: RELOCATABLE.
 * symbol: ReportWin_ShipReportTop
 *
 * The naval report's frame: the title (word SEG20:0xdd30) centred in colour 249, then the
 * four column headings (words SEG20:0xdd42..0xdd48) in colour 12 at x 77, 227, 377, 527 on
 * row 42, three vertical rules 150 apart from y 40 to 368 and ten horizontal rules 35 apart
 * from y 53. The width wrappers are defined here because the shipped calls to them are the
 * compiler's own `push cs / call `.
 */

#include "colonization.h"

#include "w16types.h"
#include "misc.h"
#include "classes.h"

extern void set_europe(int n);

extern char *strings(int n);

extern void GRPort_DrawLine(GRPort *w, short x1, short y1, short x2, short y2);

extern int port_width_1058_bcfb(GRPort *port);

void ReportWin_ShipReportTop(ReportWin *o, int nation)
{
    int y;
    int y2;
    char buf[0x50];
    int x;
    int i;

    TextColor(0xf9);
    GRPort_String((GRPort *)o, (unsigned *)&g_report_font, strings(g_misc_words[MISC_NAVAL_ADVISOR_REPORT]), port_width_1058_be8e((GRPort *)o) / 2, o->rect.top + 18, 16);
    set_europe(nation);
    x = 2;
    y = 40;
    y2 = y + 2;
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_SHIP]);
    TextColor(12);
    GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, buf, o->rect.left + x + 75, o->rect.top + y2, 0);
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_CARGO]);
    TextColor(12);
    GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, buf, o->rect.left + x + 225, o->rect.top + y2, 0);
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_LOCATION]);
    TextColor(12);
    GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, buf, o->rect.left + x + 375, o->rect.top + y2, 0);
    buf[0] = 0;
    say_string(buf, g_misc_words[MISC_DESTINATION]);
    TextColor(12);
    GRPort_String((GRPort *)o, (unsigned *)&g_heading_font, buf, o->rect.left + x + 525, o->rect.top + y2, 0);
    for (i = 1; i <= 3; i++) {
        TextColor(0xf9);
        GRPort_DrawLine((GRPort *)o, o->rect.left + x + i * 150, o->rect.top + 40, o->rect.left + x + i * 150, o->rect.top + 368);
    }
    for (i = 0; i < 10; i++) {
        TextColor(0xf9);
        GRPort_DrawLine((GRPort *)o, o->rect.left + 2, o->rect.top + i * 35 + 53, o->rect.right - 2, o->rect.top + i * 35 + 53);
    }
}
