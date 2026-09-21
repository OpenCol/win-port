/* dispose_handle_ptr -- shipped at 1078:3316.
 *
 * Ported by tools/port.py from matched/platform/dispose_handle_ptr.c.
 *
 * far/near dropped throughout: an addressing mode with no semantic here.
 */
#include "col/globals.h"

extern col_i32 MSHUnlock(col_i16 h);
extern col_i16 MSDisposeHandle(col_i16 h);

void dispose_handle_ptr(col_i16 *p)
{
    register col_i16 h;

    if (p != 0) {
        h = *p;
        if (h != 0) {
            MSHUnlock(h);
            h = MSDisposeHandle(h);
        }
    }
}
