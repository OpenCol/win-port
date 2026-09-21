/* CursorClass::CursorClass() -- shipped at 1008:f592.
 *
 * Ported by tools/port.py from matched/game/CursorClass_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class CursorClass {
public:
    col_i16 hcursor;
    col_i16 f2;
    CursorClass();
    ~CursorClass();
};

CursorClass::CursorClass()
{
    f2 = -1;
    hcursor = 0;
}
