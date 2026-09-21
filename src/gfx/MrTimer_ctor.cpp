/* MrTimer::MrTimer() -- shipped at 1068:91c8.
 *
 * Ported by tools/port.py from matched/gfx/MrTimer_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class MrTimer {
public:
    col_i16 hwnd; /* +0 */
    void *slots[17]; /* +2 */
    col_i16 args[17]; /* +0x46 */
    col_i16 tail; /* +0x68 */
    MrTimer();
    ~MrTimer();
};
extern "C" col_i16 create_timer_daemon_window(MrTimer *t);

MrTimer::MrTimer()
{
    register col_i16 i;

    for (i = 1; i <= 16; i++) {
        slots[i] = 0;
        args[i] = 0;
    }
    hwnd = create_timer_daemon_window(this);
}
