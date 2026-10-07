/* europe_redraw_status: candidate for 1048:3c2b (28 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: europe_redraw_status
 * name-source: descriptive
 *
 * Redraws the Europe screen's status line: `EuropeWin_DrawTitle` on the Europe window
 * object at SEG20:0x2fd2, the same call `EuropeWin_EuropeKeyParse` and its neighbour make
 * after changing the treasury or the tax rate.
 *
 * No call to it and no pointer to it survives in the disassembly, so it is reached through
 * a pointer the census does not label, or not at all. The call is NOP-padded (rule 47), so the
 * status drawer is declared, not stubbed.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern void EuropeWin_DrawTitle(EuropeWin *w);

void europe_redraw_status(void)
{
    EuropeWin_DrawTitle(&g_europe_win);
}
