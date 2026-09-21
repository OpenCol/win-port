/* MiniWin::~MiniWin() -- shipped at 1008:e3c5.
 *
 * Ported by tools/port.py from matched/game/MiniWin_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class ColChild {
public:
    char body[0x77a - 2];
    ColChild();
    ~ColChild();
    virtual void v0();
};
class MenuClass {
public:
    char body[0x10];
    MenuClass();
    ~MenuClass();
};
class MiniWin : public ColChild {
public:
    col_i16 f77a;
    MenuClass menu;
    MiniWin();
    ~MiniWin();
    virtual void v0();
};

MiniWin::~MiniWin()
{
}
