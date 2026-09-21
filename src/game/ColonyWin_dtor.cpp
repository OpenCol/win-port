/* ColonyWin::~ColonyWin() -- shipped at 1008:e0ef.
 *
 * Ported by tools/port.py from matched/game/ColonyWin_dtor.c.
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
class SRegion { public: char body[0x2e]; SRegion(); ~SRegion(); };
class GRPort { public: char body[0x26]; GRPort(); ~GRPort(); };
class SImageButton { public: char body[0x74]; SImageButton(); ~SImageButton(); };
class SButton { public: char body[0x22]; SButton(); ~SButton(); };
class ColonyWin : public ColChild {
public:
    SRegion region;
    GRPort p1, p2;
    SImageButton ib[3];
    SButton b[2];
    SImageButton ib2;
    ColonyWin();
    ~ColonyWin();
    virtual void v0();
};

ColonyWin::~ColonyWin()
{
}
