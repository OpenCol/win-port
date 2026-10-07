/* EndLowMemReport: candidate for 1068:b85e (202 bytes).  Verdict: RELOCATABLE.
 * name-source: sibling-build
 * flags: -ml -WS -Od -Ot -3 -P
 * symbol: EndLowMemReport
 *
 * The Okay handler of the low-memory warning, installed by `PostLowMemReport` through
 * `set_button_action_1068_db5c` -- which is why nothing calls it and one OFFSET16 relocation inside
 * that function takes its address. It re-enables the window the warning covered, hides and
 * deletes the box and its button, pushes the deadline out by 0xe10 ticks (one minute: MSGetTicks
 * counts GetTickCount * 6 / 100, sixty ticks a second) and drops the in-progress guard, which is what lets
 * `LowMemTimerProc` fire again.
 *
 * The two `ADD DWORD SS:[0x10],n` are rule 49's headcount at a `delete`: 2 for the SButton,
 * whose only destructible sub-object is its DlgControl base, and 6 for the GameWin, which
 * has five (GRPort, DialogWin, and through DialogWin: EventWin, Screen, EventTable). Get
 * either hierarchy wrong and the immediate is wrong.
 *
 * The `+ 0x26` on the way into `MSHideWindow_of` is plain pointer arithmetic and NOT the
 * upcast to DialogWin: an upcast would carry the null test that `PostLowMemReport`'s
 * call to `ctl_init_button_1068_daf8` does carry. Here there is no test, so the source
 * casts to `char *` and adds.
 *
 * Every call is the four-byte `PUSH CS / CALL NEAR` with no NOP, so every callee is
 * defined in this translation unit and appears here as a stub (rule 47).
 *
 * RENAMED 2026-09-27 from low_memory_warn_close: BEYOND THE PROPOSALS: the low-memory block maps one to one onto Mac CODE 1 0x1b66..0x22da. Mac EndLowMemReport(short), CODE 1 0x1b66, 414 bytes. Its seq is MSEnableWindow, MSHideWindow, MSDoAllTasks, MSDisposeButton, RemoveControl, destroy control, delete x2, MSGetTicks. b85e, 202 bytes, calls win_enable, MSHideWindow, MSDoAllTasks, SButton_dtor, GameWin_dtor, MSGetTicks (the Windows button and window are C++ objects). Neither has a direct caller; it is the report's OK action. Layout: directly before PostLowMemReport on both. Arity: (void) against the (short) button id. C++ file (extern "C").
 */

#include "colonization.h"

#include "w16types.h"
#pragma option -a
#include "classes.h"
#pragma option -a-
class DlgControl;
extern GameWin *g_lowmem_box;          /* DGROUP:0x56ba */
extern SButton *g_lowmem_ok;           /* DGROUP:0x56be */

/* same object: 4-byte `push cs; call ` */
extern "C" void win_enable_1068_d96a(void *o);
extern "C" void MSHideWindow_of_1068_d995(void *o);

extern "C" void EndLowMemReport(void)
{
    win_enable_1068_d96a((DialogWin *)g_lowmem_win);
    MSHideWindow_of_1068_d995((DialogWin *)((char *)g_lowmem_box + 0x26));
    MSDoAllTasks_1068_d9b6();
    delete g_lowmem_ok;
    delete g_lowmem_box;
    g_lowmem_deadline = MSGetTicks_1068_d953() + 0xe10;
    g_lowmem_busy = 0;
}
