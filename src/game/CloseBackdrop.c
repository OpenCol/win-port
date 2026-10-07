/* CloseBackdrop: candidate for 1008:4b58 (123 bytes).  Verdict: RELOCATABLE.
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: CloseBackdrop
 * name-source: sibling-build
 *
 * Deletes the desktop-sized EventWin at DGROUP:0xc6 that `OpenBackdrop` builds -- 0x8c
 * bytes, sized from `MSGetDesktopSize`, style 0x801, an empty name -- then points the long
 * at DGROUP:0x24fc back at g_main_win (`SetPopupParent`) and calls 1070:dad8, which is an empty function: fourteen bytes of
 * prologue and epilogue with nothing between them. `EuropeWin_Close` calls that same stub.
 *
 * THE THREE IS A HEADCOUNT (rule 49). `ADD DWORD SS:[0x10],n` at a delete site is `1 + the
 * number of destructible sub-objects of the class`, and EventWin has exactly two, `Screen`
 * and `EventTable`. The same site's opener at 1008:4a32 subtracts 3 after constructing it,
 * and `europe_remove_label` adds 6 for a GameWin, which has five. The immediate is a witness
 * to the class hierarchy independent of the constructor that declares it.
 *
 * The clear of the global and the two calls are all inside the `if`, unlike the popup
 * closers, where the clear is unconditional: here the null test jumps past the whole body.
 *
 * `EventWin::~EventWin()` is DEFINED here and not merely declared: rule 47 covers the calls
 * the compiler writes, so the `delete` reaches the destructor through the four-byte `PUSH CS /
 * CALL NEAR` the shipped has, and no jump displacement has to be forgiven.
 *
 * RENAMED 2026-09-26 from close_overlay_window: the Mac build's CloseBackdrop(void), bound by layout (between OpenBackdrop and IsBackdrop on both sides); the arity agrees and the Mac body is 1.14x this one.
 */

#include "colonization.h"

#pragma option -a
/* PORT: this file's own Screen, EventTable, EventWin removed: include/gen/classes.h define them once (W1 step 2). */
#include "classes.h"
#pragma option -a-


/* Defined, not merely declared: the shipped `delete` calls the destructor through the
 * four-byte `PUSH CS / CALL NEAR`, which the compiler emits only for a callee in this
 * translation unit (rule 47). */

extern EventWin *g_backdrop_win;         /* DGROUP:0xc6 */
typedef struct { char pad[1]; } MainWin;
extern MainWin g_main_win;        /* SEG20:0x5ca */
extern void SetPopupParent(void *parent);
extern void MSShowMenuBar(void);

extern "C" void CloseBackdrop(void)
{
    if (g_backdrop_win != 0) {
        delete g_backdrop_win;
        g_backdrop_win = 0;
        SetPopupParent(&g_main_win);
        MSShowMenuBar();
    }
}
