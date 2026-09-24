/* text_out -- shipped at 1070:6426.
 *
 * Ported by tools/port.py from matched/platform/text_out.c.
 *
 * STRLEN -> strlen: the C runtime's own, taken from the real header rather than
 * from this file's far-pointer prototype.
 *
 * Dropped, because col/globals.h declares them and the addresses agree:
 *   extern void far * far g_font_2ace;                   DGROUP:0x2ace
 *
 * The file's own FontRec dropped for the merged record in col/records.h.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include <string.h>
#include "col/globals.h"

/* The callees whose in-file stub came out: declared here, defined in their own
 * translation units, joined by the linker. */
col_i16 get_w_0_1070_9806(void *ref);
col_i32 pascal control_message_handler(col_u16 hwnd, col_u16 msg, col_u16 wp,
                                               col_i32 lp);
void draw_ctl_border(col_u16 hwnd, col_u16 hdc, col_u32 ctl);
void render_1070_05a9(PAINTSTRUCT *ps, col_u16 hwnd);

/* Windows APIs. windows.h declares these when Borland builds this; the
 * host has no windows.h and still has to compile the unit. */
#ifdef COL_HOST
extern col_i16 pascal DrawText(col_u16 hdc, const char *s, col_i16 n, RECT *r,
                               col_u16 fmt);
extern col_i16 pascal EndPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_i16 pascal GetClientRect(col_u16 hwnd, RECT *r);
extern col_i16 pascal GetWindowText(col_u16 hwnd, char *buf, col_i16 max);
extern col_i16 pascal SetBkMode(col_u16 hdc, col_i16 mode);
extern col_i32 pascal GetWindowLong(col_u16 hwnd, col_i16 index);
extern col_u16 pascal BeginPaint(col_u16 hwnd, PAINTSTRUCT *ps);
extern col_u16 pascal SelectObject(col_u16 hdc, col_u16 hobj);
#endif

#include "w16const.h"

extern FontRec * MSHLock(col_i16 h);
extern void MSHUnlock(col_i16 h);
/* same object: 4-byte `push cs; call ` (rule 47) */

/* same object: 4-byte `push cs; call ` */




col_i32 pascal text_out(col_u16 hwnd, col_u16 msg, col_u16 wp, col_i32 lp)
{
    register col_u16 w;
    register col_u16 hdc;
    FontRec *f;
    RECT r;
    col_u16 saved;
    col_u32 ctl;
    PAINTSTRUCT ps;
    char buf[0x100];

    w = hwnd;
    ctl = GetWindowLong(w, 6);
    switch (msg) {
    case WM_PAINT:
        hdc = BeginPaint(w, &ps);
        GetClientRect(w, &r);
        render_1070_05a9(&ps, w);
        f = MSHLock(get_w_0_1070_9806(g_font_2ace));
        saved = SelectObject(hdc, f->hfont);
        MSHUnlock(get_w_0_1070_9806(g_font_2ace));
        SetBkMode(hdc, TRANSPARENT);
        GetWindowText(w, buf, 0xff);
        DrawText(hdc, buf, strlen(buf), &r, DT_VCENTER | DT_SINGLELINE);
        draw_ctl_border(w, hdc, ctl);
        SelectObject(hdc, saved);
        EndPaint(w, &ps);
        break;
    default:
        return control_message_handler(w, msg, wp, lp);
    }
    return 0;
}
