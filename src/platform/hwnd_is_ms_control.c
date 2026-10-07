/* hwnd_is_ms_control: candidate for 1070:4f9e (114 bytes).  Verdict: RELOCATABLE.
 *
 * 1 when the window's class name is "MSCONTROLCLASS" and the object behind it has kind 3
 * (+0x1c).
 */

#include "colonization.h"

#include "records.h"

/* same object: 4-byte `push cs; call ` (rule 47) */

int hwnd_is_ms_control(unsigned hwnd)
{
    CtrlBlock *o;
    char name[50];

    GetClassName(hwnd, name, 50);
    if (STRCMP(STRUPR(name), "MSCONTROLCLASS") == 0) {
        o = object_from_hwnd(hwnd);
        if (o->kind == 3)
            return 1;
    }
    return 0;
}
