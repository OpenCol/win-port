/* popup_owner_get: candidate for 1050:0016 (21 bytes).  Verdict: RELOCATABLE.
 *
 * Returns the pointer at DGROUP:0x24fc. Two loads, `MOV DX,[0x24fe]` then
 * `MOV AX,[0x24fc]`, which is a pointer returned in DX:AX -- the high half first because
 * the low half must land in AX last.
 *
 * That word is `g_popup_owner`, which `SetPopupParent` at 1050:0000 writes and which is the
 * first function of segment 1050; this is the second. A setter and a getter back to back over
 * one pointer is the shape, and it is why the pointer's own name could be settled at all.
 */

#include "colonization.h"

#include "classes.h"

void *popup_owner_get(void)
{
    return g_popup_owner;
}
