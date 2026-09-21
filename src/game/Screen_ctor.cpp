/* Screen::Screen() -- shipped at 1060:a41c.
 *
 * Ported by tools/port.py from matched/game/Screen_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class Screen {
public:
    col_i16 f0;
    col_i32 f2;
    col_i32 f6;
    col_i16 fa;
    char fc;
    char fd;
    Screen();
    ~Screen();
};

Screen::Screen()
{
    f6 = 0;
    f2 = 0;
    fa = 0;
    f0 = 0;
    fc = 0;
}
