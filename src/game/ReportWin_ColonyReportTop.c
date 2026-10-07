/* PORT: this file's own declarations of GRPort_String removed: the header declares it from the definition (W1 step 3). */
/* PORT: explicit pointer casts where Borland C++ converted implicitly (W1), lines 24 of the imported file. */
/* ReportWin_ColonyReportTop: candidate for 1058:59f5 (98 bytes).  Verdict: RELOCATABLE.
 *
 * Title line of the colony report: swaps in palette byte 0xf9 and draws text string
 * SEG20:0xdd2e shadowed, at x = half the width port_width_1058_be8e reports at
 * y = +0x76c + 18, size 16.
 */

#include "colonization.h"

#include "misc.h"
#include "classes.h"
#include "w16types.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

extern char *strings(int n);


void ReportWin_ColonyReportTop(ReportWin *w)
{
    TextColor(0xf9);
    GRPort_String((GRPort *)w, (unsigned *)&g_report_font, strings(g_misc_words[MISC_COLONY_ADVISOR_REPORT]),
                         port_width_1058_be8e((GRPort *)(w)) / 2, w->rect.top + 0x12, 0x10);
}
