/* ClosingWin::~ClosingWin() -- shipped at 1010:029f.
 *
 * Ported by tools/port.py from matched/game/ClosingWin_dtor.c.
 *
 * C++: the classes are as the file declares them, and the method is compiled
 * by Borland and sliced from the object by its demangled name. The host
 * harness does not judge these -- a constructor's work is member
 * construction and a vtable, which a host build does not reproduce.
 */
#define COL_CPP_PORT 1
#include "col/types.h"

class MagicWin {
public:
    char body[0x100 - 2];
    MagicWin();
    ~MagicWin();
    virtual void v0();
};
class Flic {
public:
    char body[0x678 - 0x100];
    Flic();
    ~Flic();
};
class EventWin {
public:
    char body[0x8c];
    EventWin();
    ~EventWin();
};
class ClosingWin : public MagicWin {
public:
    Flic flic;
    EventWin win;
    ClosingWin();
    ~ClosingWin();
    virtual void v0();
};

ClosingWin::~ClosingWin()
{
}
