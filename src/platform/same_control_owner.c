/* same_control_owner: candidate for 1070:5090 (166 bytes).  Verdict: RELOCATABLE.
 * name-source: descriptive
 * size-source: 166, not the census 157. The trailing 9 bytes are the dead second epilogue.
 *
 * Whether two window handles belong to the same control: false unless both are radio-group
 * controls (MSCONTROLCLASS windows of kind 3, which is all `hwnd_is_ms_control` accepts), true if they are the same handle, and otherwise true only when the objects
 * behind them share the pointer at +2.
 *
 * FOUR SEPARATE EXITS, each with its own epilogue -- rule 44's arithmetic, and why a
 * 166-byte function does so little. Two of them return 1 and two return 0, and no two share
 * a tail, which is `-Od` again.
 *
 * The two objects are fetched before either pointer is read out of them, and the pointers are
 * compared as one 32-bit value (`MOV EAX / CMP EAX`), not as two halves -- a `` pointer
 * compared for equality under `-3`.
 *
 * The four locals are declared object-pointer-object-pointer, which is the order their frame
 * slots run in and not the order they are assigned: Borland gives the first-declared the
 * highest slot, and the shipped puts the first object at `[bp-6]`, its pointer at `[bp-0xa]`,
 * the second object at `[bp-0xe]` and its pointer at `[bp-0x12]`.
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` */

int same_control_owner(register unsigned other_hwnd, register unsigned hwnd)
{
    CtrlBlock *oa;
    void *pa;
    CtrlBlock *ob;
    void *pb;

    if (hwnd_is_ms_control(other_hwnd) == 0 || hwnd_is_ms_control(hwnd) == 0)
        return 0;
    if (other_hwnd == hwnd)
        return 1;
    oa = object_from_hwnd(other_hwnd);
    ob = object_from_hwnd(hwnd);
    pa = oa->ctl;
    pb = ob->ctl;
    if (pa == pb)
        return 1;
    return 0;
}
