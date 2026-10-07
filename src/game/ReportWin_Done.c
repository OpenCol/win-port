/* PORT: this file's own declarations of DialogWin_RemoveControl, EventWin_EndModal removed: the header declares them from the definition (W1 step 3). */
/* ReportWin_Done: candidate for 1058:057e (338 bytes, 9 callers).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 *
 * Closes a report window: ends the modal session on its port (+0x26) through `EventWin_EndModal` when the flag at +0x7f8 is set, marks +0x77a,
 * runs the close hook otherwise, and deletes the three SButtons at +0x8ac, +0x8b0, +0x8b4.
 * Each `delete` is Borland's: a copy to a local, the unwinder counter bumped by two, the
 * destructor with flag 3, inside an exception frame.
 *
 * THE DESTRUCTOR IS DEFINED HERE AND NOT MERELY DECLARED, and so is `win_release_1058_bca9`.
 * Rule 47 applies to the compiler's own calls too: a `delete` on a class whose destructor this
 * translation unit defines compiles to the four-byte `PUSH CS / CALL NEAR`, which is what the
 * shipped has. Declared alone it is a five-byte call, and the comparator then has to
 * collapse three of them and forgive three jump displacements as one out -- a green verdict
 * that was hiding a real difference in the source.
 *
 * RENAMED 2026-09-27 from close_report_window: CHAIN 2 step 2 (after 0bde vacates ReportWin_Done). Mac ReportWin::Done(void), 532 bytes, against 338. Callers: Mac Done has 9 callers: Close, MainWin::DefaultDesktop, KeyClick, LeftMouseClick, OnOKClick, OnTradeRouteExtraClick, ReportsForeign, MainWin::ResetWindows and load_game(void). close_report_window has exactly those 9: ReportWin_Close, MainWin_DefaultDesktop, ReportWin_KeyClick, 0c36, 0bde, ReportWin_OnTradeRouteExtraClick, ReportWin_ReportsForeign, MainWin_ResetWindows, load_game. Callees: EventWin::EndModal, MSDisposeButton, MSDestroyControl, RemoveControl and MSCloseWindow on Mac; EventWin_EndModal, SButton_dtor_1058_bbb9 and win_release here. Layout: Mac DoReport(short, char, EventWin*), Done, DrawReport; Windows ReportWin_DoReport_parent, 057e, ReportWin_DrawReport. Arity: this = (ReportWin *w).
 */

#include "colonization.h"


#include "w16types.h"
#include "classes.h"
class SButton;

/* Defined here, not just declared: the shipped calls the destructor through the four-byte
 * `PUSH CS / CALL NEAR`, which the compiler emits only for a callee in this translation unit.
 * Declared alone, the `delete` compiles to a five-byte call and the comparator has to
 * collapse it and forgive three jump displacements as one out. */

/* same object: 4-byte `push cs; call ` */
extern "C" void win_release_1058_bca9(GameWin *w);

extern "C" void ReportWin_Done(ReportWin *w)
{
    if (w->modal != 0)
        EventWin_EndModal(&(EventWin &)*w);
    w->kind = -1;
    if (w->modal == 0)
        win_release_1058_bca9(w);
    if (w->prev != 0) {
        delete w->prev;
        w->prev = 0;
    }
    if (w->next != 0) {
        delete w->next;
        w->next = 0;
    }
    if (w->extra_btn != 0) {
        delete w->extra_btn;
        w->extra_btn = 0;
    }
}
