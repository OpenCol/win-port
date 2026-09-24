/* on_labor_report_click -- shipped at 1058:3b14.
 *
 * Ported by tools/port.py from matched/game/on_labor_report_click.c.
 *
 * The file's own ReportWin dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
void dismiss_report_on_click(col_i16 x, col_i16 y);
void labor_report_colonist_detail(ReportWin *o, col_i16 x, col_i16 y);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern void pascal SetRect(RECT *r, col_i16 l, col_i16 t, col_i16 rt, col_i16 b);
#endif

extern char * current_window(void);

/* same object: 4-byte `push cs; call ` (rule 47) */


void on_labor_report_click(col_i16 x, col_i16 y)
{
    ReportWin *o;
    col_i16 hit;
    RECT r;
    char *w = current_window();
    col_i16 i;

    o = (ReportWin *)(w != 0 ? w - 0x26 : w);
    if (o == 0)
        return;
    hit = -1;
    for (i = 0; i < 0x1d; i++) {
        SetRect(&r, ((col_i16 *)((unsigned char *)(o) + 0x7fe))[i], ((col_i16 *)((unsigned char *)(o) + 0x838))[i] - 1, ((col_i16 *)((unsigned char *)(o) + 0x7fe))[i] + 0xce, ((col_i16 *)((unsigned char *)(o) + 0x838))[i] + 0x24);
        if (x > r.left && x < r.right && y > r.top && y < r.bottom)
            hit = i;
    }
    if (hit < 0) {
        dismiss_report_on_click(x, y);
        return;
    }
    labor_report_colonist_detail(o, x, y);
}
