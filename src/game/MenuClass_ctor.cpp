/* MenuClass::MenuClass() -- shipped at 1060:8f1a.
 *
 * Ported by tools/port.py from matched/game/MenuClass_ctor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class MenuClass {
public:
    col_i16 hmenu;
    MenuClass();
    ~MenuClass();
};

MenuClass::MenuClass()
{
    hmenu = 0;
}
