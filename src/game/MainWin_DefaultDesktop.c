/* MainWin_DefaultDesktop: candidate for 1048:ce37 (463 bytes).  Verdict: RELOCATABLE.
 * symbol: MainWin_DefaultDesktop
 * name-source: sibling-build
 *
 * Puts the four game windows back where they belong. The report window is closed, the
 * Colonopedia closed, the other three hidden, and then the client area is divided: a LEFT
 * column `cw - mw` wide and the full height, and a right-hand column `mw` wide carrying the
 * map above and the panel below.
 *
 * THE SPLIT DEPENDS ON THE DESKTOP, not on the window. When the map and the panel together are
 * taller than the client area, and the desktop is no bigger than 640 by 480, the map is
 * SHORTENED by the overflow -- `mw - (mw + mh - ch)` -- so the panel keeps its full height and
 * the pair fit exactly. A desktop bigger than 640 by 480 lets both keep their natural size and
 * run past the client area's bottom instead.
 *
 * The overflow expression is recomputed three times in the shipped bytes, once for each
 * coordinate that needs it, which is what a plain `mw - (mw + mh - ch)` written out three times
 * compiles to.
 *
 * A FIFTH RECTANGLE IS BUILT AND NEVER USED: `SetRect(&all, 0, 0, cw, ch)` at BP-0xe, the whole
 * client area, computed before the split and never passed to anything. In the shipped bytes.
 *
 * RENAMED 2026-09-26 from default_desktop: the Mac build's MainWin::DefaultDesktop(void): its one parameter is the unread `this`; it puts the main screen's windows back in their default layout. The width matches.
 */

#include "colonization.h"

#include "w16types.h"
#include "classes.h"

extern char g_pedia_win;              /* SEG20:0x387c */

extern MainWin g_main_win;             /* SEG20:0x5ca */
typedef struct { char pad[1]; } StatWin;
extern StatWin g_stat_win;           /* SEG20:0x1566 */
extern char g_mini_win;             /* SEG20:0xde0 */

extern void PediaWin_Done(PediaWin *w);

/* same object: 4-byte `push cs; call ` (rule 47) */
extern void MSHideWindow_of_1048_d4d5(Screen *o);
extern int win_client_width_1048_e585(Screen *o);
extern int win_client_height_1048_e5aa(Screen *o);

extern void win_move_to_rect(Screen *o, RECT *r);
extern void show_window_1048_dd42(void *o);

void MainWin_DefaultDesktop(MainWin *unused1)
{
    int ch;
    int mh;
    RECT all;
    RECT left;
    RECT top;
    RECT bottom;
    RECT desk;
    register int cw;
    register int mw;

    ReportWin_Done(&g_report_window);
    PediaWin_Done((PediaWin *)&g_pedia_win);
    MSHideWindow_of_1048_d4d5((Screen *)&g_info_win_dlg);
    MSHideWindow_of_1048_d4d5((Screen *)&g_mini_win_dlg);
    MSHideWindow_of_1048_d4d5((Screen *)&g_map_window_dlg);
    cw = win_client_width_1048_e585((Screen *)&g_main_win);
    ch = win_client_height_1048_e5aa((Screen *)&g_main_win);
    mw = port_width_1048_d4f6((GRPort *)&g_stat_win);
    mh = port_height_1048_d7c5((GRPort *)&g_stat_win);
    SetRect(&all, 0, 0, cw, ch);
    SetRect(&left, 0, 0, cw - mw, ch);
    MSGetDesktopSize(&desk);
    if (mw + mh > ch
            && (rect_width_1048_d69b(&desk) <= 0x280
                || rect_height_1048_d6ba(&desk) <= 0x1e0)) {
        SetRect(&top, cw - mw, 0, cw, mw - (mw + mh - ch));
        SetRect(&bottom, cw - mw, mw - (mw + mh - ch), cw,
                mw - (mw + mh - ch) + mh);
    } else {
        SetRect(&top, cw - mw, 0, cw, mw);
        SetRect(&bottom, cw - mw, mw, cw, mw + mh);
    }
    win_move_to_rect((Screen *)&g_map_window_dlg, &left);
    win_move_to_rect((Screen *)&g_mini_win_dlg, &top);
    win_move_to_rect((Screen *)&g_info_win_dlg, &bottom);
    show_window_1048_dd42((GameWin *)&g_stat_win);
    show_window_1048_dd42((GameWin *)&g_mini_win);
    show_window_1048_dd42((GameWin *)&g_map_window);
}
