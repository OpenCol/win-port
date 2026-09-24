/* ReportWin_ColonyReportTop -- shipped at 1058:59f5.
 *
 * Ported by tools/port.py from matched/game/ReportWin_ColonyReportTop.c.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern int far g_colony_report_title;                SEG20:0xdd2e
 *   extern char far g_report_font;                       DGROUP:0x4b26
 *
 * The file's own ReportWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_2_1058_be8e(ReportWin *w);

extern void TextColor(col_i16 v);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern char * nth_text_string(col_i16 n);


extern void draw_string_shadowed(ReportWin *w, void *font, char *s, col_i16 x, col_i16 y, col_i16 size);

void ReportWin_ColonyReportTop(ReportWin *w)
{
    TextColor(0xf9);
    draw_string_shadowed(w, &g_report_font, nth_text_string(g_colony_report_title),
                         get_w_2_1058_be8e(w) / 2, w->oy + 0x12, 0x10);
}
