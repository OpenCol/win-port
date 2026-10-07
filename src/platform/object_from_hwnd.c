/* object_from_hwnd: candidate for 1070:09ae (40 bytes).  Verdict: RELOCATABLE.
 *
 * The object pointer stored in window long 0 of hwnd; 0 for a null handle.
 */

#include "colonization.h"

#include "records.h"
CtrlBlock *object_from_hwnd(unsigned hwnd)
{
    if (hwnd != 0)
        return (CtrlBlock *)GetWindowLong(hwnd, 0);
    else
        return 0;
}
