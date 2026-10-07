/* ReportWin_KeyClick: candidate for 1058:0b13 (112 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: ReportWin_KeyClick
 *
 * Key 0xd2 closes the report; any other key goes to the accelerator handler unless the
 * report's byte +0x7f8 is set.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"
class SButton;
extern void *GetEventWindow(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void KeyEvents(int key);

void ReportWin_KeyClick(int key)
{
    ReportWin *o;
    EventWin *w = (EventWin *)GetEventWindow();

    o = (ReportWin *)w;

    switch (key) {
    case 0xd2:
        ReportWin_Done(o);
        break;
    default:
        if (o->modal == 0)
            KeyEvents(key);
        break;
    }
}
