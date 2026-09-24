/* ReportWin_KeyClick -- shipped at 1058:0b13.
 *
 * Ported by tools/port.py from matched/game/ReportWin_KeyClick.c.
 *
 * The file's own ReportWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void close_report_window(void *o);

extern char * current_window(void);
/* same object: 4-byte `push cs; call ` (rule 47) */

extern void keyboard_accelerator_handler(col_i16 key);

void ReportWin_KeyClick(col_i16 key)
{
    ReportWin *o;
    char *w = current_window();

    o = (ReportWin *)(w != 0 ? w - 0x26 : w);

    switch (key) {
    case 0xd2:
        close_report_window(o);
        break;
    default:
        if (o->no_accel == 0)
            keyboard_accelerator_handler(key);
        break;
    }
}
