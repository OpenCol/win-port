/* StatWin::~StatWin() -- shipped at 1008:e366.
 *
 * Ported by tools/port.py from matched/game/StatWin_dtor.c.
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
class GRPort {
public:
    char body[0x26];
    GRPort();
    ~GRPort();
};
class StatWin : public ColChild {
public:
    GRPort port;
    StatWin();
    ~StatWin();
    virtual void v0();
};

StatWin::~StatWin()
{
}
