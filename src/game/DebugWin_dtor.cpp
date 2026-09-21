/* DebugWin::~DebugWin() -- shipped at 1008:d3eb.
 *
 * Ported by tools/port.py from matched/game/DebugWin_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class GameWin {
public:
    char body[0xc4 - 2];
    GameWin();
    ~GameWin();
    virtual void v0();
};
class DebugWin : public GameWin {
public:
    DebugWin();
    ~DebugWin();
    virtual void v0();
};

DebugWin::~DebugWin()
{
}
